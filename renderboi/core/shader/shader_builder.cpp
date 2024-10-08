
#include <algorithm>
#include <ctime>
#include <filesystem>
#include <fstream>
#include <functional>
#include <iomanip>
#include <iostream>
#include <iterator>
#include <numeric>
#include <ranges>
#include <sstream>

#include <glad/gl.h>

#include <renderboi/utilities/resource_locator.hpp>
#include <renderboi/core/shader/shader_feature.hpp>
#include <renderboi/core/shader/shader_stage.hpp>
#include <renderboi/core/shader/shader_builder.hpp>

#include <cpptools/utility/string.hpp>

#define INFO_BUFFER_SIZE 2048

namespace rb {

using ReLoc = ResourceLocator;
using ReType = ResourceType;

std::unordered_map<std::string, std::string> ShaderBuilder::_includeStrings = std::unordered_map<std::string, std::string>();

ShaderProgram ShaderBuilder::MinimalShaderProgram() {
    static ShaderProgram Minimal = BuildShaderProgramFromConfig(ShaderConfig::MinimalConfig());
    return Minimal;
}

ShaderProgram ShaderBuilder::BuildShaderProgramFromConfig(const ShaderConfig& config, const bool dumpSource) {
    const std::vector<ShaderFeature>& Features = config.getRequestedFeatures();
    std::unordered_set<ShaderStage> requestedStages;

    // Find out which shader stage were requested in the features
    for (const auto& feature : Features) {
        auto jt = FeatureStages().find(feature);
        if (jt == FeatureStages().end()) {
            const std::string s =
                "ShaderBuilder: cannot build shader program from config, feature "
                "\"" + to_string(feature) + "\" (" + std::to_string((unsigned int) feature) + ") "
                "from unknown stage was requested.";

            throw std::runtime_error(s.c_str());
        }

        const ShaderStage stage = jt->second;
        requestedStages.insert(stage);
    }

    std::vector<Shader> shaders;
    // Generate all shader stages
    for (const auto& stage : requestedStages) {
        const Shader shader = BuildShaderStageFromConfig(stage, config, dumpSource);
        shaders.push_back(shader);
    }

    // Link them together and return
    return LinkShaders(shaders);
}

ShaderProgram ShaderBuilder::LinkShaders(const std::vector<Shader>& shaders) {
    std::unordered_set<ShaderStage> presentStages;
    std::vector<unsigned int> locations;

    // Check if there is no more than one shader per stage
    for (const auto& shader : shaders) {
        ShaderStage stage = shader.stage();
        auto jt = presentStages.find(stage);
        if (jt == presentStages.end()) {
            presentStages.insert(stage);
            locations.push_back(shader.location());
        } else {
            const std::string s = "ShaderBuilder: cannot link shaders, several objects were provided for stage \""
                + to_string(stage) + "\".";

            throw std::runtime_error(s.c_str());
        }        
    }

    // Check if there is at least a vertex shader and a fragment shader
    if (presentStages.find(ShaderStage::Vertex) == presentStages.end()) {
        throw std::runtime_error("ShaderBuilder: cannot link shaders, a vertex shader is required but none was provided.");
    }
    if (presentStages.find(ShaderStage::Fragment) == presentStages.end()) {
        throw std::runtime_error("ShaderBuilder: cannot link shaders, a fragment shader is required but none was provided.");
    }

    // Do the linking
    const unsigned int programLocation = _MakeShaderProgram(locations);
    if (!programLocation) {
        throw std::runtime_error("ShaderBuilder: the provided shaders could not be linked together. See std::cerr.");
    }

    // Aggregate supported features
    const std::vector<ShaderFeature> supportedFeatures = _AggregateShaderFeatures(shaders);

    // Return the shader instance
    return ShaderProgram(programLocation, supportedFeatures);
}

Shader ShaderBuilder::BuildShaderStageFromConfig(const ShaderStage stage, const ShaderConfig& config, const bool dumpSource) {
    const std::vector<ShaderFeature> requestedFeatures = _FilterFeaturesByStage(config.getRequestedFeatures(), stage);

    auto it = _StageTemplatePaths().find(stage);
    if (it == _StageTemplatePaths().end()) {
        const std::string s = "ShaderBuilder: cannot find template path for stage \"" + to_string(stage) + "\".";
        throw std::runtime_error(s.c_str());
    }

	// Open template 
	std::ifstream file(it->second);

	if (!file.is_open()) {
		const std::string s = "ShaderBuilder: Shader template \"" + it->second + "\" could not be found.";
        throw std::runtime_error(s.c_str());
    }

    // Add directives for version and extension usage, and define macros for
    // requested features
	std::string source = _GenerateVersionDirective()
        + _GenerateExtensionDirectives()
        + _GenerateDefineDirectives(requestedFeatures);

    // Add all file contents
	std::string line;
	while (std::getline(file, line)) {
		source += line + "\n";
    }

    return BuildShaderStageFromText(stage, source, requestedFeatures, dumpSource);
}

Shader ShaderBuilder::BuildShaderStageFromFile(
    const ShaderStage stage,
    const std::string& filename,
    const std::vector<ShaderFeature>& supportedFeatures
) {
	// Open input file 
	std::ifstream file(filename);

	if (!file.is_open()) {
		const std::string s = "ShaderBuilder: Shader source \"" + filename + "\" could not be found.";
        throw std::runtime_error(s.c_str());
    }

    // Read all of its contents
	std::string source("");
	std::string line;
	while (std::getline(file, line)){
        source += line + "\n";
    }

    // If no info was provided about supported features, try to find some
    if (!supportedFeatures.size()) {
        auto it = _FeaturesSupportedByFile().find(filename);
        if (it != _FeaturesSupportedByFile().end()) {
            return BuildShaderStageFromText(stage, source, it->second);
        }
    }
    // else
    return BuildShaderStageFromText(stage, source, supportedFeatures);
}

Shader ShaderBuilder::BuildShaderStageFromText(
    const ShaderStage stage,
    std::string text,
    const std::vector<ShaderFeature>& supportedFeatures,
    const bool dumpSource)
{
    // In case the shader makes use of #include directives, process them
    tools::strip_c_comments(text);
    _ProcessIncludeDirectives(text);

    // Get GL macro corresponding to requested shader stage
    auto it = _ShaderStageMacros().find(stage);
    if (it == _ShaderStageMacros().end()) {
        const std::string s =
            "ShaderBuilder: Unknown requested shader stage "
            "\"" + to_string(stage) + "\" (" + std::to_string((unsigned int) stage) + ").";

        throw std::runtime_error(s.c_str());
    }
    const unsigned int shaderType = _ShaderStageMacros().at(stage);

	// Compile into shader
	const char* source = text.c_str();
	const unsigned int location = glCreateShader(shaderType);
    if (!location) {
        throw std::runtime_error("ShaderBuilder: Shader resource allocation failed.");
    }

	glShaderSource(location, 1, &source, nullptr);
	glCompileShader(location);

	// Print errors if any
	int success;
	char info[INFO_BUFFER_SIZE];
	glGetShaderiv(location, GL_COMPILE_STATUS, &success);
	if (!success) {
		glGetShaderInfoLog(location, INFO_BUFFER_SIZE, nullptr, info);
        const std::string filename = _DumpShaderSource(stage, text);
		std::cerr << "Shader compilation failed:\n" << info << '\n';
        std::cerr << "Source was dumped to " << filename << std::endl;

		throw std::runtime_error("ShaderBuilder: Shader compilation failed. See std::cerr for more info.");
    }

    if (dumpSource) {
        const std::string filename = _DumpShaderSource(stage, text);
        std::cout << "Source was dumped to " << filename << std::endl;
	}

	return Shader(location, stage, supportedFeatures);
}

unsigned int
ShaderBuilder::_MakeShaderProgram(const std::vector<unsigned int>& locations) {
	const unsigned int program = glCreateProgram();

	for (const auto& loc : locations) {
		glAttachShader(program, loc);
	}

	// Link all shaders
	glLinkProgram(program);

	// Print errors if any
	int success;
	char info[INFO_BUFFER_SIZE];
	glGetProgramiv(program, GL_LINK_STATUS, &success);
	if (!success) {
		glGetProgramInfoLog(program, INFO_BUFFER_SIZE, NULL, info);
		std::cerr << "Shader linking failed:\n" << info << std::endl;
        
        glDeleteProgram(program);
		return 0;
	}

	return program;
}

void ShaderBuilder::_ProcessIncludeDirectives(std::string& text) {
    auto includeArguments = _LocateIncludeDirectivesInSource(text); // [{ includeString, { directiveStart, directiveCharCount } }]
    
    while (includeArguments.size() > 0) {
        // Process arguments in reverse, in order to preserve lower positions in the
        // string as substrings are progressively being replaced.
        for (auto& [includeArgument, location] : includeArguments | std::views::reverse) {
            auto includeString = _GetIncludeString(includeArgument);
            text.replace(location.first, location.second, std::move(includeString));
        }

        includeArguments = _LocateIncludeDirectivesInSource(text);
    }
}

std::string ShaderBuilder::_GetIncludeString(const std::string& arg) {
    // Find the string 
    auto it = _includeStrings.find(arg);

    // If not present, try to find info
    if (it == _includeStrings.end()) {
        auto jt = _IncludeFilenames().find(arg);
        if (jt == _IncludeFilenames().end()) {
                const std::string s = "ShaderBuilder: Info about include directive "
                "<" + arg + "> cannot be found.";

            throw std::runtime_error(s.c_str());
        }

        // Read file contents and cache it
        _includeStrings[arg] = tools::from_file(jt->second);
        tools::strip_c_comments(_includeStrings[arg]);
    }

    return _includeStrings[arg];
}

std::vector<std::pair<std::string, std::pair<size_t, size_t>>>
ShaderBuilder::_LocateIncludeDirectivesInSource(std::string& text) {
    static const std::string_view IncludeStringStart = "#include";
    const auto npos = std::string::npos;
    std::vector<std::pair<std::string, std::pair<size_t, size_t>>> includeArguments;

    size_t offset = text.find(IncludeStringStart, 0);
    while (offset != npos) {
        const size_t startPos = offset;
        size_t lastEol = text.find_last_of('\n', offset);
        if (lastEol == npos) {
            lastEol = 0;
        }

        const std::string before = text.substr(lastEol, offset - lastEol);
        if (!tools::is_whitespace(before)) {
            std::cout <<
                "ShaderBuilder: ignored include directive as non whitespace characters are "
                "present on the same line before the start of the directive." << std::endl;
    
            offset = text.find(IncludeStringStart, offset);
            continue;
        }

        offset += IncludeStringStart.size();
        size_t eol = text.find('\n', offset);
        if (eol == npos) {
            eol = text.size();
        }

        const size_t argLength = eol - offset;

        std::string includeArgument = text.substr(offset, argLength);
        tools::trim(includeArgument);

        const char firstDelimiter = includeArgument[0];
        char secondDelimiter = 0;
        switch (firstDelimiter) {
            case '<': secondDelimiter = '>'; break;
            case '"': secondDelimiter = '"'; break;
        }

        if (secondDelimiter == 0 || secondDelimiter != includeArgument.back()) {
            const std::string s = "Badly formatted #include directive: argument \"" + includeArgument + "\" is illegal.";
            throw std::runtime_error(s.c_str());
        }

        // Strip delimiting chars
        includeArgument = includeArgument.substr(1, includeArgument.size() - 2);

        if ((includeArgument.find(secondDelimiter) != npos) ||
            (includeArgument.find(firstDelimiter) != npos))
        {
            const std::string s = "Badly formatted #include directive: argument \"" + includeArgument + "\" is illegal.";
            throw std::runtime_error(s.c_str());
        }
        includeArguments.push_back({
            includeArgument,
            {startPos, IncludeStringStart.size() + argLength}
        });

        offset = text.find(IncludeStringStart, offset);
    }

    return includeArguments;
}

std::vector<ShaderFeature> ShaderBuilder::_AggregateShaderFeatures(const std::vector<Shader>& shaders) {
    std::vector<ShaderFeature> supportedFeatures;
    for (const auto& shader : shaders) {
        const std::vector<ShaderFeature>& features = shader.getSupportedFeatures();
        std::copy(features.begin(), features.end(), std::back_inserter(supportedFeatures));
    }

    return supportedFeatures;
}

std::vector<ShaderFeature> ShaderBuilder::_FilterFeaturesByStage(
    const std::vector<ShaderFeature>& features,
    const ShaderStage stage
)
{
    std::vector<ShaderFeature> result;

    std::function<bool(ShaderFeature)> filter = [stage](ShaderFeature v) -> bool {
        auto it = FeatureStages().find(v);
        if (it == FeatureStages().end()) return false;
        return it->second == stage;
    };

    std::copy_if(features.begin(), features.end(), std::inserter(result, result.begin()), filter);
    return result;
}

std::string ShaderBuilder::_DumpShaderSource(ShaderStage stage, const std::string& text) {
    static const std::string DumpFolder = "output/";
    std::filesystem::create_directories(DumpFolder);

    std::time_t t = std::time(nullptr);
    std::tm tm = {};
#ifdef _WIN32
    ::localtime_s(&tm, &t);
#else
    localtime_r(&t, &tm);
#endif

    std::stringstream ss;
    ss << std::put_time(&tm, "%d-%m-%Y-%H-%M-%S");

    const std::unordered_map<ShaderStage, std::string>& fileExtensions = _StageFileExtensions();
    const std::string filename = DumpFolder + to_string(stage) + "_shader_dump_" + ss.str() + '.' + fileExtensions.at(stage);

    std::ofstream file;
    file.open(filename, std::ios::out);
    file << text;
    file.close();

    return std::filesystem::absolute(filename).string();
}

const std::string& ShaderBuilder::_GenerateVersionDirective() {
    static const std::string s = "#version " + std::to_string(ShadingLanguageVersion) + " " + ShadingLanguageProfile + "\n";
    return s;
}

const std::string& ShaderBuilder::_GenerateExtensionDirectives() {
    static auto addDirective = [](std::string aggregate, std::pair<std::string, std::string> tuple) -> std::string
    {
        return aggregate + "#extension " + tuple.first + " : " + tuple.second + '\n';
    };
    static const std::unordered_map<std::string, std::string>& extensions = _ShadingLanguageExtensions(); 
    
    static std::string s = std::accumulate(
        extensions.cbegin(), extensions.cend(),
        (std::string)(""), addDirective
    );

    return s;
}

const std::unordered_map<std::string, std::string>& ShaderBuilder::_ShadingLanguageExtensions() {
    static std::unordered_map<std::string, std::string> map = {};

    return map;
}

const std::unordered_map<ShaderStage, std::string>& ShaderBuilder::_StageTemplatePaths() {
    static std::unordered_map<ShaderStage, std::string> map = {
        {ShaderStage::Vertex,   ReLoc::locate(ReType::ShaderSource, "templates/vertex_shader.vert")},
        {ShaderStage::Geometry, ReLoc::locate(ReType::ShaderSource, "templates/geometry_shader.geom")},
        {ShaderStage::Fragment, ReLoc::locate(ReType::ShaderSource, "templates/fragment_shader.frag")},
    };

    return map;
}

const std::unordered_map<ShaderFeature, std::string>& ShaderBuilder::_FeatureDefineMacros() {
    static std::unordered_map<ShaderFeature, std::string> map = {
        {ShaderFeature::VertexMVP,                      "VERTEX_MVP"},
        // {ShaderFeature::VertexFishEye,                  "VERTEX_FISH_EYE"},        // IMPLEMENT VERT LENS
        // {ShaderFeature::GeometryShowNormals,            "GEOMETRY_SHOW_NORMALS"},  // IMPLEMENT GEOM NORMALS
        {ShaderFeature::FragmentFullLight,              "FRAGMENT_FULL_LIGHT"},
        {ShaderFeature::FragmentViewDepthBuffer,        "FRAGMENT_VIEW_DEPTH_BUFFER"},
        {ShaderFeature::FragmentViewLightAttenuation,   "FRAGMENT_VIEW_LIGHT_ATTENUATION"},
        {ShaderFeature::FragmentMeshMaterial,           "FRAGMENT_MESH_MATERIAL"},
        {ShaderFeature::FragmentBypassVertexColor,      "FRAGMENT_BYPASS_VERTEX_COLOR"},
        // {ShaderFeature::FragmentFlatShading,            "FRAGMENT_FLAT_SHADING"},  // IMPLEMENT FRAG FLAT
        {ShaderFeature::FragmentPhong,                  "FRAGMENT_PHONG"},
        {ShaderFeature::FragmentBlinnPhong,             "FRAGMENT_BLINN_PHONG"},
        {ShaderFeature::FragmentGammaCorrection,        "FRAGMENT_GAMMA_CORRECTION"},
        // {ShaderFeature::FragmentOutline,                "FRAGMENT_OUTLINE"},       // IMPLEMENT FRAG OUTLINE
        // {ShaderFeature::FragmentCubemap,                "FRAGMENT_CUBEMAP"},       // IMPLEMENT FRAG CUBEMAP
        // {ShaderFeature::FragmentBlending,               "FRAGMENT_BLENDING"},      // IMPLEMENT FRAG BLENDING
        // {ShaderFeature::FragmentShadows,                "FRAGMENT_SHADOWS"},       // IMPLEMENT FRAG SHADOWS
    };

    return map;
}

const std::unordered_map<std::string, std::string>& ShaderBuilder::_IncludeFilenames() {
    static std::unordered_map<std::string, std::string> map = {
        { "/functional_blocks/gamma_correction",     ReLoc::locate(ReType::ShaderSource, "functional_blocks/gamma_correction.glsl")  },
        { "/functional_blocks/light_attenuation",    ReLoc::locate(ReType::ShaderSource, "functional_blocks/light_attenuation.glsl") },
        { "/interface_blocks/light_types",           ReLoc::locate(ReType::ShaderSource, "interface_blocks/light_types.glsl")        },
        { "/interface_blocks/vertex_attributes",     ReLoc::locate(ReType::ShaderSource, "interface_blocks/vertex_attributes.glsl")  },
        { "/interface_blocks/vertex_out",            ReLoc::locate(ReType::ShaderSource, "interface_blocks/vertex_out.glsl")         },
        { "/templates/phong",                        ReLoc::locate(ReType::ShaderSource, "templates/phong.glsl")                     },
        { "/uniform_blocks/lights",                  ReLoc::locate(ReType::ShaderSource, "uniform_blocks/lights.glsl")               },
        { "/uniform_blocks/material",                ReLoc::locate(ReType::ShaderSource, "uniform_blocks/material.glsl")             },
        { "/uniform_blocks/matrices",                ReLoc::locate(ReType::ShaderSource, "uniform_blocks/matrices.glsl")             },
    };

    return map;
}

const std::unordered_map<std::string, std::vector<ShaderFeature>>& ShaderBuilder::_FeaturesSupportedByFile() {
    using enum ShaderFeature;
    static std::unordered_map<std::string, std::vector<ShaderFeature>> map = {
        {
            ReLoc::locate(ReType::ShaderSource, "static/default.frag"), { FragmentFullLight }
        },
        {
            ReLoc::locate(ReType::ShaderSource, "static/depth.frag"),   { FragmentViewDepthBuffer }
        },
        {
            ReLoc::locate(ReType::ShaderSource, "static/mvp.vert"),     { VertexMVP }
        },
        {
            ReLoc::locate(ReType::ShaderSource, "static/phong.frag"),   { FragmentMeshMaterial, FragmentBlinnPhong, FragmentGammaCorrection }
        },
    };

    return map;
}

const std::unordered_map<ShaderStage, unsigned int>& ShaderBuilder::_ShaderStageMacros() {
    static std::unordered_map<ShaderStage, unsigned int> map = {
        { ShaderStage::Vertex,   GL_VERTEX_SHADER   },
        { ShaderStage::Geometry, GL_GEOMETRY_SHADER },
        { ShaderStage::Fragment, GL_FRAGMENT_SHADER },
    };

    return map;
}

const std::unordered_map<ShaderStage, std::string>& ShaderBuilder::_StageFileExtensions() {
    static std::unordered_map<ShaderStage, std::string> map = {
        { ShaderStage::Vertex,   "vert" },
        { ShaderStage::Geometry, "geom" },
        { ShaderStage::Fragment, "frag" },
    };

    return map;
}

std::string ShaderBuilder::_GenerateDefineDirectives(const std::vector<ShaderFeature>& features) {
    static const std::unordered_map<ShaderFeature, std::string>&
    defineMacros = _FeatureDefineMacros();

    std::function<std::string(std::string, ShaderFeature)>
    addDirective = [](std::string aggregate, ShaderFeature feature) -> std::string {
        auto it = defineMacros.find(feature);
        if (it == defineMacros.cend()) {
            std::string s = "ShaderBuilder: cannot generate #define directive "
                "for feature \"" + to_string(feature) + "\" "
                "(" + std::to_string((unsigned int)(feature)) + ").";

            throw std::runtime_error(s.c_str());
        }
        return aggregate + "#define " + it->second + "\n";
    };

    return std::accumulate(
        features.cbegin(),
        features.cend(),
        (std::string)(""),
        addDirective
    );
}

} // namespace rb

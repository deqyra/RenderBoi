#include "shader_builder.hpp"

#include <algorithm>
#include <cstdarg>
#include <ctime>
#include <filesystem>
#include <fstream>
#include <functional>
#include <iomanip>
#include <iterator>
#include <numeric>
#include <sstream>

#include "shader_feature.hpp"
#include "shader_stage.hpp"

#include <glad/gl.h>

#include <renderboi/utilities/to_string.hpp>

#include <cpptools/string_tools.hpp>

#define INFO_BUFFER_SIZE 2048

namespace Renderboi
{

std::unordered_map<std::string, bool> ShaderBuilder::_namedStringLoadStatus = std::unordered_map<std::string, bool>();

ShaderProgram ShaderBuilder::MinimalShaderProgram()
{
    static ShaderProgram Minimal = BuildShaderProgramFromConfig(ShaderConfig::MinimalConfig());
    return Minimal;
}

ShaderProgram ShaderBuilder::BuildShaderProgramFromConfig(const ShaderConfig& config, const bool dumpSource)
{
    const std::vector<ShaderFeature>& Features = config.getRequestedFeatures();
    std::unordered_set<ShaderStage> requestedStages;

    // Find out which shader stage were requested in the features
    for (auto it = Features.begin(); it != Features.end(); it++)
    {
        auto jt = FeatureStages().find(*it);
        if (jt == FeatureStages().end())
        {
            const std::string s = "ShaderBuilder: cannot build shader program from config, feature "
                "\"" + to_string(*it) + "\" (" + to_string((unsigned int) *it) + ") "
                "from unknown stage was requested.";

            throw std::runtime_error(s.c_str());
        }

        const ShaderStage stage = jt->second;
        requestedStages.insert(stage);
    }

    std::vector<Shader> shaders;
    // Generate all shader stages
    for (auto it = requestedStages.begin(); it != requestedStages.end(); it++)
    {
        const Shader shader = BuildShaderStageFromConfig(*it, config, dumpSource);
        shaders.push_back(shader);
    }

    // Link them together and return
    return LinkShaders(shaders);
}

ShaderProgram ShaderBuilder::LinkShaders(const std::vector<Shader>& shaders)
{
    std::unordered_set<ShaderStage> presentStages;
    std::vector<unsigned int> locations;

    // Check if there is no more than one shader per stage
    for (auto it = shaders.begin(); it != shaders.end(); it++)
    {
        ShaderStage stage = it->stage();
        auto jt = presentStages.find(stage);
        if (jt == presentStages.end())
        {
            presentStages.insert(stage);
            locations.push_back(it->location());
        }
        else
        {
            const std::string s = "ShaderBuilder: cannot link shaders, several objects were provided for stage \""
                + to_string(stage) + "\".";

            throw std::runtime_error(s.c_str());
        }        
    }

    // Check if there is at least a vertex shader and a fragment shader
    if (presentStages.find(ShaderStage::Vertex) == presentStages.end())
    {
        throw std::runtime_error("ShaderBuilder: cannot link shaders, a vertex shader is required but none was provided.");
    }
    if (presentStages.find(ShaderStage::Fragment) == presentStages.end())
    {
        throw std::runtime_error("ShaderBuilder: cannot link shaders, a fragment shader is required but none was provided.");
    }

    // Do the linking
    const unsigned int programLocation = _MakeShaderProgram(locations);
    if (!programLocation)
    {
        throw std::runtime_error("ShaderBuilder: the provided shaders could not be linked together. See std::cerr.");
    }

    // Aggregate supported features
    const std::vector<ShaderFeature> supportedFeatures = _AggregateShaderFeatures(shaders);

    // Return the shader instance
    return ShaderProgram(programLocation, supportedFeatures);
}

Shader ShaderBuilder::BuildShaderStageFromConfig(const ShaderStage stage, const ShaderConfig& config, const bool dumpSource)
{
    const std::vector<ShaderFeature> requested = _FilterFeaturesByStage(config.getRequestedFeatures(), stage);

    auto it = _StageTemplatePaths().find(stage);
    if (it == _StageTemplatePaths().end())
    {
        const std::string s = "ShaderBuilder: cannot find template path for stage \"" + to_string(stage) + "\".";
        throw std::runtime_error(s.c_str());
    }

	// Open template 
	std::ifstream file(it->second);

	if (!file.is_open())
    {
		const std::string s = "ShaderBuilder: Shader template \"" + it->second + "\" could not be found.";
        throw std::runtime_error(s.c_str());
    }

    // Add directives for version and extension usage, and define macros for
    // requested features
	std::string source = _GenerateVersionDirective()
        + _GenerateExtensionDirectives()
        + _GenerateDefineDirectives(requested);

    // Add all file contents
	std::string line;
	while (std::getline(file, line))
		source += line + "\n";

    return BuildShaderStageFromText(stage, source, requested, dumpSource);
}

Shader ShaderBuilder::BuildShaderStageFromFile(
    const ShaderStage stage,
    const std::string& filename,
    const std::vector<ShaderFeature>& supportedFeatures
)
{
	// Open input file 
	std::ifstream file(filename);

	if (!file.is_open())
    {
		const std::string s = "ShaderBuilder: Shader source \"" + filename + "\" could not be found.";
        throw std::runtime_error(s.c_str());
    }

    // Read all of its contents
	std::string source("");
	std::string line;
	while (std::getline(file, line))
		source += line + "\n";

    // If no info was provided about supported features, try to find some
    if (!supportedFeatures.size())
    {
        auto it = _FeaturesSupportedByFile().find(filename);
        if (it != _FeaturesSupportedByFile().end())
            return BuildShaderStageFromText(stage, source, it->second);
    }

    return BuildShaderStageFromText(stage, source, supportedFeatures);
}

Shader ShaderBuilder::BuildShaderStageFromText(
    const ShaderStage stage,
    const std::string& text,
    const std::vector<ShaderFeature>& supportedFeatures,
    const bool dumpSource)
{
    // In case the shader makes use of #include directives, process them
    _ProcessIncludeDirectives(text);

    // Get GL macro corresponding to requested shader stage
    auto it = _ShaderStageMacros().find(stage);
    if (it == _ShaderStageMacros().end())
    {
        const std::string s = "ShaderBuilder: Unknown requested shader stage \"" + to_string(stage) + "\" "
            "(" + to_string((unsigned int) stage) + ").";

        throw std::runtime_error(s.c_str());
    }
    const unsigned int shaderType = _ShaderStageMacros().at(stage);

	// Compile into shader
	const char* source = text.c_str();
	const unsigned int location = glCreateShader(shaderType);
	glShaderSource(location, 1, &source, nullptr);
	glCompileShaderIncludeARB(location, 0, nullptr, nullptr);

	// Print errors if any
	int success;
	char info[INFO_BUFFER_SIZE];
	glGetShaderiv(location, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(location, INFO_BUFFER_SIZE, nullptr, info);
        const std::string filename = _DumpShaderSource(stage, text);
		std::cerr << "Shader compilation failed:\n" << info << '\n';
        std::cerr << "Source was dumped to " << filename << std::endl;

		throw std::runtime_error("ShaderBuilder: Shader compilation failed. See std::cerr for more info.");
    }

    if (dumpSource)
    {
        const std::string filename = _DumpShaderSource(stage, text);
        std::cout << "Source was dumped to " << filename << std::endl;
	}

	return Shader(location, stage, supportedFeatures);
}

// There must be `count` arguments after `count`, all of type `unsigned int`.
unsigned int
ShaderBuilder::_MakeShaderProgram(const std::vector<unsigned int>& locations)
{
	const unsigned int program = glCreateProgram();

	for (auto it = locations.begin(); it != locations.end(); it++)
	{
		glAttachShader(program, *it);
	}

	// Link all shaders
	glLinkProgram(program);

	// Print errors if any
	int success;
	char info[INFO_BUFFER_SIZE];
	glGetProgramiv(program, GL_LINK_STATUS, &success);
	if (!success)
    {
		glGetProgramInfoLog(program, INFO_BUFFER_SIZE, NULL, info);
		std::cerr << "Shader linking failed:\n" << info << std::endl;
		return 0;
	}

	return program;
}

void ShaderBuilder::_ProcessIncludeDirectives(const std::string& text)
{
    std::unordered_set<std::string> includeDirectives = _FindIncludeDirectivesInSource(text, true);
    
    for (auto it = includeDirectives.begin(); it != includeDirectives.end(); it++)
    {
        // Find whether the named string for the requested include directive is loaded
        auto jt = _namedStringLoadStatus.find(*it);
        if (jt != _namedStringLoadStatus.end() && jt->second == true) continue;

        // Find the source file needed to create the requested named string
        auto kt = _IncludeFilenames().find(*it);
        if (kt == _IncludeFilenames().end())
        {
            const std::string s = "ShaderBuilder: Info about include directive "
                "<" + *it + "> cannot be found.";

            throw std::runtime_error(s.c_str());
        }

        _MakeNamedString(kt->first, kt->second);
    }
}

std::unordered_set<std::string> ShaderBuilder::_FindIncludeDirectivesInSource(std::string text, const bool recursive)
{
    static const std::string IncludeStringStart = "#include";
    std::unordered_set<std::string> includeDirectives;

    CppTools::String::stripComments(text);

    size_t offset = text.find(IncludeStringStart, 0);
    while (offset != std::string::npos)
    {
        size_t lastEol = text.find_last_of('\n', offset);
        if (lastEol == std::string::npos) lastEol = 0;

        const std::string before = text.substr(lastEol, offset - lastEol);
        if (!CppTools::String::stringIsWhitespace(before)) continue;

        offset += IncludeStringStart.size();
        const size_t eol = text.find('\n', offset);

        // Length of the string starting after the directive head,
        // going down to the next EOL, or end of the string altogether
        const size_t len = (eol != std::string::npos) ?
            eol - offset :
            std::string::npos;

        std::string includeArgument = text.substr(offset, len);
        CppTools::String::trim(includeArgument);

        const char firstDelimiter = includeArgument[0];
        char secondDelimiter = 0;
        switch (firstDelimiter)
        {
            case '<': secondDelimiter = '>'; break;
            case '"': secondDelimiter = '"'; break;
        }

        if (secondDelimiter == 0 || secondDelimiter != includeArgument.back())
        {
            const std::string s = "Badly formatted #include directive: argument \"" + includeArgument + "\" is illegal.";
            throw std::runtime_error(s.c_str());
        }

        // Strip delimiting chars
        includeArgument = includeArgument.substr(1, includeArgument.size() - 2);

        if ((includeArgument.find(secondDelimiter) != std::string::npos) ||
            (includeArgument.find(firstDelimiter) != std::string::npos))
        {
            const std::string s = "Badly formatted #include directive: argument \"" + includeArgument + "\" is illegal.";
            throw std::runtime_error(s.c_str());
        }
        includeDirectives.insert(includeArgument);

        offset = text.find(IncludeStringStart, offset);
    }

    if (recursive)
    {
        for (auto it = includeDirectives.begin(); it != includeDirectives.end(); it++)
        {
            // Retrieve filename for include directive
            auto jt = _IncludeFilenames().find(*it);
            if (jt == _IncludeFilenames().end())
            {
                const std::string s = "ShaderBuilder: missing info about include directive \"" + *it + "\", "
                    "cannot recursively find include directives.";

                throw std::runtime_error(s.c_str());
            }

            const std::string all = CppTools::String::readFileIntoString(jt->second);
            std::unordered_set<std::string> newIncludeDirectives = _FindIncludeDirectivesInSource(all, true);

            // Add them all to the current include directives
            includeDirectives.merge(newIncludeDirectives);
        }
    }

    return includeDirectives;
}

void ShaderBuilder::_MakeNamedString(const std::string& name, const std::string& sourceFilename)
{
    std::string all = CppTools::String::readFileIntoString(sourceFilename);
	const char* source = all.c_str();

    glNamedStringARB(GL_SHADER_INCLUDE_ARB, -1, name.c_str(), -1, source);

    _namedStringLoadStatus[name] = true;
}

std::vector<ShaderFeature> ShaderBuilder::_AggregateShaderFeatures(const std::vector<Shader>& shaders)
{
    std::vector<ShaderFeature> supportedFeatures;
    for (auto it = shaders.begin(); it != shaders.end(); it++)
    {
        const std::vector<ShaderFeature>& features = it->getSupportedFeatures();
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

    std::function<bool(ShaderFeature)> filter = [stage](ShaderFeature v) -> bool
    {
        auto it = FeatureStages().find(v);
        if (it == FeatureStages().end()) return false;
        return it->second == stage;
    };

    std::copy_if(features.begin(), features.end(), std::inserter(result, result.begin()), filter);
    return result;
}

std::string ShaderBuilder::_DumpShaderSource(ShaderStage stage, const std::string& text)
{
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

const std::string& ShaderBuilder::_GenerateVersionDirective()
{
    static const std::string s = "#version " + to_string(ShadingLanguageVersion) + " " + ShadingLanguageProfile + "\n";
    return s;
}

const std::string& ShaderBuilder::_GenerateExtensionDirectives()
{
    static bool runOnce = false;
    static std::string s;

    if (!runOnce)
    {
        std::function<std::string(std::string, std::pair<std::string, std::string>)>
        addDirective = [](std::string aggregate, std::pair<std::string, std::string> tuplet) -> std::string
        {
            return aggregate + 
                "#extension " + tuplet.first + " : " + tuplet.second + "\n";
        };

        const std::unordered_map<std::string, std::string>& extensions = _ShadingLanguageExtensions(); 
        s = std::accumulate(extensions.cbegin(), extensions.cend(), (std::string)(""), addDirective);

        runOnce = true;
    }

    return s;
}

const std::unordered_map<std::string, std::string>&
ShaderBuilder::_ShadingLanguageExtensions()
{
    static bool runOnce = false;
    static std::unordered_map<std::string, std::string> map;

    if (!runOnce)
    {
        map["GL_ARB_shading_language_include"] = "require";

        runOnce = true;
    }

    return map;
}

const std::unordered_map<ShaderStage, std::string>&
ShaderBuilder::_StageTemplatePaths()
{
    static bool runOnce = false;
    static std::unordered_map<ShaderStage, std::string> map;

    if (!runOnce)
    {
        map[ShaderStage::Vertex]    = "assets/shaders/templates/vertex_shader.vert";
        map[ShaderStage::Geometry]  = "assets/shaders/templates/geometry_shader.geom";
        map[ShaderStage::Fragment]  = "assets/shaders/templates/fragment_shader.frag";

        runOnce = true;
    }

    return map;
}

const std::unordered_map<ShaderFeature, std::string>&
ShaderBuilder::_FeatureDefineMacros()
{
    static bool runOnce = false;
    static std::unordered_map<ShaderFeature, std::string> map;

    if (!runOnce)
    {
        map[ShaderFeature::VertexMVP]                       = "VERTEX_MVP";
        // map[ShaderFeature::VertexFishEye]                   = "VERTEX_FISH_EYE";        // IMPLEMENT VERT LENS
        // map[ShaderFeature::GeometryShowNormals]             = "GEOMETRY_SHOW_NORMALS";  // IMPLEMENT GEOM NORMALS
        map[ShaderFeature::FragmentFullLight]               = "FRAGMENT_FULL_LIGHT";
        map[ShaderFeature::FragmentViewDepthBuffer]         = "FRAGMENT_VIEW_DEPTH_BUFFER";
        map[ShaderFeature::FragmentViewLightAttenuation]    = "FRAGMENT_VIEW_LIGHT_ATTENUATION";
        map[ShaderFeature::FragmentMeshMaterial]            = "FRAGMENT_MESH_MATERIAL";
        map[ShaderFeature::FragmentBypassVertexColor]       = "FRAGMENT_BYPASS_VERTEX_COLOR";
        // map[ShaderFeature::FragmentFlatShading]             = "FRAGMENT_FLAT_SHADING";  // IMPLEMENT FRAG FLAT
        map[ShaderFeature::FragmentPhong]                   = "FRAGMENT_PHONG";
        map[ShaderFeature::FragmentBlinnPhong]              = "FRAGMENT_BLINN_PHONG";
        map[ShaderFeature::FragmentGammaCorrection]         = "FRAGMENT_GAMMA_CORRECTION";
        // map[ShaderFeature::FragmentOutline]                 = "FRAGMENT_OUTLINE";       // IMPLEMENT FRAG OUTLINE
        // map[ShaderFeature::FragmentCubemap]                 = "FRAGMENT_CUBEMAP";       // IMPLEMENT FRAG CUBEMAP
        // map[ShaderFeature::FragmentBlending]                = "FRAGMENT_BLENDING";      // IMPLEMENT FRAG BLENDING
        // map[ShaderFeature::FragmentShadows]                 = "FRAGMENT_SHADOWS";       // IMPLEMENT FRAG SHADOWS

        runOnce = true;
    }

    return map;
}

const std::unordered_map<std::string, std::string>&
ShaderBuilder::_IncludeFilenames()
{
    static bool runOnce = false;
    static std::unordered_map<std::string, std::string> map;

    if (!runOnce)
    {
        map["/functional_blocks/gamma_correction"]  = "assets/shaders/functional_blocks/gamma_correction.glsl";
        map["/functional_blocks/light_attenuation"] = "assets/shaders/functional_blocks/light_attenuation.glsl";
        map["/interface_blocks/light_types"]        = "assets/shaders/interface_blocks/light_types.glsl";
        map["/interface_blocks/vertex_attributes"]  = "assets/shaders/interface_blocks/vertex_attributes.glsl";
        map["/interface_blocks/vertex_out"]         = "assets/shaders/interface_blocks/vertex_out.glsl";
        map["/templates/phong"]                     = "assets/shaders/templates/phong.glsl";
        map["/uniform_blocks/lights"]               = "assets/shaders/uniform_blocks/lights.glsl";
        map["/uniform_blocks/material"]             = "assets/shaders/uniform_blocks/material.glsl";
        map["/uniform_blocks/matrices"]             = "assets/shaders/uniform_blocks/matrices.glsl";

        runOnce = true;
    }

    return map;
}

const std::unordered_map<std::string, std::vector<ShaderFeature>>&
ShaderBuilder::_FeaturesSupportedByFile()
{
    static bool runOnce = false;
    static std::unordered_map<std::string, std::vector<ShaderFeature>> map;

    if (!runOnce)
    {
        map["assets/shaders/default.frag"]  = {
            ShaderFeature::FragmentFullLight
        };
        map["assets/shaders/depth.frag"]  = {
            ShaderFeature::FragmentViewDepthBuffer
        };
        map["assets/shaders/mvp.vert"]  = {
            ShaderFeature::VertexMVP
        };
        map["assets/shaders/phong.frag"]    = {
            ShaderFeature::FragmentMeshMaterial,
            ShaderFeature::FragmentBlinnPhong,
            ShaderFeature::FragmentGammaCorrection
        };

        runOnce = true;
    }

    return map;
}

const std::unordered_map<ShaderStage, unsigned int>&
ShaderBuilder::_ShaderStageMacros()
{
    static bool runOnce = false;
    static std::unordered_map<ShaderStage, unsigned int> map;

    if (!runOnce)
    {
        map[ShaderStage::Vertex]    = GL_VERTEX_SHADER;
        map[ShaderStage::Geometry]  = GL_GEOMETRY_SHADER;
        map[ShaderStage::Fragment]  = GL_FRAGMENT_SHADER;

        runOnce = true;
    }

    return map;
}

const std::unordered_map<ShaderStage, std::string>&
ShaderBuilder::_StageFileExtensions()
{
    static bool runOnce = false;
    static std::unordered_map<ShaderStage, std::string> map;

    if (!runOnce)
    {
        map[ShaderStage::Vertex]    = "vert";
        map[ShaderStage::Geometry]  = "geom";
        map[ShaderStage::Fragment]  = "frag";

        runOnce = true;
    }

    return map;
}

std::string
ShaderBuilder::_GenerateDefineDirectives(const std::vector<ShaderFeature>& features)
{
    static const std::unordered_map<ShaderFeature, std::string>&
    defineMacros = _FeatureDefineMacros();

    std::function<std::string(std::string, ShaderFeature)>
    addDirective = [](std::string aggregate, ShaderFeature feature) -> std::string
    {
        auto it = defineMacros.find(feature);
        if (it == defineMacros.cend())
        {
            std::string s = "ShaderBuilder: cannot generate #define directive "
                "for feature \"" + to_string(feature) + "\" "
                "(" + to_string((unsigned int)(feature)) + ").";

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

}//namespace Renderboi

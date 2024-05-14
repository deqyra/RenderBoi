#include "shader_config.hpp"

#include <iterator>
#include <stdexcept>
#include <string>

#include <cpptools/utility/string.hpp>

namespace rb {

ShaderConfig::ShaderConfig() :
    _requestedFeatures(),
    _problems(),
    _configVectorOutdated(false) {

}

ShaderConfig::ShaderConfig(std::initializer_list<ShaderFeature> list) :
    _requestedFeatures(),
    _problems(),
    _configVectorOutdated(false) {
    for (auto feature : list)
    {
        addFeature(feature);
    }
}


const ShaderConfig& ShaderConfig::MinimalConfig() {
    static ShaderConfig config = {
        ShaderFeature::VertexMVP,
        ShaderFeature::FragmentFullLight
    };

    return config;
}

bool ShaderConfig::_checkForConflicts(const ShaderFeature newFeature) const
{
    _problems.clear();

    auto it = IncompatibleFeatures().find(newFeature);
    if (it == IncompatibleFeatures().end()) return false;

    const std::vector<ShaderFeature>& incompatibleFeatures = it->second;

    // For all features incompatible with the newly requested feature
    for (const auto& feature : incompatibleFeatures)
    {
        // Check if it is part of the currently requested features
        auto kt = _requestedFeatures.find(feature);
        if (kt != _requestedFeatures.end())
        {
            // If it is, add it to the problems array
            _problems.push_back(feature);
        }
    }

    return !_problems.empty();
}

bool ShaderConfig::_checkRequirementsAreMet(const ShaderFeature newFeature) const
{
    _problems.clear();

    auto it = FeatureRequirements().find(newFeature);
    if (it == FeatureRequirements().end()) return true;

    const std::vector<ShaderFeature>& featureRequirements = it->second;

    // For all features required by the newly requested feature
    for (const auto feature : featureRequirements)
    {
        // Check if it is part of the currently requested features
        auto kt = _requestedFeatures.find(feature);
        if (kt == _requestedFeatures.end())
        {
            // If it is not, add it to the problems array
            _problems.push_back(feature);
        }
    }

    return _problems.empty();
}

void ShaderConfig::addFeature(const ShaderFeature newFeature) {
    if (_requestedFeatures.find(newFeature) != _requestedFeatures.end()) return;

    // Check that the new feature would not be incompatible with any already present
    if (_checkForConflicts(newFeature))
    {
        //! @todo: REVAMP THIS BS
        const std::string s = "ShaderConfig: cannot request new feature " + to_string(newFeature) + " as it conflicts "
            "with the following already present features: " + /* tools::iterableToString(_problems, ",") + */ ".";

        throw std::runtime_error(s.c_str());
    }

    // Check that the requirements for the new feature are all met
    if (!_checkRequirementsAreMet(newFeature))
    {
        //! @todo: REVAMP THIS BS
        const std::string s = "ShaderConfig: cannot request new feature " + to_string(newFeature)
            + " as it requires the following features, which are currently absent from the config: "
            + /* tools::iterableToString(_problems, ",") + */ ". Consider using addFeatureWithRequirements.";

        throw std::runtime_error(s.c_str());
    }

    // Add the new feature
    _requestedFeatures.insert(newFeature);
    _configVectorOutdated = true;
}

void ShaderConfig::addFeatureWithRequirements(const ShaderFeature newFeature) {
    if (_requestedFeatures.find(newFeature) != _requestedFeatures.end()) return;

    // Check that the new feature would not be incompatible with any already present
    if (_checkForConflicts(newFeature))
    {
        //! @todo: REVAMP THIS BS
        const std::string s = "ShaderConfig: cannot request new feature " + to_string(newFeature) + " as it conflicts "
            "with the following already present features: " + /* tools::iterableToString(_problems, ",") + */ ".";

        throw std::runtime_error(s.c_str());
    }

    // If not all requirements for the new feature are met, add the missing ones
    if (!_checkRequirementsAreMet(newFeature))
    {
        for (const auto& feature : _problems)
        {
            addFeatureWithRequirements(feature);
        }
    }

    _requestedFeatures.insert(newFeature);
    _configVectorOutdated = true;
}

void ShaderConfig::removeFeature(const ShaderFeature feature) {
    auto it = _requestedFeatures.find(feature);
    if (it != _requestedFeatures.end())
    {
        _requestedFeatures.erase(feature);
        _configVectorOutdated = true;
    }
}

const std::vector<ShaderFeature>& ShaderConfig::getRequestedFeatures() const
{
    static std::vector<ShaderFeature> result;

    if (_configVectorOutdated)
    {
        result.clear();
        std::copy(
            _requestedFeatures.begin(),
            _requestedFeatures.end(),
            std::back_inserter(result)
        );
        
        _configVectorOutdated = false;
    }

    return result;
}

const std::unordered_map<ShaderFeature, std::vector<ShaderFeature>>& ShaderConfig::FeatureRequirements() {
    static std::unordered_map<ShaderFeature, std::vector<ShaderFeature>> map = {
        {ShaderFeature::VertexMVP,                       {}},
        {ShaderFeature::VertexNormalsToColor,            {
            ShaderFeature::VertexMVP
        }},
        // {ShaderFeature::VertexFishEye,                   {}},   // IMPLEMENT VERT LENS
        // {ShaderFeature::GeometryShowNormals,             {}},   // IMPLEMENT GEOM NORMALS
        {ShaderFeature::FragmentFullLight,               {}},
        {ShaderFeature::FragmentViewDepthBuffer,         {}},
        {ShaderFeature::FragmentViewLightAttenuation,    {}},
        {ShaderFeature::FragmentMeshMaterial,            {}},
        {ShaderFeature::FragmentBypassVertexColor,       {}},
        {ShaderFeature::FragmentPhong,                   {
            ShaderFeature::FragmentMeshMaterial
        }},
        {ShaderFeature::FragmentBlinnPhong,              {
            ShaderFeature::FragmentMeshMaterial
        }},
        // {ShaderFeature::FragmentFlatShading,             {}},   // IMPLEMENT FRAG FLAT
        {ShaderFeature::FragmentGammaCorrection,         {}},
        // {ShaderFeature::FragmentOutline,                 {}},   // IMPLEMENT FRAG OUTLINE
        // {ShaderFeature::FragmentCubemap,                 {}},   // IMPLEMENT FRAG CUBEMAP
        // {ShaderFeature::FragmentBlending,                {}},   // IMPLEMENT FRAG BLENDING
        // {ShaderFeature::FragmentShadows,                 {}},   // IMPLEMENT FRAG SHADOWS
    };

    return map;
}

const std::unordered_map<ShaderFeature, std::vector<ShaderFeature>>& ShaderConfig::IncompatibleFeatures() {
    static std::unordered_map<ShaderFeature, std::vector<ShaderFeature>> map = {
        {ShaderFeature::VertexMVP,                       {}},
        {ShaderFeature::VertexNormalsToColor,            {}},
        // {ShaderFeature::VertexFishEye,                   {}},   // IMPLEMENT VERT LENS
        // {ShaderFeature::GeometryShowNormals,             {}},   // IMPLEMENT GEOM NORMALS
        {ShaderFeature::FragmentFullLight,               {
            ShaderFeature::FragmentViewDepthBuffer,
            ShaderFeature::FragmentViewLightAttenuation,
            ShaderFeature::FragmentPhong,
            ShaderFeature::FragmentBlinnPhong
            // ShaderFeature::FragmentFlatShading                  // IMPLEMENT FRAG FLAT
        }},
        {ShaderFeature::FragmentViewDepthBuffer,         {
            ShaderFeature::FragmentFullLight,
            ShaderFeature::FragmentViewLightAttenuation,
            ShaderFeature::FragmentPhong,
            ShaderFeature::FragmentBlinnPhong
            // ShaderFeature::FragmentFlatShading                  // IMPLEMENT FRAG FLAT
        }},
        {ShaderFeature::FragmentViewLightAttenuation,    {
            ShaderFeature::FragmentFullLight,
            ShaderFeature::FragmentViewDepthBuffer,
            ShaderFeature::FragmentPhong,
            ShaderFeature::FragmentBlinnPhong
            // ShaderFeature::FragmentFlatShading                  // IMPLEMENT FRAG FLAT
        }},
        {ShaderFeature::FragmentMeshMaterial,            {}},
        {ShaderFeature::FragmentBypassVertexColor,       {}},
        {ShaderFeature::FragmentPhong,                   {
            ShaderFeature::FragmentFullLight,
            ShaderFeature::FragmentViewDepthBuffer,
            ShaderFeature::FragmentViewLightAttenuation,
            ShaderFeature::FragmentBlinnPhong
        }},
        {ShaderFeature::FragmentBlinnPhong,              {
            ShaderFeature::FragmentFullLight,
            ShaderFeature::FragmentViewDepthBuffer,
            ShaderFeature::FragmentViewLightAttenuation,
            ShaderFeature::FragmentPhong
        }},
        /* {ShaderFeature::FragmentFlatShading,            {       // IMPLEMENT FRAG FLAT
            ShaderFeature::FragmentFullLight,
            ShaderFeature::FragmentViewDepthBuffer
        }}, */
        {ShaderFeature::FragmentGammaCorrection,         {}},
        // {ShaderFeature::FragmentOutline,                 {}},   // IMPLEMENT FRAG OUTLINE
        // {ShaderFeature::FragmentCubemap,                 {}},   // IMPLEMENT FRAG CUBEMAP
        // {ShaderFeature::FragmentBlending,                {}},   // IMPLEMENT FRAG BLENDING
        // {ShaderFeature::FragmentShadows,                 {}},   // IMPLEMENT FRAG SHADOWS
    };

    return map;
}

} // namespace rb

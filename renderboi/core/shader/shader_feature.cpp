#include "shader_feature.hpp"

#include <string>
#include <unordered_map>

#include "shader_stage.hpp"

namespace renderboi
{

const std::unordered_map<ShaderFeature, ShaderStage>& FeatureStages()
{
    static std::unordered_map<ShaderFeature, ShaderStage> map = {
        {ShaderFeature::VertexMVP,                      ShaderStage::Vertex},
        {ShaderFeature::VertexNormalsToColor,           ShaderStage::Vertex},
        // {ShaderFeature::VertexFishEye,                  ShaderStage::Vertex},      // IMPLEMENT VERT LENS
        // {ShaderFeature::GeometryShowNormals,            ShaderStage::Geometry},    // IMPLEMENT GEOM NORMALS
        {ShaderFeature::FragmentFullLight,              ShaderStage::Fragment},
        {ShaderFeature::FragmentViewDepthBuffer,        ShaderStage::Fragment},
        {ShaderFeature::FragmentViewLightAttenuation,   ShaderStage::Fragment},
        {ShaderFeature::FragmentMeshMaterial,           ShaderStage::Fragment},
        {ShaderFeature::FragmentBypassVertexColor,      ShaderStage::Fragment},
        // {ShaderFeature::FragmentFlatShading,            ShaderStage::Fragment},    // IMPLEMENT FRAG FLAT
        {ShaderFeature::FragmentPhong,                  ShaderStage::Fragment},
        {ShaderFeature::FragmentBlinnPhong,             ShaderStage::Fragment},
        {ShaderFeature::FragmentGammaCorrection,        ShaderStage::Fragment},
        // {ShaderFeature::FragmentOutline,                ShaderStage::Fragment},    // IMPLEMENT FRAG OUTLINE
        // {ShaderFeature::FragmentCubemap,                ShaderStage::Fragment},    // IMPLEMENT FRAG CUBEMAP
        // {ShaderFeature::FragmentBlending,               ShaderStage::Fragment},    // IMPLEMENT FRAG BLENDING
        // {ShaderFeature::FragmentShadows,                ShaderStage::Fragment},    // IMPLEMENT FRAG SHADOWS
    };

    return map;
}

std::string to_string(const ShaderFeature v)
{
    static std::unordered_map<ShaderFeature, std::string> featureNames = {
        {ShaderFeature::VertexMVP,                      "VertexMVP"},
        {ShaderFeature::VertexNormalsToColor,           "VertexNormalsToColor"},
        // {ShaderFeature::VertexFishEye,                  "VertexFishEye"},       // IMPLEMENT VERT LENS
        // {ShaderFeature::GeometryShowNormals,            "GeometryShowNormals"}, // IMPLEMENT GEOM NORMALS
        {ShaderFeature::FragmentFullLight,              "FragmentFullLight"},
        {ShaderFeature::FragmentViewDepthBuffer,        "FragmentViewDepthBuffer"},
        {ShaderFeature::FragmentViewLightAttenuation,   "FragmentViewLightAttenuation"},
        {ShaderFeature::FragmentMeshMaterial,           "FragmentMeshMaterial"},
        {ShaderFeature::FragmentBypassVertexColor,      "FragmentBypassVertexColor"},
        // {ShaderFeature::FragmentFlatShading,            "FragmentFlatShading"}, // IMPLEMENT FRAG FLAT
        {ShaderFeature::FragmentPhong,                  "FragmentPhong"},
        {ShaderFeature::FragmentBlinnPhong,             "FragmentBlinnPhong"},
        {ShaderFeature::FragmentGammaCorrection,        "FragmentGammaCorrection"},
        // {ShaderFeature::FragmentOutline,                "FragmentOutline"},     // IMPLEMENT FRAG OUTLINE
        // {ShaderFeature::FragmentCubemap,                "FragmentCubemap"},     // IMPLEMENT FRAG CUBEMAP
        // {ShaderFeature::FragmentBlending,               "FragmentBlending"},    // IMPLEMENT FRAG BLENDING
        // {ShaderFeature::FragmentShadows,                "FragmentShadows"},     // IMPLEMENT FRAG SHADOWS
    };

    auto it = featureNames.find(v);
    return (it != featureNames.end())
        ? it->second
        : "Unknown shader feature";
}

} // namespace renderboi

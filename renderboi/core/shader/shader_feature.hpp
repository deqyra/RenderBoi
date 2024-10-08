#ifndef RENDERBOI_CORE_SHADER_SHADER_FEATURE_HPP
#define RENDERBOI_CORE_SHADER_SHADER_FEATURE_HPP

#include <string>
#include <unordered_map>

#include "shader_stage.hpp"

namespace rb {

/// @brief Literals describing the different aspects which
/// a shader might support when rendering a scene or its elements
enum class ShaderFeature {
    // Upon adding litterals to this enum, make sure you also update the 
    // following places:
    // - ShaderBuilder::_FeatureDefineMacros() in shader_builder.cpp
    // - ShaderConfig::FeatureRequirements() in shader_config.cpp
    // - ShaderConfig::IncompatibleFeatures() in shader_config.cpp
    // - FeatureStages() in shader_feature.cpp
    // - to_string(ShaderFeature) in shader_feature.cpp
    //
    // Upon changing a litteral's name, updates in all places mentioned
    // above may be required, as well as in all shader templates where the
    // corresponding macro is used.

    /// @brief The vertex stage of the shader will handle MVP matrices
    /// Requires: nothing
    /// Incompatible with: nothing
    VertexMVP,

    // VertexFishEye,          // IMPLEMENT VERT LENS

    /// @brief The vertex stage of the shader will output vertex normals as
    /// vertex colors
    /// Requires: VertexMVP
    /// Incompatible with: nothing
    VertexNormalsToColor,

    // GeometryShowNormals,    // IMPLEMENT GEOM NORMALS

    /// @brief The fragment stage of the shader will render objects with
    /// full light, ignoring actual lighting
    /// Requires: nothing
    /// Incompatible with: FragmentViewDepthBuffer,
    /// FragmentViewLightAttenuation, FragmentPhong, FragmentBlinnPhong
    FragmentFullLight,

    /// @brief The fragment stage of the shader will render objects as they
    /// appear in the depth buffer, ignoring lighting
    /// Requires: nothing
    /// Incompatible with: FragmentFullLight, FragmentViewLightAttenuation,
    /// FragmentPhong, FragmentBlinnPhong
    FragmentViewDepthBuffer,

    /// @brief The fragment stage of the shader will render objects 
    /// completely white with lighting, showing sort of a light attenuation
    /// map. Ignores directional lighting, which is not attenuated
    /// Requires: nothing
    /// Incompatible with: FragmentFullLight, FragmentViewDepthBuffer, 
    /// FragmentPhong, FragmentBlinnPhong
    FragmentViewLightAttenuation,

    /// @brief The fragment stage of the shader will handle materials to 
    /// render objects with
    /// Requires: nothing
    /// Incompatible with: nothing
    FragmentMeshMaterial,

    /// @brief The fragment stage of the shader will ignore vertex color
    /// entirely
    /// Requires: nothing
    /// Incompatible with: nothing
    FragmentBypassVertexColor,

    // FragmentFlatShading,    // IMPLEMENT FRAG FLAT
    
    /// @brief The fragment stage of the shader will calculate object
    /// illumination using a Phong lighting model
    /// Requires: FragmentMeshMaterial
    /// Incompatible with: FragmentFullLight, FragmentViewDepthBuffer,
    /// FragmentViewLightAttenuation, FragmentBlinnPhong
    FragmentPhong,
    
    /// @brief The fragment stage of the shader will calculate object
    /// illumination using a Blinn-Phong lighting model
    /// Requires: FragmentMeshMaterial
    /// Incompatible with: FragmentFullLight, FragmentViewDepthBuffer,
    /// FragmentViewLightAttenuation, FragmentPhong
    FragmentBlinnPhong,
    
    /// @brief The fragment stage of the shader will correct the output 
    /// color according to a gamma value
    /// Requires: nothing
    /// Incompatible with: nothing
    FragmentGammaCorrection

    // FragmentOutline,        // IMPLEMENT FRAG OUTLINE

    // FragmentCubemap,        // IMPLEMENT FRAG CUBEMAP

    // FragmentBlending,       // IMPLEMENT FRAG BLENDING
    
    // FragmentShadows         // IMPLEMENT FRAG SHADOWS
};

/// @brief Get the map describing in which stage shader features are
/// implemented
///
/// @return A map describing in which stage shader features are implemented
const std::unordered_map<ShaderFeature, ShaderStage>& FeatureStages();

std::string to_string(const ShaderFeature v);

} // namespace rb

#endif//RENDERBOI_CORE_SHADER_SHADER_FEATURE_HPP
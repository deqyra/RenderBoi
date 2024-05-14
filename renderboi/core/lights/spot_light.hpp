#ifndef RENDERBOI_CORE_LIGHTS_SPOT_LIGHT_HPP
#define RENDERBOI_CORE_LIGHTS_SPOT_LIGHT_HPP

#include <renderboi/core/numeric.hpp>

#include "light_common.hpp"

namespace rb {

/// @brief Collection of parameters representing a spot light
struct SpotLight {
    /// @brief RGB color of the ambient component of the emitted light
    LightColor color;

    /// @brief Constant term of the light attenuation computation
    LightAttenuation attenuation;
    
    /// @brief Direction the spot light is facing
    num::Vec3 direction;
    
    /// @brief Angle at which the light starts fading out
    float innerCutoff;
    
    /// @brief Angle at which the light has completely faded out
    float outerCutoff;
};

} // namespace rb

#endif//RENDERBOI_CORE_LIGHTS_SPOT_LIGHT_HPP
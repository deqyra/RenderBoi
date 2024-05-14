#ifndef RENDERBOI_CORE_LIGHTS_POINT_LIGHT_HPP
#define RENDERBOI_CORE_LIGHTS_POINT_LIGHT_HPP

#include <renderboi/core/numeric.hpp>

#include "light_common.hpp"

namespace rb {

/// @brief Collection of parameters representing a point light
struct PointLight {
    /// @brief RGB color of the ambient component of the emitted light
    LightColor color;

    /// @brief Constant term of the light attenuation computation
    LightAttenuation attenuation;
};

} // namespace rb

#endif//RENDERBOI_CORE_LIGHTS_POINT_LIGHT_HPP
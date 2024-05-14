#ifndef RENDERBOI_CORE_LIGHTS_DIRECTIONAL_LIGHT_HPP
#define RENDERBOI_CORE_LIGHTS_DIRECTIONAL_LIGHT_HPP

#include <renderboi/core/numeric.hpp>
#include <renderboi/core/color.hpp>

#include "light_common.hpp"

namespace rb {

/// @brief Collection of parameters representing a directional light
struct DirectionalLight {
    /// @brief Color emitted by the light
    LightColor color;

    /// @brief Direction of the light
    num::Vec3 direction = num::Z;
};

} // namespace rb

#endif//RENDERBOI_CORE_LIGHTS_DIRECTIONAL_LIGHT_HPP
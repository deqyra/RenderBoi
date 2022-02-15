#ifndef RENDERBOI__CORE__LIGHTS__LIGHT_TYPE_HPP
#define RENDERBOI__CORE__LIGHTS__LIGHT_TYPE_HPP

namespace renderboi
{

/// @brief Collection of literals describing the available types of light.
enum class LightType
{
    /// @brief Emits light in all directions from its position.
    PointLight,
    /// @brief Emits light in the same direction for the whole scene.
    DirectionalLight,
    /// @brief Emits light in a cone from its position.
    SpotLight
};

} // namespace renderboi

#endif//RENDERBOI__CORE__LIGHTS__LIGHT_TYPE_HPP

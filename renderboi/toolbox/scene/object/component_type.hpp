#ifndef RENDERBOI__TOOLBOX__SCENE__OBJECT__COMPONENT_TYPE_HPP
#define RENDERBOI__TOOLBOX__SCENE__OBJECT__COMPONENT_TYPE_HPP

#include <string>

namespace renderboi
{

/// @brief Literals describing types of available components.
enum class ComponentType
{
    /// @brief Literal representing a MeshComponent.
    Mesh,
    /// @brief Literal representing a LightComponent.
    Light,
    /// @brief Literal representing a CameraComponent.
    Camera,
    /// @brief Literal representing a ScriptComponent.
    Script
};

std::string to_string(const ComponentType type);

} // namespace renderboi

// New literals representing new classes derived from Component must be added here

#endif//RENDERBOI__TOOLBOX__SCENE__OBJECT__COMPONENT_TYPE_HPP
#ifndef RENDERBOI__TOOLBOX__SCENE__COMPONENTS__LIGHT_COMPONENT
#define RENDERBOI__TOOLBOX__SCENE__COMPONENTS__LIGHT_COMPONENT

#include <renderboi/core/lights/light.hpp>

namespace renderboi
{

/// @brief Component allowinf to attach any light to a scene object.
class LightComponent
{
private:
    LightComponent(LightComponent& other) = delete;
    LightComponent& operator=(const LightComponent& other) = delete;

    /// @brief Pointer to the light, if owned. Is null when the referenced light
    /// isn't owned by the instance.
    LightPtr _lightPtr;

    /// @brief Reference to the mesh data of the object.
    Light& _light;

public:
    /// @param light Reference to the light which the component will use.
    ///
    /// @exception If the passed light pointer is null, the function will throw
    /// a std::runtime_error.
    LightComponent(LightPtr&& light);

    /// @param light Reference to the light which the component will use.
    LightComponent(Light& light);

    ~LightComponent();

    /// @brief Get a pointer to the light used by the component.
    ///
    /// @return A pointer to the light used by the component.
    Light& light();
};

} // namespace renderboi

#endif//RENDERBOI__TOOLBOX__SCENE__COMPONENTS__LIGHT_COMPONENT
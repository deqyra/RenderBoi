#ifndef RENDERBOI__TOOLBOX__SCENE__COMPONENTS__LIGHT_COMPONENT
#define RENDERBOI__TOOLBOX__SCENE__COMPONENTS__LIGHT_COMPONENT

#include <renderboi/core/lights/light.hpp>

#include "../component.hpp"
#include "../component_type.hpp"

namespace renderboi
{

/// @brief Component allowinf to attach any light to a scene object.
class LightComponent : public Component
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
    /// @param sceneObject Reference to the scene object which will be parent
    /// to this component.
    /// @param light Reference to the light which the component will use.
    ///
    /// @exception If the passed light pointer is null, the function will throw
    /// a std::runtime_error.
    LightComponent(SceneObject& sceneObject, LightPtr&& light);

    /// @param sceneObject Reference to the scene object which will be parent
    /// to this component.
    /// @param light Reference to the light which the component will use.
    LightComponent(SceneObject& sceneObject, Light& light);

    ~LightComponent();

    /// @brief Get a pointer to the light used by the component.
    ///
    /// @return A pointer to the light used by the component.
    Light& light();

    /////////////////////////////////////////
    ///                                   ///
    /// Methods overridden from Component ///
    ///                                   ///
    /////////////////////////////////////////

    /// @brief Get a raw pointer to a new component instance cloned 
    /// from this one. Ownership and responsibility for the allocated 
    /// resources are fully transferred to the caller.
    ///
    /// @param newParent Reference to the scene object which will be parent to
    /// the cloned component instance.
    ///
    /// @return A raw pointer to the component instance cloned from this 
    /// one.
    LightComponent* clone(SceneObject& newParent) const override;
};


template<>
struct ComponentMeta<ComponentType::Light>
{
    struct MultipleInstancesAllowed
    {
        static constexpr bool value = false;
    };

    struct ConcreteType
    {
        using type = LightComponent;
    };

    struct Name
    {
        static constexpr const char* value = "LightComponent";
    };
};

template<>
struct ComponentTypeToEnum<LightComponent>
{
    static constexpr ComponentType value = ComponentType::Light;
};

} // namespace renderboi

#endif//RENDERBOI__TOOLBOX__SCENE__COMPONENTS__LIGHT_COMPONENT
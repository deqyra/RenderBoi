#ifndef RENDERBOI__TOOLBOX__SCENE__COMPONENTS__SCRIPT_COMPONENT_HPP
#define RENDERBOI__TOOLBOX__SCENE__COMPONENTS__SCRIPT_COMPONENT_HPP

#include <string>
#include <type_traits>

#include "../component.hpp"
#include "../component_type.hpp"
#include "../../../script.hpp"

namespace renderboi
{

/// @brief Component allowing a scene object to self-update through time.
class ScriptComponent : public Component
{
private:
    ScriptComponent(ScriptComponent& other) = delete;
    ScriptComponent& operator=(const ScriptComponent& other) = delete;

    /// @brief Pointer to an owned script resource. May be null in the case 
    /// where the referenced script is external to the component instance.
    ScriptPtr _scriptPtr;

    /// @brief Reference to the actual script resource.
    Script& _script;

    /// @brief Register script to the scene which the parent scene object
    /// belongs to.
    void _registerScript();

    /// @brief Detach script from the scene it is registered at.
    void _detachScript();

public:
    /// @param sceneObject Reference to the scene object which will be parent
    /// to this component.
    /// @param script Reference to the input processing script which the 
    /// component will use.
    ScriptComponent(SceneObject& sceneObject, Script& script);

    /// @param sceneObject Reference to the scene object which will be parent
    /// to this component.
    /// @param script Reference to the input processing script which the 
    /// component will use.
    ///
    /// @exception If the passed script pointer is null, the function will 
    /// throw a std::runtime_error.
    ScriptComponent(SceneObject& sceneObject, ScriptPtr&& script);

    /// @param sceneObject Reference to the scene object which will be parent
    /// to this component.
    /// @param args Arguments to pass to the constructor of the instantiated
    /// script.
    ///
    /// @tparam T Required. Concrete type of Script to instantiate.
    /// @tparam ArgTypes Types of the arguments to be forwarded to the 
    /// constructor of T. Leave for deduction.
    template<
        typename T,
        typename... ArgTypes,
        typename = std::enable_if_t<std::is_base_of_v<Script, T>, void>
    >
    ScriptComponent(SceneObject& sceneObject, ArgTypes&& ...args);

    ~ScriptComponent() = default;

    /// @brief Get a reference to the script used by the component.
    ///
    /// @return Reference to the script used by the component.
    Script& script() const;

    /////////////////////////////////////////
    ///                                   ///
    /// Methods overridden from Component ///
    ///                                   ///
    /////////////////////////////////////////

    /// @brief Get a raw reference to a new component instance cloned 
    /// from this one. Ownership and responsibility for the allocated 
    /// resources are fully transferred to the caller.
    ///
    /// @param newParent Reference the scene object which will be parent to
    /// the cloned component instance.
    ///
    /// @return A raw reference to the component instance cloned from this 
    /// one.
    ScriptComponent* clone(SceneObject& newParent) const override;
};

template<
    typename T,
    typename... ArgTypes,
    typename
>
ScriptComponent::ScriptComponent(SceneObject& sceneObject, ArgTypes&& ...args) :
    Component(sceneObject),
    _scriptPtr(std::make_unique<T>(std::forward(args)...)),
    _script(*_scriptPtr)
{

}

template<>
struct ComponentMeta<ComponentType::Script>
{
    struct MultipleInstancesAllowed
    {
        static constexpr bool value = true;
    };

    struct ConcreteType
    {
        using type = ScriptComponent;
    };

    struct Name
    {
        static constexpr const char* value = "ScriptComponent";
    };
};

template<>
struct ComponentTypeToEnum<ScriptComponent>
{
    static constexpr ComponentType value = ComponentType::Script;
};

} // namespace renderboi

#endif//RENDERBOI__TOOLBOX__SCENE__COMPONENTS__SCRIPT_COMPONENT_HPP
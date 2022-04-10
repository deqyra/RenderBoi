#ifndef RENDERBOI__TOOLBOX__SCENE__COMPONENTS__SCRIPT_COMPONENT_HPP
#define RENDERBOI__TOOLBOX__SCENE__COMPONENTS__SCRIPT_COMPONENT_HPP

#include <string>
#include <type_traits>

#include "../../../script.hpp"

namespace renderboi
{

/// @brief Component allowing a scene object to self-update through time.
class ScriptComponent
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
    ScriptComponent(Script& script);

    /// @param sceneObject Reference to the scene object which will be parent
    /// to this component.
    /// @param script Reference to the input processing script which the 
    /// component will use.
    ///
    /// @exception If the passed script pointer is null, the function will 
    /// throw a std::runtime_error.
    ScriptComponent(ScriptPtr&& script);

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
    ScriptComponent(ArgTypes&& ...args);

    ~ScriptComponent() = default;

    /// @brief Get a reference to the script used by the component.
    ///
    /// @return Reference to the script used by the component.
    Script& script();
};

template<
    typename T,
    typename... ArgTypes,
    typename
>
ScriptComponent::ScriptComponent(ArgTypes&& ...args) :
    _scriptPtr(std::make_unique<T>(std::forward(args)...)),
    _script(*_scriptPtr)
{

}

} // namespace renderboi

#endif//RENDERBOI__TOOLBOX__SCENE__COMPONENTS__SCRIPT_COMPONENT_HPP
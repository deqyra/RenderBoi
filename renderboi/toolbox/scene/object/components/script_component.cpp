#include "script_component.hpp"

#include "../../scene.hpp"
#include <memory>

namespace renderboi
{

ScriptComponent::ScriptComponent(SceneObject& sceneObject, Script& script) :
    Component(sceneObject),
    _scriptPtr(nullptr),
    _script(script)
{
    _script.setSceneObject(&sceneObject);
    _registerScript();
}

ScriptComponent::ScriptComponent(SceneObject& sceneObject, ScriptPtr&& script) :
    Component(sceneObject),
    _scriptPtr(std::move(script)),
    _script(*_scriptPtr)
{

}

Script& ScriptComponent::script() const
{
    return _script;
}

ScriptComponent* ScriptComponent::clone(SceneObject& newParent) const
{
    ScriptPtr clonedScript = ScriptPtr(_script.clone());
    ScriptComponent* clonedComponent = new ScriptComponent(newParent, std::move(clonedScript));
    // The parent scene object needs to be registered in the component, which is to be done by the caller
    return clonedComponent;
}

void ScriptComponent::_registerScript()
{
    _sceneObject.scene.registerScript(_script);
}

void ScriptComponent::_detachScript()
{
    _sceneObject.scene.detachScript(_script.id);
}

} // namespace renderboi

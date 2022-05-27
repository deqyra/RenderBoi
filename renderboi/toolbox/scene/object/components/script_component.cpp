#include "script_component.hpp"

#include "../../scene.hpp"
#include <memory>

namespace renderboi
{

ScriptComponent::ScriptComponent(Script& script) :
    _scriptPtr(nullptr),
    _script(script)
{
    _script.setSceneObject(&sceneObject);
    _registerScript();
}

ScriptComponent::ScriptComponent(ScriptPtr&& script) :
    _scriptPtr(std::move(script)),
    _script(*_scriptPtr)
{

}

Script& ScriptComponent::script()
{
    return _script;
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

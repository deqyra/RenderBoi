#include "scene_object.hpp"

#include <stdexcept>
#include <unordered_map>

#include "../scene.hpp"

namespace Renderboi
{

unsigned int SceneObject::_count = 0;

SceneObject::SceneObject(std::string name) :
    id(_count++),
    enabled(true),
    _name(name),
    _components(),
    _scene(std::shared_ptr<Scene>(nullptr)),
    transform()
{

}

SceneObject::~SceneObject()
{
    clearComponents();
    clearRenderTraitConfigs();
}

void SceneObject::_setScene(ScenePtr scene)
{
    _scene = scene;
}

void SceneObject::init()
{
    transform.setSceneObject(shared_from_this());
}

Transform SceneObject::getWorldTransform() const
{
    return _scene->getWorldTransform(id);
}

ScenePtr SceneObject::getScene() const
{
    return _scene;
}

SceneObjectPtr SceneObject::clone() const
{
    SceneObjectPtr clonedObject = std::make_shared<SceneObject>();
    clonedObject->enabled = enabled;
    clonedObject->transform = transform;

    for (const auto& component : _components)
    {
        ComponentPtr newComponent = ComponentPtr(component->clone(clonedObject));
        clonedObject->_components.push_back(newComponent);
    }

    return clonedObject;
}

void SceneObject::clearComponents()
{
    for (const auto& component : _components)
    {
        component->_release();
    }
    _components = std::vector<ComponentPtr>();
}

void SceneObject::clearRenderTraitConfigs()
{
    for (const auto& [trait, config] : _renderTraitConfigs)
    {
        config->_release();
    }
    _renderTraitConfigs = std::unordered_map<RenderTrait, RenderTraitConfigPtr>();
}

std::vector<ComponentPtr> SceneObject::getAllComponents() const
{
    return _components;
}

}//namespace Renderboi

#include "scene_object.hpp"

#include <stdexcept>

#include "scene.hpp"

unsigned int SceneObject::_count = 0;

SceneObject::SceneObject(SceneWPtr scene) :
    id(_count++),
    enabled(true),
    _scene(scene),
    _components()
{

}

glm::vec3 SceneObject::getWorldPosition()
{
    std::shared_ptr<Scene> scene = _scene.lock();
    return scene->getWorldPosition(id);
}

SceneWPtr SceneObject::getScene()
{
    return _scene;
}

void SceneObject::setScene(SceneWPtr scene)
{
    _scene = scene;
}

SceneObjectPtr SceneObject::clone()
{
    SceneObjectPtr clonedObject = SceneObjectPtr(new SceneObject(_scene));
    clonedObject->enabled = enabled;
    for (auto it = _components.begin(); it != _components.end(); it++)
    {
        ComponentPtr newComponent = ComponentPtr((*it)->clone());
        newComponent->setSceneObject(clonedObject);
        clonedObject->_components.push_back(newComponent);
    }

    return clonedObject;
}

std::vector<ComponentWPtr> SceneObject::getAllComponents()
{
    std::vector<ComponentWPtr> components;
    components.reserve(_components.size());

    // During this copy, all ComponentPtrs are copied as ComponentsWPtrs)
    std::copy(_components.begin(), _components.end(), std::back_inserter(components));

    return components;
}

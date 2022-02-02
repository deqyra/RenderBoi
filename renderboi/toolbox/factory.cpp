#include "factory.hpp"

#include <memory>

#include "renderboi/toolbox/scene/object/object_transform.hpp"
#include "scene/scene.hpp"
#include "scene/object/scene_object.hpp"

namespace Renderboi
{

ScenePtr Factory::MakeScene()
{
    ScenePtr scene = std::make_shared<Scene>();
    scene->_init();
    return scene;
}

void Factory::DestroyScene(ScenePtr scene)
{
    scene->_terminate();
}

SceneObjectPtr Factory::MakeSceneObject(const ScenePtr scene, std::string name)
{
    SceneObjectPtr sceneObject = std::make_shared<SceneObject>(scene, name);
    sceneObject->_init();
    scene->_registerObject(sceneObject);
    return sceneObject;
}

SceneObjectPtr Factory::MakeSceneObjectWithParent(const ScenePtr scene, const unsigned int parentId, std::string name)
{
    SceneObjectPtr sceneObject = std::make_shared<SceneObject>(scene, name);
    sceneObject->_init();
    scene->_registerObject(sceneObject, parentId);
    return sceneObject;
}

} // namespace Renderboi

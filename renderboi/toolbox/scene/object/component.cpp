#include "component.hpp"

#include <memory>
#include <stdexcept>

#include "scene_object.hpp"
#include "component_type.hpp"

namespace Renderboi
{

Component::Component(const SceneObjectPtr sceneObject) :
    _sceneObject(sceneObject)
{
    if (!sceneObject)
    {
        throw std::runtime_error("Component: cannot create a component from a null scene object pointer.");
    }
}

Component::~Component()
{
    _release();
}

SceneObjectPtr Component::getSceneObject() const
{
    return _sceneObject;
}

void Component::_release()
{
    _sceneObject.reset();
}

}//namespace Renderboi

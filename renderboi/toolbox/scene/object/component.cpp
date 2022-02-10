#include "component.hpp"

#include <memory>
#include <stdexcept>

#include "scene_object.hpp"
#include "component_type.hpp"

namespace Renderboi
{

Component::Component(SceneObject& sceneObject) :
    _sceneObject(sceneObject)
{

}

Component::~Component()
{

}

SceneObject& Component::sceneObject()
{
    return _sceneObject;
}

} // namespace Renderboi

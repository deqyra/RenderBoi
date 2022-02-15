#include "component.hpp"

#include <memory>
#include <stdexcept>

#include "scene_object.hpp"
#include "component_type.hpp"

namespace renderboi
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

} // namespace renderboi

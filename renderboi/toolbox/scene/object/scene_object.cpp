#include "scene_object.hpp"

#include <memory>
#include <stdexcept>
#include <unordered_map>

#include <renderboi/core/transform.hpp>

#include "../scene.hpp"
#include "component.hpp"

namespace renderboi
{

unsigned int SceneObject::_count = 0;

SceneObject::SceneObject(Scene& scene, const std::string name) :
    id(_count++),
    scene(scene),
    enabled(true),
    name(name),
    _transform(*this),
    _componentMap(*this),
    _renderTraitConfigMap(*this)
{

}

SceneObject::SceneObject(const SceneObject& other) :
    id(_count++),
    scene(other.scene),
    enabled(other.enabled),
    name(other.name),
    _transform(*this),
    _componentMap(*this),
    _renderTraitConfigMap(*this)
{

}

SceneObject::~SceneObject()
{

}

ObjectTransform& SceneObject::transform()
{
    return _transform;
}

const ObjectTransform& SceneObject::transform() const
{
    return _transform;
}

ComponentMap& SceneObject::componentMap()
{
    return _componentMap;
}

const ComponentMap& SceneObject::componentMap() const
{
    return _componentMap;
}

RenderTraitConfigMap& SceneObject::renderTraitConfigMap()
{
    return _renderTraitConfigMap;
}

const RenderTraitConfigMap& SceneObject::renderTraitConfigMap() const
{
    return _renderTraitConfigMap;
}

Transform SceneObject::worldTransform() const
{
    return scene.getWorldTransform(id);
}

SceneObjectPtr SceneObject::clone() const
{
    return SceneObjectPtr(new SceneObject(*this));
}

} // namespace renderboi

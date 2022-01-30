#include "scene_object.hpp"

#include <memory>
#include <stdexcept>
#include <unordered_map>

#include <renderboi/core/transform.hpp>

#include "../scene.hpp"
#include "component.hpp"

namespace Renderboi
{

unsigned int SceneObject::_count = 0;

SceneObject::SceneObject(const ScenePtr scene, std::string name) :
    id(_count++),
    enabled(true),
    _componentMap(nullptr),
    _renderTraitConfigMap(nullptr),
    _transform(nullptr),
    _name(name),
    _scene(scene)
{

}

SceneObject::SceneObject(const SceneObject& other) :
    id(_count++),
    enabled(other.enabled),
    _componentMap(nullptr),
    _renderTraitConfigMap(nullptr),
    _transform(nullptr),
    _name(other._name),
    _scene(other._scene)
{

}

SceneObject::~SceneObject()
{

}

void SceneObject::_release()
{
    _transform->_release();

    _componentMap->clear();
    _componentMap->_release();

    _renderTraitConfigMap->clear();
    _renderTraitConfigMap->_release();
}

void SceneObject::_initCloned(const SceneObject& other)
{
    _componentMap = ComponentMapPtr(
        new ComponentMap(shared_from_this(), *(other._componentMap))
    );

    _renderTraitConfigMap = RenderTraitConfigMapPtr(
        new RenderTraitConfigMap(shared_from_this(), *(other._renderTraitConfigMap))
    );

    _transform = ObjectTransformPtr(
        new ObjectTransform(shared_from_this(), (Transform)(*other._transform))
    );
}


void SceneObject::init()
{
    _componentMap = ComponentMapPtr(
        new ComponentMap(shared_from_this())
    );

    _renderTraitConfigMap = RenderTraitConfigMapPtr(
        new RenderTraitConfigMap(shared_from_this())
    );

    _transform = ObjectTransformPtr(
        new ObjectTransform(shared_from_this())
    );
}

ObjectTransformPtr SceneObject::transform() const
{
    return _transform;
}

ComponentMapPtr SceneObject::componentMap() const
{
    return _componentMap;
}

RenderTraitConfigMapPtr SceneObject::renderTraitConfigMap() const
{
    return _renderTraitConfigMap;
}

Transform SceneObject::worldTransform() const
{
    return _scene->getWorldTransform(id);
}

ScenePtr SceneObject::scene() const
{
    return _scene;
}

SceneObjectPtr SceneObject::clone() const
{
    return SceneObjectPtr(new SceneObject(*this));
}

}//namespace Renderboi

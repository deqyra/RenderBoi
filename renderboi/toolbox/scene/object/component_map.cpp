#include "component_map.hpp"

#include <stdexcept>
#include <string>

#include "scene_object.hpp"

namespace Renderboi
{

ComponentMap::ComponentMap(const SceneObjectPtr sceneObject) :
    _sceneObject(sceneObject),
    _singleComponentMap(),
    _multiComponentMap()
{

}

ComponentMap::ComponentMap(const SceneObjectPtr sceneObject, const ComponentMap& other) :
    _sceneObject(sceneObject),
    _singleComponentMap(),
    _multiComponentMap()
{
    for (const auto& [type, component] : other._singleComponentMap)
    {
        _singleComponentMap[type] = ComponentPtr(component->clone(_sceneObject));
    }

    for (const auto& [type, component] : other._multiComponentMap)
    {
        _multiComponentMap.insert({
            type, ComponentPtr(component->clone(_sceneObject))
        });
    }
}

void ComponentMap::_release()
{
    _sceneObject.reset();
}

SceneObjectPtr ComponentMap::sceneObject()
{
    return _sceneObject;
}

void ComponentMap::clear()
{
    for (const auto& [type, component] : _singleComponentMap)
    {
        component->_release();
    }
    _singleComponentMap.clear();

    for (const auto& [type, component] : _multiComponentMap)
    {
        component->_release();
    }
    _multiComponentMap.clear();
}

}
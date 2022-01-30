#include "component_map.hpp"

#include <stdexcept>
#include <string>

#include "scene_object.hpp"

namespace renderboi
{

ComponentMap::ComponentMap(SceneObject& sceneObject) :
    _sceneObject(sceneObject),
    _singleComponentMap(),
    _multiComponentMap()
{

}

ComponentMap::ComponentMap(SceneObject& sceneObject, const ComponentMap& other) :
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

SceneObject& ComponentMap::sceneObject()
{
    return _sceneObject;
}

void ComponentMap::clear()
{
    _singleComponentMap.clear();
    _multiComponentMap.clear();
}

}
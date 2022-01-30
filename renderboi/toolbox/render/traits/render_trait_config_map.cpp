#include "render_trait_config_map.hpp"
#include "renderboi/toolbox/render/traits/render_trait_config.hpp"

namespace Renderboi
{

RenderTraitConfigMap::RenderTraitConfigMap(const SceneObjectPtr sceneObject) :
    _sceneObject(sceneObject),
    _map()
{

}

RenderTraitConfigMap::RenderTraitConfigMap(const SceneObjectPtr sceneObject, const RenderTraitConfigMap& other) :
    _sceneObject(sceneObject),
    _map()
{
    for (const auto& [trait, config] : other._map)
    {
        _map[trait] = RenderTraitConfigPtr(config->clone(_sceneObject));
    }
}

void RenderTraitConfigMap::_release()
{
    clear();
    _sceneObject.reset();
}

SceneObjectPtr RenderTraitConfigMap::sceneObject()
{
    return _sceneObject;
}

RenderTraitConfigPtr RenderTraitConfigMap::getConfigForTrait(const RenderTrait trait)
{
    return _map[trait];
}

bool RenderTraitConfigMap::hasConfigForTrait(const RenderTrait trait)
{
    return _map.contains(trait);
}

void RenderTraitConfigMap::setConfigForTrait(const RenderTrait trait, const RenderTraitConfigPtr config)
{
    auto it = _map.find(trait);

    if (it != _map.end())
    {
        it->second->_release();
    }

    if (config)
    {
        it->second = config;
    }
    else
    {
        _map.erase(it);
    }
}

void RenderTraitConfigMap::clearConfigForTrait(const RenderTrait trait)
{
    auto it = _map.find(trait);

    if (it != _map.end())
    {
        (it->second)->_release();
    }

    _map.erase(it);
}

void RenderTraitConfigMap::clear()
{
    for (const auto& [trait, config] : _map)
    {
        config->_release();
    }
    _map.clear();
}

}
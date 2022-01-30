#include "render_trait_config.hpp"
#include "render_trait_config_map.hpp"

namespace renderboi
{

RenderTraitConfigMap::RenderTraitConfigMap(SceneObject& sceneObject) :
    _sceneObject(sceneObject),
    _map()
{

}

RenderTraitConfigMap::RenderTraitConfigMap(SceneObject& sceneObject, const RenderTraitConfigMap& other) :
    _sceneObject(sceneObject),
    _map()
{
    for (const auto& [trait, config] : other._map)
    {
        _map[trait] = RenderTraitConfigPtr(config->clone(_sceneObject));
    }
}

SceneObject& RenderTraitConfigMap::sceneObject()
{
    return _sceneObject;
}

RenderTraitConfig& RenderTraitConfigMap::getConfigForTrait(const RenderTrait trait) const
{
    return *_map.at(trait);
}

bool RenderTraitConfigMap::hasConfigForTrait(const RenderTrait trait) const
{
    return _map.contains(trait);
}

void RenderTraitConfigMap::clearConfigForTrait(const RenderTrait trait)
{
    _map.erase(trait);
}

void RenderTraitConfigMap::clear()
{
    _map.clear();
}

}
#include "render_trait_config.hpp"
#include "render_trait_config_map.hpp"

namespace rb {

RenderTraitConfigMap::RenderTraitConfigMap(Object& Object) :
    _Object(Object),
    _map() {

}

RenderTraitConfigMap::RenderTraitConfigMap(Object& Object, const RenderTraitConfigMap& other) :
    _Object(Object),
    _map() {
    for (const auto& [trait, config] : other._map)
    {
        _map[trait] = RenderTraitConfigPtr(config->clone(_Object));
    }
}

Object& RenderTraitConfigMap::Object() {
    return _Object;
}

RenderTraitConfig& RenderTraitConfigMap::getConfigForTrait(const RenderTrait trait) const
{
    return *_map.at(trait);
}

bool RenderTraitConfigMap::hasConfigForTrait(const RenderTrait trait) const
{
    return _map.contains(trait);
}

void RenderTraitConfigMap::clearConfigForTrait(const RenderTrait trait) {
    _map.erase(trait);
}

void RenderTraitConfigMap::clear() {
    _map.clear();
}

}
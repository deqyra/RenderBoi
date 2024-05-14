#include "render_trait_config.hpp"
#include "renderboi/toolbox/scene/object/object_world_transform.hpp"

namespace rb {

RenderTraitConfig::RenderTraitConfig(Object& Object)
    : _Object(Object) {}

RenderTraitConfig::~RenderTraitConfig() {}

Object& RenderTraitConfig::Object() {
    return _Object;
}

} // namespace rb

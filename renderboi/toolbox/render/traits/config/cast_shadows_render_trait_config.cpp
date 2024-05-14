#include "cast_shadows_render_trait_config.hpp"

#include <stdexcept>

#include "../../../scene/object.hpp"
#include "../../../scene/object/component_type.hpp"
#include "../render_trait_config.hpp"

namespace rb {

CastShadowsRenderTraitConfig::CastShadowsRenderTraitConfig(Object& parentObject) :
    RenderTraitConfig(parentObject),
    _mesh(parentObject.componentMap().getComponent<ComponentType::Mesh>()) {
    if (!parentObject.componentMap().hasComponent<ComponentType::Mesh>())
    {
        throw std::runtime_error("CastShadowsConfig: cannot be instantiated on an object without a MeshComponent");
    }
}

CastShadowsRenderTraitConfig* CastShadowsRenderTraitConfig::clone(Object& newParent) const {
    return new CastShadowsRenderTraitConfig(newParent);
}

} // namespace rb

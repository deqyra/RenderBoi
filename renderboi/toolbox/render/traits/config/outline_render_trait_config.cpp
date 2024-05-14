#include "outline_render_trait_config.hpp"

#include <stdexcept>

#include "../../../scene/object.hpp"
#include "../../../scene/object/component_type.hpp"
#include "../render_trait_config.hpp"

namespace rb {

OutlineRenderTraitConfig::OutlineRenderTraitConfig(Object& parentObject) :
    RenderTraitConfig(parentObject),
    _mesh(parentObject.componentMap().getComponent<ComponentType::Mesh>()) {
    if (!parentObject.componentMap().hasComponent<ComponentType::Mesh>())
    {
        throw std::runtime_error("OutlineConfig: cannot be instantiated on an object without a MeshComponent");
    }
}

OutlineRenderTraitConfig* OutlineRenderTraitConfig::clone(Object& newParent) const {
    return new OutlineRenderTraitConfig(newParent);
}

} // namespace rb

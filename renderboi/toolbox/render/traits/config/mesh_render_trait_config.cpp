#include "mesh_render_trait_config.hpp"

#include <stdexcept>

#include "../../../scene/object.hpp"
#include "../../../scene/object/component_type.hpp"
#include "../render_trait_config.hpp"

namespace rb {

MeshRenderTraitConfig::MeshRenderTraitConfig(Object& parentObject) :
    RenderTraitConfig(parentObject),
    _mesh(parentObject.componentMap().getComponent<ComponentType::Mesh>()) {
    if (!parentObject.componentMap().hasComponent<ComponentType::Mesh>())
    {
        throw std::runtime_error("MeshConfig: cannot be instantiated on an object without a MeshComponent");
    }
}

MeshRenderTraitConfig* MeshRenderTraitConfig::clone(Object& newParent) const {
    return new MeshRenderTraitConfig(newParent);
}

} // namespace rb

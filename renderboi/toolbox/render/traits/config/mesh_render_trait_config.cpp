#include "mesh_render_trait_config.hpp"

#include <stdexcept>

#include "../../../scene/object/scene_object.hpp"
#include "../../../scene/object/component_type.hpp"
#include "../render_trait_config.hpp"

namespace renderboi
{

MeshRenderTraitConfig::MeshRenderTraitConfig(SceneObject& parentSceneObject) :
    RenderTraitConfig(parentSceneObject),
    _mesh(parentSceneObject.componentMap().getComponent<ComponentType::Mesh>())
{
    if (!parentSceneObject.componentMap().hasComponent<ComponentType::Mesh>())
    {
        throw std::runtime_error("MeshConfig: cannot be instantiated on an object without a MeshComponent");
    }
}

MeshRenderTraitConfig* MeshRenderTraitConfig::clone(SceneObject& newParent) const
{
    return new MeshRenderTraitConfig(newParent);
}

} // namespace renderboi

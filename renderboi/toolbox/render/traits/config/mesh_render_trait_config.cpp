#include "mesh_render_trait_config.hpp"

#include <stdexcept>

#include "../../../scene/object/scene_object.hpp"
#include "../../../scene/object/component_type.hpp"
#include "../render_trait_config.hpp"

namespace Renderboi
{

MeshRenderTraitConfig::MeshRenderTraitConfig(const SceneObjectPtr parentSceneObject) :
    RenderTraitConfig(parentSceneObject),
    _mesh(parentSceneObject->componentMap()->getComponent<ComponentType::Mesh>())
{
    if (!_mesh)
    {
        throw std::runtime_error("MeshConfig: cannot be instantiated on an object without a MeshComponent");
    }
}

MeshRenderTraitConfig* MeshRenderTraitConfig::clone(const SceneObjectPtr newParent) const
{
    return nullptr;
}

void MeshRenderTraitConfig::_release()
{
    _mesh.reset();
    RenderTraitConfig::_release();
}

} // namespace Renderboi

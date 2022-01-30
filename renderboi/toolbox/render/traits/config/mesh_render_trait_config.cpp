#include "mesh_render_trait_config.hpp"

#include <stdexcept>

#include "../render_trait_config.hpp"
#include "../../../scene/object/scene_object.hpp"
#include "../../../scene/object/component_type.hpp"

namespace Renderboi
{

MeshRenderTraitConfig::MeshRenderTraitConfig(SceneObjectPtr parentSceneObject) :
    RenderTraitConfig(parentSceneObject),
    _mesh(parentSceneObject->componentMap()->getComponent<ComponentType::Mesh>())
{
    if (!_mesh)
    {
        throw std::runtime_error("MeshConfig: cannot be instantiated on an object without a MeshComponent");
    }
}

void MeshRenderTraitConfig::_release()
{
    _mesh.reset();
    RenderTraitConfig::_release();
}

} // namespace Renderboi

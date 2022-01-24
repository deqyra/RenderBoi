#include "mesh_render_trait_config.hpp"

#include <stdexcept>

#include "../render_trait_config.hpp"
#include "../../../scene/object/scene_object.hpp"

namespace Renderboi
{

MeshRenderTraitConfig::MeshRenderTraitConfig(SceneObjectPtr parentSceneObject) :
    RenderTraitConfig(parentSceneObject),
    _mesh(parentSceneObject->getComponent<MeshComponent>())
{
    if (!_mesh)
    {
        throw std::runtime_error("MeshConfig: cannot be instantiated on an object without a MeshComponent");
    }
}

void MeshRenderTraitConfig::_release()
{
    _mesh = nullptr;
    RenderTraitConfig::_release();
}

} // namespace Renderboi

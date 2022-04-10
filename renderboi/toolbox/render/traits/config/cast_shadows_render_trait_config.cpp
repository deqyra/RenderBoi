#include "cast_shadows_render_trait_config.hpp"

#include <stdexcept>

#include "../../../scene/object/scene_object.hpp"
#include "../../../scene/object/component_type.hpp"
#include "../render_trait_config.hpp"

namespace renderboi
{

CastShadowsRenderTraitConfig::CastShadowsRenderTraitConfig(SceneObject& parentSceneObject) :
    RenderTraitConfig(parentSceneObject),
    _mesh(parentSceneObject.componentMap().getComponent<ComponentType::Mesh>())
{
    if (!parentSceneObject.componentMap().hasComponent<ComponentType::Mesh>())
    {
        throw std::runtime_error("CastShadowsConfig: cannot be instantiated on an object without a MeshComponent");
    }
}

CastShadowsRenderTraitConfig* CastShadowsRenderTraitConfig::clone(SceneObject& newParent) const
{
    return new CastShadowsRenderTraitConfig(newParent);
}

} // namespace renderboi

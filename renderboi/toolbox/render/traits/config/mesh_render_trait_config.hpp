#ifndef RENDERBOI__TOOLBOX__RENDER__TRAITS__CONFIG__MESH_RENDER_TRAIT_CONFIG_HPP
#define RENDERBOI__TOOLBOX__RENDER__TRAITS__CONFIG__MESH_RENDER_TRAIT_CONFIG_HPP

#include "../render_trait_config.hpp"

#include <memory>

#include "renderboi/toolbox/scene/object/components/mesh_component.hpp"

namespace Renderboi
{

/// @brief Concrete trait renderer config class, meant to be used by the
/// MeshTraitRenderer class.
class MeshRenderTraitConfig : public RenderTraitConfig
{
protected:
    /// @brief Release held references to owned resources.
    virtual void _release() override;

    /// @brief Reference to the MeshComponent that is to be used for rendering.
    std::shared_ptr<MeshComponent> _mesh;

public:
    /// @param parentSceneObject Reference to the parent scene object.
    MeshRenderTraitConfig(SceneObjectPtr parentSceneObject);

    /// @brief Constant reference to the MeshComponent which is to be used for
    /// rendering.
    const std::shared_ptr<MeshComponent>& Mesh = _mesh;
};

} // namespace Renderboi

#endif//RENDERBOI__TOOLBOX__RENDER__TRAITS__CONFIG__MESH_RENDER_TRAIT_CONFIG_HPP
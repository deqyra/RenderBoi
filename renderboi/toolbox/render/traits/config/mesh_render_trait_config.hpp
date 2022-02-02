#ifndef RENDERBOI__TOOLBOX__RENDER__TRAITS__CONFIG__MESH_RENDER_TRAIT_CONFIG_HPP
#define RENDERBOI__TOOLBOX__RENDER__TRAITS__CONFIG__MESH_RENDER_TRAIT_CONFIG_HPP


#include <memory>

#include "../../../scene/object/components/mesh_component.hpp"
#include "../../../scene/object/scene_object.hpp"
#include "../render_trait_config.hpp"

namespace Renderboi
{

/// @brief POD struct meant to represent a configuration to be used by the
/// MeshTraitRenderer class.
class MeshRenderTraitConfig : public RenderTraitConfig
{
protected:
    /// @brief Release held references to shared resources.
    virtual void _release() override;

    /// @brief Reference to the MeshComponent that is to be used for rendering.
    std::shared_ptr<MeshComponent> _mesh;

public:
    /// @param parentSceneObject Reference to the parent scene object.
    MeshRenderTraitConfig(const SceneObjectPtr parentSceneObject);

    /// @brief Constant reference to the MeshComponent which is to be used for
    /// rendering.
    const std::shared_ptr<MeshComponent>& Mesh = _mesh;

    /////////////////////////////////////////////////
    ///                                           ///
    /// Methods overridden from RenderTraitConfig ///
    ///                                           ///
    /////////////////////////////////////////////////

    /// @brief Get a raw pointer to a new render trait config instance cloned 
    /// from this one. Ownership and responsibility for the allocated 
    /// resources are fully transferred to the caller.
    ///
    /// @param newParent Pointer the scene object which will be parent to
    /// the cloned render trait config instance.
    ///
    /// @return A raw pointer to the render trait config instance cloned from 
    /// this one.
    virtual MeshRenderTraitConfig* clone(const SceneObjectPtr newParent) const override;
};

template<>
struct RenderTraitMeta<RenderTrait::Mesh>
{
    struct Renderer;

    struct Config
    {
        using type = MeshRenderTraitConfig;
    };
};

} // namespace Renderboi

#endif//RENDERBOI__TOOLBOX__RENDER__TRAITS__CONFIG__MESH_RENDER_TRAIT_CONFIG_HPP
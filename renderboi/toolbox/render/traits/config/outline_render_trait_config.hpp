#ifndef RENDERBOI__TOOLBOX__RENDER__TRAITS__CONFIG__OUTLINE_RENDER_TRAIT_CONFIG_HPP
#define RENDERBOI__TOOLBOX__RENDER__TRAITS__CONFIG__OUTLINE_RENDER_TRAIT_CONFIG_HPP


#include <memory>

#include "../../../scene/object/components/mesh_component.hpp"
#include "../../../scene/object/scene_object.hpp"
#include "../render_trait_config.hpp"

namespace renderboi
{

/// @brief Class meant to represent a configuration to be used by the
/// OutlineTraitRenderer class.
class OutlineRenderTraitConfig : public RenderTraitConfig
{
private:
    /// @brief Reference to the MeshComponent that is to be used for rendering.
    MeshComponent& _mesh;

public:
    /// @param parentSceneObject Reference to the parent scene object.
    OutlineRenderTraitConfig(SceneObject& parentSceneObject);

    /////////////////////////////////////////////////
    ///                                           ///
    /// Methods overridden from RenderTraitConfig ///
    ///                                           ///
    /////////////////////////////////////////////////

    /// @brief Get a raw pointer to a new render trait config instance cloned 
    /// from this one. Ownership and responsibility for the allocated 
    /// resources are fully transferred to the caller.
    ///
    /// @param newParent Reference the scene object which will be parent to
    /// the cloned render trait config instance.
    ///
    /// @return A raw pointer to the render trait config instance cloned from 
    /// this one.
    virtual OutlineRenderTraitConfig* clone(SceneObject& newParent) const override;
};

template<>
struct RenderTraitMeta<RenderTrait::Outline>
{
    struct Renderer;

    struct Config
    {
        using type = OutlineRenderTraitConfig;
    };
};

} // namespace renderboi

#endif//RENDERBOI__TOOLBOX__RENDER__TRAITS__CONFIG__OUTLINE_RENDER_TRAIT_CONFIG_HPP
#ifndef RENDERBOI_TOOLBOX_RENDER_TRAITS_CONFIG_OUTLINE_RENDER_TRAIT_CONFIG_HPP
#define RENDERBOI_TOOLBOX_RENDER_TRAITS_CONFIG_OUTLINE_RENDER_TRAIT_CONFIG_HPP

#include "../../../scene/object/components/mesh_component.hpp"
#include "../../../scene/object.hpp"
#include "../render_trait_config.hpp"

namespace rb {

/// @brief Class meant to represent a configuration to be used by the
/// OutlineTraitRenderer class
class OutlineRenderTraitConfig : public RenderTraitConfig {
private:
    /// @brief Reference to the MeshComponent that is to be used for rendering
    MeshComponent& _mesh;

public:
    /// @param parentObject Reference to the parent scene object
    OutlineRenderTraitConfig(Object& parentObject);

    /////////////////////////////////////////////////
    ///                                           ///
    /// Methods overridden from RenderTraitConfig ///
    ///                                           ///
    /////////////////////////////////////////////////

    /// @brief Get a raw pointer to a new render trait config instance cloned 
    /// from this one Ownership and responsibility for the allocated 
    /// resources are fully transferred to the caller
    ///
    /// @param newParent Reference the scene object which will be parent to
    /// the cloned render trait config instance
    ///
    /// @return A raw pointer to the render trait config instance cloned from 
    /// this one
    virtual OutlineRenderTraitConfig* clone(Object& newParent) const override;
};

template<>
struct RenderTraitMeta<RenderTrait::Outline> {
    struct Renderer;

    struct Config
    {
        using type = OutlineRenderTraitConfig;
    };
};

} // namespace rb

#endif//RENDERBOI_TOOLBOX_RENDER_TRAITS_CONFIG_OUTLINE_RENDER_TRAIT_CONFIG_HPP
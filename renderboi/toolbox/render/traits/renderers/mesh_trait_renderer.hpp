#ifndef RENDERBOI_TOOLBOX_RENDER_TRAITS_RENDERERS_MESH_TRAIT_RENDERER_HPP
#define RENDERBOI_TOOLBOX_RENDER_TRAITS_RENDERERS_MESH_TRAIT_RENDERER_HPP

#include "../trait_renderer.hpp"
#include "../config/mesh_render_trait_config.hpp"

namespace rb {

/// @brief Concrete trait renderer for meshes
class MeshTraitRenderer : public TraitRenderer {
public:
    /////////////////////////////////////////////
    ///                                       ///
    /// Methods overridden from TraitRenderer ///
    ///                                       ///
    /////////////////////////////////////////////

    /// @brief Render a mesh
    virtual void render(const RenderTraitConfig& config) const override;
};

struct RenderTraitMeta<RenderTrait::Mesh>::Renderer {
    using type = MeshTraitRenderer;
};

using MeshTraitRendererPtr = std::unique_ptr<MeshTraitRenderer>;

} // namespace rb

#endif//RENDERBOI_TOOLBOX_RENDER_TRAITS_RENDERERS_MESH_TRAIT_RENDERER_HPP
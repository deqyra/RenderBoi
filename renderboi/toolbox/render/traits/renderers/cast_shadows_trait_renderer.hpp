#ifndef RENDERBOI_TOOLBOX_RENDER_TRAITS_RENDERERS_CAST_SHADOWS_TRAIT_RENDERER_HPP
#define RENDERBOI_TOOLBOX_RENDER_TRAITS_RENDERERS_CAST_SHADOWS_TRAIT_RENDERER_HPP

#include "../trait_renderer.hpp"
#include "../config/cast_shadows_render_trait_config.hpp"

namespace rb {

/// @brief Concrete trait renderer for meshes
class CastShadowsTraitRenderer : public TraitRenderer {
public:
    /////////////////////////////////////////////
    ///                                       ///
    /// Methods overridden from TraitRenderer ///
    ///                                       ///
    /////////////////////////////////////////////

    /// @brief Render a cast shadow
    virtual void render(const RenderTraitConfig& config) const override;
};

struct RenderTraitMeta<RenderTrait::CastShadows>::Renderer {
    using type = CastShadowsTraitRenderer;
};

using CastShadowsTraitRendererPtr = std::unique_ptr<CastShadowsTraitRenderer>;

} // namespace rb

#endif//RENDERBOI_TOOLBOX_RENDER_TRAITS_RENDERERS_CAST_SHADOWS_TRAIT_RENDERER_HPP
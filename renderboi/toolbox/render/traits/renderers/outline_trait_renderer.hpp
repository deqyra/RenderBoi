#ifndef RENDERBOI_TOOLBOX_RENDER_TRAITS_RENDERERS_OUTLINE_TRAIT_RENDERER_HPP
#define RENDERBOI_TOOLBOX_RENDER_TRAITS_RENDERERS_OUTLINE_TRAIT_RENDERER_HPP

#include "../trait_renderer.hpp"
#include "../config/outline_render_trait_config.hpp"

namespace rb {

/// @brief Concrete trait renderer for outlines
class OutlineTraitRenderer : public TraitRenderer {
public:
    /////////////////////////////////////////////
    ///                                       ///
    /// Methods overridden from TraitRenderer ///
    ///                                       ///
    /////////////////////////////////////////////

    /// @brief Render an outline
    virtual void render(const RenderTraitConfig& config) const override;
};

struct RenderTraitMeta<RenderTrait::Outline>::Renderer {
    using type = OutlineTraitRenderer;
};

using OutlineTraitRendererPtr = std::unique_ptr<OutlineTraitRenderer>;

} // namespace rb

#endif//RENDERBOI_TOOLBOX_RENDER_TRAITS_RENDERERS_OUTLINE_TRAIT_RENDERER_HPP
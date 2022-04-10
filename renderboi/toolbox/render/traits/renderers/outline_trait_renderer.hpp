#ifndef RENDERBOI__TOOLBOX__RENDER__TRAITS__RENDERERS__OUTLINE_TRAIT_RENDERER_HPP
#define RENDERBOI__TOOLBOX__RENDER__TRAITS__RENDERERS__OUTLINE_TRAIT_RENDERER_HPP

#include "../trait_renderer.hpp"
#include "../config/outline_render_trait_config.hpp"

namespace renderboi
{

/// @brief Concrete trait renderer for outlines.
class OutlineTraitRenderer : public TraitRenderer
{
public:
    /////////////////////////////////////////////
    ///                                       ///
    /// Methods overridden from TraitRenderer ///
    ///                                       ///
    /////////////////////////////////////////////

    /// @brief Render an outline
    virtual void render(const RenderTraitConfig& config) const override;
};

struct RenderTraitMeta<RenderTrait::Outline>::Renderer
{
    using type = OutlineTraitRenderer;
};

using OutlineTraitRendererPtr = std::unique_ptr<OutlineTraitRenderer>;

} // namespace renderboi

#endif//RENDERBOI__TOOLBOX__RENDER__TRAITS__RENDERERS__OUTLINE_TRAIT_RENDERER_HPP
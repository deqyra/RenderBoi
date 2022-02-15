#ifndef RENDERBOI__TOOLBOX__SCENE__RENDER__COMPOSITE_RENDERER_HPP
#define RENDERBOI__TOOLBOX__SCENE__RENDER__COMPOSITE_RENDERER_HPP

#include <unordered_map>

#include "traits/render_trait.hpp"
#include "traits/trait_renderer.hpp"

namespace renderboi
{

/// @brief Component in charge of rendering all aspects of an object.
class CompositeRenderer
{
private:
    std::unordered_map<RenderTrait, TraitRendererPtr> _renderers;

public:
    /// @brief Render something?
    void render() const;
};

} // namespace renderboi

#endif//RENDERBOI__TOOLBOX__SCENE__RENDER__COMPOSITE_RENDERER_HPP
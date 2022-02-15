#ifndef RENDERBOI__TOOLBOX__RENDER__TRAIT_RENDERER_HPP
#define RENDERBOI__TOOLBOX__RENDER__TRAIT_RENDERER_HPP

#include <memory>

#include "render_trait.hpp"

namespace renderboi
{

/// @brief Abstract class for a renderer which focuses on rendering one single
/// aspect (trait) to be rendered.
class TraitRenderer
{
public:
    /// @brief Render something?
    virtual void render() const = 0;
};

using TraitRendererPtr = std::unique_ptr<TraitRenderer>;

} // namespace renderboi

#endif//RENDERBOI__TOOLBOX__RENDER__TRAIT_RENDERER_HPP
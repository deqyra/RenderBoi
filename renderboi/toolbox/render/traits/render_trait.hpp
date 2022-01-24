#ifndef RENDERBOI__TOOLBOX__SCENE__RENDERER__RENDER_TRAIT_HPP
#define RENDERBOI__TOOLBOX__SCENE__RENDERER__RENDER_TRAIT_HPP

namespace Renderboi
{

/// @brief Collection of literals describing different aspects of an object to
/// be rendered.
enum class RenderTrait
{
    Mesh,
    Outline,
    CastShadows
};

} // namespace Renderboi

#endif//RENDERBOI__TOOLBOX__SCENE__RENDERER__RENDER_TRAIT_HPP
#ifndef RENDERBOI__TOOLBOX__SCENE__RENDERER__RENDER_TRAIT_HPP
#define RENDERBOI__TOOLBOX__SCENE__RENDERER__RENDER_TRAIT_HPP

namespace Renderboi
{

/*           ╔════════════╗
 *           ║   README   ║
 *           ╚════════════╝
 * 
 *     WRITING A NEW RENDER TRAIT
 * ==================================
 *
 * - Add literal to RenderTrait
 * - Write renderer for new trait, inheriting from TraitRenderer
 * - Write render config for new trait, inheriting from RenderTraitConfig
 * - Below declaration of new renderer, specialize RenderTraitMeta like so:
 *
 *      template<>
 *      struct RenderTraitMeta<RenderTrait::MyNewTrait>
 *      {
 *          using RendererType = MyNewTraitRenderer;
 *      };
 *
 * - Below declaration of new config, specialize RenderTraitMeta like so:
 *
 *      template<>
 *      struct RenderTraitMeta<RenderTrait::MyNewTrait>
 *      {
 *          using RendererType = MyNewRenderTraitConfig;
 *      };
 *
 */

/// @brief Collection of literals describing different aspects of an object to
/// be rendered.
enum class RenderTrait
{
    Mesh,
    Outline,
    CastShadows
};

/// @brief Compile-time metadata about render traits and their associated render
/// classes and configurations.
template<RenderTrait T>
struct RenderTraitMeta
{
};

} // namespace Renderboi

#endif//RENDERBOI__TOOLBOX__SCENE__RENDERER__RENDER_TRAIT_HPP
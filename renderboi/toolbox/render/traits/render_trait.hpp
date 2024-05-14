#ifndef RENDERBOI_TOOLBOX_SCENE_RENDERER_RENDER_TRAIT_HPP
#define RENDERBOI_TOOLBOX_SCENE_RENDERER_RENDER_TRAIT_HPP

#include <array>

namespace rb {

/*           ╔════════════╗
 *           ║   README   ║
 *           ╚════════════╝
 * 
 *     WRITING A NEW RENDER TRAIT
 * ==================================
 *
 * - Add literal to RenderTrait enum below
 * - Add literal to OrderedRenderTraits array
 * - Write renderer for new trait, inheriting from TraitRenderer
 * - Write render config for new trait, inheriting from RenderTraitConfig
 * - Below declaration of new config, specialize RenderTraitMeta like so:
 *
 *      template<>
 *      struct RenderTraitMeta<RenderTrait::MyNewTrait>
 *      {
 *          struct Renderer;    // FORWARD-DECLARE ONLY
 *
 *          struct Config
 *          {
 *              using type = MyNewRenderTraitConfig;
 *          };
 *      };
 *
 * - Below declaration of new renderer (which includes config header by 
 *   necessity), define RenderTraitMeta<RenderTrait::MyNewTrait>::Renderer
 *   like so:
 *
 *      struct RenderTraitMeta<RenderTrait::MyNewTrait>::Renderer
 *      {
 *          using type = MyNewTraitRenderer;
 *      };
 *
 */

/// @brief Literals describing different aspects of an object to
/// be rendered
enum class RenderTrait {
    /// @brief Literal representing a mesh as the trait to be rendered
    Mesh,
    /// @brief Literal telling that an outline must be drawn
    Outline,
    /// @brief Literal indicating that the object will cast shadows
    CastShadows
};

/// @brief Compile-time metadata about render traits and their associated render
/// classes and configurations
template<RenderTrait T>
struct RenderTraitMeta {
    /// @brief Use ::type to get the concrete type of the trait renderer for T
    struct Renderer {};

    /// @brief Use ::type to get the type of config expected by the concrete 
    /// trait renderer for T
    struct Config {};
};

static inline constexpr std::array<RenderTrait, 3> OrderedRenderTraits = {
    RenderTrait::Mesh,
    RenderTrait::Outline,
    RenderTrait::CastShadows
};

} // namespace rb

#endif//RENDERBOI_TOOLBOX_SCENE_RENDERER_RENDER_TRAIT_HPP
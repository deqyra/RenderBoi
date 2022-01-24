#ifndef RENDERBOI__TOOLBOX__SCENE__RENDERER__RENDER_TRAIT_CONFIG_HPP
#define RENDERBOI__TOOLBOX__SCENE__RENDERER__RENDER_TRAIT_CONFIG_HPP

#include "render_trait.hpp"

#include "config/mesh_render_trait_config.hpp"

#include "renderers/mesh_trait_renderer.hpp"

namespace Renderboi
{

/// @brief Compile-time metadata about render traits and their associated render
/// classes and configurations.
template<RenderTrait T>
struct RenderTraitMeta
{
};

template<>
struct RenderTraitMeta<RenderTrait::Mesh>
{
    using ConfigType = MeshRenderTraitConfig;
    using RendererType = MeshTraitRenderer;
};

} // namespace Renderboi

#endif//RENDERBOI__TOOLBOX__SCENE__RENDERER__RENDER_TRAIT_CONFIG_HPP
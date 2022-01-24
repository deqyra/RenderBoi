#ifndef RENDERBOI__TOOLBOX__SCENE__RENDER__MESH_COMPOSITE_RENDERER_HPP
#define RENDERBOI__TOOLBOX__SCENE__RENDER__MESH_COMPOSITE_RENDERER_HPP

#include <unordered_map>

#include "traits/render_trait.hpp"
#include "traits/trait_renderer.hpp"

namespace Renderboi
{

/// @brief Component in charge of rendering all aspects of a single mesh.
class MeshCompositeRenderer
{
private:
    std::unordered_map<RenderTrait, TraitRendererPtr> _renderers;

public:
    /// @brief Render something?
    void render();
};

} // namespace Renderboi

#endif//RENDERBOI__TOOLBOX__SCENE__RENDER__MESH_COMPOSITE_RENDERER_HPP
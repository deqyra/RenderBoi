#ifndef RENDERBOI__TOOLBOX__RENDER__TRAITS__RENDERERS__MESH_TRAIT_RENDERER_HPP
#define RENDERBOI__TOOLBOX__RENDER__TRAITS__RENDERERS__MESH_TRAIT_RENDERER_HPP

#include "../trait_renderer.hpp"

namespace Renderboi
{

/// @brief Concrete trait renderer for meshes.
class MeshTraitRenderer : public TraitRenderer
{
public:
    /// @brief Render a mesh?
    virtual void render();
};

using MeshTraitRendererPtr = std::shared_ptr<MeshTraitRenderer>;

} // namespace Renderboi

#endif//RENDERBOI__TOOLBOX__RENDER__TRAITS__RENDERERS__MESH_TRAIT_RENDERER_HPP
#ifndef RENDERBOI__TOOLBOX__SCENE__RENDER__COMPOSITE_RENDERER_HPP
#define RENDERBOI__TOOLBOX__SCENE__RENDER__COMPOSITE_RENDERER_HPP

#include <array>
#include <type_traits>
#include <unordered_map>

#include "../scene/object/components/mesh_component.hpp"
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
    CompositeRenderer();

    /// @brief Render something?
    void render(const SceneObject& object) const;
};

} // namespace renderboi

#endif//RENDERBOI__TOOLBOX__SCENE__RENDER__COMPOSITE_RENDERER_HPP
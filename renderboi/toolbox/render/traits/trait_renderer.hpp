#ifndef RENDERBOI_TOOLBOX_RENDER_TRAIT_RENDERER_HPP
#define RENDERBOI_TOOLBOX_RENDER_TRAIT_RENDERER_HPP

#include <memory>

#include "../../scene/object/components/mesh_component.hpp"
#include "render_trait.hpp"
#include "render_trait_config.hpp"

namespace rb {

/// @brief Abstract class for a renderer which focuses on rendering one single
/// aspect (trait) to be rendered
class TraitRenderer {
public:
    /// @brief Render something
    virtual void render(const RenderTraitConfig& config) const = 0;
};

using TraitRendererPtr = std::unique_ptr<TraitRenderer>;

} // namespace rb

#endif//RENDERBOI_TOOLBOX_RENDER_TRAIT_RENDERER_HPP
#ifndef RENDERBOI_TOOLBOX_RENDER_TRAITS_RENDER_TRAIT_CONFIG_HPP
#define RENDERBOI_TOOLBOX_RENDER_TRAITS_RENDER_TRAIT_CONFIG_HPP

#include <memory>

#include "render_trait.hpp"
#include "../../scene/object.hpp"

namespace rb {

class RenderTraitConfigMap;

/// @brief Abstract configuration class for a render trait To be inherited from
/// when specializing config for specific render traits
class RenderTraitConfig {
friend RenderTraitConfigMap;

private:
    /// @brief Reference to the parent scene object of the trait config
    Object& _Object;

public:
    /// @param parentObject Reference to the parent scene object
    RenderTraitConfig(Object& parentObject);

    virtual ~RenderTraitConfig();

    /// @brief Get a reference to the parent scene object of this instance
    ///
    /// @return A reference to the parent scene object of this instance
    Object& Object();

    /// @brief Get a raw pointer to a new render trait config instance cloned 
    /// from this one Ownership and responsibility for the allocated 
    /// resources are fully transferred to the caller
    ///
    /// @param newParent Reference to the scene object which will be parent to
    /// the cloned render trait config instance
    ///
    /// @return A raw pointer to the render trait config instance cloned from 
    /// this one
    virtual RenderTraitConfig* clone(Object& newParent) const = 0;
};

using RenderTraitConfigPtr = std::unique_ptr<RenderTraitConfig>;

} // namespace rb

#endif//RENDERBOI_TOOLBOX_RENDER_TRAITS_RENDER_TRAIT_CONFIG_HPP
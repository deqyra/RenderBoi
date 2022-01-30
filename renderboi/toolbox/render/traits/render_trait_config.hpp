#ifndef RENDERBOI__TOOLBOX__RENDER__TRAITS__RENDER_TRAIT_CONFIG_HPP
#define RENDERBOI__TOOLBOX__RENDER__TRAITS__RENDER_TRAIT_CONFIG_HPP

#include <memory>

#include "renderboi/toolbox/render/traits/render_trait.hpp"

namespace Renderboi
{

class SceneObject;
using SceneObjectPtr = std::shared_ptr<SceneObject>;

class RenderTraitConfigMap;

/// @brief Abstract configuration class for a render trait. To be inherited from
/// when specializing config for specific render traits.
class RenderTraitConfig
{
friend RenderTraitConfigMap;

private:
    /// @brief Reference to the parent scene object of the trait config.
    SceneObjectPtr _sceneObject;

protected:
    /// @brief Release held references to shared resources.
    virtual void _release();

public:
    /// @param parentSceneObject Reference to the parent scene object.
    RenderTraitConfig(SceneObjectPtr parentSceneObject);

    virtual ~RenderTraitConfig();

    /// @brief Get a pointer to the parent scene object of this instance.
    ///
    /// @return A pointer to the parent scene object of this instance.
    SceneObjectPtr sceneObject();

    /// @brief Get a raw pointer to a new render trait config instance cloned 
    /// from this one. Ownership and responsibility for the allocated 
    /// resources are fully transferred to the caller.
    ///
    /// @param newParent Pointer the scene object which will be parent to
    /// the cloned render trait config instance.
    ///
    /// @return A raw pointer to the render trait config instance cloned from 
    /// this one.
    virtual RenderTraitConfig* clone(const SceneObjectPtr newParent) = 0;
};

using RenderTraitConfigPtr = std::shared_ptr<RenderTraitConfig>;

} // namespace Renderboi

#endif//RENDERBOI__TOOLBOX__RENDER__TRAITS__RENDER_TRAIT_CONFIG_HPP
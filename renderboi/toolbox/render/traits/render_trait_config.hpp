#ifndef RENDERBOI__TOOLBOX__RENDER__TRAITS__RENDER_TRAIT_CONFIG_HPP
#define RENDERBOI__TOOLBOX__RENDER__TRAITS__RENDER_TRAIT_CONFIG_HPP

#include "renderboi/toolbox/render/traits/render_trait.hpp"
#include <memory>

namespace Renderboi
{

/// @see class SceneObject in renderboi/toolbox/scene/object/scene_object.hpp
class SceneObject;
using SceneObjectPtr = std::shared_ptr<SceneObject>;

/// @brief Abstract configuration class for a render trait. To be inherited from
/// when specializing config for specific render traits.
class RenderTraitConfig
{
friend SceneObject;

protected:
    /// @brief Reference to the parent scene object of the trait config.
    SceneObjectPtr _parentSceneObject;

    /// @brief Release held references to owned resources.
    virtual void _release();

public:
    /// @param parentSceneObject Reference to the parent scene object.
    RenderTraitConfig(SceneObjectPtr parentSceneObject);

    /// @brief Constant reference to the parent scene object.
    const SceneObjectPtr& ParentSceneObject = _parentSceneObject;
};

using RenderTraitConfigPtr = std::shared_ptr<RenderTraitConfig>;

} // namespace Renderboi

#endif//RENDERBOI__TOOLBOX__RENDER__TRAITS__RENDER_TRAIT_CONFIG_HPP
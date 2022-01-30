#ifndef RENDERBOI__TOOLBOX__RENDER__TRAITS__RENDER_TRAIT_CONFIG_MAP_HPP
#define RENDERBOI__TOOLBOX__RENDER__TRAITS__RENDER_TRAIT_CONFIG_MAP_HPP

#include <initializer_list>
#include <map>
#include <memory>
#include <utility>

#include "renderboi/toolbox/render/traits/render_trait.hpp"
#include "renderboi/toolbox/render/traits/render_trait_config.hpp"

namespace Renderboi
{

class SceneObject;
using SceneObjectPtr = std::shared_ptr<SceneObject>;

class RenderTraitConfigMap
{
friend SceneObject;

private:
    using MapType = std::map<RenderTrait, RenderTraitConfigPtr>;

public:
    using ConfigEntry = std::pair<const RenderTrait, RenderTraitConfigPtr>;

private:
    /// @brief Disallowed assignment operator
    RenderTraitConfigMap& operator=(const RenderTraitConfigMap& right) = delete;

    /// @brief Actual map to store pointers render trait configs.
    MapType _map;

    /// @brief Pointer to the parent scene object.
    SceneObjectPtr _sceneObject;

    /// @brief Release held references to shared resources.
    void _release();

public:
    RenderTraitConfigMap(const SceneObjectPtr);

    /// @param other RenderTraitConfigMap instance to copy trait configs from
    RenderTraitConfigMap(const SceneObjectPtr, const RenderTraitConfigMap& other);

    /// @param list Init list of pairs of RenderTrait / RenderTraitConfigPtr to
    /// store into the trait config map.
    RenderTraitConfigMap(std::initializer_list<ConfigEntry> list);

    /// @brief Get a pointer to the parent scene object of this instance.
    ///
    /// @return A pointer to the parent scene object of this instance.
    SceneObjectPtr sceneObject();

    /// @brief Get the config pointer for the given config trait.
    /// @param trait Render trait to get the config for.
    /// @return Pointer to the config for the given render trait (can be
    /// nullptr).
    RenderTraitConfigPtr getConfigForTrait(const RenderTrait trait);

    /// @brief Return whether there is a registered config for the given config
    /// trait.
    /// @param trait Render trait to test the config for.
    /// @return Whether there is a registered config for the given config trait.
    bool hasConfigForTrait(const RenderTrait trait);

    /// @brief Set the config pointer for the given config trait.
    /// @param trait Render trait to set the config for.
    /// @param config Pointer to the config to set for the given render trait.
    void setConfigForTrait(const RenderTrait trait, const RenderTraitConfigPtr config);

    /// @brief Remove the config pointer for the given config trait.
    /// @param trait Render trait whose config is to be cleared.
    void clearConfigForTrait(const RenderTrait trait);

    /// @brief Clear all config entries from the config map.
    void clear();
};

using RenderTraitConfigMapPtr = std::shared_ptr<RenderTraitConfigMap>;

} // namespace Renderboi

#endif//RENDERBOI__TOOLBOX__RENDER__TRAITS__RENDER_TRAIT_CONFIG_MAP_HPP
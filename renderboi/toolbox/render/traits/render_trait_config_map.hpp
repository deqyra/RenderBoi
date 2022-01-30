#ifndef RENDERBOI__TOOLBOX__RENDER__TRAITS__RENDER_TRAIT_CONFIG_MAP_HPP
#define RENDERBOI__TOOLBOX__RENDER__TRAITS__RENDER_TRAIT_CONFIG_MAP_HPP

#include <initializer_list>
#include <map>
#include <memory>
#include <stdexcept>
#include <type_traits>
#include <utility>

#include "render_trait.hpp"
#include "render_trait_config.hpp"

namespace renderboi
{

class SceneObject;
using SceneObjectPtr = std::unique_ptr<SceneObject>;

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

    /// @brief Reference to the parent scene object.
    SceneObject& _sceneObject;

public:
    /// @param sceneObject Reference to the scene object parent to this config
    /// map.
    RenderTraitConfigMap(SceneObject& sceneObject);

    /// @param sceneObject Reference to the scene object parent to this config
    /// map.
    /// @param other RenderTraitConfigMap instance to copy trait configs from
    RenderTraitConfigMap(SceneObject& sceneObject, const RenderTraitConfigMap& other);

    /// @brief Get a pointer to the parent scene object of this instance.
    ///
    /// @return A pointer to the parent scene object of this instance.
    SceneObject& sceneObject();

    /// @brief Get the config pointer for the given config trait.
    ///
    /// @param trait Render trait to get the config for.
    ///
    /// @return Reference to the config for the given render trait (can be
    /// nullptr).
    RenderTraitConfig& getConfigForTrait(const RenderTrait trait) const;

    /// @brief Return whether there is a registered config for the given config
    /// trait.
    ///
    /// @param trait Render trait to test the config for.
    ///
    /// @return Whether there is a registered config for the given config trait.
    bool hasConfigForTrait(const RenderTrait trait) const;

    /// @brief Add config for a given trait.
    ///
    /// @tparam Tr Literal describing which trait to add a config for.
    /// @tparam T Concrete type of the config trait to instantiate. Leave for
    /// deduction.
    /// @tparam ArgTypes Types of the arguments to pass to the config trait
    /// constructor. Leave for deduction.
    ///
    /// @param args Arguments to pass to the config constructor.
    template<
        RenderTrait Tr,
        typename T = typename RenderTraitMeta<Tr>::Config::type,
        typename... ArgTypes,
        typename = std::enable_if_t<std::is_base_of_v<RenderTraitConfig, T>, void>
    >
    T& addConfigForTrait(ArgTypes&&... args);

    /// @brief Set the config a given trait.
    ///
    /// @tparam Tr Literal describing which trait to add a config for.
    /// @tparam T Concrete type of the config trait to instantiate. Leave for
    /// deduction.
    /// @tparam ArgTypes Types of the arguments to pass to the config trait
    /// constructor. Leave for deduction.
    ///
    /// @param config Reference to the config to set for the given render trait.
    template<
        RenderTrait Tr,
        typename T = typename RenderTraitMeta<Tr>::Config::type,
        typename = std::enable_if_t<std::is_base_of_v<RenderTraitConfig, T>, void>
    >
    T& setConfigForTrait(std::unique_ptr<T>&& config);

    /// @brief Remove the config pointer for the given config trait.
    ///
    /// @param trait Render trait whose config is to be cleared.
    void clearConfigForTrait(const RenderTrait trait);

    /// @brief Clear all config entries from the config map.
    void clear();
};

template<
    RenderTrait Tr,
    typename T,
    typename... ArgTypes,
    typename
>
T& RenderTraitConfigMap::addConfigForTrait(ArgTypes&&... args)
{
    auto it = _map.insert({
        Tr,
        std::make_unique<T>(std::forward(args)...)
    });

    return *(it->second);
}

template<
    RenderTrait Tr,
    typename T,
    typename
>
T& RenderTraitConfigMap::setConfigForTrait(std::unique_ptr<T>&& config)
{
    if (!config)
    {
        std::string s = "RenderTraitConfigMap: cannot set null config for "
        "trait " + to_string(Tr) + ".";

        throw std::runtime_error(s.c_str());
    }

    auto it = _map.insert({
        Tr,
        std::move(config)
    });

    return *(it->second);
}


using RenderTraitConfigMapPtr = std::unique_ptr<RenderTraitConfigMap>;

} // namespace renderboi

#endif//RENDERBOI__TOOLBOX__RENDER__TRAITS__RENDER_TRAIT_CONFIG_MAP_HPP
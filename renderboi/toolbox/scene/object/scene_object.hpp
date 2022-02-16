#ifndef RENDERBOI__TOOLBOX__SCENE__OBJECT__SCENE_OBJECT_HPP
#define RENDERBOI__TOOLBOX__SCENE__OBJECT__SCENE_OBJECT_HPP

#include <algorithm>
#include <functional>
#include <iterator>
#include <memory>
#include <string>
#include <stdexcept>
#include <type_traits>
#include <map>
#include <vector>

#include <renderboi/core/transform.hpp>

#include "../../render/traits/render_trait.hpp"
#include "../../render/traits/render_trait_config.hpp"
#include "../../render/traits/render_trait_config_map.hpp"
#include "object_transform.hpp"
#include "component.hpp"
#include "component_map.hpp"
#include "component_type.hpp"

namespace renderboi
{

class Scene;

/// @brief An object meant to be part of a scene. Abstract entity made up of 
/// components which give it concrete aspects in the context of a scene. Also
/// contains information on how to draw itself.
class SceneObject
{
public:
    /// @brief Unique ID of the object.
    const unsigned int id;

    /// @brief Reference to the parent scene of this object.
    Scene& scene;

    /// @brief Whether this object is enabled in its parent scene.
    bool enabled;

    /// @brief Name of the object in the scene.
    const std::string name;

private:
    SceneObject& operator=(const SceneObject& other) = delete;

    /// @param other Instance to copy-construct from
    SceneObject(const SceneObject& other);

    /// @brief Keeps track of how many scene objects were instantiated (used
    /// as a unique ID system).
    static unsigned int _count;

    /// @brief The 3D transform of this object.
    ObjectTransform _transform;

    /// @brief Concrete aspects of the scene object.
    ComponentMap _componentMap;

    /// @brief How the object should be rendered according to certain traits.
    RenderTraitConfigMap _renderTraitConfigMap;

public:
    /// @param scene Pointer the parent scene of this object.
    /// @param name Name to give to the scene object.
    SceneObject(Scene& scene, const std::string name = "");

    ~SceneObject();

    /// @brief Get a pointer to the transform of this object.
    ///
    /// @return A pointer to the transform of this object.
    ObjectTransform& transform();

    /// @brief Get a pointer to the transform of this object.
    ///
    /// @return A pointer to the transform of this object.
    const ObjectTransform& transform() const;

    /// @brief Get a pointer to the component map of this object.
    ///
    /// @return A pointer to the component map of this object.
    ComponentMap& componentMap();

    /// @brief Get a pointer to the component map of this object.
    ///
    /// @return A pointer to the component map of this object.
    const ComponentMap& componentMap() const;

    /// @brief Get a pointer to the render trait map of this object.
    ///
    /// @return A pointer to the render trait map of this object.
    RenderTraitConfigMap& renderTraitConfigMap();

    /// @brief Get a pointer to the render trait map of this object.
    ///
    /// @return A pointer to the render trait map of this object.
    const RenderTraitConfigMap& renderTraitConfigMap() const;

    /// @brief Get the world transform of the object in the scene.
    ///
    /// @return The world transform of the object in the scene.
    Transform worldTransform() const;

    /// @brief Get a pointer to a new scene object instance cloned from this
    /// one. All components are cloned as well. Ownership and responsibility for
    /// the allocated resources are fully transferred to the caller (although 
    /// wrapped in a unique pointer).
    ///
    /// @return A unique pointer to the cloned scene object instance.
    SceneObjectPtr clone() const;
};

using SceneObjectPtr = std::unique_ptr<SceneObject>;

} // namespace renderboi

#endif//RENDERBOI__TOOLBOX__SCENE__OBJECT__SCENE_OBJECT_HPP
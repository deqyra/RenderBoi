#ifndef RENDERBOI__TOOLBOX__SCENE__SCENE_OBJECT_HPP
#define RENDERBOI__TOOLBOX__SCENE__SCENE_OBJECT_HPP

#include <algorithm>
#include <stdexcept>
#include <functional>
#include <iterator>
#include <memory>
#include <string>
#include <stdexcept>
#include <type_traits>
#include <map>
#include <vector>

#include <renderboi/core/transform.hpp>

#include <renderboi/utilities/to_string.hpp>

#include "object_transform.hpp"
#include "component.hpp"
#include "component_map.hpp"
#include "component_type.hpp"
#include "../../render/traits/render_trait.hpp"
#include "../../render/traits/render_trait_config.hpp"
#include "../../render/traits/render_trait_config_map.hpp"

namespace Renderboi
{

class Scene;
using ScenePtr = std::shared_ptr<Scene>;
using SceneWPtr = std::weak_ptr<Scene>;

class Factory;

/// @brief An object meant to be part of a scene. Abstract entity made up of 
/// components which give it concrete aspects in the context of a scene. Also
/// contains information on how to draw itself.
class SceneObject : public std::enable_shared_from_this<SceneObject>
{
friend Scene;
friend Factory;

private:
    SceneObject& operator=(const SceneObject& other) = delete;

    /// @param other Instance to copy-construct from
    SceneObject(const SceneObject& other);

    /// @brief Keeps track of how many scene objects were instantiated (used
    /// as a unique ID system).
    static unsigned int _count;

    /// @brief Pointer to the parent scene of this object.
    ScenePtr _scene;

    /// @brief The 3D transform of this object.
    ObjectTransformPtr _transform;

    /// @brief Concrete aspects of the scene object.
    ComponentMapPtr _componentMap;

    /// @brief How the object should be rendered according to certain traits.
    RenderTraitConfigMapPtr _renderTraitConfigMap;

    /// @brief Notify instance to let go of references to shared resources.
    void _release();

    /// @brief Link the scene object to its transform and internal maps (which
    /// cannot be done during construction). To be called before any other
    /// operation is performed on the scene object.
    void _init();

    /// @brief Same as _init(), but to be called on a newly cloned instance.
    /// 
    /// @param other Instance this one was cloned from.
    void _initCloned(const SceneObject& other);

public:
    /// @param scene Pointer the parent scene of this object.
    /// @param name Name to give to the scene object.
    SceneObject(const ScenePtr scene, const std::string name = "");

    ~SceneObject();

    /// @brief Get a pointer to the transform of this object.
    ///
    /// @return A pointer to the transform of this object.
    ObjectTransformPtr transform() const;

    /// @brief Get a pointer to the component map of this object.
    ///
    /// @return A pointer to the component map of this object.
    ComponentMapPtr componentMap() const;

    /// @brief Get a pointer to the render trait map of this object.
    ///
    /// @return A pointer to the render trait map of this object.
    RenderTraitConfigMapPtr renderTraitConfigMap() const;

    /// @brief Get the world transform of the object in the scene.
    ///
    /// @return The world transform of the object in the scene.
    Transform worldTransform() const;

    /// @brief Get the parent scene of this object.
    ///
    /// @return A pointer to the parent scene of this object.
    ScenePtr scene() const;

    /// @brief Get a shared pointer to a new scene object instance cloned from
    /// this one. All components are cloned as well. Ownership and 
    /// responsibility for the allocated resources are fully transferred to
    /// the caller (although wrapped in a shared pointer).
    ///
    /// @return A shared pointer to the cloned scene object instance.
    SceneObjectPtr clone() const;

    /// @brief Unique ID of the object.
    const unsigned int id;

    /// @brief Whether this object is enabled in its parent scene.
    bool enabled;

    /// @brief Name of the object in the scene.
    const std::string _name;
};

using SceneObjectPtr = std::shared_ptr<SceneObject>;
using SceneObjectWPtr = std::weak_ptr<SceneObject>;

}//namespace Renderboi

#endif//RENDERBOI__TOOLBOX__SCENE__SCENE_OBJECT_HPP
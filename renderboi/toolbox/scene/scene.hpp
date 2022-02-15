#ifndef RENDERBOI__TOOLBOX__SCENE__SCENE_HPP
#define RENDERBOI__TOOLBOX__SCENE__SCENE_HPP

#include <algorithm>
#include <chrono>
#include <functional>
#include <limits>
#include <memory>
#include <tuple>
#include <type_traits>
#include <unordered_map>
#include <vector>

#include <glm/glm.hpp>

#include <cpptools/container/tree.hpp>

#include "../script.hpp"
#include "object/component_type.hpp"
#include "object/scene_object.hpp"
#include "object/scene_object_metadata.hpp"

namespace renderboi
{

class Factory;

/// @brief A scene containing 3D objects organised in a tree structure. Handles
/// self-updating scripts and processes input from the application. Use Factory
/// to instantiate and terminate.

class Scene
{
friend Factory;

public:
    using ObjectTree = cpptools::Tree<SceneObjectPtr>;
    using TransformTree = cpptools::Tree<Transform>;
    using BoolTree = cpptools::Tree<bool>;

private:
    /// @brief Scene graph. Contains all objects in the scene, 
    /// hierarchically organised in a tree.
    ObjectTree _objects;

    /// @brief Tree replicating the structure of the scene graph, containing
    /// the world transforms of the mirrored scene objects.
    TransformTree _transforms;

    /// @brief Tree replicating the structure of the scene graph, telling 
    /// whether the transforms of the mirrored scene objects need an update.
    BoolTree _updateMarkers;

    /// @brief Indicates how many transforms in the scene are out of date.
    mutable unsigned int _outdatedTransformNodes;

    /// @brief Map scene object IDs to object metadata structs.
    std::unordered_map<unsigned int, SceneObjectMetadata> _objectMetadata;

    /// @brief Map script IDs to script references.
    std::unordered_map<unsigned int, Script&> _scripts;

    /// @brief Last time a scene update was triggered.
    std::chrono::time_point<std::chrono::system_clock> _lastTime;

    /// @brief Initialize the root of object tree in the scene, as well as
    /// the first scene object metadata entry.
    void _init();

    /// @brief Prepares the scene for destruction, by releasing strong 
    /// references in its scene objects, and delting the contents of all
    /// trees.
    void _terminate();

    /// @brief Callback linked to the transform notifier of every object in 
    /// the scene.
    ///
    /// @param id ID of the scene object whose transform emitted the 
    /// notification.
    ///
    /// @exception If the provided ID does not match that of an object 
    /// present in the scene, the function will throw a std::runtime_error.
    void _objectTransformModified(const unsigned int id);

    /// @brief Mark the world transform of an object for update in the
    /// update tree.
    ///
    /// @param id ID of the scene object whose world transform needs 
    /// updating.
    void _markForUpdate(const unsigned int id);

    /// @brief Get the metadata entry for the object of provided ID, or 
    /// throw an exception.
    ///
    /// @param id ID of the object whose metadata to find.
    /// @param failureMessage Quick indicating in which context a bad ID was
    /// provided.
    ///
    /// @return The metadata entry of the object of provided ID, if found.
    ///
    /// @exception If the provided ID does not match that of an object 
    /// registered in the scene, the function will throw a 
    /// std::runtime_error.
    SceneObjectMetadata& _findObjectMetaOrThrow(const unsigned int id, const std::string& failureMessage);

    /// @brief Get the metadata entry for the object of provided ID, or 
    /// throw an exception.
    ///
    /// @param id ID of the object whose metadata to find.
    /// @param failureMessage Quick indicating in which context a bad ID was
    /// provided.
    ///
    /// @return The metadata entry of the object of provided ID, if found.
    ///
    /// @exception If the provided ID does not match that of an object 
    /// registered in the scene, the function will throw a 
    /// std::runtime_error.
    const SceneObjectMetadata& _findObjectMetaOrThrow(const unsigned int id, const std::string& failureMessage) const;

    /// @brief Perform the actual scene object registration. Call after all
    /// sanity checks have passed.
    ///
    /// @param object Reference to the object to register in the scene.
    /// @param parentMeta Metadata entry for the scene object which should 
    /// be parent the newly registered scene object.
    void _performObjectRegistration(SceneObjectPtr&& object, const SceneObjectMetadata& parentMeta);

    /// @brief Recursively update world transforms in a DFS traversal of the
    /// transform tree.
    ///
    /// @param startingId ID of the object whose transform should be the 
    /// starting point of the recursive update, going downwards in the tree.
    void _worldTransformDFSUpdate(const unsigned int startingId) const;

    /// @brief For an object with given ID, find the topmost parent node 
    /// whose update marker is set, and return the parent ID chain to it.
    ///
    /// @param id ID of the object from which to start searching.
    ///
    /// @return An array filled with the IDs of the successive parents of 
    /// the object, up to the topmost one whose world transform was marked
    /// as outdated.
    std::vector<unsigned int> _findLongestOutdatedParentIdChain(const unsigned int id) const;

    /// @brief Update the world transform of the scene object with provided 
    /// ID, as well as the world transforms of all of its children.
    ///
    /// @param id ID of the object whose transform should be the starting
    /// point of the recursive update, goind downwards in the tree.
    void _worldTransformCascadeUpdate(const unsigned int id) const;

    /// @brief Update the world transform of the scene object with provided
    /// ID, but do not reverberate the update to the children transforms.
    ///
    /// @param id ID of the object whose world transform should be updated.
    void _worldTransformUpdateNoCascade(const unsigned int id) const;

    /// @brief Whether an object has a disabled parent in the scene graph.
    ///
    /// @param id ID of the object from which to start searching.
    ///
    /// @return Whether the object with provided ID has a disabled parent.
    bool _hasDisabledParent(const unsigned int id) const;

    /// @brief Maximum value an unsigned int can hold.
    static constexpr unsigned int _MaxUInt = std::numeric_limits<unsigned int>::max();

public:
    Scene();
    ~Scene();

    /// @brief Get a pointer to the object of provided ID.
    ///
    /// @param id ID of the object whose pointer to get.
    ///
    /// @return A reference to the object of provided ID.
    ///
    /// @exception If the provided ID does not match that of an object 
    /// present in the scene, the function will throw a std::runtime_error.
    SceneObject& operator[](const unsigned int id);

    /// @brief Create a new object and attach it to the graph as a root 
    /// child.
    ///
    /// @param name Name to give to the scene object.
    ///
    /// @return A reference to the newly created object.
    SceneObject& newObject(const std::string& name = "");

    /// @brief Create a new object and attach it to the graph as a child of
    /// the object with provided ID.
    ///
    /// @param parentId ID of the object which should be parent to the newly
    /// created object.
    ///
    /// @return A reference to the newly created object.
    ///
    /// @exception If the provided ID does not match that of an object 
    /// present in the scene, the function will throw a std::runtime_error.
    SceneObject& newObject(const unsigned int parentId);

    /// @brief Create a new object and attach it to the graph as a child of
    /// the object with provided ID.
    ///
    /// @param name Name to give to the scene object.
    /// @param parentId ID of the object which should be parent to the newly
    /// created object.
    ///
    /// @return A reference to the newly created object.
    ///
    /// @exception If the provided ID does not match that of an object 
    /// present in the scene, the function will throw a std::runtime_error.
    SceneObject& newObject(const std::string& name, const unsigned int parentId);

    /// @brief Remove the object with provided ID from the scene, as well as
    /// all of its children.
    ///
    /// @param id ID of the object to remove from the scene.
    ///
    /// @exception If the provided ID does not match that of an object 
    /// present in the scene, the function will throw a std::runtime_error.
    void removeObject(const unsigned int id);

    /// @brief Move the object with provided ID (as well as all of its 
    /// children) in the tree so that its new parent is the object of 
    /// second provided ID.
    ///
    /// @param id ID of the object to move in the tree.
    /// @param newParentId ID of the object which should be the new parent
    /// to the moved object.
    /// @param worldPositionStays Whether or not the transform of the moved
    /// object should be updated so that its world transform remains the 
    /// as before being moved.
    ///
    /// @exception If the provided IDs do not match that of objects present 
    /// in the scene, the function will throw a std::runtime_error.
    void moveObject(const unsigned int id, const unsigned int newParentId, const bool worldPositionStays = true);

    /// @brief Get the ID of the object which is parent to the object with 
    /// provided ID in the scene.
    ///
    /// @param id ID of the object whose parent ID to find.
    ///
    /// @return The ID of the object which is parent to the object with 
    /// provided ID.
    ///
    /// @exception If the provided ID does not match that of an object 
    /// present in the scene, the function will throw a std::runtime_error.
    unsigned int getParentId(const unsigned int id) const;

    /// @brief Get a pointer to the object which is parent to the object 
    /// with provided ID in the scene.
    ///
    /// @param id ID of the object whose parent to find.
    ///
    /// @return A pointer to the object which is parent to the object 
    /// with provided ID in the scene.
    ///
    /// @exception If the provided ID does not match that of an object 
    /// present in the scene, the function will throw a std::runtime_error.
    SceneObject& getParent(const unsigned int id) const;
    
    /// @brief Update all world transforms of objects marked for update.
    void updateAllTransforms();

    /// @brief Get the world transform of the object with provided ID.
    ///
    /// @param id ID of the object whose world transform to get.
    /// @param cascadeUpdate In case the world transform of the object needs
    /// to be recalculated, whether or not to reverberate the update to the
    /// world transforms of the children of that object.
    ///
    /// @exception If the provided ID does not match that of an object 
    /// present in the scene, the function will throw a std::runtime_error.
    Transform getWorldTransform(const unsigned int id, const bool cascadeUpdate = true) const;

    /// @brief Get references to all scene objects.
    ///
    /// @param mustBeEnabled Whether or not to filter the objects depending
    /// on their enabled state.
    ///
    /// @return An array filled with references to all the objects in the 
    /// scene which meet the criteria.
    std::vector<std::reference_wrapper<SceneObject>> getAllObjects(const bool mustBeEnabled = true) const;

    /// @brief Register a script in the scene. It will then receive update
    /// signals from the scene.
    ///
    /// @param script Reference to the script to register in the scene.
    ///
    /// @exception If the provided script pointer is null, the function will
    /// throw a std::runtime_error.
    void registerScript(Script& script);

    /// @brief Detach a script from the scene so that it no longer receives 
    /// update signals from it.
    ///
    /// @param id ID of the script to detach from the scene.
    ///
    /// @return A pointer to the script that was just detached.
    void detachScript(const unsigned int id);

    /// @brief Trigger a scene update, which will send an update signal to 
    /// all registered scripts.
    void triggerUpdate();

    /// @brief Get pointers to all scene objects which have a certain 
    /// component.
    ///
    /// @tparam T Literal describing the type of component to query.
    ///
    /// @param mustBeEnabled Whether or not to filter the objects depending
    /// on their enabled state.
    ///
    /// @return An array filled with pointers to all the objects in the 
    /// scene which meet the criteria.
    template<ComponentType T>
    std::vector<std::reference_wrapper<SceneObject>> getObjectsWithComponent(const bool mustBeEnabled = true) const;
};

template<ComponentType T>
std::vector<std::reference_wrapper<SceneObject>> Scene::getObjectsWithComponent(const bool mustBeEnabled) const
{
    const std::vector<std::reference_wrapper<SceneObject>> all = getAllObjects();
    std::vector<std::reference_wrapper<SceneObject>> result;

    // Tells whether an object whose weak pointer is provided should be added to the result vector
    std::function<bool(const SceneObject&)> componentChecker = [this, mustBeEnabled](const SceneObject& obj) -> bool
    {
        // Skip if the object is not enabled or if any of its parents is not enabled
        if (mustBeEnabled && !obj.enabled) return false;
        if (mustBeEnabled && _hasDisabledParent(obj.id)) return false;
        // Return whether the object has the component being asked for
        return obj.componentMap().hasComponent<T>();
    };

    // Copy all object pointers into the result vector using the check function
    std::copy_if(all.begin(), all.end(), std::back_inserter(result), componentChecker);

    return result;
}

using ScenePtr = std::unique_ptr<Scene>;

} // namespace renderboi

#endif//RENDERBOI__TOOLBOX__SCENE__SCENE_HPP
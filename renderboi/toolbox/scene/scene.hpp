#ifndef RENDERBOI_TOOLBOX_SCENE_SCENE_HPP
#define RENDERBOI_TOOLBOX_SCENE_SCENE_HPP

#include <unordered_map>
#include <utility>
#include <vector>

#include <cpptools/container/tree.hpp>

#include <renderboi/core/numeric.hpp>
#include <renderboi/core/3d/transform.hpp>
#include <renderboi/core/3d/affine/affine_operation.hpp>

#include <renderboi/toolbox/interfaces/transform_proxy.hpp>
#include <renderboi/toolbox/scene/components/world_transform.hpp>
#include <renderboi/toolbox/scene/components/local_transform.hpp>

#include "object.hpp"

namespace rb {

class Scene {
public:
    Scene();
    ~Scene();

    /// @brief Get the root object of the scene
    /// @return The root object of the scene
    Object root() const;

    /// @brief Create a new object in the scene
    /// @param parent Object which should be parent to the newly created object
    /// @param name Name to give to the scene object
    /// @return The newly created object
    Object create(Object parent, std::string name = "");

    /// @brief Remove an object from the scene, together with its descendants
    /// and all of their components
    /// @param id ID of the object to remove from the scene
    void erase(Object object);

    /// @brief Reparent an object in the scene
    /// @param object Object to reparent
    /// @param newParent Object which should be the new parent to the moved object
    /// @param worldTransformStays Whether or not the local transform of the moved
    /// object should be updated so that its world transform remains the same
    /// as before being moved
    void reparent(Object object, Object newParent, bool worldTransformStays = true);

    /// @brief Get an object's parent object
    /// @param object Object whose parent to find
    /// @return The object's parent
    Object parentOf(Object object) const;

    /// @brief Update all world transforms of objects marked for update
    void update();

    /// @brief Get an object's world transform, updating it along the way if needed
    /// @param object Object whose world transform to get
    /// @param cascadeUpdate In case the world transform of the object needs
    /// to be recalculated, whether or not to reverberate the update to the
    /// world transforms of the children of that object
    const RawTransform& worldTransform(Object object, bool cascadeUpdate = false);

    class LocalTransformProxy;

    /// @brief Get a wrapper around the provided object's local transform
    /// @param object The object to get a local transform wrapper for
    /// @note The returned wrapper satisfies the TransformProxy concept
    /// @return A wrapper around the object's local transform
    LocalTransformProxy& localTransform(Object object);

    template<typename C, typename... CArgs>
    C& emplace(Object object, CArgs&&... compArgs) {
        static_assert(not (std::is_same_v<C, WorldTransform> or std::is_same_v<C, LocalTransform>), "Scene::emplace shall not be used to put a world transform or a local transform on an object, those are automatically managed");

        return _registry.emplace<C>(object, std::forward<CArgs>(compArgs)...);
    }

    /// @brief Get a reference to an object's component
    /// @tparam C The type of the component to retrieve
    /// @param object The object on which the component to be retrieved is attached
    /// @note WorldTransform and LocalTransform shall not be queried through this function, the worldTransform and localTransform function can respectively be used instead
    template<typename C>
    C& get(Object object) {
        static_assert(not (std::is_same_v<C, WorldTransform> or std::is_same_v<C, LocalTransform>), "Scene::get shall not be used to retrieve world transforms or local transforms, use Scene::worldTransform and Scene::localTransform respectively instead.");

        return _registry.get<C>(object);
    }

    /// @copydoc template<typename>Scene::get(Object)
    template<typename C>
    const C& get(Object object) const {
        static_assert(not (std::is_same_v<C, WorldTransform> or std::is_same_v<C, LocalTransform>), "Scene::get shall not be used to retrieve world transforms or local transforms, use Scene::worldTransform and Scene::localTransform respectively instead.");

        return _registry.get<C>(object);
    }

    template<typename... Cs>
    using ComponentView = decltype(std::declval<ObjectRegistry>().view<Cs...>());

    /// @brief Get a view on all objects that have a given set of components
    /// @tparam Cs The types of component to have for an object to be included in the view
    /// @note WorldTransform and LocalTransform shall not be queried through this function, the worldTransform and localTransform function can respectively be used instead
    template<typename... Cs>
    ComponentView<Cs...> view() {
        static_assert((not (std::is_same_v<Cs, WorldTransform> or std::is_same_v<Cs, LocalTransform>) && ...), "Scene::view shall not be used to retrieve world transforms or local transforms, use Scene::worldTransform and Scene::localTransform respectively instead.");

        return _registry.view<Cs...>();
    }

    template<typename... Cs>
    using ComponentGroup = decltype(std::declval<ObjectRegistry>().group<Cs...>());

    /// @brief Get a view on all objects that have a given set of components
    /// @tparam Cs The types of component to have for an object to be included in the view
    /// @note WorldTransform and LocalTransform shall not be queried through this function, the worldTransform and localTransform function can respectively be used instead
    template<typename... Cs>
    ComponentGroup<Cs...> group() {
        static_assert((not (std::is_same_v<Cs, WorldTransform> or std::is_same_v<Cs, LocalTransform>) && ...), "Scene::group shall not be used to retrieve world transforms or local transforms, use Scene::worldTransform and Scene::localTransform respectively instead.");

        return _registry.group<Cs...>();
    }
    
private:
    using ObjectTree = tools::tree<Object>;
    using ObjectNode = ObjectTree::node_handle_t;

    /// @brief Information about an object within the scene
    struct ObjectMetadata {
        /// @brief Handle to the node of the object in the scene graph
        ObjectNode node;

        /// @brief Name of the object
        std::string name;

        /// @brief ID of the graph node containing the world transform of the
        /// object this metadata refers to
        bool enabled;

        /// @brief ID of the graph node containing the object this metadata
        /// refers to
        bool transformOutdated;
    };

    /// @brief Component store for the objects
    ObjectRegistry _registry;

    /// @brief Scene graph
    ObjectTree _objects;

    /// @brief Object at the root of the scene
    Object _root;

    /// @brief Mapping of tree node handles to the objects they contain
    std::unordered_map<Object, ObjectMetadata> _metadata;

    /// @brief How many transforms in the scene are out of date
    mutable unsigned int _outdatedTransformCount;

    /// @brief Create a new object and attach it to the scene as a child of
    /// the provided object
    /// @param parentMeta Metadata entry of the object which should be parent to
    /// the newly created object
    /// @param name Name to give to the scene object
    /// @return The newly created object
    Object _newObject(const ObjectMetadata& parentMeta, std::string&& name);
    
    /// @brief Callback linked to the transform notifier of every object in
    /// the scene
    /// @param object Scene object whose transform emitted the notification
    void _objectTransformModified(Object object);

    /// @brief Mark the world transform of an object for update in the
    /// update tree
    /// @param object Scene object whose world transform needs updating
    void _markForUpdate(Object object);

    /// @brief Recursively update world transforms in a DFS traversal of the
    /// transform tree
    /// @param startingId ID of the object whose transform should be the
    /// starting point of the recursive update, going downwards in the tree
    void _worldTransformDFSUpdate(Object start);

    /// @brief For an object with given ID, find the topmost parent node
    /// whose update marker is set, and return the parent ID chain to it
    /// @param id ID of the object from which to start searching
    /// @return An array filled with the IDs of the successive parents of
    /// the object, up to the topmost one whose world transform was marked
    /// as outdated
    std::vector<Object> _computeOutdatedParentChain(Object object);

    /// @brief Update the world transform of the provided object, as well as
    /// that of all of its children
    /// @param object Object whose world transform should be updated, along with
    /// that of its children
    /// @pre The world transform of the provided object's parent is up-to-date
    void _updateAllWorldTransforms(Object object);

    /// @brief Update the world transform of the provided object only
    /// @param Object Object whose world transform should be updated
    /// @pre The world transform of the provided object's parent is up-to-date
    void _updateThisWorldTransform(Object object);

    /// @brief Whether an object has a disabled parent in the scene graph
    /// @param id ID of the object from which to start searching
    /// @return Whether the object with provided ID has a disabled parent
    bool _hasDisabledParent(Object object) const;

public:
    /// @brief Wrapper for a scene object's local transform, providing convenient
    /// operator overloads to apply operations on it.
    /// @note This wrapper may be copied and moved around, but not assigned to.
    /// The wrapper is invalidated whenever the object whose transform is being
    /// wrapped is reparented (its immediate parent changes).
    class LocalTransformProxy {
    public:
        LocalTransformProxy(const LocalTransformProxy& other) = default;
        LocalTransformProxy(LocalTransformProxy&& other)      = default;

        LocalTransformProxy& operator=(const LocalTransformProxy& other) = default;
        LocalTransformProxy& operator=(LocalTransformProxy&& other)      = default;

        LocalTransformProxy& operator=(const RawTransform& other);
        LocalTransformProxy& operator=(RawTransform&& other);

        template<affine::AffineOperation Op>
        friend LocalTransformProxy& operator<<(LocalTransformProxy& proxy, const Op& op) {
            op.apply(*(proxy._transform));
            proxy._markObjectForUpdateInScene();

            return proxy;
        }

        template<affine::AffineOperation Op>
        friend LocalTransformProxy&& operator<<(LocalTransformProxy&& proxy, const Op& op) {
            op.apply(*(proxy._transform));
            proxy._markObjectForUpdateInScene();

            return proxy;
        }

        LocalTransformProxy(Scene& scene, Object object, RawTransform& transform);

    private:
        void _markObjectForUpdateInScene();

        /// @brief The scene which this proxy should report updates to
        Scene* _scene;
        
        /// @brief The wrapped local transform
        RawTransform* _transform;

        /// @brief The object whose local transform is being wrapped by this proxy
        Object _object;
    };
    static_assert(TransformProxy<LocalTransformProxy>);

    friend class LocalTransformProxy;
};

using LocalTransformProxy = Scene::LocalTransformProxy;

} // namespace rb

#endif // RENDERBOI_TOOLBOX_SCENE_SCENE_HPP

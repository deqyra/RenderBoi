#include <algorithm>
#include <ranges>

#include <cpptools/container/tree.hpp>
#include <cpptools/utility/unary.hpp>

#include <renderboi/core/numeric.hpp>
#include <renderboi/core/3d/transform.hpp>
#include <renderboi/core/3d/affine/transformation.hpp>

#include "scene.hpp"
#include "object.hpp"
#include "components/local_transform.hpp"
#include "components/world_transform.hpp"

namespace rb {

Scene::Scene()
    : _registry()
    , _objects()
    , _root()
    , _metadata()
    , _outdatedTransformCount(0) {
    _root = _registry.create();
    auto node = _objects.emplace_node(_objects.root(), _root);
    
    _registry.emplace<LocalTransform>(_root);
    _registry.emplace<WorldTransform>(_root);

    auto meta = ObjectMetadata {
        .node              = node,
        .name              = "Scene root",
        .enabled           = true,
        .transformOutdated = false
    };  

    _metadata.insert({_root, meta});
}

Scene::~Scene() {
    _objects.clear();
    _registry.clear();
}

Object Scene::root() const {
    return _root;
}

Object Scene::create(const Object parent, std::string name) {
    return _newObject(_metadata.at(parent), std::move(name));
}

void Scene::erase(const Object object) {
    auto handle = _metadata.at(object).node;
    auto removedObjects = _objects.chop_subtree(handle);

    unsigned int outdatedObjects = 0;
    for (auto obj : removedObjects) {
        auto& meta = _metadata.at(obj);
        outdatedObjects += meta.transformOutdated;
        _registry.destroy(obj);
    }
    _outdatedTransformCount -= outdatedObjects;
}

void Scene::reparent(
    const Object object,
    const Object newParent,
    const bool worldTransformStays
) {
    auto handle = _metadata.at(object).node;
    auto parentHandle = _metadata.at(newParent).node;

    if (worldTransformStays) {
        // update local transform of moved object so that its world transform remains the same
        // we first need to get the up-to-date world transform of this object
        const RawTransform& thisWorldTransform = worldTransform(object, false);

        // reparent object
        _objects.move_subtree(parentHandle, handle);

        // compute new local transform from new parent world transform
        const RawTransform& parentWorldTransform = worldTransform(newParent, false);
        RawTransform newLocalTransform = inverse(parentWorldTransform) * thisWorldTransform;
        _registry.replace<LocalTransform>(object, std::move(newLocalTransform));

        // no need to mark the object for update as it stayed in place
    }
    else {
        _objects.move_subtree(parentHandle, handle);
        _markForUpdate(object);
    }
}

Object Scene::parentOf(const Object object) const {
    return *(_metadata.at(object).node.parent());
}

void Scene::update() {
    if (_outdatedTransformCount > 0) {
        _worldTransformDFSUpdate(_root);
    }
}

const RawTransform& Scene::worldTransform(const Object object, const bool cascadeUpdate) {
    if (_outdatedTransformCount > 0) {
        // Track the furthest outdated parent object
        const std::vector<Object> outdatedObjects = _computeOutdatedParentChain(object);
        // if non-empty, the result of this function includes the object itself at the back of the vector

        if (!outdatedObjects.empty()) {
            for (auto it = outdatedObjects.begin(); it != outdatedObjects.end() - 1; ++it) {
                _updateThisWorldTransform(*it);
            }

            if (cascadeUpdate) {
                _updateAllWorldTransforms(outdatedObjects.back());
            } else {
                _updateThisWorldTransform(outdatedObjects.back());
            }
        }
    }

    return _registry.get<WorldTransform>(object);
}

Scene::LocalTransformProxy& Scene::localTransform(Object object) {
    if (!_registry.all_of<LocalTransformProxy>(object)) {
        return _registry.emplace<LocalTransformProxy>(object, *this, object, _registry.get<LocalTransform>(object).value);
    }
    return _registry.get<LocalTransformProxy>(object);
}

Scene::LocalTransformProxy::LocalTransformProxy(Scene& scene, Object object, RawTransform& transform)
    : _scene(&scene)
    , _transform(&transform)
    , _object(object)
{

}

Scene::LocalTransformProxy& Scene::LocalTransformProxy::operator=(const RawTransform& other)
{
    *_transform = other;
    _markObjectForUpdateInScene();
    
    return *this;
}

Scene::LocalTransformProxy& Scene::LocalTransformProxy::operator=(RawTransform&& other)
{
    *_transform = std::move(other);
    _markObjectForUpdateInScene();
    
    return *this;
}

void Scene::LocalTransformProxy::_markObjectForUpdateInScene() {
    _scene->_markForUpdate(_object);
}

Object Scene::_newObject(const ObjectMetadata& parentMeta, std::string&& name) {
    auto object = _registry.create();
    auto parentNode = parentMeta.node;

    auto node = _objects.emplace_node(parentNode, object);
    const RawTransform& parentWorldTransform = _registry.get<WorldTransform>(*parentNode);
    
    _registry.emplace<LocalTransform>(object); // Initialized to the identity transform
    _registry.emplace<WorldTransform>(object, parentWorldTransform); // Copy the parent's (possibly outdated) world transform

    auto meta = ObjectMetadata {
        .node                  = node,
        .name                  = std::move(name),
        .enabled               = true,
        .transformOutdated     = parentMeta.transformOutdated // Copy whether the parent's world transform was outdated when we copied it
    };  

    _metadata.insert({object, meta});
    return object;
}

void Scene::_objectTransformModified(const Object object) {
    _markForUpdate(object);
}

void Scene::_markForUpdate(Object object) {
    auto& meta = _metadata.at(object);

    if (!meta.transformOutdated) {
        _outdatedTransformCount++;
        meta.transformOutdated = true;
    }
}

void Scene::_worldTransformDFSUpdate(const Object object) {
    auto& meta = _metadata.at(object);

    if (meta.transformOutdated) {
        _updateAllWorldTransforms(object);
    } else {
        for (const auto& child : meta.node.children()) {
            _worldTransformDFSUpdate(*child);
        }
    }
}

std::vector<Object> Scene::_computeOutdatedParentChain(const Object object) {
    std::vector<ObjectNode> parentChain;
    for (auto handle = _metadata.at(object).node; handle != ObjectNode::null_handle; handle = handle.parent()) {
        // this includes the passed in object's node in the parent chain -- this is on purpose
        parentChain.push_back(handle);
    }

    auto isOutdated = [this](const ObjectNode& node) -> bool {
        return _metadata.at(*node).transformOutdated;
    };
    
    namespace stdv = std::views;
    auto reversedParentChain = parentChain | stdv::reverse;
    // find the furthest outdated parent
    auto furthestOutdatedParent = std::ranges::find_if(reversedParentChain, isOutdated);

    // dereference the nodes from that point backwards
    auto reversedOutdatedParentObjects = reversedParentChain
        | stdv::drop(std::distance(reversedParentChain.begin(), furthestOutdatedParent))
        | stdv::transform(tools::dereference<const ObjectNode&>);

    // return the result
    return { reversedOutdatedParentObjects.begin(), reversedOutdatedParentObjects.end() };
}

void Scene::_updateAllWorldTransforms(const Object object) {
    auto meta = _metadata.at(object);
    _updateThisWorldTransform(object);

    for (const auto child : meta.node.children()) {
        _updateAllWorldTransforms(*child);
    }
}

void Scene::_updateThisWorldTransform(const Object object) {
    auto& meta = _metadata.at(object);
    RawTransform& localTransform = _registry.get<LocalTransform>(object);
    RawTransform& worldTransform = _registry.get<WorldTransform>(object);
    worldTransform = localTransform;

    const auto parentNode = meta.node.parent();
    if (parentNode != ObjectNode::null_handle) {
        const auto parentTransform = _registry.get<WorldTransform>(*parentNode);
        affine::transform(worldTransform, parentTransform);
    }

    if (meta.transformOutdated) {
        meta.transformOutdated = false;
        --_outdatedTransformCount;
    }
}

bool Scene::_hasDisabledParent(const Object Object) const {
    auto handle = _metadata.at(Object).node;

    while (handle != ObjectNode::null_handle) {
        auto& meta = _metadata.at(*handle);
        if (!meta.enabled) {
            return true;
        }
    }
    return false;
}

} // namespace rb

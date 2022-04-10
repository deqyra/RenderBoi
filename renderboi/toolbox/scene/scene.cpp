#include "scene.hpp"

#include <algorithm>
#include <chrono>
#include <functional>
#include <memory>
#include <unordered_map>
#include <vector>

#include <glm/glm.hpp>

#include <cpptools/container/tree.hpp>

#include "../factory.hpp"
#include "../script.hpp"
#include "object/scene_object.hpp"

namespace renderboi
{

Scene::Scene() :
    _objects(std::make_unique<SceneObject>(*this, "SCENE_ROOT")),
    _transforms(Transform()),
    _updateMarkers(false),
    _outdatedTransformCount(0),
    _objectMetadata(),
    _scripts(),
    _lastTime(std::chrono::system_clock::now()),
    _registry()
{
    const ObjectTree::NodePtr objectRootNode = _objects.getRoot();
    const TransformTree::NodePtr transformRootNode = _transforms.getRoot();
    const BoolTree::NodePtr updateRootNode = _updateMarkers.getRoot();

    const unsigned int rootId = objectRootNode->value->id;
    // Set and map metadata
    _objectMetadata[rootId] = {
        .id                     = rootId,
        .parentId               = _MaxUInt,
        .objectNodeId           = objectRootNode->id,
        .transformNodeId        = transformRootNode->id,
        .updateNodeId           = updateRootNode->id,
        .transformSubscriberId  = _MaxUInt
    };
    
}

Scene::~Scene()
{
    // Remove scene references in all scene objects, unsubscribe scene from object updates
    for (const auto& [_, meta] : _objectMetadata)
    {
        _objects[meta.id]->value->transform().getNotifier().deleteSubscriber(meta.transformSubscriberId);
    }

    // Clear all trees
    _objects.clear();
    _transforms.clear();
    _updateMarkers.clear();

    // Clear metadata, scripts
    _objectMetadata.clear();
    _scripts.clear();
}

SceneObject& Scene::operator[](const unsigned int id)
{
    const SceneObjectMetadata& meta = _findObjectMetaOrThrow(id, "cannot retrieve this object");
    const ObjectTree::NodePtr node = _objects[meta.objectNodeId];
    return *(node->value);
}

SceneObject& Scene::newObject(const std::string& name)
{
    const ObjectTree::NodePtr node = _objects.getRoot();
    const unsigned int rootId = node->value->id;

    SceneObjectPtr object = std::make_unique<SceneObject>(*this, name);
    SceneObject& objectRef = *object;

    _performObjectRegistration(std::move(object), _objectMetadata[rootId]);
    return objectRef;
}

SceneObject& Scene::newObject(const unsigned int parentId)
{
    const SceneObjectMetadata& meta = _findObjectMetaOrThrow(parentId, "cannot create new (unnamed) object with this parent");

    SceneObjectPtr object = std::make_unique<SceneObject>(*this);
    SceneObject& objectRef = *object;

    _performObjectRegistration(std::move(object), meta);
    return objectRef;
}

SceneObject& Scene::newObject(const std::string& name, const unsigned int parentId)
{
    const SceneObjectMetadata& meta = _findObjectMetaOrThrow(parentId, "cannot create new object with this parent");

    SceneObjectPtr object = std::make_unique<SceneObject>(*this, name);
    SceneObject& objectRef = *object;

    _performObjectRegistration(std::move(object), meta);
    return objectRef;
}

void Scene::removeObject(const unsigned int id)
{
    // Retrieve object metadata
    const SceneObjectMetadata& meta = _findObjectMetaOrThrow(id, "cannot remove this object");

    const ObjectTree::NodePtr objectNode = _objects[meta.objectNodeId];
    const std::vector<ObjectTree::NodePtr> children = objectNode->getChildren();

    // Recursively remove all children before carrying on with the object at hand
    for (const auto child : children)
    {
        removeObject(child->value->id);
    }

    // Within the markers about to be removed, subtract the count of those set 
    // to true from the outdated transform count
    _outdatedTransformCount -= _updateMarkers.countValue(true, meta.updateNodeId);

    // Remove nodes in graphs
    _objects.removeBranch(meta.objectNodeId);
    _transforms.removeBranch(meta.transformNodeId);
    _updateMarkers.removeBranch(meta.updateNodeId);
    
    // Unsubscribe from transform notifier
    objectNode->value->transform().getNotifier().deleteSubscriber(meta.transformSubscriberId);

    // Remove metadata
    _objectMetadata.erase(meta.id);
}

void Scene::moveObject(const unsigned int id, const unsigned int newParentId, const bool worldPositionStays)
{
    // Retrieve IDs of both nodes in all graphs
    const SceneObjectMetadata& meta = _findObjectMetaOrThrow(id, "cannot move this object");
    const SceneObjectMetadata& parentMeta = _findObjectMetaOrThrow(id, "cannot move to this object");

    // Fetch the world transform of moved object, updating if necessary
    const Transform worldTransform = getWorldTransform(id);

    // Move nodes in all graphs
    _objects.moveBranch(meta.objectNodeId, parentMeta.objectNodeId);
    _transforms.moveBranch(meta.transformNodeId, parentMeta.transformNodeId);
    _updateMarkers.moveBranch(meta.updateNodeId, parentMeta.updateNodeId);

    if (worldPositionStays)
    {
        // Get the world transform of the new parent, not cascading the update
        const Transform parentTransform = getWorldTransform(newParentId, false);
        _objects[meta.objectNodeId]->value->transform() = worldTransform.compoundFrom(parentTransform);
        // No need to mark the object for update as it stayed in place
    }
    // Mark object for update if it did not keep its world position
    else _markForUpdate(id);
}

unsigned int Scene::getParentId(const unsigned int id) const
{
    // Retrieve object metadata
    const SceneObjectMetadata& meta = _findObjectMetaOrThrow(id, "cannot retrieve the parent ID of this object");
    return meta.parentId;
}

SceneObject& Scene::getParent(const unsigned int id) const
{
    // Retrieve object metadata
    const SceneObjectMetadata& meta = _findObjectMetaOrThrow(id, "cannot retrieve the parent of this object");
    return *(_objects[meta.parentId]->value);
}

void Scene::updateAllTransforms()
{
    if (_outdatedTransformCount)
    {
        const ObjectTree::NodePtr objectRootNode = _objects.getRoot();
        const std::vector<ObjectTree::NodePtr> childNodes = objectRootNode->getChildren();

        // Start the DFS routine on each of the root children
        for(auto it = childNodes.begin(); it != childNodes.end(); it++)
        {
            SceneObject& object = *((*it)->value);
            _worldTransformDFSUpdate(object.id);
        }

        _outdatedTransformCount = 0;
    }
}

Transform Scene::getWorldTransform(const unsigned int id, const bool cascadeUpdate) const
{
    const SceneObjectMetadata& meta = _findObjectMetaOrThrow(id, "cannot retrieve world transform of this object");

    // Update transform if required
    if (_outdatedTransformCount > 0)
    {
        // Find the IDs of parents that make up the longest chain of outdated object
        const std::vector<unsigned int> outdatedIds = _findLongestOutdatedParentIdChain(id);

        // If an outdated parent chain was found
        if (outdatedIds.size() != 0)
        {
            // If a cascade update is request, simply cascade update the furthest parent
            if (cascadeUpdate)
            {
                _worldTransformCascadeUpdate(outdatedIds.back());
            }
            else
            {
                // Otherwise, update each element in the parent chain from the top, not cascading
                for (auto it = outdatedIds.rbegin(); it != outdatedIds.rend(); it++)
                {
                    _worldTransformUpdateNoCascade(*it);
                }

                // Finally, update this object, not cascading
                _worldTransformUpdateNoCascade(id);
            }
        }
        // If no outdated parent chain was found
        else
        {
            // Update the object only if required
            if (_updateMarkers[meta.updateNodeId]->value)
            {
                if (cascadeUpdate)
                {
                    _worldTransformCascadeUpdate(id);
                }
                else
                {
                    _worldTransformUpdateNoCascade(id);
                }
            }
        }
    }

    // Retrieve matrix graph node ID and return matrix
    const TransformTree::NodePtr node = _transforms[meta.transformNodeId];
    return node->value;
}

std::vector<std::reference_wrapper<SceneObject>> Scene::getAllObjects(const bool mustBeEnabled) const
{
    std::vector<std::reference_wrapper<SceneObject>> result;

    for (const auto& [_, meta] : _objectMetadata)
    {
        const ObjectTree::NodePtr node = _objects[meta.objectNodeId];
        if (!mustBeEnabled || node->value->enabled)
            result.push_back(*(node->value));
    }

    return result;
}

void Scene::registerScript(Script& script)
{
    _scripts.insert({script.id, script});
}

void Scene::detachScript(const unsigned int id)
{
    _scripts.erase(id);
}

void Scene::triggerUpdate()
{
    // Get time delta (in seconds) and upate last update time
    const std::chrono::time_point<std::chrono::system_clock> now = std::chrono::system_clock::now();
    const std::chrono::duration<double> delta = now - _lastTime;
    _lastTime = now;

    // Update all registered scripts
    for (const auto& [_, script] : _scripts)
    {
        script.update((float)(delta.count()));
    }
}

void Scene::_objectTransformModified(const unsigned int id)
{
    auto it = _objectMetadata.find(id);
    if (it == _objectMetadata.end())
    {
        std::string s = "Scene: the object transform callback was called with non-existing ID " + std::to_string(id) + "!";
        throw std::runtime_error(s.c_str());
    }

    _markForUpdate(id);
}

void Scene::_markForUpdate(const unsigned int id)
{
    auto it = _objectMetadata.find(id);
    // Retrieve object metadata
    const SceneObjectMetadata& meta = it->second;
    const BoolTree::NodePtr updateNode = _updateMarkers[meta.updateNodeId];
    // Set the marker to true
    if (!updateNode->value)
    {
        _outdatedTransformCount++;
        updateNode->value = true;
    }
}

const SceneObjectMetadata& Scene::_findObjectMetaOrThrow(const unsigned int id, const std::string& failureMessage) const
{
    auto it = _objectMetadata.find(id);
    if (it == _objectMetadata.end())
    {
        const std::string s = "Scene: no SceneObject with ID " + std::to_string(id)
                            + " - " + failureMessage + ".";

        throw std::runtime_error(s.c_str());
    }

    return it->second;
}

SceneObjectMetadata& Scene::_findObjectMetaOrThrow(const unsigned int id, const std::string& failureMessage)
{
    const SceneObjectMetadata& meta = _findObjectMetaOrThrow(id, failureMessage);
    
    // I feel utterly ashamed for writing this purposefully, but it's better than repeating code IMHO
    return const_cast<SceneObjectMetadata&>(meta);
}

void Scene::_performObjectRegistration(SceneObjectPtr&& object, const SceneObjectMetadata& parentMeta)
{
    const Transform parentTransform = _transforms[parentMeta.transformNodeId]->value;
    SceneObject& objectRef = *object;

    // Get transform of new object and apply parent transform to it
    Transform newTransform = (Transform)(objectRef.transform());
    newTransform = newTransform.applyOver(parentTransform);

    // Create nodes in graphs, using parent node IDs from retrieved metadata
    const unsigned int objectNodeId = _objects.addNode(std::move(object), parentMeta.objectNodeId);
    const unsigned int transformNodeId = _transforms.addNode(std::move(newTransform), parentMeta.transformNodeId);
    const unsigned int updateNodeId = _updateMarkers.addNode(false, parentMeta.updateNodeId);

    // Hook up the notification receiver of the scene with the notifier of the object transform
    std::function<void(const unsigned int)> callback = [this](const unsigned int id)
    {
        this->_objectTransformModified(id);
    };
    const unsigned int transformSubscriberId = objectRef.transform().getNotifier().addSubscriber(callback);

    // Create metadata
    _objectMetadata[objectRef.id] = {
        .id                     = objectRef.id,
        .parentId               = parentMeta.id,
        .objectNodeId           = objectNodeId,
        .transformNodeId        = transformNodeId,
        .updateNodeId           = updateNodeId,
        .transformSubscriberId  = transformSubscriberId
    };

    // Register object in dynamic collections
}

void Scene::_worldTransformDFSUpdate(const unsigned int startingId) const
{
    const SceneObjectMetadata meta = _objectMetadata.at(startingId);

    if (_updateMarkers[meta.updateNodeId]->value)
    {
        // If the current object is marked for update, update all transforms downwards
        _worldTransformCascadeUpdate(startingId);
    }
    else
    {
        // Otherwise, run the DFS routine on each of its children
        const ObjectTree::NodePtr object = _objects[meta.objectNodeId];
        const std::vector<ObjectTree::NodePtr> children = object->getChildren();

        for (const auto& child : children)
        {
            _worldTransformDFSUpdate(child->id);
        }
    }
}

std::vector<unsigned int> Scene::_findLongestOutdatedParentIdChain(const unsigned int id) const
{
    auto it = _objectMetadata.find(id);
    // Retrieve the object node ID, the node pointer as well as its parent chain
    const SceneObjectMetadata& meta = it->second;
    const ObjectTree::NodePtr objectNode = _objects[meta.objectNodeId];
    const std::vector<ObjectTree::NodePtr> parentChain = objectNode->getParentChain();

    std::vector<unsigned int> parentIdChain;
    parentIdChain.reserve(parentChain.size());

    // Find the count of elements in the chain up until the last outdated element
    unsigned int count = 0;
    unsigned int outdatedCount = 0;
    for (const auto& parent : parentChain)
    {
        count++;

        const unsigned int id = parent->value->id;
        parentIdChain.push_back(id);
        const SceneObjectMetadata meta = _objectMetadata.at(id);

        // Use the flag to check on the modified state of the object transform
        if (_updateMarkers[meta.updateNodeId]->value)
        {
            // Keep track of the element count up until the last outdated element
            outdatedCount = count;
        }
    }

    // If there are no outdated parents, return an empty vector
    if (outdatedCount == 0) return std::vector<unsigned int>();

    // If the last parent is outdated, return the whole ID vector
    if (outdatedCount == parentChain.size()) return parentIdChain;

    // Otherwise, copy only the outdated element IDs into a new vector and return it
    std::vector<unsigned int> outdatedChain;
    outdatedChain.reserve(outdatedCount);
    std::copy(parentIdChain.begin(), parentIdChain.begin() + outdatedCount, std::back_inserter(outdatedChain));

    return outdatedChain;
}

void Scene::_worldTransformCascadeUpdate(const unsigned int id) const
{
    auto it = _objectMetadata.find(id);
    // Retrieve node metadata
    const SceneObjectMetadata& meta = it->second;

    // Get object node, update object
    const ObjectTree::NodePtr objectNode = _objects[meta.objectNodeId];
    _worldTransformUpdateNoCascade(id);
    
    // Reverberate changes to children transforms
    const std::vector<ObjectTree::NodePtr> children = objectNode->getChildren();
    for (const auto child : children)
    {
        _worldTransformCascadeUpdate(child->value->id);
    }
}

void Scene::_worldTransformUpdateNoCascade(const unsigned int id) const
{
    auto it = _objectMetadata.find(id);
    const SceneObjectMetadata& meta = it->second;

    // Get object node and parent transform node
    const ObjectTree::NodePtr objectNode = _objects[meta.objectNodeId];
    const TransformTree::NodePtr transformNode = _transforms[meta.transformNodeId];
    const TransformTree::NodePtr parentTransformNode = transformNode->getParent();

    if (parentTransformNode != nullptr)
    {
        // Apply parent world transform to object transform
        const Transform objectTransform = (Transform)(objectNode->value->transform());
        const Transform newTransform = objectTransform.applyOver(parentTransformNode->value);
        
        // Save result to object world transform node
        transformNode->value = newTransform;
    }
    else
    {
        transformNode->value = (Transform)(objectNode->value->transform());
    }

    // Reset the update marker
    if (_updateMarkers[meta.updateNodeId]->value)
    {
        _updateMarkers[meta.updateNodeId]->value = false;
        _outdatedTransformCount--;
    }
}

bool Scene::_hasDisabledParent(const unsigned int id) const
{
    auto it = _objectMetadata.find(id);
    // Retrieve graph node IDs, object node pointer and node parent chain
    const SceneObjectMetadata meta = it->second;
    const ObjectTree::NodePtr objectNode = _objects[meta.objectNodeId];
    const std::vector<ObjectTree::NodePtr> parents = objectNode->getParentChain();

    for (const auto parent : parents)
    {
        // Return whether any parent in the chain is disabled
        if (!parent->value->enabled) return true;
    }
    return false;
}

} // namespace renderboi

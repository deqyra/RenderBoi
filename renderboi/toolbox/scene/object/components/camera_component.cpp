#include "camera_component.hpp"

#include "../../scene.hpp"
#include "../scene_object.hpp"
#include "../object_transform.hpp"

namespace renderboi
{

CameraComponent::CameraComponent(SceneObject& sceneObject, CameraPtr&& camera) :
    Component(sceneObject),
    _cameraPtr(std::move(camera)),
    _camera(*_cameraPtr)
{

}

CameraComponent::CameraComponent(SceneObject& sceneObject, Camera& camera) :
    Component(sceneObject),
    _cameraPtr(nullptr),
    _camera(camera)
{

}

CameraComponent::~CameraComponent()
{

}

Camera& CameraComponent::camera() 
{
    return _camera;
}

glm::mat4 CameraComponent::getViewMatrix() const
{
    // Update camera and compute view matrix
    _camera.setParentWorldTransform(_sceneObject.worldTransform());
    return _camera.getViewMatrix();
}

glm::vec3 CameraComponent::worldPositionToViewSpace(const glm::vec3 worldPosition) const
{
    glm::vec4 viewPosition = getViewMatrix() * glm::vec4(worldPosition, 1.f);
    return glm::vec3(viewPosition);
}

glm::mat4 CameraComponent::getProjectionMatrix() const
{
    return _camera.getProjectionMatrix();
}

glm::mat4 CameraComponent::getViewProjectionMatrix() const
{
    _camera.setParentWorldTransform(_sceneObject.worldTransform());
    return _camera.getViewProjectionMatrix();
}

CameraComponent* CameraComponent::clone(SceneObject& newParent) const
{
    return new CameraComponent(newParent, std::make_unique<Camera>(_camera));
}

} // namespace renderboi

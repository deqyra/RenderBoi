#include "camera.hpp"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "transform.hpp"

namespace renderboi
{

Camera::Camera(
    const CameraParameters cameraParameters,
    const ProjectionParameters projectionParameters,
    const Transform parentWorldTransform
) :
    ParentDependentVPMatrixProvider(parentWorldTransform),
    _forward(glm::vec3(0.f, 0.f, -1.f)),
    _left(glm::vec3(-1.f, 0.f, 0.f)),
    _up(glm::vec3(0.f, 1.f, 0.f)),
    _yaw(cameraParameters.yaw + 90.f),
    _pitch(cameraParameters.pitch),
    _zoomFactor(cameraParameters.zoomFactor),
    _verticalFov(projectionParameters.verticalFov),
    _aspectRatio(projectionParameters.aspectRatio),
    _nearDistance(projectionParameters.nearDistance),
    _farDistance(projectionParameters.farDistance)
{

}

Camera::Camera(const Camera& other) :
    ParentDependentVPMatrixProvider(*this),
    _forward(other._forward),
    _left(other._left),
    _up(other._up),
    _yaw(other._yaw),
    _pitch(other._pitch),
    _zoomFactor(other._zoomFactor),
    _verticalFov(other._verticalFov),
    _aspectRatio(other._aspectRatio),
    _nearDistance(other._nearDistance),
    _farDistance(other._farDistance)
{

}

void Camera::processRotation(const float yawOffset, const float pitchOffset)
{
    _yaw += yawOffset;
    if (_yaw > 180.f)  _yaw -= 360.f;
    if (_yaw < -180.f) _yaw += 360.f;

    _pitch += pitchOffset;
    // Constrain pitch
    if (_pitch > 89.0f)
        _pitch = 89.0f;
    if (_pitch < -89.0f)
        _pitch = -89.0f;

    _vectorsOutdated = true;
    _viewMatrixOutdated = true;
}

void Camera::processZoom(const float scrollOffset)
{
    // Not used
    if (_zoomFactor >= 1.0f && _zoomFactor <= 45.0f)
        _zoomFactor -= scrollOffset;
    if (_zoomFactor <= 1.0f)
        _zoomFactor = 1.0f;
    if (_zoomFactor >= 45.0f)
        _zoomFactor = 45.0f;
}

float Camera::getAspectRatio() const
{
    return _aspectRatio;
}

void Camera::setAspectRatio(const float aspectRatio)
{
    _aspectRatio = aspectRatio;
    _projectionMatrixOutdated = true;
    _vpMatrixOutdated = true;
}

float Camera::getVerticalFov() const
{
    return _verticalFov;
}

void Camera::setVerticalFov(const float verticalFov)
{
    _verticalFov = verticalFov;
    _projectionMatrixOutdated = true;
    _vpMatrixOutdated = true;
}

float Camera::getNearDistance() const
{
    return _nearDistance;
}

void Camera::setNearDistance(const float nearDistance)
{
    _nearDistance = nearDistance;
    _projectionMatrixOutdated = true;
    _vpMatrixOutdated = true;
}

float Camera::getFarDistance() const
{
    return _farDistance;
}

void Camera::setFarDistance(const float farDistance)
{
    _farDistance = farDistance;
    _projectionMatrixOutdated = true;
    _vpMatrixOutdated = true;
}

void Camera::_updateVectors() const
{
    if (!_vectorsOutdated)
    {
        return;
    }

    glm::vec3 forward;
    // Compute new front based on yaw and pitch
    forward.x = glm::cos(glm::radians(_yaw)) * glm::cos(glm::radians(_pitch));
    forward.y = glm::sin(glm::radians(_pitch));
    forward.z = glm::sin(glm::radians(_yaw)) * glm::cos(glm::radians(_pitch));
    // Apply the world rotation of the parent
    forward = _parentWorldTransform.getRotation() * forward;
    _forward = glm::normalize(forward);

    // Compute new left and up from new forward and parent up
    _left = glm::normalize(glm::cross(_parentWorldTransform.up(), _forward));
    _up = glm::normalize(glm::cross(_forward, _left));

    _vectorsOutdated = false;
}

glm::vec3 Camera::forward() const
{
    _updateVectors();
    return _forward;
}

glm::vec3 Camera::left() const
{
    _updateVectors();
    return _left;
}

glm::vec3 Camera::up() const
{
    _updateVectors();
    return _up;
}

glm::mat4 Camera::_computeProjectionMatrix() const
{
    return glm::perspective(_verticalFov, _aspectRatio, _nearDistance, _farDistance);
}

glm::mat4 Camera::_computeViewMatrix() const
{
    _updateVectors();
    return glm::lookAt(_parentWorldTransform.getPosition(), _parentWorldTransform.getPosition() + _forward, _up);
}

} // namespace renderboi

#include "camera.hpp"

#include <renderboi/core/numeric.hpp>
#include <renderboi/core/3d/transform.hpp>

namespace rb {

Camera::Camera(const ViewParameters viewParams, const ProjectionParameters projParams)
    : BasisProvider()
    , _basis()
    , _localZOutdated(true)
    , _basisOutdated(true)
    , _projectionMatrix()
    , _projectionMatrixOutdated(true)
    , up          (_localZOutdated, viewParams.up)
    , yaw         (_localZOutdated, viewParams.yaw)
    , pitch       (_localZOutdated, viewParams.pitch)
    , verticalFov (_projectionMatrixOutdated, projParams.verticalFov)
    , zoom        (_projectionMatrixOutdated, projParams.zoom)
    , aspectRatio (_projectionMatrixOutdated, projParams.aspectRatio)
    , near        (_projectionMatrixOutdated, projParams.near)
    , far         (_projectionMatrixOutdated, projParams.far)
{

}

Camera::Camera(const Camera& other)
    : Camera(
        ViewParameters{
            .up          = other.up,
            .yaw         = other.yaw,
            .pitch       = other.pitch,
        },
        ProjectionParameters{
            .verticalFov = other.verticalFov,
            .zoom        = other.zoom,
            .aspectRatio = other.aspectRatio,
            .near        = other.near,
            .far         = other.far,
        }
    )
{

}

void Camera::_recalculateLocalZ() const {
    // Compute new forward based on yaw and pitch
    _basis.z = num::Vec3(
        num::sin(yaw) * num::cos(pitch),
                        num::sin(pitch),
        num::cos(yaw) * glm::cos(pitch)
    );

    _localZOutdated = false;
    _basisOutdated = true; // local Z updated, rest of basis needs recomputing
}

void Camera::_recalculateBasisFromZ() const {
    _basis.z = num::normalize(_basis.z);
    // Compute new left from new forward and parent up
    _basis.x = num::normalize(num::cross(up.get(), _basis.z));
    // Compute new up from new forward and new left
    _basis.y = num::normalize(num::cross(_basis.z, _basis.x));

    _basisOutdated = false;
}


void Camera::_recalculateProjectionMatrix() const {
    _projectionMatrix = num::perspective(
        verticalFov.get() / zoom.get(),
        aspectRatio.get(),
        near.get(),
        far.get()
    );

    _projectionMatrixOutdated = false;
}

void Camera::processYawPitch(float yawOffset, float pitchOffset) {
    yaw += yawOffset;
    if (yaw > 180.f)  yaw -= 360.f;
    if (yaw < -180.f) yaw += 360.f;

    pitch = num::fclamp(pitch + pitchOffset, -89.f, 89.f);
}

num::Mat4 Camera::viewMatrix(const num::Vec3& position) const {
    if (_localZOutdated) {
        _recalculateLocalZ();
    }

    return num::lookAt(
        position,
        position + _basis.z,
        up.get()
    );
}

num::Mat4 Camera::projMatrix() const {
    if (_projectionMatrixOutdated) {
        _recalculateProjectionMatrix();
    }

    return _projectionMatrix;
}

Basis Camera::basis() const {
    if (_localZOutdated) {
        _recalculateLocalZ();
    }
    if (_basisOutdated) {
        _recalculateBasisFromZ();
    }

    return _basis;
}

} // namespace rb

#ifndef RENDERBOI_CORE_CAMERA_HPP
#define RENDERBOI_CORE_CAMERA_HPP

#include <renderboi/core/numeric.hpp>
#include <renderboi/core/3d/transform.hpp>
#include <renderboi/core/3d/basis_provider.hpp>

#include <cpptools/utility/monitored_value.hpp>

namespace rb {

struct ViewParameters {
    num::Vec3 up    = num::Y;
    float     yaw   = 0.f;
    float     pitch = 0.f;
};

struct ProjectionParameters {
    float verticalFov = num::radians(45.f);
    float zoom        = 1.f;
    float aspectRatio = 16.f / 9.f;
    float near        = 0.1f;
    float far         = 100.f;
};

/// @brief A camera managing its own orientation and providing view and
/// projection matrices
class Camera : public BasisProvider {
private:
    /// @brief Basis
    mutable Basis _basis;

    /// @brief Whether or not the local basis needs recalculating to reflect parameters
    mutable bool _localZOutdated;

    /// @brief Whether or not the local basis needs recalculating to reflect parameters
    mutable bool _basisOutdated;

    /// @brief Projection matrix
    mutable num::Mat4 _projectionMatrix;

    /// @brief Whether or not the projection matrix needs recalculating to reflect parameters
    mutable bool _projectionMatrixOutdated;

    /// @brief Recalculate local basis to match camera rotation parameters
    void _recalculateLocalZ() const;

    /// @brief Recalculate local basis to match camera rotation parameters
    void _recalculateBasisFromZ() const;

    /// @brief Recalculate projection matrix to ma1ch camera projection parameters
    void _recalculateProjectionMatrix() const;

public:
    Camera(ViewParameters viewParams = {}, ProjectionParameters projParams = {});

    Camera(const Camera& other);

    /// @brief Up direction of the camera in world coordinates
    tools::monitored_value<num::Vec3> up;

    /// @brief Yaw (rotation around the up vector) of the camera in radians
    tools::monitored_value<float> yaw;

    /// @brief Pitch (rotation around X) of the camera in radians
    tools::monitored_value<float> pitch;

    /// @brief Angle (in radians) of the vertical field of view of the camera
    tools::monitored_value<float> verticalFov;

    /// @brief Zoom factor of the camera
    tools::monitored_value<float> zoom;

    /// @brief Aspect ratio of the image produced by the camera
    tools::monitored_value<float> aspectRatio;

    /// @brief Distance to the near plane of the camera, in front of which
    /// elements will not be rendered
    tools::monitored_value<float> near;

    /// @brief Distance to the far plane of the camera, beyond which elements
    /// will not be rendered
    tools::monitored_value<float> far;

    /// @brief Add to the camera's pitch and yaw, keeping them in a constrained
    /// range of [-180, 180[ for the yaw and [-89, 89] for the pitch
    void processYawPitch(float yawOffset, float pitchOffset);

    /// @brief Get the view matrix of the camera
    num::Mat4 viewMatrix(const num::Vec3& position) const;

    /// @brief Get the projection matrix of the camera
    num::Mat4 projMatrix() const;

    /////////////////////////////////////////////
    /// Methods overridden from BasisProvider ///
    /////////////////////////////////////////////

    /// @copydoc BasisProvider::basis
    Basis basis() const override;
};

} // namespace rb

#endif // RENDERBOI_CORE_CAMERA_HPP

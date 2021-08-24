#ifndef RENDERBOI__CORE__CAMERA_HPP
#define RENDERBOI__CORE__CAMERA_HPP

#include <memory>
#include <glm/glm.hpp>
#include <glm/gtx/quaternion.hpp>

#include "interfaces/basis_provider.hpp"
#include "implementation/parent_dependent_vp_matrix_provider.hpp"
#include "transform.hpp"

namespace Renderboi
{

/// @brief A camera managing its own orientation and providing view and projection matrices.
class Camera : public BasisProvider, public ParentDependentVPMatrixProvider
{
private:
    /// @brief Direction the camera is facing.
    mutable glm::vec3 _forward;

    /// @brief Direction to the left of the camera.
    mutable glm::vec3 _left;

    /// @brief Upward direction of the camera.
    mutable glm::vec3 _up;

    /// @brief Whether or not local vectors need recalculating in order to 
    /// reflect new parameters.
    mutable bool _vectorsOutdated;

    /// @brief Yaw (rotation around Y) of the camera in degrees.
    float _yaw;

    /// @brief Pitch (rotation around X) of the camera in degrees.
    float _pitch;

    /// @brief Zoom factor of the camera.
    float _zoomFactor;

    /// @brief Angle (in radians) of the vertical field of view of the 
    /// camera.
    float _verticalFov;

    /// @brief Aspect ratio of the image produced by the camera.
    float _aspectRatio;

    /// @brief Distance to the near plane of the camera, before which 
    /// elements will not be rendered.
    float _nearDistance;

    /// @brief Distance to the far plane of the camera, beyond which 
    /// elements will not be rendered.
    float _farDistance;

    /// @brief Recalculate local vectors so they match camera rotation parameters.
    void _updateVectors() const;

public:
    struct CameraParameters
    {
        float yaw;
        float pitch;
        float zoomFactor;
    };

    static constexpr CameraParameters DefaultCameraParameters = {0.f, 0.f, 1.f};

    struct ProjectionParameters
    {
        float verticalFov;
        float aspectRatio;
        float nearDistance;
        float farDistance;
    };

    static constexpr ProjectionParameters DefaultProjectionParameters = {glm::radians(45.f), 16.f / 9.f, 0.1f, 100.f};

    /// @param cameraParameters Parameters of the camera: yaw, pitch, zoom.
    /// @param projectionParameters Parameters of the projection used by the
    /// camera: vertical FOV, aspect ratio, near and far plane distances.
    /// @param parentTransform Transform of the object the camera is 
    /// attached to.
    Camera(
        const CameraParameters cameraParameters = DefaultCameraParameters,
        const ProjectionParameters projectionParameters = DefaultProjectionParameters,
        const Transform parentTransform = Transform()
    );

    Camera(const Camera& other);

    /// @brief Update the rotation of the camera to fit recorded offsets in 
    /// yaw and pitch.
    ///
    /// @param yawOffset How much the camera was rotated around Y in degrees.
    /// @param pitchOffset How much the camera was rotated around X in degrees.
    void processRotation(const float yawOffset, const float pitchOffset);
    
    /// @brief DO NOT USE - Update the projection matrix of the camera to 
    /// zoom in or out.
    ///
    /// @param scrollOffset How much the camera was zoomed in or out.
    void processZoom(const float scrollOffset);

    /// @brief Get the vertical field of view of the camera.
    ///
    /// @return The angle of the vertical field of view of the camera.
    float getVerticalFov() const;

    /// @brief Set the vertical field of view of the camera.
    ///
    /// @param verticalFov The new angle of the vertical field of view of 
    /// the camera.
    void setVerticalFov(const float verticalFov);

    /// @brief Get the aspect ratio of the image produced by the camera.
    ///
    /// @return The aspect ratio of the image produced by the camera.
    float getAspectRatio() const;

    /// @brief Set the aspect ratio of the image produced by the camera.
    ///
    /// @param aspectRatio The new aspect ratio of the camera.
    void setAspectRatio(const float aspectRatio);

    /// @brief Get the distance to the near plane of the camera.
    ///
    /// @return The distance to the near plane of the camera.
    float getNearDistance() const;

    /// @brief Set the distance to the near plane of the camera.
    ///
    /// @param nearDistance The new distance to the near plane of the camera.
    void setNearDistance(const float nearDistance);

    /// @brief Get the distance to the far plane of the camera.
    ///
    /// @return The distance to the far plane of the camera.
    float getFarDistance() const;

    /// @brief Set the distance to the far plane of the camera.
    ///
    /// @param farDistance The new distance to the far plane of the camera.
    void setFarDistance(const float farDistance);

    /////////////////////////////////////////////
    ///                                       ///
    /// Methods overridden from BasisProvider ///
    ///                                       ///
    /////////////////////////////////////////////

    /// @brief Get the facing direction of the camera.
    ///
    /// @return The direction the camera is facing.
    glm::vec3 forward() const override;

    /// @brief Get the direction to the left of the camera.
    ///
    /// @return The direction to the left of the camera.
    glm::vec3 left() const override;

    /// @brief Get the upwards direction of the camera.
    ///
    /// @return The upwards direction of the camera.
    glm::vec3 up() const override;

protected:
    ///////////////////////////////////////////////////////////////
    ///                                                         ///
    /// Methods overridden from ParentDependentVPMatrixProvider ///
    ///                                                         ///
    ///////////////////////////////////////////////////////////////

    /// @brief Compute the actual projection matrix using camera lens parameters.
    virtual glm::mat4 _computeProjectionMatrix() const override;

    /// @brief Compute the actual view matrix using camera rotation parameters.
    virtual glm::mat4 _computeViewMatrix() const override;
};

using CameraPtr = std::shared_ptr<Camera>;
using CameraWPtr = std::weak_ptr<Camera>;

}//namespace Renderboi

#endif//RENDERBOI__CORE__CAMERA_HPP
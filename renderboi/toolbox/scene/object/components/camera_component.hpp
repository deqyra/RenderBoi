#ifndef RENDERBOI__TOOLBOX__SCENE__COMPONENTS__CAMERA_COMPONENT_HPP
#define RENDERBOI__TOOLBOX__SCENE__COMPONENTS__CAMERA_COMPONENT_HPP

#include <string>

#include <renderboi/core/camera.hpp>

namespace renderboi
{

/// @brief Component allowing to attach a camera to a scene object.
class CameraComponent
{
private:
    CameraComponent(CameraComponent& other) = delete;
    CameraComponent& operator=(const CameraComponent& other) = delete;

    /// @brief Pointer to the camera of the object, if owned. Is null when the
    /// referenced camera isn't owned by the instance.
    CameraPtr _cameraPtr;

    /// @brief Reference to the camera.
    Camera& _camera;

public:
    /// @param camera Reference to the camera for the component to use.
    ///
    /// @exception If the provided camera pointer is null, the 
    /// constructor will throw a std::runtime_error.
    CameraComponent(CameraPtr&& camera);

    /// @param camera Reference to the camera for the component to use.
    CameraComponent(Camera& camera);

    ~CameraComponent();

    /// @brief Get the camera used by the component.
    ///
    /// @return The camera used by the component.
    Camera& camera();

    /// @brief Get the view matrix corresponding to the position and facing
    /// of the camera.
    ///
    /// @return The view matrix provided by the camera.
    glm::mat4 getViewMatrix() const;

    /// @brief Get the view space coordinates of a position given in world 
    /// space coordinates.
    ///
    /// @param worldPosition The position whose coordinates to transform.
    ///
    /// @return View space coordinates of the provided world position.
    glm::vec3 worldPositionToViewSpace(const glm::vec3 worldPosition) const;

    /// @brief Get the projection matrix of the camera.
    ///
    /// @return The projection matrix provided by the camera.
    glm::mat4 getProjectionMatrix() const;

    /// @brief Get the view-projection matrix of the camera.
    ///
    /// @return The view-projection matrix provided by the camera.
    glm::mat4 getViewProjectionMatrix() const;
};

} // namespace renderboi

#endif//RENDERBOI__TOOLBOX__SCENE__COMPONENTS__CAMERA_COMPONENT_HPP
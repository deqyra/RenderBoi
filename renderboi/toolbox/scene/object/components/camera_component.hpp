#ifndef RENDERBOI__TOOLBOX__SCENE__COMPONENTS__CAMERA_COMPONENT_HPP
#define RENDERBOI__TOOLBOX__SCENE__COMPONENTS__CAMERA_COMPONENT_HPP

#include <string>

#include <renderboi/core/camera.hpp>

#include "../component.hpp"
#include "../component_type.hpp"

namespace renderboi
{

/// @brief Component allowing to attach a camera to a scene object.
class CameraComponent : public Component
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
    /// @param sceneObject Reference to the scene object which will be parent
    /// to this component.
    /// @param camera Reference to the camera for the component to use.
    ///
    /// @exception If the provided camera pointer is null, the 
    /// constructor will throw a std::runtime_error.
    CameraComponent(SceneObject& sceneObject, CameraPtr&& camera);

    /// @param sceneObject Reference to the scene object which will be parent
    /// to this component.
    /// @param camera Reference to the camera for the component to use.
    CameraComponent(SceneObject& sceneObject, Camera& camera);

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

    /////////////////////////////////////////
    ///                                   ///
    /// Methods overridden from Component ///
    ///                                   ///
    /////////////////////////////////////////

    /// @brief Get a raw pointer to a new component instance cloned 
    /// from this one. Ownership and responsibility for the allocated 
    /// resources are fully transferred to the caller.
    ///
    /// @param newParent Pointer the scene object which will be parent to
    /// the cloned component instance.
    ///
    /// @return A raw pointer to the component instance cloned from this 
    /// one.
    CameraComponent* clone(SceneObject& newParent) const override;
};

template<>
struct ComponentMeta<ComponentType::Camera>
{
    struct MultipleInstancesAllowed
    {
        static constexpr bool value = false;
    };

    struct ConcreteType
    {
        using type = CameraComponent;
    };

    struct Name
    {
        static constexpr const char* value = "CameraComponent";
    };
};

template<>
struct ComponentTypeToEnum<CameraComponent>
{
    static constexpr ComponentType value = ComponentType::Camera;
};

} // namespace renderboi

#endif//RENDERBOI__TOOLBOX__SCENE__COMPONENTS__CAMERA_COMPONENT_HPP
#ifndef RENDERBOI_TOOLBOX_RUNNABLES_GAMEPAD_CAMERA_MANAGER_HPP
#define RENDERBOI_TOOLBOX_RUNNABLES_GAMEPAD_CAMERA_MANAGER_HPP

#include <renderboi/core/numeric.hpp>
#include <renderboi/core/3d/camera.hpp>

#include <renderboi/window/gamepad/gamepad_input_processor.hpp>
#include <renderboi/window/gamepad/gamepad.hpp>

#include "../script.hpp"

namespace rb {

/// @brief Provides event callbacks to manage a camera as in a FPS game This 
/// script must be attached to an object which has a camera component, otherwise
/// an std::runtime_error will be thrown upon attaching
class GamepadCameraManager : public GamepadInputProcessor, public Script {
private:
    GamepadCameraManager(const GamepadCameraManager&) = default;
    GamepadCameraManager(GamepadCameraManager&&) = default;
    GamepadCameraManager& operator=(const GamepadCameraManager&) = delete;
    GamepadCameraManager& operator=(GamepadCameraManager&&) = delete;

    /// @brief Reference to the camera controlled by the script
    Camera& _camera;

    /// @brief Camera sensitivity (right thumbstick)
    float _lookSensitivity;

    /// @brief Direction in which the camera look is requested to move in
    num::Vec2 _direction;

public:
    /// @brief The default thumbstick sensitivity
    static constexpr float DefaultLookSensitivity = 0.1f;

    /// @param camera Reference to the camera whose orientation to manage
    /// @param sensitivity The amplitude of the rotation induced by the right 
    /// thumbstick
    GamepadCameraManager(Camera& camera, const float sensitivity = DefaultLookSensitivity);

    //////////////////////////////////////
    ///                                ///
    /// Methods overridden from Script ///
    ///                                ///
    //////////////////////////////////////

    /// @brief Make the script run and do its things
    ///
    /// @param timeElapsed How much time passed (in seconds) since the last
    /// update
    void update(const float timeElapsed) override;

    /////////////////////////////////////////////////////
    ///                                               ///
    /// Methods overridden from GamepadInputProcessor ///
    ///                                               ///
    /////////////////////////////////////////////////////

    /// @brief Callback for a gamepad axis event
    ///
    /// @param axis Literal describing which axis triggered the event
    /// @param value Value at which the axis was polled
    virtual void processAxis(const Gamepad& gamepad, const Window::Input::Gamepad::Axis axis, const float value) override;
};

} // namespace rb

#endif//RENDERBOI_TOOLBOX_RUNNABLES_GAMEPAD_CAMERA_MANAGER_HPP
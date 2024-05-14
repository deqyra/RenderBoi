#include "gamepad_camera_manager.hpp"

namespace rb {

GamepadCameraManager::GamepadCameraManager(Camera& camera, const float sensitivity) :
    _camera(camera),
    _lookSensitivity(sensitivity),
    _direction{0.f, 0.f}
{

}

void GamepadCameraManager::update(const float timeElapsed) {
    _camera.processYawPitch(_direction.x, _direction.y);
}

void GamepadCameraManager::processAxis(const Gamepad& gamepad, const Window::Input::Gamepad::Axis axis, const float value) {
    if (axis == Window::Input::Gamepad::Axis::RightX)
        _direction.x = value;

    if (axis == Window::Input::Gamepad::Axis::RightY)
        _direction.y = -value;
}

} // namespace rb

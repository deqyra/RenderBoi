#include "gamepad_camera_manager.hpp"

namespace renderboi
{

GamepadCameraManager::GamepadCameraManager(Camera& camera, const float sensitivity) :
    _camera(camera),
    _lookSensitivity(sensitivity),
    _direction{0.f, 0.f}
{

}

void GamepadCameraManager::update(const float timeElapsed)
{
    float velocity = timeElapsed * _lookSensitivity;

    _camera.processRotation(_direction.x, _direction.y);
}

void GamepadCameraManager::setSceneObject(SceneObject* const sceneObject)
{
    if (sceneObject)
    {
        std::string s = "GamepadCameraManager: may not be linked to a scene object.";
        throw std::runtime_error(s.c_str());
    }
}

GamepadCameraManager* GamepadCameraManager::clone() const
{
    return new GamepadCameraManager(_camera, _lookSensitivity);
}

void GamepadCameraManager::processAxis(const Gamepad& gamepad, const Window::Input::Gamepad::Axis axis, const float value)
{
    if (axis == Window::Input::Gamepad::Axis::RightX)
        _direction.x = value;

    if (axis == Window::Input::Gamepad::Axis::RightY)
        _direction.y = -value;
}

} // namespace renderboi

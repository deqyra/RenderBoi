#include "keyboard_movement_script.hpp"

#include <stdexcept>

#include <renderboi/core/frame_of_reference.hpp>

#include <renderboi/window/enums.hpp>

namespace renderboi
{

using Ref = FrameOfReference;

KeyboardMovementScript::KeyboardMovementScript(
    BasisProvider& basisProvider,
    const float speed,
    const float sprintMultiplier
) :
    _basisProvider(basisProvider),
    _moveSpeed(speed),
    _sprintMultiplier(sprintMultiplier),
    _movementFlags{ false },
    _sprint(false)
{

}

void KeyboardMovementScript::update(const float timeElapsed)
{
    // Compute distance to cover in this frame
    float velocity = timeElapsed * _moveSpeed;
    if (_sprint)
        velocity *= _sprintMultiplier;

    // Retrieve the linked scene object.
    glm::vec3 position = _sceneObject->transform().getPosition();

    // Compute velocity across basis vectors
    float forwardVelocity = 0.f;
    float leftVelocity = 0.f;

    if (_movementFlags[_IndexForward])
        forwardVelocity += velocity;
    if (_movementFlags[_IndexBackward])
        forwardVelocity -= velocity;
    if (_movementFlags[_IndexLeft])
        leftVelocity += velocity;
    if (_movementFlags[_IndexRight])
        leftVelocity -= velocity;

    // Compute new position
    position += _basisProvider.forward() * forwardVelocity;
    position += _basisProvider.left() * leftVelocity;

    // Update parent position
    _sceneObject->transform().setPosition<Ref::Parent>(position);
}

void KeyboardMovementScript::setSceneObject(SceneObject* const sceneObject)
{
    if (!sceneObject)
    {
        std::string s = "KeyboardMovementScript: null scene object pointer was provided.";
        throw std::runtime_error(s.c_str());
    }

    _sceneObject = sceneObject;
}

KeyboardMovementScript* KeyboardMovementScript::clone() const
{
    return new KeyboardMovementScript(_basisProvider, _moveSpeed, _sprintMultiplier);
}

void KeyboardMovementScript::triggerAction(const KeyboardMovementAction& action)
{
    switch (action)
    {
        case KeyboardMovementAction::Forward:
            _movementFlags[_IndexForward] = true;
            break;
        case KeyboardMovementAction::Backward:
            _movementFlags[_IndexBackward] = true;
            break;
        case KeyboardMovementAction::Left:
            _movementFlags[_IndexLeft] = true;
            break;
        case KeyboardMovementAction::Right:
            _movementFlags[_IndexRight] = true;
            break;
        case KeyboardMovementAction::Sprint:
            _sprint = true;
            break;
    }
}

void KeyboardMovementScript::stopAction(const KeyboardMovementAction& action)
{
    switch (action)
    {
        case KeyboardMovementAction::Forward:
            _movementFlags[_IndexForward] = false;
            break;
        case KeyboardMovementAction::Backward:
            _movementFlags[_IndexBackward] = false;
            break;
        case KeyboardMovementAction::Left:
            _movementFlags[_IndexLeft] = false;
            break;
        case KeyboardMovementAction::Right:
            _movementFlags[_IndexRight] = false;
            break;
        case KeyboardMovementAction::Sprint:
            _sprint = false;
            break;
    }
}

const ControlScheme<KeyboardMovementAction>& KeyboardMovementScript::getDefaultControlScheme() const
{
    static ControlScheme<KeyboardMovementAction> scheme;
    static bool runOnce = false;

    if (!runOnce)
    {
        using Window::Input::Key;
        scheme.bindControl(Control(Key::W), KeyboardMovementAction::Forward);
        scheme.bindControl(Control(Key::A), KeyboardMovementAction::Left);
        scheme.bindControl(Control(Key::S), KeyboardMovementAction::Backward);
        scheme.bindControl(Control(Key::D), KeyboardMovementAction::Right);
        scheme.bindControl(Control(Key::LeftShift), KeyboardMovementAction::Sprint);

        runOnce = true;
    }

    return scheme;
}

std::string to_string(const KeyboardMovementAction action)
{
    static std::unordered_map<KeyboardMovementAction, std::string> enumNames = {
        {KeyboardMovementAction::Forward,   "Forward"},
        {KeyboardMovementAction::Backward,  "Backward"},
        {KeyboardMovementAction::Left,      "Left"},
        {KeyboardMovementAction::Right,     "Right"},
        {KeyboardMovementAction::Sprint,    "Sprint"},
    };

    auto it = enumNames.find(action);
    return (it != enumNames.end())
        ? it->second
        : "Unknown";
}

} // namespace renderboi

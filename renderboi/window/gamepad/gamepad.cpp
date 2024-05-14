#include "../enums.hpp"
#include "gamepad.hpp"
#include "gamepad_input_processor.hpp"
#include "gamepad_manager.hpp"
#include "gamepad_state.hpp"

#include <unordered_map>
#include <utility>

namespace rb::Window {

using Axis = Window::Input::Gamepad::Axis;
using Button = Window::Input::Gamepad::Button;

GamepadInputProcessorPtr Gamepad::_DefaultInputProcessor = std::make_unique<GamepadInputProcessor>();

Gamepad::Gamepad(GamepadManager& manager, const Window::Input::Joystick slot, const std::string name) :
    _state(),
    _enabled(false),
    _axisDeadZones(DefaultAxisDeadZones()),
    _axisWasInDeadZone(),
    _inputProcessor(_DefaultInputProcessor.get()),
    _manager(manager),
    slot(slot),
    name(name)
{
    _axisWasInDeadZone[Axis::LeftX]         = true;
    _axisWasInDeadZone[Axis::LeftY]         = true;
    _axisWasInDeadZone[Axis::RightX]        = true;
    _axisWasInDeadZone[Axis::RightY]        = true;
    _axisWasInDeadZone[Axis::LeftTrigger]   = true;
    _axisWasInDeadZone[Axis::RightTrigger]  = true;
}

void Gamepad::enable() {
    if (!_enabled)
    {
        _manager.startGamepadPolling(slot);

        _enabled = true;
    }
}

void Gamepad::disable() {
    if (_enabled)
    {
        _manager.stopGamepadPolling(slot);

        _enabled = false;
    }
}

void Gamepad::setEnabled(const bool enabled) {
    enabled ? enable() : disable();
}

const GamepadState& Gamepad::getState() const {
    return _state;
}

bool Gamepad::isPressed(const Button button) const {
    return _state[button];
}

float Gamepad::getValue(const Axis axis) const {
    return _state[axis];
}

GamepadInputProcessor& Gamepad::getInputProcessor() const {
    return *_inputProcessor;
}

void Gamepad::registerInputProcessor(GamepadInputProcessor& inputProcessor) {
    _inputProcessor = &inputProcessor;
}

void Gamepad::detachInputProcessor() {
    _inputProcessor = _DefaultInputProcessor.get();
}

const std::pair<float, float>& Gamepad::getAxisDeadZone(const Axis axis) const {
    return _axisDeadZones.at(axis);
}

void Gamepad::setAxisDeadZone(const Axis axis, std::pair<float, float> deadZone) {
    _axisDeadZones[axis] = deadZone;
}

const std::unordered_map<Axis, std::pair<float, float>>& Gamepad::DefaultAxisDeadZones() {
    static std::unordered_map<Axis, std::pair<float, float>> defaultDeadZones = {
        { Axis::LeftX,        {-.1f, .1f} },
        { Axis::LeftY,        {-.1f, .1f} },
        { Axis::RightX,       {-.1f, .1f} },
        { Axis::RightY,       {-.1f, .1f} },
        { Axis::LeftTrigger,  {-1.f, -.95f} },
        { Axis::RightTrigger, {-1.f, -.95f} }
    };

    return defaultDeadZones;
}

const std::unordered_map<Axis, float>& Gamepad::AxisRestValues() {
    static std::unordered_map<Axis, float> axisRestValues = {
        { Axis::LeftX,          0.f },
        { Axis::LeftY,          0.f },
        { Axis::RightX,         0.f },
        { Axis::RightY,         0.f },
        { Axis::LeftTrigger,   -1.f },
        { Axis::RightTrigger,  -1.f }
    };

    return axisRestValues;
}

void Gamepad::_processConnected() {
    _inputProcessor->processConnected(*this);
}

void Gamepad::_processDisconnected() {
    _inputProcessor->processDisconnected(*this);
}

void Gamepad::_setNewState(const GamepadState newState) {
    _compareAndFireEvents(_state, newState);
    _state = newState;
}

void Gamepad::_compareAndFireEvents(const GamepadState& before, const GamepadState& after) {
    using Window::Input::Action;

    if (before.A != after.A)
    {
        after.A
            ? _inputProcessor->processButton(*this, Button::A, Action::Press)
            : _inputProcessor->processButton(*this, Button::A, Action::Release);
    }

    if (before.B != after.B)
    {
        after.B
            ? _inputProcessor->processButton(*this, Button::B, Action::Press)
            : _inputProcessor->processButton(*this, Button::B, Action::Release);
    }

    if (before.X != after.X)
    {
        after.X
            ? _inputProcessor->processButton(*this, Button::X, Action::Press)
            : _inputProcessor->processButton(*this, Button::X, Action::Release);
    }

    if (before.Y != after.Y)
    {
        after.Y
            ? _inputProcessor->processButton(*this, Button::Y, Action::Press)
            : _inputProcessor->processButton(*this, Button::Y, Action::Release);
    }

    if (before.LeftBumper != after.LeftBumper)
    {
        after.LeftBumper
            ? _inputProcessor->processButton(*this, Button::LeftBumper, Action::Press)
            : _inputProcessor->processButton(*this, Button::LeftBumper, Action::Release);
    }

    if (before.RightBumper != after.RightBumper)
    {
        after.RightBumper
            ? _inputProcessor->processButton(*this, Button::RightBumper, Action::Press)
            : _inputProcessor->processButton(*this, Button::RightBumper, Action::Release);
    }

    if (before.DPadUp != after.DPadUp)
    {
        after.DPadUp
            ? _inputProcessor->processButton(*this, Button::DPadUp, Action::Press)
            : _inputProcessor->processButton(*this, Button::DPadUp, Action::Release);
    }

    if (before.DPadRight != after.DPadRight)
    {
        after.DPadRight
            ? _inputProcessor->processButton(*this, Button::DPadRight, Action::Press)
            : _inputProcessor->processButton(*this, Button::DPadRight, Action::Release);
    }

    if (before.DPadDown != after.DPadDown)
    {
        after.DPadDown
            ? _inputProcessor->processButton(*this, Button::DPadDown, Action::Press)
            : _inputProcessor->processButton(*this, Button::DPadDown, Action::Release);
    }

    if (before.DPadLeft != after.DPadLeft)
    {
        after.DPadLeft
            ? _inputProcessor->processButton(*this, Button::DPadLeft, Action::Press)
            : _inputProcessor->processButton(*this, Button::DPadLeft, Action::Release);
    }

    if (before.LeftThumb != after.LeftThumb)
    {
        after.LeftThumb
            ? _inputProcessor->processButton(*this, Button::LeftThumb, Action::Press)
            : _inputProcessor->processButton(*this, Button::LeftThumb, Action::Release);
    }

    if (before.RightThumb != after.RightThumb)
    {
        after.RightThumb
            ? _inputProcessor->processButton(*this, Button::RightThumb, Action::Press)
            : _inputProcessor->processButton(*this, Button::RightThumb, Action::Release);
    }

    if (before.Start != after.Start)
    {
        after.Start
            ? _inputProcessor->processButton(*this, Button::Start, Action::Press)
            : _inputProcessor->processButton(*this, Button::Start, Action::Release);
    }

    if (before.Select != after.Select)
    {
        after.Select
            ? _inputProcessor->processButton(*this, Button::Select, Action::Press)
            : _inputProcessor->processButton(*this, Button::Select, Action::Release);
    }

    if (before.Home != after.Home)
    {
        after.Home
            ? _inputProcessor->processButton(*this, Button::Home, Action::Press)
            : _inputProcessor->processButton(*this, Button::Home, Action::Release);
    }

    bool dead = true;

    dead = _valueIsInAxisDeadZone(after.LeftX, Axis::LeftX);
    if (!_axisWasInDeadZone[Axis::LeftX] && dead)
    {
        _axisWasInDeadZone[Axis::LeftX] = true;
        _inputProcessor->processAxis(*this, Axis::LeftX, AxisRestValues().at(Axis::LeftX));
    }
    else if (!dead && (before.LeftX != after.LeftX))
    {
        _axisWasInDeadZone[Axis::LeftX] = false;
        _inputProcessor->processAxis(*this, Axis::LeftX, after.LeftX);
    }

    dead = _valueIsInAxisDeadZone(after.LeftY, Axis::LeftY);
    if (!_axisWasInDeadZone[Axis::LeftY] && dead)
    {
        _axisWasInDeadZone[Axis::LeftY] = true;
        _inputProcessor->processAxis(*this, Axis::LeftY, AxisRestValues().at(Axis::LeftY));
    }
    else if (!dead && (before.LeftY != after.LeftY))
    {
        _axisWasInDeadZone[Axis::LeftY] = false;
        _inputProcessor->processAxis(*this, Axis::LeftY, after.LeftY);
    }

    dead = _valueIsInAxisDeadZone(after.RightX, Axis::RightX);
    if (!_axisWasInDeadZone[Axis::RightX] && dead)
    {
        _axisWasInDeadZone[Axis::RightX] = true;
        _inputProcessor->processAxis(*this, Axis::RightX, AxisRestValues().at(Axis::RightX));
    }
    else if (!dead && (before.RightX != after.RightX))
    {
        _axisWasInDeadZone[Axis::RightX] = false;
        _inputProcessor->processAxis(*this, Axis::RightX, after.RightX);
    }

    dead = _valueIsInAxisDeadZone(after.RightY, Axis::RightY);
    if (!_axisWasInDeadZone[Axis::RightY] && dead)
    {
        _axisWasInDeadZone[Axis::RightY] = true;
        _inputProcessor->processAxis(*this, Axis::RightY, AxisRestValues().at(Axis::RightY));
    }
    else if (!dead && (before.RightY != after.RightY))
    {
        _axisWasInDeadZone[Axis::RightY] = false;
        _inputProcessor->processAxis(*this, Axis::RightY, after.RightY);
    }

    dead = _valueIsInAxisDeadZone(after.LeftTrigger, Axis::LeftTrigger);
    if (!_axisWasInDeadZone[Axis::LeftTrigger] && dead)
    {
        _axisWasInDeadZone[Axis::LeftTrigger] = true;
        _inputProcessor->processAxis(*this, Axis::LeftTrigger, AxisRestValues().at(Axis::LeftTrigger));
    }
    else if (!dead && (before.LeftTrigger != after.LeftTrigger))
    {
        _axisWasInDeadZone[Axis::LeftTrigger] = false;
        _inputProcessor->processAxis(*this, Axis::LeftTrigger, after.LeftTrigger);
    }

    dead = _valueIsInAxisDeadZone(after.RightTrigger, Axis::RightTrigger);
    if (!_axisWasInDeadZone[Axis::RightTrigger] && dead)
    {
        _axisWasInDeadZone[Axis::RightTrigger] = true;
        _inputProcessor->processAxis(*this, Axis::RightTrigger, AxisRestValues().at(Axis::RightTrigger));
    }
    else if (!dead && (before.RightTrigger != after.RightTrigger))
    {
        _axisWasInDeadZone[Axis::RightTrigger] = false;
        _inputProcessor->processAxis(*this, Axis::RightTrigger, after.RightTrigger);
    }
}

bool Gamepad::_valueIsInAxisDeadZone(const float value, const Axis axis) const {
    return (value >= _axisDeadZones.at(axis).first) && (value <= _axisDeadZones.at(axis).second);
}

} // namespace rb::Window

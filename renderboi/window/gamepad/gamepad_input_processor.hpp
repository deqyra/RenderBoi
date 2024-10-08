#ifndef RENDERBOI_WINDOW_GAMEPAD_GAMEPAD_INPUT_PROCESSOR_HPP
#define RENDERBOI_WINDOW_GAMEPAD_GAMEPAD_INPUT_PROCESSOR_HPP

#include <memory>
#include <vector>

#include "../enums.hpp"

namespace rb {
namespace Window {

class Gamepad;

/// @brief Callback manager to handle input from a gamepad
class GamepadInputProcessor {
protected:
    /// @brief Keeps track of how many instances were created (used as a 
    /// unique ID system)
    static unsigned int _count;

    using Action = Input::Action;
    using Joystick = Input::Joystick;
    using Button = Input::Gamepad::Button;
    using Axis = Input::Gamepad::Axis;

public:
    /// @brief Collection of litterals describing the different event types
    /// handled by an InputProcessor
    enum class EventType {
        Connect,
        Disconnect,
        Button,
        Axis
    };

    /// @brief Array listing all of the litterals defined by the EventType
    /// enum
    static const inline std::vector<EventType> EventTypeList = {
        EventType::Connect,
        EventType::Disconnect,
        EventType::Axis,
        EventType::Button
    };

    GamepadInputProcessor();

    /// @brief Callback for when the gamepad is connected
    virtual void processConnected(const Gamepad& gamepad);

    /// @brief Callback for when the gamepad is disconnected
    virtual void processDisconnected(const Gamepad& gamepad);

    /// @brief Callback for a gamepad button event
    ///
    /// @param button Literal describing which key triggered the event
    /// @param action Literal describing what action was performed on
    /// the button which triggered the event
    virtual void processButton(const Gamepad& gamepad, const Input::Gamepad::Button button, const Input::Action action);

    /// @brief Callback for a gamepad axis event
    ///
    /// @param axis Literal describing which axis triggered the event
    /// @param value Value at which the axis was polled
    virtual void processAxis(const Gamepad& gamepad, const Input::Gamepad::Axis axis, const float value);

    /// @brief Unique identifier of the gamepad input processor
    const unsigned int id;
};

} // namespace Window

using GamepadInputProcessor = Window::GamepadInputProcessor;
using GamepadInputProcessorPtr = std::unique_ptr<GamepadInputProcessor>;

} // namespace rb

#endif//RENDERBOI_WINDOW_GAMEPAD_GAMEPAD_INPUT_PROCESSOR_HPP
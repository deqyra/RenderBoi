#ifndef RENDERBOI_TOOLBOX_RUNNABLES_INPUT_LOGGER_HPP
#define RENDERBOI_TOOLBOX_RUNNABLES_INPUT_LOGGER_HPP

#include <iostream>
#include <memory>
#include <unordered_map>

#include <renderboi/window/input_processor.hpp>
#include <renderboi/window/gl_window.hpp>
#include <renderboi/window/gamepad/gamepad_input_processor.hpp>

namespace rb {

class InputLogger : public InputProcessor, public GamepadInputProcessor {
private:
    using IEventType = InputProcessor::EventType;
    using GEventType = GamepadInputProcessor::EventType;
    using Key = Window::Input::Key;
    using MButton = Window::Input::MouseButton;
    using Joystick = Window::Input::Joystick;
    using GButton = Window::Input::Gamepad::Button;
    using Axis = Window::Input::Gamepad::Axis;
    using Action = Window::Input::Action;

    /// @brief Structure telling whether events of a certain type should be
    /// logged
    std::unordered_map<IEventType, bool> _inputLoggingStatus;

    /// @brief Structure telling whether events of a certain type should be
    /// logged
    std::unordered_map<GEventType, bool> _gamepadInputLoggingStatus;

    /// @brief Stream to which events should be logged Very shady and prone
    /// to errors but will do for now
    std::ostream& _outputStream;

public:
    /// @param outputStream Stream to which events should be logged
    InputLogger(std::ostream& outputStream = std::cout);

    /// @brief Enables logging for a certain type of input event
    ///
    /// @param eventType Litteral describing the type of the event for which
    /// to enable logging
    void enableEventLog(const IEventType eventType);

    /// @brief Disables logging for a certain type of input event
    ///
    /// @param eventType Litteral describing the type of the event for which
    /// to disable logging
    void disableEventLog(const IEventType eventType);

    /// @brief Enables logging for a certain type of input event
    ///
    /// @param eventType Litteral describing the type of the event for which
    /// to enable logging
    void enableEventLog(const GEventType eventType);

    /// @brief Disables logging for a certain type of input event
    ///
    /// @param eventType Litteral describing the type of the event for which
    /// to disable logging
    void disableEventLog(const GEventType eventType);

    /// @brief Set the logging status for a certain type of input event: 
    /// enabled or disabled
    ///
    /// @param eventType Litteral describing the type of the event for which
    /// to set the logging status
    /// @param enable Whether to enable or disable logging for the event 
    /// type
    void setEventLoggingStatus(const IEventType eventType, const bool enable);

    /// @brief Set the logging status for a certain type of input event: 
    /// enabled or disabled
    ///
    /// @param eventType Litteral describing the type of the event for which
    /// to set the logging status
    /// @param enable Whether to enable or disable logging for the event 
    /// type
    void setEventLoggingStatus(const GEventType eventType, const bool enable);

    //////////////////////////////////////////////
    ///                                        ///
    /// Methods overridden from InputProcessor ///
    ///                                        ///
    //////////////////////////////////////////////

    /// @brief Callback for a framebuffer resize event
    ///
    /// @param window Reference to the GLWindow in which the event was
    /// triggered
    /// @param width New width (in pixels) of the framebuffer
    /// @param height New height (in pixels) of the framebuffer
    void processFramebufferResize(
        GLWindow& window,
        const unsigned int width,
        const unsigned int height
    ) override;

    /// @brief Callback for a keyboard event
    ///
    /// @param window Reference to the GLWindow in which the event was
    /// triggered
    /// @param key Literal describing which key triggered the event
    /// @param scancode Scancode of the key which triggered the event 
    /// Platform-dependent, but consistent over time
    /// @param action Literal describing what action was performed on
    /// the key which triggered the event
    /// @param mods Bit field describing which modifiers were enabled 
    /// during the key event (Ctrl, Shift, etc)
    void processKeyboard(
        GLWindow& window,
        const Key key,
        const int scancode,
        const Action action,
        const int mods
    ) override;

    /// @brief Callback for a mouse button event
    ///
    /// @param window Reference to the GLWindow in which the event was
    /// triggered
    /// @param button Literal describing which button triggered the
    /// event
    /// @param action Literal describing what action was performed on
    /// the button which triggered the event
    /// @param mods Bit field describing which modifiers were enabled 
    /// during the button event (Ctrl, Shift, etc)
    void processMouseButton(
        GLWindow& window,
        const MButton button,
        const Action action,
        const int mods
    ) override;

    /// @brief Callback for a mouse cursor event
    ///
    /// @param window Reference to the GLWindow in which the event was
    /// triggered
    /// @param xpos X coordinate of the new position of the mouse
    /// @param ypos Y coordinate of the new position of the mouse
    void processMouseCursor(
        GLWindow& window,
        const double xpos,
        const double ypos
    ) override;

    /////////////////////////////////////////////////////
    ///                                               ///
    /// Methods overridden from GamepadInputProcessor ///
    ///                                               ///
    /////////////////////////////////////////////////////

    /// @brief Callback for when the gamepad is connected
    void processConnected(const Gamepad& gamepad) override;

    /// @brief Callback for when the gamepad is disconnected
    void processDisconnected(const Gamepad& gamepad) override;

    /// @brief Callback for a gamepad button event
    ///
    /// @param button Literal describing which key triggered the event
    /// @param action Literal describing what action was performed on
    /// the button which triggered the event
    void processButton(const Gamepad& gamepad, const GButton button, const Action action) override;

    /// @brief Callback for a gamepad axis event
    ///
    /// @param axis Literal describing which axis triggered the event
    /// @param value Value at which the axis was polled
    void processAxis(const Gamepad& gamepad, const Axis axis, const float value) override;
};

using InputLoggerPtr = std::unique_ptr<InputLogger>;

} // namespace rb

#endif//RENDERBOI_TOOLBOX_RUNNABLES_INPUT_LOGGER_HPP
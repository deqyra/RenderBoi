#ifndef RENDERBOI__TOOLBOX__INPUT_SPLITTER_HPP
#define RENDERBOI__TOOLBOX__INPUT_SPLITTER_HPP

#include <memory>
#include <unordered_map>

#include <renderboi/window/input_processor.hpp>
#include <renderboi/window/gl_window.hpp>
#include <renderboi/window/gamepad/gamepad_input_processor.hpp>

namespace renderboi
{

/// @brief Forwards all input it receives to multiple subscribers.
class InputSplitter : public InputProcessor, public GamepadInputProcessor
{
private:
    using Key = Window::Input::Key;
    using MButton = Window::Input::MouseButton;
    using Joystick = Window::Input::Joystick;
    using GButton = Window::Input::Gamepad::Button;
    using Axis = Window::Input::Gamepad::Axis;
    using Action = Window::Input::Action;

    /// @brief Used to uniquely identify subscriptions.
    unsigned int _subscriberRollingCount;

    /// @brief Array of subscribers to which input must be forwarded.
    std::unordered_map<unsigned int, InputProcessor&> _subscribers;

    /// @brief Array of subscribers to which gamepad input must be forwarded.
    std::unordered_map<unsigned int, GamepadInputProcessor&> _gamepadSubscribers;

public:
    InputSplitter();

    /// @brief Register a new input processor. Use the returned subscription
    /// ID to detach it.
    ///
    /// @param inputProcessor Pointer to the new input processor.
    ///
    /// @return The subscription ID, used to unsubscribe the registered
    /// input processor.
    unsigned int registerInputProcessor(InputProcessor& inputProcessor);

    /// @brief Unsubscribe an input processor.
    ///
    /// @param subscriptionId ID returned when the input processor was first
    /// registered to the input splitter.
    void detachInputProcessor(const unsigned int subscriptionId);

    /// @brief Unsubscribe all input processors from the input splitter.
    void detachAllInputProcessors();

    /// @brief Register a new input processor. Use the returned subscription
    /// ID to detach it.
    ///
    /// @param inputProcessor Pointer to the new input processor.
    ///
    /// @return The subscription ID, used to unsubscribe the registered
    /// input processor.
    unsigned int registerGamepadInputProcessor(GamepadInputProcessor& gamepadInputProcessor);

    /// @brief Unsubscribe an input processor.
    ///
    /// @param subscriptionId ID returned when the input processor was first
    /// registered to the input splitter.
    void detachGamepadInputProcessor(const unsigned int subscriptionId);

    /// @brief Unsubscribe all input processors from the input splitter.
    void detachAllIGamepadnputProcessors();

    //////////////////////////////////////////////
    ///                                        ///
    /// Methods overridden from InputProcessor ///
    ///                                        ///
    //////////////////////////////////////////////

    /// @brief Callback for a framebuffer resize event.
    ///
    /// @param window Reference to the GLWindow in which the event was
    /// triggered.
    /// @param width New width (in pixels) of the framebuffer.
    /// @param height New height (in pixels) of the framebuffer.
    void processFramebufferResize(GLWindow& window, const unsigned int width, const unsigned int height) override;

    /// @brief Callback for a keyboard event.
    ///
    /// @param window Reference to the GLWindow in which the event was
    /// triggered.
    /// @param key Literal describing which key triggered the event.
    /// @param scancode Scancode of the key which triggered the event. 
    /// Platform-dependent, but consistent over time.
    /// @param action Literal describing what action was performed on
    /// the key which triggered the event.
    /// @param mods Bit field describing which modifiers were enabled 
    /// during the key event (Ctrl, Shift, etc).
    void processKeyboard(
        GLWindow& window,
        const Key key,
        const int scancode,
        const Action action,
        const int mods
    ) override;

    /// @brief Callback for a mouse button event.
    ///
    /// @param window Reference to the GLWindow in which the event was
    /// triggered.
    /// @param button Literal describing which button triggered the
    /// event.
    /// @param action Literal describing what action was performed on
    /// the button which triggered the event.
    /// @param mods Bit field describing which modifiers were enabled 
    /// during the button event (Ctrl, Shift, etc).
    void processMouseButton(
        GLWindow& window,
        const MButton button,
        const Action action,
        const int mods
    ) override;

    /// @brief Callback for a mouse cursor event.
    ///
    /// @param window Reference to the GLWindow in which the event was
    /// triggered.
    /// @param xpos X coordinate of the new position of the mouse.
    /// @param ypos Y coordinate of the new position of the mouse.
    void processMouseCursor(GLWindow& window, const double xpos, const double ypos) override;

    /////////////////////////////////////////////////////
    ///                                               ///
    /// Methods overridden from GamepadInputProcessor ///
    ///                                               ///
    /////////////////////////////////////////////////////

    /// @brief Callback for when the gamepad is connected.
    void processConnected(const Gamepad& gamepad) override;

    /// @brief Callback for when the gamepad is disconnected.
    void processDisconnected(const Gamepad& gamepad) override;

    /// @brief Callback for a gamepad button event.
    ///
    /// @param button Literal describing which key triggered the event.
    /// @param action Literal describing what action was performed on
    /// the button which triggered the event.
    void processButton(const Gamepad& gamepad, const GButton button, const Action action) override;

    /// @brief Callback for a gamepad axis event.
    ///
    /// @param axis Literal describing which axis triggered the event.
    /// @param value Value at which the axis was polled.
    void processAxis(const Gamepad& gamepad, const Axis axis, const float value) override;
};

using InputSplitterPtr = std::unique_ptr<InputSplitter>;

} // namespace renderboi

#endif//RENDERBOI__TOOLBOX__INPUT_SPLITTER_HPP
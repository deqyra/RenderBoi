#ifndef RENDERBOI_TOOLBOX_CONTROLS_CONTROL_EVENT_TRANSLATOR_HPP
#define RENDERBOI_TOOLBOX_CONTROLS_CONTROL_EVENT_TRANSLATOR_HPP

#include <memory>

#include <renderboi/toolbox/interfaces/action_event_receiver.hpp>

#include <renderboi/window/gl_window.hpp>
#include <renderboi/window/input_processor.hpp>

#include "control.hpp"
#include "control_scheme.hpp"

namespace rb {

/// @brief Class to be plugged in directly into the window Will receive and
/// filter control events, translate them into actions if appropriate, and
/// forward those to their listener
///
/// @tparam T Required. Type of the action to translate controls to
template<typename T>
class ControlEventTranslator : public InputProcessor
{
private:
    using ActionEventReceiverType = ActionEventReceiver<T>;

    /// @brief Structure mapping actions to the control they are bound to
    std::unordered_map<Control, T, ControlHash> _controlBindings;

    /// @brief Object which will be notified of which actions were triggered
    /// by their bound controls
    ActionEventReceiverType& _listener;

public:
    /// @param controlScheme Object which can tell which controls are 
    /// mapped to which actions
    /// @param listener Object which will be notified of which actions 
    /// were triggered by their bound controls
    ControlEventTranslator(
        const ControlScheme<T>& controlScheme,
        ActionEventReceiverType& listener
    );

    //////////////////////////////////////////////
    ///                                        ///
    /// Methods overridden from InputProcessor ///
    ///                                        ///
    //////////////////////////////////////////////

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
        const Window::Input::Key key, 
        const int scancode, 
        const Window::Input::Action action, 
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
        const Window::Input::MouseButton button, 
        const Window::Input::Action action, 
        const int mods
    ) override;

private:
    /// @brief Translate a given control into an action and forward it to the
    /// listener
    ///
    /// @param control Structure of litterals describing the control which was
    /// just processed
    /// @param action Object describing the action that was performed on the
    /// control
    void _translateAndNotify(const Control& control, Window::Input::Action action) const;
};

template<typename T>
ControlEventTranslator<T>::ControlEventTranslator(
    const ControlScheme<T>& controlScheme,
    ActionEventReceiverType& listener
) :
    _controlBindings(controlScheme.actions()),
    _listener(listener) {

}


template<typename T>
void ControlEventTranslator<T>::processKeyboard(
    GLWindow& window,
    const Window::Input::Key key,
    const int scancode,
    const Window::Input::Action action,
    const int mods
)
{
    _translateAndNotify(Control(key), action);
}

template<typename T>
void ControlEventTranslator<T>::processMouseButton(
    GLWindow& window,
    const Window::Input::MouseButton button,
    const Window::Input::Action action,
    const int mods
)
{
    _translateAndNotify(Control(button), action);
}

template<typename T>
void ControlEventTranslator<T>::_translateAndNotify(
    const Control& control,
    Window::Input::Action action
) const
{
    const auto it = _controlBindings.find(control);
    if (it == _controlBindings.cend()) return;

    if (action == Window::Input::Action::Release)
    {
        _listener.stopAction(it->second);
    }
    else // if (action == Window::Input::Action::Press)
    {
        _listener.triggerAction(it->second);
    }
}

template<typename T>
using ControlEventTranslatorPtr = std::unique_ptr<ControlEventTranslator<T>>;

} // namespace rb

#endif//RENDERBOI_TOOLBOX_CONTROLS_CONTROL_EVENT_TRANSLATOR_HPP
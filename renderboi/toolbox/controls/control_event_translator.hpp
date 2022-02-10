#ifndef RENDERBOI__TOOLBOX__CONTROLS__CONTROL_EVENT_TRANSLATOR_HPP
#define RENDERBOI__TOOLBOX__CONTROLS__CONTROL_EVENT_TRANSLATOR_HPP

#include <algorithm>
#include <functional>
#include <iterator>
#include <map>
#include <memory>
#include <utility>
#include <vector>

#include <cpptools/oo/interfaces/action_event_receiver.hpp>

#include <renderboi/window/gl_window.hpp>
#include <renderboi/window/input_processor.hpp>

#include "control.hpp"
#include "control_scheme.hpp"

namespace Renderboi
{

/// @brief Class to be plugged in directly into the window. Will receive and
/// filter control events, translate them into actions if appropriate, and
/// forward those to their listener.
template<typename T>
class ControlEventTranslator : public InputProcessor
{
private:
    using ActionEventReceiverType = type_utils::apply_types<
        typename cpptools::ActionEventReceiver,
        typename T::TemplateTypes
    >;

    /// @brief Structure mapping actions to the control they are bound to.
    const std::multimap<T, Control>& _controlBindings;

    /// @brief Object which will be notified of which actions were triggered
    /// by their bound controls.
    ActionEventReceiverType& _listener;

    /// @brief Translate a given control into action(s) and forward those to
    /// the listener.
    ///
    /// @param control Structure of litterals describing the control which 
    /// just processed.
    /// @param action Object describing the action that was performed on the
    /// control.
    /// @param window Reference to the window on which the control was
    /// captured.
    void _translateAndNotify(const Control& control, Window::Input::Action action, GLWindow* const window) const;

    // std::tuple<

public:
    /// @param controlScheme Object which can tell which controls are 
    /// mapped to which actions.
    /// @param listener Object which will be notified of which actions 
    /// were triggered by their bound controls.
    ControlEventTranslator(
        const ControlScheme<T>& controlScheme,
        const ActionEventReceiverType& listener
    );

    //////////////////////////////////////////////
    ///                                        ///
    /// Methods overridden from InputProcessor ///
    ///                                        ///
    //////////////////////////////////////////////

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
        const Window::Input::Key key, 
        const int scancode, 
        const Window::Input::Action action, 
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
        const Window::Input::MouseButton button, 
        const Window::Input::Action action, 
        const int mods
    ) override;
};

template<typename T>
ControlEventTranslator<T>::ControlEventTranslator(
    const ControlScheme<T>& controlScheme,
    const ActionEventReceiverType& listener
) :
    _controlBindings(controlScheme.getAllBoundControls()),
    _listener(listener)
{

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
    _translateAndNotify(Control(key), action, window);
}

template<typename T>
void ControlEventTranslator<T>::processMouseButton(
    GLWindow& window,
    const Window::Input::MouseButton button,
    const Window::Input::Action action,
    const int mods
)
{
    _translateAndNotify(Control(button), action, window);
}

template<typename T>
void ControlEventTranslator<T>::_translateAndNotify(
    const Control& control,
    Window::Input::Action action,
    GLWindow* const window
) const
{
    using Iter = typename std::multimap<Control, T>::const_iterator;
    const std::pair<Iter, Iter> range = _controlBindings.equal_range(control);
    
    if (action == Window::Input::Action::Release)
    {
        for (Iter it = range.first; it != range.second; it++)
        {
            _listener->stopAction(it->second, window);
        }
    }
    else // if (action == Window::Input::Action::Press)
    {
        for (Iter it = range.first; it != range.second; it++)
        {
            _listener->triggerAction(it->second, window);
        }
    }    
}

template<typename T>
using ControlEventTranslatorPtr = std::unique_ptr<ControlEventTranslator<T>>;

} // namespace Renderboi

#endif//RENDERBOI__TOOLBOX__CONTROLS__CONTROL_EVENT_TRANSLATOR_HPP
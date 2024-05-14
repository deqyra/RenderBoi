#include "control.hpp"

#include <string>

#include <renderboi/window/enums.hpp>

namespace rb {

Control::Control(const Window::Input::Key key) :
    key(key),
    kind(ControlKind::Key)
{

}

Control::Control(const Window::Input::MouseButton mouseButton) :
    mouseButton(mouseButton),
    kind(ControlKind::MouseButton)
{

}

bool operator==(const Control& left, const Control& right) {
    // Accessing .key regardless of what member actually is active in the union
    // is okay because all enums in the union are based on the same underlying,
    // type, which makes them layout-compatible enum types, which makes all a 
    // unique common initial sequence that the union can be safely accessed through
    return left.kind == right.kind && left.key == right.key;
}

bool operator<(const Control& left, const Control& right) {
    return (left.kind < right.kind) || ((left.kind == right.kind) && (left.key < right.key));
}

std::size_t ControlHash::operator()(Control const& c) const {
    std::size_t res = 0;

    tools::hash_combine(res, c.kind);
    tools::hash_combine(res, c.key);

    return res;
}

std::string to_string(const Control& control) {
    switch(control.kind)
    {
        case ControlKind::Key:
            return to_string(control.key);
        case ControlKind::MouseButton:
            return to_string(control.mouseButton);
        case ControlKind::GamepadButton:
            return to_string(control.gamepadButton);
        case ControlKind::GamepadAxis:
            return to_string(control.gamepadAxis);
        case ControlKind::Joystick:
            return to_string(control.joystick);
    }
    return "Unknown control";
}

} // namespace rb

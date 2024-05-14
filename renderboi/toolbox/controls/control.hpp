#ifndef RENDERBOI_TOOLBOX_CONTROLS_CONTROL_HPP
#define RENDERBOI_TOOLBOX_CONTROLS_CONTROL_HPP

#include <string>

#include <renderboi/window/enums.hpp>

#include <cpptools/utility/hash_combine.hpp>

namespace rb {

/// @brief Litterals describing the flavor of a control
enum class ControlKind : unsigned char {
    Key,
    MouseButton,
    GamepadButton,
    GamepadAxis,
    Joystick
};

/// @brief Struct describing a control as well as the kind of device it originated from
struct Control {
    static_assert(
        std::is_same_v<std::underlying_type_t<Window::Input::Key>, std::underlying_type_t<Window::Input::MouseButton>>
    &&  std::is_same_v<std::underlying_type_t<Window::Input::Key>, std::underlying_type_t<Window::Input::Gamepad::Button>>
    &&  std::is_same_v<std::underlying_type_t<Window::Input::Key>, std::underlying_type_t<Window::Input::Gamepad::Axis>>
    &&  std::is_same_v<std::underlying_type_t<Window::Input::Key>, std::underlying_type_t<Window::Input::Joystick>>
    ); // all enum types used in the union must be based on the same integral type
    
    union {
        Window::Input::Key key;
        Window::Input::MouseButton mouseButton;
        Window::Input::Gamepad::Button gamepadButton;
        Window::Input::Gamepad::Axis gamepadAxis;
        Window::Input::Joystick joystick;
    };
    ControlKind kind;

    Control() = default;

    Control(const Window::Input::Key key);
    Control(const Window::Input::MouseButton mouseButton);
};

bool operator==(const Control& left, const Control& right);
bool operator<(const Control& left, const Control& right);

struct ControlHash {
    std::size_t operator()(Control const& c) const;
};

std::string to_string(const Control& control);

} // namespace rb

#endif//RENDERBOI_TOOLBOX_CONTROLS_CONTROL_HPP
#include "enums.hpp"

#include <string>

namespace renderboi
{

namespace Window::Input
{
    extern const std::vector<Joystick> Joysticks = {
        Joystick::J1,
        Joystick::J2,
        Joystick::J3,
        Joystick::J4,
        Joystick::J5,
        Joystick::J6,
        Joystick::J7,
        Joystick::J8,
        Joystick::J9,
        Joystick::J10,
        Joystick::J11,
        Joystick::J12,
        Joystick::J13,
        Joystick::J14,
        Joystick::J15,
        Joystick::J16
    };
}

unsigned int operator&(unsigned int left, Window::Input::Modifier right)
{
    return left & static_cast<unsigned int>(right);
}

unsigned int operator|(unsigned int left, Window::Input::Modifier right)
{
    return left | static_cast<unsigned int>(right);
}

bool any(Window::Input::Modifier value)
{
    return value != Window::Input::Modifier::None;
}

std::string to_string(const Window::Input::Mode::Target target)
{
    static std::unordered_map<Window::Input::Mode::Target, std::string> enumNames = {
        {Window::Input::Mode::Target::Cursor,              "Cursor"},
        {Window::Input::Mode::Target::StickyKeys,          "StickyKeys"},
        {Window::Input::Mode::Target::StickyMouseButtons,  "StickyMouseButtons"},
        {Window::Input::Mode::Target::LockKeyMods,         "LockKeyMods"},
        {Window::Input::Mode::Target::RawMouseMotion,      "RawMouseMotion"},
    };

    auto it = enumNames.find(target);
    return (it != enumNames.end())
        ? it->second
        : "Unknown window input mode target";
}

std::string to_string(const Window::Input::Mode::Value value)
{
    static std::unordered_map<Window::Input::Mode::Value, std::string> enumNames = {
        {Window::Input::Mode::Value::True,             "True"},
        {Window::Input::Mode::Value::False,            "False"},
        {Window::Input::Mode::Value::NormalCursor,     "NormalCursor"},
        {Window::Input::Mode::Value::HiddenCursor,     "HiddenCursor"},
        {Window::Input::Mode::Value::DisabledCursor,   "DisabledCursor"},
    };

    auto it = enumNames.find(value);
    return (it != enumNames.end())
        ? it->second
        : "Unknown window input mode value";
}

std::string to_string(const Window::Input::Action action)
{
    static std::unordered_map<Window::Input::Action, std::string> enumNames = {
        {Window::Input::Action::Release,   "Release"},
        {Window::Input::Action::Press,     "Press"},
        // {Window::Input::Action::Repeat,    "Repeat"},      // FIX ME IF REPEAT KEYS MUST BE HANDLED
    };

    auto it = enumNames.find(action);
    return (it != enumNames.end())
        ? it->second
        : "Unknown input action";
}

std::string to_string(const Window::Input::Key key)
{
    static std::unordered_map<Window::Input::Key, std::string> enumNames = {
        {Window::Input::Key::Unknown,          "Unknown"},
        {Window::Input::Key::Space,            "Space"},
        {Window::Input::Key::Apostrophe,       "Apostrophe"},
        {Window::Input::Key::Comma,            "Comma"},
        {Window::Input::Key::Minus,            "Minus"},
        {Window::Input::Key::Period,           "Period"},
        {Window::Input::Key::Slash,            "Slash"},
        {Window::Input::Key::Key0,             "Key0"},
        {Window::Input::Key::Key1,             "Key1"},
        {Window::Input::Key::Key2,             "Key2"},
        {Window::Input::Key::Key3,             "Key3"},
        {Window::Input::Key::Key4,             "Key4"},
        {Window::Input::Key::Key5,             "Key5"},
        {Window::Input::Key::Key6,             "Key6"},
        {Window::Input::Key::Key7,             "Key7"},
        {Window::Input::Key::Key8,             "Key8"},
        {Window::Input::Key::Key9,             "Key9"},
        {Window::Input::Key::Semicolon,        "Semicolon"},
        {Window::Input::Key::Equal,            "Equal"},
        {Window::Input::Key::A,                "A"},
        {Window::Input::Key::B,                "B"},
        {Window::Input::Key::C,                "C"},
        {Window::Input::Key::D,                "D"},
        {Window::Input::Key::E,                "E"},
        {Window::Input::Key::F,                "F"},
        {Window::Input::Key::G,                "G"},
        {Window::Input::Key::H,                "H"},
        {Window::Input::Key::I,                "I"},
        {Window::Input::Key::J,                "J"},
        {Window::Input::Key::K,                "K"},
        {Window::Input::Key::L,                "L"},
        {Window::Input::Key::M,                "M"},
        {Window::Input::Key::N,                "N"},
        {Window::Input::Key::O,                "O"},
        {Window::Input::Key::P,                "P"},
        {Window::Input::Key::Q,                "Q"},
        {Window::Input::Key::R,                "R"},
        {Window::Input::Key::S,                "S"},
        {Window::Input::Key::T,                "T"},
        {Window::Input::Key::U,                "U"},
        {Window::Input::Key::V,                "V"},
        {Window::Input::Key::W,                "W"},
        {Window::Input::Key::X,                "X"},
        {Window::Input::Key::Y,                "Y"},
        {Window::Input::Key::Z,                "Z"},
        {Window::Input::Key::LeftBracket,      "LeftBracket"},
        {Window::Input::Key::Backslash,        "Backslash"},
        {Window::Input::Key::RightBracket,     "RightBracket"},
        {Window::Input::Key::GraveAccent,      "GraveAccent"},
        {Window::Input::Key::World1,           "World1"},
        {Window::Input::Key::World2,           "World2"},
        {Window::Input::Key::Escape,           "Escape"},
        {Window::Input::Key::Enter,            "Enter"},
        {Window::Input::Key::Tab,              "Tab"},
        {Window::Input::Key::Backspace,        "Backspace"},
        {Window::Input::Key::Insert,           "Insert"},
        {Window::Input::Key::Delete,           "Delete"},
        {Window::Input::Key::Right,            "Right"},
        {Window::Input::Key::Left,             "Left"},
        {Window::Input::Key::Down,             "Down"},
        {Window::Input::Key::Up,               "Up"},
        {Window::Input::Key::PageUp,           "PageUp"},
        {Window::Input::Key::PageDown,         "PageDown"},
        {Window::Input::Key::Home,             "Home"},
        {Window::Input::Key::End,              "End"},
        {Window::Input::Key::CapsLock,         "CapsLock"},
        {Window::Input::Key::ScrollLock,       "ScrollLock"},
        {Window::Input::Key::NumLock,          "NumLock"},
        {Window::Input::Key::PrintScreen,      "PrintScreen"},
        {Window::Input::Key::Pause,            "Pause"},
        {Window::Input::Key::F1,               "F1"},
        {Window::Input::Key::F2,               "F2"},
        {Window::Input::Key::F3,               "F3"},
        {Window::Input::Key::F4,               "F4"},
        {Window::Input::Key::F5,               "F5"},
        {Window::Input::Key::F6,               "F6"},
        {Window::Input::Key::F7,               "F7"},
        {Window::Input::Key::F8,               "F8"},
        {Window::Input::Key::F9,               "F9"},
        {Window::Input::Key::F10,              "F10"},
        {Window::Input::Key::F11,              "F11"},
        {Window::Input::Key::F12,              "F12"},
        {Window::Input::Key::F13,              "F13"},
        {Window::Input::Key::F14,              "F14"},
        {Window::Input::Key::F15,              "F15"},
        {Window::Input::Key::F16,              "F16"},
        {Window::Input::Key::F17,              "F17"},
        {Window::Input::Key::F18,              "F18"},
        {Window::Input::Key::F19,              "F19"},
        {Window::Input::Key::F20,              "F20"},
        {Window::Input::Key::F21,              "F21"},
        {Window::Input::Key::F22,              "F22"},
        {Window::Input::Key::F23,              "F23"},
        {Window::Input::Key::F24,              "F24"},
        {Window::Input::Key::F25,              "F25"},
        {Window::Input::Key::Keypad0,          "Keypad0"},
        {Window::Input::Key::Keypad1,          "Keypad1"},
        {Window::Input::Key::Keypad2,          "Keypad2"},
        {Window::Input::Key::Keypad3,          "Keypad3"},
        {Window::Input::Key::Keypad4,          "Keypad4"},
        {Window::Input::Key::Keypad5,          "Keypad5"},
        {Window::Input::Key::Keypad6,          "Keypad6"},
        {Window::Input::Key::Keypad7,          "Keypad7"},
        {Window::Input::Key::Keypad8,          "Keypad8"},
        {Window::Input::Key::Keypad9,          "Keypad9"},
        {Window::Input::Key::KeypadDecimal,    "KeypadDecimal"},
        {Window::Input::Key::KeypadDivide,     "KeypadDivide"},
        {Window::Input::Key::KeypadMultiply,   "KeypadMultiply"},
        {Window::Input::Key::KeypadSubtract,   "KeypadSubtract"},
        {Window::Input::Key::KeypadAdd,        "KeypadAdd"},
        {Window::Input::Key::KeypadEnter,      "KeypadEnter"},
        {Window::Input::Key::KeypadEqual,      "KeypadEqual"},
        {Window::Input::Key::LeftShift,        "LeftShift"},
        {Window::Input::Key::LeftControl,      "LeftControl"},
        {Window::Input::Key::LeftAlt,          "LeftAlt"},
        {Window::Input::Key::LeftSuper,        "LeftSuper"},
        {Window::Input::Key::RightShift,       "RightShift"},
        {Window::Input::Key::RightControl,     "RightControl"},
        {Window::Input::Key::RightAlt,         "RightAlt"},
        {Window::Input::Key::RightSuper,       "RightSuper"},
        {Window::Input::Key::Menu,             "Menu"},
    };

    auto it = enumNames.find(key);
    return (it != enumNames.end())
        ? it->second
        : "Unknown keyboard key";
}

std::string to_string(const Window::Input::Modifier mod)
{
    static std::unordered_map<Window::Input::Modifier, std::string> enumNames = {
        {Window::Input::Modifier::None,        "None"},
        {Window::Input::Modifier::Shift,       "Shift"},
        {Window::Input::Modifier::Control,     "Control"},
        {Window::Input::Modifier::Alt,         "Alt"},
        {Window::Input::Modifier::Super,       "Super"},
        {Window::Input::Modifier::CapsLock,    "CapsLock"},
        {Window::Input::Modifier::NumLock,     "NumLock"},
    };

    auto it = enumNames.find(mod);
    return (it != enumNames.end())
        ? it->second
        : "Unknown keyboard modifier";
}

std::string to_string(const Window::Input::MouseButton mouseButton)
{
    static std::unordered_map<Window::Input::MouseButton, std::string> enumNames = {
        {Window::Input::MouseButton::B1,   "B1"},
        {Window::Input::MouseButton::B2,   "B2"},
        {Window::Input::MouseButton::B3,   "B3"},
        {Window::Input::MouseButton::B4,   "B4"},
        {Window::Input::MouseButton::B5,   "B5"},
        {Window::Input::MouseButton::B6,   "B6"},
        {Window::Input::MouseButton::B7,   "B7"},
        {Window::Input::MouseButton::B8,   "B8"},
    };
    static std::unordered_map<Window::Input::MouseButton, std::string> specialNames = {
        {Window::Input::MouseButton::Left,      "Left"},
        {Window::Input::MouseButton::Right,     "Right"},
        {Window::Input::MouseButton::Middle,    "Middle"},
    };

    auto it = specialNames.find(mouseButton);
    if (it != specialNames.end()) return it->second;

    it = enumNames.find(mouseButton);
    return (it != enumNames.end())
        ? it->second
        : "Unknown mouse button";
}

std::string to_string(const Window::Input::Joystick joystick)
{
    static std::unordered_map<Window::Input::Joystick, std::string> enumNames = {
        {Window::Input::Joystick::J1,  "J1"},
        {Window::Input::Joystick::J2,  "J2"},
        {Window::Input::Joystick::J3,  "J3"},
        {Window::Input::Joystick::J4,  "J4"},
        {Window::Input::Joystick::J5,  "J5"},
        {Window::Input::Joystick::J6,  "J6"},
        {Window::Input::Joystick::J7,  "J7"},
        {Window::Input::Joystick::J8,  "J8"},
        {Window::Input::Joystick::J9,  "J9"},
        {Window::Input::Joystick::J10, "J10"},
        {Window::Input::Joystick::J11, "J11"},
        {Window::Input::Joystick::J12, "J12"},
        {Window::Input::Joystick::J13, "J13"},
        {Window::Input::Joystick::J14, "J14"},
        {Window::Input::Joystick::J15, "J15"},
        {Window::Input::Joystick::J16, "J16"},
    };

    auto it = enumNames.find(joystick);
    return (it != enumNames.end())
        ? it->second
        : "Unknown joystick";
}

std::string to_string(const Window::Input::Gamepad::Button gamepadButton)
{
    static std::unordered_map<Window::Input::Gamepad::Button, std::string> enumNames = {
        {Window::Input::Gamepad::Button::A,             "A"},
        {Window::Input::Gamepad::Button::B,             "B"},
        {Window::Input::Gamepad::Button::X,             "X"},
        {Window::Input::Gamepad::Button::Y,             "Y"},
        {Window::Input::Gamepad::Button::LeftBumper,    "LeftBumper"},
        {Window::Input::Gamepad::Button::RightBumper,   "RightBumper"},
        {Window::Input::Gamepad::Button::Select,        "Select"},
        {Window::Input::Gamepad::Button::Start,         "Start"},
        {Window::Input::Gamepad::Button::Home,          "Home"},
        {Window::Input::Gamepad::Button::LeftThumb,     "LeftThumb"},
        {Window::Input::Gamepad::Button::RightThumb,    "RightThumb"},
        {Window::Input::Gamepad::Button::DPadUp,        "DPadUp"},
        {Window::Input::Gamepad::Button::DPadRight,     "DPadRight"},
        {Window::Input::Gamepad::Button::DPadDown,      "DPadDown"},
        {Window::Input::Gamepad::Button::DPadLeft,      "DPadLeft"},
    };

    auto it = enumNames.find(gamepadButton);
    return (it != enumNames.end())
        ? it->second
        : "Unknown gamepad button";
}

std::string to_string(const Window::Input::Gamepad::Axis gamepadAxis)
{
    static std::unordered_map<Window::Input::Gamepad::Axis, std::string> enumNames = {
        {Window::Input::Gamepad::Axis::LeftX,           "LeftX"},
        {Window::Input::Gamepad::Axis::LeftY,           "LeftY"},
        {Window::Input::Gamepad::Axis::RightX,          "RightX"},
        {Window::Input::Gamepad::Axis::RightY,          "RightY"},
        {Window::Input::Gamepad::Axis::LeftTrigger,     "LeftTrigger"},
        {Window::Input::Gamepad::Axis::RightTrigger,    "RightTrigger"},
    };

    auto it = enumNames.find(gamepadAxis);
    return (it != enumNames.end())
        ? it->second
        : "Unknown gamepad axis";
}

} // namespace renderboi

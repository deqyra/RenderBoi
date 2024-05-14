#include "enums.hpp"

#include <unordered_map>
#include <string>

namespace rb::Window::Input {

unsigned int operator&(const unsigned int left, const Modifier right) {
    return left & static_cast<unsigned int>(right);
}

unsigned int operator|(const unsigned int left, const Modifier right) {
    return left | static_cast<unsigned int>(right);
}

bool any(const Modifier value) {
    return value != Modifier::None;
}

namespace Mode {
    std::string to_string(const Target target)
    {
        static const std::unordered_map<Target, std::string> enumNames = {
            { Target::Cursor,              "Cursor"             },
            { Target::StickyKeys,          "StickyKeys"         },
            { Target::StickyMouseButtons,  "StickyMouseButtons" },
            { Target::LockKeyMods,         "LockKeyMods"        },
            { Target::RawMouseMotion,      "RawMouseMotion"     },
        };

        auto it = enumNames.find(target);
        return (it != enumNames.end())
            ? it->second
            : "Unknown window input mode target";
    }

    std::string to_string(const Value value)
    {
        static const std::unordered_map<Value, std::string> enumNames = {
            { Value::True,             "True"           },
            { Value::False,            "False"          },
            { Value::NormalCursor,     "NormalCursor"   },
            { Value::HiddenCursor,     "HiddenCursor"   },
            { Value::DisabledCursor,   "DisabledCursor" },
        };

        auto it = enumNames.find(value);
        return (it != enumNames.end())
            ? it->second
            : "Unknown window input mode value";
    }    
} // namespace Mode 

std::string to_string(const Action action) {
    static const std::unordered_map<Action, std::string> enumNames = {
        { Action::Release,   "Release" },
        { Action::Press,     "Press" },
        // {Action::Repeat,    "Repeat"},      // UNCOMMENT IF REPEAT KEYS MUST BE HANDLED
    };

    auto it = enumNames.find(action);
    return (it != enumNames.end())
        ? it->second
        : "Unknown input action";
}

std::string to_string(const Key key) {
    static const std::unordered_map<Key, std::string> enumNames = {
        { Key::Unknown,          "Unknown"        },
        { Key::Space,            "Space"          },
        { Key::Apostrophe,       "Apostrophe"     },
        { Key::Comma,            "Comma"          },
        { Key::Minus,            "Minus"          },
        { Key::Period,           "Period"         },
        { Key::Slash,            "Slash"          },
        { Key::Key0,             "Key0"           },
        { Key::Key1,             "Key1"           },
        { Key::Key2,             "Key2"           },
        { Key::Key3,             "Key3"           },
        { Key::Key4,             "Key4"           },
        { Key::Key5,             "Key5"           },
        { Key::Key6,             "Key6"           },
        { Key::Key7,             "Key7"           },
        { Key::Key8,             "Key8"           },
        { Key::Key9,             "Key9"           },
        { Key::Semicolon,        "Semicolon"      },
        { Key::Equal,            "Equal"          },
        { Key::A,                "A"              },
        { Key::B,                "B"              },
        { Key::C,                "C"              },
        { Key::D,                "D"              },
        { Key::E,                "E"              },
        { Key::F,                "F"              },
        { Key::G,                "G"              },
        { Key::H,                "H"              },
        { Key::I,                "I"              },
        { Key::J,                "J"              },
        { Key::K,                "K"              },
        { Key::L,                "L"              },
        { Key::M,                "M"              },
        { Key::N,                "N"              },
        { Key::O,                "O"              },
        { Key::P,                "P"              },
        { Key::Q,                "Q"              },
        { Key::R,                "R"              },
        { Key::S,                "S"              },
        { Key::T,                "T"              },
        { Key::U,                "U"              },
        { Key::V,                "V"              },
        { Key::W,                "W"              },
        { Key::X,                "X"              },
        { Key::Y,                "Y"              },
        { Key::Z,                "Z"              },
        { Key::LeftBracket,      "LeftBracket"    },
        { Key::Backslash,        "Backslash"      },
        { Key::RightBracket,     "RightBracket"   },
        { Key::GraveAccent,      "GraveAccent"    },
        { Key::World1,           "World1"         },
        { Key::World2,           "World2"         },
        { Key::Escape,           "Escape"         },
        { Key::Enter,            "Enter"          },
        { Key::Tab,              "Tab"            },
        { Key::Backspace,        "Backspace"      },
        { Key::Insert,           "Insert"         },
        { Key::Delete,           "Delete"         },
        { Key::Right,            "Right"          },
        { Key::Left,             "Left"           },
        { Key::Down,             "Down"           },
        { Key::Up,               "Up"             },
        { Key::PageUp,           "PageUp"         },
        { Key::PageDown,         "PageDown"       },
        { Key::Home,             "Home"           },
        { Key::End,              "End"            },
        { Key::CapsLock,         "CapsLock"       },
        { Key::ScrollLock,       "ScrollLock"     },
        { Key::NumLock,          "NumLock"        },
        { Key::PrintScreen,      "PrintScreen"    },
        { Key::Pause,            "Pause"          },
        { Key::F1,               "F1"             },
        { Key::F2,               "F2"             },
        { Key::F3,               "F3"             },
        { Key::F4,               "F4"             },
        { Key::F5,               "F5"             },
        { Key::F6,               "F6"             },
        { Key::F7,               "F7"             },
        { Key::F8,               "F8"             },
        { Key::F9,               "F9"             },
        { Key::F10,              "F10"            },
        { Key::F11,              "F11"            },
        { Key::F12,              "F12"            },
        { Key::F13,              "F13"            },
        { Key::F14,              "F14"            },
        { Key::F15,              "F15"            },
        { Key::F16,              "F16"            },
        { Key::F17,              "F17"            },
        { Key::F18,              "F18"            },
        { Key::F19,              "F19"            },
        { Key::F20,              "F20"            },
        { Key::F21,              "F21"            },
        { Key::F22,              "F22"            },
        { Key::F23,              "F23"            },
        { Key::F24,              "F24"            },
        { Key::F25,              "F25"            },
        { Key::Keypad0,          "Keypad0"        },
        { Key::Keypad1,          "Keypad1"        },
        { Key::Keypad2,          "Keypad2"        },
        { Key::Keypad3,          "Keypad3"        },
        { Key::Keypad4,          "Keypad4"        },
        { Key::Keypad5,          "Keypad5"        },
        { Key::Keypad6,          "Keypad6"        },
        { Key::Keypad7,          "Keypad7"        },
        { Key::Keypad8,          "Keypad8"        },
        { Key::Keypad9,          "Keypad9"        },
        { Key::KeypadDecimal,    "KeypadDecimal"  },
        { Key::KeypadDivide,     "KeypadDivide"   },
        { Key::KeypadMultiply,   "KeypadMultiply" },
        { Key::KeypadSubtract,   "KeypadSubtract" },
        { Key::KeypadAdd,        "KeypadAdd"      },
        { Key::KeypadEnter,      "KeypadEnter"    },
        { Key::KeypadEqual,      "KeypadEqual"    },
        { Key::LeftShift,        "LeftShift"      },
        { Key::LeftControl,      "LeftControl"    },
        { Key::LeftAlt,          "LeftAlt"        },
        { Key::LeftSuper,        "LeftSuper"      },
        { Key::RightShift,       "RightShift"     },
        { Key::RightControl,     "RightControl"   },
        { Key::RightAlt,         "RightAlt"       },
        { Key::RightSuper,       "RightSuper"     },
        { Key::Menu,             "Menu"           },
    };

    auto it = enumNames.find(key);
    return (it != enumNames.end())
        ? it->second
        : "Unknown keyboard key";
}

std::string to_string(const Modifier mod) {
    static const std::unordered_map<Modifier, std::string> enumNames = {
        { Modifier::None,        "None"     },
        { Modifier::Shift,       "Shift"    },
        { Modifier::Control,     "Control"  },
        { Modifier::Alt,         "Alt"      },
        { Modifier::Super,       "Super"    },
        { Modifier::CapsLock,    "CapsLock" },
        { Modifier::NumLock,     "NumLock"  },
    };

    auto it = enumNames.find(mod);
    return (it != enumNames.end())
        ? it->second
        : "Unknown keyboard modifier";
}

std::string to_string(const MouseButton mouseButton) {
    static const std::unordered_map<MouseButton, std::string> specialNames = {
        { MouseButton::Left,      "Left"   },
        { MouseButton::Right,     "Right"  },
        { MouseButton::Middle,    "Middle" },
    };

    static const std::unordered_map<MouseButton, std::string> enumNames = {
        { MouseButton::B1,   "B1" },
        { MouseButton::B2,   "B2" },
        { MouseButton::B3,   "B3" },
        { MouseButton::B4,   "B4" },
        { MouseButton::B5,   "B5" },
        { MouseButton::B6,   "B6" },
        { MouseButton::B7,   "B7" },
        { MouseButton::B8,   "B8" },
    };

    auto it = specialNames.find(mouseButton);
    if (it != specialNames.end()) return it->second;

    it = enumNames.find(mouseButton);
    return (it != enumNames.end())
        ? it->second
        : "Unknown mouse button";
}

std::string to_string(const Joystick joystick) {
    static const std::unordered_map<Joystick, std::string> enumNames = {
        { Joystick::J1,  "J1"  },
        { Joystick::J2,  "J2"  },
        { Joystick::J3,  "J3"  },
        { Joystick::J4,  "J4"  },
        { Joystick::J5,  "J5"  },
        { Joystick::J6,  "J6"  },
        { Joystick::J7,  "J7"  },
        { Joystick::J8,  "J8"  },
        { Joystick::J9,  "J9"  },
        { Joystick::J10, "J10" },
        { Joystick::J11, "J11" },
        { Joystick::J12, "J12" },
        { Joystick::J13, "J13" },
        { Joystick::J14, "J14" },
        { Joystick::J15, "J15" },
        { Joystick::J16, "J16" },
    };

    auto it = enumNames.find(joystick);
    return (it != enumNames.end())
        ? it->second
        : "Unknown joystick";
}

namespace Gamepad {
    std::string to_string(const Button gamepadButton)
    {
        static const std::unordered_map<Button, std::string> enumNames = {
            { Button::A,             "A"           },
            { Button::B,             "B"           },
            { Button::X,             "X"           },
            { Button::Y,             "Y"           },
            { Button::LeftBumper,    "LeftBumper"  },
            { Button::RightBumper,   "RightBumper" },
            { Button::Select,        "Select"      },
            { Button::Start,         "Start"       },
            { Button::Home,          "Home"        },
            { Button::LeftThumb,     "LeftThumb"   },
            { Button::RightThumb,    "RightThumb"  },
            { Button::DPadUp,        "DPadUp"      },
            { Button::DPadRight,     "DPadRight"   },
            { Button::DPadDown,      "DPadDown"    },
            { Button::DPadLeft,      "DPadLeft"    },
        };

        auto it = enumNames.find(gamepadButton);
        return (it != enumNames.end())
            ? it->second
            : "Unknown gamepad button";
    }

    std::string to_string(const Axis gamepadAxis)
    {
        static const std::unordered_map<Axis, std::string> enumNames = {
            { Axis::LeftX,           "LeftX"        },
            { Axis::LeftY,           "LeftY"        },
            { Axis::RightX,          "RightX"       },
            { Axis::RightY,          "RightY"       },
            { Axis::LeftTrigger,     "LeftTrigger"  },
            { Axis::RightTrigger,    "RightTrigger" },
        };

        auto it = enumNames.find(gamepadAxis);
        return (it != enumNames.end())
            ? it->second
            : "Unknown gamepad axis";
    }
} // namespace Gamepad

} // namespace rb::Window::Input

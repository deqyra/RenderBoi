#ifndef RENDERBOI_WINDOW_ENUMS_HPP
#define RENDERBOI_WINDOW_ENUMS_HPP

#include <array>
#include <string>

namespace rb::Window {

namespace Input {
    namespace Mode {
        /// @brief Literals describing the aspects of a window
        /// whose input mode can be modified
        enum class Target {
            Cursor,
            StickyKeys,
            StickyMouseButtons,
            LockKeyMods,
            RawMouseMotion,
        };
        std::string to_string(Target mode);

        /// @brief Literals describing the input modes which 
        /// certain aspects of a window may have
        enum class Value {
            True,
            False,
            NormalCursor,
            HiddenCursor,
            DisabledCursor,
        };
        std::string to_string(Value value);
    }

    /// @brief Literals describing an action which was performed
    /// on a key or button. The repeat action is ignored altogether
    enum class Action {
        Press,
        // Repeat,      // UNCOMMENT IF REPEAT KEYS MUST BE HANDLED
        Release,
    };
    std::string to_string(Action action);

    /// @brief Literals describing the keys on a keyboard
    enum class Key : unsigned int {
        Unknown,
        Space,
        Apostrophe,     /* ' */
        Comma,          /* , */
        Minus,          /* - */
        Period,         /* . */
        Slash,          /* / */
        Key0,
        Key1,
        Key2,
        Key3,
        Key4,
        Key5,
        Key6,
        Key7,
        Key8,
        Key9,
        Semicolon,      /* ; */
        Equal,          /* = */
        A,
        B,
        C,
        D,
        E,
        F,
        G,
        H,
        I,
        J,
        K,
        L,
        M,
        N,
        O,
        P,
        Q,
        R,
        S,
        T,
        U,
        V,
        W,
        X,
        Y,
        Z,
        LeftBracket,    /* [ */
        Backslash,      /* \ */
        RightBracket,   /* ] */
        GraveAccent,    /* ` */
        World1,         /* non-US #1 */
        World2,         /* non-US #2 */
        Escape,
        Enter,
        Tab,
        Backspace,
        Insert,
        Delete,
        Right,
        Left,
        Down,
        Up,
        PageUp,
        PageDown,
        Home,
        End,
        CapsLock,
        ScrollLock,
        NumLock,
        PrintScreen,
        Pause,
        F1,
        F2,
        F3,
        F4,
        F5,
        F6,
        F7,
        F8,
        F9,
        F10,
        F11,
        F12,
        F13,
        F14,
        F15,
        F16,
        F17,
        F18,
        F19,
        F20,
        F21,
        F22,
        F23,
        F24,
        F25,
        Keypad0,
        Keypad1,
        Keypad2,
        Keypad3,
        Keypad4,
        Keypad5,
        Keypad6,
        Keypad7,
        Keypad8,
        Keypad9,
        KeypadDecimal,
        KeypadDivide,
        KeypadMultiply,
        KeypadSubtract,
        KeypadAdd,
        KeypadEnter,
        KeypadEqual,
        LeftShift,
        LeftControl,
        LeftAlt,
        LeftSuper,
        RightShift,
        RightControl,
        RightAlt,
        RightSuper,
        Menu,
    };
    std::string to_string(Key key);

    /// @brief Literals describing modifier key statuses 
    /// Every literal is assigned a power of 2 in order to ease bitwise 
    /// operations on a recorded status flag
    enum class Modifier : unsigned int {
        None        = 0x0000,
        Shift       = 0x0001,
        Control     = 0x0002,
        Alt         = 0x0004,
        Super       = 0x0008,
        CapsLock    = 0x0010,
        NumLock     = 0x0020,
    };
    std::string to_string(Modifier mod);
    unsigned int operator&(unsigned int left, Modifier right);
    unsigned int operator|(unsigned int left, Modifier right);
    bool any(Modifier value);

    /// @brief Literals describing the buttons on a mouse
    enum class MouseButton : unsigned int {
        B1,
        B2,
        B3,
        B4,
        B5,
        B6,
        B7,
        B8,
        Last = B8,
        Left = B1,
        Right = B2,
        Middle = B3,
    };
    std::string to_string(MouseButton mouseButton);

    /// @brief Literals describing hardware joysticks that can
    /// be reached and polled
    enum class Joystick : unsigned int {
        J1  = 0x0,
        J2  = 0x1,
        J3  = 0x2,
        J4  = 0x3,
        J5  = 0x4,
        J6  = 0x5,
        J7  = 0x6,
        J8  = 0x7,
        J9  = 0x8,
        J10 = 0x9,
        J11 = 0xA,
        J12 = 0xB,
        J13 = 0xC,
        J14 = 0xD,
        J15 = 0xE,
        J16 = 0xF,
    };
    std::string to_string(Joystick joystick);

    inline constexpr std::array<Joystick, 16> Joysticks = {
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
        Joystick::J16,
    };

    namespace Gamepad {
        /// @brief Literals describing the buttons on a modern
        /// gamepad (XBox, DualShock, etc)
        enum class Button : unsigned int {
            A           = 0x0,
            B           = 0x1,
            X           = 0x2,
            Y           = 0x3,
            LeftBumper  = 0x4,
            RightBumper = 0x5,
            Select      = 0x6,
            Start       = 0x7,
            Home        = 0x8,
            LeftThumb   = 0x9,
            RightThumb  = 0xA,
            DPadUp      = 0xB,
            DPadRight   = 0xC,
            DPadDown    = 0xD,
            DPadLeft    = 0xE,
            Cross       = A,
            Circle      = B,
            Square      = X,
            Triangle    = Y,
        };
        std::string to_string(Button gamepadButton);

        /// @brief Literals describing the continous input 
        /// controllers on a modern gamepad
        enum class Axis : unsigned int {
            LeftX           = 0x0,
            LeftY           = 0x1,
            RightX          = 0x2,
            RightY          = 0x3,
            LeftTrigger     = 0x4,
            RightTrigger    = 0x5,
        };
        std::string to_string(Axis gamepadAxis);
    } // namespace Gamepad
} // namespace Input

/// @brief Literals describing the different OpenGL profiles
enum class OpenGLProfile {
    Core,
    Compatibility,
    Any,
};

} // namespace rb::Window

#endif//RENDERBOI_WINDOW_ENUMS_HPP
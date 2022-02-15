#ifndef RENDERBOI__WINDOW__GLFW3_ADAPTER_HPP
#define RENDERBOI__WINDOW__GLFW3_ADAPTER_HPP

#include <unordered_map>

#include "../enums.hpp"

namespace renderboi::Window
{

/// @brief Contains utilities to translate GLFW constants to RenderBoi enums 
/// and the other way around.
namespace GLFW3Adapter
{
    /// @brief Contains mapping data from RenderBoi enums to GLFW3 macros.
    namespace ValueMaps
    {
        const std::unordered_map<Input::Mode::Target, unsigned int>& InputModeTargetValues();
        const std::unordered_map<Input::Mode::Value, unsigned int>& InputModeValueValues();
        const std::unordered_map<Input::Action, unsigned int>& ActionValues();
        const std::unordered_map<Input::Key, unsigned int>& KeyValues();
        const std::unordered_map<Input::Modifier, unsigned int>& KeyModifierValues();
        const std::unordered_map<Input::MouseButton, unsigned int>& MouseButtonValues();
        const std::unordered_map<Input::Joystick, unsigned int>& JoystickValues();
        const std::unordered_map<Input::Gamepad::Button, unsigned int>& GamepadButtonValues();
        const std::unordered_map<Input::Gamepad::Axis, unsigned int>& GamepadAxisValues();
        const std::unordered_map<OpenGLProfile, unsigned int>& GlProfileValues();
    }

    /// @brief Contains mapping data from GLFW3 macros to RenderBoi enums.
    namespace EnumMaps
    {
        const std::unordered_map<unsigned int, Input::Mode::Target>& InputModeTargetEnums();
        const std::unordered_map<unsigned int, Input::Mode::Value>& InputModeValueEnums();
        const std::unordered_map<unsigned int, Input::Action>& ActionEnums();
        const std::unordered_map<unsigned int, Input::Key>& KeyEnums();
        const std::unordered_map<unsigned int, Input::Modifier>& KeyModifierEnums();
        const std::unordered_map<unsigned int, Input::MouseButton>& MouseButtonEnums();
        const std::unordered_map<unsigned int, Input::Joystick>& JoystickEnums();
        const std::unordered_map<unsigned int, Input::Gamepad::Button>& GamepadButtonEnums();
        const std::unordered_map<unsigned int, Input::Gamepad::Axis>& GamepadAxisEnums();
        const std::unordered_map<unsigned int, OpenGLProfile>& GlProfileEnums();
    }

    /// @brief Get the RenderBoi enum corresponding to the given GLFW3 macro.
    ///
    /// @tparam E Type of the literal to find.
    ///
    /// @param constant GLFW3 macro to translate into a RenderBoi literal.
    ///
    /// @return A literal of type E which describes the provided GLFW3 macro.
    template<typename E>
    E getEnum(const int constant);

    template<>
    Input::Mode::Target getEnum(const int macro);

    template<>
    Input::Mode::Value getEnum(const int macro);

    template<>
    Input::Action getEnum(const int macro);

    template<>
    Input::Key getEnum(const int macro);

    template<>
    Input::Modifier getEnum(const int macro);

    template<>
    Input::MouseButton getEnum(const int macro);

    template<>
    Input::Joystick getEnum(const int macro);

    template<>
    Input::Gamepad::Button getEnum(const int macro);

    template<>
    Input::Gamepad::Axis getEnum(const int macro);

    template<>
    OpenGLProfile getEnum(const int macro);

    /// @brief Get the GLFW3 macro corresponding to the given RenderBoi enum.
    ///
    /// @tparam E Type of the literal to translate.
    ///
    /// @param literal The literal to translate.
    ///
    /// @return The GLFW3 macro which corresponds to the given literal.
    template<typename E>
    int getValue(const E literal);

    template<>
    int getValue(const Input::Mode::Target literal);

    template<>
    int getValue(const Input::Mode::Value literal);

    template<>
    int getValue(const Input::Action literal);

    template<>
    int getValue(const Input::Key literal);

    template<>
    int getValue(const Input::Modifier literal);

    template<>
    int getValue(const Input::MouseButton literal);

    template<>
    int getValue(const Input::Joystick literal);

    template<>
    int getValue(const Input::Gamepad::Button literal);

    template<>
    int getValue(const Input::Gamepad::Axis literal);

    template<>
    int getValue(const OpenGLProfile literal);
}

} // namespace renderboi::Window

#endif//RENDERBOI__WINDOW__GLFW3_ADAPTER_HPP
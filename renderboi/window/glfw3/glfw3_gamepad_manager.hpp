#ifndef RENDERBOI_WINDOW_GLFW3_GLFW3_GAMEPAD_MANAGER_HPP
#define RENDERBOI_WINDOW_GLFW3_GLFW3_GAMEPAD_MANAGER_HPP

#include <atomic>
#include <unordered_map>
#include <vector>

#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>
#undef GLFW_INCLUDE_NONE

#include "../enums.hpp"
#include "../gamepad/gamepad_manager.hpp"

#include "glfw3_utilities.hpp"

namespace rb::Window {

/// @brief Specialization of a GamepadManager for GLFW3
class GLFW3GamepadManager : public GamepadManager {
private:
    friend void GLFW3Utilities::subscribeToGlfwJoystickStatus(GLWindow* const window);
    friend void GLFW3Utilities::unsubscribeFromGlfwJoystickStatus(GLWindow* const window);
    friend void GLFW3Utilities::globalGlfwJoystickCallback(const int jid, const int event);
    friend void GLFW3Utilities::initGamepadStatuses();

    using Joystick = Input::Joystick;

    /// @brief Structure mapping managed gamepad to litterals describing the 
    /// slot which they are assigned to
    mutable std::unordered_map<Joystick, GamepadPtr> _managedGamepads;

    /// @brief Structure mapping managed gamepad to whether or not polling for
    /// them is enabled
    mutable std::unordered_map<Joystick, std::atomic<bool>> _enabledGamepads;
    
    /// @brief Structure mapping gamepad manager IDs to the instances with those IDs
    static std::unordered_map<unsigned int, GamepadManager&> _GamepadManagers;

    /// @brief Structure mapping joystick IDs to whether or not they are present
    /// on the system
    static std::unordered_map<int, std::atomic<bool>> _PresentJoysticks;

    /// @brief Structure mapping joystick IDs to whether or not they are gamepads
    static std::unordered_map<int, std::atomic<bool>> _PresentGamepads;

    /// @brief Whether or not some newly detected gamepads need to be scanned for
    /// more info about their status
    static std::atomic<bool> _JoystickStatusRefreshFlag;

    /// @brief Get a Renderboi gamepad state from a GLFW3 gamepad state
    ///
    /// @param glfwState GLFW gamepad state to convert to a Renderboi gamepad state
    static GamepadState _GamepadStateFromGlfwGamepadState(const GLFWgamepadstate& glfwState);

public:
    GLFW3GamepadManager();

    //////////////////////////////////////////////
    ///                                        ///
    /// Methods overridden from GamepadManager ///
    ///                                        ///
    //////////////////////////////////////////////

    /// @brief Callback for when a gamepad is connected on a slot
    /// 
    /// @param slot Litteral describing the slot on which the gamepad was
    /// connected
    void gamepadConnected(const Joystick slot) const override;

    /// @brief Callback for when a gamepad is disconnected from a slot
    /// 
    /// @param slot Litteral describing the slot from which the gamepad was
    /// disconnected
    void gamepadDisconnected(const Joystick slot) const override;

    /// @brief Get an array filled with litterals representing handles to
    /// present gamepads. This function may be called only from the main thread
    ///
    /// @param mustBeUnused Whether to return all present gamepads or only those
    /// not already in use by the window
    ///
    /// @return An array filled with litterals representing handles to present 
    /// gamepads
    std::vector<Joystick> pollPresentGamepads(const bool mustBeUnused = true) const override;

    /// @brief Get a gamepad plugged into a certain slot This function may be 
    /// called only from the main thread
    ///
    /// @param slot Virtual slot on which to find the controller to manage
    ///
    /// @return A reference to the gamepad
    ///
    /// @exception If a gamepad cannot be found on the provided slot, the 
    /// function will throw an std::runtime_error
    Gamepad& getGamepad(const Joystick slot) override;

    /// @brief Enable polling the state for a gamepad May be called from any
    /// thread
    ///
    /// @param slot Virtual slot on which to find the controller to start polling
    void startGamepadPolling(const Joystick slot) const override;

    /// @brief Disable polling the state for a gamepad May be called from any
    /// thread
    ///
    /// @param slot Virtual slot on which to find the controller to stop polling
    void stopGamepadPolling(const Joystick slot) const override;

    /// @brief Process any pending gamepad connection event
    void refreshGamepadStatuses() const override;

    /// @brief Poll the state for gamepads. And forward it to gamepads
    void pollGamepadStates() const override;
};

} // namespace rb::Window


#endif//RENDERBOI_WINDOW_GLFW3_GLFW3_GAMEPAD_MANAGER_HPP
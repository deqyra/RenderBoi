#ifndef RENDERBOI_WINDOW_GAMEPAD_GAMEPAD_MANAGER_HPP
#define RENDERBOI_WINDOW_GAMEPAD_GAMEPAD_MANAGER_HPP

#include "gamepad.hpp"
#include "../enums.hpp"

namespace rb {
namespace Window {

/// @brief Interface for an entity in charge of managing gamepad handles and 
/// forwarding input from the window
class GamepadManager {
private:
    using Joystick = Window::Input::Joystick;

    /// @brief An instance count used for uniquely identifying instances
    static unsigned int _count;

public:
    GamepadManager();
    virtual ~GamepadManager() = default;

    /// @brief Callback for when a gamepad is connected on a slot
    /// 
    /// @param slot Litteral describing the slot on which the gamepad was
    /// connected
    virtual void gamepadConnected(const Joystick slot) const = 0;

    /// @brief Callback for when a gamepad is disconnected from a slot
    /// 
    /// @param slot Litteral describing the slot from which the gamepad was
    /// disconnected
    virtual void gamepadDisconnected(const Joystick slot) const = 0;

    /// @brief Get an array filled with litterals representing handles to
    /// present gamepads. This function may be called only from the main thread
    ///
    /// @param unused Whether to return all present gamepads or only those not
    /// already in use by the window
    ///
    /// @return An array filled with litterals representing handles to present 
    /// gamepads
    virtual std::vector<Joystick> pollPresentGamepads(const bool unused = true) const = 0;

    /// @brief Get a gamepad plugged into a certain slot The gamepad can then
    /// be enabled to start receiving input from the main thread This function 
    /// may be called only from the main thread
    ///
    /// @param slot Virtual slot on which to find the managed controller
    ///
    /// @return A referenced to the requested managed gamepad
    ///
    /// @exception If a gamepad cannot be found on the provided slot, the 
    /// function will throw an std::runtime_error
    virtual Gamepad& getGamepad(const Joystick slot) = 0;

    /// @brief Enable polling the state for a gamepad May be called from any
    /// thread
    virtual void startGamepadPolling(const Joystick slot) const = 0;

    /// @brief Disable polling the state for a gamepad May be called from any
    /// thread
    virtual void stopGamepadPolling(const Joystick slat) const = 0;

    /// @brief Process any pending gamepad connection event
    virtual void refreshGamepadStatuses() const = 0;

    /// @brief Poll the state for gamepads and forward it to gamepad entities
    virtual void pollGamepadStates() const = 0;

    /// @brief Unique ID of the gamepad manager
    const unsigned int id;

protected:
    /// @brief Method allowing inheriting classes to set the state of a gamepad
    /// without privileged access to it
    ///
    /// @param gamepad Reference to the gamepad instance whose state to set
    /// @param state State to set within the gamepad
    static void setGamepadState(Gamepad& gamepad, GamepadState state);

    /// @brief Method allowing inheriting classes to notify a gamepad about its
    /// connection status without privileged access to it
    ///
    /// @param gamepad Reference to the gamepad instance to notify of its 
    /// connection status
    static void setGamepadConnected(Gamepad& gamepad);

    /// @brief Method allowing inheriting classes to notify a gamepad about its
    /// disconnection status without privileged access to it
    ///
    /// @param gamepad Reference to the gamepad instance to notify of its 
    /// disconnection status
    static void setGamepadDisconnected(Gamepad& gamepad);

    /// @brief Method allowing inheriting classes to instantiate a gamepad,
    /// without privileged access to it (private constructor may not be called
    /// directly by them)
    ///
    /// @param args Argument to construct the gamepad with
    template<typename ...ArgTypes>
    static GamepadPtr createGamepad(ArgTypes&& ...args);
};

template<typename ...ArgTypes>
GamepadPtr GamepadManager::createGamepad(ArgTypes&& ...args) {
    return GamepadPtr(new Gamepad(std::forward<ArgTypes>(args)...));
}

} // namespace Window

using GamepadManager = Window::GamepadManager;
using GamepadManagerPtr = std::unique_ptr<GamepadManager>;

} // namespace rb

#endif//RENDERBOI_WINDOW_GAMEPAD_GAMEPAD_MANAGER_HPP
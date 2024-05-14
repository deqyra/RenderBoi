#include "gamepad_manager.hpp"

namespace rb::Window {

unsigned int GamepadManager::_count = 0;

GamepadManager::GamepadManager() :
    id(_count++)
{

}

void GamepadManager::setGamepadState(Gamepad& gamepad, GamepadState state) {
    gamepad._setNewState(state);
}

void GamepadManager::setGamepadConnected(Gamepad& gamepad) {
    gamepad._processConnected();
}

void GamepadManager::setGamepadDisconnected(Gamepad& gamepad) {
    gamepad._processDisconnected();
}

} // namespace rb::Window
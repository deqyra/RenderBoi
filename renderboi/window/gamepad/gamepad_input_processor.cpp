#include "gamepad_input_processor.hpp"

namespace Renderboi::Window
{

unsigned int GamepadInputProcessor::_count = 0;

GamepadInputProcessor::GamepadInputProcessor() :
    id(_count++)
{

}

void GamepadInputProcessor::processConnected(const Gamepad& gamepad)
{

}

void GamepadInputProcessor::processDisconnected(const Gamepad& gamepad)
{

}

void GamepadInputProcessor::processButton(const Gamepad& gamepad, const Window::Input::Gamepad::Button button, const Window::Input::Action action)
{

}

void GamepadInputProcessor::processAxis(const Gamepad& gamepad, const Window::Input::Gamepad::Axis axis, const float value)
{

}

} // namespace Renderboi::Window
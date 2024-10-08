#include "input_logger.hpp"

#include <bitset>
#include <iomanip>
#include <vector>

#include <renderboi/window/gl_window.hpp>

namespace rb {

InputLogger::InputLogger(std::ostream& outputStream) :
    _inputLoggingStatus(),
    _outputStream(outputStream)
{
    const std::vector<IEventType>& inputTypeList = InputProcessor::EventTypeList;
    
    for (const auto& inputType : inputTypeList)
    {
        _inputLoggingStatus[inputType] = true;
    }

    const std::vector<GEventType>& gamepadInputTypeList = GamepadInputProcessor::EventTypeList;
    
    for (const auto inputType : gamepadInputTypeList)
    {
        _gamepadInputLoggingStatus[inputType] = true;
    }
}

void InputLogger::enableEventLog(const IEventType eventType) {
    _inputLoggingStatus[eventType] = true;
}

void InputLogger::disableEventLog(const IEventType eventType) {
    _inputLoggingStatus[eventType] = false;
}

void InputLogger::enableEventLog(const GEventType eventType) {
    _gamepadInputLoggingStatus[eventType] = true;
}

void InputLogger::disableEventLog(const GEventType eventType) {
    _gamepadInputLoggingStatus[eventType] = false;
}

void InputLogger::setEventLoggingStatus(const IEventType eventType, const bool enable) {
    _inputLoggingStatus[eventType] = enable;
}

void InputLogger::setEventLoggingStatus(const GEventType eventType, const bool enable) {
    _gamepadInputLoggingStatus[eventType] = enable;
}

void InputLogger::processFramebufferResize(
    GLWindow& window,
    const unsigned int width,
    const unsigned int height
) {
    if (!_inputLoggingStatus[IEventType::FramebufferResize]) return;

    _outputStream << "Event on window \"" << window.getTitle() << "\": framebuffer resized.\n"
                     "New dimensions: w=" << width << ";h=" << height << ".\n" << std::endl;
}

void InputLogger::processKeyboard(
    GLWindow& window, 
    const Key key, 
    const int scancode, 
    const Action action,
    const int mods
) {
    if (!_inputLoggingStatus[IEventType::Keyboard]) return;

    std::bitset<8> bits(mods);

    _outputStream << "Event on window \"" << window.getTitle() << "\": keyboard.\n"
                     "Key: " << to_string(key) << " (scancode: " << std::hex << scancode << std::dec << ")\n"
                     "Action: " << to_string(action) << "\n"
                     "Mod flags: " << bits << "\n" << std::endl;
}

void InputLogger::processMouseButton(
    GLWindow& window, 
    const MButton button, 
    const Action action, 
    const int mods
) {
    if (!_inputLoggingStatus[IEventType::MouseButton]) return;

    std::bitset<8> bits(mods);

    _outputStream << "Event on window \"" << window.getTitle() << "\": mouse button.\n"
                     "Button: " << to_string(button) << "\n"
                     "Action: " << to_string(action) << "\n"
                     "Mod flags: " << bits << "\n" << std::endl;
}

void InputLogger::processMouseCursor(
    GLWindow& window,
    const double xpos,
    const double ypos
) {
    if (!_inputLoggingStatus[IEventType::MouseCursor]) return;

    _outputStream << std::setprecision(3) <<
                     "Event on window \"" << window.getTitle() << "\": mouse moved.\n"
                     "New position: x=" << xpos << ";y=" << ypos << ".\n" << std::endl;
}

void InputLogger::processConnected(const Gamepad& gamepad) {
    if (!_gamepadInputLoggingStatus[GEventType::Connect]) return;

    _outputStream << "Gamepad \"" << gamepad.name << "\" (slot " << to_string(gamepad.slot) << ") was connected.\n" << std::endl;
}

void InputLogger::processDisconnected(const Gamepad& gamepad) {
    if (!_gamepadInputLoggingStatus[GEventType::Disconnect]) return;

    _outputStream << "Gamepad \"" << gamepad.name << "\" (slot " << to_string(gamepad.slot) << ") was disconnected.\n" << std::endl;
}

void InputLogger::processButton(const Gamepad& gamepad, const GButton button, const Window::Input::Action action) {
    if (!_gamepadInputLoggingStatus[GEventType::Button]) return;

    _outputStream << "Event on gamepad (slot " << to_string(gamepad.slot) << "): button.\n"
                     "Gamepad name: " << gamepad.name << "\n"
                     "Button: " << to_string(button) << "\n"
                     "Action: " << to_string(action) << "\n" << std::endl;
}

void InputLogger::processAxis(const Gamepad& gamepad, const Axis axis, const float value) {
    if (!_gamepadInputLoggingStatus[GEventType::Axis]) return;

    _outputStream << std::setprecision(3) <<
                     "Event on gamepad (slot " << to_string(gamepad.slot) << "): axis.\n"
                     "Gamepad name: " << gamepad.name << "\n"
                     "Axis: " << to_string(axis) << "\n"
                     "New value: " << value << "\n" << std::endl;
}

} // namespace rb

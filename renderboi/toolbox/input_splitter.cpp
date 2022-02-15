#include "input_splitter.hpp"

#include <stdexcept>

namespace renderboi
{

InputSplitter::InputSplitter() :
    _subscriberRollingCount(0),
    _subscribers(),
    _gamepadSubscribers()
{

}

unsigned int InputSplitter::registerInputProcessor(InputProcessor& inputProcessor)
{
    _subscribers.insert({_subscriberRollingCount, inputProcessor});
    return _subscriberRollingCount++;
}

void InputSplitter::detachInputProcessor(const unsigned int subscriptionId)
{
    _subscribers.erase(subscriptionId);
}

void InputSplitter::detachAllInputProcessors()
{
    _subscribers.clear();
}

unsigned int InputSplitter::registerGamepadInputProcessor(GamepadInputProcessor& inputProcessor)
{
    _gamepadSubscribers.insert({_subscriberRollingCount, inputProcessor});
    return _subscriberRollingCount++;
}

void InputSplitter::detachGamepadInputProcessor(const unsigned int subscriptionId)
{
    _gamepadSubscribers.erase(subscriptionId);
}

void InputSplitter::detachAllIGamepadnputProcessors()
{
    _gamepadSubscribers.clear();
}

void InputSplitter::processFramebufferResize(GLWindow& window, const unsigned int width, const unsigned int height)
{
    for (auto& [_, inputProc] : _subscribers)
    {
        inputProc.processFramebufferResize(window, width, height);
    }
}

void InputSplitter::processKeyboard(
    GLWindow& window, 
    const Key key, 
    const int scancode, 
    const Action action, 
    const int mods
)
{
    for (auto& [_, inputProc] : _subscribers)
    {
        inputProc.processKeyboard(window, key, scancode, action, mods);
    }
}

void InputSplitter::processMouseButton(
    GLWindow& window, 
    const MButton button, 
    const Action action, 
    const int mods
)
{
    for (auto& [_, inputProc] : _subscribers)
    {
        inputProc.processMouseButton(window, button, action, mods);
    }
}

void InputSplitter::processMouseCursor(GLWindow& window, const double xpos, const double ypos)
{
    for (auto& [_, inputProc] : _subscribers)
    {
        inputProc.processMouseCursor(window, xpos, ypos);
    }
}

void InputSplitter::processConnected(const Gamepad& gamepad)
{
    for (auto& [_, inputProc] : _gamepadSubscribers)
    {
        inputProc.processConnected(gamepad);
    }
}

void InputSplitter::processDisconnected(const Gamepad& gamepad)
{
    for (auto& [_, inputProc] : _gamepadSubscribers)
    {
        inputProc.processDisconnected(gamepad);
    }
}

void InputSplitter::processButton(const Gamepad& gamepad, const GButton button, const Action action)
{
    for (auto& [_, inputProc] : _gamepadSubscribers)
    {
        inputProc.processButton(gamepad, button, action);
    }
}

void InputSplitter::processAxis(const Gamepad& gamepad, const Axis axis, const float value)
{
    for (auto& [_, inputProc] : _gamepadSubscribers)
    {
        inputProc.processAxis(gamepad, axis, value);
    }
}

} // namespace renderboi

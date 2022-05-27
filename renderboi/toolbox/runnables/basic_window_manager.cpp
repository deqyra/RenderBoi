#include "basic_window_manager.hpp"

#include <iostream>
#include <string>

#include <glad/gl.h>

#include <renderboi/window/gl_window.hpp>
#include <renderboi/window/enums.hpp>

namespace renderboi
{

BasicWindowManager::BasicWindowManager(GLWindow& window) :
    _window(window)
{
    
}

void BasicWindowManager::triggerAction(const BasicWindowManagerAction& action)
{
    switch (action)
    {
    case BasicWindowManagerAction::Terminate:
        _window.signalExit();
        break;

    case BasicWindowManagerAction::PolygonFill:
        _setPolygonMode(PolygonMode::Fill);
        break;

    case BasicWindowManagerAction::PolygonLine:
        _setPolygonMode(PolygonMode::Line);
        break;

    case BasicWindowManagerAction::PolygonPoint:
        _setPolygonMode(PolygonMode::Point);
        break;
        
    case BasicWindowManagerAction::ToggleFullscreen:
        _toggleFullscreen();
    }
}

void BasicWindowManager::stopAction(const BasicWindowManagerAction& action)
{

}

void BasicWindowManager::processFramebufferResize(
    GLWindow& _window,
    const unsigned int width,
    const unsigned int height
)
{
    _window.forwardContextEvent(Window::GLContextEvent::FitFramebufferToWindow);
}

const ControlScheme<BasicWindowManagerAction>& BasicWindowManager::getDefaultControlScheme() const
{
    using Window::Input::Key;
    static ControlScheme<BasicWindowManagerAction> scheme = {
        { Control(Key::Escape), BasicWindowManagerAction::Terminate },
        { Control(Key::Space),  BasicWindowManagerAction::Terminate },
        { Control(Key::F1),     BasicWindowManagerAction::PolygonFill },
        { Control(Key::F2),     BasicWindowManagerAction::PolygonLine },
        { Control(Key::F3),     BasicWindowManagerAction::PolygonPoint },
        { Control(Key::F11),    BasicWindowManagerAction::ToggleFullscreen }
    };

    return scheme;
}

void BasicWindowManager::_toggleFullscreen() const
{
    if (_window.isFullscreen())
    {
        _window.exitFullscreen();
    }
    else
    {
        _window.goFullscreen(true);
    }
}

void BasicWindowManager::_setPolygonMode(const PolygonMode mode) const
{
    using Window::GLContextEvent;

    switch (mode)
    {
    case PolygonMode::Fill:
        _window.forwardContextEvent(GLContextEvent::PolygonModeFill);
        break;

    case PolygonMode::Line:
        _window.forwardContextEvent(GLContextEvent::PolygonModeLine);
        break;

    case PolygonMode::Point:
        _window.forwardContextEvent(GLContextEvent::PolygonModePoint);
        break;
    }
}

std::string to_string(const BasicWindowManagerAction action)
{
    static std::unordered_map<BasicWindowManagerAction, std::string> enumNames =
    {
        {BasicWindowManagerAction::Terminate,          "Terminate"},
        {BasicWindowManagerAction::PolygonFill,        "PolygonFill"},
        {BasicWindowManagerAction::PolygonLine,        "PolygonLine"},
        {BasicWindowManagerAction::PolygonPoint,       "PolygonPoint"},
        {BasicWindowManagerAction::ToggleFullscreen,   "ToggleFullscreen"},
    };

    auto it = enumNames.find(action);
    return (it != enumNames.end())
        ? it->second
        : "Unknown";
}

} // namespace renderboi

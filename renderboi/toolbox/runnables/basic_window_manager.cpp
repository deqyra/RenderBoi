#include "basic_window_manager.hpp"

#include <iostream>
#include <string>

#include <glad/gl.h>

#include <renderboi/window/gl_window.hpp>
#include <renderboi/window/enums.hpp>

namespace Renderboi
{

BasicWindowManager::BasicWindowManager()
{
    
}

void BasicWindowManager::triggerAction(const BasicWindowManagerAction& action, GLWindow& window)
{
    switch (action)
    {
    case BasicWindowManagerAction::Terminate:
        window.signalExit();
        break;

    case BasicWindowManagerAction::PolygonFill:
        _setPolygonMode(window, PolygonMode::Fill);
        break;

    case BasicWindowManagerAction::PolygonLine:
        _setPolygonMode(window, PolygonMode::Line);
        break;

    case BasicWindowManagerAction::PolygonPoint:
        _setPolygonMode(window, PolygonMode::Point);
        break;
        
    case BasicWindowManagerAction::ToggleFullscreen:
        _toggleFullscreen(window);
    }
}

void BasicWindowManager::stopAction(const BasicWindowManagerAction& action, GLWindow& window)
{

}

void BasicWindowManager::processFramebufferResize(
    GLWindow& window,
    const unsigned int width,
    const unsigned int height
)
{
    window.forwardContextEvent(Window::GLContextEvent::FitFramebufferToWindow);
}

const ControlScheme<BasicWindowManagerAction>& BasicWindowManager::getDefaultControlScheme() const
{
    static ControlScheme<BasicWindowManagerAction> scheme;
    static bool runOnce = false;

    if (!runOnce)
    {
        using Window::Input::Key;
        scheme.bindControl(Control(Key::Escape), BasicWindowManagerAction::Terminate);
        scheme.bindControl(Control(Key::Space), BasicWindowManagerAction::Terminate);
        scheme.bindControl(Control(Key::F1), BasicWindowManagerAction::PolygonFill);
        scheme.bindControl(Control(Key::F2), BasicWindowManagerAction::PolygonLine);
        scheme.bindControl(Control(Key::F3), BasicWindowManagerAction::PolygonPoint);
        scheme.bindControl(Control(Key::F11), BasicWindowManagerAction::ToggleFullscreen);

        runOnce = true;
    }

    return scheme;
}

void BasicWindowManager::_toggleFullscreen(GLWindow& window) const
{
    if (window.isFullscreen())
    {
        window.exitFullscreen();
    }
    else
    {
        window.goFullscreen(true);
    }
}

void BasicWindowManager::_setPolygonMode(GLWindow& window, const PolygonMode mode) const
{
    using Window::GLContextEvent;

    switch (mode)
    {
    case PolygonMode::Fill:
        window.forwardContextEvent(GLContextEvent::PolygonModeFill);
        break;

    case PolygonMode::Line:
        window.forwardContextEvent(GLContextEvent::PolygonModeLine);
        break;

    case PolygonMode::Point:
        window.forwardContextEvent(GLContextEvent::PolygonModePoint);
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

} // namespace Renderboi

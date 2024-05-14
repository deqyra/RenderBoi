#include "basic_window_manager.hpp"

#include <glad/gl.h>

#include <renderboi/window/gl_window.hpp>
#include <renderboi/window/enums.hpp>

namespace rb {

BasicWindowManager::BasicWindowManager(GLWindow& window) :
    _window(window)
{
    
}

void BasicWindowManager::triggerAction(const BasicWindowManagerAction& action) {
    using enum BasicWindowManagerAction;
    switch (action) {
    case Terminate:        _window.signalExit();                break;
    case PolygonFill:      _setPolygonMode(PolygonMode::Fill);  break;
    case PolygonLine:      _setPolygonMode(PolygonMode::Line);  break;
    case PolygonPoint:     _setPolygonMode(PolygonMode::Point); break;
    case ToggleFullscreen: _toggleFullscreen();
    }
}

void BasicWindowManager::stopAction(const BasicWindowManagerAction& action) {

}

void BasicWindowManager::processFramebufferResize(
    GLWindow& _window,
    const unsigned int width,
    const unsigned int height
)
{
    _window.forwardContextEvent(Window::GLContextEvent::FitFramebufferToWindow);
}

const ControlScheme<BasicWindowManagerAction>& BasicWindowManager::defaultControlScheme() const {
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

void BasicWindowManager::_toggleFullscreen() const {
    if (_window.isFullscreen())
    {
        _window.exitFullscreen();
    }
    else
    {
        _window.goFullscreen(true);
    }
}

void BasicWindowManager::_setPolygonMode(const PolygonMode mode) const {
    using enum Window::GLContextEvent;
    using enum PolygonMode;

    switch (mode) {
    case Fill:  _window.forwardContextEvent(PolygonModeFill);  break;
    case Line:  _window.forwardContextEvent(PolygonModeLine);  break;
    case Point: _window.forwardContextEvent(PolygonModePoint); break;
    }
}

} // namespace rb

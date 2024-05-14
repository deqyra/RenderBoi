#include "gl_context_event_manager.hpp"

#include <glad/gl.h>

#include "../gl_window.hpp"

namespace rb {

namespace Window {

GLContextEventManager::GLContextEventManager(GLWindow& window) :
    _window(window),
    _eventMutex(),
    _eventQueue()
{

}

void GLContextEventManager::processPendingEvents() {
    // Process all awaiting events
    _eventMutex.lock();
    while (!_eventQueue.empty())
    {
        GLContextEvent e = _eventQueue.front();
        _eventMutex.unlock();

        _processEvent(e);

        _eventMutex.lock();
        _eventQueue.pop();
    }
    _eventMutex.unlock();
}

void GLContextEventManager::queueEvent(const GLContextEvent& event) {
    _eventMutex.lock();
    _eventQueue.push(event);
    _eventMutex.unlock();
}

void GLContextEventManager::_processEvent(const GLContextEvent event) {
    switch (event)
    {
    case GLContextEvent::FitFramebufferToWindow:
        int w, h;
        _window.getFramebufferSize(w, h);
        glViewport(0, 0, w, h);
        break;

    case GLContextEvent::PolygonModeFill:
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
        break;
        
    case GLContextEvent::PolygonModeLine:
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
        break;

    case GLContextEvent::PolygonModePoint:
        glPolygonMode(GL_FRONT_AND_BACK, GL_POINT);
        break;

    default:
        std::string s = "GLContextEventManager: cannot process unknown event \""
        + to_string(event) + "\"."; 
        throw std::runtime_error(s.c_str());
    }
}

}// namespace Window

}// namespace rb

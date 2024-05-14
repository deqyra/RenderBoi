#include "gl_window.hpp"

#include <memory>
#include <string>

#include "input_processor.hpp"
#include "event/gl_context_event_manager.hpp"

namespace rb::Window {

const InputProcessorPtr GLWindow::_DefaultInputProcessor = std::make_unique<InputProcessor>();

GLWindow::GLWindow(std::string title) :
    _title(title),
    _exitSignaled(false),
    _gamepadManager(nullptr),
    _glContextEventManager(std::make_unique<GLContextEventManager>(*this)),
    _inputProcessor(_DefaultInputProcessor.get())
{

}

void GLWindow::processFramebufferResize(const int width, const int height) {
    _inputProcessor->processFramebufferResize(*this, (unsigned int) width, (unsigned int) height);
}

void GLWindow::processKeyboard(
    const Window::Input::Key key,
    const int scancode,
    const Window::Input::Action action,
    const int mods
) {
    _inputProcessor->processKeyboard(*this, key, scancode, action, mods);
}

void GLWindow::processMouseButton(
    const Window::Input::MouseButton button,
    const Window::Input::Action action,
    const int mods
) {
    _inputProcessor->processMouseButton(*this, button, action, mods);
}

void GLWindow::processMouseCursor(const double xpos, const double ypos) {
    _inputProcessor->processMouseCursor(*this, xpos, ypos);
}

std::string GLWindow::getTitle() const {
    return _title;
}

void GLWindow::signalExit(const bool value) {
    _exitSignaled = value;
}

bool GLWindow::exitSignaled() {
    return _exitSignaled;
}

GamepadManager& GLWindow::getGamepadManager() {
    return *_gamepadManager;
}

void GLWindow::registerContextEventManager(std::unique_ptr<GLContextEventManager>&& contextEventManager) {
    if (!contextEventManager)
    {
        throw std::runtime_error("GLWindow: cannot register null context event manager.");
    }
    
    _glContextEventManager = std::move(contextEventManager);
}

void GLWindow::detachContextEventManager() {
    _glContextEventManager = std::make_unique<GLContextEventManager>(*this);
}

void GLWindow::forwardContextEvent(const GLContextEvent event) {
    _glContextEventManager->queueEvent(event);
}

void GLWindow::registerInputProcessor(InputProcessor& inputProcessor) {
    _inputProcessor = &inputProcessor;
}

const InputProcessor& GLWindow::getInputProcessor() {
    return *_inputProcessor;
}

void GLWindow::detachInputProcessor() {
    _inputProcessor = _DefaultInputProcessor.get();
}

void GLWindow::processPendingContextEvents() {
    _glContextEventManager->processPendingEvents();
}

void GLWindow::pollAllEvents() {
    pollEvents();

    _gamepadManager->refreshGamepadStatuses();
    _gamepadManager->pollGamepadStates();
}

void GLWindow::startPollingLoop() {
    _exitSignaled = false;
    while (!_exitSignaled)
    {
        pollAllEvents();
    }
}

} // namespace rb::Window

#include "input_processor.hpp"

namespace rb::Window {

// Initialize static stuff
unsigned int InputProcessor::_count = 0;

InputProcessor::InputProcessor() :
    id(_count++)                    // Generate ID from instance count
{

}

// Default callbacks do nothing

void InputProcessor::processFramebufferResize(GLWindow& window, const unsigned int width, const unsigned int height) {

}

void InputProcessor::processKeyboard(
    GLWindow& window,
    const Window::Input::Key key,
    const int scancode,
    const Window::Input::Action action,
    const int mods
)
{

}

void InputProcessor::processMouseButton(
    GLWindow& window,
    const Window::Input::MouseButton button,
    const Window::Input::Action action,
    const int mods
)
{

}

void InputProcessor::processMouseCursor(GLWindow& window, const double xpos, const double ypos) {

}

} // namespace rb::Window

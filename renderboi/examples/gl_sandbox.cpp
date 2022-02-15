#include "gl_sandbox.hpp"

#include <glad/gl.h>

#include <renderboi/utilities/gl_utilities.hpp>

namespace renderboi
{

GLSandbox::GLSandbox(GLWindow& window, const GLSandboxParameters params) :
    _window(window),
    _parameters(params)
{

}

void GLSandbox::_initContext()
{
    _window.makeContextCurrent();

    if (_parameters.debug)
    {
        if (!_window.extensionSupported("GL_ARB_debug_output"))
        {
            throw std::runtime_error("A debug context was requested but GL_ARB_debug_output is not available.");
        }

        glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS_ARB);
        glDebugMessageCallbackARB(glDebugCallback, nullptr);
        glDebugMessageControlARB(GL_DONT_CARE, GL_DONT_CARE, GL_DONT_CARE, 0, nullptr, GL_TRUE);
    }
}

void GLSandbox::_terminateContext()
{
    _window.releaseContext();
}

}

#ifndef WINDOW__TOOLS_HPP
#define WINDOW__TOOLS_HPP

#include <string>

#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>
#undef GLFW_INCLUDE_NONE

#include "gl_window.hpp"

/// @brief Create an OpenGL context within a GLFW window, bind callbacks and 
/// initialize OpenGL function pointers.
///
/// @param title Title of the window.
/// @param width Width of the window.
/// @param height Height of the window.
/// @param glVersionMajor Version major of the OpenGL specification to use.
/// @param glVersionMinor Version minor of the OpenGL specification to use.
/// @param glProfile Literal describing the OpenGL profile to use.
/// @param bool Whether or not to print out the debugging output.
///
/// @return A pointer to an instantiated GLWindow.
///
/// @exception If the window could not be created, or if the OpenGL function 
/// pointers could not be loaded, the function will throw a std::runtime_error.
/// If a debug capable context was requested but the environment does not 
/// support GL_ARB_debug_output, the function will throw a std::runtime_error.
GLWindowPtr makeGLFWWindow(std::string title, int width = 640, int height = 480, int glVersionMajor = 1, int glVersionMinor = 0, Window::OpenGLProfile glProfile = Window::OpenGLProfile::Core, bool debug = false);

/// @brief Callback given to OpenGL, which will print out error messages from
/// debug capable contexts.
void APIENTRY glDebugOutput(unsigned int source, unsigned int type, unsigned int id, unsigned int severity, int length, const char* message, const void* userParam);

#endif//WINDOW__TOOLS_HPP
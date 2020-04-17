#ifndef WINDOWING__TOOLS_HPP
#define WINDOWING__TOOLS_HPP

#include <string>

#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>
#undef GLFW_INCLUDE_NONE

#include "gl_window.hpp"

// Create a GL context within a GLFW window, bind callbacks and initialize GL function pointers
GLWindowPtr makeWindow(std::string title, int width = 640, int height = 480, int glVersionMajor = -1, int glVersionMinor = -1, int glProfile = GLFW_OPENGL_CORE_PROFILE, bool debug = false);

// Output callback, called upon getting GL errors
void APIENTRY glDebugOutput(unsigned int source, unsigned int type, unsigned int id, unsigned int severity, int length, const char* message, const void* userParam);

// Return the aspect ratio (width / height) of the current context
float glAspectRatio();

#endif//WINDOWING__TOOLS_HPP
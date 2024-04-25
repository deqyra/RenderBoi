/*      // \\
       //   \\
      //     \\
     //WARNING\\
    //         \\
    =============
    
    DO NOT edit window/env_info.hpp, or your changes
    will be OVERWRITTEN at the next run of CMake.
    Instead, edit window/config/env_info.config.hpp.
*/

#ifndef RENDERBOI__WINDOW__ENV_INFO_HPP
#define RENDERBOI__WINDOW__ENV_INFO_HPP

#if 0
#define EGL_DETECTED
#endif

#if 1
    #define GLFW3_BORDERLESS_POLICY_NATIVE_MODE

    #if ( != 1) && ( == 1)
        #undef GLFW3_BORDERLESS_POLICY_NATIVE_MODE
        #define GLFW3_BORDERLESS_POLICY_MAX_MODE
    #endif

    #include <renderboi/window/glfw3/glfw3_window_factory.hpp>
    #include <renderboi/window/glfw3/glfw3_utilities.hpp>
          
    static const void* AppWindowErrorCallback = (void*)(&renderboi::Window::GLFW3Utilities::globalGlfwErrorCallback);
#endif

#include "window_backend.hpp"

namespace rb = renderboi;
namespace rbw = rb::Window;

static constexpr rbw::WindowBackend AppBackend = rbw::WindowBackend::GLFW3;

#endif//RENDERBOI__WINDOW__ENV_INFO_HPP

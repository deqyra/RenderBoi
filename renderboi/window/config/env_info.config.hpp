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

#ifndef RENDERBOI_WINDOW_ENV_INFO_HPP
#define RENDERBOI_WINDOW_ENV_INFO_HPP

#if @EGL_DETECTED@
#define EGL_DETECTED
#endif

#if @USE_GLFW3@
    #define GLFW3_BORDERLESS_POLICY_NATIVE_MODE

    #if (@GLFW3_BORDERLESS_POLICY_NATIVE_MODE_VAL@ != 1) && (@GLFW3_BORDERLESS_POLICY_MAX_MODE_VAL@ == 1)
        #undef GLFW3_BORDERLESS_POLICY_NATIVE_MODE
        #define GLFW3_BORDERLESS_POLICY_MAX_MODE
    #endif

    #include <renderboi/window/glfw3/glfw3_window_factory.hpp>
    #include <renderboi/window/glfw3/glfw3_utilities.hpp>
          
    static const void* AppWindowErrorCallback = (void*)(&rb::Window::GLFW3Utilities::globalGlfwErrorCallback);
#endif

#include <renderboi/window/window_backend.hpp>

namespace rbw = rb::Window;

static constexpr rbw::WindowBackend AppBackend = rbw::WindowBackend::@WINDOW_BACKEND_ENUM@;

#endif//RENDERBOI_WINDOW_ENV_INFO_HPP
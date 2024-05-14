#ifndef RENDERBOI_EXAMPLES_GL_SANDBOX_PARAMETERS_HPP
#define RENDERBOI_EXAMPLES_GL_SANDBOX_PARAMETERS_HPP

#include <renderboi/window/gl_window.hpp>

namespace rb {

/// @brief POD struct storing parameters to run a sandbox with
struct GLSandboxParameters {
    /// @brief Tells whether debug output should be enabled in the sandbox
    bool debug;
};

}

#endif//RENDERBOI_EXAMPLES_GL_SANDBOX_PARAMETERS_HPP
#ifndef RENDERBOI__EXAMPLES__GL_SANDBOX_PARAMETERS_HPP
#define RENDERBOI__EXAMPLES__GL_SANDBOX_PARAMETERS_HPP

#include <renderboi/window/gl_window.hpp>

namespace renderboi
{

/// @brief POD struct storing parameters to run a sandbox with.
struct GLSandboxParameters
{
    /// @brief Tells whether debug output should be enabled in the sandbox.
    bool debug;
};

}

#endif//RENDERBOI__EXAMPLES__GL_SANDBOX_PARAMETERS_HPP
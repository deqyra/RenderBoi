#ifndef RENDERBOI_EXAMPLES_GL_SANDBOX_RUNNER_HPP
#define RENDERBOI_EXAMPLES_GL_SANDBOX_RUNNER_HPP

#include <concepts>
#include <memory>
#include <thread>

#include <renderboi/window/gl_window.hpp>
#include <renderboi/window/gamepad/gamepad_manager.hpp>

#include "gl_sandbox.hpp"
#include "gl_sandbox_parameters.hpp"

namespace rb {

/// @brief Encapsulates the instantiation, set-up, execution and tear-down of a 
/// GLSandbox
///
/// @tparam T Type of the GLSandbox to instantiate
template<std::derived_from<GLSandbox> T>
class GLSandboxRunner {
public:
    using SandboxType = T;
    using SandboxPtr = std::unique_ptr<T>;

private:
    /// @brief Pointer to the hosted sandbox
    SandboxPtr _sandbox;

    /// @brief Reference to the window to run the sandbox on
    GLWindow& _window;

public:
    /// @param window Reference to the window to run the sandbox on
    /// @param params Parameters to run the sandbox with
    GLSandboxRunner(GLWindow& window, const GLSandboxParameters& params) :
        _window(window),
        _sandbox(std::make_unique<SandboxType>(window, params))
    {

    }

    ~GLSandboxRunner() {
        _sandbox->tearDown();
    }

    /// @brief Get a pointer to the sandbox being ran
    ///
    /// @return A pointer to the sandbox being ran
    SandboxType& getSandbox() {
        return *_sandbox;
    }

    /// @brief Poll the input of the window repeatedly, until the window's exit
    /// signal is raised by another thread Then, the worker is requested to
    /// finalize, and the function returns when it is
    void run() {
        _sandbox->setUp();

		std::thread th(&GLSandbox::run, _sandbox.get());
		_window.startPollingLoop();

		th.join();
		_sandbox->tearDown();
    }
};

}

#endif//RENDERBOI_EXAMPLES_GL_SANDBOX_RUNNER_HPP
#ifndef RENDERBOI_EXAMPLES_GL_SANDBOX_HPP
#define RENDERBOI_EXAMPLES_GL_SANDBOX_HPP

#include <renderboi/window/gl_window.hpp>

#include "gl_sandbox_parameters.hpp"

namespace rb {

/// @brief Abstract class to run any 3D scene in a GL window
class GLSandbox {
public:
    /// @brief Set up the window prior to running the example
    /// @note Must be called from the main thread
	virtual void setUp() = 0;

    /// @brief Run something in the provided GL window
    /// @note Should run on its own thread
	virtual void run() = 0;

    /// @brief Restore the window back to how it was before the example ran
    /// This function should perform the opposite steps from setUp().
    /// @note Must be called from the main thread once run() has returned
	virtual void tearDown() = 0;

protected:
	/// @param window Reference to the window on which the sandbox should run
	/// @param params Strcture packing the parameters according to which the
	/// sandbox should run
	GLSandbox(GLWindow& window, const GLSandboxParameters params);

	/// @brief Initialize an OpenGL context for the sandbox
	virtual void _initContext();

	/// @brief Release the OpenGL context of the sandbox
	virtual void _terminateContext();

	/// @brief Reference to the window on which the sandbow should run
	GLWindow& _window;

	/// @brief Structure packing the parameters according to which the 
	/// sandbox should run
	GLSandboxParameters _parameters;

private:
	GLSandbox(const GLSandbox&) = delete;
	GLSandbox(GLSandbox&&) = delete;
	GLSandbox& operator=(const GLSandbox&) = delete;
	GLSandbox& operator=(GLSandbox&&) = delete;
};

} // namespace rb

#endif//RENDERBOI_EXAMPLES_GL_SANDBOX_HPP
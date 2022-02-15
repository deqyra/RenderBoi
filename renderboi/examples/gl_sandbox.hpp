#ifndef RENDERBOI__EXAMPLES__GL_SANDBOX_HPP
#define RENDERBOI__EXAMPLES__GL_SANDBOX_HPP

#include <memory>
#include <string>

#include <renderboi/window/gl_window.hpp>

#include "gl_sandbox_parameters.hpp"

namespace renderboi
{

/// @brief Abstract class to run any 3D scene in a GL window.
class GLSandbox
{
	public:
		/// @brief Set up the window prior to running the example. Must be
		/// called from the main thread.
		virtual void setUp() = 0;

        /// @brief Run something in the provided GL window. Must be run by a
		/// separate thread.
		virtual void run() = 0;

		/// @brief Restore the window back to how it was before the example ran.
		/// The contents of this function should be the opposite from those in
		/// setUp(). Must be called from the main thread once run() has returned.
		virtual void tearDown() = 0;

	protected:
		/// @param window Reference to the window on which the sandbox should run.
        /// @param params Strcture packing the parameters according to which the
        /// sandbox should run.
		GLSandbox(GLWindow& window, const GLSandboxParameters params);

		/// @brief Initialize an OpenGL context for the sandbox.
		virtual void _initContext();

		/// @brief Release the OpenGL context of the sandbox.
		virtual void _terminateContext();

		/// @brief Reference to the window on which the sandbow should run.
		GLWindow& _window;

		/// @brief Structure packing the parameters according to which the 
		/// sandbox should run.
		GLSandboxParameters _parameters;
};

} // namespace renderboi

#endif//RENDERBOI__EXAMPLES__GL_SANDBOX_HPP
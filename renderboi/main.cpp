#include <exception>
#include <iostream>
#include <string>
#include <vector>

#include <renderboi/window/enums.hpp>
#include <renderboi/window/window_factory.hpp>
#include <renderboi/window/window_backend.hpp>

#include <renderboi/examples/gl_sandbox.hpp>
#include <renderboi/examples/lighting_sandbox.hpp>
#include <renderboi/examples/shadow_sandbox.hpp>

#include "project_macros.hpp"

#ifdef _WIN32
	#include <windows.h>
#endif

using GLFW3 = WindowFactory<WindowBackend::GLFW3>;

// Halt the execution with a clean exit
int abortWithError(std::string message, bool terminateGLFW = true);

// Function to use for error-reporting by the window backend
static void glfwErrorCallback(int error, const char* description);

// Instantiate all available sandboxes
std::vector<GLSandbox*> createAllSandboxes();

//Initialise OpenGL and display a window with an active GL context
int main(int argc, char** argv)
{
	#ifdef _WIN32
		SetConsoleOutputCP(65001);
	#endif

    std::cout << PROJECT_NAME << " v" << PROJECT_VERSION << '\n';
    std::cout << COPYLEFT_NOTICE << std::endl;

	GLFW3::setGLFWErrorCallback(glfwErrorCallback);

	if (!GLFW3::initializeBackend())
		return EXIT_FAILURE;

	// Init window, GL context and GL pointers
	GLWindowPtr window;
	try
	{
		window = GLFW3::makeWindow("RenderBoi", 1280, 720, GL_CONTEXT_VERSION_MAJOR, GL_CONTEXT_VERSION_MINOR, Window::OpenGLProfile::Core, true);
	}
	catch(const std::exception& e)
	{
		std::cerr << "Exception thrown during window creation:" << '\n'<< e.what() << std::endl;
		return abortWithError("Window creation failed. Aborting...");
	}
	
    // Instantiate and run examples
	std::vector<GLSandbox*> examples = createAllSandboxes();
    for (auto it = examples.begin(); it != examples.end(); it++)
    {
        (*it)->run(window);
        delete (*it);
    }

	// Destroy window by resetting what should be the only shared pointer to it
	window = nullptr;
	GLFW3::terminateBackend();
	return EXIT_SUCCESS;
}

int abortWithError(std::string message, bool terminateGLFW)
{
	std::cerr << message;
	if (terminateGLFW)
	{
		GLFW3::terminateBackend();
	}

	return EXIT_FAILURE;
}

static void glfwErrorCallback(int error, const char* description)
{
	std::cerr << "GLFW error: 0x" << std::hex << error << ", \"" << description << "\"" << std::endl;
}

std::vector<GLSandbox*> createAllSandboxes()
{
    // Try and instantiate all sandboxes.
    try
    {
        return std::vector<GLSandbox*>({
            //new LightingSandbox(),
			new ShadowSandbox()
        });
    }
    catch (std::runtime_error e)
    {
        std::cerr << "createAllSandboxes: " << e.what();
    }

    return std::vector<GLSandbox*>();
}
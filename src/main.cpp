#include <exception>
#include <iostream>
#include <string>
#include <vector>

#include "glad/glad.h"
#include "GLFW/glfw3.h"

#include "tools/gl_utils.hpp"
#include "gl_sandbox.hpp"
#include "gl_sandboxes/lighting_sandbox.hpp"

// Shortcut-function to halt the execution of the program with a clean exit.
int abortWithError(std::string message, bool terminateGLFW = true);

// Instantiate all available sandboxes
std::vector<GLSandbox*> createAllSandboxes();

//Initialise OpenGL and display a window with an active GL context
int main(int argc, char** argv)
{
	glfwInit();
	// Init window, GL context and GL pointers
	GLFWwindow* window = makeWindow("Test", 1280, 720);

	if (!window)
	{
		return abortWithError("Window creation failed. Aborting...");
	}

    // Instantiate and run examples
	std::vector<GLSandbox*> examples = createAllSandboxes();
    for (auto it = examples.begin(); it != examples.end(); it++)
    {
        (*it)->run(window);
        delete (*it);
    }

	// Tear down GL context and destroy window
	destroyWindow(window);
	glfwTerminate();
	return EXIT_SUCCESS;
}

int abortWithError(std::string message, bool terminateGLFW)
{
	std::cerr << message;
	if (terminateGLFW)
	{
		glfwTerminate();
	}

	return EXIT_FAILURE;
}

std::vector<GLSandbox*> createAllSandboxes()
{
    // Try and instantiate all sandboxes.
    try
    {
        return std::vector<GLSandbox*>({
            new LightingSandbox()
        });
    }
    catch (std::runtime_error e)
    {
        std::cerr << "createAllSandboxes: " << e.what();
    }

    return std::vector<GLSandbox*>();
}
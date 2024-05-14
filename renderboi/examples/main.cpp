#include <cstdlib>
#include <exception>
#include <filesystem>
#include <iostream>

#ifdef _WIN32
	#include "sane_windows.h" // IWYU pragma: keep
#endif//_WIN32

#include <renderboi/window/enums.hpp>
#include <renderboi/window/window_factory.hpp>
#include <renderboi/window/window_backend.hpp>

#include <renderboi/utilities/gl_utilities.hpp>
#include <renderboi/utilities/resource_locator.hpp>

#include <cpptools/cli/argument_parsing.hpp>

#include "gl_sandbox_parameters.hpp"
#include "gl_sandbox_runner.hpp"
#include "lighting_sandbox.hpp"
//#include "shadow_sandbox.hpp"

#include "project_env.hpp"
#include "renderboi_parameters.hpp"

namespace rbw = rb::Window;
namespace fs = std::filesystem;

namespace {

void printHelp() {
    std::cout
		<< PROJECT_NAME << " demo executable, v" << PROJECT_VERSION << "\n"
		<< "Usage: " << PROJECT_NAME << " [(-a|--assets) <path>]\n"
		<< "\n"
		<< "<path>: path to the directory where assets/ is located." << std::endl;
}

}

//Initialise OpenGL and display a window with an active GL context
int main(int argc, const char** argv) {
	if (AppBackend == rbw::WindowBackend::Unknown)
	{
		std::cerr << "No window backend was selected for the app. Cannot run examples." << std::endl;
		return EXIT_FAILURE;
	}

	#ifdef _WIN32
		SetConsoleOutputCP(65001);
	#endif

	RenderboiParameters rbParams = {
		.assetsPath = fs::current_path()
	};

	{
		using namespace tools::cli;

		auto parsedArgs = parse_arguments(argc, argv);

		auto helpArg = argument_name{ .long_name = "help", .short_name = 'h' };
		if (parsedArgs.has(helpArg)) {
			printHelp();
			return EXIT_SUCCESS;
		}

		auto assetsPathArg = argument_name{ .long_name = "assets", .short_name = 'a' };
		if (parsedArgs.has(assetsPathArg)) {
			rbParams.assetsPath = parsedArgs[assetsPathArg].at(0);
		}
	}

	fs::path assetsDir = fs::absolute(rbParams.assetsPath / "assets/");
	if (!fs::exists(assetsDir))
	{
		std::cerr 
			<< "Error: assets/ could not be found in the current directory, "
			<< "or in the path provided by command line argument.\n"
			<< "Path currently in use: " << assetsDir.string() << "\n"
			<< "Aborting..." << std::endl;
		
		return EXIT_FAILURE;
	}

	using ReLoc = rb::ResourceLocator;
	using ReType = rb::ResourceType;
	ReLoc::setPrefixFor(ReType::ShaderSource, assetsDir / "shaders/");
	ReLoc::setPrefixFor(ReType::Texture,      assetsDir / "textures/");
	ReLoc::setPrefixFor(ReType::Any,          assetsDir);

    std::cout << PROJECT_NAME << " v" << PROJECT_VERSION << '\n';
    std::cout << COPYRIGHT_NOTICE << '\n';
	std::cout << MIT_LICENSE_NOTICE << '\n' << std::endl;

	if (!AppWindowFactory::InitializeBackend())
	{
		std::cout << "Failed to initialize window backend. Aborting..." << std::endl;
		return EXIT_FAILURE;
	}

	// RAII-friendly scope :)
	{
		// Init window, GL context and GL pointers
		rbw::GLWindowPtr window;
		rbw::WindowCreationParameters windowParams = {
			.title                  = "RenderBoi",
			.width                  = 1280,
			.height                 = 720,
			.resizable              = true,
			.glVersionMajor         = GL_CONTEXT_VERSION_MAJOR,
			.glVersionMinor         = GL_CONTEXT_VERSION_MINOR,
			.glProfile              = rbw::OpenGLProfile::Core,
			.shareContext           = nullptr,
			.monitor                = nullptr,
			.borderlessFullscreen   = false,
			.autoMinimize           = true,
			.decorated              = true,
			.transparentFramebuffer = false,
			.visible                = true,
			.maximized              = false,
			.alwaysOnTop            = false,
			.focused                = true,
			.focusOnShow            = true,
			.scaleToMonitor         = true,
			.debug                  = true 
		};

		try
		{
			window = AppWindowFactory::MakeWindow(windowParams);
		}
		catch(const std::exception& e)
		{
			std::cerr 	<< "Exception thrown during window creation:\n"
						<< e.what() << '\n'
						<< "Window creation failed. Aborting..." << std::endl;

			AppWindowFactory::TerminateBackend();
			return EXIT_FAILURE; 
		}

		rb::glIgnoreDebugMessagesOfType(GL_DEBUG_TYPE_PERFORMANCE_ARB);

		const rb::GLSandboxParameters sbParams = {
			.debug = true
		};

		// Run examples

		{
			auto lightingSandbox = rb::GLSandboxRunner<rb::LightingSandbox>(*window, sbParams);

			lightingSandbox.run();
		}

		// {
			// auto shadowSandbox = rb::GLSandboxRunner<rb::ShadowSandbox>(*window, sbParams);

			// shadowSandbox.run();
		// }

		AppWindowFactory::DestroyWindow(std::move(window));
	}

	AppWindowFactory::TerminateBackend();

	return EXIT_SUCCESS;
}

#include "glfw3_window_factory.hpp"

#include <algorithm>
#include <cstdlib>
#include <cstdarg>
#include <cstring>
#include <fstream>
#include <iostream>
#include <iterator>
#include <string>

#include <glad/gl.h>
#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>
#undef GLFW_INCLUDE_NONE

#include <cpptools/utility/string_tools.hpp>

#include <renderboi/utilities/gl_utilities.hpp>
#include <renderboi/utilities/resource_locator.hpp>

#include "../enums.hpp"
#include "../env_info.hpp"
#include "../gl_window.hpp"
#include "../window_backend.hpp"
#include "../window_factory.hpp"
#include "../window_creation_parameters.hpp"

#include "glfw3_adapter.hpp"
#include "glfw3_monitor.hpp"
#include "glfw3_window.hpp"
#include "glfw3_utilities.hpp"

namespace renderboi::Window
{

using ReLoc = ResourceLocator;
using ReType = ResourceType;

std::map<unsigned int, Monitor::VideoMode>
WindowFactory<WindowBackend::GLFW3>::_nativeVideoModes = std::map<unsigned int, Monitor::VideoMode>();

std::map<unsigned int, GLFW3MonitorPtr>
WindowFactory<WindowBackend::GLFW3>::_monitors = std::map<unsigned int, GLFW3MonitorPtr>();

GLFWmonitorfun* WindowFactory<WindowBackend::GLFW3>::_monitorCallback = nullptr;

int WindowFactory<WindowBackend::GLFW3>::InitializeBackend()
{
    int result = glfwInit();
    if (!result) return result;

    glfwSetErrorCallback(GLFW3Utilities::globalGlfwErrorCallback);

    ///////////////////////////
    // GAMEPAD RELATED STUFF //
    ///////////////////////////
    std::string gamepadMappingsPath = ReLoc::locate(ReType::Any, "gamecontrollerdb.txt");
    std::string gamepadMappings = cpptools::String::readFileIntoString(gamepadMappingsPath);
    glfwUpdateGamepadMappings(gamepadMappings.c_str());
    glfwSetJoystickCallback(GLFW3Utilities::globalGlfwJoystickCallback);
    GLFW3Utilities::initGamepadStatuses();

    ///////////////////////////
    // MONITOR RELATED STUFF //
    ///////////////////////////
    _monitors.clear();
    _monitors = _ListMonitors();
    _nativeVideoModes.clear();
    _SaveMonitorVideoModes();
    glfwSetMonitorCallback(_GlobalGlfwMonitorCallback);

    return result;
}

void WindowFactory<WindowBackend::GLFW3>::TerminateBackend()
{
    _monitors.clear();
    _nativeVideoModes.clear();
    
    glfwTerminate();
}

void WindowFactory<WindowBackend::GLFW3>::SetErrorCallback(const void* callback)
{
    glfwSetErrorCallback(*((ErrorCallbackSignature*)callback));
}

Monitor& WindowFactory<WindowBackend::GLFW3>::GetPrimaryMonitor()
{
    GLFWmonitor* monitor = glfwGetPrimaryMonitor();
    GLFW3Monitor& glfw3Monitor = *(GLFW3Monitor*)(glfwGetMonitorUserPointer(monitor));
    return glfw3Monitor;
}

std::map<unsigned int, Monitor&> WindowFactory<WindowBackend::GLFW3>::GetMonitors()
{
    std::map<unsigned int, Monitor&> monitors;

    std::transform(
        _monitors.begin(), _monitors.end(),
        std::inserter(monitors, monitors.end()),
        [](const auto& pair) -> const std::pair<unsigned int, Monitor&>
        {
            const auto& [id, monitor] = pair;
            return {id, *monitor};
        }
    );

    return monitors;
}

void WindowFactory<WindowBackend::GLFW3>::SetMonitorCallback(const void* callback)
{
    _monitorCallback = (MonitorCallbackSignature*) callback;
}

Monitor::VideoMode WindowFactory<WindowBackend::GLFW3>::GetMonitorNativeVideoMode(const Monitor& monitor)
{
    auto it = _nativeVideoModes.find(monitor.id);
    if (it == _nativeVideoModes.end())
        throw std::runtime_error("WindowFactory<GLFW3>: native video mode could not be retrieved for passed monitor.");

    return it->second;
}

GLWindowPtr WindowFactory<WindowBackend::GLFW3>::MakeWindow(const WindowCreationParameters& params)
{
    // Window hints
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR,      params.glVersionMajor);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR,      params.glVersionMinor);
    glfwWindowHint(GLFW_RESIZABLE,                  params.resizable                ? GL_TRUE : GL_FALSE);
    glfwWindowHint(GLFW_AUTO_ICONIFY,               params.autoMinimize             ? GL_TRUE : GL_FALSE);
    glfwWindowHint(GLFW_DECORATED,                  params.decorated                ? GL_TRUE : GL_FALSE);
    glfwWindowHint(GLFW_VISIBLE,                    params.visible                  ? GL_TRUE : GL_FALSE);
    glfwWindowHint(GLFW_MAXIMIZED,                  params.maximized                ? GL_TRUE : GL_FALSE);
    glfwWindowHint(GLFW_FLOATING,                   params.alwaysOnTop              ? GL_TRUE : GL_FALSE);
    glfwWindowHint(GLFW_FOCUSED,                    params.focused                  ? GL_TRUE : GL_FALSE);
    glfwWindowHint(GLFW_FOCUS_ON_SHOW,              params.focusOnShow              ? GL_TRUE : GL_FALSE);
    glfwWindowHint(GLFW_SCALE_TO_MONITOR,           params.scaleToMonitor           ? GL_TRUE : GL_FALSE);
    glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT,       params.debug                    ? GL_TRUE : GL_FALSE);

#ifndef EGL_DETECTED
	glfwWindowHint(GLFW_OPENGL_PROFILE,             Window::GLFW3Adapter::getValue(params.glProfile));
#endif//EGL_DETECTED

    // Any GLWindow pointer is EXPECTED to be a GLFW3Window pointer, allowing for a static_cast

    // Shared GL context
    GLFWwindow* sharedContextWindow = nullptr;
    if (params.shareContext != nullptr)
    {
        const GLFW3Window* const glfw3SharedWindow = static_cast<const GLFW3Window* const>(params.shareContext);
        sharedContextWindow = glfw3SharedWindow->_w;
    }

    // Fullscreen
    GLFWmonitor* fullscreenMonitor = nullptr;
    if (params.monitor != nullptr)
    {
        const GLFW3Monitor* const glfw3Monitor = static_cast<const GLFW3Monitor* const>(params.monitor);
        fullscreenMonitor = glfw3Monitor->_m;
    }

    int width = params.width;
    int height = params.height;
    int refreshRate = GLFW_DONT_CARE;
    // Bit depths for red, green and blue buffers
    int framebufferDepth[3] = {8, 8, 8};

    if (params.borderlessFullscreen)
    {
        if (fullscreenMonitor == nullptr)
        {
            fullscreenMonitor = glfwGetPrimaryMonitor();
        }

        GLFW3Monitor* glfw3Monitor = (GLFW3Monitor*) glfwGetMonitorUserPointer(fullscreenMonitor);
        Monitor::VideoMode mode = _nativeVideoModes[glfw3Monitor->id];
        width = mode.width;
        height = mode.height;
        refreshRate = mode.refreshRate;
        framebufferDepth[0] = mode.redBits;
        framebufferDepth[1] = mode.greenBits;
        framebufferDepth[2] = mode.blueBits;
    }

    // Framebuffer hints    
    glfwWindowHint(GLFW_TRANSPARENT_FRAMEBUFFER,    params.transparentFramebuffer   ? GL_TRUE : GL_FALSE);
    glfwWindowHint(GLFW_RED_BITS,                   framebufferDepth[0]);
    glfwWindowHint(GLFW_GREEN_BITS,                 framebufferDepth[1]);
    glfwWindowHint(GLFW_BLUE_BITS,                  framebufferDepth[2]);
    glfwWindowHint(GLFW_REFRESH_RATE,               refreshRate);

	// Instantiate window
	GLFWwindow* window = glfwCreateWindow(width, height, params.title.c_str(), fullscreenMonitor, sharedContextWindow);

	if (!window)
	{
        throw std::runtime_error("WIndowsFactory<GLFW3>: Failed to create window.");
    }

    // Initialize a GLWindow instance with a GLFWwindow object
    GLFW3WindowPtr glfw3Window = std::make_unique<GLFW3Window>(window, params.title);
    // Set the user pointer of the GLFWwindow to the newly created GLWindow instance
    GLWindowPtr glWindow = std::move(glfw3Window);
    glfwSetWindowUserPointer(window, (void*)glWindow.get());

    // Then, a function can retrieve the GLWindow instance from the GLFWwindow
    // object and call the appropriate callback on the GLWindow instance.
    // Bind all relevant callbacks to such functions.
    glfwSetFramebufferSizeCallback(window, globalGlfwFramebufferResizeCallback);
    glfwSetKeyCallback(window, globalGlfwKeyboardCallback);
    glfwSetMouseButtonCallback(window, globalGlfwMouseButtonCallback);
    glfwSetCursorPosCallback(window, globalGlfwMouseCursorCallback);

    // Plug in joystick events
    GLFW3Utilities::subscribeToGlfwJoystickStatus(glWindow.get());

    return std::move(glWindow);
}

void WindowFactory<WindowBackend::GLFW3>::DestroyWindow(GLWindowPtr&& window)
{
    // Any GLWindow pointer passed in is REQUIRED to be a GLFW3Window
    GLFWwindow* glfw3Window = static_cast<GLFW3Window*>(window.get())->_w;
    GLFW3Utilities::unsubscribeFromGlfwJoystickStatus(window.get());
    glfwSetWindowShouldClose(glfw3Window, true);
    glfwDestroyWindow(glfw3Window);
}

std::map<unsigned int, GLFW3MonitorPtr> WindowFactory<WindowBackend::GLFW3>::_ListMonitors()
{
    int count;
    GLFWmonitor** monitors = glfwGetMonitors(&count);

    std::map<unsigned int, GLFW3MonitorPtr> managedMonitors;
    for (int i = 0; i < count; i++)
    {
        GLFW3Monitor* glfw3Monitor = new GLFW3Monitor(monitors[i]);
        glfwSetMonitorUserPointer(monitors[i], static_cast<void*>(glfw3Monitor));

        managedMonitors[glfw3Monitor->id] = GLFW3MonitorPtr(glfw3Monitor);
    }

    return managedMonitors;
}

void WindowFactory<WindowBackend::GLFW3>::_SaveMonitorVideoModes()
{
    for (const auto& [id, monitor] : _monitors)
    {
        _nativeVideoModes[id] = monitor->getCurrentVideoMode();
    }
}

void WindowFactory<WindowBackend::GLFW3>::_GlobalGlfwMonitorCallback(GLFWmonitor* m, int event)
{
    if (_monitorCallback != nullptr)
        (*_monitorCallback)(m, event);

    if (event == GLFW_CONNECTED)
    {
        GLFW3Monitor* glfw3Monitor = new GLFW3Monitor(m);
        glfwSetMonitorUserPointer(m, static_cast<void*>(glfw3Monitor));

        _monitors[glfw3Monitor->id] = GLFW3MonitorPtr(glfw3Monitor);
    }
    else // if (event == GLFW_DISCONNECTED)
    {
        GLFW3Monitor* monitor = (GLFW3Monitor*)glfwGetMonitorUserPointer(m);
        if (monitor != nullptr)
        {
            _monitors.erase(monitor->id);
            _nativeVideoModes.erase(monitor->id);
        }
    }
}

} // namespace renderboi::Window

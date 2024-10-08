#ifndef RENDERBOI_WINDOW_GLFW3_GLFW3_WINDOW_FACTORY_HPP
#define RENDERBOI_WINDOW_GLFW3_GLFW3_WINDOW_FACTORY_HPP

#include <map>

#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>
#undef GLFW_INCLUDE_NONE

#include "../window_factory.hpp"
#include "../window_backend.hpp"
#include "../window_creation_parameters.hpp"

#include "glfw3_monitor.hpp"

namespace rb::Window {

/// @brief GLFW3 specialization of the window factory These functions may be 
/// called only from the main thread
template<>
class WindowFactory<WindowBackend::GLFW3> {
public:
    using ErrorCallbackSignature = GLFWerrorfun;
    using MonitorCallbackSignature = GLFWmonitorfun;

private:
    /// @brief List of monitors connected to the system (mapped by IDs)
    static std::map<unsigned int, GLFW3MonitorPtr> _monitors;

    /// @brief List of video modes the monitors were using when detected on the system
    static std::map<unsigned int, Monitor::VideoMode> _nativeVideoModes;

    /// @brief Pointer to the user callback for monitor (dis)connection events
    static GLFWmonitorfun* _monitorCallback;

    /// @brief List the monitors currently connected to the system
    static std::map<unsigned int, GLFW3MonitorPtr> _ListMonitors();

    /// @brief Save the video modes of monitors currently connected to the system
    static void _SaveMonitorVideoModes();

    /// @brief Callback for monitor (dis)connection events in GLFW
    ///
    /// @param m Pointer to the monitor which the (dis)connection event relates to
    /// @param event Constant describing the event which just took place
    static void _GlobalGlfwMonitorCallback(GLFWmonitor* m, int event);

public:
    static int InitializeBackend();

    static void TerminateBackend();

    static void SetErrorCallback(const void* callback);

    static Monitor& GetPrimaryMonitor();

    static std::map<unsigned int, Monitor&> GetMonitors();

    static void SetMonitorCallback(const void* callback);

    static Monitor::VideoMode GetMonitorNativeVideoMode(const Monitor& monitor);

    static GLWindowPtr MakeWindow(const WindowCreationParameters& params);

    static void DestroyWindow(GLWindowPtr&& window);
};

} // namespace rb::Window

#endif//RENDERBOI_WINDOW_GLFW3_GLFW3_WINDOW_FACTORY_HPP
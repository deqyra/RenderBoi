#ifndef RENDERBOI_WINDOW_GLFW3_GLFW3_MONITOR_HPP
#define RENDERBOI_WINDOW_GLFW3_GLFW3_MONITOR_HPP

#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>
#undef GLFW_INCLUDE_NONE

#include "../monitor.hpp"
#include "../window_backend.hpp"
#include "../window_factory.hpp"

/// @brief Wrapper for a monitor resource in GLFW3
namespace rb::Window {

template<>
class WindowFactory<WindowBackend::GLFW3>;

class GLFW3Monitor : public Monitor {
private:
    friend WindowFactory<WindowBackend::GLFW3>;

    /// @brief Pointer to the underlying GLFWmonitor
    GLFWmonitor* _m;

    GLFW3Monitor(GLFWmonitor* monitor);

    /// @brief Array containing all video modes supported by this monitor
    std::vector<VideoMode> _supportedVideoModes;

    /// @brief Pointer to the video mode which the monitor is currently in
    VideoMode* _currentVideoMode;

    /// @brief A hypothetical made up largest video mode which the
    /// monitor would support
    VideoMode _largestVideoMode;

    /// @brief Enumerate the video modes supported by the monitor in a vector
    ///
    /// @param m Pointer to the GLFW monitor whose supported video modes should
    /// be enumerated
    static std::vector<VideoMode> _ListVideoModes(GLFWmonitor* m);

    /// @brief Get the largest video mode supported by the monitor This 
    /// function simply takes in all video modes supported by the monitor and
    /// returns a new "maximum" video mode made from the largest encountered 
    /// parameters across all video modes The monitor MAY NOT actually support
    /// such a video mode, but GLFW will select the closest available video mode
    /// upon attempting to set the monitor to the maximum video mode
    ///
    /// @param modes Array of video modes from which to make up a largest video
    /// mode
    ///
    /// @return The "largest" video mode supported by the monitor
    static VideoMode _LargestVideoMode(const std::vector<VideoMode>& modes);

public:
    ~GLFW3Monitor();

    /// @brief Get the pointer to the actual GLFW monitor
    ///
    /// @return The pointer to the actual GLFW monitor
    GLFWmonitor* getGlfwMonitorPointer() const;

    ///////////////////////////////
    ///                         ///
    /// Overridden from Monitor ///
    ///                         ///
    ///////////////////////////////

    /// @brief Get the current video mode of the monitor
    ///
    /// @return The current video mode of the monitor
    const VideoMode getCurrentVideoMode() const override;

    /// @brief Get the video modes supported by the monitor
    ///
    /// @return An array filled with the video modes support by the monitor
    const std::vector<VideoMode>& getVideoModes() const override;

    /// @brief Get the physical size of the monitor in millimetres
    ///
    /// @param[out] width_mm To receive the width of the monitor
    /// @param[out] height_mm To receive the height of the monitor
    void getPhysicalSize(int& width_mm, int& height_mm) const override;

    /// @brief Get the content scale of the monitor
    ///
    /// @param[out] xscale To receive the X content scale of the monitor
    /// @param[out] yscale To receive the Y content scale of the monitor
    void getContentScale(float& xscale, float& yscale) const override;

    /// @brief Get the virtual position of the monitor in screen coordinates
    /// (which are not necessarily 1:1 to pixels)
    ///
    /// @param[out] xpos To receive the X position of the monitor
    /// @param[out] ypos To receive the Y position of the monitor
    void getPosition(int& xpos, int& ypos) const override;

    /// @brief Get the work area of the monitor, that is the area of the monitor
    /// which is not occupied by system widgets like task bars and such The 
    /// coordinates of the area are screen coordinates
    ///
    /// @param[out] xpos To receive the X position of the work area
    /// @param[out] ypos To receive the Y position of the work area
    /// @param[out] width To receive the width of the work area
    /// @param[out] height To receive the height of the work area
    void getWorkArea(int& xpos, int& ypos, int& width, int& height) const override;

    /// @brief Get the gamma ramp of the monitor
    ///
    /// @return The gamma ramp of the monitor
    Monitor::GammaRamp getGammaRamp() const override;

    /// @brief Set the gamma ramp of the monitor The size of the ramp 
    ///
    /// @return The gamma ramp to set for the monitor
    void setGammaRamp(const Monitor::GammaRamp& gammaRamp) const override;

    /// @brief Get the hypothetical "largest" video mode supported by the monitor
    ///
    /// @return The "largest" video mode supported by the monitor
    VideoMode getLargestVideoMode() const override;
};

using GLFW3MonitorPtr = std::unique_ptr<GLFW3Monitor>;

} // namespace rb::Window

#endif//RENDERBOI_WINDOW_GLFW3_GLFW3_MONITOR_HPP
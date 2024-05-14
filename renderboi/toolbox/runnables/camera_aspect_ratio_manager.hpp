#ifndef RENDERBOI_TOOLBOX_RUNNABLES_CAMERA_ASPECT_RATIO_MANAGER_HPP
#define RENDERBOI_TOOLBOX_RUNNABLES_CAMERA_ASPECT_RATIO_MANAGER_HPP

#include <renderboi/core/3d/camera.hpp>

#include <renderboi/window/input_processor.hpp>
#include <renderboi/window/gl_window.hpp>

namespace rb {

/// @brief Provides event callbacks to manage the aspect ratio of a camera as 
/// the framebuffer size changes This script must be attached to an object 
/// which has a camera component, otherwise an std::runtime_error will be thrown
/// upon attaching
class CameraAspectRatioManager : public InputProcessor {
private:
    CameraAspectRatioManager(const CameraAspectRatioManager&) = default;
    CameraAspectRatioManager(CameraAspectRatioManager&&) = default;
    CameraAspectRatioManager& operator=(const CameraAspectRatioManager&) = delete;
    CameraAspectRatioManager& operator=(CameraAspectRatioManager&&) = delete;

    /// @brief Reference to the camera controlled by the script
    Camera& _camera;
    
public:
    /// @param camera Reference to the camera whose aspect ratio to manage
    CameraAspectRatioManager(Camera& camera);

    //////////////////////////////////////////////
    ///                                        ///
    /// Methods overridden from InputProcessor ///
    ///                                        ///
    //////////////////////////////////////////////

    /// @brief Callback for a framebuffer resize event
    ///
    /// @param window Reference to the GLWindow in which the event was
    /// triggered
    /// @param width New width (in pixels) of the framebuffer
    /// @param height New height (in pixels) of the framebuffer
    void processFramebufferResize(GLWindow& window, const unsigned int width, const unsigned int height) override;
};

} // namespace rb

#endif//RENDERBOI_TOOLBOX_RUNNABLES_CAMERA_ASPECT_RATIO_MANAGER_HPP
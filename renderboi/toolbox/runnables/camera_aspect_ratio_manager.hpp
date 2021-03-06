#ifndef RENDERBOI__TOOLBOX__RUNNABLES__CAMERA_ASPECT_RATIO_MANAGER_HPP
#define RENDERBOI__TOOLBOX__RUNNABLES__CAMERA_ASPECT_RATIO_MANAGER_HPP

#include <renderboi/core/camera.hpp>

#include <renderboi/window/input_processor.hpp>
#include <renderboi/window/gl_window.hpp>

namespace Renderboi
{

/// @brief Provides event callbacks to manage the aspect ratio of a camera as 
/// the framebuffer size changes. This script must be attached to an object 
/// which has a camera component, otherwise an std::runtime_error will be thrown
/// upon attaching.
class CameraAspectRatioManager : public InputProcessor
{
private:
    CameraAspectRatioManager(const CameraAspectRatioManager& other) = delete;
    CameraAspectRatioManager& operator=(const CameraAspectRatioManager& other) = delete;

    /// @brief Pointer to the camera controlled by the script.
    const CameraPtr _camera;
    
public:
    /// @param camera Pointer to the camera whose aspect ratio to manage.
    CameraAspectRatioManager(const CameraPtr camera);

    //////////////////////////////////////////////
    ///                                        ///
    /// Methods overridden from InputProcessor ///
    ///                                        ///
    //////////////////////////////////////////////

    /// @brief Callback for a framebuffer resize event.
    ///
    /// @param window Pointer to the GLWindow in which the event was
    /// triggered.
    /// @param width New width (in pixels) of the framebuffer.
    /// @param height New height (in pixels) of the framebuffer.
    void processFramebufferResize(const GLWindowPtr window, const unsigned int width, const unsigned int height) override;
};

}//namespace Renderboi

#endif//RENDERBOI__TOOLBOX__RUNNABLES__CAMERA_ASPECT_RATIO_MANAGER_HPP
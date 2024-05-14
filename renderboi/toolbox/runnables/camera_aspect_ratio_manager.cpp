#include "camera_aspect_ratio_manager.hpp"

namespace rb {

CameraAspectRatioManager::CameraAspectRatioManager(Camera& camera) :
    _camera(camera)
{

}

void CameraAspectRatioManager::processFramebufferResize(GLWindow& window, const unsigned int width, const unsigned int height) {
    if (height != 0.f) {
        _camera.aspectRatio = (float)width / (float)height;
    }
}

} // namespace rb

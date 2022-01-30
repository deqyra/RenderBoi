#include "camera_aspect_ratio_manager.hpp"

namespace renderboi
{

CameraAspectRatioManager::CameraAspectRatioManager(Camera& camera) :
    _camera(camera)
{

}

void CameraAspectRatioManager::processFramebufferResize(GLWindow& window, const unsigned int width, const unsigned int height)
{
    if (height != 0.f)
    {
        _camera.setAspectRatio((float)width / (float)height);
    }
}

} // namespace renderboi

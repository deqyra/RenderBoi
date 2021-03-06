#include "camera_aspect_ratio_manager.hpp"

namespace Renderboi
{

CameraAspectRatioManager::CameraAspectRatioManager(const CameraPtr camera) :
    _camera(camera)
{

}

void CameraAspectRatioManager::processFramebufferResize(const GLWindowPtr window, const unsigned int width, const unsigned int height)
{
    if (height != 0.f)
    {
        _camera->setAspectRatio((float)width / (float)height);
    }
}

}//namespace Renderboi

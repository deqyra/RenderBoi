#include "glfw3_monitor.hpp"

#include "GLFW/glfw3.h"

#include <cstring>
#include <vector>

namespace rb::Window {

GLFW3Monitor::GLFW3Monitor(GLFWmonitor* monitor) :
    Monitor(glfwGetMonitorName(monitor)),
    _m(monitor),
    _supportedVideoModes(_ListVideoModes(monitor)),
    _currentVideoMode(),
    _largestVideoMode(_LargestVideoMode(_supportedVideoModes))
{
    glfwSetMonitorUserPointer(_m, (void*)this);
}

GLFW3Monitor::~GLFW3Monitor() {
    glfwSetMonitorUserPointer(_m, nullptr);
}

GLFWmonitor* GLFW3Monitor::getGlfwMonitorPointer() const {
    return _m;
}

const Monitor::VideoMode GLFW3Monitor::getCurrentVideoMode() const {
    const GLFWvidmode* mode = glfwGetVideoMode(_m);

    return Monitor::VideoMode{
        mode->width,
        mode->height,
        mode->redBits,
        mode->greenBits,
        mode->blueBits,
        mode->refreshRate
    };}

const std::vector<Monitor::VideoMode>& GLFW3Monitor::getVideoModes() const {
    return _supportedVideoModes;
}

void GLFW3Monitor::getPhysicalSize(int& width_mm, int& height_mm) const {
    glfwGetMonitorPhysicalSize(_m, &width_mm, &height_mm);
}

void GLFW3Monitor::getContentScale(float& xscale, float& yscale) const {
    glfwGetMonitorContentScale(_m, &xscale, &yscale);
}

void GLFW3Monitor::getPosition(int& xpos, int& ypos) const {
    glfwGetMonitorPos(_m, &xpos, &ypos);
}

void GLFW3Monitor::getWorkArea(int& xpos, int& ypos, int& width, int& height) const {
    glfwGetMonitorWorkarea(_m, &xpos, &ypos, &width, &height);
}

Monitor::GammaRamp GLFW3Monitor::getGammaRamp() const {
    const GLFWgammaramp* ramp = glfwGetGammaRamp(_m);

    std::vector<unsigned short> red = std::vector<unsigned short>(ramp->size);
    std::vector<unsigned short> green = std::vector<unsigned short>(ramp->size);
    std::vector<unsigned short> blue = std::vector<unsigned short>(ramp->size);

    std::memcpy(red.data(), ramp->red, ramp->size);
    std::memcpy(green.data(), ramp->green, ramp->size);
    std::memcpy(blue.data(), ramp->blue, ramp->size);

    return GammaRamp(ramp->size, red, green, blue);
}

void GLFW3Monitor::setGammaRamp(const GammaRamp& gammaRamp) const {
    GLFWgammaramp ramp;
    ramp.size = gammaRamp.getSize();

    // const_cast to avoid a useless copy and because GLFW guarantees not to 
    // modify the values.
    ramp.red = const_cast<unsigned short*>(gammaRamp.getRed().data());
    ramp.green = const_cast<unsigned short*>(gammaRamp.getGreen().data());
    ramp.blue = const_cast<unsigned short*>(gammaRamp.getBlue().data());

    glfwSetGammaRamp(_m, &ramp);
}

Monitor::VideoMode GLFW3Monitor::getLargestVideoMode() const {
    return _largestVideoMode;
}

std::vector<Monitor::VideoMode> GLFW3Monitor::_ListVideoModes(GLFWmonitor* m)
{
    int count;
    const GLFWvidmode* modes = glfwGetVideoModes(m, &count);

    std::vector<VideoMode> videoModes = std::vector<VideoMode>(count);
    for (int i = 0; i < count; i++)
    {
        videoModes.push_back(VideoMode{
            modes[i].width,
            modes[i].height,
            modes[i].redBits,
            modes[i].greenBits,
            modes[i].blueBits,
            modes[i].refreshRate
        });
    }

    return videoModes;
}

GLFW3Monitor::VideoMode GLFW3Monitor::_LargestVideoMode(const std::vector<VideoMode>& modes) {
    int maxWidth = 0;
    int maxHeight = 0;
    int maxRefreshRate = 0;
    int maxColorBits[3] = {0, 0, 0};    
    for (const VideoMode& mode : modes)
    {
        if (mode.width > maxWidth)
            maxWidth = mode.width;
        if (mode.height > maxHeight)
            maxHeight = mode.height;
        if (mode.refreshRate > maxRefreshRate)
            maxRefreshRate = mode.refreshRate;
        if (mode.redBits > maxColorBits[0])
            maxColorBits[0] = mode.redBits;
        if (mode.greenBits > maxColorBits[1])
            maxColorBits[1] = mode.greenBits;
        if (mode.blueBits > maxColorBits[2])
            maxColorBits[2] = mode.blueBits;
    }

    Monitor::VideoMode v = {
        .width = maxWidth,
        .height = maxHeight,
        .redBits = maxColorBits[0],
        .greenBits = maxColorBits[1],
        .blueBits = maxColorBits[2],
        .refreshRate = maxRefreshRate
    };

    return v;
}

} // namespace rb::Window
#ifndef RENDERBOI__WINDOW__EVENT__GL_CONTEXT_EVENT_HPP
#define RENDERBOI__WINDOW__EVENT__GL_CONTEXT_EVENT_HPP

#include <string>

namespace renderboi
{

namespace Window
{

/// @brief Collection of literals describing different render events that can
/// take place in the event polling thread.
enum class GLContextEvent
{
    FitFramebufferToWindow,
    PolygonModeFill,
    PolygonModeLine,
    PolygonModePoint
};

} // namespace Window

std::string to_string(const Window::GLContextEvent event);

} // namespace renderboi

#endif//RENDERBOI__WINDOW__EVENT__GL_CONTEXT_EVENT_HPP
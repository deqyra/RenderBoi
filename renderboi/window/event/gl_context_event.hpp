#ifndef RENDERBOI_WINDOW_EVENT_GL_CONTEXT_EVENT_HPP
#define RENDERBOI_WINDOW_EVENT_GL_CONTEXT_EVENT_HPP

#include <string>

namespace rb {

namespace Window {

/// @brief Literals describing different render events that can
/// take place in the event polling thread
enum class GLContextEvent {
    FitFramebufferToWindow,
    PolygonModeFill,
    PolygonModeLine,
    PolygonModePoint
};

} // namespace Window

std::string to_string(const Window::GLContextEvent event);

} // namespace rb

#endif//RENDERBOI_WINDOW_EVENT_GL_CONTEXT_EVENT_HPP
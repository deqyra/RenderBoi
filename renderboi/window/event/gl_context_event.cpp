#include "gl_context_event.hpp"

#include <unordered_map>

namespace rb {

using Window::GLContextEvent;

std::string to_string(const GLContextEvent event) {
    static std::unordered_map<GLContextEvent, std::string> enumNames = {
        {GLContextEvent::FitFramebufferToWindow,    "FitFramebufferToWindow"},
        {GLContextEvent::PolygonModeFill,           "PolygonModeFill"},
        {GLContextEvent::PolygonModeLine,           "PolygonModeLine"},
        {GLContextEvent::PolygonModePoint,          "PolygonModePoint"},
    };

    auto it = enumNames.find(event);
    return (it != enumNames.end())
        ? it->second
        : "Unknown";
}

} // namespace rb

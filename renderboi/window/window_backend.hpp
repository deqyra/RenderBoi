#ifndef RENDERBOI_WINDOW_WINDOW_BACKEND_HPP
#define RENDERBOI_WINDOW_WINDOW_BACKEND_HPP

namespace rb::Window {

/// @brief Literals describing the available window backends
enum class WindowBackend {
    Unknown,
    GLFW3
};

} // namespace rb::Window

#endif//RENDERBOI_WINDOW_WINDOW_BACKEND_HPP
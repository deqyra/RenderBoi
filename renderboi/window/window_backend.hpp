#ifndef RENDERBOI__WINDOW__WINDOW_BACKEND_HPP
#define RENDERBOI__WINDOW__WINDOW_BACKEND_HPP

namespace renderboi::Window
{

/// @brief Collection of literals describing the available window backends.
enum class WindowBackend
{
    Unknown,
    GLFW3
};

} // namespace renderboi::Window

#endif//RENDERBOI__WINDOW__WINDOW_BACKEND_HPP
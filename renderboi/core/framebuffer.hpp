#ifndef RENDERBOI__CORE__FRAMEBUFFER_HPP
#define RENDERBOI__CORE__FRAMEBUFFER_HPP

#include <unordered_map>

namespace renderboi
{

/// @brief Handler for a framebuffer resource on the GPU.
class Framebuffer
{
public:
    /// @brief Collection of literals describing the available targets to bind
    /// framebuffers to.
    enum class Mode
    {
        ReadWrite,
        Read,
        Write
    };

private:
    /// @brief The location of the framebuffer resource on the GPU.
    unsigned int _location;

    /// @brief Map keeping track of the framebuffer locations currently being 
    /// bound to the GL context.
    static const std::unordered_map<Mode, unsigned int> _targetMap;

public:
    Framebuffer();
    ~Framebuffer();

    /// @brief Make this framebuffer the target for all subsequent read and/or
    /// write operations, as specified by [mode].
    /// 
    /// @param mode Literal describing which target to bind the framebuffer to.
    void bind(Mode mode = Mode::ReadWrite);

    /// @brief Restore the default framebuffer as the target for all subsequent
    /// read and/or write operations, as specified by [mode].
    /// 
    /// @param mode Literal describing which target to bind the default
    /// framebuffer to.
    static void Unbind(Mode mode = Mode::ReadWrite);
};

} // namespace renderboi

#endif//RENDERBOI__CORE__FRAMEBUFFER_HPP
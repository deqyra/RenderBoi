#include "framebuffer.hpp"

#include <glad/gl.h>

namespace renderboi
{

const std::unordered_map<Framebuffer::Mode, unsigned int> Framebuffer::_targetMap = {
    {Mode::ReadWrite,   GL_FRAMEBUFFER},
    {Mode::Read,        GL_READ_FRAMEBUFFER},
    {Mode::Write,       GL_DRAW_FRAMEBUFFER}
};

Framebuffer::Framebuffer() :
    _location(0)
{
    glGenFramebuffers(1, &_location);
}

Framebuffer::~Framebuffer()
{
    glDeleteFramebuffers(1, &_location);
}

void Framebuffer::bind(Framebuffer::Mode mode)
{
    glBindFramebuffer(_targetMap.at(mode), _location);
}

void Framebuffer::Unbind(Framebuffer::Mode mode)
{
    glBindFramebuffer(_targetMap.at(mode), 0);
}

} // namespace renderboi
#include "texture_2d.hpp"

#include <stdexcept>

#include <glad/gl.h>

#include <stb/stb_image.h>


#include <renderboi/utilities/resource_locator.hpp>

#include "pixel_space.hpp"

namespace {

using ReLoc  = rb::ResourceLocator;
using ReType = rb::ResourceType;

unsigned int loadAndSendTexture(const std::string& filename, const rb::PixelSpace space) {
    // Load the image from disk
    int width, height, nChannels;
    unsigned char *data = stbi_load(filename.c_str(), &width, &height, &nChannels, 0);

    if (data)
    {
        GLenum format = GL_RGB;
        if (nChannels == 1)
            format = GL_RED;
        else if (nChannels == 4)
            format = GL_RGBA;

        GLenum internalFormat = GL_RGB;
        if (space == rb::PixelSpace::sRGB)
        {
            internalFormat = GL_SRGB;
            if (format == GL_RGBA)
                internalFormat = GL_SRGB_ALPHA;
        }

        // Create a texture resource on the GPU
        unsigned int location;
        glGenTextures(1, &location);

        // Send the texture to the GPU
        glBindTexture(GL_TEXTURE_2D, location);
        glTexImage2D(GL_TEXTURE_2D, 0, internalFormat, width, height, 0, format, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);

        // Set texture wrapping and filtering options
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        stbi_image_free(data);
        return location;
    }

    std::string s = "Texture2D: failed to load image located at \"" + filename + "\".";
    throw std::runtime_error(s.c_str());
}

}

namespace rb {

std::unordered_map<unsigned int, unsigned int> Texture2D::_locationRefCounts = std::unordered_map<unsigned int, unsigned int>();
std::unordered_map<std::string , unsigned int> Texture2D::_objectLocations  = std::unordered_map<std::string, unsigned int>();

Texture2D::Texture2D(const std::string& filename, const PixelSpace space)  :
    _tex()
{
    _tex = _getOrCreateTextureLocation(filename, space);
    ++_locationRefCounts[_tex];
}

Texture2D::Texture2D(const Texture2D& other) :
    _tex(other._tex)
{
    _locationRefCounts[_tex]++;
}

Texture2D& Texture2D::operator=(const Texture2D& other) {
    _cleanup();

    _tex = other._tex;
    _locationRefCounts[_tex]++;

    return *this;
}

Texture2D::~Texture2D() {
    _cleanup();
}

void Texture2D::_cleanup() {
    unsigned int count = --_locationRefCounts[_tex];
    if (!count)
    {
        glDeleteTextures(1, &_tex);
    };
}

unsigned int Texture2D::_getOrCreateTextureLocation(const std::string& filename, const PixelSpace space) {
    auto it = _objectLocations.find(filename);

    if (it == _objectLocations.end() || _locationRefCounts.find(it->second)->second == 0) {
        return _objectLocations[filename] = loadAndSendTexture(filename, space);
    } else {
        return it->second;
    }
}

unsigned int Texture2D::location() const
{
    return _tex;
}

void Texture2D::bind() const
{
    glBindTexture(GL_TEXTURE_2D, _tex);
}

void Texture2D::bind(unsigned int unit) const
{
    // static constexpr std::size_t MaxTextureUnit = GL_TEXTURE31;
    // static constexpr std::size_t MaxTextureUnitIndex = MaxTextureUnit - GL_TEXTURE0;

    unsigned int realUnit = GL_TEXTURE0 + unit;

    glActiveTexture(realUnit);
    glBindTexture(GL_TEXTURE_2D, _tex);
}

} // namespace rb

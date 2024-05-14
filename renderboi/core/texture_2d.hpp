#ifndef RENDERBOI_CORE_TEXTURE_2D_HPP
#define RENDERBOI_CORE_TEXTURE_2D_HPP

#include <string>
#include <unordered_map>

#include "pixel_space.hpp"

namespace rb {

/// @brief A 2D texture resource on the GPU
class Texture2D
{
private:
    /// @brief GPU texture locations mapped to the path of the image they were constructed from
    static std::unordered_map<std::string , unsigned int> _objectLocations;

    /// @brief Reference count mapped to a texture object location on the GPU
    static std::unordered_map<unsigned int, unsigned int> _locationRefCounts;

    /// @brief The location of the texture resource on the GPU
    unsigned int _tex;
    
    /// @brief Given an image file, make a texture out of its contents and send
    /// it to the GPU, or return the existing texture location for that image
    /// @param path Path to the image file
    /// @return The texture object location on the GPU
    static unsigned int _getOrCreateTextureLocation(const std::string& filename, const PixelSpace space);

    /// @brief Free resources upon instance destruction
    void _cleanup();

public:
    /// @param filename Local path to an image file out of which the texture
    /// should be generated
    /// @param pixelSpace Literal describing which space the texture pixels
    /// are in
    Texture2D(const std::string& filename, const PixelSpace space);

    Texture2D(const Texture2D& other);

    ~Texture2D();

    Texture2D& operator=(const Texture2D& other);
    
    /// @brief Get the location of the texture on the GPU
    ///
    /// @return The location of the texture on the GPU
    unsigned int location() const;

    /// @brief Bind the texture to the current texture unit on the GPU
    void bind() const;

    /// @brief Bind the texture to a particular texture unit on the GPU
    /// @param unit 0-based index of the unit to which the texture should
    /// be bound
    void bind(unsigned int unit) const;
};

} // namespace rb

#endif//RENDERBOI_CORE_TEXTURE_2D_HPP
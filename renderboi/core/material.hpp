#ifndef RENDERBOI_CORE_MATERIAL_HPP
#define RENDERBOI_CORE_MATERIAL_HPP

#include <vector>

#include <renderboi/core/numeric.hpp>
#include <renderboi/core/texture_2d.hpp>

namespace rb {

/// @brief Material with ambient, diffuse and specular components, as well as 
/// diffuse and specular maps
struct Material {
    /// @brief The fixed ambient reflection component of the material
    num::Vec3 ambient;

    /// @brief The fixed diffuse reflection component of the material
    num::Vec3 diffuse;

    /// @brief The fixed specular reflection component of the material
    num::Vec3 specular;

    /// @brief The shininess (specular strength) of the material
    float shininess;

    /// @brief Array of textures for the diffuse reflection of the material
    std::vector<Texture2D> diffuseMaps;

    /// @brief Array of textures for the specular reflection of the material
    std::vector<Texture2D> specularMaps;

    /// @brief The maximum amount of diffuse maps the material can have
    static constexpr std::size_t DiffuseMapMaxCount = 8;

    /// @brief The maximum amount of specular maps the material can have
    static constexpr std::size_t SpecularMapMaxCount = 8;
};

/// @brief Bind all textures registered in the material to a texture channel on the GPU
void bindTextures(const Material& mat);

} // namespace rb

#endif//RENDERBOI_CORE_MATERIAL_HPP
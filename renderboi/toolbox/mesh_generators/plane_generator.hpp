#ifndef RENDERBOI_TOOLBOX_MESH_GENERATORS_PLANE_GENERATOR_HPP
#define RENDERBOI_TOOLBOX_MESH_GENERATORS_PLANE_GENERATOR_HPP

#include <renderboi/core/numeric.hpp>
#include <renderboi/core/3d/mesh.hpp>
#include <renderboi/core/color.hpp>

#include "mesh_generator.hpp"

namespace rb {

/// @brief Generates vertices for a tiled plane
class PlaneGenerator {
public:
    /// @brief Struct packing together the parameters of the vertex
    /// generation
    struct Parameters {
        /// @brief Size the tiles of the plane will have along the X and Y directions
        num::Vec2 tileSize = { 1.f, 1.f };

        /// @brief Amount of tiles in the plane along the X and Y directions
        num::Vec2ui tileAmount = { 1, 1 };

        /// @brief Size of the texture along the X direction Affects the
        /// generated texture coordinates The bigger the provided texture
        /// size, the smaller the texture coordinates will be Specifying 0 
        /// or a negative value will cause the texture coordinates to be 
        /// spread evenly from 0 to 1 on the plane
        num::Vec2 texSize = { 0.f, 0.f };

        /// @brief Offset by which to shift texture coordinates
        num::Vec2 texOffset = { 0.f, 0.f };

        /// @brief Whether or not to invert the tex coords along the X and Y axes
        num::Vec2b invertTexCoords = { false, false };
        
        /// @brief Angle (in radians) by which to rotate the texture after the
        /// reversal of coordinates if specified
        float texRotation = 0.f;

        /// @brief RGB color of the generated vertices
        num::Vec3 color = color::White;
    };

    PlaneGenerator() = default;
    PlaneGenerator(const Parameters& parameters);
    
    /// @brief Parameters of the vertex generation
    Parameters parameters;

    /// @brief Generate the vertex data, put it in a new mesh object and 
    /// return it
    /// @return A pointer to the mesh containing the generated vertices
    std::unique_ptr<Mesh> generate() const;
};

static_assert(MeshGenerator<PlaneGenerator>);

} // namespace rb

#endif//RENDERBOI_TOOLBOX_MESH_GENERATORS_PLANE_GENERATOR_HPP

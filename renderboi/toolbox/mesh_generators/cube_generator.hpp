#ifndef RENDERBOI_TOOLBOX_MESH_GENERATORS_CUBE_GENERATOR_HPP
#define RENDERBOI_TOOLBOX_MESH_GENERATORS_CUBE_GENERATOR_HPP

#include <renderboi/core/numeric.hpp>
#include <renderboi/core/color.hpp>
#include <renderboi/core/3d/mesh.hpp>

#include "mesh_generator.hpp"

#include <optional>

namespace rb {

/// @brief Generates vertices for a multi- or single-colored cube
class CubeGenerator {
public:
    /// @brief Struct packing together the parameters of the vertex generation
    struct Parameters {
        /// @brief Size the cube will have (radius of encompassing sphere)
        float size = 1.f;

        /// @brief RGB color of the generated vertices
        std::optional<num::Vec3> color = std::nullopt;
    };

    CubeGenerator() = default;
    CubeGenerator(const Parameters& parameters);
    
    /// @brief Parameters of the vertex generation
    Parameters parameters;

    /// @brief Generate the vertex data, put it in a new mesh object and return it
    /// @return A pointer to the mesh containing the generated vertices
    std::unique_ptr<Mesh> generate() const;
};

static_assert(MeshGenerator<CubeGenerator>);

} // namespace rb

#endif//RENDERBOI_TOOLBOX_MESH_GENERATORS_CUBE_GENERATOR_HPP

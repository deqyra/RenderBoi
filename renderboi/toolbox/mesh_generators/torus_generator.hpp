#ifndef RENDERBOI_TOOLBOX_MESH_GENERATORS_TORUS_GENERATOR_HPP
#define RENDERBOI_TOOLBOX_MESH_GENERATORS_TORUS_GENERATOR_HPP

#include <renderboi/core/3d/mesh.hpp>

#include "mesh_generator.hpp"

namespace rb {

/// @brief Generates the vertex data for a torus
class TorusGenerator {
public:
    /// @brief Struct packing together the parameters of the vertex generation
    struct Parameters {
        /// @brief Toroidal / "large" radius of the torus
        float toroidalRadius = 2.f;

        /// @brief Poloidal / "small" radius of the torus
        float poloidalRadius = 0.5f;

        /// @brief How many vertices to use along the toroidal circumference of the torus
        unsigned int toroidalVertexRes = 36;

        /// @brief How many vertices to use along the poloidal circumference of the torus
        unsigned int poloidalVertexRes = 12;
    };

    TorusGenerator() = default;
    TorusGenerator(const Parameters& parameters);

    /// @brief Parameters of the vertex generation
    Parameters parameters;

    /// @brief Generate the vertex data, put it in a new mesh object and return it
    /// @return A pointer to the mesh containing the generated vertices
    std::unique_ptr<Mesh> generate() const;
};

static_assert(MeshGenerator<TorusGenerator>);

} // namespace rb

#endif//RENDERBOI_TOOLBOX_MESH_GENERATORS_TORUS_GENERATOR_HPP

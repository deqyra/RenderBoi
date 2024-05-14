#ifndef RENDERBOI_TOOLBOX_MESH_GENERATORS_AXES_GENERATOR_HPP
#define RENDERBOI_TOOLBOX_MESH_GENERATORS_AXES_GENERATOR_HPP

#include <renderboi/core/3d/mesh.hpp>

#include "mesh_generator.hpp"

namespace rb {

/// @brief Generates vertices for three X, Y and Z axes, colored red, green and blue
class AxesGenerator {
public:
    /// @brief Struct packing together the parameters of the vertex generation
    struct Parameters {
        /// @brief Length the axes will have
        float axisLength = 1.f;
    };

    AxesGenerator() = default;
    AxesGenerator(const Parameters& parameters);
    
    /// @brief Parameters of the vertex generation
    Parameters parameters;

    /// @brief Generate the vertex data, put it in a new mesh object and 
    /// return it
    /// @return A pointer to the mesh containing the generated vertices
    std::unique_ptr<Mesh> generate() const;
};

static_assert(MeshGenerator<AxesGenerator>);

} // namespace rb

#endif//RENDERBOI_TOOLBOX_MESH_GENERATORS_AXES_GENERATOR_HPP

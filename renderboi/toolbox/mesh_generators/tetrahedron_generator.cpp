#include "tetrahedron_generator.hpp"

#include <memory>
#include <vector>

#include <renderboi/core/numeric.hpp>
#include <renderboi/core/3d/mesh.hpp>
#include <renderboi/core/3d/vertex.hpp>

#include <glad/gl.h>

namespace rb {

TetrahedronGenerator::TetrahedronGenerator(const Parameters& parameters) :
    parameters(parameters)
{
    
}

std::unique_ptr<Mesh> TetrahedronGenerator::generate() const {
    num::Vec3 top           = num::Vec3( 0.f,                0.75f * num::Sqrt2,   0.f) * parameters.size;
    num::Vec3 baseBackLeft  = num::Vec3(-0.5f * num::Sqrt3, -0.25f * num::Sqrt2, -0.5f) * parameters.size;
    num::Vec3 baseBackRight = num::Vec3( 0.5f * num::Sqrt3, -0.25f * num::Sqrt2, -0.5f) * parameters.size;
    num::Vec3 baseFront     = num::Vec3( 0.f,               -0.25f * num::Sqrt2,   1.f) * parameters.size;

    const auto& c = parameters.color;

    std::vector<Vertex> vertices = {
        // Position         // Color                    // Normal       // Tex coord
        { baseBackLeft,     c.value_or(color::Red),     -top,           num::Origin2 },     // Face 1
        { baseBackRight,    c.value_or(color::Green),   -top,           num::Origin2 },     // -Y
        { baseFront,        c.value_or(color::Blue),    -top,           num::Origin2 },

        { baseBackRight,    c.value_or(color::Green),   -baseFront,     num::Origin2 },     // Face 2
        { baseBackLeft,     c.value_or(color::Red),     -baseFront,     num::Origin2 },     // -Z
        { top,              c.value_or(color::White),   -baseFront,     num::Origin2 },

        { top,              c.value_or(color::White),   -baseBackRight, num::Origin2 },     // Face 3
        { baseBackLeft,     c.value_or(color::Red),     -baseBackRight, num::Origin2 },     // -X
        { baseFront,        c.value_or(color::Blue),    -baseBackRight, num::Origin2 },

        { baseFront,        c.value_or(color::Blue),    -baseBackLeft,  num::Origin2 },     // Face 4
        { baseBackRight,    c.value_or(color::Green),   -baseBackLeft,  num::Origin2 },     //  X
        { top,              c.value_or(color::White),   -baseBackLeft,  num::Origin2 }
    };

    std::vector<unsigned int> indices = {
        0,  1,  2,
        3,  4,  5,
        6,  7,  8,
        9, 10, 11
    };

    return std::make_unique<Mesh>(GL_TRIANGLES, vertices, indices);
}

} // namespace rb

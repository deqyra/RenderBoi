#include "cube_generator.hpp"

#include <vector>
#include <memory>

#include <renderboi/core/numeric.hpp>

#include <renderboi/core/3d/mesh.hpp>
#include <renderboi/core/3d/vertex.hpp>

#include <glad/gl.h>

namespace rb {

CubeGenerator::CubeGenerator(const Parameters& parameters) :
    parameters(parameters)
{
    
}

std::unique_ptr<Mesh> CubeGenerator::generate() const {
    float s = num::sqrt(2 * parameters.size * parameters.size);
    const auto& c = parameters.color;

    std::vector<Vertex> vertices = {
        // Position                   // Color                    // Normal   // Tex coord
        {  num::Vec3( s,  s,  s),     c.value_or(color::White),    num::X,    num::Vec2(0.f, 1.f) },  // Face 1
        {  num::Vec3( s,  s, -s),     c.value_or(color::Red),      num::X,    num::Vec2(1.f, 1.f) },  // +X
        {  num::Vec3( s, -s,  s),     c.value_or(color::Green),    num::X,    num::Vec2(0.f, 0.f) },
        {  num::Vec3( s, -s, -s),     c.value_or(color::Yellow),   num::X,    num::Vec2(1.f, 0.f) },

        {  num::Vec3( s, -s,  s),     c.value_or(color::Green),   -num::Y,    num::Vec2(1.f, 1.f) },  // Face 2
        {  num::Vec3( s, -s, -s),     c.value_or(color::Yellow),  -num::Y,    num::Vec2(1.f, 0.f) },  // -Y
        {  num::Vec3(-s, -s,  s),     c.value_or(color::Cyan),    -num::Y,    num::Vec2(0.f, 1.f) },
        {  num::Vec3(-s, -s, -s),     c.value_or(color::Black),   -num::Y,    num::Vec2(0.f, 0.f) },

        {  num::Vec3(-s, -s,  s),     c.value_or(color::Cyan),    -num::X,    num::Vec2(1.f, 0.f) },  // Face 3
        {  num::Vec3(-s, -s, -s),     c.value_or(color::Black),   -num::X,    num::Vec2(0.f, 0.f) },  // -X
        {  num::Vec3(-s,  s,  s),     c.value_or(color::Blue),    -num::X,    num::Vec2(1.f, 1.f) },
        {  num::Vec3(-s,  s, -s),     c.value_or(color::Magenta), -num::X,    num::Vec2(0.f, 1.f) },

        {  num::Vec3(-s,  s,  s),     c.value_or(color::Blue),     num::Y,    num::Vec2(1.f, 0.f) },  // Face 4
        {  num::Vec3(-s,  s, -s),     c.value_or(color::Magenta),  num::Y,    num::Vec2(1.f, 1.f) },  // +Y
        {  num::Vec3( s,  s,  s),     c.value_or(color::White),    num::Y,    num::Vec2(1.f, 1.f) },
        {  num::Vec3( s,  s, -s),     c.value_or(color::Red),      num::Y,    num::Vec2(0.f, 1.f) },

        {  num::Vec3(-s,  s, -s),     c.value_or(color::Magenta), -num::Z,    num::Vec2(1.f, 1.f) },  // Face 5
        {  num::Vec3(-s, -s, -s),     c.value_or(color::Black),   -num::Z,    num::Vec2(1.f, 0.f) },  // -Z
        {  num::Vec3( s,  s, -s),     c.value_or(color::Red),     -num::Z,    num::Vec2(0.f, 1.f) },
        {  num::Vec3( s, -s, -s),     c.value_or(color::Yellow),  -num::Z,    num::Vec2(0.f, 0.f) },
        
        {  num::Vec3(-s, -s,  s),     c.value_or(color::Cyan),     num::Z,    num::Vec2(0.f, 0.f) },  // Face 6
        {  num::Vec3(-s,  s,  s),     c.value_or(color::Blue),     num::Z,    num::Vec2(0.f, 1.f) },  // +Z
        {  num::Vec3( s, -s,  s),     c.value_or(color::Green),    num::Z,    num::Vec2(1.f, 0.f) },
        {  num::Vec3( s,  s,  s),     c.value_or(color::White),    num::Z,    num::Vec2(1.f, 1.f) }
    };

    std::vector<unsigned int> indices = {
         0,  1,  2,  3, // One strip of 2 triangles per face
         4,  5,  6,  7,
         8,  9, 10, 11,
        12, 13, 14, 15,
        16, 17, 18, 19,
        20, 21, 22, 23
    };

    std::vector<unsigned int> primitiveSizes(6, 4);
    std::vector<void*> primitiveOffsets(6);
    for (unsigned int i = 0; i < 6; i++) {
        primitiveOffsets[i] = reinterpret_cast<void*>(i * 4 * sizeof(unsigned int));
    }

    return std::make_unique<Mesh>(
        GL_TRIANGLE_STRIP, 
        std::move(vertices), 
        std::move(indices), 
        std::move(primitiveSizes), 
        std::move(primitiveOffsets)
    );
}

} // namespace rb

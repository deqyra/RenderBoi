#include "tetrahedron_generator.hpp"

#include <vector>
#include <memory>
#include <glm/glm.hpp>

#include <renderboi/core/mesh.hpp>
#include <renderboi/core/vertex.hpp>

#include "../common.hpp"

namespace renderboi
{

TetrahedronGenerator::TetrahedronGenerator() :
    TetrahedronGenerator(DefaultSize)
{

}

TetrahedronGenerator::TetrahedronGenerator(const float size) :
    parameters{ size, glm::vec3(0.f), false }
{
    
}

TetrahedronGenerator::TetrahedronGenerator(const float size, const glm::vec3 color) :
    parameters{ size, color, true}
{

}

TetrahedronGenerator::TetrahedronGenerator(const Parameters parameters) :
    parameters(parameters)
{
    
}

MeshPtr TetrahedronGenerator::generateMesh() const
{
    using namespace common;

    const unsigned int nVertices = 12;

    glm::vec3 top           = glm::vec3( 0.f,           0.75f * Sqrt2,  0.f)  * parameters.size;
    glm::vec3 baseBackLeft  = glm::vec3(-0.5f * Sqrt3, -0.25f * Sqrt2, -0.5f) * parameters.size;
    glm::vec3 baseBackRight = glm::vec3( 0.5f * Sqrt3, -0.25f * Sqrt2, -0.5f) * parameters.size;
    glm::vec3 baseFront     = glm::vec3( 0.f,          -0.25f * Sqrt2,  1.f)  * parameters.size;

    std::vector<Vertex> vertices = {
        // Position         // Color            // Normal       // Tex coord
        { baseBackLeft,     vec(Red),     -top,           glm::vec2(0.f,  0.f) },     // Vertex 1     // Face 1
        { baseBackRight,    vec(Green),   -top,           glm::vec2(1.f,  0.f) },     // Vertex 2     // -Y
        { baseFront,        vec(Blue),    -top,           glm::vec2(0.5f, 1.f) },     // ...

        { baseBackRight,    vec(Green),   -baseFront,     glm::vec2(0.f,  0.f) },                     // Face 2
        { baseBackLeft,     vec(Red),     -baseFront,     glm::vec2(1.f,  0.f) },                     // -Z
        { top,              vec(White),   -baseFront,     glm::vec2(0.5f, 1.f) },

        { top,              vec(White),   -baseBackRight, glm::vec2(0.5f, 1.f) },                     // Face 3
        { baseBackLeft,     vec(Red),     -baseBackRight, glm::vec2(0.f,  0.f) },                     // -X
        { baseFront,        vec(Blue),    -baseBackRight, glm::vec2(0.f,  1.f) },

        { baseFront,        vec(Blue),    -baseBackLeft,  glm::vec2(0.f,  0.f) },                     // Face 4
        { baseBackRight,    vec(Green),   -baseBackLeft,  glm::vec2(1.f,  0.f) },                     //  X
        { top,              vec(White),   -baseBackLeft,  glm::vec2(0.5f, 1.f) }
    };

    if (parameters.useColor)
    {
        for (int i = 0; i < nVertices; i++)
        {
            vertices[i].color = parameters.color;
        }
    }

    std::vector<unsigned int> indices = {
        0,  1,  2,
        3,  4,  5,
        6,  7,  8,
        9, 10, 11
    };

    return std::make_unique<Mesh>(GL_TRIANGLES, vertices, indices);
}

} // namespace renderboi

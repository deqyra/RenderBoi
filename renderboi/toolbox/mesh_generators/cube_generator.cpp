#include "cube_generator.hpp"

#include <vector>
#include <memory>

#include <glm/glm.hpp>

#include <renderboi/core/mesh.hpp>
#include <renderboi/core/vertex.hpp>

#include "../common.hpp"

namespace renderboi
{

CubeGenerator::CubeGenerator() :
    CubeGenerator(DefaultSize)
{

}

CubeGenerator::CubeGenerator(const float size) :
    parameters{ size, glm::vec3(0.f), false }
{
    
}

CubeGenerator::CubeGenerator(const float size, const glm::vec3 color) :
    parameters{ size, color, true}
{

}

CubeGenerator::CubeGenerator(const Parameters parameters) :
    parameters(parameters)
{
    
}

MeshPtr CubeGenerator::generateMesh() const
{
    using namespace common;

    float len = glm::sqrt(2 * parameters.size * parameters.size);

    const unsigned int nVertices = 24;

    std::vector<Vertex> vertices = {
        // Position                         // Color        // Normal       // Tex coord
        {  glm::vec3( len,  len,  len),     vec(White),     vec(x),         glm::vec2(0.f, 1.f) },  // Vertex 1     // Face 1
        {  glm::vec3( len,  len, -len),     vec(Red),       vec(x),         glm::vec2(1.f, 1.f) },  // Vertex 2     // +X
        {  glm::vec3( len, -len,  len),     vec(Green),     vec(x),         glm::vec2(0.f, 0.f) },  // ...
        {  glm::vec3( len, -len, -len),     vec(Yellow),    vec(x),         glm::vec2(1.f, 0.f) },

        {  glm::vec3( len, -len,  len),     vec(Green),     vec(Y(-1.f)),   glm::vec2(1.f, 1.f) },                  // Face 2
        {  glm::vec3( len, -len, -len),     vec(Yellow),    vec(Y(-1.f)),   glm::vec2(1.f, 0.f) },                  // -Y
        {  glm::vec3(-len, -len,  len),     vec(Cyan),      vec(Y(-1.f)),   glm::vec2(0.f, 1.f) },
        {  glm::vec3(-len, -len, -len),     vec(Black),     vec(Y(-1.f)),   glm::vec2(0.f, 0.f) },

        {  glm::vec3(-len, -len,  len),     vec(Cyan),      vec(X(-1.f)),   glm::vec2(1.f, 0.f) },                  // Face 3
        {  glm::vec3(-len, -len, -len),     vec(Black),     vec(X(-1.f)),   glm::vec2(0.f, 0.f) },                  // -X
        {  glm::vec3(-len,  len,  len),     vec(Blue),      vec(X(-1.f)),   glm::vec2(1.f, 1.f) },
        {  glm::vec3(-len,  len, -len),     vec(Magenta),   vec(X(-1.f)),   glm::vec2(0.f, 1.f) },

        {  glm::vec3(-len,  len,  len),     vec(Blue),      vec(y),         glm::vec2(1.f, 0.f) },                  // Face 4
        {  glm::vec3(-len,  len, -len),     vec(Magenta),   vec(y),         glm::vec2(1.f, 1.f) },                  // +Y
        {  glm::vec3( len,  len,  len),     vec(White),     vec(y),         glm::vec2(1.f, 1.f) },
        {  glm::vec3( len,  len, -len),     vec(Red),       vec(y),         glm::vec2(0.f, 1.f) },

        {  glm::vec3(-len,  len, -len),     vec(Magenta),   vec(Z(-1.f)),   glm::vec2(1.f, 1.f) },                  // Face 5
        {  glm::vec3(-len, -len, -len),     vec(Black),     vec(Z(-1.f)),   glm::vec2(1.f, 0.f) },                  // -Z
        {  glm::vec3( len,  len, -len),     vec(Red),       vec(Z(-1.f)),   glm::vec2(0.f, 1.f) },
        {  glm::vec3( len, -len, -len),     vec(Yellow),    vec(Z(-1.f)),   glm::vec2(0.f, 0.f) },

        {  glm::vec3(-len, -len,  len),     vec(Cyan),      vec(z),         glm::vec2(0.f, 0.f) },                  // Face 6
        {  glm::vec3(-len,  len,  len),     vec(Blue),      vec(z),         glm::vec2(0.f, 1.f) },                  // +Z
        {  glm::vec3( len, -len,  len),     vec(Green),     vec(z),         glm::vec2(1.f, 0.f) },
        {  glm::vec3( len,  len,  len),     vec(White),     vec(z),         glm::vec2(1.f, 1.f) }
    };

    if (parameters.useColor)
    {
        for (int i = 0; i < nVertices; i++)
        {
            vertices[i].color = parameters.color;
        }
    }

    std::vector<unsigned int> indices = {
        // Face n strip
         0,  1,  2,  3,
         4,  5,  6,  7,
         8,  9, 10, 11,
        12, 13, 14, 15,
        16, 17, 18, 19,
        20, 21, 22, 23
    };

    std::vector<unsigned int> primitiveSizes(6, 4);
    std::vector<void*> primitiveOffsets(6);
    for (unsigned int i = 0; i < 6; i++)
    {
        primitiveOffsets[i] = (void*)(i * 4 * sizeof(unsigned int));
    }

    return std::make_unique<Mesh>(GL_TRIANGLE_STRIP, vertices, indices, primitiveSizes, primitiveOffsets);
}

} // namespace renderboi

#include "axes_generator.hpp"

#include <vector>
#include <memory>

#include <glm/glm.hpp>

#include <renderboi/core/mesh.hpp>
#include <renderboi/core/vertex.hpp>

#include "../common.hpp"

namespace renderboi
{

AxesGenerator::AxesGenerator() :
    AxesGenerator(DefaultAxisLength)
{

}

AxesGenerator::AxesGenerator(const float length) : 
    parameters{ length }
{

}

AxesGenerator::AxesGenerator(const Parameters parameters) :
    parameters(parameters)
{

}

MeshPtr AxesGenerator::generateMesh() const
{
    using namespace common;

    float len = parameters.axisLength;

    std::vector<Vertex> vertices = {
        // Position      // Color      // Normal      // Tex coord
        {  vec(O3),      vec(Red),     vec(Y(-1.f)),  vec(O2) },   // Vertex 1
        {  vec(X(len)),  vec(Red),     vec(Y(-1.f)),  vec(O2) },   // Vertex 2
        {  vec(O3),      vec(Green),   vec(Z(-1.f)),  vec(O2) },   // ...
        {  vec(Y(len)),  vec(Green),   vec(Z(-1.f)),  vec(O2) },
        {  vec(O3),      vec(Blue),    vec(X(-1.f)),  vec(O2) },
        {  vec(Z(len)),  vec(Blue),    vec(X(-1.f)),  vec(O2) }
    };

    std::vector<unsigned int> indices = {
        0, 1,   // X axis
        2, 3,   // Y axis
        4, 5    // Z axis
    };

    return std::make_unique<Mesh>(GL_LINES, vertices, indices);
}

} // namespace renderboi

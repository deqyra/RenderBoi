#include "torus_generator.hpp"

#include <vector>
#include <memory>

#include <renderboi/core/color.hpp>
#include <renderboi/core/numeric.hpp>
#include <renderboi/core/3d/mesh.hpp>
#include <renderboi/core/3d/vertex.hpp>

#include <glad/gl.h>

namespace rb {

TorusGenerator::TorusGenerator(const Parameters& parameters) :
    parameters(parameters)
{

}

std::unique_ptr<Mesh> TorusGenerator::generate() const
{
    const Parameters& p = parameters;

    unsigned int nVertices = static_cast<unsigned int>(p.toroidalVertexRes * p.poloidalVertexRes);
    std::vector<Vertex> vertices = std::vector<Vertex>(nVertices);

    unsigned int singleStripLength = (p.toroidalVertexRes * 2) + 2;
    unsigned int stripTotalLength = p.poloidalVertexRes * singleStripLength;
    std::vector<unsigned int> indices = std::vector<unsigned int>(stripTotalLength);

    float toroidalAngleStep = (2 * num::Pi) / p.toroidalVertexRes;
    float poloidalAngleStep = (2 * num::Pi) / p.poloidalVertexRes;

    // Generate vertex position, colors and normals
    for (unsigned int i = 0; i < p.poloidalVertexRes; ++i) {
        float pAngle = i * poloidalAngleStep;
        float projectedGap = (1 - num::cos(pAngle)) * p.poloidalRadius;
        float innerPeripheralRadius = p.toroidalRadius - p.poloidalRadius;
        float ringRadius = innerPeripheralRadius + projectedGap;
        float ringHeight = num::sin(pAngle) * p.poloidalRadius;

        for (unsigned int j = 0; j < p.toroidalVertexRes; ++j) {
            float tAngle = j * toroidalAngleStep;

            float vertexX = num::cos(tAngle) * ringRadius;
            float vertexY = ringHeight;
            float vertexZ = sin(tAngle) * ringRadius;

            float normalX = -num::cos(pAngle) * num::cos(tAngle);
            float normalY =  num::sin(pAngle);
            float normalZ = -num::cos(pAngle) * num::sin(tAngle);

            unsigned int index = (i * p.toroidalVertexRes) + j;

            vertices[index].position = num::Vec3(vertexX, vertexY, vertexZ);
            vertices[index].color    = color::White;
            vertices[index].normal   = num::Vec3(normalX, normalY, normalZ);
            vertices[index].texCoord = num::Origin2;
        }
    }

    // Generate index sequences for triangle strips
    unsigned int stripSize = (p.toroidalVertexRes * 2) + 2;
    for (unsigned int i = 0; i < p.poloidalVertexRes; ++i) {
        for (unsigned int j = 0; j < p.toroidalVertexRes; ++j) {
            unsigned int currentVertex = (i * p.toroidalVertexRes) + j;
            unsigned int nextVertex;
            if (i == p.poloidalVertexRes - 1) {
                nextVertex = j;
            } else {
                nextVertex = ((i + 1) * p.toroidalVertexRes) + j;
            }

            unsigned int index  = (i * stripSize) + (j * 2);
            indices[index]      = currentVertex;
            indices[index + 1]  = nextVertex;
        }

        unsigned int currentVertex = i * p.toroidalVertexRes;
        unsigned int nextVertex;
        if (i == p.poloidalVertexRes - 1) {
            nextVertex = 0;
        } else {
            nextVertex = (i + 1) * p.toroidalVertexRes;
        }

        unsigned int index  = (i * stripSize) + (p.toroidalVertexRes * 2);
        indices[index]      = currentVertex;
        indices[index + 1]  = nextVertex;
    }

    return std::make_unique<Mesh>(GL_TRIANGLE_STRIP, vertices, indices);
}

} // namespace rb

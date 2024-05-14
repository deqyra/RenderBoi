#include "plane_generator.hpp"

#include <vector>
#include <memory>

#include <renderboi/core/numeric.hpp>
#include <renderboi/core/3d/mesh.hpp>
#include <renderboi/core/3d/vertex.hpp>

#include <glad/gl.h>

namespace rb {

PlaneGenerator::PlaneGenerator(const Parameters& parameters) :
    parameters(parameters)
{
    if (parameters.texSize.x <= 0.f) {
        this->parameters.texSize.x = parameters.tileAmount.x * parameters.tileSize.x;
    }

    if (parameters.texSize.y <= 0.f) {
        this->parameters.texSize.y = parameters.tileAmount.y * parameters.tileSize.y;
    }
}

std::unique_ptr<Mesh> PlaneGenerator::generate() const
{
    const Parameters& p = parameters;
    const unsigned int nVertices = (p.tileAmount.x + 1) * (p.tileAmount.y + 1);

    std::vector<Vertex> vertices;
    vertices.reserve(nVertices);

    for (unsigned int j = 0; j < p.tileAmount.y + 1; ++j) {
        float yPos = j * p.tileSize.y;

        float baseTexY = yPos / p.texSize.y;
        if (p.invertTexCoords.y) {
            baseTexY = p.texSize.y - baseTexY;
        }

        for (unsigned int i = 0; i < p.tileAmount.x + 1; ++i) {
            float xPos = i * p.tileSize.x;

            float baseTexX = xPos / p.texSize.x;
            if (p.invertTexCoords.x) {
                baseTexX = p.texSize.x - baseTexX;
            }

            // Do a complex rotation to find the rotated texture coordinates.
            // The opposite rotation is actually calculated, because the current 
            // vertex coordinates are not being rotated. What happens then is 
            // that the texture coordinates are moving in reverse relative to 
            // the vertex coordinates.

            // Complex coefficients
            float rotRe = num::cos(-p.texRotation);
            float rotIm = num::sin(-p.texRotation);

            // Actual rotation
            float rotatedTexX = (baseTexX * rotRe) - (baseTexY * rotIm);
            float rotatedTexY = (baseTexX * rotIm) + (baseTexY * rotRe);

            // Add offset
            float fullTexX = rotatedTexX - p.texOffset.x;
            float fullTexY = rotatedTexY - p.texOffset.y;

            vertices.push_back({
                {xPos, yPos, 0.f},      // Position
                p.color,                // Color
                {0.f, 0.f, 1.f},        // Normal
                {fullTexX, fullTexY}    // Tex coord
            });
        } 
    }

    const unsigned int nIndices = ((p.tileAmount.x + 1) * (p.tileAmount.y) * 2);

    std::vector<unsigned int> indices;
    indices.reserve(nIndices);

    unsigned int xVertexAmount = p.tileAmount.x + 1;

    for (unsigned int j = 0; j < p.tileAmount.y; ++j) {
        for (unsigned int i = 0; i < xVertexAmount; ++i) {
            indices.push_back(i + (xVertexAmount * (j + 1)));
            indices.push_back(i + (xVertexAmount * j));
        }
    }

    unsigned int primitiveSize = 2 * xVertexAmount;
    std::vector<unsigned int> primitiveSizes(p.tileAmount.y, primitiveSize);

    std::vector<void*> primitiveOffsets(p.tileAmount.y);
    for (unsigned int j = 0; j < p.tileAmount.y; ++j) {
        primitiveOffsets[j] = reinterpret_cast<void*>(j * primitiveSize * sizeof(int));
    }

    return std::make_unique<Mesh>(GL_TRIANGLE_STRIP, vertices, indices, primitiveSizes, primitiveOffsets);
}

} // namespace rb

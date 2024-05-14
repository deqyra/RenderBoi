#include "axes_generator.hpp"

#include <vector>
#include <memory>

#include <renderboi/core/color.hpp>
#include <renderboi/core/numeric.hpp>
#include <renderboi/core/3d/mesh.hpp>
#include <renderboi/core/3d/vertex.hpp>

#include <glad/gl.h>

namespace rb {

AxesGenerator::AxesGenerator(const Parameters& parameters) :
    parameters(parameters)
{

}

std::unique_ptr<Mesh> AxesGenerator::generate() const
{
    float len = parameters.axisLength;

    std::vector<Vertex> vertices = {
        // Position       // Color       // Normal       // Tex coord
        {  num::Origin3,  color::Red,    num::Y * -1.f,  num::Origin2 },
        {  num::X * len,  color::Red,    num::Y * -1.f,  num::Origin2 },
        {  num::Origin3,  color::Green,  num::Z * -1.f,  num::Origin2 },
        {  num::Y * len,  color::Green,  num::Z * -1.f,  num::Origin2 },
        {  num::Origin3,  color::Blue,   num::X * -1.f,  num::Origin2 },
        {  num::Z * len,  color::Blue,   num::X * -1.f,  num::Origin2 }
    };

    std::vector<unsigned int> indices = {
        0, 1,   // X axis
        2, 3,   // Y axis
        4, 5    // Z axis
    };

    return std::make_unique<Mesh>(GL_LINES, std::move(vertices), std::move(indices));
}

} // namespace rb

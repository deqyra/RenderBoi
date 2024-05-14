#ifndef RENDERBOI_CORE_VERTEX_HPP
#define RENDERBOI_CORE_VERTEX_HPP

#include <renderboi/core/numeric.hpp>

namespace rb {

/// @brief A vertex with basic attributes
struct Vertex {
    /// @brief The 3D position of the vertex
    num::Vec3 position;

    /// @brief The RGB color of the vertex
    num::Vec3 color;

    /// @brief The normal vector of the vertex
    num::Vec3 normal;

    /// @brief Texture coordinates associated with the vertex
    num::Vec2 texCoord;
};

} // namespace rb

#endif//RENDERBOI_CORE_VERTEX_HPP

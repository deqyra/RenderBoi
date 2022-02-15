#ifndef RENDERBOI__TOOLBOX__MESH_GENERATORS__MESH_TYPE_HPP
#define RENDERBOI__TOOLBOX__MESH_GENERATORS__MESH_TYPE_HPP

namespace renderboi
{

/// @brief Collection of literals describing the different shapes a mesh can 
// come in. Each of these can be mapped to a concrete MeshGenerator subclass.
enum class MeshType
{
    Axes,
    Cube,
    Plane,
    Tetrahedron,
    Torus
};

/// @brief Templated meta data meant to be specialized by inheriting
/// mesh generators.
template<MeshType T>
struct MeshTypeMeta
{
    /// @brief Use ::type to get the concrete type of the mesh generator for T.
    struct Generator {};
};

} // namespace renderboi

#endif//RENDERBOI__TOOLBOX__MESH_GENERATORS__MESH_TYPE_HPP
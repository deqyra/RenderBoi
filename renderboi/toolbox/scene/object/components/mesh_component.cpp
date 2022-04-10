#include "mesh_component.hpp"

#include <cpptools/utility/bitwise_enum_ops.hpp>

#include <renderboi/core/materials.hpp>
#include <renderboi/core/shader/shader_builder.hpp>

namespace renderboi::component
{

MeshComponent::MeshComponent(
    MeshPtr&& mesh,
    const Material material,
    const ShaderProgram shader
) :
    _meshPtr(std::move(mesh)),
    _mesh(*_meshPtr),
    _material(material),
    _shader(shader)
{
    if (!_meshPtr)
    {
        throw std::runtime_error("MeshComponentComponent: cannot construct from null mesh pointer.");
    }
}

MeshComponent::MeshComponent(
    Mesh& mesh,
    const Material material,
    const ShaderProgram shader
) :
    _meshPtr(nullptr),
    _mesh(mesh),
    _material(material),
    _shader(shader)
{
}

MeshComponent::~MeshComponent()
{

}

Mesh& MeshComponent::mesh()
{
    return _mesh;
}

Material& MeshComponent::material()
{
    return _material;
}

ShaderProgram& MeshComponent::shader()
{
    return _shader;
}

} // namespace renderboi::component

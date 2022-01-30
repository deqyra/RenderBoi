#include "mesh_component.hpp"

#include <cpptools/utility/bitwise_enum_ops.hpp>

#include <renderboi/core/materials.hpp>
#include <renderboi/core/shader/shader_builder.hpp>

namespace renderboi
{

MeshComponent::MeshComponent(
    SceneObject& sceneObject,
    MeshPtr&& mesh,
    const Material material,
    const ShaderProgram shader
) :
    Component(sceneObject),
    _meshPtr(std::move(mesh)),
    _mesh(*_meshPtr),
    _material(material),
    _shader(shader)
{
    if (!_meshPtr)
    {
        throw std::runtime_error("MeshComponent: cannot construct from null mesh pointer.");
    }
}

MeshComponent::MeshComponent(
    SceneObject& sceneObject,
    Mesh& mesh,
    const Material material,
    const ShaderProgram shader
) :
    Component(sceneObject),
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

MeshComponent* MeshComponent::clone(SceneObject& newParent) const
{
    // Material and shader clones are automatically copy-constructed by the following call
    return new MeshComponent(
        newParent,
        std::make_unique<Mesh>(_mesh),
        _material,
        _shader
    );
}

} // namespace renderboi

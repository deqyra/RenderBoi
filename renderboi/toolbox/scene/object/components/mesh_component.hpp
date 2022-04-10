#ifndef RENDERBOI__TOOLBOX__SCENE__COMPONENTS__MESH_COMPONENT_HPP
#define RENDERBOI__TOOLBOX__SCENE__COMPONENTS__MESH_COMPONENT_HPP

#include <string>
#include <type_traits>

#include <cpptools/utility/bitwise_enum_ops.hpp>

#include <renderboi/core/mesh.hpp>
#include <renderboi/core/material.hpp>
#include <renderboi/core/shader/shader_program.hpp>

#include "renderboi/core/shader/shader_builder.hpp"

namespace renderboi::component
{

/// @brief Component allowing to attach a mesh to a scene object.
class MeshComponent
{
private:
    MeshComponent(MeshComponent& other) = delete;
    MeshComponent& operator=(const MeshComponent& other) = delete;

    /// @brief Pointer to the mesh data of the object, if owned. Is null when
    /// the referenced mesh isn't owned by the instance.
    MeshPtr _meshPtr;

    /// @brief Reference to the mesh data of the object.
    Mesh& _mesh;

    /// @brief Material to paint the mesh with.
    Material _material;
    
    /// @brief Shader program to render the mesh with.
    ShaderProgram _shader;

public:
    /// @param mesh Reference to the mesh which the component should use.
    /// @param material Material which the mesh should be rendered with.
    /// @param shader Shader program which the mesh should be rendred by.
    ///
    /// @exception If the provided mesh pointer is null, the constructor
    /// will throw a std::runtime_error.
    MeshComponent(
        MeshPtr&& mesh,
        const Material material,
        const ShaderProgram shader = ShaderBuilder::MinimalShaderProgram()
    );

    /// @param mesh Reference to the mesh which the component should use.
    /// @param material Material which the mesh should be rendered with.
    /// @param shader Shader program which the mesh should be rendred by.
    MeshComponent(
        Mesh& mesh,
        const Material material,
        const ShaderProgram shader = ShaderBuilder::MinimalShaderProgram()
    );

    ~MeshComponent();

    /// @brief Get the mesh used by the component.
    ///
    /// @return The mesh used by the component.
    Mesh& mesh();

    /// @brief Get the material used by the component.
    ///
    /// @return The material used by the component.
    Material& material();

    /// @brief Get the shader used by the component.
    ///
    /// @return The shader used by the component.
    ShaderProgram& shader();
};

} // namespace renderboi

#endif//RENDERBOI__TOOLBOX__SCENE__COMPONENTS__MESH_COMPONENT_HPP
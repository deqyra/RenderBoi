#ifndef RENDERBOI_TOOLBOX_SCENE_COMPONENTS_RENDERED_MESH_COMPONENT_HPP
#define RENDERBOI_TOOLBOX_SCENE_COMPONENTS_RENDERED_MESH_COMPONENT_HPP

#include <renderboi/core/3d/mesh.hpp>
#include <renderboi/core/material.hpp>
#include <renderboi/core/shader/shader_program.hpp>

namespace rb {

/// @brief Component allowing to attach a mesh to a scene object
struct RenderedMeshComponent {
    /// @brief Reference to the mesh data of the object
    Mesh* mesh;

    /// @brief Material to paint the mesh with
    Material* material;
    
    /// @brief Shader program to render the mesh with
    ShaderProgram* shader;
};

} // namespace rb

#endif//RENDERBOI_TOOLBOX_SCENE_COMPONENTS_RENDERED_MESH_COMPONENT_HPP
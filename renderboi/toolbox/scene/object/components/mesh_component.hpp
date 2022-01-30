#ifndef RENDERBOI__TOOLBOX__SCENE__COMPONENTS__MESH_COMPONENT_HPP
#define RENDERBOI__TOOLBOX__SCENE__COMPONENTS__MESH_COMPONENT_HPP

#include <string>
#include <type_traits>

#include <cpptools/utility/bitwise_enum_ops.hpp>

#include <renderboi/core/mesh.hpp>
#include <renderboi/core/material.hpp>
#include <renderboi/core/shader/shader_program.hpp>

#include "../component.hpp"
#include "../component_type.hpp"
#include "renderboi/core/shader/shader_builder.hpp"

namespace renderboi
{

/// @brief Component allowing to attach a mesh to a scene object.
class MeshComponent : public Component
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
    /// @param sceneObject Reference to the scene object which will be parent
    /// to this component.
    /// @param mesh Reference to the mesh which the component should use.
    /// @param material Material which the mesh should be rendered with.
    /// @param shader Shader program which the mesh should be rendred by.
    ///
    /// @exception If the provided mesh pointer is null, the constructor
    /// will throw a std::runtime_error.
    MeshComponent(
        SceneObject& sceneObject,
        MeshPtr&& mesh,
        const Material material,
        const ShaderProgram shader = ShaderBuilder::MinimalShaderProgram()
    );

    /// @param sceneObject Reference to the scene object which will be parent
    /// to this component.
    /// @param mesh Reference to the mesh which the component should use.
    /// @param material Material which the mesh should be rendered with.
    /// @param shader Shader program which the mesh should be rendred by.
    MeshComponent(
        SceneObject& sceneObject,
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

    /////////////////////////////////////////
    ///                                   ///
    /// Methods overridden from Component ///
    ///                                   ///
    /////////////////////////////////////////

    /// @brief Get a raw pointer to a new component instance cloned 
    /// from this one. Ownership and responsibility for the allocated 
    /// resources are fully transferred to the caller.
    ///
    /// @param newParent Pointer the scene object which will be parent to
    /// the cloned component instance.
    ///
    /// @return A raw pointer to the component instance cloned from this 
    /// one.
    MeshComponent* clone(SceneObject& newParent) const override;
};


template<>
struct ComponentMeta<ComponentType::Mesh>
{
    struct MultipleInstancesAllowed
    {
        static constexpr bool value = false;
    };

    struct ConcreteType
    {
        using type = MeshComponent;
    };

    struct Name
    {
        static constexpr const char* value = "MeshComponent";
    };
};

template<>
struct ComponentTypeToEnum<MeshComponent>
{
    static constexpr ComponentType value = ComponentType::Mesh;
};

} // namespace renderboi

#endif//RENDERBOI__TOOLBOX__SCENE__COMPONENTS__MESH_COMPONENT_HPP
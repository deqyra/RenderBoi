#ifndef MESH_COMPONENT_HPP
#define MESH_COMPONENT_HPP

#include "../scene_object_component.hpp"
#include "../scene_object_component_type.hpp"
#include "../../mesh.hpp"
#include "../../material.hpp"
#include "../../shader.hpp"

#include <string>

class MeshComponent : public SceneObjectComponent
{
    public:
        MeshComponent(MeshPtr mesh);
        MeshComponent(MeshPtr mesh, Material material);
        MeshComponent(MeshPtr mesh, Shader shader);
        MeshComponent(MeshPtr mesh, Material material, Shader shader);

        virtual ~MeshComponent();

        MeshPtr mesh;
        Material material;
        Shader shader;
};

template<>
SceneObjectComponentType SceneObjectComponent::componentType<MeshComponent>();

template<>
std::string SceneObjectComponent::componentTypeString<MeshComponent>();

#endif//MESH_COMPONENT_HPP
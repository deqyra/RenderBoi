#ifndef RENDERBOI__TOOLBOX__FACTORY_HPP
#define RENDERBOI__TOOLBOX__FACTORY_HPP

#include <renderboi/core/materials.hpp>
#include <renderboi/core/mesh.hpp>
#include <renderboi/core/shader/shader_builder.hpp>

#include "script.hpp"
#include "mesh_generators/all_mesh_generators.hpp"
#include "mesh_generators/mesh_generator.hpp"
#include "mesh_generators/mesh_type.hpp"
#include "scene/scene.hpp"
#include "scene/object/scene_object.hpp"
#include "scene/object/component.hpp"
#include "scene/object/components/mesh_component.hpp"
#include "scene/object/components/script_component.hpp"

namespace renderboi
{

/// @brief Provides static methods to facilitate complex object instantiation.
class Factory
{
    public:
        /// @brief Create a mesh with vertices arranged in a certain shape.
        ///
        /// @tparam T Required. Literal describing which mesh generator to use.
        /// @tparam Gen Concrete type of the mesh generator for T. Leave for
        /// deduction.
        ///
        /// @param parameters Parameters to pass to the mesh generator.
        /// Look up generators in core/mesh_generators/ for details.
        ///
        /// @return Pointer to the generated mesh.
        template<
            MeshType T,
            typename Gen = typename MeshTypeMeta<T>::Generator::type
        >
        static MeshPtr MakeMesh(const typename Gen::Parameters& parameters);

        /// @brief Instantiate and initiliaze a scene object, generate a mesh,
        /// and attach it to the scene object within a mesh component.
        ///
        /// @tparam T Required. Literal describing which mesh generator to use.
        /// @tparam Gen Concrete type of the mesh generator for T. Leave for
        /// deduction.
        ///
        /// @param scene Reference to the parent of the newly created object.
        /// @param parameters Parameters of the mesh generation.
        /// @param mat Material to render the mesh in.
        /// @param shader Shader program to render the mesh with.
        ///
        /// @return Reference to the instantiated scene object.
        template<
            MeshType T,
            typename MeshGen = typename MeshTypeMeta<T>::Generator::type
        >
        static SceneObject& MakeSceneObjectWithMesh(
            Scene& scene,
            const typename MeshGen::Parameters& parameters,
            const Material& mat,
            const ShaderProgram& shader
        );

        /// @brief Instantiate and initiliaze a scene object, generate a mesh,
        /// and attach it to the scene object within a mesh component.
        ///
        /// @tparam T Required. Literal describing which mesh generator to use.
        /// @tparam Gen Concrete type of the mesh generator for T. Leave for
        /// deduction.
        ///
        /// @param scene Reference to the parent of the newly created object.
        /// @param name Name to give to the scene object.
        /// @param parameters Parameters of the mesh generation.
        /// @param mat Material to render the mesh in.
        /// @param shader Shader program to render the mesh with.
        ///
        /// @return Reference to the instantiated scene object.
        template<
            MeshType T,
            typename MeshGen = typename MeshTypeMeta<T>::Generator::type
        >
        static SceneObject& MakeSceneObjectWithMesh(
            Scene& scene,
            const std::string& name,
            const typename MeshGen::Parameters& parameters,
            const Material& mat,
            const ShaderProgram& shader
        );

        /// @brief Create a script of any type and attach it to a scene object.
        ///
        /// @tparam T The class of the script to instantiate.
        /// @tparam ArgTypes Types of the arguments which will be used to create
        /// the script.
        ///
        /// @param object Reference to the scene object to which the newly 
        /// instantiated script should be attached.
        /// @param args Arguments to use to construct the script.
        ///
        /// @return A pointer to the newly instantiated script.
        ///
        /// @exception If the provided scene object does not meet criteria 
        /// required by the script about to be attached to it, exceptions may be
        /// thrown and behavior may be off.
        template<
            class T, typename... ArgTypes,
            typename = std::enable_if_t<std::is_base_of_v<Script, T>, Script>
        >
        static T& CreateScriptAndAttachToObject(SceneObject& object, ArgTypes&&... args);
};

template<
    MeshType T,
    typename MeshGen
>
MeshPtr Factory::MakeMesh(const typename MeshGen::Parameters& parameters)
{
    MeshGen gen = MeshGen(parameters);
    return gen.generateMesh();
}

template<
    MeshType T,
    typename MeshGen
>
SceneObject& Factory::MakeSceneObjectWithMesh(
    Scene& scene,
    const typename MeshGen::Parameters& parameters,
    const Material& mat,
    const ShaderProgram& shader
)
{
    return MakeSceneObjectWithMesh(scene, "", parameters, mat, shader);
}

template<
    MeshType T,
    typename Gen
>
SceneObject& Factory::MakeSceneObjectWithMesh(
    Scene& scene,
    const std::string& name,
    const typename Gen::Parameters& parameters,
    const Material& mat,
    const ShaderProgram& shader
)
{
    MeshPtr mesh = Factory::MakeMesh<T>(parameters);
    SceneObject& obj = scene.newObject(name);
    obj.componentMap().addComponent<ComponentType::Mesh>(std::move(mesh), mat, shader);
    return obj;
}

template<typename T, typename... ArgTypes, typename>
T& Factory::CreateScriptAndAttachToObject(SceneObject& object, ArgTypes&&... args)
{
    T* concreteScript = new T(std::forward<ArgTypes>(args)...);
    Script* baseScript = (Script*)concreteScript;
    object.componentMap().addComponent<ComponentType::Script>(ScriptPtr(baseScript));

    return *concreteScript;
}

} // namespace renderboi

#endif//RENDERBOI__TOOLBOX__FACTORY_HPP
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

namespace Renderboi
{

/// @brief Provides static methods to facilitate complex object instantiation.
class Factory
{
    public:
        /// @brief Instantiate and initialize a new scene.
        ///
        /// @return Pointer to the instantiated scene.
        static ScenePtr MakeScene();

        /// @brief Terminate a scene. Also releases the provided pointer.
        ///
        /// @param scene Pointer to the scene to terminate, which will be 
        /// released once done.
        static void DestroyScene(ScenePtr scene);

        /// @brief Instantiate and initialize a new scene object.
        ///
        /// @param scene Pointer to the parent of the newly created object.
        /// @param name Name to give to the scene object.
        ///
        /// @return Pointer to the instantiated scene object.
        static SceneObjectPtr MakeSceneObject(const ScenePtr scene, const std::string name = "");

        /// @brief Instantiate and initialize a new scene object.
        ///
        /// @param scene Pointer to the parent of the newly created object.
        /// @param parentId ID of the parent scene object in the scene.
        /// @param name Name to give to the scene object.
        ///
        /// @return Pointer to the instantiated scene object.
        SceneObjectPtr MakeSceneObjectWithParent(const ScenePtr scene, const unsigned int parentId, const std::string name = "");

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
        static MeshPtr MakeMesh(const typename Gen::Parameters parameters);

        /// @brief Instantiate and initiliaze a scene object, generate a mesh,
        /// and attach it to the scene object within a mesh component.
        ///
        /// @tparam T Required. Literal describing which mesh generator to use.
        /// @tparam Gen Concrete type of the mesh generator for T. Leave for
        /// deduction.
        ///
        /// @param scene Pointer to the parent of the newly created object.
        /// @param parameters Parameters of the mesh generation.
        /// @param mat Material to render the mesh in.
        /// @param shader Shader program to render the mesh with.
        ///
        /// @return Pointer to the instantiated scene object.
        template<
            MeshType T,
            typename Gen = typename MeshTypeMeta<T>::Generator::type
        >
        static SceneObjectPtr MakeSceneObjectWithMesh(
            const ScenePtr scene,
            const typename Gen::Parameters parameters,
            const Material mat = Material(Materials::Default),
            const ShaderProgram shader = ShaderBuilder::MinimalShaderProgram()
        );

        /// @brief Instantiate and initiliaze a scene object, generate a mesh,
        /// and attach it to the scene object within a mesh component.
        ///
        /// @tparam T Required. Literal describing which mesh generator to use.
        /// @tparam Gen Concrete type of the mesh generator for T. Leave for
        /// deduction.
        ///
        /// @param scene Pointer to the parent of the newly created object.
        /// @param name Name to give to the scene object.
        /// @param parameters Parameters of the mesh generation.
        /// @param mat Material to render the mesh in.
        /// @param shader Shader program to render the mesh with.
        ///
        /// @return Pointer to the instantiated scene object.
        template<
            MeshType T,
            typename Gen = typename MeshTypeMeta<T>::Generator::type
        >
        static SceneObjectPtr MakeSceneObjectWithMesh(
            const ScenePtr scene,
            const std::string name,
            const typename Gen::Parameters parameters,
            const Material mat = Materials::Default,
            const ShaderProgram shader = ShaderBuilder::MinimalShaderProgram()
        );

        /// @brief Create a script of any type and attach it to a scene object.
        ///
        /// @tparam T The class of the script to instantiate.
        /// @tparam ArgTypes Types of the arguments which will be used to create
        /// the script.
        ///
        /// @param object Pointer to the scene object to which the newly 
        /// instantiated script should be attached.
        /// @param args Arguments to use to construct the script.
        ///
        /// @return A pointer to the newly instantiated script.
        ///
        /// @exception If the provided scene object does not meet criteria 
        /// required by the script about to be attached to it, exceptions may be
        /// thrown and behavior may be off.
        template<class T, typename... ArgTypes>
        static std::shared_ptr<T> CreateScriptAndAttachToObject(SceneObjectPtr object, ArgTypes&&... args);
};

template<
    MeshType T,
    typename Gen
>
MeshPtr Factory::MakeMesh(const typename Gen::Parameters parameters)
{
    std::shared_ptr<Gen> gen = std::make_shared<Gen>(parameters);
    return gen->generateMesh();
}

template<
    MeshType T,
    typename Gen
>
SceneObjectPtr Factory::MakeSceneObjectWithMesh(
    const ScenePtr scene,
    const typename Gen::Parameters parameters,
    const Material mat,
    const ShaderProgram shader
)
{
    return MakeSceneObjectWithMesh(scene, "", parameters, mat, shader);
}

template<
    MeshType T,
    typename Gen
>
SceneObjectPtr Factory::MakeSceneObjectWithMesh(
    const ScenePtr scene,
    const std::string name,
    const typename Gen::Parameters parameters,
    const Material mat,
    const ShaderProgram shader
)
{
    MeshPtr mesh = Factory::MakeMesh<T>(parameters);
    SceneObjectPtr obj = Factory::MakeSceneObject(scene, name);
    obj->componentMap()->addComponent<ComponentType::Mesh>(mesh, mat, shader);
    return obj;
}

template<typename T, typename... ArgTypes>
std::shared_ptr<T> Factory::CreateScriptAndAttachToObject(SceneObjectPtr object, ArgTypes&&... args)
{
    std::shared_ptr<T> script = std::make_shared<T>(std::forward<ArgTypes>(args)...);
    ScriptPtr baseScript = std::static_pointer_cast<Script>(script);
    object->componentMap()->addComponent<ComponentType::Script>(baseScript);

    return script;
}

} // namespace Renderboi

#endif//RENDERBOI__TOOLBOX__FACTORY_HPP
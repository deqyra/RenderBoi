#ifndef CORE__FACTORY_HPP
#define CORE__FACTORY_HPP

#include "mesh.hpp"
#include "mesh_generator.hpp"
#include "mesh_generators/mesh_type.hpp"
#include "mesh_generators/type_to_gen_mapping.hpp"
#include "scene/scene.hpp"
#include "scene/scene_object.hpp"

/// @brief Provides static methods to facilitate complex object instantiation.
class Factory
{
    public:
        /// @brief Instantiate and initiliaze a new scene.
        ///
        /// @return Pointer to the instantiated scene.
        static ScenePtr makeScene();

        /// @brief Terminate a scene. Also releases the provided pointer.
        ///
        /// @param scene Pointer to the scene to terminate, which will be 
        /// released once done.
        static void destroyScene(ScenePtr& scene);

        /// @brief Instantiate and initialize a new scene object.
        ///
        /// @param name Name to give to the scene object.
        ///
        /// @return Pointer to the instantiated scene object.
        static SceneObjectPtr makeSceneObject(std::string name = "");

        /// @brief Create a mesh with vertices arranged in a certain shape.
        ///
        /// @tparam T Literal describing which mesh generator to use.
        ///
        /// @param parameters Parameters to pass to the mesh generator.
        /// Look up generators in core/mesh_generators/ for details.
        ///
        /// @return Pointer to the generated mesh.
        template<MeshType T>
        static MeshPtr makeMesh(typename TypeToGenMapping<T>::GenType::Parameters parameters);

        /// @brief Instantiate and initiliaze a scene object, generate a mesh,
        /// and attach it to the scene object within a mesh component.
        ///
        /// @tparam T Literal describing which mesh generator to use.
        ///
        /// @param parameters Parameters of the mesh generation.
        /// @param mat Material to render the mesh in.
        /// @param shader Shader to render the mesh with.
        ///
        /// @return Pointer to the instantiated scene object.
        template<MeshType T>
        static SceneObjectPtr makeSceneObjectWithMesh(typename TypeToGenMapping<T>::GenType::Parameters parameters, Material mat = Material(), Shader shader = Shader());

        /// @brief Instantiate and initiliaze a scene object, generate a mesh,
        /// and attach it to the scene object within a mesh component.
        ///
        /// @tparam T Literal describing which mesh generator to use.
        ///
        /// @param name Name to give to the scene object.
        /// @param parameters Parameters of the mesh generation.
        /// @param mat Material to render the mesh in.
        /// @param shader Shader to render the mesh with.
        ///
        /// @return Pointer to the instantiated scene object.
        template<MeshType T>
        static SceneObjectPtr makeSceneObjectWithMesh(std::string name, typename TypeToGenMapping<T>::GenType::Parameters parameters, Material mat = Material(), Shader shader = Shader());

        /// @brief Attach a MouseCameraScript and a KeyboardMovementScript to
        /// the provided object. The object must have a camera component and 
        /// must be registered in a scene prior to calling this function, 
        /// otherwise mouse and keyboard input will not be forwarded to the 
        /// scripts.
        ///
        /// @param object Pointer to the object which the new scripts should be
        /// attached to. 
        ///
        /// @exception If the provided object has no camera component, the 
        /// function will throw a std::runtime_error.
        static void addMouseKBScriptsToCameraObject(SceneObjectPtr object);
};

template<MeshType T>
MeshPtr Factory::makeMesh(typename TypeToGenMapping<T>::GenType::Parameters parameters)
{
    using GenType = typename TypeToGenMapping<T>::GenType;
    std::shared_ptr<GenType> gen = std::make_shared<GenType>(parameters);
    return gen->generatePtr();
}

template<MeshType T>
SceneObjectPtr Factory::makeSceneObjectWithMesh(typename TypeToGenMapping<T>::GenType::Parameters parameters, Material mat, Shader shader)
{
    return makeSceneObjectWithMesh("", parameters, mat, shader);
}

template<MeshType T>
SceneObjectPtr Factory::makeSceneObjectWithMesh(std::string name, typename TypeToGenMapping<T>::GenType::Parameters parameters, Material mat, Shader shader)
{
    MeshPtr mesh = Factory::makeMesh<T>(parameters);
    SceneObjectPtr obj = Factory::makeSceneObject(name);
    obj->addComponent<MeshComponent>(mesh, mat, shader);
    return obj;
}

#endif//CORE__FACTORY_HPP
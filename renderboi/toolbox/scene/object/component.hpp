#ifndef RENDERBOI__TOOLBOX__SCENE__OBJECT__COMPONENT_HPP
#define RENDERBOI__TOOLBOX__SCENE__OBJECT__COMPONENT_HPP

#include <memory>
#include <string>

#include "component_type.hpp"

namespace Renderboi
{

class SceneObject;
using SceneObjectPtr = std::shared_ptr<SceneObject>;
using SceneObjectWPtr = std::weak_ptr<SceneObject>;

class ComponentMap;

/*         ╔════════════╗
 *         ║   README   ║
 *         ╚════════════╝
 * 
 *     DERIVING FROM Component
 * ===============================
 *
 * - Write new component
 * - Add new enum value to ComponentType
 * - Add corresponding case line in to_string (in component_type.cpp)
 * - Add corresponding case line in Component::MultipleInstancesAllowed (in all_components.cpp)
 * - Specialize ComponentMeta like so:
 *
 *     template<>
 *     struct ComponentMeta<ComponentType::MyNewComponent>
 *     {
 *         struct MultipleInstancesAllowed
 *         {
 *             static constexpr bool value = (true or false);
 *         };
 *         struct ConcreteType
 *         {
 *             using type = MyNewComponent;
 *         };
 *         struct Name
 *         {
 *             static constexpr const char* value = "MyNewComponent";
 *         };
 *     };
 *
 * - Specialize ComponentTypeToEnum like so:
 *
 *     template<>
 *     struct ComponentTypeToEnum<MyNewComponent>
 *     {
 *         static constexpr ComponentType value = ComponentType::MyNewComponent;
 *     };
 *
 * - Include header in all_components.hpp
 */

/// @brief Abstract class meant to represent a certain aspect in a scene object
/// (a mesh, a light, etc). Refer to README section at the top of the .hpp file
/// for info about deriving from Component.
class Component
{
private:
    friend ComponentMap;

    Component() = delete;
    Component(const Component& other) = delete;
    Component& operator=(const Component& other) = delete;

protected:
    /// @param sceneObject Pointer to the SceneObject instance this 
    /// component belongs to.
    ///
    /// @exception If the provided scene object pointer is null, the constructor
    /// will throw an std::runtime_error.
    Component(const SceneObjectPtr sceneObject);

    /// @brief Pointer to the SceneObject this component belongs to.
    SceneObjectPtr _sceneObject;

    /// @brief Release held references to shared resources.
    virtual void _release();

public:
    virtual ~Component();

    /// @brief Get a pointer to the parent scene object of this component.
    ///
    /// @return A pointer to the parent scene object of this component.
    SceneObjectPtr getSceneObject() const;

    /// @brief Get a raw pointer to a new component instance cloned 
    /// from this one. Ownership and responsibility for the allocated 
    /// resources are fully transferred to the caller.
    ///
    /// @param newParent Pointer the scene object which will be parent to
    /// the cloned component instance.
    ///
    /// @return A raw pointer to the component instance cloned from this 
    /// one.
    virtual Component* clone(const SceneObjectPtr newParent) const = 0;

    /// @brief Returns whether more than one instance of this component type can
    /// be present on a same scene object.
    ///
    /// @see See all_components.cpp for implementation (separate to keep
    /// concrete stuff out).
    ///
    /// @param type Litteral describing the type of component subclass to test.
    ///
    /// @return Whether or not more than one instance of this component type can
    /// be present on a same scene object.
    static bool MultipleInstancesAllowed(const ComponentType type);

};

/// @brief Templated meta data meant to be specialized by inheriting
/// components.
template<ComponentType T>
struct ComponentMeta
{
    /// @brief Use ::value to tell whether multiple instances of the
    /// component type described by T are allowed within one scene object.
    struct MultipleInstancesAllowed {};

    /// @brief Use ::type to get the concrete component type that corresponds to
    /// T.
    struct ConcreteType {};

    /// @brief Use ::value to get the name of this component type.
    struct Name {};
};

/// @brief Templated meta data meant to be specialized by inheriting
/// components.
template<class T>
struct ComponentTypeToEnum {};

using ComponentPtr = std::shared_ptr<Component>;
using ComponentWPtr = std::weak_ptr<Component>;

} // namespace Renderboi

#endif//RENDERBOI__TOOLBOX__SCENE__OBJECT__COMPONENT_HPP
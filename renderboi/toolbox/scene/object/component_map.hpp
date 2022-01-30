#ifndef RENDERBOI__TOOLBOX__SCENE__OBJECT__COMPONENT_MAP_HPP
#define RENDERBOI__TOOLBOX__SCENE__OBJECT__COMPONENT_MAP_HPP

#include <initializer_list>
#include <map>
#include <memory>
#include <stdexcept>
#include <type_traits>
#include <utility>
#include <vector>

#include "component.hpp"
#include "component_type.hpp"

namespace renderboi
{

class SceneObject;
using SceneObjectPtr = std::unique_ptr<SceneObject>;

class ComponentMap
{
friend SceneObject;

private:
    using MapType = std::map<ComponentType, ComponentPtr>;
    using MultimapType = std::multimap<ComponentType, ComponentPtr>;

public:
    using ComponentEntry = std::pair<const ComponentType, ComponentPtr>;

private:
    ComponentMap& operator=(const ComponentMap& right) = delete;

    /// @param sceneObject Reference to the parent scene object of this instance.
    /// @param other Instance to copy-construct from.
    ComponentMap(SceneObject& sceneObject, const ComponentMap& other);

    /// @brief Map to store pointers to components for which only single
    /// instances are allowed on a single scene object.
    MapType _singleComponentMap;

    /// @brief Multimap to store pointers to components for which multiple
    /// instances are allowed on a single scene object.
    MultimapType _multiComponentMap;

    /// @brief Reference to the parent scene object.
    SceneObject& _sceneObject;

public:
    /// @param sceneObject Reference to the parent scene object of this instance.
    ComponentMap(SceneObject& sceneObject);

    /// @brief Get a pointer to the parent scene object of this instance.
    ///
    /// @return A pointer to the parent scene object of this instance.
    SceneObject& sceneObject();

    /// @brief Get the component pointer of the given component type (single 
    /// instance variant).
    ///
    /// @tparam Ty Required. Litteral describing the type of component to get a
    /// pointer to.
    ///
    /// @return Reference to the component for the given type (can be null).
    template<
        ComponentType Ty,
        typename CTy = typename ComponentMeta<Ty>::ConcreteType::type,
        std::enable_if_t<!ComponentMeta<Ty>::MultipleInstancesAllowed::value, bool> = true
    >
    CTy& getComponent() const;

    /// @brief Get an iterator range to component pointers of the given
    /// component type (multi instance variant).
    ///
    /// @tparam Ty Required. Litteral describing the type of component to get a
    /// range of pointers to.
    ///
    /// @return Iterator range to pointers to the components for the given type.
    template<
        ComponentType Ty,
        typename CTy = typename ComponentMeta<Ty>::ConcreteType::type,
        std::enable_if_t<ComponentMeta<Ty>::MultipleInstancesAllowed::value, bool> = true
    >
    std::vector<std::reference_wrapper<CTy>> getComponents() const;

    /// @brief Return whether there are registered components for the given
    /// component type (multi instance variant).
    ///
    /// @tparam Ty Required. Component type to test components for.
    ///
    /// @return Whether there are registered components for the given component
    /// type.
    template<
        ComponentType Ty,
        std::enable_if_t<!ComponentMeta<Ty>::MultipleInstancesAllowed::value, bool> = true
    >
    bool hasComponent() const;

    /// @brief Return whether there are registered components for the given
    /// component type (single instance variant).
    ///
    /// @tparam Ty Required. Component type to test components for.
    ///
    /// @return Whether there are registered components for the given component
    /// type.
    template<
        ComponentType Ty,
        std::enable_if_t<ComponentMeta<Ty>::MultipleInstancesAllowed::value, bool> = true
    >
    bool hasComponent() const;

    /// @brief Construct a component in place (single instance variant).
    ///
    /// @tparam Ty Required. Litteral describing the type of the component.
    /// @tparam CTy Concrete type of the component. Leave for deduction.
    /// @tparam ArgTypes... Types of the arguments that are to be forwarded to 
    /// the constructor of the component. Leave for deduction.
    ///
    /// @param args Arguments to be passed to the constructor of the component.
    ///
    /// @return A reference to the newly constructed component.
    template<
        ComponentType Ty,
        class CTy = typename ComponentMeta<Ty>::ConcreteType::type,
        class... ArgTypes,
        std::enable_if_t<!ComponentMeta<Ty>::MultipleInstancesAllowed::value, bool> = true
    >
    CTy& addComponent(ArgTypes&&... args);

    /// @brief Construct a component in place (single instance variant).
    ///
    /// @tparam Ty Required. Litteral describing the type of the component.
    /// @tparam CTy Concrete type of the component. Leave for deduction.
    /// @tparam ArgTypes... Types of the arguments that are to be forwarded to 
    /// the constructor of the component. Leave for deduction.
    ///
    /// @param args Arguments to be passed to the constructor of the component.
    ///
    /// @return A reference to the newly constructed component.
    template<
        ComponentType Ty,
        class CTy = typename ComponentMeta<Ty>::ConcreteType::type,
        class... ArgTypes,
        std::enable_if_t<ComponentMeta<Ty>::MultipleInstancesAllowed::value, bool> = true
    >
    CTy& addComponent(ArgTypes&&... args);

    /// @brief Register a component (single instance variant).
    ///
    /// @tparam CTy Concrete type of the component. Leave for deduction.
    /// @tparam Ty Litteral describing the type of the component. Leave for
    /// deduction.
    ///
    /// @param type Type to set the component for.
    /// @param component Reference to the component to set for that type.
    template<
        class CTy,
        ComponentType Ty = ComponentTypeToEnum<CTy>::value,
        std::enable_if_t<!ComponentMeta<Ty>::MultipleInstancesAllowed::value, bool> = true
    >
    CTy& putComponent(std::unique_ptr<CTy>&& component);

    /// @brief Register a component (multi instance variant).
    ///
    /// @tparam CTy Concrete type of the component. Leave for deduction.
    /// @tparam Ty Litteral describing the type of the component. Leave for
    /// deduction.
    ///
    /// @param type Type to set the component for.
    /// @param component Reference to the component to set for that type.
    template<
        class CTy,
        ComponentType Ty = ComponentTypeToEnum<CTy>::value,
        std::enable_if_t<ComponentMeta<Ty>::MultipleInstancesAllowed::value, bool> = true
    >
    CTy& putComponent(std::unique_ptr<CTy>&& component);

    /// @brief Remove the component pointer for the given component type (single
    /// instance variant).
    ///
    /// @tparam Ty Required. Type whose component is to be cleared.
    template<
        ComponentType Ty,
        std::enable_if_t<!ComponentMeta<Ty>::MultipleInstancesAllowed::value, bool> = true
    >
    void clear();

    /// @brief Remove the component pointers for the given component type (multi
    /// instance variant).
    ///
    /// @tparam Ty Required. Type whose components are to be cleared.
    template<
        ComponentType Ty,
        std::enable_if_t<ComponentMeta<Ty>::MultipleInstancesAllowed::value, bool> = true
    >
    void clear();

    /// @brief Clear all component entries from the component map.
    void clear();
};

using ComponentMapPtr = std::unique_ptr<ComponentMap>;

template<
    ComponentType Ty,
    typename CTy,
    std::enable_if_t<!ComponentMeta<Ty>::MultipleInstancesAllowed::value, bool>
>
CTy& ComponentMap::getComponent() const
{
    auto it = _singleComponentMap.find(Ty);

    if (it == _singleComponentMap.end())
    {
        std::string s = "ComponentMap: cannot get component \"" + to_string(Ty)
        + "\" from object.";
        throw std::runtime_error(s.c_str());
    }

    return (CTy&)(*(it->second));
}

template<
    ComponentType Ty,
    typename CTy,
    std::enable_if_t<ComponentMeta<Ty>::MultipleInstancesAllowed::value, bool>
>
std::vector<std::reference_wrapper<CTy>> ComponentMap::getComponents() const
{
    std::vector<std::reference_wrapper<CTy>> components;

    auto range = _multiComponentMap.equal_range(Ty);
    for (auto it = range.first; it != range.second; it++)
    {
        components.push_back(*(it->second));
    }

    return components;
}

template<
    ComponentType Ty,
    std::enable_if_t<!ComponentMeta<Ty>::MultipleInstancesAllowed::value, bool>
>
bool ComponentMap::hasComponent() const
{
    return _singleComponentMap.contains(Ty);
}

template<
    ComponentType Ty,
    std::enable_if_t<ComponentMeta<Ty>::MultipleInstancesAllowed::value, bool>
>
bool ComponentMap::hasComponent() const
{
    return _multiComponentMap.contains(Ty);
}

template<
    ComponentType Ty,
    class CTy,
    class... ArgTypes,
    std::enable_if_t<!ComponentMeta<Ty>::MultipleInstancesAllowed::value, bool>
>
CTy& ComponentMap::addComponent(ArgTypes&&... args)
{
    if (hasComponent<Ty>())
    {
        using std::to_string;

        std::string s = "ComponentMap: cannot construct another instance of "
        "component of type \"" + to_string(Ty) + "\" into single component map";

        throw std::runtime_error(s.c_str());
    }

    // Construct component from passed arguments
    std::unique_ptr<CTy> component = std::make_unique<CTy>(_sceneObject, std::forward<ArgTypes>(args)...);
    CTy& componentRef = *component;

    auto [it, _] = _singleComponentMap.insert({
        Ty,
        std::move(component)
    });

    return componentRef;
}

template<
    ComponentType Ty,
    class CTy,
    class... ArgTypes,
    std::enable_if_t<ComponentMeta<Ty>::MultipleInstancesAllowed::value, bool>
>
CTy& ComponentMap::addComponent(ArgTypes&&... args)
{
    // Construct component from passed arguments
    std::unique_ptr<CTy> component = std::make_unique<CTy>(_sceneObject, std::forward<ArgTypes>(args)...);
    CTy& componentRef = *component;

    auto it = _multiComponentMap.insert({
        Ty,
        std::move(component)
    });

    return componentRef;
}

template<
    class CTy,
    ComponentType Ty,
    std::enable_if_t<!ComponentMeta<Ty>::MultipleInstancesAllowed::value, bool>
>
CTy& ComponentMap::putComponent(std::unique_ptr<CTy>&& component)
{
    auto it = _singleComponentMap.find(Ty);

    if (it != _singleComponentMap.end())
    {
        using std::to_string;

        std::string s = "ComponentMap: cannot insert several instances of "
        "component of type \"" + to_string(Ty) + "\" into single component map";

        throw std::runtime_error(s.c_str());
    }

    CTy& ref = *component;
    _singleComponentMap.insert({Ty, std::move(component)});

    return ref;
}

template<
    class CTy,
    ComponentType Ty,
    std::enable_if_t<ComponentMeta<Ty>::MultipleInstancesAllowed::value, bool>
>
CTy& ComponentMap::putComponent(std::unique_ptr<CTy>&& component)
{
    CTy& ref = *component;
    _multiComponentMap.insert({Ty, std::move(component)});

    return ref;
}

template<
    ComponentType Ty,
    std::enable_if_t<!ComponentMeta<Ty>::MultipleInstancesAllowed::value, bool>
>
void ComponentMap::clear()
{
    auto it = _singleComponentMap.find(Ty);
    if (it == _singleComponentMap.end()) return;

    _singleComponentMap.erase(it);
}

template<
    ComponentType Ty,
    std::enable_if_t<ComponentMeta<Ty>::MultipleInstancesAllowed::value, bool>
>
void ComponentMap::clear()
{
    auto [first, last] = _multiComponentMap.equal_range(Ty);
    _multiComponentMap.erase(first, last);
}


} // namespace renderboi

#endif//RENDERBOI__TOOLBOX__SCENE__OBJECT__COMPONENT_MAP_HPP
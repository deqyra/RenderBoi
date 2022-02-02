#ifndef RENDERBOI__TOOLBOX__SCENE__OBJECT__COMPONENT_MAP_HPP
#define RENDERBOI__TOOLBOX__SCENE__OBJECT__COMPONENT_MAP_HPP

#include <initializer_list>
#include <map>
#include <memory>
#include <stdexcept>
#include <type_traits>
#include <utility>

#include "component.hpp"
#include "component_type.hpp"

namespace Renderboi
{

class SceneObject;
using SceneObjectPtr = std::shared_ptr<SceneObject>;

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

    /// @param sceneObject Pointer to the parent scene object of this instance.
    /// @param other Instance to copy-construct from.
    ComponentMap(const SceneObjectPtr sceneObject, const ComponentMap& other);

    /// @brief Map to store pointers to components for which only single
    /// instances are allowed on a single scene object.
    MapType _singleComponentMap;

    /// @brief Multimap to store pointers to components for which multiple
    /// instances are allowed on a single scene object.
    MultimapType _multiComponentMap;

    /// @brief Pointer to the parent scene object.
    SceneObjectPtr _sceneObject;

    /// @brief Release held references to shared resources.
    void _release();

public:
    /// @param sceneObject Pointer to the parent scene object of this instance.
    ComponentMap(const SceneObjectPtr sceneObject);

    /// @brief Get a pointer to the parent scene object of this instance.
    ///
    /// @return A pointer to the parent scene object of this instance.
    SceneObjectPtr sceneObject() const;

    /// @brief Get the component pointer of the given component type (single 
    /// instance variant).
    /// @tparam Ty Required. Litteral describing the type of component to get a
    /// pointer to.
    /// @return Pointer to the component for the given type (can be null).
    template<
        ComponentType Ty,
        typename CTy = typename ComponentMeta<Ty>::ConcreteType::type,
        std::enable_if_t<!ComponentMeta<Ty>::MultipleInstancesAllowed::value, bool> = true
    >
    std::shared_ptr<CTy> getComponent() const;

    /// @brief Get an iterator range to component pointers of the given
    /// component type (multi instance variant).
    /// @tparam Ty Required. Litteral describing the type of component to get a
    /// range of pointers to.
    /// @return Iterator range to pointers to the components for the given type.
    template<
        ComponentType Ty,
        std::enable_if_t<ComponentMeta<Ty>::MultipleInstancesAllowed::value, bool> = true
    >
    std::pair<ComponentMap::MultimapType::const_iterator, ComponentMap::MultimapType::const_iterator>
    getComponents() const;

    /// @brief Return whether there are registered components for the given
    /// component type (multi instance variant).
    /// @tparam Ty Required. Component type to test components for.
    /// @return Whether there are registered components for the given component
    /// type.
    template<
        ComponentType Ty,
        std::enable_if_t<!ComponentMeta<Ty>::MultipleInstancesAllowed::value, bool> = true
    >
    bool hasComponent() const;

    /// @brief Return whether there are registered components for the given
    /// component type (single instance variant).
    /// @tparam Ty Required. Component type to test components for.
    /// @return Whether there are registered components for the given component
    /// type.
    template<
        ComponentType Ty,
        std::enable_if_t<ComponentMeta<Ty>::MultipleInstancesAllowed::value, bool> = true
    >
    bool hasComponent() const;

    /// @brief Construct a component in place (single instance variant).
    /// @tparam Ty Required. Litteral describing the type of the component.
    /// @tparam CTy Concrete type of the component. Leave for deduction.
    /// @tparam ArgTypes... Types of the arguments that are to be forwarded to 
    /// the constructor of the component. Leave for deduction.
    /// @param args Arguments to be passed to the constructor of the component.
    /// @return A shared pointer to the newly constructed component.
    template<
        ComponentType Ty,
        class CTy = typename ComponentMeta<Ty>::ConcreteType::type,
        class... ArgTypes,
        std::enable_if_t<!ComponentMeta<Ty>::MultipleInstancesAllowed::value, bool> = true
    >
    std::shared_ptr<CTy> addComponent(ArgTypes&&... args);

    /// @brief Construct a component in place (single instance variant).
    /// @tparam Ty Required. Litteral describing the type of the component.
    /// @tparam CTy Concrete type of the component. Leave for deduction.
    /// @tparam ArgTypes... Types of the arguments that are to be forwarded to 
    /// the constructor of the component. Leave for deduction.
    /// @param args Arguments to be passed to the constructor of the component.
    /// @return A shared pointer to the newly constructed component.
    template<
        ComponentType Ty,
        class CTy = typename ComponentMeta<Ty>::ConcreteType::type,
        class... ArgTypes,
        std::enable_if_t<ComponentMeta<Ty>::MultipleInstancesAllowed::value, bool> = true
    >
    std::shared_ptr<CTy> addComponent(ArgTypes&&... args);

    /// @brief Register a component (single instance variant).
    /// @tparam CTy Concrete type of the component. Leave for deduction.
    /// @tparam Ty Litteral describing the type of the component. Leave for
    /// deduction.
    /// @param type Type to set the component for.
    /// @param component Pointer to the component to set for that type.
    template<
        class CTy,
        ComponentType Ty = ComponentTypeToEnum<CTy>::value,
        std::enable_if_t<!ComponentMeta<Ty>::MultipleInstancesAllowed::value, bool> = true
    >
    void putComponent(const CTy component);

    /// @brief Register a component (multi instance variant).
    /// @tparam CTy Concrete type of the component. Leave for deduction.
    /// @tparam Ty Litteral describing the type of the component. Leave for
    /// deduction.
    /// @param type Type to set the component for.
    /// @param component Pointer to the component to set for that type.
    template<
        class CTy,
        ComponentType Ty = ComponentTypeToEnum<CTy>::value,
        std::enable_if_t<ComponentMeta<Ty>::MultipleInstancesAllowed::value, bool> = true
    >
    void putComponent(const CTy component);

    /// @brief Remove the component pointer for the given component type (single
    /// instance variant).
    /// @tparam Ty Required. Type whose component is to be cleared.
    template<
        ComponentType Ty,
        std::enable_if_t<!ComponentMeta<Ty>::MultipleInstancesAllowed::value, bool> = true
    >
    void clear();

    /// @brief Remove the component pointers for the given component type (multi
    /// instance variant).
    /// @tparam Ty Required. Type whose components are to be cleared.
    template<
        ComponentType Ty,
        std::enable_if_t<ComponentMeta<Ty>::MultipleInstancesAllowed::value, bool> = true
    >
    void clear();

    /// @brief Clear all component entries from the component map.
    void clear();
};

using ComponentMapPtr = std::shared_ptr<ComponentMap>;

template<
    ComponentType Ty,
    typename CTy,
    std::enable_if_t<!ComponentMeta<Ty>::MultipleInstancesAllowed::value, bool>
>
std::shared_ptr<CTy> ComponentMap::getComponent() const
{
    auto it = _singleComponentMap.find(Ty);

    if (it == _singleComponentMap.end()) return nullptr;

    return static_pointer_cast<CTy>(it->second);
}

template<
    ComponentType Ty,
    std::enable_if_t<ComponentMeta<Ty>::MultipleInstancesAllowed::value, bool>
>
std::pair<ComponentMap::MultimapType::const_iterator, ComponentMap::MultimapType::const_iterator>
ComponentMap::getComponents() const
{
    return _multiComponentMap.equal_range(Ty);
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
std::shared_ptr<CTy> ComponentMap::addComponent(ArgTypes&&... args)
{
    bool xd = ComponentMeta<Ty>::MultipleInstancesAllowed::value;

    if (hasComponent<Ty>())
    {
        using std::to_string;

        std::string s = "ComponentMap: cannot construct another instance of "
        "component of type \"" + to_string(Ty) + "\" into single component map";

        throw std::runtime_error(s.c_str());
    }

    // Construct component from passed arguments
    const std::shared_ptr<CTy> realComponent = std::make_shared<CTy>(_sceneObject, std::forward<ArgTypes>(args)...);

    // std::shared_ptr<Component> and std::shared_ptr<MyComponent> are not covariant types, even if Component and MyComponent are.
    // Cast to base Component in order to add to collection.
    const ComponentPtr baseComponent = std::static_pointer_cast<Component>(realComponent);
    _singleComponentMap[Ty] = baseComponent;

    return realComponent;
}

template<
    ComponentType Ty,
    class CTy,
    class... ArgTypes,
    std::enable_if_t<ComponentMeta<Ty>::MultipleInstancesAllowed::value, bool>
>
std::shared_ptr<CTy> ComponentMap::addComponent(ArgTypes&&... args)
{
    bool xd = ComponentMeta<Ty>::MultipleInstancesAllowed::value;

    // Construct component from passed arguments
    const std::shared_ptr<CTy> realComponent = std::make_shared<CTy>(_sceneObject, std::forward<ArgTypes>(args)...);

    // std::shared_ptr<Component> and std::shared_ptr<MyComponent> are not covariant types, even if Component and MyComponent are.
    // Cast to base Component in order to add to collection.
    const ComponentPtr baseComponent = std::static_pointer_cast<Component>(realComponent);
    _multiComponentMap.insert({Ty, baseComponent});

    return realComponent;
}

template<
    class CTy,
    ComponentType Ty,
    std::enable_if_t<!ComponentMeta<Ty>::MultipleInstancesAllowed::value, bool>
>
void ComponentMap::putComponent(const CTy component)
{
    auto it = _singleComponentMap.find(Ty);

    if (it != _singleComponentMap.end())
    {
        using std::to_string;

        std::string s = "ComponentMap: cannot insert several instances of "
        "component of type \"" + to_string(Ty) + "\" into single component map";

        throw std::runtime_error(s.c_str());
    }

    _singleComponentMap[Ty] = component;
}

template<
    class CTy,
    ComponentType Ty,
    std::enable_if_t<ComponentMeta<Ty>::MultipleInstancesAllowed::value, bool>
>
void ComponentMap::putComponent(const CTy component)
{
    _multiComponentMap.insert({Ty, component});
}

template<
    ComponentType Ty,
    std::enable_if_t<!ComponentMeta<Ty>::MultipleInstancesAllowed::value, bool>
>
void ComponentMap::clear()
{
    auto it = _singleComponentMap.find(Ty);
    if (it == _singleComponentMap.end()) return;

    it->second->_release();
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


} // namespace Renderboi

#endif//RENDERBOI__TOOLBOX__SCENE__OBJECT__COMPONENT_MAP_HPP
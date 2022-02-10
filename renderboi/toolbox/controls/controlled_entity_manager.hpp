#ifndef RENDERBOI__TOOLBOX__CONTROLS__CONTROLLED_ENTITY_MANAGER_HPP
#define RENDERBOI__TOOLBOX__CONTROLS__CONTROLLED_ENTITY_MANAGER_HPP

#include <memory>
#include <type_traits>

#include <cpptools/utility/type_traits.hpp>
#include <cpptools/oo/interfaces/action_event_receiver.hpp>

#include "../interfaces/default_control_scheme_provider.hpp"
#include "control_scheme.hpp"
#include "control_event_translator.hpp"
#include "cpptools/utility/type_traits_impl/type_list.hpp"

namespace Renderboi
{

/// @brief Eases out management of entities which provide a default control 
/// scheme.
///
/// @tparam T Type of the entity to manage. It must have a public ActionType
/// typedef, and inherit from DefaultControlSchemeProvider<ActionType>, as well
/// as from ActionEventReceiver<ActionType, ArgTypes...>.
template<
    class T,
    typename = std::enable_if_t<
        std::is_base_of_v<
            T,
            DefaultControlSchemeProvider<typename T::ActionType>
        >
    >,
    typename = std::enable_if_t<
        std::is_base_of_v<
            T,
            typename type_utils::apply_types<
                typename cpptools::ActionEventReceiver,
                typename T::TemplateTypes
            >::type
        >
    >
>
class ControlledEntityManager
{
private:
    /// @brief Managed entity.
    T _entity;

    /// @brief Scheme manager provided by the entity.
    ControlScheme<typename T::ActionType> _controlScheme;

    /// @brief Object to translate events to actions and forward them to the
    /// managed entity.
    ControlEventTranslator<typename T::ActionType> _translator;

public:
    using ActionEventReceiverType = type_utils::apply_types<
        typename cpptools::ActionEventReceiver,
        type_utils::type_list<
            typename T::ActionType,
            typename T::ArgTypes
        >
    >;

    /// @tparam ArgTypes Types of the arguments to forward to the 
    /// constructor of the managed entity.
    ///
    /// @param args Arguments to forward to the constructor of the managed 
    /// entity.
    template<typename... ArgTypes>
    ControlledEntityManager(ArgTypes&&... args) :
        _entity(std::forward<ArgTypes>(args)...),
        _controlScheme(_entity.getDefaultControlScheme()),
        _translator(_controlScheme, (const ActionEventReceiverType&)(_entity))
    {
        
    }

    /// @brief Get a pointer to the managed entity.
    ///
    /// @return A pointer to the managed entity.
    const T& entity() const
    {
        return _entity;
    }

    /// @brief Get a pointer to the control scheme manager of the managed 
    /// entity.
    ///
    /// @return A pointer to the control scheme manager of the managed 
    /// entity.
    const ControlScheme<typename T::ActionType>& controlSchemeManager() const
    {
        return _controlScheme;
    }

    /// @brief Get a pointer to the control event translator of the managed
    /// entity.
    ///
    /// @return A pointer to the control event translator of the managed 
    /// entity.
    const ControlEventTranslator<typename T::ActionType>& eventTranslator() const
    {
        return _translator;
    }
};

} // namespace Renderboi

#endif//RENDERBOI__TOOLBOX__CONTROLS__CONTROLLED_ENTITY_MANAGER_HPP
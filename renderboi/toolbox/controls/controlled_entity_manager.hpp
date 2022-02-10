#ifndef RENDERBOI__TOOLBOX__CONTROLS__CONTROLLED_ENTITY_MANAGER_HPP
#define RENDERBOI__TOOLBOX__CONTROLS__CONTROLLED_ENTITY_MANAGER_HPP

#include <memory>
#include <type_traits>

#include <cpptools/utility/type_traits.hpp>
#include <cpptools/oo/interfaces/action_event_receiver.hpp>
#include <cpptools/oo/interfaces/argument_provider.hpp>

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
/// as from ActionEventReceiver<ActionType, ArgTypes...>. Leave all other types
/// for deductions.
template<
    class T,
    typename ActionEventReceiverType = typename type_utils::apply_types<
        typename cpptools::ActionEventReceiver,
        typename type_utils::make_type_list<
            typename T::ActionType,
            typename T::ArgTypes
        >::type
    >::type,
    typename ArgumentProviderType = typename type_utils::apply_types<
        typename cpptools::ArgumentProvider,
        typename T::ArgTypes
    >::type,
    typename ControlEventTranslatorType = typename type_utils::apply_types<
        ControlEventTranslator,
        typename type_utils::make_type_list<
            typename T::ActionType,
            typename T::ArgTypes
        >::type
    >::type,
    typename = std::enable_if_t<
        std::is_base_of_v<T,DefaultControlSchemeProvider<typename T::ActionType>>
    >,
    typename = std::enable_if_t<
        std::is_base_of_v<T, ActionEventReceiverType>
    >,
    typename = std::enable_if_t<
        ActionEventReceiverType::ActionHasArgs ||
        std::is_base_of_v<T, ArgumentProviderType>
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
    ControlEventTranslatorType _translator;

public:
    /// @tparam ArgTypes Types of the arguments to forward to the 
    /// constructor of the managed entity.
    ///
    /// @note Constructor variant for when the event requires no arguments.
    ///
    /// @param args Arguments to forward to the constructor of the managed 
    /// entity.
    template<
        typename... ArgTypes,
        std::enable_if_t<!ActionEventReceiverType::ActionHasArgs, void>
    >
    ControlledEntityManager(ArgTypes&&... args) :
        _entity(std::forward<ArgTypes>(args)...),
        _controlScheme(_entity.getDefaultControlScheme()),
        _translator(_controlScheme, (const ActionEventReceiverType&)(_entity))
    {
        
    }

    /// @tparam ArgTypes Types of the arguments to forward to the 
    /// constructor of the managed entity.
    ///
    /// @note Constructor variant for when the event requires arguments.
    ///
    /// @param args Arguments to forward to the constructor of the managed 
    /// entity.
    template<
        typename... ArgTypes,
        std::enable_if_t<ActionEventReceiverType::ActionHasArgs, void>
    >
    ControlledEntityManager(ArgTypes&&... args) :
        _entity(std::forward<ArgTypes>(args)...),
        _controlScheme(_entity.getDefaultControlScheme()),
        _translator(
            _controlScheme,
            (const ActionEventReceiverType&)(_entity),
            (const ArgumentProviderType&)(_entity)
        )
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
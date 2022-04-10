#ifndef RENDERBOI__TOOLBOX__CONTROLS__CONTROLLED_ENTITY_MANAGER_HPP
#define RENDERBOI__TOOLBOX__CONTROLS__CONTROLLED_ENTITY_MANAGER_HPP

#include <memory>
#include <type_traits>

#include <cpptools/utility/type_utils.hpp>
#include <cpptools/oo/interfaces/action_event_receiver.hpp>

#include "../interfaces/default_control_scheme_provider.hpp"
#include "control_scheme.hpp"
#include "control_event_translator.hpp"

namespace renderboi
{

/// @brief Eases out management of entities which provide a default control 
/// scheme.
///
/// @tparam T Required. Type of the entity to manage. It must have a public 
/// ActionType typedef, and inherit from DefaultControlSchemeProvider<ActionType>,
/// as well as from ActionEventReceiver<ActionType, ArgTypes...>. Leave all 
/// other types for deduction.
template<
    class T,
    typename ActionEventReceiverType = cpptools::ActionEventReceiver<typename T::ActionType>,
    typename ControlEventTranslatorType = ControlEventTranslator<typename T::ActionType>,
    typename = std::enable_if_t<
        std::is_base_of_v<DefaultControlSchemeProvider<typename T::ActionType>, T>, void
    >,
    typename = std::enable_if_t<
        std::is_base_of_v<ActionEventReceiverType, T>, void
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
    /// @note Constructor variant for when the event requires no arguments.
    ///
    /// @param args Arguments to forward to the constructor of the managed 
    /// entity.
    template<typename... ArgTypes>
    ControlledEntityManager(ArgTypes&&... args) :
        _entity(std::forward<ArgTypes>(args)...),
        _controlScheme(_entity.getDefaultControlScheme()),
        _translator(_controlScheme, static_cast<ActionEventReceiverType&>(_entity))
    {
        
    }

    /// @brief Get a pointer to the managed entity.
    ///
    /// @return A pointer to the managed entity.
    T& entity() const
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
    ControlEventTranslator<typename T::ActionType>& eventTranslator() const
    {
        return _translator;
    }
};

} // namespace renderboi

#endif//RENDERBOI__TOOLBOX__CONTROLS__CONTROLLED_ENTITY_MANAGER_HPP
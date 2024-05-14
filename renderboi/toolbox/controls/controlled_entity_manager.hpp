#ifndef RENDERBOI_TOOLBOX_CONTROLS_CONTROLLED_ENTITY_MANAGER_HPP
#define RENDERBOI_TOOLBOX_CONTROLS_CONTROLLED_ENTITY_MANAGER_HPP

#include <concepts>
#include <type_traits>

#include <renderboi/toolbox/interfaces/action_event_receiver.hpp>
#include <renderboi/toolbox/interfaces/default_control_scheme_provider.hpp>

#include "control_scheme.hpp"
#include "control_event_translator.hpp"

namespace rb {

template<typename T>
concept ControllableEntity = requires {
    std::is_enum_v<typename T::ActionType>;
    std::derived_from<T, DefaultControlSchemeProvider<typename T::ActionType>>;
    std::derived_from<T, ActionEventReceiver<typename T::ActionType>>;
};

/// @brief Eases out management of entities which provide a default control scheme
template<
    ControllableEntity T,
    typename ActionEventReceiverType    = ActionEventReceiver<typename T::ActionType>,
    typename ControlEventTranslatorType = ControlEventTranslator<typename T::ActionType>
>
class ControlledEntityManager {
private:
    using Action = typename T::ActionType;
    using EventReceiver = ActionEventReceiver<Action>;

    /// @brief Managed entity
    T _entity;

    /// @brief Scheme manager provided by the entity
    ControlScheme<Action> _controlScheme;

    /// @brief Object to translate events to actions and forward them to the
    /// managed entity
    ControlEventTranslatorType _translator;

public:
    /// @tparam ArgTypes Types of the arguments to forward to the 
    /// constructor of the managed entity
    /// @note Constructor variant for when the event requires no arguments
    /// @param args Arguments to forward to the constructor of the managed entity
    template<typename... ArgTypes>
    ControlledEntityManager(ArgTypes&&... args) :
        _entity(std::forward<ArgTypes>(args)...),
        _controlScheme(_entity.defaultControlScheme()),
        _translator(_controlScheme, static_cast<EventReceiver&>(_entity))
    {
        
    }

    /// @brief Get a pointer to the managed entity
    /// @return A pointer to the managed entity
    T& entity() {
        return _entity;
    }

    /// @brief Get a pointer to the control scheme manager of the managed entity
    /// @return A pointer to the control scheme manager of the managed entity
    const ControlScheme<Action>& controlSchemeManager() const {
        return _controlScheme;
    }

    /// @brief Get a pointer to the control event translator of the managed entity
    /// @return A pointer to the control event translator of the managed entity
    ControlEventTranslator<Action>& eventTranslator() {
        return _translator;
    }
};

} // namespace rb

#endif//RENDERBOI_TOOLBOX_CONTROLS_CONTROLLED_ENTITY_MANAGER_HPP
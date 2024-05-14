#ifndef RENDERBOI_TOOLBOX_CONTROLS_CONTROL_SCHEME_HPP
#define RENDERBOI_TOOLBOX_CONTROLS_CONTROL_SCHEME_HPP

#include <initializer_list>
#include <ranges>
#include <unordered_map>
#include <utility>

#include <cpptools/utility/concepts.hpp>
#include <cpptools/utility/unary.hpp>

#include "control.hpp"

namespace rb {

/// @brief Handles everything related to binding controls to actions 
/// @tparam A Enum representing the action to which a control can be bound 
template<tools::enumeration A>
class ControlScheme {
public:
    using ActionTable  = std::unordered_map<Control, A, ControlHash>;
    using ControlTable = std::unordered_map<A, Control>;
    using Binding      = ActionTable::value_type;

private:
    /// @brief Structure mapping actions to a control bound to them
    ActionTable _actions;

    /// @brief Structure mapping controls to the action they are bound to
    ControlTable _controls;

public:
    ControlScheme() = default;
    ControlScheme(std::initializer_list<std::pair<const Control, A>> bindings) :
        _actions(bindings),
        _controls(_actions | std::views::transform(tools::reverse_map_pair<const Binding&>) | std::ranges::to<ControlTable>())
    {

    }

    /// @brief Bind a control to an action
    /// @param control The control to bind
    /// @param action The action to bind the control to
    /// @note A control cannot be bound to several actions, binding an existing
    /// control to a different action will first remove the current binding.
    void bind(const Control& control, const A action) {
        _actions[action] = control;
        _controls[control] = action;
    }

    /// @brief Unbind a control in the control scheme
    /// @param control The control to unbind
    void unbind(const Control& control) {
        auto it = _controls.find(control);
        if (it != _controls.end()) {
            _actions.erase(it->second);
            _controls.erase(it);
        }
    }

    /// @brief Unbind all controls from a provided action in the control scheme
    /// @param action The action to unbind controls from
    void unbind(const A action) {
        auto it = _actions.find(action);
        if (it != _actions.end()) {
            _controls.erase(it->second);
            _actions.erase(it);
        }
    }

    /// @brief Tells whether or not a control is bound to an action in this 
    /// control scheme
    /// @param control The control to check
    /// @return Whether or not the provided control is bound
    bool isBound(const Control& control) const {
        auto it = _controls.find(control);
        return it != _controls.end();
    }

    /// @brief Get the action bound to a control
    /// @param control The control
    /// @return The action which the control is bound to
    A actionFor(const Control& control) const {
        return _controls[control];
    }

    /// @brief Get the map of actions bound per control
    /// @return The action table
    const ActionTable& actions() const {
        return _actions;
    }
};

} // namespace rb

#endif//RENDERBOI_TOOLBOX_CONTROLS_CONTROL_SCHEME_HPP
#ifndef RENDERBOI__TOOLBOX__CONTROLS__CONTROL_SCHEME_HPP
#define RENDERBOI__TOOLBOX__CONTROLS__CONTROL_SCHEME_HPP

#include <algorithm>
#include <functional>
#include <initializer_list>
#include <iterator>
#include <map>
#include <memory>
#include <stdexcept>
#include <unordered_map>
#include <utility>
#include <vector>

#include <cpptools/utility/map_tools.hpp>

#include "control.hpp"
#include "../interfaces/control_binding_provider.hpp"

namespace renderboi
{

/// @brief Given an enum of actions, this class allows easy management of
/// controls bound to these actions.
///
/// @tparam T Class representing the action to which a control can be bound. 
template<typename T>
class ControlScheme : public ControlBindingProvider<T>
{
protected:
    /// @brief Maximum number of controls bound to a single action in the
    /// ControlScheme.
    const unsigned int _MaxControlsPerAction;

    /// @brief Structure mapping controls to the actions they are bound to
    /// (several controls can be bound to a single action).
    std::multimap<T, Control> _controlsBoundToAction;

    /// @brief Structure mapping actions to a control bound to them (a 
    /// control can only be bound to one action in the same scheme).
    std::unordered_map<Control, T, ControlHash> _actionBoundToControl;

public:
    /// @brief The default maximum number of controls bound to a single
    /// action in the ControlScheme.
    static constexpr unsigned int DefaultMaxControlsPerAction = 4;

    /// @param maxControlsPerAction Maximum number of controls bound to a
    /// single action in the ControlScheme.
    ControlScheme(const unsigned int maxControlsPerAction = DefaultMaxControlsPerAction);

    /// @param bindings List of bindings to add to the control scheme.
    ControlScheme(std::initializer_list<std::pair<Control, T>> bindings);

    /// @brief Bind a control to an action. A control cannot be bound to
    /// several actions, and a binding of the provided control to another
    /// action will be removed if present. However, several controls can be
    /// bound to the same action, and bindings of other controls to the
    /// provided action will not be removed. If the provided control is
    /// already bound to the provided action, nothing happens.
    /// 
    /// @param control Structure of litterals describing the control to bind.
    /// @param action Object describing the action to bind the control to.
    ///
    /// @exception If binding the provided control to the provided action
    /// would exceed the max number of bindings per action as defined at 
    /// construction, then the function will throw an std::runtime_error.
    void bindControl(const Control control, const T action);

    /// @brief Unbind a provided control in the control scheme.
    ///
    /// @param control Structure of litterals describing the control to
    /// unbind.
    void unbindControl(const Control& control);

    /// @brief Unbind all controls from a provided action in the control
    /// scheme.
    ///
    /// @param action Object describing the action to unbind controls
    /// from.
    ///
    /// @return The amount of controls that were unbound.
    unsigned int unbindAllControlsFromAction(const T action);

    /////////////////////////////////////////////////////////
    ///                                                   ///
    /// Methods overridden from ControlBindingProvider<T> ///
    ///                                                   ///
    /////////////////////////////////////////////////////////

    /// @brief Tells whether or not a control is bound to an action in this 
    /// control scheme.
    ///
    /// @param control Structure of litterals describing the control whose 
    /// binding to check.
    ///
    /// @return Whether or not the provided control is bound.
    bool controlIsBound(const Control& control) const override;

    /// @brief Returns the action bound to the provided control.
    ///
    /// @param control Structure of litterals describing the control bound
    /// to the actions to be retrieved.
    ///
    /// @return The array of actions to which the provided control is bound.
    ///
    /// @exception If the provided control is not bound in this control 
    /// scheme, the function will throw an std::runtime_error.
    T getActionBoundToControl(const Control& control) const override;

    /// @brief Tells whether or not an action has a control bound to it in 
    /// this control scheme.
    ///
    /// @param action Object describing the action whose bindings to check.
    ///
    /// @return Whether or not the provided action has a binding.
    bool actionIsBound(const T action) const override;

    /// @brief Returns the array of controls which are bound to the provided
    /// action.
    ///
    /// @param action Object describing the action whose bound controls must
    /// be retrieved.
    ///
    /// @return The array of controls bound to the provided action.
    std::vector<Control> getControlsBoundToAction(const T action) const override;

    /// @brief Returns the array of all controls which are bound to an 
    /// action, paired with the action they are bound to.
    ///
    /// @return The array of all controls bound to an action.
    const std::multimap<T, Control>& getAllBoundControls() const override;

    /// @brief Returns the array of all controls which are bound to an 
    /// action, paired with the action they are bound to.
    ///
    /// @return The array of all controls bound to an action.
    const std::unordered_map<Control, T, ControlHash>& getAllBoundActions() const override;
};

template<typename T>
ControlScheme<T>::ControlScheme(unsigned int maxControlsPerAction) :
    _MaxControlsPerAction(maxControlsPerAction),
    _controlsBoundToAction(),
    _actionBoundToControl()
{
    
}

template<typename T>
ControlScheme<T>::ControlScheme(std::initializer_list<std::pair<Control, T>> bindings) :
    _MaxControlsPerAction(DefaultMaxControlsPerAction),
    _controlsBoundToAction(),
    _actionBoundToControl()
{
    for (const auto& b : bindings)
    {
        bindControl(b.first, b.second);
    }
}

template<typename T>
void ControlScheme<T>::bindControl(const Control control, const T action)
{
    const unsigned int actionBindingCount = (unsigned int)_controlsBoundToAction.count(action);
    const bool alreadyPresent = cpptools::mapContainsPair(_actionBoundToControl, {control, action});

    if (actionBindingCount >= _MaxControlsPerAction)
    {
        if (!alreadyPresent)
        {
            const std::string s = "ControlScheme: cannot bind control " + to_string(control)
                        + " to action " + to_string(action) + ", as it already has "
                        "the max number of bindings (" + std::to_string(_MaxControlsPerAction) + ").";

            throw std::runtime_error(s.c_str());
        }
        else return;
    }

    if (!alreadyPresent)
    {
        _actionBoundToControl.insert({control, action});
        _controlsBoundToAction.insert({action, control});
    }
}

template<typename T>
void ControlScheme<T>::unbindControl(const Control& control)
{
    auto it = _actionBoundToControl.find(control);

    if (it == _actionBoundToControl.end()) return;
    const T action = it->second;

    _actionBoundToControl.erase(control);

    using Iter = typename std::unordered_multimap<Control, T>::iterator;
    std::pair<Iter, Iter> range = _controlsBoundToAction.equal_range(action);
    for (Iter it = range.first; it != range.second; it++)
    {
        if (it->second == control)
        {
            _controlsBoundToAction.erase(it);
            return;
        }
    }
}

template<typename T>
unsigned int ControlScheme<T>::unbindAllControlsFromAction(const T action)
{
    if (_controlsBoundToAction.count(action) == 0) return 0;

    using Iter = typename std::unordered_multimap<T, Control>::iterator;
    std::pair<Iter, Iter> range = _controlsBoundToAction.equal_range(action);
    for (Iter it = range.first; it != range.second; it++)
    {
        _actionBoundToControl.erase(it->second);
    }

    return (unsigned int) _controlsBoundToAction.erase(action);
}

template<typename T>
bool ControlScheme<T>::controlIsBound(const Control& control) const
{
    return _actionBoundToControl.contains(control);
}

template<typename T>
T ControlScheme<T>::getActionBoundToControl(const Control& control) const
{
    if (!_actionBoundToControl.contains(control))
    {
        const std::string s = "ControlScheme: "
            "control " + to_string(control) + " is not bound, cannot retrieve action.";

        throw std::runtime_error(s.c_str());
    }

    return _actionBoundToControl.at(control);
}

template<typename T>
bool ControlScheme<T>::actionIsBound(const T action) const
{
    return _controlsBoundToAction.contains(action);
}

template<typename T>
std::vector<Control> ControlScheme<T>::getControlsBoundToAction(const T action) const
{
    auto range = _controlsBoundToAction.equal_range(action);

    std::function<Control(std::pair<const T, Control>)> controlFromPair = [](const std::pair<const T, Control>& p) -> Control
    {
        return p.second;
    };

    std::vector<Control> controls;
    std::transform(range.first, range.second, std::back_inserter(controls), controlFromPair);

    return controls;
}

template<typename T>
const std::multimap<T, Control>& ControlScheme<T>::getAllBoundControls() const
{
    return _controlsBoundToAction;
}

template<typename T>
const std::unordered_map<Control, T, ControlHash>& ControlScheme<T>::getAllBoundActions() const
{
    return _actionBoundToControl;
}

template<typename T>
using ControlSchemePtr = std::unique_ptr<ControlScheme<T>>;

} // namespace renderboi

#endif//RENDERBOI__TOOLBOX__CONTROLS__CONTROL_SCHEME_HPP
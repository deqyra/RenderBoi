#ifndef RENDERBOI__TOOLBOX__INTERFACES__CONTROL_BINDING_PROVIDER_HPP
#define RENDERBOI__TOOLBOX__INTERFACES__CONTROL_BINDING_PROVIDER_HPP

#include <map>
#include <memory>

#include "../controls/control.hpp"

namespace renderboi
{

/// @brief Interface for a class which provides bindings between controls
/// and an enum describing actions.
///
/// @tparam T Enum whose literal describe the actions to be taken.
template<typename T>
class ControlBindingProvider
{
public:
    /// @brief Tells whether or not a control is bound to an action in this 
    /// control scheme.
    ///
    /// @param control Structure of litterals describing the control whose 
    /// binding to check.
    ///
    /// @return Whether or not the provided control is bound.
    virtual bool controlIsBound(const Control& control) const = 0;

    /// @brief Returns the action bound to the provided control.
    ///
    /// @param control Structure of litterals describing the control bound
    /// to the actions to be retrieved.
    ///
    /// @return The array of actions to which the provided control is bound.
    ///
    /// @exception If the provided control is not bound in this control
    /// scheme, depending on how the concrete function is implemented, it 
    /// may throw an std::runtime_error.
    virtual T getActionBoundToControl(const Control& control) const = 0;

    /// @brief Tells whether or not an action has a control bound to it in 
    /// this control scheme.
    ///
    /// @param action Object describing the action whose bindings to check.
    ///
    /// @return Whether or not the provided action has a binding.
    virtual bool actionIsBound(const T action) const = 0;

    /// @brief Returns the array of controls which are bound to the provided
    /// action.
    ///
    /// @param action Object describing the action whose bound controls must
    /// be retrieved.
    ///
    /// @return The array of controls bound to the provided action.
    virtual std::vector<Control> getControlsBoundToAction(const T action) const = 0;

    /// @brief Returns a container of all controls which are bound to an 
    /// action, paired with the action they are bound to.
    ///
    /// @return A pointer to the container of all controls bound to an action.
    virtual const std::multimap<T, Control>& getAllBoundControls() const = 0;

    /// @brief Returns a container of all actions to which a control is bound,
    /// paired with the bound control.
    ///
    /// @return A pointer to the container of all actions with a bound control.
    virtual const std::unordered_map<Control, T, ControlHash>& getAllBoundActions() const = 0;
};

template<typename T>
using ControlBindingProviderPtr = std::unique_ptr<ControlBindingProvider<T>>;

} // namespace renderboi

#endif//RENDERBOI__TOOLBOX__INTERFACES__CONTROL_BINDING_PROVIDER_HPP
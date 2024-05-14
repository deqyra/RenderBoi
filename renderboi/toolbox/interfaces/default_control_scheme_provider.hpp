#ifndef RENDERBOI_TOOLBOX_INTERFACES_DEFAULT_CONTROL_SCHEME_PROVIDER_HPP
#define RENDERBOI_TOOLBOX_INTERFACES_DEFAULT_CONTROL_SCHEME_PROVIDER_HPP

#include <renderboi/toolbox/controls/control_scheme.hpp>

namespace rb {

template<typename T>
class DefaultControlSchemeProvider {
public:
    /// @brief Get the default control scheme for the implementing entity
    /// @return The default control scheme for the implementing entity
    virtual const ControlScheme<T>& defaultControlScheme() const = 0;
};

} // namespace rb

#endif//RENDERBOI_TOOLBOX_INTERFACES_DEFAULT_CONTROL_SCHEME_PROVIDER_HPP
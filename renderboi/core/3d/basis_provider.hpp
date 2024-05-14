#ifndef RENDERBOI_CORE_3D_BASIS_PROVIDER_HPP
#define RENDERBOI_CORE_3D_BASIS_PROVIDER_HPP

#include <renderboi/core/3d/basis.hpp>

namespace rb {

/// @brief Interface for a class able to provide basis vectors
class BasisProvider {
public:
    /// @brief Get the basis vectors
    /// @return The basis vectors
    virtual Basis basis() const = 0;
};

} // namespace rb

#endif//RENDERBOI_CORE_3D_BASIS_PROVIDER_HPP
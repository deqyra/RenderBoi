#ifndef RENDERBOI_CORE_3D_BASIS_HPP
#define RENDERBOI_CORE_3D_BASIS_HPP

#include <renderboi/core/numeric.hpp>

namespace rb {

/// @brief Describes the coordinates of local vectors in another basis
struct Basis {
    num::Vec3 x = num::X;
    num::Vec3 y = num::Y;
    num::Vec3 z = num::Z;
};

/// @brief Transform a set of coordinates expressed in a given basis into global
/// coordinates
/// @param position The vector to transform
/// @param basis The basis in which the vector is expressed
/// @return The vector expressed in global coordinates
num::Vec3 transform(const num::Vec3& position, const Basis& basis);

/// @brief Get the inverse of a basis. For the basis B whose vectors are expressed
/// in global coordinates and a vector V expressed in global coordinates, B' is
/// the inverse basis of B if changing V' to basis B' yields V back, where V' is
/// the result of changing V to basis B.
Basis inverse(const Basis& b);

} // namespace rb

#endif//RENDERBOI_CORE_3D_BASIS_HPP
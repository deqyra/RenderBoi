#ifndef RENDERBOI_CORE_3D_TRANSFORM_HPP
#define RENDERBOI_CORE_3D_TRANSFORM_HPP

#include "../numeric.hpp"
#include "basis.hpp"

namespace rb {

/// @brief Represents the 3D-space properties of an object: position, orientation
/// and scale
struct RawTransform {
    /// @brief Quaternion representing the rotation of the object
    num::Quat orientation = num::Identity;

    /// @brief 3D position of the object
    num::Vec3 position = num::Origin3;

    /// @brief 3D scale of the object
    num::Vec3 scale = num::XYZ;
};

/// @brief Compute the local basis vectors of a transform, in world coordinates
/// @param t The transform whose local vectors should be computed
/// @return The local vectors
Basis basisOf(const RawTransform& t);

/// @brief Apply a transform on a position vector
/// @param position The position to be transformed
/// @param transform The transform to apply
/// @return The transformed position
num::Vec3 transformPosition(const num::Vec3& position, const RawTransform& t);

/// @brief Apply a transform on a rotation
/// @param rotation The rotation to be transformed
/// @param transform The transform to apply
/// @return The transformed rotation
num::Quat transformRotation(const num::Quat& rotation, const RawTransform& t);

/// @brief Apply a transform on a scale
/// @param scale The scale to be transformed
/// @param transform The transform to apply
/// @return The transformed scale
num::Vec3 transformScale(const num::Vec3& scale, const RawTransform& t);

/// @brief Compute the inverse transform of a transform
/// @param transform The transform to compute the inverse of
/// @return The inverse transform
RawTransform inverse(const RawTransform& t);

/// @brief Apply a child transform on top of a parent one
/// @param left The parent transform
/// @param right The child transform
/// @return The resulting transform
RawTransform operator*(const RawTransform& left, const RawTransform& right);

/// @brief Get the model matrix for a transform
/// @param transform The transform to make a model matrix out of
/// @return The model matrix
num::Mat4 toModelMatrix(const RawTransform& t);

} // namespace rb

#endif//RENDERBOI_CORE_3D_TRANSFORM_HPP
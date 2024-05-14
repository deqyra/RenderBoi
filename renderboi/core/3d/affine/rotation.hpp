#ifndef RENDERBOI_CORE_3D_AFFINE_ROTATION_HPP
#define RENDERBOI_CORE_3D_AFFINE_ROTATION_HPP

#include <renderboi/core/3d/transform.hpp>
#include <renderboi/core/3d/affine/affine_operation.hpp>

namespace rb::affine {

/// @brief Rotate a transform
/// @param t Transform to rotate
/// @param rotation Rotation to apply to the transform
void rotate(RawTransform& t, num::Quat rotation);

/// @brief Rotate a transform
/// @param t Transform to rotate
/// @param radAngle Angle in radiants by which the transform should be rotated
/// @param axis Axis about which the transform should be rotated
void rotate(RawTransform& t, float radAngle, const num::Vec3& axis);

/// @brief Represents a 3D rotation
class Rotation {
public:
    Rotation();
    Rotation(const num::Quat& rotation);
    Rotation(float radAngle, num::Vec3 axis);

    Rotation(const Rotation&) = default;
    Rotation(Rotation&&)      = default;

    Rotation& operator=(const Rotation&) = default;
    Rotation& operator=(Rotation&&)      = default;

    /// @brief Apply the rotation represented by this object to a transform
    /// @param t Transform to apply the rotation to
    void apply(RawTransform& t) const;

private:
    /// @brief Rotation quaternion represented by this object
    num::Quat _rotation;
};

static_assert(AffineOperation<Rotation>);

} // namespace rb::affine

#endif//RENDERBOI_CORE_3D_AFFINE_ROTATION_HPP

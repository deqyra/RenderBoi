#ifndef RENDERBOI_CORE_3D_AFFINE_LOOK_AT_HPP
#define RENDERBOI_CORE_3D_AFFINE_LOOK_AT_HPP

#include <renderboi/core/3d/transform.hpp>
#include <renderboi/core/3d/affine/affine_operation.hpp>

namespace rb::affine {

/// @brief Rotate a transform so that its front (local Z axis) is directed towards the target position
/// @param t Transform to rotate
/// @param target Position towards which the transform's Z axis should be oriented
/// @param up Vector which the Z and Y axes of the transform must be coplanar with after rotation
void turn(RawTransform& t, const num::Vec3& target, const num::Vec3& up);

/// @brief Represents a 3D rotation towards a fixed position
class Turn {
public:
    Turn();
    Turn(num::Vec3 target, num::Vec3 up);

    Turn(const Turn&) = default;
    Turn(Turn&&)      = default;

    Turn& operator=(const Turn&) = default;
    Turn& operator=(Turn&&)      = default;

    /// @brief Apply the turn represented by this object to a transform
    /// @param t Transform to apply the turn to
    void apply(RawTransform& t) const;

private:
    /// @brief Target which the turn operation represented by this object should rotate transforms towards
    num::Vec3 _target;
    /// @brief Vector which the Z and Y axes of transforms should be coplanar with after rotation
    num::Vec3 _up;
};

static_assert(AffineOperation<Turn>);

} // namespace rb::affine

#endif//RENDERBOI_CORE_3D_AFFINE_LOOK_AT_HPP
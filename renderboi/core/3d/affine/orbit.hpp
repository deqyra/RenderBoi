#ifndef RENDERBOI_CORE_3D_AFFINE_ORBIT_HPP
#define RENDERBOI_CORE_3D_AFFINE_ORBIT_HPP

#include <renderboi/core/3d/transform.hpp>
#include <renderboi/core/3d/affine/affine_operation.hpp>

namespace rb::affine {

/// @brief Orbit a transform around an axis and center
/// @param t Transform to orbit
/// @param rotation Quaternion encoding the rotation to perform
/// @param center Point around which the object should orbit
/// @param selfRotate Whether the object should rotate on itself
void orbit(RawTransform& t, const num::Quat& rotation, const num::Vec3& center, bool rotateSelf);

/// @brief Orbit a transform around an axis and center
/// @param t Transform to orbit
/// @param radAngle Angle by which the object should orbit
/// @param axis Axis about which the object should orbit
/// @param center Point around which the object should orbit
/// @param selfRotate Whether the object should rotate on itself
void orbit(RawTransform& t, float radAngle, const num::Vec3& axis, const num::Vec3& center, bool rotateSelf);

/// @brief Represents an orbital movement in 3D
class Orbit {
public:
    Orbit();
    Orbit(const num::Quat& rotation, num::Vec3 center, bool rotateInPlace);
    Orbit(float radAngle, const num::Vec3& axis, num::Vec3 center, bool rotateInPlace);

    Orbit(const Orbit&) = default;
    Orbit(Orbit&&)      = default;

    Orbit& operator=(const Orbit&) = default;
    Orbit& operator=(Orbit&&)      = default;

    /// @brief Apply the orbit represented by this object to a transform
    /// @param t Transform to apply the orbit to
    void apply(RawTransform& t) const;

private:
    /// @brief Rotation component of the orbit represented by this object
    num::Quat _rotation;
    /// @brief Center point around which the orbit represented by this object takes place
    num::Vec3 _center;
    /// @brief Whether or not the transform should be rotating around itself in addition to orbiting around the center
    bool _rotateInPlace;
};

static_assert(AffineOperation<Orbit>);

} // namespace rb::affine

#endif//RENDERBOI_CORE_3D_AFFINE_ORBIT_HPP
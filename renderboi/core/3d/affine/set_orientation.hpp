#ifndef RENDERBOI_CORE_3D_AFFINE_ORIENTATION_HPP
#define RENDERBOI_CORE_3D_AFFINE_ORIENTATION_HPP

#include <renderboi/core/3d/transform.hpp>
#include <renderboi/core/3d/affine/affine_operation.hpp>

namespace rb::affine {

/// @brief Represents a 3D orientation
class SetOrientation {
public:
    SetOrientation();
    SetOrientation(num::Vec3 orientation);

    SetOrientation(const SetOrientation&) = default;
    SetOrientation(SetOrientation&&)      = default;

    SetOrientation& operator=(const SetOrientation&) = default;
    SetOrientation& operator=(SetOrientation&&)      = default;

    /// @brief Apply the orientation represented by this object to a transform
    /// @param t Transform to apply the orientation to
    void apply(RawTransform& t) const;

private:
    /// @brief Orientation vector represented by this object
    num::Vec3 _orientation;
};

static_assert(AffineOperation<SetOrientation>);

} // namespace rb::affine

#endif//RENDERBOI_CORE_3D_AFFINE_ORIENTATION_HPP
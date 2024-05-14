#ifndef RENDERBOI_CORE_3D_AFFINE_POSITION_HPP
#define RENDERBOI_CORE_3D_AFFINE_POSITION_HPP

#include <renderboi/core/3d/transform.hpp>
#include <renderboi/core/3d/affine/affine_operation.hpp>

namespace rb::affine {

/// @brief Represents a 3D position
class SetPosition {
public:
    SetPosition();
    SetPosition(num::Vec3 position);

    SetPosition(const SetPosition&) = default;
    SetPosition(SetPosition&&)      = default;

    SetPosition& operator=(const SetPosition&) = default;
    SetPosition& operator=(SetPosition&&)      = default;

    /// @brief Apply the position represented by this object to a transform
    /// @param t Transform to apply the position to
    void apply(RawTransform& t) const;

private:
    /// @brief Position vector represented by this object
    num::Vec3 _position;
};

static_assert(AffineOperation<SetPosition>);

} // namespace rb::affine

#endif//RENDERBOI_CORE_3D_AFFINE_POSITION_HPP
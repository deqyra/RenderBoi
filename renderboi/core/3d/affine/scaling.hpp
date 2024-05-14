#ifndef RENDERBOI_CORE_3D_AFFINE_SCALING_HPP
#define RENDERBOI_CORE_3D_AFFINE_SCALING_HPP

#include <renderboi/core/3d/transform.hpp>
#include <renderboi/core/3d/affine/affine_operation.hpp>

namespace rb::affine {

/// @brief Scale a transform
/// @param t Transform to scale
/// @param scaling Scaling to apply to the transform
void scale(RawTransform& t, num::Vec3 scaling);

/// @brief Represents a 3D scaling
class Scaling {
public:
    Scaling();
    Scaling(num::Vec3 scaling);

    Scaling(const Scaling&) = default;
    Scaling(Scaling&&)      = default;

    Scaling& operator=(const Scaling&) = default;
    Scaling& operator=(Scaling&&)      = default;

    /// @brief Apply the scaling represented by this object to a transform
    /// @param t Transform to apply the scaling to
    void apply(RawTransform& t) const;

    private:
    /// @brief Scaling vector represented by this object
    num::Vec3 _scaling;
};

static_assert(AffineOperation<Scaling>);

} // namespace rb::affine

#endif//RENDERBOI_CORE_3D_AFFINE_SCALING_HPP
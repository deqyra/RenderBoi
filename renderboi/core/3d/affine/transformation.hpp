#ifndef RENDERBOI_CORE_3D_AFFINE_TRANSFORMATION_HPP
#define RENDERBOI_CORE_3D_AFFINE_TRANSFORMATION_HPP

#include <renderboi/core/3d/transform.hpp>
#include <renderboi/core/3d/affine/affine_operation.hpp>

namespace rb::affine {

/// @brief Apply a transform on a transform
/// @param t The transform to transform
/// @param toApply The transform to apply
void transform(RawTransform& t, const RawTransform& toApply);

/// @brief Represents an affine transformation combining translation, rotation and scaling
class Transformation {
public:
    Transformation();
    Transformation(RawTransform t);
    Transformation(num::Vec3 translation, const num::Quat& rotation, num::Vec3 scaling);

    Transformation(const Transformation&) = default;
    Transformation(Transformation&&)      = default;

    Transformation& operator=(const Transformation&) = default;
    Transformation& operator=(Transformation&&)      = default;

    /// @brief Apply the Transformation represented by this object to a transform
    /// @param t Transform to apply the Transformation to
    void apply(RawTransform& t) const;

private:
    /// @brief Rotation component of the Transformation represented by this object
    RawTransform _transform;
};

static_assert(AffineOperation<Transformation>);

} // namespace rb::affine

#endif//RENDERBOI_CORE_3D_AFFINE_TRANSFORMATION_HPP
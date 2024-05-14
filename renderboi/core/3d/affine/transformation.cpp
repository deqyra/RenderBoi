#include "transformation.hpp"

namespace rb::affine {

void transform(RawTransform& t, const RawTransform& toApply) {
    t = toApply * t;
}

Transformation::Transformation()
    : Transformation(num::Origin3, num::Identity, num::XYZ)
{

}

Transformation::Transformation(RawTransform t)
    : _transform(std::move(t))
{

}

Transformation::Transformation(num::Vec3 translation, const num::Quat& rotation, num::Vec3 scaling)
    : _transform({
          .orientation = normalize(rotation),
          .position    = std::move(translation),
          .scale       = std::move(scaling)
    })
{
    
}

void Transformation::apply(RawTransform& t) const {
    transform(t, _transform);
}

} // namespace rb::affine
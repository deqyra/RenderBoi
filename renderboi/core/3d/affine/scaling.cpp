#include "scaling.hpp"

namespace rb::affine {

void scale(RawTransform& t, num::Vec3 scaling) {
    t.scale.x *= scaling.x;
    t.scale.y *= scaling.y;
    t.scale.z *= scaling.z;
}

Scaling::Scaling()
    : Scaling({0.f, 0.f, 0.f})
{

}

Scaling::Scaling(num::Vec3 scaling)
    : _scaling(std::move(scaling))
{

}

void Scaling::apply(RawTransform& t) const {
    scale(t, _scaling);
}

} // namespace rb::affine
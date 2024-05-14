#include "set_orientation.hpp"

namespace rb::affine {

SetOrientation::SetOrientation()
    : SetOrientation({0.f, 0.f, 0.f})
{

}

SetOrientation::SetOrientation(num::Vec3 orientation)
    : _orientation(std::move(orientation))
{

}

void SetOrientation::apply(RawTransform& t) const {
    t.orientation = _orientation;
}

} // namespace rb::affine
#include "set_position.hpp"

namespace rb::affine {

SetPosition::SetPosition()
    : SetPosition({0.f, 0.f, 0.f})
{

}

SetPosition::SetPosition(num::Vec3 position)
    : _position(std::move(position))
{

}

void SetPosition::apply(RawTransform& t) const {
    t.position = _position;
}

} // namespace rb::affine
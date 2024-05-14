#include "turn.hpp"
#include "rotation.hpp"

namespace rb::affine {

void turn(RawTransform& t, const num::Vec3& target, const num::Vec3& up) {
    num::Vec3 direction = normalize(target - t.position);
    num::Quat rotation  = quatLookAt(direction, up);
    rotate(t, rotation);
}

Turn::Turn()
    : Turn(num::Origin3, num::Y)
{

}

Turn::Turn(num::Vec3 target, num::Vec3 up)
    : _target(std::move(target))
    , _up(std::move(up))
{

}

void Turn::apply(RawTransform& t) const {
    turn(t, _target, _up);
}

} // namespace rb::affine
#include "rotation.hpp"

namespace rb::affine {

void rotate(RawTransform& t, num::Quat rotation) {
    // Note so I can easily be convinced, after second-guessing myself 10 times
    // with this, that YES, this is the correct order to multiply quaternions:
    //
    // The object's current orientation is t.orientation, and we want to rotate it
    // further, so the desired rotation needs to be stacked on top of the current one.
    // It must logically apply AFTER the current rotation, so it must appear as the
    // LEFTMOST term of the multiplication.
    t.orientation = rotation * t.orientation;
}

void rotate(RawTransform& t, float radAngle, const num::Vec3& axis) {
    num::Quat rotation = normalize(angleAxis(radAngle, axis));
    rotate(t, rotation);
}

Rotation::Rotation()
    : Rotation({0.f, 0.f, 0.f, 1.f})
{

}

Rotation::Rotation(const num::Quat& rotation)
    : _rotation(normalize(rotation))
{

}

Rotation::Rotation(float radAngle, num::Vec3 axis)
    : Rotation(angleAxis(radAngle, axis))
{
    
}

void Rotation::apply(RawTransform& t) const {
    rotate(t, _rotation);
}

} // namespace rb::affine
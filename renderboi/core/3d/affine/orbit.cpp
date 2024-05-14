#include "orbit.hpp"
#include "rotation.hpp"

namespace rb::affine {

void orbit(RawTransform& t, const num::Quat& rotation, const num::Vec3& center, bool rotateSelf) {
    num::Vec3 positionOffset = t.position - center;
    t.position = (rotation * positionOffset) + center;

    if (rotateSelf) {
        rotate(t, rotation);
    }
}

void orbit(RawTransform& t, float radAngle, const num::Vec3& axis, const num::Vec3& center, bool rotateSelf) {
    num::Quat rotation = normalize(angleAxis(radAngle, axis));
    orbit(t, rotation, center, rotateSelf);
}

Orbit::Orbit()
    : Orbit({0.f, 0.f, 0.f, 1.f}, {0.f, 0.f, 0.f}, false)
{

}

Orbit::Orbit(const num::Quat& rotation, num::Vec3 center, bool rotateInPlace)
    : _rotation(normalize(rotation))
    , _center(std::move(center))
    , _rotateInPlace(rotateInPlace)
{

}

Orbit::Orbit(float radAngle, const num::Vec3& axis, num::Vec3 center, bool rotateInPlace)
    : Orbit(angleAxis(radAngle, axis), std::move(center), rotateInPlace)
{
    
}

void Orbit::apply(RawTransform& t) const {
    orbit(t, _rotation, _center, _rotateInPlace);
}

} // namespace rb::affine
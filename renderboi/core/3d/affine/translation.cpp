#include "translation.hpp"

namespace rb::affine {

void translate(RawTransform& t, num::Vec3 translation) {
    t.position += translation;
}

Translation::Translation()
    : Translation({0.f, 0.f, 0.f})
{

}

Translation::Translation(num::Vec3 translation)
    : _translation(std::move(translation))
{

}

void Translation::apply(RawTransform& t) const {
    translate(t, _translation);
}

} // namespace rb::affine
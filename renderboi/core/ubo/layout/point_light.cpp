#include "point_light.hpp"
#include "renderboi/core/lights/point_light.hpp"

namespace rb {

UBOLayout<PointLight>::UBOLayout(const num::Vec3& position, const PointLight& pointLight) :
    position(position),
    _padding1(),
    ambient(pointLight.color.ambient),
    constant(pointLight.attenuation.constant),
    diffuse(pointLight.color.diffuse),
    linear(pointLight.attenuation.linear),
    specular(pointLight.color.specular),
    quadratic(pointLight.attenuation.quadratic)
{

}

const void* UBOLayout<PointLight>::data() {
    return this;
}

} // namespace rb

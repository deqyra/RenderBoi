#include "spot_light.hpp"
#include "renderboi/core/lights/spot_light.hpp"

namespace rb {

UBOLayout<SpotLight>::UBOLayout(const num::Vec3& position, const SpotLight& spotLight) :
    position(position),
    innerCutoff(spotLight.innerCutoff),
    ambient(spotLight.color.ambient),
    constant(spotLight.attenuation.constant),
    diffuse(spotLight.color.diffuse),
    linear(spotLight.attenuation.linear),
    specular(spotLight.color.specular),
    quadratic(spotLight.attenuation.quadratic)
{

}

const void* UBOLayout<SpotLight>::data() {
    return this;
}

} // namespace rb

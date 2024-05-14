#include "directional_light.hpp"
#include "renderboi/core/lights/directional_light.hpp"

namespace rb {

UBOLayout<DirectionalLight>::UBOLayout(const DirectionalLight& dirLight) :
    direction(dirLight.direction),
    _padding1(),
    ambient(dirLight.color.ambient),
    _padding2(),
    diffuse(dirLight.color.diffuse),
    _padding3(),
    specular(dirLight.color.specular),
    _padding4()
{

}

const void* UBOLayout<DirectionalLight>::data() {
    return this;
}

} // namespace rb

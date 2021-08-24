#include "directional_light.hpp"

namespace Renderboi
{

DirectionalLight::DirectionalLight(const glm::vec3 direction) :
    DirectionalLight(direction, glm::vec3(1.f), glm::vec3(1.f), glm::vec3(1.f))
{

}

DirectionalLight::DirectionalLight(
    const glm::vec3 direction,
    const glm::vec3 ambient,
    const glm::vec3 diffuse,
    const glm::vec3 specular
) :
    Light(LightType::DirectionalLight),
    direction(direction),
    ambient(ambient),
    diffuse(diffuse),
    specular(specular)
{

}

DirectionalLight* DirectionalLight::clone() const
{
    return new DirectionalLight(direction, ambient, diffuse, specular);
}

glm::mat4 DirectionalLight::_computeProjectionMatrix() const
{
    return glm::mat4();
}

glm::mat4 DirectionalLight::_computeViewMatrix() const
{
    return glm::mat4();
}

}//namespace Renderboi

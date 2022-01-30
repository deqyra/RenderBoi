#include "light.hpp"

namespace renderboi
{

unsigned int Light::_count = 0;

Light::Light(const LightType type) :
    ParentDependentVPMatrixProvider(Transform()),
    id(_count++),
    lightType(type)
{

}

} // namespace renderboi

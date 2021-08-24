#include "light.hpp"

namespace Renderboi
{

unsigned int Light::_count = 0;

Light::Light(const LightType type) :
    ParentDependentVPMatrixProvider(Transform()),
    id(_count++),
    lightType(type)
{

}

}//namespace Renderboi

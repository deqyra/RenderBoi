#ifndef RENDERBOI_TOOLBOX_SCENE_COMPONENTS_POINT_LIGHT_COMPONENT
#define RENDERBOI_TOOLBOX_SCENE_COMPONENTS_POINT_LIGHT_COMPONENT

#include "basic_component.hpp"

#include <renderboi/core/lights/point_light.hpp>

namespace rb {

struct PointLightComponent : BasicComponent<PointLight*> {};

} // namespace rb

#endif//RENDERBOI_TOOLBOX_SCENE_COMPONENTS_POINT_LIGHT_COMPONENT
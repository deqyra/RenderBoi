#ifndef RENDERBOI_TOOLBOX_SCENE_COMPONENTS_SPOT_LIGHT_COMPONENT
#define RENDERBOI_TOOLBOX_SCENE_COMPONENTS_SPOT_LIGHT_COMPONENT

#include "basic_component.hpp"

#include <renderboi/core/lights/spot_light.hpp>

namespace rb {

struct SpotLightComponent : BasicComponent<SpotLight*> {};

} // namespace rb

#endif//RENDERBOI_TOOLBOX_SCENE_COMPONENTS_SPOT_LIGHT_COMPONENT
#ifndef RENDERBOI_TOOLBOX_SCENE_COMPONENTS_DIRECTIONAL_LIGHT_COMPONENT
#define RENDERBOI_TOOLBOX_SCENE_COMPONENTS_DIRECTIONAL_LIGHT_COMPONENT

#include "basic_component.hpp"

#include <renderboi/core/lights/directional_light.hpp>

namespace rb {

struct DirectionalLightComponent : BasicComponent<DirectionalLight*> {};

} // namespace rb

#endif//RENDERBOI_TOOLBOX_SCENE_COMPONENTS_DIRECTIONAL_LIGHT_COMPONENT
#ifndef RENDERBOI_TOOLBOX_SCENE_COMPONENTS_CAMERA_COMPONENT_HPP
#define RENDERBOI_TOOLBOX_SCENE_COMPONENTS_CAMERA_COMPONENT_HPP

#include "basic_component.hpp"

#include <renderboi/core/3d/camera.hpp>

namespace rb {

struct CameraComponent : BasicComponent<Camera*> {};

} // namespace rb

#endif//RENDERBOI_TOOLBOX_SCENE_COMPONENTS_CAMERA_COMPONENT_HPP
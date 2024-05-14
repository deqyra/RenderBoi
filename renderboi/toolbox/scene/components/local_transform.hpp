#ifndef RENDERBOI_TOOLBOX_SCENE_COMPONENTS_LOCAL_TRANSFORM_HPP
#define RENDERBOI_TOOLBOX_SCENE_COMPONENTS_LOCAL_TRANSFORM_HPP

#include "basic_component.hpp"

#include <renderboi/core/3d/transform.hpp>

namespace rb {

struct LocalTransform : public BasicComponent<RawTransform> {};

} // namespace rb

#endif//RENDERBOI_TOOLBOX_SCENE_COMPONENTS_LOCAL_TRANSFORM_HPP
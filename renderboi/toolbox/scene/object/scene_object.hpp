#ifndef RENDERBOI__TOOLBOX__SCENE__OBJECT__SCENE_OBJECT_HPP
#define RENDERBOI__TOOLBOX__SCENE__OBJECT__SCENE_OBJECT_HPP

#include <string>

#include <entt/entt.hpp>

namespace renderboi
{

using SceneObject = entt::entity;

static_assert(
    std::is_integral_v<SceneObject> || std::is_enum_v<SceneObject>,
    "SceneObject must have integral or enum type"
);

} // namespace renderboi

#endif//RENDERBOI__TOOLBOX__SCENE__OBJECT__SCENE_OBJECT_HPP
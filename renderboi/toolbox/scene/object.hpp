#ifndef RENDERBOI_TOOLBOX_SCENE_OBJECT_OBJECT_HPP
#define RENDERBOI_TOOLBOX_SCENE_OBJECT_OBJECT_HPP

#include <entt/entt.hpp>

namespace rb {

using Object         = entt::entity;
using ObjectRegistry = entt::basic_registry<Object>;
using ObjectHandle   = entt::basic_handle<ObjectRegistry>;

constexpr Object NullObject = entt::null;

} // namespace rb

#endif//RENDERBOI_TOOLBOX_SCENE_OBJECT_OBJECT_HPP
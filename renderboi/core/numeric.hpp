#ifndef RENDERBOI_CORE_3D_NUMERIC_HPP
#define RENDERBOI_CORE_3D_NUMERIC_HPP

#include <numbers>

#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtx/quaternion.hpp>

namespace rb::num {

using Vec2   = glm::vec2;
using Vec2ui = glm::vec<2, unsigned int>;
using Vec2b  = glm::vec<2, bool>;
using Vec3   = glm::vec3;
using Vec4   = glm::vec4;
using Quat   = glm::quat;
using Mat3   = glm::mat3;
using Mat4   = glm::mat4;
using Mat3x4 = glm::mat3x4;

inline constexpr num::Vec2  Origin2 = num::Vec2{ 0.f, 0.f };
inline constexpr num::Vec3  Origin3 = num::Vec3{ 0.f, 0.f, 0.f };
inline constexpr num::Vec3        X = num::Vec3{ 1.f, 0.f, 0.f };
inline constexpr num::Vec3        Y = num::Vec3{ 0.f, 1.f, 0.f };
inline constexpr num::Vec3        Z = num::Vec3{ 0.f, 0.f, 1.f };
inline constexpr num::Vec3      XYZ = num::Vec3{ 1.f, 1.f, 1.f };
inline constexpr num::Quat Identity = num::Quat{ 0.f, 0.f, 0.f, 1.f };

inline constexpr double Sqrt2 = std::numbers::sqrt2;
inline constexpr double Sqrt3 = std::numbers::sqrt3;
inline constexpr double Pi    = std::numbers::pi;

using glm::abs;
using glm::acos;
using glm::angleAxis;
using glm::fclamp;
using glm::cos;
using glm::cross;
using glm::dot;
using glm::inverse;
using glm::lookAt;
using glm::normalize;
using glm::perspective;
using glm::radians;
using glm::sin;
using glm::sqrt;
using glm::transpose;

} // namespace rb::num

#endif//RENDERBOI_CORE_3D_NUMERIC_HPP
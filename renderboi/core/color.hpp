#ifndef RENDERBOI_CORE_COLOR_HPP
#define RENDERBOI_CORE_COLOR_HPP

#include <renderboi/core/numeric.hpp>

namespace rb {

using Color = num::Vec3;

namespace color {

static constexpr Color Black   = { 0.f, 0.f, 0.f };
static constexpr Color White   = { 1.f, 1.f, 1.f };
static constexpr Color Red     = { 1.f, 0.f, 0.f };
static constexpr Color Green   = { 0.f, 1.f, 0.f };
static constexpr Color Blue    = { 0.f, 0.f, 1.f };
static constexpr Color Cyan    = { 0.f, 1.f, 1.f };
static constexpr Color Magenta = { 1.f, 0.f, 1.f };
static constexpr Color Yellow  = { 1.f, 1.f, 0.f };

} // namespace color

} // namespace rb

#endif//RENDERBOI_CORE_COLOR_HPP
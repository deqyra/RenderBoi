#ifndef RENDERBOI_CORE_3D_AFFINE_AFFINE_OPERATION_HPP
#define RENDERBOI_CORE_3D_AFFINE_AFFINE_OPERATION_HPP

#include <concepts>

#include <renderboi/core/3d/transform.hpp>

namespace rb::affine {

template<typename A>
concept AffineOperation = requires(const A& op, RawTransform& t) {
    { op.apply(t) } -> std::same_as<void>;
};

} // namespace rb::affine

#endif//RENDERBOI_CORE_3D_AFFINE_AFFINE_OPERATION_HPP
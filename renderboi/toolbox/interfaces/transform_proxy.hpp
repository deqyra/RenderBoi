#ifndef RENDERBOI_TOOLBOX_INTERFACES_TRANSFORM_PROXY_HPP
#define RENDERBOI_TOOLBOX_INTERFACES_TRANSFORM_PROXY_HPP

#include <renderboi/core/3d/transform.hpp>
#include <renderboi/core/3d/affine/affine_operation.hpp>

namespace rb {

namespace detail {

struct DummyOp {
    DummyOp() = default;
    void apply(RawTransform&) const {};
};

static_assert(affine::AffineOperation<DummyOp>);

}

template<typename P>
concept TransformProxy = requires (P& proxy, P&& rvalue_proxy) {
    {                  proxy  << detail::DummyOp{} } -> std::same_as<P&>;  // enforce that you can apply any AffineOperation on the proxy
    { std::move(rvalue_proxy) << detail::DummyOp{} } -> std::same_as<P&&>; // same thing on a rvalue 
};

} // namespace rb

#endif//RENDERBOI_TOOLBOX_INTERFACES_TRANSFORM_PROXY_HPP
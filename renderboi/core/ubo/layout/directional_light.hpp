#ifndef RENDERBOI_CORE_UBO_LAYOUT_DIRECTIONAL_LIGHT_HPP
#define RENDERBOI_CORE_UBO_LAYOUT_DIRECTIONAL_LIGHT_HPP

#include <renderboi/core/numeric.hpp>
#include <renderboi/core/ubo/ubo_layout.hpp>
#include <renderboi/core/lights/directional_light.hpp>

#include <cpptools/utility/concepts.hpp>

namespace rb {

template<>
struct UBOLayout<DirectionalLight> {
    num::Vec3 direction;
    float _padding1;

    num::Vec3 ambient;  // color
    float _padding2;
    
    num::Vec3 diffuse;  // color
    float _padding3;
    
    num::Vec3 specular; // color
    float _padding4;

    UBOLayout() = default;
    UBOLayout(const DirectionalLight& dirLight);

    const void* data();
};

static_assert(sizeof(UBOLayout<DirectionalLight>) == 64);
static_assert(tools::is_implicit_lifetime_type_v<UBOLayout<DirectionalLight>>);

} // namespace rb

#endif//RENDERBOI_CORE_UBO_LAYOUT_DIRECTIONAL_LIGHT_HPP
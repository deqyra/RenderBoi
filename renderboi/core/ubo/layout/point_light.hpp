#ifndef RENDERBOI_CORE_UBO_LAYOUT_POINT_LIGHT_HPP
#define RENDERBOI_CORE_UBO_LAYOUT_POINT_LIGHT_HPP

#include <renderboi/core/numeric.hpp>
#include <renderboi/core/ubo/ubo_layout.hpp>
#include <renderboi/core/lights/point_light.hpp>

#include <cpptools/utility/concepts.hpp>

namespace rb {

template<>
struct UBOLayout<PointLight> {
    num::Vec3 position;
    float _padding1;

    num::Vec3 ambient;  // color
    float constant;     // attenuation
    
    num::Vec3 diffuse;  // color
    float linear;       // attenuation
    
    num::Vec3 specular; // color
    float quadratic;    // attenuation

    UBOLayout() = default;
    UBOLayout(const num::Vec3& position, const PointLight& pointLight);

    const void* data();
};

static_assert(sizeof(UBOLayout<PointLight>) == 64);
static_assert(tools::is_implicit_lifetime_type_v<UBOLayout<PointLight>>);

} // namespace rb

#endif//RENDERBOI_CORE_UBO_LAYOUT_POINT_LIGHT_HPP
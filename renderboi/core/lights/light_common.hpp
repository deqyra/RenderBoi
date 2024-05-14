#ifndef RENDERBOI_CORE_LIGHTS_LIGHT_COMMON_HPP
#define RENDERBOI_CORE_LIGHTS_LIGHT_COMMON_HPP

#include <renderboi/core/numeric.hpp>
#include <renderboi/core/color.hpp>

namespace rb {

/// @brief Parameters for the color emitted by a light
struct LightColor {
    /// @brief Ambient component of the color
    num::Vec3 ambient = color::White;

    /// @brief Diffuse component of the color
    num::Vec3 diffuse = color::White;

    /// @brief Specular component of the color
    num::Vec3 specular = color::White;
};

/// @brief Parameters for the computation of a light's attenuation over a distance
struct LightAttenuation {
    /// @brief Constant term
    float constant = 1.f;
    
    /// @brief Linear term
    float linear = 0.7f;
    
    /// @brief Quadratic term
    float quadratic = 1.8f;
};

namespace detail {
/**
 * Light attenuation calculation factors depending on the desired range of
 * a light:
 * (http://wiki.ogre3d.org/tiki-index.php?page=-Point+Light+Attenuation)
 * 
 * Distance 	Constant 	Linear 	Quadratic
 * ==========================================
 * 7            1.0         0.7     1.8
 * 13           1.0         0.35    0.44
 * 20           1.0         0.22    0.20
 * 32           1.0         0.14    0.07
 * 50           1.0         0.09    0.032
 * 65           1.0         0.07    0.017
 * 100          1.0         0.045   0.0075
 * 160          1.0         0.027   0.0028
 * 200          1.0         0.022   0.0019
 * 325          1.0         0.014   0.0007
 * 600          1.0         0.007   0.0002
 * 3250         1.0         0.0014  0.000007
 * 
 * Factors involved in calculating the attenuation of a given light depend
 * on the range of said light.
 * 
 * The parameters defined hereafter are used to calculate the _actual_ 
 * attenuation factors, and were computed to best fit the previous data.
 **/

constexpr float QuadraticComponentInQuadraticFactor = 0.0108699f;
constexpr float LinearComponentInQuadraticFactor = 0.066653f;
constexpr float ConstantComponentInQuadraticFactor = -0.443622f;

constexpr float LinearComponentInLinearFactor = 0.234686f;
constexpr float ConstantComponentInLinearFactor = -0.212775f;

inline constexpr float quadraticFactorForRange(const float range) {
    const float quadratic = QuadraticComponentInQuadraticFactor * range * range;
    const float linear = LinearComponentInQuadraticFactor * range;

    return (float)(1.f / (quadratic + linear + ConstantComponentInQuadraticFactor));
};

inline constexpr float linearFactorForRange(const float range) {
    const float linear = LinearComponentInLinearFactor * range;
    return (float)(1.f / (linear + ConstantComponentInLinearFactor));
};
}

inline constexpr LightAttenuation attenuationFactors(const float range) {
    // for some reason light attenuation according to the above parameters is
    // too aggressive and dims out a lot more than expected, so in wait of a proper
    // fix this static boost to the desired range will have to do
    const float adaptiveFactor = 5;

    return {
        .constant  = 1.f,
        .linear    = detail::linearFactorForRange(range * adaptiveFactor),
        .quadratic = detail::quadraticFactorForRange(range * adaptiveFactor)
    };
}

} // namespace rb

#endif//RENDERBOI_CORE_LIGHTS_LIGHT_COMMON_HPP

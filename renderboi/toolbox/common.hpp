#ifndef RENDERBOI__TOOLBOX__COMMON_HPP
#define RENDERBOI__TOOLBOX__COMMON_HPP

#include "glm/detail/qualifier.hpp"
#include <array>

#include <glm/glm.hpp>

namespace renderboi::common
{

/// @brief Approximation of the square root of 2.
constexpr float Sqrt2 = 1.4142135623730950488016887242097;

/// @brief Approximation of the square root of 3.
constexpr float Sqrt3 = 1.7320508075688772935274463415059;

/// @brief Coordinates of the origin (2D).
constexpr std::array<float, 2> O2 = {0.0f, 0.0f};

/// @brief Coordinates of the origin (3D).
constexpr std::array<float, 3> O3 = {0.0f, 0.0f, 0.0f};

/// @brief Return the vector X multiplied by a scalar.
///
/// @param l Scalar by which the vector should be multiplied.
///
/// @return The vector X multiplied by the specified scalar.
static inline constexpr std::array<float, 3> X(float l)
{
    return {l, 0.f, 0.f};
}

/// @brief Coordinates of the X vector.
constexpr std::array<float, 3> x = X(1.f);

/// @brief Return the vector Y multiplied by a scalar.
///
/// @param l Scalar by which the vector should be multiplied.
///
/// @return The vector Y multiplied by the specified scalar.
static inline constexpr std::array<float, 3> Y(float l)
{
    return {0.f, l, 0.f};
}

/// @brief Coordinates of the Y vector.
constexpr std::array<float, 3> y = Y(1.f);

/// @brief Return the vector Z multiplied by a scalar.
///
/// @param l Scalar by which the vector should be multiplied.
///
/// @return The vector Z multiplied by the specified scalar.
static inline constexpr std::array<float, 3> Z(float l)
{
    return {0.f, 0.f, l};
}

/// @brief Coordinates of the Z vector.
constexpr std::array<float, 3> z = Z(1.f);

/// @brief Return the vector I multiplied by a scalar.
///
/// @param l Scalar by which the vector should be multiplied.
///
/// @return The vector I multiplied by the specified scalar.
static inline constexpr std::array<float, 3> I(float l)
{
    return X(l);
}

/// @brief Coordinates of the I vector.
constexpr std::array<float, 3> i = I(1.f);

/// @brief Return the vector J multiplied by a scalar.
///
/// @param l Scalar by which the vector should be multiplied.
///
/// @return The vector J multiplied by the specified scalar.
static inline constexpr std::array<float, 3> J(float l)
{
    return Y(l);
}

/// @brief Coordinates of the J vector.
constexpr std::array<float, 3> j = J(1.f);

/// @brief Return the vector K multiplied by a scalar.
///
/// @param l Scalar by which the vector should be multiplied.
///
/// @return The vector K multiplied by the specified scalar.
static inline constexpr std::array<float, 3> K(float l)
{
    return Z(l);
}

/// @brief Coordinates of the K vector.
constexpr std::array<float, 3> k = K(1.f);

/// @brief RGB components of white.
constexpr std::array<float, 3> White = {1.0f, 1.0f, 1.0f};
/// @brief RGB components of grey.
constexpr std::array<float, 3> Grey = {0.5f, 0.5f, 0.5f};
/// @brief RGB components of black.
constexpr std::array<float, 3> Black = {0.0f, 0.0f, 0.0f};
/// @brief RGB components of red.
constexpr std::array<float, 3> Red = {1.0f, 0.0f, 0.0f};
/// @brief RGB components of green.
constexpr std::array<float, 3> Green = {0.0f, 1.0f, 0.0f};
/// @brief RGB components of blue.
constexpr std::array<float, 3> Blue = {0.0f, 0.0f, 1.0f};
/// @brief RGB components of cyan.
constexpr std::array<float, 3> Cyan = {0.0f, 1.0f, 1.0f};
/// @brief RGB components of magenta.
constexpr std::array<float, 3> Magenta = {1.0f, 0.0f, 1.0f};
/// @brief RGB components of yellow.
constexpr std::array<float, 3> Yellow = {1.0f, 1.0f, 0.0f};

namespace
{

template<
    size_t size,
    typename type,
    glm::qualifier qual = glm::defaultp,
    size_t... I,
    typename vectype = glm::vec<size, type, qual>
>
vectype vec_impl(const std::array<type, size>& array, std::index_sequence<I...>)
{
    return vectype(std::get<I>(array)...);
}

}

/// @brief Convert an std::array to a glm::vec of according size.
template<
    size_t size,
    typename type,
    glm::qualifier qual = glm::defaultp,
    typename vectype = glm::vec<size, type, qual>
>
vectype vec(std::array<type, size> array)
{
    return vec_impl(array, std::make_index_sequence<size>{});
}

}

#endif//RENDERBOI__TOOLBOX__COMMON_HPP
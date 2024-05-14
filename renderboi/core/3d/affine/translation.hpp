#ifndef RENDERBOI_CORE_3D_AFFINE_TRANSLATION_HPP
#define RENDERBOI_CORE_3D_AFFINE_TRANSLATION_HPP

#include <renderboi/core/3d/transform.hpp>
#include <renderboi/core/3d/affine/affine_operation.hpp>

namespace rb::affine {

/// @brief Translate a transform
/// @param t Transform to translate
/// @param translation Translation to apply to the transform
void translate(RawTransform& t, num::Vec3 translation);

/// @brief Represents a 3D translation
class Translation {
public:
    Translation();
    Translation(num::Vec3 translation);

    Translation(const Translation&) = default;
    Translation(Translation&&)      = default;

    Translation& operator=(const Translation&) = default;
    Translation& operator=(Translation&&)      = default;

    /// @brief Apply the translation represented by this object to a transform
    /// @param t Transform to apply the translation to
    void apply(RawTransform& t) const;

private:
    /// @brief Translation vector represented by this object
    num::Vec3 _translation;
};

static_assert(AffineOperation<Translation>);

} // namespace rb::affine

#endif//RENDERBOI_CORE_3D_AFFINE_TRANSLATION_HPP
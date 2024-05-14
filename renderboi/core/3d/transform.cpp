#include "transform.hpp"

namespace rb {

Basis basisOf(const RawTransform& t) {
    return Basis {
        .x = normalize(t.orientation * num::X) * t.scale.x,
        .y = normalize(t.orientation * num::Y) * t.scale.y,
        .z = normalize(t.orientation * num::Z) * t.scale.z,
    };
}

num::Vec3 transformPosition(const num::Vec3& position, const RawTransform& t) {
    return transform(position, basisOf(t));
}

num::Quat transformRotation(const num::Quat& rotation, const RawTransform& t) {
    // Rotating a rotation = rotating in a different frame of reference:
    // 
    // - cancel the parent rotation                  inverse(parent.orientation)
    // - apply the world rotation                    rotation
    // - reapply the parent rotation back            parent.orientation
    // 
    // These steps in order yield the correct multiplication from the rightmost term to the leftmost one
    return normalize(t.orientation * rotation * inverse(t.orientation));
}

num::Vec3 transformScale(const num::Vec3& scale, const RawTransform& t) {
    /// @TODO: this is BS, needs to be addressed to handle shear
    return transform(scale, basisOf(t));
}

RawTransform operator*(const RawTransform& left, const RawTransform& right) {
    return RawTransform {
        .orientation = transformRotation(right.orientation, left),
        .position = transformPosition(right.position, left),
        .scale = transformScale(right.scale, left)
    };
}

RawTransform inverse(const RawTransform& t) {
    return {
        .orientation = inverse(t.orientation),
        .position = transform(t.position, inverse(basisOf(t))),
        .scale = transform(t.scale, inverse(basisOf(t)))
    };
}

num::Mat4 toModelMatrix(const RawTransform& t) {
    // Generate 4x4 matrix from quaternion
    glm::mat4 rotation = glm::toMat4(t.orientation);

    // Scale things up
    glm::mat4 scale = glm::mat4(1.f);
    scale[0][0] = t.scale[0];
    scale[1][1] = t.scale[1];
    scale[2][2] = t.scale[2];

    auto result = rotation * scale;

    // Include translation to current position
    result[3] = num::Vec4(t.position, 1.f);

    return result;
}

} // namespace rb
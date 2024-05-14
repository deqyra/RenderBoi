#include "basis.hpp"

namespace rb {

num::Vec3 transform(const num::Vec3& position, const Basis& basis) {
    return position.x * basis.x + position.y * basis.y + position.z * basis.z;
}

Basis inverse(const Basis& basis) {
    auto basisMat = num::Mat3(
        basis.x.x, basis.y.x, basis.z.x,
        basis.x.y, basis.y.y, basis.z.y,
        basis.x.z, basis.y.z, basis.z.z
    );

    auto inv = inverse(basisMat);
    return Basis {
        { inv[0][0], inv[0][1], inv[0][2] },
        { inv[1][0], inv[1][1], inv[1][2] },
        { inv[2][0], inv[2][1], inv[2][2] },
    };
}

} // namespace rb
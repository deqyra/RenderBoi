#ifndef RENDERBOI_CORE_3D_TRANSFORM_HPP
#define RENDERBOI_CORE_3D_TRANSFORM_HPP

#include "../numeric.hpp"
#include "basis.hpp"
#include "basis_provider.hpp"

namespace rb {

/// @brief Represents the 3D-space properties of an object: position, orientation
/// and scale
struct RawTransform {
    /// @brief Quaternion representing the rotation of the object
    num::Quat orientation = num::Identity;

    /// @brief 3D position of the object
    num::Vec3 position = num::Origin3;

    /// @brief 3D scale of the object
    num::Vec3 scale = num::XYZ;
};

/// @brief Compute the local basis vectors of a transform, in world coordinates
/// @param t The transform whose local vectors should be computed
/// @return The local vectors
Basis basisOf(const RawTransform& t);

/// @brief Apply a transform on a position vector
/// @param position The position to be transformed
/// @param transform The transform to apply
/// @return The transformed position
num::Vec3 transformPosition(const num::Vec3& position, const RawTransform& t);

/// @brief Apply a transform on a rotation
/// @param rotation The rotation to be transformed
/// @param transform The transform to apply
/// @return The transformed rotation
num::Quat transformRotation(const num::Quat& rotation, const RawTransform& t);

/// @brief Apply a transform on a scale
/// @param scale The scale to be transformed
/// @param transform The transform to apply
/// @return The transformed scale
num::Vec3 transformScale(const num::Vec3& scale, const RawTransform& t);

/// @brief Compute the inverse transform of a transform
/// @param transform The transform to compute the inverse of
/// @return The inverse transform
RawTransform inverse(const RawTransform& t);

/// @brief Apply a child transform on top of a parent one
/// @param left The parent transform
/// @param right The child transform
/// @return The resulting transform
RawTransform operator*(const RawTransform& left, const RawTransform& right);

/// @brief Get the model matrix for a transform
/// @param transform The transform to make a model matrix out of
/// @return The model matrix
num::Mat4 toModelMatrix(const RawTransform& t);

/// @brief Represents the 3D-space properties of an object: position, orientation
/// and scale. Also provides basis vectors and a model matrix
class Transform final : public BasisProvider {
public:
    Transform();

    /// @param position Position to be represented by the transform
    /// @param rotation Orientation to be represented by the transform
    /// @param scale Scale to be represented by the transform
    Transform(num::Vec3 position, num::Quat rotation, num::Vec3 scale);

    /// @brief Get the position of the object
    /// @return The position of the object
    const num::Vec3& getPosition() const;

    /// @brief Set the position of the object
    /// @param position The new position the object should have
    /// @return The new position of the object relative to its parent
    void setPosition(num::Vec3 position);

    /// @brief Get the rotation of the object
    /// @return The rotation of the object
    const num::Quat& getRotation() const;

    /// @brief Set the rotation of the object
    /// @param rotation The new rotation the object should have
    /// @return The new rotation of the object relative to its parent
    void setRotation(num::Quat rotation);

    /// @brief Get the scale of the object
    /// @return The scale of the object
    const num::Vec3& getScale() const;

    /// @brief Set the scale of the object
    /// @param scale The new scale the object should have
    /// @return The new scale of the object relative to its parent
    void setScale(num::Vec3 scale);

    /// @brief Get the matrix which applies the parameters of the transform
    /// to any point which it multiplies
    /// @return The model matrix corresponding to the transform
    num::Mat4 getModelMatrix() const;

    /////////////////////////////////////////////
    /// Methods overridden from BasisProvider ///
    /////////////////////////////////////////////

    /// @copydoc BasisProvider::getBasis
    Basis basis() const override;

private:
    /// @brief Model matrix of the object
    mutable num::Mat4 _modelMatrix;

    /// @brief 3D properties of the object
    RawTransform _raw;

    /// @brief Local vectors of the object in world coordinates
    mutable Basis _localVectors;

    /// @brief Whether the local vectors no longer reflects the transform
    /// parameters
    mutable bool _localVectorsOutdated;

    /// @brief Whether the model matrix no longer reflects the transform
    /// parameters
    mutable bool _matrixOutdated;

    /// @brief Update the local vectors so that they reflect the transform
    /// parameters
    void _updateLocalVectors() const;

    /// @brief Update the model matrix of the object so that it reflects the
    /// transform parameters
    void _updateMatrix() const;
};

} // namespace rb

#endif//RENDERBOI_CORE_3D_TRANSFORM_HPP
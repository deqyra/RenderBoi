#ifndef RENDERBOI__CORE__IMPLEMENTATION__PARENT_DEPENDENT_VP_MATRIX_PROVIDER_HPP
#define RENDERBOI__CORE__IMPLEMENTATION__PARENT_DEPENDENT_VP_MATRIX_PROVIDER_HPP

#include <glm/matrix.hpp>

#include "../interfaces/vp_matrix_provider.hpp"
#include "../transform.hpp"

namespace Renderboi
{

/// @brief Implementation of the VPMatrixProvider interface which relies on a
/// provided parent transform to perform matrix calculations and lazy updates.
class ParentDependentVPMatrixProvider : public VPMatrixProvider
{
public:
    /// @param parentTransform Transform of the object the entity is 
    /// attached to.
    ParentDependentVPMatrixProvider(Transform parentWorldTransform);

    ParentDependentVPMatrixProvider(const ParentDependentVPMatrixProvider& other);

    /// @brief Get the view matrix of the entity from its view point.
    ///
    /// @return The view matrix of the entity.
    virtual glm::mat4 getViewMatrix() const override;
    
    /// @brief Transform vector coordinates from world space to view space 
    /// from the entity view point.
    ///
    /// @param worldPosition Coordinates to transform into view space.
    ///
    /// @return The transformed coordinates.
    virtual glm::vec3 worldPositionToViewSpace(const glm::vec3& worldPosition) const override;
    
    /// @brief Get the projection matrix of the entity.
    ///
    /// @return The projection matrix of the entity.
    virtual glm::mat4 getProjectionMatrix() const override;
    
    /// @brief Get the view-projection matrix of the entity.
    ///
    /// @param viewPoint Position of the entity in world coordinates.
    ///
    /// @return The view-projection matrix of the entity.
    virtual glm::mat4 getViewProjectionMatrix() const override;

    /// @brief Get the world transform of the parent of the entity.
    ///
    /// @return The world transform of the parent of the entity.
    Transform getParentWorldTransform() const;

    /// @brief Update the registered world transform of the parent of the entity.
    ///
    /// @param transform New world transform of the parent of the entity.
    void setParentWorldTransform(Transform parentTransform);

protected:
    /// @brief World transform of the object the entity is attached to.
    Transform _parentWorldTransform;

    /// @brief Projection matrix of the entity. Provides perspective and 
    /// lens effect.
    mutable glm::mat4 _projectionMatrix;

    /// @brief View matrix of the entity. Transforms world coordinates into
    /// entity-space coordinates.
    mutable glm::mat4 _viewMatrix;

    /// @brief View-projection matrix of the entity. Combines the view matrix
    /// and the projection matrix.
    mutable glm::mat4 _vpMatrix;

    /// @brief Whether or not the projection matrix needs recalculating in 
    /// order to reflect new parameters.
    mutable bool _projectionMatrixOutdated;

    /// @brief Whether or not the view matrix needs recalculating in order to 
    /// reflect new parameters.
    mutable bool _viewMatrixOutdated;

    /// @brief Whether or not the VP matrix needs recalculating in order to 
    /// reflect new parameters.
    mutable bool _vpMatrixOutdated;

    /// @brief Recalculate the projection matrix based on the registered 
    /// parameters.
    void _updateProjectionMatrix() const;

    /// @brief Recalculate the view matrix based on the registered parameters.
    void _updateViewMatrix() const;

    /// @brief Recalculate the VP matrix based on the registered parameters.
    void _updateVPMatrix() const;

    /// @brief Compute the actual projection matrix using implementation defined parameters.
    virtual glm::mat4 _computeProjectionMatrix() const = 0;

    /// @brief Compute the actual view matrix using implementation defined parameters.
    virtual glm::mat4 _computeViewMatrix() const = 0;
};

} // namespace Renderboi

#endif//RENDERBOI__CORE__IMPLEMENTATION__PARENT_DEPENDENT_VP_MATRIX_PROVIDER_HPP
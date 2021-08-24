#ifndef RENDERBOI__CORE__INTERFACES__VP_MATRIX_PROVIDER_HPP
#define RENDERBOI__CORE__INTERFACES__VP_MATRIX_PROVIDER_HPP

#include <memory>

#include <glm/vec3.hpp>
#include <glm/matrix.hpp>

namespace Renderboi
{

/// @brief Interface for a class able to provide view and projection matrices.
class VPMatrixProvider
{
public:
    /// @brief Get the view matrix of the entity from its view point.
    ///
    /// @return The view matrix of the entity.
    virtual glm::mat4 getViewMatrix() const = 0;
    
    /// @brief Transform vector coordinates from world space to view space 
    /// from the entity view point.
    ///
    /// @param worldPosition Coordinates to transform into view space.
    ///
    /// @return The transformed coordinates.
    virtual glm::vec3 worldPositionToViewSpace(const glm::vec3& worldPosition) const = 0;
    
    /// @brief Get the projection matrix of the entity.
    ///
    /// @return The projection matrix of the entity.
    virtual glm::mat4 getProjectionMatrix() const = 0;
    
    /// @brief Get the view-projection matrix of the entity.
    ///
    /// @param viewPoint Position of the entity in world coordinates.
    ///
    /// @return The view-projection matrix of the entity.
    virtual glm::mat4 getViewProjectionMatrix() const = 0;
};

using VPMatrixProviderPtr = std::shared_ptr<VPMatrixProvider>;

} // namespace Renderboi

#endif//RENDERBOI__CORE__INTERFACES__VP_MATRIX_PROVIDER_HPP
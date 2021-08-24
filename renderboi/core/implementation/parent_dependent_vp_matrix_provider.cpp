#include "parent_dependent_vp_matrix_provider.hpp"

namespace Renderboi
{

ParentDependentVPMatrixProvider::ParentDependentVPMatrixProvider(Transform parentWorldTransform) :
    _parentWorldTransform(parentWorldTransform),
    _viewMatrixOutdated(true),
    _projectionMatrixOutdated(true),
    _vpMatrixOutdated(true)
{

}

ParentDependentVPMatrixProvider::ParentDependentVPMatrixProvider(const ParentDependentVPMatrixProvider& other) :
    _parentWorldTransform(other._parentWorldTransform),
    _viewMatrixOutdated(other._viewMatrixOutdated),
    _projectionMatrixOutdated(other._projectionMatrixOutdated),
    _vpMatrixOutdated(other._vpMatrixOutdated)
{

}

Transform ParentDependentVPMatrixProvider::getParentWorldTransform() const
{
    return _parentWorldTransform;
}

void ParentDependentVPMatrixProvider::setParentWorldTransform(Transform parentWorldTransform)
{
    _parentWorldTransform = parentWorldTransform;

    _viewMatrixOutdated = true;
    _vpMatrixOutdated = true;
}

glm::mat4 ParentDependentVPMatrixProvider::getViewMatrix() const
{
    _updateViewMatrix();

    return _viewMatrix;
}

glm::mat4 ParentDependentVPMatrixProvider::getProjectionMatrix() const
{
    _updateProjectionMatrix();

    return _projectionMatrix;
}

glm::mat4 ParentDependentVPMatrixProvider::getViewProjectionMatrix() const
{
    _updateVPMatrix();

    return _vpMatrix;
}


glm::vec3 ParentDependentVPMatrixProvider::worldPositionToViewSpace(const glm::vec3& worldPosition) const
{
    glm::vec4 transformedPosition = getViewMatrix() * glm::vec4(worldPosition, 1.f);
    return glm::vec3(transformedPosition);
}

void ParentDependentVPMatrixProvider::_updateProjectionMatrix() const
{
    if (!_projectionMatrixOutdated)
    {
        return;
    }

    _projectionMatrix = _computeProjectionMatrix();
    _projectionMatrixOutdated = false;
}

void ParentDependentVPMatrixProvider::_updateViewMatrix() const
{
    if (!_viewMatrixOutdated)
    {
        return;
    }

    _viewMatrix = _computeViewMatrix();
    _viewMatrixOutdated = false;
}

void ParentDependentVPMatrixProvider::_updateVPMatrix() const
{
    if (!_vpMatrixOutdated)
    {
        return;
    }

    _vpMatrix = _projectionMatrix * _viewMatrix;
    _vpMatrixOutdated = false;
}

}
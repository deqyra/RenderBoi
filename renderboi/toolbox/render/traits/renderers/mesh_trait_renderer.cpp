#include "mesh_trait_renderer.hpp"

namespace renderboi
{

void MeshTraitRenderer::render(const RenderTraitConfig& config) const
{
    const Transform objectTransform = meshObject.worldTransform();
    const glm::mat4 modelMatrix = objectTransform.getModelMatrix();

    // Detect non uniform scaling: compute the dot product of the world scale
    // of the object and a uniform scale along all three axes. If the dot 
    // product is not 1, then the object has non-uniform scaling.
    const glm::vec3 scaling = glm::normalize(objectTransform.getScale());
    const float dot = glm::dot(scaling, glm::normalize(glm::vec3(1.f, 1.f, 1.f)));

    // Compute normal matrix
    glm::mat4 normalMatrix = viewMatrix * modelMatrix;
    if (1.f - glm::abs(dot) > 1.e-6)
    {
        // Restore normals if a non-uniform scaling was detected
        normalMatrix = glm::transpose(glm::inverse(normalMatrix));
    }

    // Set up matrices in UBO
    _matrixUbo.setModel(modelMatrix);
    _matrixUbo.setNormal(normalMatrix);

    MeshComponent& meshComponent = meshObject.componentMap().getComponent<ComponentType::Mesh>();
    
    // Set up shader and material
    const Material& material = meshComponent.material();
    ShaderProgram& shader = meshComponent.shader();
    shader.use();
    material.bindTextures();

    if (shader.supports(ShaderFeature::FragmentMeshMaterial))
    {
        shader.setMaterial("material", material);
    }

    meshComponent.mesh().draw();
}

} // namespace renderboi

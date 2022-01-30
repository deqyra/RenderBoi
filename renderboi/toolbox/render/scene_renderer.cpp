#include "scene_renderer.hpp"

#include <chrono>
#include <functional>
#include <iostream>
#include <memory>
#include <thread>
#include <vector>

#include <renderboi/core/lights/light.hpp>
#include <renderboi/core/material.hpp>
#include <renderboi/core/mesh.hpp>
#include <renderboi/core/shader/shader_program.hpp>
#include <renderboi/core/transform.hpp>
#include <renderboi/core/ubo/matrix_ubo.hpp>
#include <renderboi/core/ubo/light_ubo.hpp>

#include "../scene/scene.hpp"
#include "../scene/object/scene_object.hpp"
#include "../scene/object/component.hpp"
#include "../scene/object/components/mesh_component.hpp"
#include "../scene/object/components/light_component.hpp"
#include "../scene/object/components/camera_component.hpp"

namespace renderboi
{

SceneRenderer::SceneRenderer(const unsigned int framerateLimit) :
    _matrixUbo(),
    _lightUbo(),
    _frameIntervalUs((int64_t)(1000000.f/framerateLimit)),
    _lastTimestamp(std::chrono::system_clock::now())
{

}

void SceneRenderer::renderScene(Scene& scene) const
{
    scene.updateAllTransforms();

    // Get pointers to meshes, lights, and the scene camera
    const ObjectVector meshObjects = scene.getObjectsWithComponent<ComponentType::Mesh>();
    const ObjectVector lightObjects = scene.getObjectsWithComponent<ComponentType::Light>();
    const ObjectVector cameraObjects = scene.getObjectsWithComponent<ComponentType::Camera>();

    if (cameraObjects.size() == 0)
    {
        std::cout << "SceneRenderer: No camera in provided scene, cannot render scene." << std::endl;
        return;
    }
    if (cameraObjects.size() > 1)
    {
        std::cout << "SceneRenderer: More than one camera in provided scene, "
            "the first one will be used for rendering." << std::endl;
    }

    // Get the actual camera
    const SceneObject& cameraObj = cameraObjects[0];
    const CameraComponent& cameraComp = cameraObj.componentMap().getComponent<ComponentType::Camera>();

    // Set up matrices in their UBO
    const glm::mat4 view = cameraComp.getViewMatrix();
    const glm::mat4 projection = cameraComp.getProjectionMatrix();
    _matrixUbo.setView(view);
    _matrixUbo.setProjection(projection);

    // Preprocess lights
    LightVector lights;
    std::vector<Transform> worldTransforms;
    for (const SceneObject& lightObj : lightObjects)
    {
        LightComponent& lightComp = lightObj.componentMap().getComponent<ComponentType::Light>();
        // Get the actual light and its world model matrix (needed to compute its world position)
        lights.push_back(lightComp.light());
        worldTransforms.push_back(scene.getWorldTransform(lightObj.id));
    }
    _sendLightData(lights, worldTransforms, view);

    // Compute time elapsed between frames and limit framerate if necessary
    const std::chrono::time_point<std::chrono::system_clock> newTimestamp = std::chrono::system_clock::now();
    const std::chrono::duration<double> duration = newTimestamp - _lastTimestamp;
    _lastTimestamp = newTimestamp;
    const int64_t gap = _frameIntervalUs - std::chrono::duration_cast<std::chrono::microseconds>(duration).count();
    std::this_thread::sleep_for(std::chrono::microseconds(gap));

    for (const auto& meshObj : meshObjects)
    {
        // Draw the mesh
        drawMesh(meshObj, cameraComp.getViewMatrix());
    }
}

void SceneRenderer::setFramerateLimit(const unsigned int framerateLimit)
{
    _frameIntervalUs = (int64_t)(1000000.f / framerateLimit);
}

void SceneRenderer::_sendLightData(
    const LightVector& lights,
    const std::vector<Transform>& worldTransforms,
    const glm::mat4& view
) const
{
    // Light counts need to be kept track of
    unsigned int pLightIndex = 0;
    unsigned int sLightIndex = 0;
    unsigned int dLightIndex = 0;

    for (unsigned int i = 0; i < lights.size(); i++)
    {
        Light& light = lights[i];
        glm::vec3 position = glm::vec3(view * glm::vec4(worldTransforms[i].getPosition(), 1.f));

        switch (light.lightType)
        {
            case LightType::PointLight:
                if (pLightIndex < LightUBO::PointLightMaxCount)
                {
                    // Retrieve the light as a PointLight
                    PointLight& pLight = static_cast<PointLight&>(light);
                    // Send it to the UBO
                    _lightUbo.setPoint(pLightIndex++, pLight, position);
                }
                else
                {
                    throw std::runtime_error("SceneRenderer: PointLight max count exceeded, cannot send more lights to UBO.");
                }
                
                break;
            case LightType::SpotLight:
                if (sLightIndex < LightUBO::SpotLightMaxCount)
                {
                    // Retrieve the light as a SpotLight
                    SpotLight& sLight = static_cast<SpotLight&>(light);
                    // Send it to the UBO
                    _lightUbo.setSpot(sLightIndex++, sLight, position);
                }
                else
                {
                    throw std::runtime_error("SceneRenderer: SpotLight max count exceeded, cannot send more lights to UBO.");
                }
                
                break;
            case LightType::DirectionalLight:
                if (dLightIndex < LightUBO::DirectionalLightMaxCount)
                {
                    // Retrieve the light as a DirectionalLight
                    DirectionalLight& dLight = static_cast<DirectionalLight&>(light);
                    // Send it to the UBO
                    _lightUbo.setDirectional(dLightIndex++, dLight);
                }
                else
                {
                    throw std::runtime_error("SceneRenderer: DirectionalLight max count exceeded, cannot send more lights to UBO.");
                }
                
                break;
        }
    }

    // Set light counts
    _lightUbo.setPointCount(pLightIndex);
    _lightUbo.setSpotCount(sLightIndex);
    _lightUbo.setDirectionalCount(dLightIndex);
}

void SceneRenderer::drawMesh(SceneObject& meshObject, const glm::mat4& viewMatrix) const
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

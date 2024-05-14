#include <chrono>

#include <renderboi/core/material.hpp>
#include <renderboi/core/3d/mesh.hpp>
#include <renderboi/core/shader/shader_program.hpp>
#include <renderboi/core/ubo/light_ubo.hpp>
#include <renderboi/core/ubo/matrix_ubo.hpp>
#include <renderboi/core/3d/transform.hpp>

#include <renderboi/toolbox/scene/components/camera_component.hpp>
#include <renderboi/toolbox/scene/components/directional_light_component.hpp>
#include <renderboi/toolbox/scene/components/point_light_component.hpp>
#include <renderboi/toolbox/scene/components/spot_light_component.hpp>
#include <renderboi/toolbox/scene/components/rendered_mesh_component.hpp>
#include <renderboi/toolbox/scene/object.hpp>
#include <renderboi/toolbox/scene/scene.hpp>

#include "renderboi/core/lights/spot_light.hpp"
#include "scene_renderer.hpp"

namespace rb {

SceneRenderer::SceneRenderer(const unsigned int framerateLimit)
    : _matrixUbo()
    , _lightUbo()
    , _lastTimestamp(std::chrono::steady_clock::now())
    , _frameIntervalUs((int64_t)(1000000.f / framerateLimit))
{

}

void SceneRenderer::render(Scene& scene) const {
    scene.update();

    // Camera
    auto cameraGroup = scene.group<CameraComponent>();
    Object cameraObj = cameraGroup.front();
    const auto& cameraComp = cameraGroup.get<CameraComponent>(cameraObj);

    const auto& camera          = *(cameraComp.value);
    const auto& cameraTransform = scene.worldTransform(cameraObj);
    const num::Mat4 view        = camera.viewMatrix(cameraTransform.position);
    const num::Mat4 projection  = camera.projMatrix();

    _matrixUbo.setView(view);
    _matrixUbo.setProjection(projection);
    _matrixUbo.commitViewProjection();

    // Lights
    std::size_t i = 0;
    auto spotLights = scene.group<SpotLightComponent>();
    for (auto&& [lightObj, lightComp] : spotLights.each()) {
        const auto& lightTransform = scene.worldTransform(lightObj);
        _lightUbo.get<SpotLight>(i) = { lightTransform.position, *(lightComp.value) };
    }
    _lightUbo.count<SpotLight>() = spotLights.size();
    
    i = 0;
    auto pointLights = scene.group<PointLightComponent>();
    for (auto&& [lightObj, lightComp] : pointLights.each()) {
        const auto& lightTransform = scene.worldTransform(lightObj);
        _lightUbo.get<PointLight>(i) = { lightTransform.position, *(lightComp.value) };
    }
    _lightUbo.count<PointLight>() = spotLights.size();
    
    i = 0;
    auto directionalLights = scene.group<DirectionalLightComponent>();
    for (auto&& [lightObj, lightComp] : directionalLights.each()) {
        _lightUbo.get<DirectionalLight>(i) = { *(lightComp.value) };
    }
    _lightUbo.count<DirectionalLight>() = spotLights.size();

    _lightUbo.commit();

    // Compute time elapsed between frames and limit framerate if necessary
    // const Timestamp newTimestamp = std::chrono::steady_clock::now();
    // const auto duration = newTimestamp - _lastTimestamp;
    // _lastTimestamp = newTimestamp;
    // const int64_t gap = _frameIntervalUs - std::chrono::duration_cast<std::chrono::microseconds>(duration).count();
    // std::this_thread::sleep_for(std::chrono::microseconds(gap));

    auto meshes = scene.group<RenderedMeshComponent>();
    for (auto&& [meshObj, meshComp] : meshes.each()) {
        // Draw the mesh
        drawMesh(meshComp, scene.worldTransform(meshObj), view);
    }
}

void SceneRenderer::drawMesh(const RenderedMeshComponent& renderedMesh, const RawTransform& transform, const num::Mat4& viewMatrix) const {
    const num::Mat4 modelMatrix = toModelMatrix(transform);

    // Detect non uniform scaling: compute the dot product of the world scale
    // of the object and a uniform scale along all three axes. If the dot
    // product is not 1, then the object has non-uniform scaling.
    const float dot = num::dot(transform.scale, num::normalize(num::XYZ));

    // Compute normal matrix
    num::Mat4 normalMatrix = viewMatrix * modelMatrix;
    if (1.f - num::abs(dot) > 1.e-6) {
        // Restore normals if a non-uniform scaling was detected
        normalMatrix = num::transpose(num::inverse(normalMatrix));
    }

    // Set up matrices in UBO
    _matrixUbo.setModel(modelMatrix);
    _matrixUbo.setNormal(normalMatrix);
    _matrixUbo.commitModelNormal();

    // Set up shader and material
    const Material& material = *(renderedMesh.material);
    ShaderProgram&  shader   = *(renderedMesh.shader);
    Mesh&           mesh     = *(renderedMesh.mesh);

    shader.use();
    bindTextures(material);

    if (shader.supports(ShaderFeature::FragmentMeshMaterial)) {
        shader.setMaterial("material", material);
    }

    mesh.draw();
}

} // namespace rb

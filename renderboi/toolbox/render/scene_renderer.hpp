#ifndef RENDERBOI_TOOLBOX_SCENE_SCENE_RENDERER_HPP
#define RENDERBOI_TOOLBOX_SCENE_SCENE_RENDERER_HPP

#include <chrono>
#include <cstdint>
#include <memory>

#include <renderboi/core/3d/transform.hpp>
#include <renderboi/core/ubo/light_ubo.hpp>
#include <renderboi/core/ubo/matrix_ubo.hpp>

#include <renderboi/toolbox/scene/scene.hpp>
#include <renderboi/toolbox/scene/components/rendered_mesh_component.hpp>

namespace rb {

/// @brief Manages the render process of a scene
class SceneRenderer {
private:
    using Timestamp = std::chrono::time_point<std::chrono::steady_clock>;

    /// @brief Handle to a UBO for matrices on the GPU
    mutable MatrixUBO _matrixUbo;

    /// @brief Handle to a UBO for lights on the GPU
    mutable LightUBO _lightUbo;

    /// @brief Last recorded render timestamp. Used to limit the framerate
    mutable Timestamp _lastTimestamp;

    /// @brief Minimum time interval to keep between rendered frames
    int64_t _frameIntervalUs;

    /// @brief Issue draw commands for a single mesh
    ///
    /// @param meshInfo The mesh to draw, along with its material and the shader to draw it with
    /// @param transform The transform of the mesh
    /// @param viewMatrix The view matrix, provided by the scene camera
    void drawMesh(const RenderedMeshComponent& renderedMesh, const RawTransform& transform, const num::Mat4& viewMatrix) const;

public:
    /// @param framerateLimit How many frames per second the SceneRenderer
    /// should seek to render
    SceneRenderer(const unsigned int framerateLimit = 60);

    /// @brief Render the provided scene
    ///
    /// @param scene A pointer to the scene which should be rendered
    ///
    /// @exception If the scene has too many lights of any type for the
    /// light UBO to handle, the function will throw a std::runtime_error
    void render(Scene& scene) const;
};

using SceneRendererPtr = std::unique_ptr<SceneRenderer>;

} // namespace rb

#endif // RENDERBOI_TOOLBOX_SCENE_SCENE_RENDERER_HPP

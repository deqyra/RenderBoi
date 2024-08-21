#include <chrono>
#include <optional>

#include <renderboi/core/color.hpp>
#include <renderboi/core/numeric.hpp>
#include <renderboi/core/material.hpp>
#include <renderboi/core/materials.hpp>
#include <renderboi/core/3d/camera.hpp>
#include <renderboi/core/3d/mesh.hpp>
#include <renderboi/core/3d/affine/orbit.hpp>
#include <renderboi/core/3d/affine/rotation.hpp>
#include <renderboi/core/3d/affine/set_position.hpp>
#include <renderboi/core/3d/affine/translation.hpp>
#include <renderboi/core/3d/affine/turn.hpp>
#include <renderboi/core/lights/light_common.hpp>
#include <renderboi/core/lights/point_light.hpp>
#include <renderboi/core/shader/shader.hpp>
#include <renderboi/core/shader/shader_builder.hpp>
#include <renderboi/core/shader/shader_program.hpp>

#include <renderboi/toolbox/controls/control_event_translator.hpp>
#include <renderboi/toolbox/controls/control_scheme.hpp>
#include <renderboi/toolbox/controls/controlled_entity_manager.hpp>
#include <renderboi/toolbox/input_splitter.hpp>
#include <renderboi/toolbox/mesh_generators/axes_generator.hpp>
#include <renderboi/toolbox/mesh_generators/cube_generator.hpp>
#include <renderboi/toolbox/mesh_generators/tetrahedron_generator.hpp>
#include <renderboi/toolbox/mesh_generators/torus_generator.hpp>
#include <renderboi/toolbox/render/scene_renderer.hpp>
#include <renderboi/toolbox/runnables/basic_window_manager.hpp>
#include <renderboi/toolbox/runnables/camera_aspect_ratio_manager.hpp>
#include <renderboi/toolbox/runnables/input_logger.hpp>
#include <renderboi/toolbox/runnables/keyboard_movement_script.hpp>
#include <renderboi/toolbox/runnables/mouse_camera_manager.hpp>
#include <renderboi/toolbox/scene/object.hpp>
#include <renderboi/toolbox/scene/scene.hpp>
#include <renderboi/toolbox/scene/components/camera_component.hpp>
#include <renderboi/toolbox/scene/components/point_light_component.hpp>
#include <renderboi/toolbox/scene/components/rendered_mesh_component.hpp>

#include <renderboi/window/gl_window.hpp>
#include <renderboi/window/window_factory.hpp>

#include "lighting_sandbox.hpp"

namespace rb {

LightingSandbox::LightingSandbox(GLWindow& window, const GLSandboxParameters& params)
    : GLSandbox(window, params)
{

}

void LightingSandbox::setUp() {
    // Update window title
    _title = _window.getTitle();
    _window.setTitle(_title + " - Lighting");

    // Remove cursor from window
    namespace InputMode = Window::Input::Mode;
    _window.setInputMode(
        InputMode::Target::Cursor, InputMode::Value::DisabledCursor
    );
}

void LightingSandbox::run() {
    GLSandbox::_initContext();

    ShaderConfig lightConfig;
    lightConfig.addFeature(ShaderFeature::VertexMVP);
    lightConfig.addFeature(ShaderFeature::FragmentMeshMaterial);
    lightConfig.addFeature(ShaderFeature::FragmentBlinnPhong);
    ShaderProgram lightingShader = ShaderBuilder::BuildShaderProgramFromConfig(lightConfig);
    Material emerald = Materials::Emerald;
    Material gold    = Materials::Gold;
    Material def     = Materials::Default;

    Scene scene;

    // Input splitter that will broadcast the input received by the window
    InputSplitter splitter;
    _window.registerInputProcessor(static_cast<InputProcessor&>(splitter));

    // BIG TORUS
    const auto bigTorusObj = scene.create(scene.root(), "Big torus");
    auto bigTorusMesh = TorusGenerator({ 2.f, 0.5f, 72, 48 }).generate();
    scene.emplace<RenderedMeshComponent>(
        bigTorusObj,
        RenderedMeshComponent{
            .mesh = bigTorusMesh.get(),
            .material = &emerald,
            .shader = &lightingShader
        }
    );

    // SMALL TORUS
    const auto smallTorusObj = scene.create(bigTorusObj, "Small torus");
    auto smallTorusMesh = TorusGenerator({ 0.75f, 0.25f, 64, 32 }).generate();
    scene.emplace<RenderedMeshComponent>(
        smallTorusObj,
        RenderedMeshComponent{
            .mesh = smallTorusMesh.get(),
            .material = &gold,
            .shader = &lightingShader
        }
    );

    ShaderProgram minimal = ShaderBuilder::MinimalShaderProgram();

    // AXES
    const auto axesObj = scene.create(scene.root(), "Axes");
    auto axesMesh = AxesGenerator({ 3.f }).generate();
    scene.emplace<RenderedMeshComponent>(
        axesObj,
        RenderedMeshComponent{
            .mesh = axesMesh.get(),
            .material = &def,
            .shader = &minimal
        }
    );

    // CUBE
    const auto cubeObj = scene.create(scene.root(), "Light cube");
    auto cubeMesh = CubeGenerator(CubeGenerator::Parameters{ .size = 0.3f, .color = std::nullopt }).generate();
    scene.emplace<RenderedMeshComponent>(
        cubeObj,
        RenderedMeshComponent{
            .mesh = cubeMesh.get(),
            .material = &def,
            .shader = &lightingShader
        }
    );

    PointLight light{
        .color = {},
        .attenuation = attenuationFactors(LightBaseRange)
    };
    scene.emplace<PointLightComponent>(
        cubeObj,
        PointLightComponent{ &light }
    );

    // TETRAHEDRON
    const auto tetrahedronObj = scene.create(smallTorusObj, "Tetrahedron");
    auto tetrahedronMesh = TetrahedronGenerator(TetrahedronGenerator::Parameters{ .size = 0.5f, .color = std::nullopt }).generate();
    scene.emplace<RenderedMeshComponent>(
        tetrahedronObj,
        RenderedMeshComponent{
            .mesh = tetrahedronMesh.get(),
            .material = &def,
            .shader = &minimal
        }
    );

    // CAMERA
    const auto cameraObj = scene.create(scene.root(), "Camera");
    Camera camera = { CameraViewParams, CameraProjParams };
    scene.emplace<CameraComponent>(
        cameraObj,
        CameraComponent{ &camera }
    );

    // Link camera to MouseCameraManager
    MouseCameraManager cameraManager(camera);
    splitter.registerInputProcessor(cameraManager);

    // Link camera to CameraAspectRatioManager
    CameraAspectRatioManager cameraAspectRatioManager(camera);
    splitter.registerInputProcessor(cameraAspectRatioManager);

    // Attach object movement script to scene
    LightingSandboxScript rotationScript(
        scene.localTransform(cubeObj),
        scene.localTransform(bigTorusObj),
        scene.localTransform(smallTorusObj),
        scene.localTransform(tetrahedronObj),
        scene.worldTransform(cameraObj),
        light,
        LightBaseRange
    );
    splitter.registerInputProcessor(rotationScript);

    // KeyboardMovementScript
    auto keyboardScriptManager = ControlledEntityManager<KeyboardMovementScript<LocalTransformProxy>>(
        scene.localTransform(cameraObj),
        camera
    );
    splitter.registerInputProcessor(keyboardScriptManager.eventTranslator());

    // Window script
    auto windowManager = ControlledEntityManager<BasicWindowManager>(_window);
    splitter.registerInputProcessor(windowManager.entity());
    splitter.registerInputProcessor(windowManager.eventTranslator());

    // Instantiate an input logger
    InputLogger logger;
    splitter.registerInputProcessor(logger);


    // Move stuff around
    using namespace affine;
    scene.localTransform(bigTorusObj)    << Rotation(num::radians(90.f), num::X);
    scene.localTransform(smallTorusObj)  << Rotation(num::radians(90.f), num::X) << Translation(-2.f * num::X);
    scene.localTransform(cubeObj)        << SetPosition(StartingLightPosition);
    scene.localTransform(tetrahedronObj) << Translation(-1.2f * num::X)          << Rotation(glm::radians(90.f), num::Z);
    scene.localTransform(cameraObj)      << SetPosition(StartingCameraPosition)  << Rotation(glm::radians(180.f), num::Y);

    SceneRenderer sceneRenderer;

    glClearColor(0.2f, 0.0f, 0.3f, 1.0f);
    glEnable(GL_DEPTH_TEST);

    using Clock = std::chrono::steady_clock;
    auto lastTimestamp = Clock::now();
    while (!_window.exitSignaled()) {
        // Process events which require to be processed on the rendering thread
        _window.processPendingContextEvents();

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // Update and draw scene
        scene.update();
        sceneRenderer.render(scene);
        _window.swapBuffers();

        // Update scripts
        auto now = Clock::now();
        auto delta = std::chrono::duration_cast<std::chrono::milliseconds>(now - lastTimestamp).count() / 1000.f;
        lastTimestamp = now;

        keyboardScriptManager.entity().update(delta);
        rotationScript.update(delta);
    }

    GLSandbox::_terminateContext();
}

void LightingSandbox::tearDown() {
    // Reset everything back to how it was
    namespace InputMode = Window::Input::Mode;
    _window.setInputMode(
        InputMode::Target::Cursor, InputMode::Value::NormalCursor
    );
    _window.detachInputProcessor();
    _window.setTitle(_title);
}

LightingSandboxScript::LightingSandboxScript(
    LocalTransformProxy& cube,
    LocalTransformProxy& bigTorus,
    LocalTransformProxy& smallTorus,
    LocalTransformProxy& tetrahedron,
    const RawTransform& cameraWorldTransform,
    PointLight& light,
    float      baseLightRange
)
    : _cube(cube)
    , _bigTorus(bigTorus)
    , _smallTorus(smallTorus)
    , _tetrahedron(tetrahedron)
    , _cameraWorldTransform(cameraWorldTransform)
    , _light(light)
    , _autoRotate(true)
    , _speedFactor(1.75f)
    , _sine(LightVariationFrequency)
    , _baseRange(baseLightRange)
{
    _sine.start();
}

void LightingSandboxScript::update(float timeElapsed) {
    _light.attenuation = attenuationFactors(_baseRange + _sine.value() * (LightVariationAmplitude / 2.f));

    using namespace affine;
    if (_autoRotate) {
        // Update object transforms
        float delta = _speedFactor * timeElapsed;

        _bigTorus    << Rotation(num::radians(45.f * delta), BigTorusRotationAxis);
        _tetrahedron << Rotation(num::radians(45.f * delta), TetrahedronRotationAxis);
        _cube        <<    Orbit(num::radians(45.f * delta), CubeOrbitAxis,          num::Vec3(0.f, 3.f, 0.f), true);
        _smallTorus  <<    Orbit(num::radians(45.f * delta), SmallTorusRotationAxis, num::Vec3(0.f, 0.f, 0.f), true);
        _tetrahedron <<    Orbit(num::radians(45.f * delta), TetrahedronOrbitAxis,   num::Vec3(0.f), true);
    } else {
        _bigTorus << Turn(_cameraWorldTransform.position, num::Y);
    }
}

void LightingSandboxScript::processKeyboard(
    GLWindow&                   window,
    const Window::Input::Key    key,
    const int                   scancode,
    const Window::Input::Action action,
    const int                   mods
) {
    using Key    = Window::Input::Key;
    using Action = Window::Input::Action;
    using Mod    = Window::Input::Modifier;

    if (_autoRotate) {
        if (key == Key::Up && (action == Action::Press) &&
            (mods & Mod::Control) && _speedFactor < 10.f) {
            _speedFactor *= 1.1f;
        } else if (key == Key::Down && (action == Action::Press) && (mods & Mod::Control) && _speedFactor > 0.2f) {
            _speedFactor /= 1.1f;
        }
    }

    if (key == Key::Enter && action == Action::Press) {
        _autoRotate = !_autoRotate;
    }
}

} // namespace rb

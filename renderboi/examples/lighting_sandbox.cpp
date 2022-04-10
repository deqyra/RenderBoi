#include "lighting_sandbox.hpp"
#include "renderboi/core/shader/shader_program.hpp"

#include <iostream>
#include <memory>
#include <string>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <renderboi/window/gl_window.hpp>
#include <renderboi/window/window_factory.hpp>

#include <renderboi/core/mesh.hpp>
#include <renderboi/core/materials.hpp>
#include <renderboi/core/lights/point_light.hpp>
#include <renderboi/core/frame_of_reference.hpp>
#include <renderboi/core/shader/shader_builder.hpp>

#include <renderboi/toolbox/factory.hpp>
#include <renderboi/toolbox/input_splitter.hpp>
#include <renderboi/toolbox/controls/control_scheme.hpp>
#include <renderboi/toolbox/controls/control_event_translator.hpp>
#include <renderboi/toolbox/controls/controlled_entity_manager.hpp>
#include <renderboi/toolbox/mesh_generators/mesh_type.hpp>
#include <renderboi/toolbox/scene/scene.hpp>
#include <renderboi/toolbox/scene/object/scene_object.hpp>
#include <renderboi/toolbox/scene/object/component_type.hpp>
#include <renderboi/toolbox/scene/object/components/all_components.hpp>
#include <renderboi/toolbox/render/scene_renderer.hpp>
#include <renderboi/toolbox/runnables/input_logger.hpp>
#include <renderboi/toolbox/runnables/mouse_camera_manager.hpp>
#include <renderboi/toolbox/runnables/keyboard_movement_script.hpp>
#include <renderboi/toolbox/runnables/basic_window_manager.hpp>
#include <renderboi/toolbox/runnables/camera_aspect_ratio_manager.hpp>

namespace renderboi
{

using Ref = FrameOfReference;

LightingSandbox::LightingSandbox(GLWindow& window, const GLSandboxParameters& params) :
    GLSandbox(window, params)
{

}

void LightingSandbox::setUp()
{
    // Update window title
    _title = _window.getTitle();
    _window.setTitle(_title + " - Lighting");

    // Remove cursor from window
    namespace InputMode = Window::Input::Mode;
    _window.setInputMode(InputMode::Target::Cursor, InputMode::Value::DisabledCursor);
}

void LightingSandbox::run()
{
    GLSandbox::_initContext();

    ShaderConfig lightConfig;
    lightConfig.addFeature(ShaderFeature::VertexMVP);
    lightConfig.addFeature(ShaderFeature::FragmentMeshMaterial);
    lightConfig.addFeature(ShaderFeature::FragmentBlinnPhong);
    ShaderProgram lightingShader = ShaderBuilder::BuildShaderProgramFromConfig(lightConfig);

    Scene scene;

    // BIG TORUS
    SceneObject& bigTorusObj = Factory::MakeSceneObjectWithMesh<MeshType::Torus>(
        scene, "Big torus",
        {2.f, 0.5f, 72, 48},
        Materials::Emerald,
        lightingShader
    );

    // SMALL TORUS
    SceneObject& smallTorusObj = Factory::MakeSceneObjectWithMesh<MeshType::Torus>(
        scene, "Small torus",
        {0.75f, 0.25f, 64, 32},
        Materials::Gold,
        lightingShader
    );
    
    ShaderProgram minimal = ShaderBuilder::MinimalShaderProgram();

    // AXES
    SceneObject& axesObj = Factory::MakeSceneObjectWithMesh<MeshType::Axes>(
        scene, "Axes",
        {3.f},
        Materials::Default,
        minimal
    );
    
    // CUBE
    SceneObject& cubeObj = Factory::MakeSceneObjectWithMesh<MeshType::Cube>(
        scene, "Light cube",
        (CubeGenerator::Parameters){0.3f, {0.f, 0.f, 0.f}, false},
        Materials::Default,
        lightingShader
    );
    PointLight light(LightBaseRange);
    cubeObj.componentMap().addComponent<ComponentType::Light>(light);

    // TETRAHEDRON
    SceneObject& tetrahedronObj = Factory::MakeSceneObjectWithMesh<MeshType::Tetrahedron>(
        scene, "Tetrahedron",
        {0.5f},
        Materials::Default,
        lightingShader
    );

    // CAMERA
    SceneObject& cameraObj = scene.newObject("Camera");
    Camera camera(CameraParams);
    cameraObj.componentMap().addComponent<ComponentType::Camera>(camera);

    // Link camera to MouseCameraManager
    MouseCameraManager cameraManager(camera);

    // Link camera to CameraAspectRatioManager
    CameraAspectRatioManager cameraAspectRatioManager(camera);

    // Attach object movement script to scene
    LightingSandboxScript rotationScript(cubeObj, bigTorusObj, smallTorusObj, tetrahedronObj, cameraObj, light, LightBaseRange);
    scene.registerScript(rotationScript);
    
    // Add script component to camera: KeyboardMovementScript
    ControlledEntityManager<KeyboardMovementScript> keyboardScriptManager(static_cast<BasisProvider&>(camera));
    cameraObj.componentMap().addComponent<ComponentType::Script>(static_cast<Script&>(keyboardScriptManager.entity()));

    // Window script
    ControlledEntityManager<BasicWindowManager> windowManager(_window);

    // Instantiate an input logger
    InputLogger logger;

    // Register the scene and the control translator to the splitter
    InputSplitter splitter;
    splitter.registerInputProcessor(static_cast<InputProcessor&>(logger));
    splitter.registerInputProcessor(static_cast<InputProcessor&>(cameraManager));
    splitter.registerInputProcessor(static_cast<InputProcessor&>(cameraAspectRatioManager));
    splitter.registerInputProcessor(static_cast<InputProcessor&>(rotationScript));
    splitter.registerInputProcessor(static_cast<InputProcessor&>(keyboardScriptManager.eventTranslator()));
    splitter.registerInputProcessor(static_cast<InputProcessor&>(windowManager.entity()));
    splitter.registerInputProcessor(static_cast<InputProcessor&>(windowManager.eventTranslator()));
    
    // Register the splitter to the window
    _window.registerInputProcessor(static_cast<InputProcessor&>(splitter));


    // Register the splitter to the window
    _window.registerInputProcessor(static_cast<InputProcessor&>(splitter));

    const glm::vec3 X = Transform::X;
    const glm::vec3 Y = Transform::Y;
    const glm::vec3 Z = Transform::Z;

    // Move stuff around
    bigTorusObj.transform().rotateBy<Ref::World>((float)glm::radians(90.f), X);
    smallTorusObj.transform().rotateBy<Ref::Parent>(glm::radians(90.f), X);
    smallTorusObj.transform().translateBy<Ref::Parent>(-2.f * X);
    cubeObj.transform().setPosition<Ref::World>(StartingLightPosition);
    tetrahedronObj.transform().translateBy<Ref::Parent>(-1.2f * X);
    tetrahedronObj.transform().rotateBy<Ref::Parent>(glm::radians(90.f), Z);
    cameraObj.transform().setPosition<Ref::World>(StartingCameraPosition);
    cameraObj.transform().rotateBy<Ref::Parent>(glm::radians(180.f), Y);

    SceneRenderer sceneRenderer;

    glClearColor(0.0f, 0.0f, 0.1f, 1.0f);
    glEnable(GL_DEPTH_TEST);
    while (!_window.exitSignaled())
    {
        // Process events which require to be processed on the rendering thread
        _window.processPendingContextEvents();

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // Update and draw scene
        scene.triggerUpdate();
        sceneRenderer.renderScene(scene);
        _window.swapBuffers();
    }

    GLSandbox::_terminateContext();
}

void LightingSandbox::tearDown()
{
    // Reset everything back to how it was
    namespace InputMode = Window::Input::Mode;
    _window.setInputMode(InputMode::Target::Cursor, InputMode::Value::NormalCursor);
    _window.detachInputProcessor();
    _window.setTitle(_title);
}

LightingSandboxScript::LightingSandboxScript(
    SceneObject& cubeObj,
    SceneObject& bigTorusObj,
    SceneObject& smallTorusObj,
    SceneObject& tetrahedronObj,
    SceneObject& cameraObj,
    PointLight& light,
    float baseLightRange
) :
    _cubeObj(cubeObj),
    _bigTorusObj(bigTorusObj),
    _smallTorusObj(smallTorusObj),
    _tetrahedronObj(tetrahedronObj),
    _cameraObj(cameraObj),
    _light(light),
    _autoRotate(true),
    _speedFactor(1.75f),
    _sine(LightVariationFrequency),
    _baseRange(baseLightRange)
{
    _sine.start();
}

void LightingSandboxScript::update(float timeElapsed)
{
    _light.setRange(_baseRange + _sine.get() * (LightVariationAmplitude / 2.f));

    if (_autoRotate)
    {
        // Update object transforms
        float delta = _speedFactor * timeElapsed;

        _cubeObj.transform().orbit<Ref::World>((float)glm::radians(45.f * delta), CubeOrbitAxis, glm::vec3(0.f, 3.f, 0.f), true);
        _bigTorusObj.transform().rotateBy<Ref::Parent>((float)glm::radians(45.f * delta), BigTorusRotationAxis);
        _smallTorusObj.transform().orbit<Ref::Parent>((float)glm::radians(45.f * delta), SmallTorusRotationAxis, glm::vec3(0.f, 0.f, 0.f), true);
        _tetrahedronObj.transform().rotateBy<Ref::Self>((float)glm::radians(45.f * delta), TetrahedronRotationAxis);
        _tetrahedronObj.transform().orbit<Ref::Parent>((float)glm::radians(45.f * delta), TetrahedronOrbitAxis, glm::vec3(0.f), true);
    }
    else
    {
        _bigTorusObj.transform().lookAt<Ref::World>(_cameraObj.worldTransform().getPosition(), {0.f, 1.f, 0.f});
    }
}

void LightingSandboxScript::processKeyboard(GLWindow& window, const Window::Input::Key key, const int scancode, const Window::Input::Action action, const int mods)
{
    using Key = Window::Input::Key;
    using Action = Window::Input::Action;
    using Mod = Window::Input::Modifier;

    if (_autoRotate)
    {
        if (key == Key::Up && (action == Action::Press) && (mods & Mod::Control) && _speedFactor < 10.f)
        {
            _speedFactor *= 1.1f;
        }
        else if (key == Key::Down && (action == Action::Press) && (mods & Mod::Control) && _speedFactor > 0.2f)
        {
            _speedFactor /= 1.1f;
        }
    }

    if (key == Key::Enter && action == Action::Press)
    {
        _autoRotate = !_autoRotate;
    }
}

LightingSandboxScript* LightingSandboxScript::clone() const
{
    return nullptr;
}

} // namespace renderboi

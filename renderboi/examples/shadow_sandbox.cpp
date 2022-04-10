#include "shadow_sandbox.hpp"

#include <string>

#include <renderboi/core/camera.hpp>
#include <renderboi/core/frame_of_reference.hpp>
#include <renderboi/core/material.hpp>
#include <renderboi/core/materials.hpp>
#include <renderboi/core/pixel_space.hpp>
#include <renderboi/core/texture_2d.hpp>
#include <renderboi/core/lights/point_light.hpp>
#include <renderboi/core/shader/shader.hpp>

#include <renderboi/toolbox/factory.hpp>
#include <renderboi/toolbox/input_splitter.hpp>
#include <renderboi/toolbox/controls/controlled_entity_manager.hpp>
#include <renderboi/toolbox/mesh_generators/mesh_type.hpp>
#include <renderboi/toolbox/mesh_generators/plane_generator.hpp>
#include <renderboi/toolbox/mesh_generators/torus_generator.hpp>
#include <renderboi/toolbox/render/scene_renderer.hpp>
#include <renderboi/toolbox/scene/scene.hpp>
#include <renderboi/toolbox/scene/object/scene_object.hpp>
#include <renderboi/toolbox/scene/object/components/camera_component.hpp>
#include <renderboi/toolbox/scene/object/components/light_component.hpp>
#include <renderboi/toolbox/scene/object/components/mesh_component.hpp>
#include <renderboi/toolbox/scene/object/components/script_component.hpp>
#include <renderboi/toolbox/runnables/basic_window_manager.hpp>
#include <renderboi/toolbox/runnables/camera_aspect_ratio_manager.hpp>
#include <renderboi/toolbox/runnables/gamepad_movement_script.hpp>
#include <renderboi/toolbox/runnables/gamepad_camera_manager.hpp>
#include <renderboi/toolbox/runnables/input_logger.hpp>
#include <renderboi/toolbox/runnables/keyboard_movement_script.hpp>
#include <renderboi/toolbox/runnables/mouse_camera_manager.hpp>

namespace renderboi
{

using Ref = FrameOfReference;

ShadowSandbox::ShadowSandbox(GLWindow& window, const GLSandboxParameters& params) :
    GLSandbox(window, params),
    _title(window.getTitle()),
    _gamepad(nullptr)
{

}

void ShadowSandbox::setUp()
{
    // Update window title
    _window.setTitle(_title + " - Shadows");

    // Remove cursor from window
    namespace InputMode = Window::Input::Mode;
    _window.setInputMode(InputMode::Target::Cursor, InputMode::Value::DisabledCursor);

    using Joystick = Window::Input::Joystick;
    GamepadManager& gManager = _window.getGamepadManager();
    std::vector<Joystick> presentGamepads = gManager.pollPresentGamepads();

    if (presentGamepads.size())
    {
        _gamepad = &(gManager.getGamepad(presentGamepads[0]));
        _gamepad->enable();
    }
}

void ShadowSandbox::run()
{
    GLSandbox::_initContext();

    ////////////////////////////
    ///                      ///
    ///   Generate shaders   ///
    ///                      ///
    ////////////////////////////

    ShaderConfig lightConfig;
    lightConfig.addFeature(ShaderFeature::VertexMVP);
    lightConfig.addFeature(ShaderFeature::FragmentFullLight);
    ShaderProgram fullLightShader = ShaderBuilder::BuildShaderProgramFromConfig(lightConfig);

    lightConfig.removeFeature(ShaderFeature::FragmentFullLight);
    lightConfig.addFeature(ShaderFeature::FragmentMeshMaterial);
    lightConfig.addFeature(ShaderFeature::FragmentBlinnPhong);
    lightConfig.addFeature(ShaderFeature::FragmentGammaCorrection);
    ShaderProgram blinnPhongShader = ShaderBuilder::BuildShaderProgramFromConfig(lightConfig);

    //ShaderConfig depthConfig;
    //depthConfig.addFeature(ShaderFeature::VertexMVP);
    //depthConfig.addFeature(ShaderFeature::FragmentViewDepthBuffer);
    //ShaderProgram depthShader = ShaderBuilder::BuildShaderProgramFromConfig(depthConfig);
    


    /////////////////////////////////////////////
    ///                                       ///
    ///   Create scene, instantiate objects   ///
    ///                                       ///
    /////////////////////////////////////////////

    Scene scene;

    PlaneGenerator::Parameters planeParameters = {
        PlaneTileSize,      // tileSizeX
        PlaneTileSize,      // tileSizeY
        PlaneTileCount,     // tileAmountX
        PlaneTileCount,     // tileAmountY
        5.f,                // xTexSize
        5.f,                // yTexSize
        0.f,                // xTexCoordOffset
        0.f,                // yTexCoordOffset
        false,              // invertXTexCoords
        false,              // invertYTexCoords
        0.f,                // texRotation
        {1.f, 1.f, 1.f}     // color
    };

    // FLOOR
    Material floorMaterial = Material(
        glm::vec3(0.f),
        glm::vec3(1.f),
        glm::vec3(0.3203125f, 0.254296875f, 0.180859375f),
        128.f
    );
    Texture2D floorTex = Texture2D("wood.png", PixelSpace::sRGB);
    floorMaterial.pushDiffuseMap(floorTex);
    SceneObject* fp = nullptr;

    SceneObject& floorObj = Factory::MakeSceneObjectWithMesh<MeshType::Plane>(scene, "Floor", planeParameters, floorMaterial, blinnPhongShader);

    // WALLS
    Material wallMaterial = Material(
        glm::vec3(0.f), 
        glm::vec3(1.f), 
        glm::vec3(0.25f, 0.15f, 0.15f), 
        2.f
    );
    Texture2D wallTex = Texture2D("wall.jpg", PixelSpace::sRGB);
    wallMaterial.pushDiffuseMap(wallTex);

    // XY wall
    SceneObject& xyWallObj = Factory::MakeSceneObjectWithMesh<MeshType::Plane>(scene, "XY wall", planeParameters, wallMaterial, blinnPhongShader);

    // YZ wall
    SceneObject& yzWallObj = Factory::MakeSceneObjectWithMesh<MeshType::Plane>(scene, "YZ wall", planeParameters, wallMaterial, blinnPhongShader);

    TorusGenerator::Parameters torusParameters = {
        4.f,    // toroidalRadius
        0.5f,   // poloidalRadius
        96,     // toroidalVertexRes
        48      // poloidalVertexRes
    };

    // TORUS
    SceneObject& torusObj = Factory::MakeSceneObjectWithMesh<MeshType::Torus>(scene, "Torus", torusParameters, Materials::Default, blinnPhongShader);

    // LIGHT
    SceneObject& lightObj = Factory::MakeSceneObjectWithMesh<MeshType::Cube>(scene, "Light cube", {0.3f, {0.f, 0.f, 0.f}, false}, Materials::Default, fullLightShader);
    PointLight light(LightBaseRange);
    lightObj.componentMap().addComponent<ComponentType::Light>(light);

    // CAMERA
    SceneObject& cameraObj = scene.newObject("Camera");
    Camera camera(CameraParams);
    cameraObj.componentMap().addComponent<ComponentType::Camera>(camera);

    //depthShader.setFloat("near", camera->getNearDistance());
    //depthShader.setFloat("far", camera->getFarDistance());



    ///////////////////////////////
    ///                         ///
    ///   Move objects around   ///
    ///                         ///
    ///////////////////////////////

    const glm::vec3 X = Transform::X;
    const glm::vec3 Y = Transform::Y;
    const glm::vec3 Z = Transform::Z;

    floorObj.transform().rotateBy<Ref::World>(glm::radians(-90.f), X);
    floorObj.transform().translateBy<Ref::World>({0.f, 0.f, WallSize});
    yzWallObj.transform().rotateBy<Ref::World>(glm::radians(90.f), Y);
    yzWallObj.transform().translateBy<Ref::World>({0.f, 0.f, WallSize});
    torusObj.transform().translateBy<Ref::World>({WallSize / 2.f, WallSize / 2.f, WallSize / 2.f});
    torusObj.transform().rotateBy<Ref::World>(glm::radians(45.f), X);
    lightObj.transform().setPosition<Ref::World>(LightPosition);
    cameraObj.transform().setPosition<Ref::World>(StartingCameraPosition);
    cameraObj.transform().rotateBy<Ref::Parent>(glm::radians(180.f), Y);



    /////////////////////////////////////
    ///                               ///
    ///   Add and configure scripts   ///
    ///                               ///
    /////////////////////////////////////

    // Link camera to MouseCameraManager
    MouseCameraManager cameraManager(camera);

    // Link camera to CameraAspectRatioManager
    CameraAspectRatioManager cameraAspectRatioManager(camera);

    // Attach object movement script to scene
    ShadowSandboxScript sandboxScript(lightObj, torusObj);
    scene.registerScript(static_cast<Script&>(sandboxScript));
    
    BasisProvider& cameraAsBasisProvider = static_cast<BasisProvider&>(camera);

    // Add script component to camera: KeyboardMovementScript
    ControlledEntityManager<KeyboardMovementScript> keyboardMovementScriptManager(cameraAsBasisProvider);
    cameraObj.componentMap().addComponent<ComponentType::Script>(static_cast<Script&>(keyboardMovementScriptManager.entity()));

    // Add script component to camera: GamepadMovementScript
    GamepadMovementScript gamepadMovementScript(cameraAsBasisProvider);
    cameraObj.componentMap().addComponent<ComponentType::Script>(static_cast<Script&>(gamepadMovementScript));

    // Add script component to scene: GamepadCameraManager
    GamepadCameraManager gamepadCameraManager(camera);
    scene.registerScript(static_cast<Script&>(gamepadCameraManager));

    // Window script
    ControlledEntityManager<BasicWindowManager> windowManager(_window);

    // Instantiate an input logger
    InputLogger logger;
    logger.disableEventLog(InputProcessor::EventType::MouseCursor);
    logger.disableEventLog(GamepadInputProcessor::EventType::Axis);

    // Register all input processors to the splitter
    InputSplitter splitter;
    splitter.registerInputProcessor(static_cast<InputProcessor&>(cameraManager));
    splitter.registerInputProcessor(static_cast<InputProcessor&>(cameraAspectRatioManager));
    splitter.registerInputProcessor(static_cast<InputProcessor&>(sandboxScript));
    splitter.registerInputProcessor(static_cast<InputProcessor&>(keyboardMovementScriptManager.eventTranslator()));
    splitter.registerInputProcessor(static_cast<InputProcessor&>(windowManager.entity()));
    splitter.registerInputProcessor(static_cast<InputProcessor&>(windowManager.eventTranslator()));

    // Register all gamepad input processors to the splitter
    splitter.registerGamepadInputProcessor(static_cast<GamepadInputProcessor&>(gamepadMovementScript));
    splitter.registerGamepadInputProcessor(static_cast<GamepadInputProcessor&>(gamepadCameraManager));

    // Register the logger as both a classic input processor and a gamepad input processor
    splitter.registerInputProcessor(static_cast<InputProcessor&>(logger));
    splitter.registerGamepadInputProcessor(static_cast<GamepadInputProcessor&>(logger));
    
    // Register the splitter to the window and the gamepad
    _window.registerInputProcessor(static_cast<InputProcessor&>(splitter));
    if (_gamepad)
    {
        _gamepad->registerInputProcessor(static_cast<GamepadInputProcessor&>(splitter));
    }

    glClearColor(0.0f, 0.0f, 0.1f, 1.0f);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_STENCIL_TEST);

    SceneRenderer sceneRenderer;

    while (!_window.exitSignaled())
    {
        // Process awaiting render events
        _window.processPendingContextEvents();

        // Do a single render pass
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT); 

        // Update and draw scene
        scene.triggerUpdate();
        sceneRenderer.renderScene(scene);
        _window.swapBuffers();
    }

    splitter.detachAllInputProcessors();

    GLSandbox::_terminateContext();
}

void ShadowSandbox::tearDown()
{
    namespace InputMode = Window::Input::Mode;

    // Reset everything back to how it was
    _window.setInputMode(InputMode::Target::Cursor, InputMode::Value::NormalCursor);
    _window.detachInputProcessor();
    _window.setTitle(_title);
}

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
/////////////////////////////                       ////////////////////////////
/////////////////////////////  ShadowSandboxScript  ////////////////////////////
/////////////////////////////                       ////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

ShadowSandboxScript::ShadowSandboxScript(SceneObject& lightObj, SceneObject& torusObj) :
    _lightObj(lightObj),
    _torusObj(torusObj),
    _lightStartingPos(lightObj.worldTransform().getPosition()),
    _pause(false),
    _speedFactor(2.f),
    _sine(LightMovementFrequency)
{

}

void ShadowSandboxScript::update(float timeElapsed)
{
    static bool firstUpdate = true;
    if (firstUpdate)
    {
        _sine.start();
        firstUpdate = false;
    }

    if (!_pause)
    {
        glm::vec3 newLightPos = _lightStartingPos + LightMovementAxis * _sine.get() * (LightMovementAmplitude / 2.f);
        _lightObj.transform().setPosition<Ref::World>(newLightPos);

        float delta = _speedFactor * timeElapsed;
        _torusObj.transform().rotateBy<Ref::World>(glm::radians(45.f * delta), TorusRotationAxis);
    }
}

void ShadowSandboxScript::processKeyboard(GLWindow& _window, const Window::Input::Key key, const int scancode, const Window::Input::Action action, const int mods)
{
    using Key = Window::Input::Key;
    using Action = Window::Input::Action;
    using Mod = Window::Input::Modifier;

    if (!_pause)
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
        _pause = !_pause;

        if (_pause) _sine.pause();
        else _sine.start();
    }
}

ShadowSandboxScript* ShadowSandboxScript::clone() const
{
    return nullptr;
}

} // namespace renderboi

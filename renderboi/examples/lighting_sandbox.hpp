#ifndef RENDERBOI_EXAMPLES_LIGHTING_EXAMPLE_HPP
#define RENDERBOI_EXAMPLES_LIGHTING_EXAMPLE_HPP

#include <string>

#include <cpptools/math/sine_generator.hpp>

#include <renderboi/core/numeric.hpp>
#include <renderboi/core/3d/camera.hpp>
#include <renderboi/core/lights/point_light.hpp>

#include <renderboi/window/gl_window.hpp>
#include <renderboi/window/input_processor.hpp>

#include <renderboi/toolbox/script.hpp>
#include <renderboi/toolbox/scene/object.hpp>
#include <renderboi/toolbox/scene/scene.hpp>

#include "gl_sandbox.hpp"
#include "gl_sandbox_parameters.hpp"

namespace rb {

/// @brief Example class to display lit moving objects
class LightingSandbox : public GLSandbox {
private:
    /// @brief Used to temporarily store the original title of the window
    std::string _title;

    static constexpr float LightBaseRange = 30.f;
    static constexpr ViewParameters CameraViewParams = {
        .up    = num::Y,
        .yaw   = -45.f,
        .pitch = -40.f
    };
    static constexpr ProjectionParameters CameraProjParams = {};
    static constexpr num::Vec3 StartingCameraPosition = {5.f, 6.f, 5.f};
    static constexpr num::Vec3 StartingLightPosition = {-3.f, 3.f, 0.f};

public:
    /// @param window Reference to the window on which the sandbox should run
    /// @param params Strcture packing the parameters according to which the
    /// sandbox should run
    LightingSandbox(GLWindow& window, const GLSandboxParameters& params);

    /////////////////////////////////////////
    ///                                   ///
    /// Methods overridden from GLSandbox ///
    ///                                   ///
    /////////////////////////////////////////

    /// @brief Set up the window prior to running the example
    /// @note Must be called from the main thread
    virtual void setUp() override;

    /// @brief Run something in the provided GL window
    /// @note Should run on its own thread
    virtual void run() override;

    /// @brief Restore the window back to how it was before the example ran
    /// This function should perform the opposite steps from setUp().
    /// @note Must be called from the main thread once run() has returned
    virtual void tearDown() override;
};

// Handle object movement in the scene displayed by LightingSandbox
class LightingSandboxScript : public InputProcessor, public Script {
private:
    /// @brief Reference to the cube of the LightingSandbox
    LocalTransformProxy& _cube;

    /// @brief Reference to the big torus of the LightingSandbox
    LocalTransformProxy& _bigTorus;

    /// @brief Reference to the small torus of the LightingSandbox
    LocalTransformProxy& _smallTorus;

    /// @brief Reference to the tetrahedron of the LightingSandbox
    LocalTransformProxy& _tetrahedron;

    /// @brief Reference to the world transform of the camera of the LightingSandbox
    const RawTransform& _cameraWorldTransform;

    /// @brief Reference to the light whose range to vary
    PointLight& _light;

    /// @brief Whether objects should move
    bool _autoRotate;

    /// @brief How fast objects should move
    float _speedFactor;

    /// @brief Sine along which to vary the light range
    tools::sine_generator<float> _sine;

    /// @brief Base range of the light
    float _baseRange;
    
public:
    static constexpr num::Vec3 CubeOrbitAxis           = num::Y;
    static constexpr num::Vec3 BigTorusRotationAxis    = num::X;
    static constexpr num::Vec3 SmallTorusRotationAxis  = num::Y;
    static constexpr num::Vec3 TetrahedronRotationAxis = num::Y;
    static constexpr num::Vec3 TetrahedronOrbitAxis    = num::Y;
    static constexpr float LightVariationAmplitude     = 50.f;
    static constexpr float LightVariationFrequency     = 1.f;

    /// @param cubeObj Reference to the cube of the LightingSandbox
    /// @param bigTorusObj Reference to the big torus of the LightingSandbox
    /// @param smallTorusObj Reference to the small torus of the LightingSandbox
    /// @param tetrahedronObj Reference to the tetrahedron of the LightingSandbox
    /// @param cameraObj Reference to the camera of the LightingSandbox
    /// @param light Reference to the light whose range to vary
    /// @param baseLightRange Base range of the light
    LightingSandboxScript(
        LocalTransformProxy& cube,
        LocalTransformProxy& bigTorus,
        LocalTransformProxy& smallTorus,
        LocalTransformProxy& tetrahedron,
        const RawTransform& cameraWorldTransform,
        PointLight& light,
        float baseLightRange
    );

    //////////////////////////////////////
    ///                                ///
    /// Methods overridden from Script ///
    ///                                ///
    //////////////////////////////////////

    /// @brief Make the script run and do its things
    ///
    /// @param timeElapsed How much time passed (in seconds) since the last
    /// update
    void update(float timeElapsed) override;

    //////////////////////////////////////////////
    ///                                        ///
    /// Methods overridden from InputProcessor ///
    ///                                        ///
    //////////////////////////////////////////////

    /// @brief Callback for a keyboard event
    ///
    /// @param window Reference to the GLWindow in which the event was
    /// triggered
    /// @param key Literal describing which key triggered the event
    /// @param scancode Scancode of the key which triggered the event 
    /// Platform-dependent, but consistent over time
    /// @param action Literal describing what action was performed on
    /// the key which triggered the event
    /// @param mods Bit field describing which modifiers were enabled 
    /// during the key event (Ctrl, Shift, etc)
    void processKeyboard(
        GLWindow& window,
        const Window::Input::Key key,
        const int scancode,
        const Window::Input::Action action,
        const int mods
    ) override;
};

} // namespace rb

#endif//RENDERBOI_EXAMPLES_LIGHTING_EXAMPLE_HPP
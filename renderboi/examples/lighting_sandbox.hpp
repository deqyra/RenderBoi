#ifndef RENDERBOI__EXAMPLES__LIGHTING_EXAMPLE_HPP
#define RENDERBOI__EXAMPLES__LIGHTING_EXAMPLE_HPP

#include <string>

#include <glm/glm.hpp>

#include <cpptools/math/sine_generator.hpp>

#include <renderboi/core/camera.hpp>
#include <renderboi/core/lights/point_light.hpp>

#include <renderboi/window/gl_window.hpp>
#include <renderboi/window/input_processor.hpp>

#include <renderboi/toolbox/script.hpp>
#include <renderboi/toolbox/scene/object/scene_object.hpp>

#include "gl_sandbox.hpp"
#include "gl_sandbox_parameters.hpp"

namespace renderboi
{

/// @brief Example class to display lit moving objects.
class LightingSandbox : public GLSandbox
{
    private:
        /// @brief Used to temporarily store the original title of the window.
        std::string _title;

    public:
        static constexpr float LightBaseRange = 30.f;
        static constexpr Camera::CameraParameters CameraParams = {-45.f, -40.f, 1.f};
        static constexpr glm::vec3 StartingCameraPosition = {5.f, 6.f, 5.f};
        static constexpr glm::vec3 StartingLightPosition = {-3.f, 3.f, 0.f};

		/// @param window Reference to the window on which the sandbox should run.
        /// @param params Strcture packing the parameters according to which the
        /// sandbox should run.
        LightingSandbox(GLWindow& window, const GLSandboxParameters& params);

        /////////////////////////////////////////
        ///                                   ///
        /// Methods overridden from GLSandbox ///
        ///                                   ///
        /////////////////////////////////////////

		/// @brief Set up the window prior to running the example. Must be
		/// called from the main thread.
		virtual void setUp() override;

        /// @brief Run something in the provided GL window. Must be run by a
		/// separate thread.
		virtual void run() override;

		/// @brief Restore the window back to how it was before the example ran.
		/// The contents of this function should be the opposite from those in
		/// setUp(). Must be called from the main thread once run() has returned.
		virtual void tearDown() override;
};

// Handle object movement in the scene displayed by LightingSandbox
class LightingSandboxScript : public InputProcessor, public Script
{
    private:
        /// @brief Reference to the cube of the LightingSandbox.
        SceneObject& _cubeObj;

        /// @brief Reference to the big torus of the LightingSandbox.
        SceneObject& _bigTorusObj;

        /// @brief Reference to the small torus of the LightingSandbox.
        SceneObject& _smallTorusObj;

        /// @brief Reference to the tetrahedron of the LightingSandbox.
        SceneObject& _tetrahedronObj;

        /// @brief Reference to the camera of the LightingSandbox.
        SceneObject& _cameraObj;

        /// @brief Reference to the light whose range to vary.
        PointLight& _light;

        /// @brief Whether objects should move.
        bool _autoRotate;

        /// @brief How fast objects should move.
        float _speedFactor;

        /// @brief Sine along which to vary the light range.
        cpptools::SineGenerator<float> _sine;

        /// @brief Base range of the light.
        float _baseRange;
        
    public:
        static constexpr glm::vec3 CubeOrbitAxis = {0.f, 1.f, 0.f};
        static constexpr glm::vec3 BigTorusRotationAxis = {1.f, 0.f, 0.f};
        static constexpr glm::vec3 SmallTorusRotationAxis = {0.f, 1.f, 0.f};
        static constexpr glm::vec3 TetrahedronRotationAxis = {0.f, 1.f, 0.f};
        static constexpr glm::vec3 TetrahedronOrbitAxis = {0.f, 1.f, 0.f};
        static constexpr float LightVariationAmplitude = 50.f;
        static constexpr float LightVariationFrequency = 0.f;

        /// @param cubeObj Reference to the cube of the LightingSandbox.
        /// @param bigTorusObj Reference to the big torus of the LightingSandbox.
        /// @param smallTorusObj Reference to the small torus of the LightingSandbox.
        /// @param tetrahedronObj Reference to the tetrahedron of the LightingSandbox.
        /// @param cameraObj Reference to the camera of the LightingSandbox.
        /// @param light Reference to the light whose range to vary.
        /// @param baseLightRange Base range of the light
        LightingSandboxScript(
            SceneObject& cubeObj,
            SceneObject& bigTorusObj,
            SceneObject& smallTorusObj,
            SceneObject& tetrahedronObj,
            SceneObject& cameraObj,
            PointLight& light,
            float baseLightRange
        );

        //////////////////////////////////////
        ///                                ///
        /// Methods overridden from Script ///
        ///                                ///
        //////////////////////////////////////

        /// @brief Make the script run and do its things.
        ///
        /// @param timeElapsed How much time passed (in seconds) since the last
        /// update.
        void update(float timeElapsed) override;

        /// @brief Get a raw pointer to a new script instance cloned 
        /// from this one. Ownership and responsibility for the allocated 
        /// resources are fully transferred to the caller.
        ///
        /// @return A raw pointer to the script instance cloned from this one.
        LightingSandboxScript* clone() const override;

        //////////////////////////////////////////////
        ///                                        ///
        /// Methods overridden from InputProcessor ///
        ///                                        ///
        //////////////////////////////////////////////

        /// @brief Callback for a keyboard event.
        ///
        /// @param window Reference to the GLWindow in which the event was
        /// triggered.
        /// @param key Literal describing which key triggered the event.
        /// @param scancode Scancode of the key which triggered the event. 
        /// Platform-dependent, but consistent over time.
        /// @param action Literal describing what action was performed on
        /// the key which triggered the event.
        /// @param mods Bit field describing which modifiers were enabled 
        /// during the key event (Ctrl, Shift, etc).
        void processKeyboard(
            GLWindow& window,
            const Window::Input::Key key,
            const int scancode,
            const Window::Input::Action action,
            const int mods
        ) override;
};

} // namespace renderboi

#endif//RENDERBOI__EXAMPLES__LIGHTING_EXAMPLE_HPP
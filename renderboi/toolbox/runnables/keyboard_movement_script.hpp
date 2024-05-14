#ifndef RENDERBOI_TOOLBOX_RUNNABLES_KEYBOARD_MOVEMENT_SCRIPT_HPP
#define RENDERBOI_TOOLBOX_RUNNABLES_KEYBOARD_MOVEMENT_SCRIPT_HPP

#include <array>
#include <algorithm>
#include <string_view>
#include <utility>

#include <cpptools/utility/predicate.hpp>

#include <renderboi/core/numeric.hpp>
#include <renderboi/core/3d/basis_provider.hpp>
#include <renderboi/core/3d/affine/translation.hpp>

#include <renderboi/window/gl_window.hpp>

#include <renderboi/toolbox/script.hpp>
#include <renderboi/toolbox/controls/control_scheme.hpp>
#include <renderboi/toolbox/interfaces/action_event_receiver.hpp>
#include <renderboi/toolbox/interfaces/default_control_scheme_provider.hpp>
#include <renderboi/toolbox/interfaces/transform_proxy.hpp>
#include <renderboi/toolbox/scene/scene.hpp>

namespace rb {

/// @brief Litterals describing the actions which can be performed by the
/// KeyboardMovementScript
enum class KeyboardMovementAction {
    Forward,
    Backward,
    Left,
    Right,
    Sprint
};

/// @brief Provides bindings to move an entity using the keyboard
/// @tparam Transform_t Type providing access to a transform that will receive the translations
template<TransformProxy Transform_t>
class KeyboardMovementScript :
    public Script,
    public ActionEventReceiver<KeyboardMovementAction>,
    public DefaultControlSchemeProvider<KeyboardMovementAction> {
private:
    struct MovementFlags {
        bool forward  = false;
        bool left     = false;
        bool backward = false;
        bool right    = false;
    };

    KeyboardMovementScript(const KeyboardMovementScript&) = default;
    KeyboardMovementScript(KeyboardMovementScript&&) = default;

    KeyboardMovementScript& operator=(const KeyboardMovementScript&) = delete;
    KeyboardMovementScript& operator=(KeyboardMovementScript&&) = delete;

    /// @brief Transform whose position should be updated 
    Transform_t& _transform;

    /// @brief Reference to the entity which will provide directional vectors,
    /// used to move in the correct directions
    BasisProvider& _basisProvider;
    
    /// @brief Directional movement flags
    MovementFlags _movementFlags;

    /// @brief Speed of the movement induced by keypresses
    float _moveSpeed;

    /// @brief Speed multiplier when sprinting
    float _sprintMultiplier;

    /// @brief Sprint flag
    bool _sprint;
    
public:
    using ActionType = KeyboardMovementAction;

    /// @brief The default move speed (movement with WASD keys)
    static constexpr float DefaultMoveSpeed = 4.f;
    
    /// @brief The default sprint multiplier
    static constexpr float DefaultSprintMultiplier = 1.5f;

    /// @param transform A transform proxy through which translations will be applied to the moved entity
    /// @param basisProvider Entity which will provide directional vectors
    /// @param speed The speed of movement induced by keypresses
    /// @param sprintMultiplier The multiplier to be used when sprinting
    KeyboardMovementScript(
        Transform_t& transform,
        BasisProvider& basisProvider,
        const float speed = DefaultMoveSpeed,
        const float sprintMultiplier = DefaultSprintMultiplier
    ) :
        _transform(transform),
        _basisProvider(basisProvider),
        _movementFlags{},
        _moveSpeed(speed),
        _sprintMultiplier(sprintMultiplier),
        _sprint(false)
    {

    }

    //////////////////////////////////////
    ///                                ///
    /// Methods overridden from Script ///
    ///                                ///
    //////////////////////////////////////

    /// @brief Make the script run and do its script things
    /// @param timeElapsed How much time passed (in seconds) since the last update
    void update(const float timeElapsed) override {
        // Compute distance to cover in this frame
        float velocity = timeElapsed * _moveSpeed;
        if (_sprint) {
            velocity *= _sprintMultiplier;
        }

        // Compute velocity across basis vectors
        float forwardVelocity = velocity * (_movementFlags.forward - _movementFlags.backward);
        float leftVelocity    = velocity * (_movementFlags.left    - _movementFlags.right);

        const auto& basis = _basisProvider.basis();
        num::Vec3 translation =
            basis.x * leftVelocity +
            basis.z * forwardVelocity;

        // Update parent position
        _transform << affine::Translation(std::move(translation));
    }

    ///////////////////////////////////////////////////////////////////////////
    ///                                                                     ///
    /// Methods overridden from ActionEventReceiver<KeyboardMovementAction> ///
    ///                                                                     ///
    ///////////////////////////////////////////////////////////////////////////

    /// @brief Start the processing for an action
    /// @param action Object describing the action to start processing
    void triggerAction(const KeyboardMovementAction& action) override {
        using enum KeyboardMovementAction;
        switch (action) {
        case Forward:  _movementFlags.forward  = true; break;
        case Backward: _movementFlags.backward = true; break;
        case Left:     _movementFlags.left     = true; break;
        case Right:    _movementFlags.right    = true; break;
        case Sprint:   _sprint                 = true; break;
        }
    }

    /// @brief Stop the processing for an action
    /// @param action Object describing the action to stop processing
    void stopAction(const KeyboardMovementAction& action) override {
        using enum KeyboardMovementAction;
        switch (action) {
        case Forward:  _movementFlags.forward  = false; break;
        case Backward: _movementFlags.backward = false; break;
        case Left:     _movementFlags.left     = false; break;
        case Right:    _movementFlags.right    = false; break;
        case Sprint:   _sprint                 = false; break;
        }
    }

    ////////////////////////////////////////////////////////////////////////////////////
    ///                                                                              ///
    /// Methods overridden from DefaultControlSchemeProvider<KeyboardMovementAction> ///
    ///                                                                              ///
    ////////////////////////////////////////////////////////////////////////////////////

    /// @brief Get the default control scheme for the keyboard movement script
    /// @return The default control scheme for the keyboard movement script
    const ControlScheme<KeyboardMovementAction>& defaultControlScheme() const override {
        using Window::Input::Key;
        static ControlScheme<KeyboardMovementAction> scheme = {
            { Control(Key::W),          KeyboardMovementAction::Forward },
            { Control(Key::A),          KeyboardMovementAction::Left },
            { Control(Key::S),          KeyboardMovementAction::Backward },
            { Control(Key::D),          KeyboardMovementAction::Right },
            { Control(Key::LeftShift),  KeyboardMovementAction::Sprint }
        };

        return scheme;
    }
};

inline std::string_view to_string(const KeyboardMovementAction action) {
    static const std::array<std::pair<KeyboardMovementAction, std::string_view>, 5> enumNames = {{
        { KeyboardMovementAction::Forward,   "Forward"  },
        { KeyboardMovementAction::Backward,  "Backward" },
        { KeyboardMovementAction::Left,      "Left"     },
        { KeyboardMovementAction::Right,     "Right"    },
        { KeyboardMovementAction::Sprint,    "Sprint"   },
    }};

    using tools::pred::first_member;
    using tools::pred::equals;

    auto it = std::ranges::find_if(enumNames, first_member(equals(action)));
    return (it != enumNames.end())
        ? it->second
        : "Unknown";
}

} // namespace rb

#endif//RENDERBOI_TOOLBOX_RUNNABLES_KEYBOARD_MOVEMENT_SCRIPT_HPP
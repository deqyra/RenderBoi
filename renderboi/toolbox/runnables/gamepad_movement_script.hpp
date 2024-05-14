#ifndef RENDERBOI_TOOLBOX_RUNNABLES_GAMEPAD_MOVEMENT_SCRIPT_HPP
#define RENDERBOI_TOOLBOX_RUNNABLES_GAMEPAD_MOVEMENT_SCRIPT_HPP

#include <utility>

#include <renderboi/core/numeric.hpp>
#include <renderboi/core/3d/basis_provider.hpp>
#include <renderboi/core/3d/affine/translation.hpp>

#include <renderboi/toolbox/script.hpp>
#include <renderboi/toolbox/interfaces/transform_proxy.hpp>

#include <renderboi/window/gamepad/gamepad.hpp>
#include <renderboi/window/gamepad/gamepad_input_processor.hpp>

namespace rb {

/// @brief Provides bindings to move an entity using a gamepad
/// @tparam Transform_t Type providing access to a transform that will receive the translations
template<TransformProxy Transform_t>
class GamepadMovementScript : public Script, public GamepadInputProcessor {
private:
    GamepadMovementScript(const GamepadMovementScript&) = default;
    GamepadMovementScript(GamepadMovementScript&&) = default;
    
    GamepadMovementScript& operator=(const GamepadMovementScript&) = delete;
    GamepadMovementScript& operator=(GamepadMovementScript&&) = delete;

    /// @brief Transform whose position should be updated 
    Transform_t& _transform;

    /// @brief Reference to the entity which will provide directional vectors,
    /// used to move in the correct directions
    BasisProvider& _basisProvider;

    /// @brief Vector telling the direction in which to go
    num::Vec2 _direction;

    /// @brief Speed of the movement induced by keypresses
    float _moveSpeed;

    /// @brief Speed multiplier when sprinting
    float _sprintMultiplier;
    
    /// @brief Sprint flag
    bool _sprint;
    
public:
    /// @brief The default move speed (movement with left thumbstick)
    static constexpr float DefaultMoveSpeed = 4.f;
    
    /// @brief The default sprint multiplier
    static constexpr float DefaultSprintMultiplier = 1.5f;

    /// @param transform A transform proxy through which translations will be applied to the moved entity
    /// @param basisProvider Entity which will provide directional vectors
    /// @param speed The speed of movement induced by gamepad input
    /// @param sprintMultiplier The multiplier to be used when sprinting
    GamepadMovementScript(
        Transform_t& transform,
        BasisProvider& basisProvider,
        const float speed = DefaultMoveSpeed,
        const float sprintMultiplier = DefaultSprintMultiplier
    ) :
        _transform(transform),
        _basisProvider(basisProvider),
        _direction{},
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

    /// @brief Make the script run and do its things
    /// @param timeElapsed How much time passed (in seconds) since the last
    /// update
    void update(const float timeElapsed) override {
        // Compute distance to cover in this frame
        float velocity = timeElapsed * _moveSpeed;
        if (_sprint) {
            velocity *= _sprintMultiplier;
        }

        num::Vec2 movement = velocity * _direction;

        // Compute velocity across basis vectors
        const auto& basis = _basisProvider.basis();
        num::Vec3 translation =
            basis.x * movement.x +
            basis.z * movement.y;

        // Update parent position
        _transform << affine::Translation(std::move(translation));
    }

    /////////////////////////////////////////////////////
    ///                                               ///
    /// Methods overridden from GamepadInputProcessor ///
    ///                                               ///
    /////////////////////////////////////////////////////

    /// @brief Callback for a gamepad button event
    /// @param button Literal describing which key triggered the event
    /// @param action Literal describing what action was performed on
    /// the button which triggered the event
    virtual void processButton(const Gamepad&, const Button button, const Action action) override {
        if (button == Button::A) {
            _sprint = (action == Action::Press);
        }
    }

    /// @brief Callback for a gamepad axis event
    /// @param axis Literal describing which axis triggered the event
    /// @param value Value at which the axis was polled
    virtual void processAxis(const Gamepad&, const Axis axis, const float value) override {
        if (axis == Axis::LeftX) {
            _direction.x = -value;
        }

        if (axis == Axis::LeftY) {
            _direction.y = -value;
        }
    }

};

} // namespace rb

#endif//RENDERBOI_TOOLBOX_RUNNABLES_GAMEPAD_MOVEMENT_SCRIPT_HPP
#ifndef RENDERBOI_TOOLBOX_RUNNABLES_BASIC_INPUT_MANAGER_HPP
#define RENDERBOI_TOOLBOX_RUNNABLES_BASIC_INPUT_MANAGER_HPP

#include <algorithm>
#include <string_view>

#include <cpptools/utility/predicate.hpp>

#include <renderboi/toolbox/controls/control_scheme.hpp>
#include <renderboi/toolbox/interfaces/action_event_receiver.hpp>
#include <renderboi/toolbox/interfaces/default_control_scheme_provider.hpp>

#include <renderboi/window/gl_window.hpp>
#include <renderboi/window/input_processor.hpp>

namespace rb {

enum class BasicWindowManagerAction {
    Terminate,
    PolygonFill,
    PolygonLine,
    PolygonPoint,
    ToggleFullscreen
};

enum class PolygonMode {
    Fill,
    Line,
    Point
};

/// @brief Provides event callbacks associated with basic window management
class BasicWindowManager : 
    public InputProcessor,
    public DefaultControlSchemeProvider<BasicWindowManagerAction>,
    public ActionEventReceiver<BasicWindowManagerAction> {
private:
    using GLWindow = Window::GLWindow;

    BasicWindowManager(const BasicWindowManager&) = default;
    BasicWindowManager(BasicWindowManager&&) = default;
    BasicWindowManager& operator=(const BasicWindowManager&) = delete;
    BasicWindowManager& operator=(BasicWindowManager&&) = delete;

    /// @brief Toggles the fullscreen state of the managed window
    void _toggleFullscreen() const;

    /// @brief Queues an event to set the polygon mode of the render context
    void _setPolygonMode(const PolygonMode mode) const;

    /// @brief Windows on which the actions should be carried out
    GLWindow& _window;

public:
    using ActionType = BasicWindowManagerAction;

    BasicWindowManager(GLWindow& window);

    //////////////////////////////////////////////
    ///                                        ///
    /// Methods overridden from InputProcessor ///
    ///                                        ///
    //////////////////////////////////////////////

    /// @brief Callback for a framebuffer resize event
    ///
    /// @param window Reference to the GLWindow in which the event was
    /// triggered
    /// @param width New width (in pixels) of the framebuffer
    /// @param height New height (in pixels) of the framebuffer
    void processFramebufferResize(GLWindow& window, const unsigned int width, const unsigned int height) override;

    ////////////////////////////////////////////////////////////////////////////////////
    ///                                                                              ///
    /// Methods overridden from DefaultControlSchemeProvider<KeyboardMovementAction> ///
    ///                                                                              ///
    ////////////////////////////////////////////////////////////////////////////////////

    /// @brief Get the default control scheme for the keyboard movement
    /// script
    ///
    /// @return The default control scheme for the keyboard movement script
    const ControlScheme<BasicWindowManagerAction>& defaultControlScheme() const override;

    ////////////////////////////////////////////////////////////////
    ///                                                          ///
    ///                  Methods overridden from                 ///
    /// ActionEventReceiver<BasicWindowManagerAction, GLWindow&> ///
    ///                                                          ///
    ////////////////////////////////////////////////////////////////

    /// @brief Start the processing for an action
    ///
    /// @param action Object describing the action to start processing
    void triggerAction(const BasicWindowManagerAction& action) override;

    /// @brief Stop the processing for an action
    ///
    /// @param action Object describing the action to stop processing
    void stopAction(const BasicWindowManagerAction& action) override;

    ///////////////////////////////////////////////////////////
    ///                                                     ///
    /// Methods overridden from ArgumentProvider<GLWindow&> ///
    ///                                                     ///
    ///////////////////////////////////////////////////////////
};

inline std::string_view to_string(const BasicWindowManagerAction action) {
    static const std::array<std::pair<BasicWindowManagerAction, std::string_view>, 5> enumNames = {{
        { BasicWindowManagerAction::Terminate,          "Terminate"        },
        { BasicWindowManagerAction::PolygonFill,        "PolygonFill"      },
        { BasicWindowManagerAction::PolygonLine,        "PolygonLine"      },
        { BasicWindowManagerAction::PolygonPoint,       "PolygonPoint"     },
        { BasicWindowManagerAction::ToggleFullscreen,   "ToggleFullscreen" },
    }};

    using tools::pred::first_member;
    using tools::pred::equals;

    auto it = std::ranges::find_if(enumNames, first_member(equals(action)));
    return (it != enumNames.end())
        ? it->second
        : "Unknown";   
}

} // namespace rb

#endif//RENDERBOI_TOOLBOX_RUNNABLES_BASIC_INPUT_MANAGER_HPP
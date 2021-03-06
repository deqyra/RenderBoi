#ifndef RENDERBOI__TOOLBOX__RUNNABLES__BASIC_INPUT_MANAGER_HPP
#define RENDERBOI__TOOLBOX__RUNNABLES__BASIC_INPUT_MANAGER_HPP

#include <string>

#include <renderboi/window/input_processor.hpp>

#include "../controls/control_scheme_manager.hpp"
#include "../interfaces/action_event_receiver.hpp"
#include "../interfaces/default_control_scheme_provider.hpp"

namespace Renderboi
{

enum class BasicWindowManagerAction
{
    Terminate,
    PolygonFill,
    PolygonLine,
    PolygonPoint,
    ToggleFullscreen
};

enum class PolygonMode
{
    Fill,
    Line,
    Point
};


/// @brief Provides event callbacks associated with basic window management.
class BasicWindowManager :  public InputProcessor,
                            public ActionEventReceiver<BasicWindowManagerAction>,
                            public DefaultControlSchemeProvider<BasicWindowManagerAction>
{
private:
    BasicWindowManager(const BasicWindowManager& other) = delete;
    BasicWindowManager& operator=(const BasicWindowManager& other) = delete;

    /// @brief Toggles the fullscreen state of the managed window.
    void _toggleFullscreen(const GLWindowPtr window) const;

    /// @brief Queues an event to set the polygon mode of the render context.
    void _setPolygonMode(const GLWindowPtr window, const PolygonMode mode) const;

public:
    using ActionType = BasicWindowManagerAction;

    BasicWindowManager();

    /////////////////////////////////////////////////////////////////////////////
    ///                                                                       ///
    /// Methods overridden from ActionEventReceiver<BasicWindowManagerAction> ///
    ///                                                                       ///
    /////////////////////////////////////////////////////////////////////////////

    /// @brief Start the processing for an action.
    ///
    /// @param action Object describing the action to start processing.
    void triggerAction(const GLWindowPtr window, const BasicWindowManagerAction& action) override;

    /// @brief Stop the processing for an action.
    ///
    /// @param action Object describing the action to stop processing.
    void stopAction(const GLWindowPtr window, const BasicWindowManagerAction& action) override;

    //////////////////////////////////////////////
    ///                                        ///
    /// Methods overridden from InputProcessor ///
    ///                                        ///
    //////////////////////////////////////////////

    /// @brief Callback for a framebuffer resize event.
    ///
    /// @param window Pointer to the GLWindow in which the event was
    /// triggered.
    /// @param width New width (in pixels) of the framebuffer.
    /// @param height New height (in pixels) of the framebuffer.
    void processFramebufferResize(const GLWindowPtr window, const unsigned int width, const unsigned int height) override;

    ////////////////////////////////////////////////////////////////////////////////////
    ///                                                                              ///
    /// Methods overridden from DefaultControlSchemeProvider<KeyboardMovementAction> ///
    ///                                                                              ///
    ////////////////////////////////////////////////////////////////////////////////////

    /// @brief Get the default control scheme for the keyboard movement
    /// script.
    ///
    /// @return The default control scheme for the keyboard movement script.
    ControlSchemeManagerPtr<BasicWindowManagerAction> getDefaultControlScheme() const override;
};

std::string to_string(const BasicWindowManagerAction& action);

}//namespace Renderboi

#endif//RENDERBOI__TOOLBOX__RUNNABLES__BASIC_INPUT_MANAGER_HPP
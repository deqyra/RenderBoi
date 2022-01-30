#ifndef RENDERBOI__TOOLBOX__RUNNABLES__BASIC_INPUT_MANAGER_HPP
#define RENDERBOI__TOOLBOX__RUNNABLES__BASIC_INPUT_MANAGER_HPP

#include <string>

#include <cpptools/oo/interfaces/action_event_receiver.hpp>

#include <renderboi/window/input_processor.hpp>

#include "../controls/control_scheme.hpp"
#include "../interfaces/default_control_scheme_provider.hpp"
#include "cpptools/oo/interfaces/argument_provider.hpp"
#include "renderboi/window/gl_window.hpp"

namespace renderboi
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
class BasicWindowManager : 
    public InputProcessor,
    public DefaultControlSchemeProvider<BasicWindowManagerAction>,
    public cpptools::ActionEventReceiver<BasicWindowManagerAction>
{
private:
    using GLWindow = Window::GLWindow;

    BasicWindowManager(const BasicWindowManager& other) = delete;
    BasicWindowManager& operator=(const BasicWindowManager& other) = delete;

    /// @brief Toggles the fullscreen state of the managed window.
    void _toggleFullscreen() const;

    /// @brief Queues an event to set the polygon mode of the render context.
    void _setPolygonMode(const PolygonMode mode) const;

    /// @brief Windows on which the actions should be carried out.
    GLWindow& _window;

public:
    BasicWindowManager(GLWindow& window);

    //////////////////////////////////////////////
    ///                                        ///
    /// Methods overridden from InputProcessor ///
    ///                                        ///
    //////////////////////////////////////////////

    /// @brief Callback for a framebuffer resize event.
    ///
    /// @param window Reference to the GLWindow in which the event was
    /// triggered.
    /// @param width New width (in pixels) of the framebuffer.
    /// @param height New height (in pixels) of the framebuffer.
    void processFramebufferResize(GLWindow& window, const unsigned int width, const unsigned int height) override;

    ////////////////////////////////////////////////////////////////////////////////////
    ///                                                                              ///
    /// Methods overridden from DefaultControlSchemeProvider<KeyboardMovementAction> ///
    ///                                                                              ///
    ////////////////////////////////////////////////////////////////////////////////////

    /// @brief Get the default control scheme for the keyboard movement
    /// script.
    ///
    /// @return The default control scheme for the keyboard movement script.
    const ControlScheme<BasicWindowManagerAction>& getDefaultControlScheme() const override;

    ////////////////////////////////////////////////////////////////
    ///                                                          ///
    ///                  Methods overridden from                 ///
    /// ActionEventReceiver<BasicWindowManagerAction, GLWindow&> ///
    ///                                                          ///
    ////////////////////////////////////////////////////////////////

    /// @brief Start the processing for an action.
    ///
    /// @param action Object describing the action to start processing.
    void triggerAction(const BasicWindowManagerAction& action) override;

    /// @brief Stop the processing for an action.
    ///
    /// @param action Object describing the action to stop processing.
    void stopAction(const BasicWindowManagerAction& action) override;

    ///////////////////////////////////////////////////////////
    ///                                                     ///
    /// Methods overridden from ArgumentProvider<GLWindow&> ///
    ///                                                     ///
    ///////////////////////////////////////////////////////////
};

std::string to_string(const BasicWindowManagerAction action);

} // namespace renderboi

#endif//RENDERBOI__TOOLBOX__RUNNABLES__BASIC_INPUT_MANAGER_HPP
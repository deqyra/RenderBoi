#ifndef RENDERBOI_WINDOW_GL_WINDOW_HPP
#define RENDERBOI_WINDOW_GL_WINDOW_HPP

#include <atomic>
#include <memory>
#include <string>

#include "enums.hpp"
#include "input_processor.hpp"
#include "monitor.hpp"
#include "event/gl_context_event_manager.hpp"
#include "gamepad/gamepad_manager.hpp"

namespace rb {
    
namespace Window {

/// @brief Abstract window, supporting event callbacks through a custom 
/// InputProcessor
class GLWindow {
protected:
    /// @param title Title of the window
    GLWindow(std::string title);

    /// @param title Title of the window
    /// @param contextEventManager Reference to the context event manager of the
    /// window
    GLWindow(std::string title, GLContextEventManager& contextEventManager);

public:
    virtual ~GLWindow() = default;

    ////////////////////////////
    ///   INPUT FORWARDING   ///
    ////////////////////////////

    /// @brief Callback for a framebuffer resize event
    ///
    /// @param width New width (in pixels) of the framebuffer
    /// @param height New height (in pixels) of the framebuffer
    virtual void processFramebufferResize(const int width, const int height);

    /// @brief Callback for a keyboard event
    ///
    /// @param key Literal describing which key triggered the event
    /// @param scancode Scancode of the key which triggered the event 
    /// Platform-dependent, but consistent over time
    /// @param action Literal describing what action was performed on
    /// the key which triggered the event
    /// @param mods Bit field describing which modifiers were enabled 
    /// during the key event (Ctrl, Shift, etc)
    virtual void processKeyboard(
        const Window::Input::Key key,
        const int scancode,
        const Window::Input::Action action,
        const int mods
    );

    /// @brief Callback for a mouse button event
    ///
    /// @param button Literal describing which button triggered the
    /// event
    /// @param action Literal describing what action was performed on
    /// the button which triggered the event
    /// @param mods Bit field describing which modifiers were enabled 
    /// during the button event (Ctrl, Shift, etc)
    virtual void processMouseButton(
        const Window::Input::MouseButton button,
        const Window::Input::Action action,
        const int mods
    );

    /// @brief Callback for a mouse cursor event
    ///
    /// @param xpos X coordinate of the new position of the mouse
    /// @param ypos Y coordinate of the new position of the mouse
    virtual void processMouseCursor(const double xpos, const double ypos);

    /////////////////////////////
    ///   WINDOW MANAGEMENT   ///
    /////////////////////////////

    /// @brief Get the title of the window. May be called from any thread
    ///
    /// @return The title of the window
    virtual std::string getTitle() const;

    /// @brief Set the title of the window May only be called from the main
    /// thread
    ///
    /// @return The title of the window
    virtual void setTitle(std::string title) = 0;

    /// @brief Tells whether an exit signal was sent to the window
    virtual bool exitSignaled();

    /// @brief Send an exit signal to the window
    ///
    /// @param value Whether the exit signal sent is positive or negative
    virtual void signalExit(const bool value = true);

    /// @brief Get a pointer to the entity which manages gamepads May be called
    /// from any thread
    ///
    /// @return A pointer to the gamepad manager
    virtual GamepadManager& getGamepadManager();

    /// @brief Register an entity to which context events should be forwarded
    /// when they occur
    ///
    /// @param context Pointer to the context event manager
    ///
    /// @exception If the provided pointer is nullptr, the function will throw
    /// an std::runtime_error
    void registerContextEventManager(std::unique_ptr<GLContextEventManager>&& contextEventManager);

    /// @brief Discard the registered custom context event manager This
    /// restores the default context event manager
    virtual void detachContextEventManager();

    /// @brief Forward a context event which occurred to the registered context
    /// client
    ///
    /// @param event Literal describing the event to forward to the registered
    /// context client
    void forwardContextEvent(const GLContextEvent event);

    /// @brief Register an input processor to the window Input events 
    /// will be forwarded to it
    ///
    /// @param inputProcessor Reference to the input processor to register
    /// to the window
    ///
    /// @exception If the provided pointer is null, the function will 
    /// throw a std::runtime_error
    virtual void registerInputProcessor(InputProcessor& inputProcessor);

    /// @brief Get a pointer to the input processor of the window
    ///
    /// @return A pointer to the input processor of the window
    virtual const InputProcessor& getInputProcessor();

    /// @brief Discard the currently registered custom input processor This
    /// restores the default input processor
    virtual void detachInputProcessor();

    /////////////////////////
    ///   INPUT POLLING   ///
    /////////////////////////

    /// @brief Process all context events that were registered May be called
    /// from the rendering thread only, as this may in turn issue gl* calls
    void processPendingContextEvents();

    /// @brief Poll input and process queued events May only be called from the
    /// main thread
    void pollAllEvents();

    /// @brief Call pollAllEvents() in a loop until requested to exit May only
    /// be called from the main thread
    ///
    /// @see signalExit()
    /// @see exitSignaled()
    void startPollingLoop();

    ////////////////////////////
    ///   ABSTRACT METHODS   ///
    ////////////////////////////

    /// @brief Set the input mode of a certain target in the window May only be
    /// called from the main thread
    ///
    /// @param target Literal describing which aspect of the window whose
    /// input mode should be set
    /// @param value Literal describing which input to set the target to
    virtual void setInputMode(const Input::Mode::Target target, const Input::Mode::Value value) = 0;

    /// @brief Hide the window. May be called only from the main thread
    virtual void hide() = 0;

    /// @brief Show the window. May be called only from the main thread
    virtual void show() = 0;

    /// @brief Whether the window is visible May be called only from the main 
    /// thread
    ///
    /// @return Whether the window is visible
    virtual bool isVisible() const = 0;

    /// @brief Bring focus to the window May be called only from the main 
    /// thread
    virtual void focus() = 0;

    /// @brief Whether the window is visible May be called only from the main 
    /// thread
    ///
    /// @return Whether the window is visible
    virtual bool isFocused() const = 0;

    /// @brief Maximize the window. May be called only from the main thread
    virtual void maximize() = 0;

    /// @brief Whether or not the window is maximized May be called only from 
    /// the main thread
    ///
    /// @return Whether or not the window is maximized
    virtual bool isMaximized() const = 0;

    /// @brief Minimize the window. May be called only from the main thread
    virtual void minimize() = 0;

    /// @brief Whether or not the window is minimized May be called only from 
    /// the main thread
    ///
    /// @return Whether or not the window is minimized
    virtual bool isMinimized() const = 0;

    /// @brief Retrieve the width and height of the window in screen coordinates
    ///
    /// @param[out] width Will receive the width of the window
    /// @param[out] height Will receive the height of the window
    virtual void getSize(int& width, int& height) const = 0;

    /// @brief Retrieve the width and height of the framebuffer in pixels
    ///
    /// @param[out] width Will receive the width of the framebuffer
    /// @param[out] height Will receive the height of the framebuffer
    virtual void getFramebufferSize(int& width, int& height) const = 0;

    /// @brief Display the window in fullscreen on the primary monitor May be
    /// called only from the main thread
    ///
    /// @param borderless Whether or not to go borderless fullscreen When 
    /// enabled, this makes the window fit exactly the video mode of the monitor
    /// it is on. Otherwise, the attributes of the window are left untouched
    virtual void goFullscreen(const bool borderless = false) = 0;

    /// @brief Display the window in fullscreen on the specified monitor May be
    /// called only from the main thread
    ///
    /// @param monitor Monitor on which to make the window go fullscreen
    /// @param borderless Whether or not to go borderless fullscreen When 
    /// enabled, this makes the window fit exactly the video mode of the monitor
    /// it is on. Otherwise, the attributes of the window are left untouched
    virtual void goFullscreen(Monitor& monitor, const bool borderless = false) = 0;

    /// @brief Display the window in fullscreen on the primary monitor The
    /// window will switch to the video mode which is the closest to the 
    /// requested parameters. May be called only from the main thread
    ///
    /// @param width Desired width of the video mode
    /// @param height Desired height of the video mode
    /// @param refreshRate Desired refresh rate
    virtual void goFullscreen(
        const int width = -1,
        const int height = -1,
        const int refreshRate = -1
    ) = 0;

    /// @brief Display the window in fullscreen on the specified monitor The
    /// window will switch to the video mode which is the closest to the 
    /// indicated parameters. May be called only from the main thread
    ///
    /// @param monitor Monitor on which to make the window go fullscreen
    /// @param width Desired width of the video mode
    /// @param height Desired height of the video mode
    /// @param refreshRate Desired refresh rate
    virtual void goFullscreen(
        Monitor& monitor,
        const int width = -1,
        const int height = -1,
        const int refreshRate = -1
    ) = 0;

    /// @brief Whether or not the window is displayed in fullscreen mode May be
    /// called only from the main thread
    ///
    /// @return Whether or not the window is displayed in fullscreen mode
    virtual bool isFullscreen() const = 0;

    /// @brief Display the window in windowed mode May be called only from the
    /// main thread
    virtual void exitFullscreen() = 0;

    /// @brief Set the refresh rate of a fullscreen window Has no effect on a
    /// window displayed in windowed mode May be called only from the main 
    /// thread
    virtual void setRefreshRate(const int rate) = 0;

    /// @brief Whether the window was flagged for closing May be called from
    /// any thread
    ///
    /// @return Whether or not the window was flagged for closing
    virtual bool shouldClose() const = 0;

    /// @brief Set the window closing flag. May be called from any thread
    ///
    /// @param value Whether or not the window should be flagged for closing
    virtual void setShouldClose(const bool value) = 0;

    /// @brief Swap the front and back buffers of the window May be called from
    /// any thread
    virtual void swapBuffers() = 0;

    /// @brief Poll the event queue of the window May only be called from the 
    /// main thread
    virtual void pollEvents() const = 0;
    
    /// @brief Get the aspect ratio of the framebuffer used by the window
    ///
    /// @return The aspect ratio of the framebuffer used by the window
    virtual float getAspectRatio() const = 0;

    /// @brief Get position of the mouse cursor in the window
    ///
    /// @param x [Output parameter] The X coordinate of the cursor
    /// @param y [Output parameter] The Y coordinate of the cursor
    virtual void getCursorPos(double& x, double& y) const = 0;

    /// @brief Make the GL context current for the calling thread May be called
    /// from any thread
    virtual void makeContextCurrent() = 0;

    /// @brief Make the GL context non-current for the calling thread May be 
    /// called from any thread
    virtual void releaseContext() = 0;

    /// @brief Tell whether the GL context supports a certain extension A GL 
    /// context must be current on the calling thread
    ///
    /// @param extName String containing the name of the extension to query
    virtual bool extensionSupported(const std::string extName) = 0;

protected:
    /// @brief Default input processor of all windows
    static const InputProcessorPtr _DefaultInputProcessor;

    /// @brief Title of the window
    std::string _title;

    /// @brief Flag to indicate exiting the event polling loop
    std::atomic<bool> _exitSignaled;

    /// @brief Entity to manage gamepads Must be initialized at construction
    /// by inheriting classes
    GamepadManagerPtr _gamepadManager;

    /// @brief Entity to which context events will be forwarded when received
    std::unique_ptr<GLContextEventManager> _glContextEventManager;

    /// @brief Custom input processor to be registered at any point, 
    /// providing custom callbacks
    InputProcessor* _inputProcessor;

private:
};

using GLWindowPtr = std::unique_ptr<GLWindow>;

} // namespace Window

using GLWindow = Window::GLWindow;
using GLWindowPtr = std::unique_ptr<GLWindow>;

} // namespace rb

#endif//RENDERBOI_WINDOW_GL_WINDOW_HPP

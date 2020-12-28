#ifndef RENDERBOI__WINDOW__INPUT_PROCESSOR_HPP
#define RENDERBOI__WINDOW__INPUT_PROCESSOR_HPP

#include <memory>
#include <string>
#include <vector>

#include "enums.hpp"

class GLWindow;
using GLWindowPtr = std::shared_ptr<GLWindow>;

/// @brief Callback manager to handle input from a GLWindow.
class InputProcessor
{
    private:
        /// @brief Keeps track of how many instances were created (used as a 
        /// unique ID system).
        static unsigned int _count;

    public:
        /// @brief Collection of litterals describing the different event types
        /// handled by an InputProcessor.
        enum class EventType
        {
            FramebufferResize,
            Keyboard,
            MouseButton,
            MouseCursor
        };

        /// @brief Array listing all of the litterals defined by the EventType
        /// enum.
        static const std::vector<EventType> EventTypeList;

        InputProcessor();

        /// @brief Callback for a framebuffer resize event.
        ///
        /// @param window Pointer to the GLWindow in which the event was
        /// triggered.
        /// @param width New width (in pixels) of the framebuffer.
        /// @param height New height (in pixels) of the framebuffer.
        virtual void processFramebufferResize(const GLWindowPtr window, const int width, const int height);

        /// @brief Callback for a keyboard event.
        ///
        /// @param window Pointer to the GLWindow in which the event was
        /// triggered.
        /// @param key Literal describing which key triggered the event.
        /// @param scancode Scancode of the key which triggered the event. 
        /// Platform-dependent, but consistent over time.
        /// @param action Literal describing what action was performed on
        /// the key which triggered the event.
        /// @param mods Bit field describing which modifiers were enabled 
        /// during the key event (Ctrl, Shift, etc).
        virtual void processKeyboard(
            const GLWindowPtr window,
            const Window::Input::Key key,
            const int scancode,
            const Window::Input::Action action,
            const int mods
        );

        /// @brief Callback for a mouse button event.
        ///
        /// @param window Pointer to the GLWindow in which the event was
        /// triggered.
        /// @param button Literal describing which button triggered the
        /// event.
        /// @param action Literal describing what action was performed on
        /// the button which triggered the event.
        /// @param mods Bit field describing which modifiers were enabled 
        /// during the button event (Ctrl, Shift, etc).
        virtual void processMouseButton(
            const GLWindowPtr window,
            const Window::Input::MouseButton button,
            const Window::Input::Action action,
            const int mods
        );

        /// @brief Callback for a mouse cursor event.
        ///
        /// @param window Pointer to the GLWindow in which the event was
        /// triggered.
        /// @param xpos X coordinate of the new position of the mouse.
        /// @param ypos Y coordinate of the new position of the mouse.
        virtual void processMouseCursor(const GLWindowPtr window, const double xpos, const double ypos);

        /// @brief Unique ID of the input processor.
        const unsigned int id;
};

using InputProcessorPtr = std::shared_ptr<InputProcessor>;
using InputProcessorWPtr = std::weak_ptr<InputProcessor>;

#endif//RENDERBOI__WINDOW__INPUT_PROCESSOR_HPP

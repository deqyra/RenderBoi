#ifndef RENDERBOI_WINDOW_EVENT_GL_CONTEXT_EVENT_MANAGER_HPP
#define RENDERBOI_WINDOW_EVENT_GL_CONTEXT_EVENT_MANAGER_HPP

#include <mutex>
#include <queue>

#include <renderboi/toolbox/interfaces/event_receiver.hpp>

#include "gl_context_event.hpp"

namespace rb {

namespace Window {

class GLWindow;

/// @brief Implementation of a class able to receive render events which were
/// queued to a thread running on a GL context
///
/// @tparam E Type of the events to be received 
class GLContextEventManager : public EventReceiver<GLContextEvent> {
public:
    /// @param window Window in charge of painting the context whose events will
    /// be received
    GLContextEventManager(GLWindow& window);

    /// @brief Process all events that were registered, and clear the
    /// queue May be called from the rendering thread only, as this may in 
    /// turn issue gl* calls
    virtual void processPendingEvents();

    /////////////////////////////////////////////
    ///                                       ///
    /// Methods overridden from EventReceiver ///
    ///                                       ///
    /////////////////////////////////////////////

    /// @brief Register an event to the entity
    ///
    /// @param event Literal describing the event to queue
    void queueEvent(const GLContextEvent& event) override;

protected:
    /// @brief Process a single event in the queue
    ///
    /// @param event Literal describing the event to process
    void _processEvent(const GLContextEvent event);

    /// @brief Window in charge of painting the context whose events will
    /// be received
    GLWindow& _window;

    /// @brief Mutex for accessing the event queue
    std::mutex _eventMutex;

    /// @brief Queue for events
    std::queue<GLContextEvent> _eventQueue;
};

}// namespace Window

}// namespace rb


#endif//RENDERBOI_WINDOW_EVENT_GL_CONTEXT_EVENT_MANAGER_HPP
#ifndef RENDERBOI_TOOLBOX_INTERFACES_EVENT_RECEIVER_HPP
#define RENDERBOI_TOOLBOX_INTERFACES_EVENT_RECEIVER_HPP

/// @brief Interface for a class able to receive events and queue them.
///
/// @tparam E Type of events to be queued. 
template<typename E>
class EventReceiver
{
public:
    virtual ~EventReceiver() = default;

    /// @brief Register a critical event to the window.
    ///
    /// @param event Literal describing the event to queue.
    virtual void queueEvent(const E& event) = 0;
};

#endif//RENDERBOI_TOOLBOX_INTERFACES_EVENT_RECEIVER_HPP
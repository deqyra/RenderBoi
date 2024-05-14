#ifndef RENDERBOI_TOOLBOX_INTERFACES_ACTION_EVENT_RECEIVER
#define RENDERBOI_TOOLBOX_INTERFACES_ACTION_EVENT_RECEIVER

/// @brief Interface for an object able to receive and process events related to
/// an action of a certain type.
///
/// @tparam T Type of the action to be handled by the implementing class.
template<typename T>
class ActionEventReceiver {
public:
    using action_type = T;

    virtual ~ActionEventReceiver() = default;

    /// @brief Start the processing for an action.
    ///
    /// @param action Object describing the action to start processing.
    virtual void triggerAction(const T& action) = 0;

    /// @brief Stop the processing for an action.
    ///
    /// @param action Object describing the action to stop processing.
    virtual void stopAction(const T& action) = 0;
};

#endif//RENDERBOI_TOOLBOX_INTERFACES_ACTION_EVENT_RECEIVER

#ifndef RENDERBOI_TOOLBOX_SCENE_SCRIPT_HPP
#define RENDERBOI_TOOLBOX_SCENE_SCRIPT_HPP

namespace rb {

/// @brief Abstract class to autonomously update scene objects
class Script {
public:
    Script(const Script&) = default;
    Script(Script&&) = default;
    Script& operator=(const Script&) = default;
    Script& operator=(Script&&) = default;

    Script() = default;
    virtual ~Script() = default;

    /// @brief Make the script run and do its things
    ///
    /// @param timeElapsed How much time passed (in seconds) since the last
    /// update
    virtual void update(const float timeElapsed) = 0;
};

} // namespace rb

#endif//RENDERBOI_TOOLBOX_SCENE_SCRIPT_HPP
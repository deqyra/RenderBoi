#ifndef RENDERBOI__TOOLBOX__SCENE__SCRIPT_HPP
#define RENDERBOI__TOOLBOX__SCENE__SCRIPT_HPP

#include <memory>

#include "scene/object/scene_object.hpp"

namespace renderboi
{

/// @brief Abstract class to autonomously update scene objects.
class Script
{
    private:
        Script(const Script& other) = delete;
        Script& operator=(const Script& other) = delete;

        // Keeps track of how many script instances were created (used as a 
        /// unique ID system).
        static unsigned int _count;

    protected:
        /// @brief Reference to a scene object which the script might be attached
        /// to.
        SceneObject* _sceneObject;

    public:
        Script();

        virtual ~Script();

        /// @brief Make the script run and do its things.
        ///
        /// @param timeElapsed How much time passed (in seconds) since the last
        /// update.
        virtual void update(const float timeElapsed) = 0;

        /// @brief Get a pointer to the parent scene object, if any.
        ///
        /// @return A pointer to the parent scene object, or nullptr.
        virtual SceneObject* sceneObject() const;

        /// @brief Set the parent scene object of the script.
        ///
        /// @param object A pointer to scene object which should be parent to 
        /// the script (can be nullptr).
        virtual void setSceneObject(SceneObject* const object);

        /// @brief Get a raw pointer to a new script instance cloned 
        /// from this one. Ownership and responsibility for the allocated 
        /// resources are fully transferred to the caller.
        ///
        /// @return A raw pointer to the script instance cloned from this one.
        virtual Script* clone() const = 0;

        /// @brief Unique ID of the script.
        const unsigned int id;
};

using ScriptPtr = std::unique_ptr<Script>;

} // namespace renderboi

#endif//RENDERBOI__TOOLBOX__SCENE__SCRIPT_HPP
#ifndef RENDERBOI_TOOLBOX_SCENE_COMPONENTS_BASIC_COMPONENT_HPP
#define RENDERBOI_TOOLBOX_SCENE_COMPONENTS_BASIC_COMPONENT_HPP

namespace rb {

template<typename T>
struct BasicComponent {
    T value;

    inline operator T&() { return value; }

    inline operator const T&() const { return value; }
};

template<>
struct BasicComponent<void> {};

} // namespace rb

#endif// RENDERBOI_TOOLBOX_SCENE_COMPONENTS_BASIC_COMPONENT_HPP

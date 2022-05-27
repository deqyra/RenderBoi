#ifndef RENDERBOI__TOOLBOX__SCENE__COMPONENTS__BASIC_COMPONENT_HPP
#define RENDERBOI__TOOLBOX__SCENE__COMPONENTS__BASIC_COMPONENT_HPP

namespace renderboi::component
{

template<typename T>
struct BasicComponent
{
    T value;

    inline operator T&()
    {
        return value;
    }

    inline operator const T&() const
    {
        return value;
    }
};

} // namespace renderboi::component

#endif//RENDERBOI__TOOLBOX__SCENE__COMPONENTS__BASIC_COMPONENT_HPP
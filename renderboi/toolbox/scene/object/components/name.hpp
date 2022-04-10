#ifndef RENDERBOI__TOOLBOX__SCENE__COMPONENTS__NAME_HPP
#define RENDERBOI__TOOLBOX__SCENE__COMPONENTS__NAME_HPP

#include <string>

#include "basic_component.hpp"

namespace renderboi::component
{

struct Name : public BasicComponent<std::string> { };

} // namespace renderboi::component

#endif//RENDERBOI__TOOLBOX__SCENE__COMPONENTS__NAME_HPP
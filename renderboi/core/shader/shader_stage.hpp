#ifndef RENDERBOI__CORE__SHADER__SHADER_STAGE_HPP
#define RENDERBOI__CORE__SHADER__SHADER_STAGE_HPP

#include <string>
#include <unordered_map>

namespace renderboi
{

enum class ShaderStage
{
    Vertex,
    Geometry,
    Fragment
};

std::string to_string(const ShaderStage v);

} // namespace renderboi

#endif//RENDERBOI__CORE__SHADER__SHADER_STAGE_HPP
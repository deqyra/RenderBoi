#ifndef RENDERBOI_CORE_SHADER_SHADER_STAGE_HPP
#define RENDERBOI_CORE_SHADER_SHADER_STAGE_HPP

#include <string>

namespace rb {

enum class ShaderStage {
    Vertex,
    Geometry,
    Fragment
};

std::string to_string(const ShaderStage v);

} // namespace rb

#endif//RENDERBOI_CORE_SHADER_SHADER_STAGE_HPP
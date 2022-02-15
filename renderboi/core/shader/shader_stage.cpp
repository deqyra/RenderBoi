#include "shader_stage.hpp"

#include <string>
#include <unordered_map>

#include <glad/gl.h>

namespace renderboi
{

std::string to_string(const ShaderStage v)
{
    static std::unordered_map<ShaderStage, std::string> _stageNames = {
        {ShaderStage::Vertex,    "Vertex"},
        {ShaderStage::Geometry,  "Geometry"},
        {ShaderStage::Fragment,  "Fragment"},
    };

    auto it = _stageNames.find(v);
    return (it != _stageNames.end())
        ? it->second
        : "Unknown shader stage";
}

} // namespace renderboi

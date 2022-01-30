#include "component_type.hpp"

namespace renderboi
{

std::string to_string(const ComponentType type)
{
    switch (type)
    {
    case ComponentType::Camera:
        return "Camera";
        
    case ComponentType::Mesh:
        return "Mesh";
        
    case ComponentType::Light:
        return "Light";
        
    case ComponentType::Script:
        return "Script";

    default:
        return "Unknown";
    }
}

}
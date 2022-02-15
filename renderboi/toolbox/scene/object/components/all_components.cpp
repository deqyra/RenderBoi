#include "all_components.hpp"
#include <stdexcept>
#include <string>

namespace renderboi
{

bool Component::MultipleInstancesAllowed(const ComponentType type)
{
    switch (type)
    {
    case ComponentType::Camera:
        return ComponentMeta<ComponentType::Camera>
        ::MultipleInstancesAllowed::value;
        
    case ComponentType::Mesh:
        return ComponentMeta<ComponentType::Mesh>
        ::MultipleInstancesAllowed::value;
        
    case ComponentType::Light:
        return ComponentMeta<ComponentType::Light>
        ::MultipleInstancesAllowed::value;
        
    case ComponentType::Script:
        return ComponentMeta<ComponentType::Script>
        ::MultipleInstancesAllowed::value;

    default:
        std::string s = "Component: cannot tell whether multiple instances are"
        "allowed for unknown component type " + std::to_string((int)type) + ".";

        throw std::runtime_error(s.c_str());
    }
}

}
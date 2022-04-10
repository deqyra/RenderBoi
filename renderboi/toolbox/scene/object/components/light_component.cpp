#include "light_component.hpp"

#include <stdexcept>

#include "../scene_object.hpp"

namespace renderboi
{

LightComponent::LightComponent(LightPtr&& light) :
    _lightPtr(std::move(light)),
    _light(*_lightPtr)
{
    if (!light)
    {
        throw std::runtime_error("LightComponent: cannot construct from null light pointer.");
    }
}

LightComponent::LightComponent(Light& light) :
    _lightPtr(nullptr),
    _light(light)
{
}

LightComponent::~LightComponent()
{

}

Light& LightComponent::light()
{
    return _light;
}

} // namespace renderboi

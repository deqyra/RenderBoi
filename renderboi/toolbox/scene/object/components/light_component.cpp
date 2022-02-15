#include "light_component.hpp"

#include <stdexcept>

#include "../scene_object.hpp"

namespace renderboi
{

LightComponent::LightComponent(SceneObject& sceneObject, LightPtr&& light) :
    Component(sceneObject),
    _lightPtr(std::move(light)),
    _light(*_lightPtr)
{
    if (!light)
    {
        throw std::runtime_error("LightComponent: cannot construct from null light pointer.");
    }
}

LightComponent::LightComponent(SceneObject& sceneObject, Light& light) :
    Component(sceneObject),
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

LightComponent* LightComponent::clone(SceneObject& newParent) const
{
    return new LightComponent(
        newParent,
        std::move(LightPtr(_light.clone()))
    );
}

} // namespace renderboi

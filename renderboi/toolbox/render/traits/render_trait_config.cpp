#include "render_trait_config.hpp"
#include "renderboi/toolbox/scene/object/object_transform.hpp"

namespace Renderboi
{

RenderTraitConfig::RenderTraitConfig(SceneObject& sceneObject) :
    _sceneObject(sceneObject)
{

}

RenderTraitConfig::~RenderTraitConfig()
{
    
}

SceneObject& RenderTraitConfig::sceneObject()
{
    return _sceneObject;
}

} // namespace Renderboi

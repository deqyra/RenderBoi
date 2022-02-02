#include "render_trait_config.hpp"
#include "renderboi/toolbox/scene/object/object_transform.hpp"

namespace Renderboi
{

RenderTraitConfig::RenderTraitConfig(SceneObjectPtr sceneObject) :
    _sceneObject(sceneObject)
{

}

RenderTraitConfig::~RenderTraitConfig()
{
    
}

void RenderTraitConfig::_release()
{
    _sceneObject.reset();
}

SceneObjectPtr RenderTraitConfig::sceneObject() const
{
    return _sceneObject;
}

} // namespace Renderboi

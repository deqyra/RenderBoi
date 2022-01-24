#include "render_trait_config.hpp"
#include "renderboi/toolbox/scene/object/object_transform.hpp"

namespace Renderboi
{

RenderTraitConfig::RenderTraitConfig(SceneObjectPtr parentSceneObject) :
    _parentSceneObject(parentSceneObject)
{

}

void RenderTraitConfig::_release()
{
    _parentSceneObject.reset();
}

} // namespace Renderboi

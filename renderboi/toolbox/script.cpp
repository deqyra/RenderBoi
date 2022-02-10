#include "script.hpp"

namespace Renderboi
{

unsigned int Script::_count = 0;

Script::Script() :
    id(_count++)
{

}

Script::~Script()
{

}

SceneObject* Script::sceneObject() const
{
    return _sceneObject;
}

void Script::setSceneObject(SceneObject* const sceneObject)
{
    _sceneObject = sceneObject;
}

} // namespace Renderboi

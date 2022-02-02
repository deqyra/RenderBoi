#include "gl_context_client.hpp"

namespace Renderboi
{

namespace Window
{

GLContextClient::GLContextClient(const GLWindowPtr window) :
    _window(window),
    _eventManager(std::make_shared<GLContextEventManager>(window))
{

}

GLContextClient::GLContextClient(const GLWindowPtr window, const GLContextEventManagerPtr eventManager) :
    _window(window),
    _eventManager(eventManager)
{

}

} // namespace Window

} // namespace Renderboi

#ifndef RENDERBOI__TOOLBOX__SCENE__RENDERER__RENDER_TRAIT_CONFIG_HPP
#define RENDERBOI__TOOLBOX__SCENE__RENDERER__RENDER_TRAIT_CONFIG_HPP

#include "render_trait.hpp"

namespace Renderboi
{

template<RenderTrait T>
struct ConfigTypeForRenderTrait
{
    using type = void;
};

} // namespace Renderboi

#endif//RENDERBOI__TOOLBOX__SCENE__RENDERER__RENDER_TRAIT_CONFIG_HPP
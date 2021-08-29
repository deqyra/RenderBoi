#ifndef RENDERBOI__CORE__SHADOW_MAP_HPP
#define RENDERBOI__CORE__SHADOW_MAP_HPP

namespace Renderboi
{

class ShadowMap
{
public:
    /// @param width Horizontal resolution of the shadow map.
    /// @param height Vertical resolution of the shadow map.
    ShadowMap(unsigned int width, unsigned int height);

    /// @brief Horizontal resolution of the shadow map.
    const unsigned int width;

    /// @brief Vertical resolution of the shadow map.
    const unsigned int height;

private:
    void _GenerateShadowMap();
};

}//namespace Renderboi

#endif//RENDERBOI__CORE__SHADOW_MAP_HPP
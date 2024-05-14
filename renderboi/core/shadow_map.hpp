#ifndef RENDERBOI_CORE_SHADOW_MAP_HPP
#define RENDERBOI_CORE_SHADOW_MAP_HPP

namespace rb {

class ShadowMap
{
public:
    /// @param width Horizontal resolution of the shadow map
    /// @param height Vertical resolution of the shadow map
    ShadowMap(unsigned int width, unsigned int height);

    /// @brief Horizontal resolution of the shadow map
    const unsigned int width;

    /// @brief Vertical resolution of the shadow map
    const unsigned int height;

private:
    void _GenerateShadowMap();
};

} // namespace rb

#endif//RENDERBOI_CORE_SHADOW_MAP_HPP
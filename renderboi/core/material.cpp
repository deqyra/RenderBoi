#include "material.hpp"

namespace rb {

void bindTextures(const Material& mat) {
    for (unsigned int i = 0; i < mat.diffuseMaps.size(); i++)
    {
        mat.diffuseMaps[i].bind(i);
    }

    for (unsigned int i = 0; i < mat.specularMaps.size(); i++)
    {
        mat.specularMaps[i].bind(Material::DiffuseMapMaxCount + i);
    }
}

} // namespace rb

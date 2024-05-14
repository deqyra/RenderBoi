#include "light_ubo.hpp"

#include <memory>

#include <glad/gl.h>

#include <renderboi/core/numeric.hpp>

namespace rb {

LightUBO::LightUBO() :
    _ubo(GL_INVALID_INDEX),
    _storage(std::make_unique<storage_t>())
    // _members(...) // <- relies on the default initializer in class body
{
    // Generate the buffer and allocate space
    glGenBuffers(1, &_ubo);
    glBindBuffer(GL_UNIFORM_BUFFER, _ubo);
    glBufferData(GL_UNIFORM_BUFFER, storage_t::Size, NULL, GL_DYNAMIC_DRAW);

    glBindBufferBase(GL_UNIFORM_BUFFER, BindingPoint, _ubo); 
}

UBOLayout<PointLight>& LightUBO::add(const PointLight& pointLight, const num::Vec3& position) {
    return _elements.point.add({ position, pointLight });
}

UBOLayout<SpotLight>& LightUBO::add(const SpotLight& spotLight, const num::Vec3& position) {
    return _elements.spot.add({ position, spotLight });
}

UBOLayout<DirectionalLight>& LightUBO::add(const DirectionalLight& directionalLight) {
    return _elements.directional.add(directionalLight);
}

void LightUBO::commit() const {
    _commitDataToGPU(0, storage_t::Size);
}

void LightUBO::_commitDataToGPU(std::size_t offset, std::size_t byteCount) const {
    glBindBuffer(GL_UNIFORM_BUFFER, _ubo);
    glBufferSubData(
        GL_UNIFORM_BUFFER,
        offset,
        byteCount,
        _storage->data() + offset
    );
}

} // namespace rb

#include "matrix_ubo.hpp"

#include <glad/gl.h>

#include <glm/gtc/type_ptr.hpp>

namespace rb {

MatrixUBO::MatrixUBO() {
    // Generate the buffer and allocate space
    glGenBuffers(1, &_ubo);
    glBindBuffer(GL_UNIFORM_BUFFER, _ubo);
    glBufferData(GL_UNIFORM_BUFFER, storage_t::Size, NULL, GL_STATIC_DRAW);

    // Bind to binding point
    glBindBufferBase(GL_UNIFORM_BUFFER, BindingPoint, _ubo); 
}

void MatrixUBO::setView(const num::Mat4& view) {
    _elements.view = view;
}

void MatrixUBO::setProjection(const num::Mat4& projection) {
    _elements.projection = projection;
}

void MatrixUBO::setModel(const num::Mat4& model) {
    _elements.model = model;
}

void MatrixUBO::setNormal(const num::Mat3& normal) {
    _elements.normal = normal;
}

void MatrixUBO::commitView() {
    _commitDataToGPU(Layout.view.offset, Layout.view.size);
}

void MatrixUBO::commitProjection() {
    _commitDataToGPU(Layout.projection.offset, Layout.projection.size);
}

void MatrixUBO::commitViewProjection() {
    _commitDataToGPU(Layout.view.offset, Layout.view.size + Layout.projection.size);
}

void MatrixUBO::commitModel() {
    _commitDataToGPU(Layout.model.offset, Layout.model.size);
}

void MatrixUBO::commitNormal() {
    _commitDataToGPU(Layout.normal.offset, Layout.normal.size);
}

void MatrixUBO::commitModelNormal() {
    _commitDataToGPU(Layout.model.offset, Layout.model.size + Layout.normal.size);
}

void MatrixUBO::commit() {
    _commitDataToGPU(0, storage_t::Size);
}

void MatrixUBO::_commitDataToGPU(std::size_t offset, std::size_t byteCount) const {
    glBindBuffer(GL_UNIFORM_BUFFER, _ubo);
    glBufferSubData(
        GL_UNIFORM_BUFFER,
        offset,
        byteCount,
        _storage->data() + offset
    );
}

} // namespace rb

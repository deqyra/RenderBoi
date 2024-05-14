#ifndef RENDERBOI_CORE_UBO_MATRIX_UBO_HPP
#define RENDERBOI_CORE_UBO_MATRIX_UBO_HPP

#include <renderboi/core/numeric.hpp>
#include <renderboi/core/ubo/common.hpp>

#include <cpptools/memory/contiguous_storage.hpp>

/* UNIFORM BLOCK LAYOUT
 * ====================
 *
 * layout (std140, binding = 0) uniform Matrices {    // Size // Align // Offset
 *     mat4 view;                                     //   64 //    64 //      0
 *     mat4 projection;                               //   64 //    64 //     64
 *     mat4 model;                                    //   64 //    64 //    128
 *     mat3 normal;                                   //   48 //    64 //    192
 * } matrices;                                        // Size: 240
 *
 * ANY CHANGE TO THIS LAYOUT MUST BE REFLECTED IN MatrixUBO::storage_t BELOW
 **/

namespace rb {

/// @brief Manager for a UBO resource on the GPU, meant for vertex 
/// transformation matrices
class MatrixUBO {
private:
    /// @brief Info about the layout of a the raw storage / UBO
    struct LayoutInfo {
        ValueLayoutInfo view;
        ValueLayoutInfo projection;
        ValueLayoutInfo model;
        ValueLayoutInfo normal;
    };

    /// @brief A convenient access provider to the objects in the raw storage
    struct StorageProxy {
        num::Mat4&   view;
        num::Mat4&   projection;
        num::Mat4&   model;
        num::Mat3x4& normal;
    };

    /// @brief A bunch of uninitialized memory whose layout exactly matches that
    /// of the uniform block storage in GPU memory
    using storage_t = tools::contiguous_storage<
        num::Mat4,
        num::Mat4,
        num::Mat4,
        num::Mat3x4
    >;
    static_assert(storage_t::Size == 240);
    static_assert(storage_t::has_implicit_lifetime);

    /// @brief The handle to the UBO on the GPU
    unsigned int _ubo;

    /// @brief The raw storage, which contains the data to be sent to the GPU
    std::unique_ptr<storage_t> _storage;

    /// @brief Convenient shorthands for element access
    /// @note THIS MEMBER MUST REMAIN AMONG THE LAST TO BE DECLARED or the
    /// default initializer will be SKIPPED (which should fail to compile anyway)
    StorageProxy _elements = {
        .view       = _storage->get<0>().value(),
        .projection = _storage->get<1>().value(),
        .model      = _storage->get<2>().value(),
        .normal     = _storage->get<3>().value()
    };

    /// @brief Member metadata
    static constexpr LayoutInfo Layout = {
        .view       = { .size = storage_t::Sizes[0], .offset = storage_t::Offsets[0] },
        .projection = { .size = storage_t::Sizes[1], .offset = storage_t::Offsets[1] },
        .model      = { .size = storage_t::Sizes[2], .offset = storage_t::Offsets[2] },
        .normal     = { .size = storage_t::Sizes[3], .offset = storage_t::Offsets[3] }
    };

    void _commitDataToGPU(std::size_t offset, std::size_t byteCount) const;

public:
    /// @brief The total size, in bytes, of the UBO in the GPU memory
    static constexpr unsigned int BindingPoint = 0;
    
    MatrixUBO();

    /// @brief Set the view matrix in the UBO
    /// @param view The view matrix to set in the UBO
    /// @note This function does NOT commit the new value to the GPU.
    void setView(const num::Mat4& view);

    /// @brief Set the projection matrix in the UBO
    /// @param projection The projection matrix to set in the UBO
    /// @note This function does NOT commit the new value to the GPU.
    void setProjection(const num::Mat4& projection);

    /// @brief Set the model matrix in the UBO
    /// @param model The model matrix to set in the UBO
    /// @note This function does NOT commit the new value to the GPU.
    void setModel(const num::Mat4& model);

    /// @brief Set the normal matrix in the UBO
    /// @param normal The normal matrix to set in the UBO
    /// @note This function does NOT commit the new value to the GPU.
    void setNormal(const num::Mat3& normal);

    /// @brief Send the view matrix to the GPU
    void commitView();

    /// @brief Send the projection matrix to the GPU
    void commitProjection();

    /// @brief Send the view and projection matrices to the GPU
    void commitViewProjection();

    /// @brief Send the model matrix to the GPU
    void commitModel();

    /// @brief Send the normal matrix to the GPU
    void commitNormal();

    /// @brief Send the model and normal matrices to the GPU
    void commitModelNormal();

    /// @brief Send all matrices to the GPU
    void commit();
};

} // namespace rb

#endif//RENDERBOI_CORE_UBO_MATRIX_UBO_HPP
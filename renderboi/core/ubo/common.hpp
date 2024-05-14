#ifndef RENDERBOI_CORE_UBO_COMMON_HPP
#define RENDERBOI_CORE_UBO_COMMON_HPP

#include <cstddef>

namespace rb {

/// @brief Info about the layout of something
struct ValueLayoutInfo {
    std::size_t size;
    std::size_t offset;
};

} // namespace rb

#endif//RENDERBOI_CORE_UBO_COMMON_HPP
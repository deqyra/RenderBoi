#ifndef RENDERBOI_CORE_UBO_UBO_LAYOUT_HPP
#define RENDERBOI_CORE_UBO_UBO_LAYOUT_HPP

namespace rb {

/// @brief Specialize this struct for types that need to end up in GPU memory
template<typename T>
struct UBOLayout {};

using PaddingByte = unsigned char;

} // namespace rb

#endif//RENDERBOI_CORE_UBO_UBO_LAYOUT_HPP
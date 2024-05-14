#ifndef RENDERBOI_TOOLBOX_MESH_GENERATOR_HPP
#define RENDERBOI_TOOLBOX_MESH_GENERATOR_HPP

#include <renderboi/core/3d/mesh.hpp>

#include <concepts>
#include <memory>
#include <type_traits>

namespace rb {

template<typename T>
concept MeshGenerator =
    std::is_aggregate_v<typename T::Parameters>
    && requires (const T& gen) {
    { gen.generate() } -> std::same_as<std::unique_ptr<Mesh>>;
};

} // namespace rb

#endif//RENDERBOI_TOOLBOX_MESH_GENERATOR_HPP
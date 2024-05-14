#ifndef RENDERBOI_CORE_UBO_LIGHT_UBO_HPP
#define RENDERBOI_CORE_UBO_LIGHT_UBO_HPP

#include <renderboi/core/numeric.hpp>

#include <renderboi/core/lights/point_light.hpp>
#include <renderboi/core/lights/directional_light.hpp>
#include <renderboi/core/lights/spot_light.hpp>

#include <renderboi/core/ubo/common.hpp>
#include <renderboi/core/ubo/layout/point_light.hpp>
#include <renderboi/core/ubo/layout/directional_light.hpp>
#include <renderboi/core/ubo/layout/spot_light.hpp>

#include <cpptools/memory/contiguous_storage.hpp>
#include <cpptools/utility/concepts.hpp>
#include "renderboi/core/ubo/ubo_layout.hpp"

#include <memory>
#include <type_traits>

/* UNIFORM BLOCK LAYOUT
 * ====================
 * layout (std140, binding = 1) uniform Lights {   // Size    // Align // Offset
 *     PointLight point[64];                       // 64 * 64 //    16 //      0
 *     uint pointCount;                            //       4 //     4 //   4096
 *
 *     SpotLight spot[64];                         // 64 * 80 //    16 //   4112
 *     uint spotCount;                             //       4 //     4 //   9232
 *
 *     DirectionalLight direct[4]                  //  4 * 64 //    16 //   9248
 *     uint directionalCount;                      //       4 //     4 //   9504
 * } lights;                                       // Size: 9508
 *
 * ANY CHANGE TO THIS LAYOUT MUST BE REFLECTED IN LightUBO::storage_t BELOW
 **/

namespace rb {

template<typename T>
concept UBOCompatibleLightType = requires {
    std::disjunction_v<
        std::is_same<T, PointLight>,
        std::is_same<T, SpotLight>,
        std::is_same<T, DirectionalLight>
    >;
};

template<UBOCompatibleLightType T>
struct UBOLightCount {};

template<>
struct UBOLightCount<PointLight> {
    static constexpr std::size_t value = 64;
};

template<>
struct UBOLightCount<SpotLight> {
    static constexpr std::size_t value = 64;
};

template<>
struct UBOLightCount<DirectionalLight> {
    static constexpr std::size_t value = 4;
};

namespace detail {

/// @brief Provides access to a fixed-size array and maintains the count of elements 
/// stored in it (only when modified through member functions or manually maintained)
/// @note Since elements have implicit lifetime they're all "alive" at any given time,
/// and assigning to them is always fine. Reading from them can still be UB if the
/// read element has not been initialized.
/// @note The elements are kept contiguous: erasing in the middle results in a leftwards
/// shift of all elements on the right of the erased element, up to the count of elements.
template<UBOCompatibleLightType Light> 
struct CountedArrayRef {
    static constexpr std::size_t Size = UBOLightCount<Light>::value;
    using Element = UBOLayout<Light>;
    using Array = Element[Size];

    /// @brief The wrapped array
    Array& array;
    /// @brief The count of elements alive in the wrapped array
    unsigned int& count;

    Element& add(const Element& light) {
        Element& e = array[count++];
        e = light;
        return e;
    }

    std::size_t erase(std::size_t index) {
        auto maxIndex = count;

        auto itThis = std::begin(array) + index;
        auto itNext = itThis + 1;
        auto itEnd = std::begin(array) + maxIndex;
        std::move(itNext, itEnd, itThis);

        return --count;
    }
};

}

class LightUBO {
private:
    /// @brief A convenient access provider to the objects in the raw storage
    struct StorageProxy {
        detail::CountedArrayRef<PointLight>       point;
        detail::CountedArrayRef<SpotLight>        spot;
        detail::CountedArrayRef<DirectionalLight> directional;
    };

    /// @brief Info about the layout of a counted array in the raw storage / UBO
    struct ArrayLayoutInfo {
        ValueLayoutInfo array;
        ValueLayoutInfo count;
    };

    /// @brief Info about the layout of a the raw storage / UBO
    struct LayoutInfo {
        ArrayLayoutInfo point;
        ArrayLayoutInfo spot;
        ArrayLayoutInfo directional;
    };

    using PointArray       = UBOLayout<PointLight>      [UBOLightCount<PointLight>::value];
    using SpotArray        = UBOLayout<SpotLight>       [UBOLightCount<SpotLight>::value];
    using DirectionalArray = UBOLayout<DirectionalLight>[UBOLightCount<DirectionalLight>::value];

    /// @brief A bunch of uninitialized memory whose layout exactly matches that
    /// of the uniform block storage in GPU memory
    using storage_t = tools::contiguous_storage<
        PointArray       /* 0 */ , unsigned int /* 1 */ , PaddingByte[12],
        SpotArray        /* 3 */ , unsigned int /* 4 */ , PaddingByte[12],
        DirectionalArray /* 6 */ , unsigned int /* 7 */ 
    >;
    static_assert(storage_t::Size == 9508);
    static_assert(storage_t::has_implicit_lifetime);

    LightUBO(const LightUBO&) = delete;
    LightUBO(LightUBO&&) = delete;
    LightUBO& operator=(const LightUBO&) = delete;
    LightUBO& operator=(LightUBO&&) = delete;

    /// @brief The handle to the UBO on the GPU
    unsigned int _ubo;

    /// @brief The raw storage, which contains the data to be sent to the GPU
    std::unique_ptr<storage_t> _storage;

    /// @brief Convenient shorthands for element access
    /// @note THIS MEMBER MUST REMAIN AMONG THE LAST TO BE DECLARED or the
    /// default initializer will be SKIPPED (which should fail to compile anyway)
    StorageProxy _elements = {
        .point = {
            .array = _storage->get<0>().value(),
            .count = _storage->get<1>().value()
        },
        .spot = {
            .array = _storage->get<3>().value(),
            .count = _storage->get<4>().value()
        },
        .directional = {
            .array = _storage->get<6>().value(),
            .count = _storage->get<7>().value()
        }
    };

    /// @brief Member metadata
    static constexpr LayoutInfo Layout = {
        .point = {
            .array = { .size = storage_t::Sizes[0], .offset = storage_t::Offsets[0] },
            .count = { .size = storage_t::Sizes[1], .offset = storage_t::Offsets[1] }
        },
        .spot = {
            .array = { .size = storage_t::Sizes[3], .offset = storage_t::Offsets[3] },
            .count = { .size = storage_t::Sizes[4], .offset = storage_t::Offsets[4] }
        },
        .directional = {
            .array = { .size = storage_t::Sizes[6], .offset = storage_t::Offsets[6] },
            .count = { .size = storage_t::Sizes[7], .offset = storage_t::Offsets[7] }
        }
    };

    template<UBOCompatibleLightType Light>
    constexpr detail::CountedArrayRef<Light>& _blockStorage() {
        if constexpr (std::is_same_v<Light, PointLight>) {
            return _elements.point;
        } else if constexpr (std::is_same_v<Light, SpotLight>) {
            return _elements.spot;
        } else /* if constexpr (std::is_same_v<Light, PointLight>) */ {
            return _elements.directional;
        }
    }

    // clang does not support static member variable templates at the moment,
    // so this specialized struct will have to do for now
    template<UBOCompatibleLightType Light>
    struct MemberLayoutInfo {};

    template<>
    struct MemberLayoutInfo<PointLight> {
        static constexpr const ArrayLayoutInfo& value = Layout.point;
    };

    template<>
    struct MemberLayoutInfo<SpotLight> {
        static constexpr const ArrayLayoutInfo& value = Layout.spot;
    };

    template<>
    struct MemberLayoutInfo<DirectionalLight> {
        static constexpr const ArrayLayoutInfo& value = Layout.directional;
    };

    void _commitDataToGPU(std::size_t offset, std::size_t byteCount) const;

public:
    static constexpr unsigned int BindingPoint = 1;

    LightUBO();

    /// @brief Add a point light in the UBO
    /// @param position The position of the point light to send to the UBO
    /// @param pointLight The point light to send to the UBO
    /// @return The index at which the light was inserted
    /// @note This function does NOT commit the newly added light to the GPU
    UBOLayout<PointLight>& add(const PointLight& pointLight, const num::Vec3& position);

    /// @brief Add a spot light in the UBO
    /// @param position The position of the spot light to send to the UBO
    /// @param spotLight The spot light to send to the UBO
    /// @return The index at which the light was inserted
    /// @note This function does NOT commit the newly added light to the GPU
    UBOLayout<SpotLight>& add(const SpotLight& spotLight, const num::Vec3& position);

    /// @brief Add a directional light in the UBO
    /// @param directionalLight The directional light to send to the UBO
    /// @return The index at which the light was inserted
    /// @note This function does NOT commit the newly added light to the GPU
    UBOLayout<DirectionalLight>& add(const DirectionalLight& directionalLight);

    /// @brief Get a pointer to a light of a given type at a given index in the UBO
    /// @tparam Light The type of the light to get from the UBO
    /// @param index The index of the light to get from the UBO
    template<UBOCompatibleLightType Light>
    UBOLayout<Light>& get(std::size_t index) {
        return _blockStorage<Light>().array[index];
    }

    /// @brief Get the count of lights of a given type in the UBO
    /// @tparam Light The type of lights to get the count of in the UBO
    template<UBOCompatibleLightType Light>
    unsigned int& count() {
        return _blockStorage<Light>().count;
    }

    /// @brief Remove the light of a given type at a given index in the UBO
    /// @tparam Light The type of the light to remove from the UBO
    /// @param index The index of the light to remove from the UBO
    /// @note This function does NOT commit the removal to the GPU
    template<UBOCompatibleLightType Light>
    void remove(std::size_t index) {
        _blockStorage<Light>().erase(index);
    }

    /// @brief Send all lights of a given type to GPU memory
    /// @tparam Light The type of the lights to commit the GPU
    /// @note The count for this type of light IS COMMITTED TOO as part of this operation
    template<UBOCompatibleLightType Light>
    void commitAll() {
        constexpr std::size_t Size   = MemberLayoutInfo<Light>::value.array.size + MemberLayoutInfo<Light>::value.count.size;
        constexpr std::size_t Offset = MemberLayoutInfo<Light>::value.array.offset;

        _commitDataToGPU(Offset, Size);
    }

    /// @brief Send a single light of a given to GPU memory
    /// @tparam Light The type of the light to commit the GPU
    /// @param index The index of the light to send to GPU memory
    /// @note The count for the given light type is NOT COMMITED as part of this operation
    template<UBOCompatibleLightType Light>
    void commit(std::size_t index) {
        constexpr std::size_t Size   = sizeof(UBOLayout<Light>);
        constexpr std::size_t Offset = MemberLayoutInfo<Light>::value.array.offset + Size * index;

        _commitDataToGPU(Offset, Size);
    }

    /// @brief Send the whole UBO to GPU memory
    void commit() const;
};

} // namespace rb

#endif//RENDERBOI_CORE_UBO_LIGHT_UBO_HPP
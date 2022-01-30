#ifndef RENDERBOI__CORE__LIGHTS__LIGHT_HPP
#define RENDERBOI__CORE__LIGHTS__LIGHT_HPP

#include <memory>


#include "../common/parent_dependent_vp_matrix_provider.hpp"
#include "../interfaces/vp_matrix_provider.hpp"
#include "../transform.hpp"
#include "light_type.hpp"

namespace renderboi
{

/// @brief Abstract light.
class Light : public ParentDependentVPMatrixProvider
{
private:
    /// @brief Keeps track of how many instances were created (used as a unique
    /// ID system).
    static unsigned int _count;

protected:
    /// @param type Literal describing which type the light will be.
    Light(const LightType type);

public:
    Light() = delete;
    virtual ~Light() = default;

    /// @brief ID of the light instance.
    const unsigned int id;

    /// @brief Type of the light instance.
    const LightType lightType;

    /// @brief Get a raw pointer to a new light instance cloned from this one.
    /// Ownership and responsibility for the allocated resources are fully
    /// transferred to the caller.
    ///
    /// @return A raw pointer to the light instance cloned from this one.
    virtual Light* clone() const = 0;

private:
    //////////////////////////////////////////////////////////////
    ///                                                        ///
    /// Methods inherited from ParentDependentVPMatrixProvider ///
    ///                                                        ///
    //////////////////////////////////////////////////////////////

    /// @brief Compute the actual projection matrix using light perspective parameters.
    virtual glm::mat4 _computeProjectionMatrix() const = 0;

    /// @brief Compute the actual view matrix using light rotation parameters.
    virtual glm::mat4 _computeViewMatrix() const = 0;
};

using LightPtr = std::unique_ptr<Light>;

} // namespace renderboi

#endif//RENDERBOI__CORE__LIGHTS__LIGHT_HPP

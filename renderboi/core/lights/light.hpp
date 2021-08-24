#ifndef RENDERBOI__CORE__LIGHT_HPP
#define RENDERBOI__CORE__LIGHT_HPP

#include <memory>

#include <renderboi/core/transform.hpp>

#include "../interfaces/vp_matrix_provider.hpp"
#include "../implementation/parent_dependent_vp_matrix_provider.hpp"
#include "light_type.hpp"

namespace Renderboi
{

/// @brief Abstract light.
class Light : public ParentDependentVPMatrixProvider
{
private:
    Light() = delete;

    /// @brief Keeps track of how many instances were created (used as a unique
    /// ID system).
    static unsigned int _count;

protected:
    /// @param type Literal describing which type the light will be.
    Light(const LightType type);

public:
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

using LightPtr = std::shared_ptr<Light>;
using LightWPtr = std::weak_ptr<Light>;

}//namespace Renderboi

#endif//RENDERBOI__CORE__LIGHT_HPP

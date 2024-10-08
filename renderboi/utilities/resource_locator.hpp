#ifndef RENDERBOI_UTILITIES_RESOURCE_LOCATOR_HPP
#define RENDERBOI_UTILITIES_RESOURCE_LOCATOR_HPP

#include <string>
#include <filesystem>
#include <unordered_map>

namespace rb {

enum class ResourceType {
    ShaderSource,
    Texture,
    Any
};

/// @brief Eases out finding the correct full path to a ressource
class ResourceLocator {
private:
    /// @brief Structure mapping path prefixes to the resource type they lead to
    static std::unordered_map<ResourceType, std::filesystem::path> _resourceLocationPrefixes;

public:
    /// @brief Set the path prefix of a certain resource type Provide prefix
    /// "" to remove entry
    ///
    /// @param what Resource type to assign the path prefix to
    /// @param prefix Path prefix to assign to the resource type
    static void setPrefixFor(const ResourceType what, const std::string& prefix);

    /// @brief Set the path prefix of a certain resource type Provide prefix
    /// "" to remove entry
    ///
    /// @param what Resource type to assign the path prefix to
    /// @param prefix Path prefix to assign to the resource type
    static void setPrefixFor(const ResourceType what, const std::filesystem::path& prefix);

    /// @brief Retrieve the full path to a certain resource, given its type and
    /// filename
    ///
    /// @param type Type of the resource to locate
    /// @param filename Incomplete path to the ressource
    ///
    /// @return The full path to the resource
    static std::string locate(const ResourceType type, const std::string& filename);
};

} // namespace rb


#endif//RENDERBOI_UTILITIES_RESOURCE_LOCATOR_HPP
#ifndef POINT_LIGHT_HPP
#define POINT_LIGHT_HPP

#include <glm/glm.hpp>

#include "../positioned_object.hpp"

class PointLight : public PositionedObject
{
    public:
        PointLight(glm::vec3 position, glm::vec3 ambient, glm::vec3 diffuse, glm::vec3 specular);

        glm::vec3 ambient;
        glm::vec3 diffuse;
        glm::vec3 specular;
};

#endif//POINT_LIGHT_HPP
#ifndef MATERIAL_HPP
#define MATERIAL_HPP

#include <glm/glm.hpp>

class Material
{
    public:
        Material();
        Material(glm::vec3 ambient, glm::vec3 diffuse, glm::vec3 specular, float shininess);

        glm::vec3 ambient;
        glm::vec3 diffuse;
        glm::vec3 specular;
        float shininess;
};

#endif//MATERIAL_HPP
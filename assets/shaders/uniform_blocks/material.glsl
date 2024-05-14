#ifndef UNIFORM_BLOCKS_MATERIAL
#define UNIFORM_BLOCKS_MATERIAL

#define DIFFUSE_MAP_MAX_COUNT   8
#define SPECULAR_MAP_MAX_COUNT  8

// THIS STRUCT (including the macros above) MUST BE KEPT IN SYNC WITH renderboi/core/material.hpp

struct Material {
    vec3 ambient;
    float shininess;
    vec3 diffuse;
	uint diffuseMapCount;
    vec3 specular;
	uint specularMapCount;

	sampler2D diffuseMaps[DIFFUSE_MAP_MAX_COUNT];
	sampler2D specularMaps[SPECULAR_MAP_MAX_COUNT];
};

uniform Material material;

#endif//UNIFORM_BLOCKS_MATERIAL
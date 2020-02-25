#version 460 core
in VertexOut 
{
	vec3 color;
	vec3 normal;
	vec2 texCoord;
	vec3 fragPos;
} vertOut;

out vec4 fragColor;

// Types and constants
// ===================

struct PointLight
{											// Base alignment	// Base offset
    vec3 position;							// 16				//  0
    vec3 ambient;							// 16				// 16
    vec3 diffuse;							// 16				// 32
    vec3 specular;							// 16				// 48
};											// Size: 64

struct Material
{
    vec4 ambient;
    vec4 diffuse;
    vec4 specular;
    float shininess;

	sampler2D diffuseMap;
	sampler2D specularMap;

	bool diffuseMapEnabled;
	bool specularMapEnabled;
};

#define POINT_LIGHT_MAX_COUNT 128

// Uniforms
// ========

layout (std140, binding = 1) uniform lights
{											// Base alignment	// Base offset
	PointLight point[POINT_LIGHT_MAX_COUNT];// 64 * 128			//    0
	unsigned int nPoint;					// 4				// 8192
};											// Size: 8196

uniform Material material;

// Functions
// =========

vec4 processPointLight(int i);

void main()
{
	// Process all point lights
	vec3 pLightTotal = vec3(0.f);
	for (int i = 0; i < nPoint; i++)
	{
		pLightTotal += processPointLight(i);
	}

	// Combine components together
    vec4 result = vec4(pLightTotal, 1.f) * vec4(vertOut.color, 1.f);
    fragColor = result;
}

vec4 processPointLight(int i)
{
	vec3 normal = normalize(vertOut.normal);
	vec3 viewDir = normalize(vertOut.fragPos);
	vec3 lightDir = normalize(vertOut.fragPos - point[i].position);
	vec3 reflectDir = reflect(lightDir, normal);

	// Ambient lighting
    vec4 ambient = vec4(point[i].ambient, 1.f) * material.ambient;

	// Diffuse lighting
	vec4 diffuseTexel = vec4(1.f);
	if (material.diffuseMapEnabled)
		diffuseTexel = texture(material.diffuseMap, vertOut.texCoord);
	
	float diffusionFactor = max(dot(normal, -lightDir), 0.0);
	vec4 diffuse = vec4(point[i].diffuse, 1.f) * material.diffuse * diffuseTexel * diffusionFactor;

	// Specular
	vec4 specularTexel = vec4(1.f);
	if (material.specularMapEnabled)
		specularTexel = texture(material.specularMap, vertOut.texCoord);

	float spec = pow(max(dot(-viewDir, reflectDir), 0.0), material.shininess);
	vec4 specular = vec4(point[i].specular, 1.f) * material.specular * specularTexel * spec;

	return ambient + diffuse + specular;
}
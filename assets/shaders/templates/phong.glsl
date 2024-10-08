#ifndef TEMPLATES_PHONG
#define TEMPLATES_PHONG

vec3 phong(vec3 lightDirection, vec3 ambientLight, vec3 diffuseLight, vec3 specularLight) {
	vec3 viewDirection = normalize(vertOut.fragPos);

	// Ambient lighting
    vec3 total = ambientLight;
#ifdef FRAGMENT_MESH_MATERIAL
	total *= material.ambient;
#endif//FRAGMENT_MESH_MATERIAL

	// Diffuse lighting
	float diffusionFactor = max(dot(vertOut.normal, -lightDirection), 0.0);
	vec3 diffuseTexel = vec3(1.f);
#ifdef FRAGMENT_MESH_MATERIAL
	if (material.diffuseMapCount > 0)
	{
		vec4 texSample = texture(material.diffuseMaps[0], vertOut.texCoord);
		diffuseTexel = texSample.xyz * texSample.w;
	}
	diffuseTexel *= material.diffuse;
#endif//FRAGMENT_MESH_MATERIAL
	
	total += (diffuseTexel * diffuseLight) * diffusionFactor;

	// Specular
	vec3 specularTexel = vec3(1.f);
#ifdef FRAGMENT_MESH_MATERIAL
	if (material.specularMapCount > 0)
	{
		vec4 texSample = texture(material.specularMaps[0], vertOut.texCoord);
		specularTexel = texSample.xyz * texSample.w;
	}
#endif//FRAGMENT_MESH_MATERIAL

#ifdef FRAGMENT_BLINN_PHONG
	vec3 halfwayDirection = normalize(viewDirection + lightDirection);
	float spec = pow(max(dot(vertOut.normal, -halfwayDirection), 0.0), material.shininess);
#else
	vec3 reflectDirection = reflect(lightDirection, vertOut.normal);
	float spec = pow(max(dot(-viewDirection, reflectDirection), 0.0), material.shininess);
#endif//FRAGMENT_BLINN_PHONG 

	total += specularLight * material.specular * specularTexel * spec;

	return total;
}

#endif//TEMPLATES_PHONG
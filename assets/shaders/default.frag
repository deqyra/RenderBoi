#version 460 core
layout (location = 0) in vec3 inVertexColor;

out vec4 fragColor;

void main()
{
	fragColor = vec4(inVertexColor, 1.0f); 
}
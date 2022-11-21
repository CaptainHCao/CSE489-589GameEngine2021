#version 450 core

out vec4 fragmentColor;

in vec3 TexCoords;

layout(shared) uniform worldEyeBlock
{
	vec3 worldEyePosition;
};

layout(binding = 0) uniform samplerCube skyboxSampler;

void main()
{    
	fragmentColor = texture(skyboxSampler, TexCoords);
}
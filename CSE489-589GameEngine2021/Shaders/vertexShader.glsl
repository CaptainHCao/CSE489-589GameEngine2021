// Targeting version 4.5 of GLSL. If the compiler does not support 4.5 it will cause an error.
#version 450 core

//layout (location = 1001) uniform mat4 projectionMatrix; 
//layout (location = 1002) uniform mat4 viewMatrix; 
//layout (location = 1003) uniform mat4 modelMatrix;

layout(shared) uniform transformBlock
{
	mat4 modelMatrix;
	mat4 normalModelMatrix;
	mat4 viewMatrix;
	mat4 projectionMatrix;
};

out vec3 worldPos;
out vec3 worldNorm;
out vec2 texCoord0;

layout (location = 0) in vec4 vertexPosition;
layout (location = 1) in vec3 normal;
layout (location = 2) in vec2 vertexTexCoord;

void main()
{
	// Transform the position of the vertex to clip 
	// coordinates (minus perspective division)
	gl_Position = projectionMatrix * viewMatrix * modelMatrix * vertexPosition;

	// Transform the position of the vertex to world 
	// coords for lighting
	worldPos = (modelMatrix * vertexPosition).xyz;

	// Transform the normal to world coords for lighting
	worldNorm = normalize(mat3(normalModelMatrix) * normal ); 
	
	// Pass through the texture coordinate
	texCoord0 = vertexTexCoord;

}
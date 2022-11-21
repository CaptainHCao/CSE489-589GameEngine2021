#version 450 core

layout (location = 0) in vec4 vertexPosition;

out vec3 TexCoords;

layout(shared) uniform transformBlock
{
	mat4 modelMatrix;
	mat4 normalModelMatrix;
	mat4 viewMatrix;
	mat4 projectionMatrix;
};

void main()
{
    TexCoords = vec3(vertexPosition.x, vertexPosition.y, vertexPosition.z);

	vec4 pos = projectionMatrix * viewMatrix * modelMatrix * vertexPosition;

	gl_Position = pos.xyww;

}  
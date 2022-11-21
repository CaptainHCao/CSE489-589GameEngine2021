#include "SkyBoxComponent.h"

#include "stb_image.h"
#include "BuildShaderProgram.h"
#include "SharedTransformations.h"

#define VERBOSE false

bool SkyBoxComponent::areSkyboxShadersBuilt = false;
GLuint SkyBoxComponent::skyboxShaderProgram = 0;

SkyBoxComponent::SkyBoxComponent(int updateOrder)
	:Component(updateOrder)
{
    componentType = SKYBOX;
}

SkyBoxComponent::~SkyBoxComponent()
{
	glDeleteVertexArrays(1, &VAO);

	glDeleteBuffers(1, &VBO);

	glDeleteTextures(1, &skyboxTexture);
}

void SkyBoxComponent::initialize()
{
	if (VERBOSE) cout << "SkyBoxComponent::initialize" << endl;
	
	// If the skybox shaders haven't been built, build them
	if (!areSkyboxShadersBuilt)
	{
		buildSkyboxShaderProgram();
		areSkyboxShadersBuilt = true;
	}

	glUseProgram(skyboxShaderProgram);

	// Build the mesh
	buildSkyBoxMesh();

	// Load the skybox's faces
	loadCubeMap();

}

void SkyBoxComponent::renderSkyBox()
{
	if (VERBOSE) cout << "SkyBoxComponent::renderSkyBox" << endl;

	glDepthFunc(GL_LEQUAL);

	// Use the shader program
	glUseProgram(skyboxShaderProgram);

	// Get the modeling matrix
	glm::mat4 trans = glm::translate(owningGameObject->getPosition(WORLD));

	SharedTransformations::setModelingMatrix(trans);

	// Bind VAO, draw cube, then 
	glBindVertexArray(VAO);

	glBindTextureUnit(0, skyboxTexture);

	glDrawArrays(GL_TRIANGLES, 0, 36);
	
	//glBindVertexArray(0);

	glDepthFunc(GL_LESS);
}


bool SkyBoxComponent::loadCubeMap()
{
	// Passing true to this function will cause it to output images the way OpenGL expects
	stbi_set_flip_vertically_on_load(false);

	glGenTextures(1, &skyboxTexture);
	glBindTexture(GL_TEXTURE_CUBE_MAP, skyboxTexture);

	int width = 0, height = 0, nrChannels;

	// For each face in the cube, convert it to a texture
	for (unsigned int i = 0; i < faces.size(); i++)
	{
		unsigned char* data = stbi_load(faces[i].c_str(), &width, &height, &nrChannels, 0);

		// Check bitmap parameters to determine is a valid image was loaded
		if (data == nullptr || width == 0 || height == 0) {
			std::cerr << "ERROR: Unable to load " << faces[i] << "!" << std::endl;
			return false;
		}

		if (VERBOSE) cout << faces[i].c_str() << " number of channels " << nrChannels << endl;


		if (nrChannels == 3) {

			glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);

		}
		else if (nrChannels == 4) {

			glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);

		}
		else {
			std::cerr << faces[i] << " has " << nrChannels << " channels!" << std::endl;
			return false;

		}

		if (VERBOSE) std::cout << "Loaded: " << faces[i].c_str() << " texture. width " << width << " height " << height << std::endl;
	
		stbi_image_free(data);
	}

	// Generate the texture image and set the parameters
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

	glBindTexture(GL_TEXTURE_CUBE_MAP, 0);

	return true;
}

void SkyBoxComponent::buildSkyboxShaderProgram()
{
	// Set the shaders
	ShaderInfo shaders[] = {
	{ GL_VERTEX_SHADER, "Shaders/skyboxVertexShader.vs.glsl" },
	{ GL_FRAGMENT_SHADER, "Shaders/skyboxFragmentShader.fs.glsl" },
	{ GL_NONE, NULL } // signals that there are no more shaders 
	};

	// Build and use the shader program
	skyboxShaderProgram = BuildShaderProgram(shaders);

	SharedTransformations::setUniformBlockForShader(skyboxShaderProgram);
}

bool SkyBoxComponent::buildSkyBoxMesh()
{
	if (VERBOSE) cout << "SkyBoxComponent::buildSkyBoxMesh" << endl;

	GLfloat skyboxVertices[] = {
		
		// Positions          
		-20.0f,  20.0f, -20.0f, 1.0f,
		-20.0f, -20.0f, -20.0f, 1.0f,
		 20.0f, -20.0f, -20.0f, 1.0f,
		 20.0f, -20.0f, -20.0f, 1.0f,
		 20.0f,  20.0f, -20.0f, 1.0f,
		-20.0f,  20.0f, -20.0f, 1.0f,

		-20.0f, -20.0f,  20.0f, 1.0f,
		-20.0f, -20.0f, -20.0f, 1.0f,
		-20.0f,  20.0f, -20.0f, 1.0f,
		-20.0f,  20.0f, -20.0f, 1.0f,
		-20.0f,  20.0f,  20.0f, 1.0f,
		-20.0f, -20.0f,  20.0f, 1.0f,

		 20.0f, -20.0f, -20.0f, 1.0f,
		 20.0f, -20.0f,  20.0f, 1.0f,
		 20.0f,  20.0f,  20.0f, 1.0f,
		 20.0f,  20.0f,  20.0f, 1.0f,
		 20.0f,  20.0f, -20.0f, 1.0f,
		 20.0f, -20.0f, -20.0f, 1.0f,

		-20.0f, -20.0f,  20.0f, 1.0f,
		-20.0f,  20.0f,  20.0f, 1.0f,
		 20.0f,  20.0f,  20.0f, 1.0f,
		 20.0f,  20.0f,  20.0f, 1.0f,
		 20.0f, -20.0f,  20.0f, 1.0f,
		-20.0f, -20.0f,  20.0f, 1.0f,

		-20.0f,  20.0f, -20.0f, 1.0f,
		 20.0f,  20.0f, -20.0f, 1.0f,
		 20.0f,  20.0f,  20.0f, 1.0f,
		 20.0f,  20.0f,  20.0f, 1.0f,
		-20.0f,  20.0f,  20.0f, 1.0f,
		-20.0f,  20.0f, -20.0f, 1.0f,

		-20.0f, -20.0f, -20.0f, 1.0f,
		-20.0f, -20.0f,  20.0f, 1.0f,
		 20.0f, -20.0f, -20.0f, 1.0f,
		 20.0f, -20.0f, -20.0f, 1.0f,
		-20.0f, -20.0f,  20.0f, 1.0f,
		 20.0f, -20.0f,  20.0f, 1.0f
	};

	// Bind the vertex array object
	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);

	// Generate and bind the vertex buffer object
	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(skyboxVertices), &skyboxVertices, GL_STATIC_DRAW);
	
	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(GLfloat), (GLvoid*)0);
	glEnableVertexAttribArray(0);

	// Unbind everything
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	//glBindVertexArray(0);

	return true;
}

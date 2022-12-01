#pragma once
#include "Component.h"
class SkyBoxComponent : public Component
{
public:
	SkyBoxComponent(int updateOrder = 100);

	~SkyBoxComponent();

	virtual void initialize() override;

	void renderSkyBox();

protected:

	bool loadCubeMap();

	void buildSkyboxShaderProgram();

	bool buildSkyBoxMesh();

	//std::vector<string> faces;

	std::vector<string> faces
	{
		"Assets/Cube Map/right.jpg",
		"Assets/Cube Map/left.jpg",
		"Assets/Cube Map/top.jpg",
		"Assets/Cube Map/bottom.jpg",
		"Assets/Cube Map/back.jpg",
		"Assets/Cube Map/front.jpg"

	};

	GLuint skyboxTexture = 0;

	unsigned int VAO = 0, VBO = 0;

	static GLuint skyboxShaderProgram;
	
	static bool areSkyboxShadersBuilt;

};


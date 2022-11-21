#pragma once

#include "MathLibsConstsFuncs.h"

using namespace constants_and_types;

struct Material
{
	Material()
	{
		static int id = 0; 
		_id = id++;
	}

	void setAmbientMat(glm::vec4 ambientMat)
	{
		this->ambientMat = ambientMat;
	}

	void setDiffuseMat(glm::vec4 diffuseMat)
	{
		this->diffuseMat = diffuseMat;
	}

	void setSpecularMat(glm::vec4 specularMat)
	{
		this->specularMat = specularMat;
	}

	void setSpecularExponentMat(float specularExpMat)
	{
		this->specularExpMat = specularExpMat;
	}

	void setEmissiveMat(glm::vec4 emissiveMat)
	{
		this->emissiveMat = emissiveMat;
	}

	void setAmbientAndDiffuseMat(glm::vec4 objectColor)
	{
		setAmbientMat(objectColor);
		setDiffuseMat(objectColor);

	}

	void setTextureMode(TextureMode textureMode)
	{
		this->textureMode = textureMode;
	}

	void setDiffuseTexture(GLint textureObject)
	{
		this->diffuseTextureObject = textureObject;
		setTextureMode(REPLACE_AMBIENT_DIFFUSE);
		diffuseTextureEnabled = true;

	} // end setDiffuseTexture

	void setSpecularTexture(GLint textureObject)
	{
		this->specularTextureObject = textureObject;
		setTextureMode(REPLACE_AMBIENT_DIFFUSE);
		specularTextureEnabled = true;

	} // end setSpecularTexture

	int _id;

	glm::vec4 ambientMat = glm::vec4(0.75f, 0.75f, 0.75f, 1.0f);

	glm::vec4 diffuseMat = glm::vec4(0.75f, 0.75f, 0.75f, 1.0f);

	glm::vec4 specularMat = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);

	glm::vec4 emissiveMat = glm::vec4(0.0f, 0.0f, 0.0f, 1.0f);

	float specularExpMat = 64.0f;

	TextureMode textureMode = NO_TEXTURE;

	GLuint diffuseTextureObject = 0;
	bool diffuseTextureEnabled = false;

	GLuint specularTextureObject = 0;
	bool specularTextureEnabled = false;

};



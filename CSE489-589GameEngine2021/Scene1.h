#pragma once

#include "GameEngine.h"

class Scene1 : public Game
{
	void loadScene() override
	{
		// Set the window title
		glfwSetWindowTitle(renderWindow, "Scene 1");

		// Set the clear color
		glClearColor(0.5f, 0.5f, 0.5f, 1.0f);

		// Build and use the shader program
		ShaderInfo shaders[] = {
			{ GL_VERTEX_SHADER, "Shaders/vertexShader.glsl" },
			{ GL_FRAGMENT_SHADER, "Shaders/fragmentShader.glsl" },
			{ GL_NONE, NULL } // signals that there are no more shaders 
		};

		GLuint shaderProgram = BuildShaderProgram(shaders);

		SharedMaterials::setUniformBlockForShader(shaderProgram);
		SharedTransformations::setUniformBlockForShader(shaderProgram);
		SharedLighting::setUniformBlockForShader(shaderProgram);

		// ****** Directional light *********

		GameObject* directionaLightGO = new GameObject();
		addChildGameObject(directionaLightGO);
		directionaLightGO->rotateTo(vec3(-1.0f, -1.0f, -1.0f), WORLD);

		DirectionalLightComponent* dlc = new DirectionalLightComponent(GLFW_KEY_D);
		dlc->setAmbientColor(vec4(0.3f, 0.3f, 0.3f, 1.0f));
		dlc->setDiffuseColor(vec4(1.0f, 1.0f, 1.0f, 1.0f));
		dlc->setSpecularColor(vec4(1.0f, 1.0f, 1.0f, 1.0f));
		dlc->setEnable(true);
		directionaLightGO->addComponent(dlc);

		// ****** Camera *********

		GameObject* cameraObject = new GameObject();
		addChildGameObject(cameraObject);
		cameraObject->setPosition(vec3(0.0f, 0.0f, 0.0f), WORLD);

		CameraComponent* cam = new CameraComponent();
		cam->setViewPort(0.0f, 0.0f, 1.0f, 1.0f);
		cameraObject->addComponent(cam);

		cameraObject->addComponent(new ModelMakerComponent(shaderProgram, "Assets/Dinosaur/Trex.obj"));

		// ****** Falling Dinosaurs  *********

		for (int i = 0; i < 10; i++) {

			GameObject* dinoObject = new GameObject();
			this->addChildGameObject(dinoObject);
			ModelMeshComponent* dino = new ModelMeshComponent("Assets/Dinosaur/Trex.obj", shaderProgram);
			dinoObject->addComponent(dino);
			dinoObject->setPosition(vec3(rand() % 3 - 1.5f, 150.0f + rand() % 50 - 25.0f, rand() % 3 - 1.5f -85.0f), WORLD);
			dinoObject->addComponent(new RemoveComponent(15.0f));
			dinoObject->addComponent(new RigidBodyComponent(dino, DYNAMIC));
		}

		// ****** Red Sphere  *********

		// Add a game object to hold a red sphere
		GameObject* sphereObject = new GameObject();
		addChildGameObject(sphereObject);
		sphereObject->setPosition(vec3(1.0f, 0.0f, 0.0f), WORLD);

		// Create a red material for the sphere
		Material sphereMaterial;
		sphereMaterial.setAmbientAndDiffuseMat(vec4(1.0f, 0.0f, 0.0f, 1.0f));
		sphereMaterial.setSpecularMat(vec4(1.0f, 1.0f, 1.0f, 1.0f));

		// Create the sphere mesh
		SphereMeshComponent* sphereMesh = new SphereMeshComponent(shaderProgram, sphereMaterial, 1.0f, 16, 32);
		sphereObject->addComponent(sphereMesh);

		RigidBodyComponent* rg = new RigidBodyComponent(sphereMesh, DYNAMIC, 1000);
		sphereObject->addComponent(rg);
		rg->setVelocity(20.0f * vec3(0.0f, 1.0f, -1.0f));
		rg->setGravityOn(true);

		// Add a point light that will be inside the sphere
		LightComponent* pointlight = new PositionLightComponent(GLFW_KEY_P);
		pointlight->setAmbientColor(vec4(0.1f, 0.1f, 0.1f, 1.0f));
		pointlight->setDiffuseColor(vec4(1.0f, 1.0f, 1.0f, 1.0f));
		pointlight->setSpecularColor(0.5f * vec4(1.0f, 1.0f, 1.0f, 1.0f));
		pointlight->setEnable(true);

		sphereObject->addComponent(pointlight);
		sphereObject->addComponent(new RemoveComponent());

		// ****** Blue Sphere  *********

		GameObject* sphereObject2 = new GameObject();
		addChildGameObject(sphereObject2);
		sphereObject2->setPosition(vec3(0.0f, 0.0f, -85.0f), WORLD);

		Material sphereMaterial2;
		sphereMaterial2.setAmbientAndDiffuseMat(vec4(0.0f, 0.0f, 1.0f, 1.0f));

		SphereMeshComponent* sphereMesh2 = new SphereMeshComponent(shaderProgram, sphereMaterial2, 10.0f, 16, 32);
		sphereObject2->addComponent(sphereMesh2);

		RigidBodyComponent* rg2 = new RigidBodyComponent(sphereMesh2, STATIONARY);
		sphereObject2->addComponent(rg2);

	}
};

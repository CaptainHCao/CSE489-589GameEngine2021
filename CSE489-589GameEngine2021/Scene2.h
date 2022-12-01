#pragma once

#include "GameEngine.h"

class Scene2 : public Game
{
	float startTime;
	float currentTime;

	void loadScene() override
	{
		startTime = glfwGetTime();

		// Set the window title
		glfwSetWindowTitle(renderWindow, "Scene 2");

		// Set the clear color
		glClearColor(0.5f, 0.5f, 0.5f, 1.0f);

		// Build shader program
		ShaderInfo shaders[] = {
			{ GL_VERTEX_SHADER, "Shaders/vertexShader.glsl" },
			{ GL_FRAGMENT_SHADER, "Shaders/fragmentShader.glsl" },
			{ GL_NONE, NULL } // signals that there are no more shaders 
		};

		// Build the shader program
		GLuint shaderProgram = BuildShaderProgram(shaders);

		glUseProgram(shaderProgram);

		SharedTransformations::setUniformBlockForShader(shaderProgram);
		SharedMaterials::setUniformBlockForShader(shaderProgram);
		SharedLighting::setUniformBlockForShader(shaderProgram);

		// ****** Point light *********

		GameObject* pointLightGO = new GameObject();
		addChildGameObject(pointLightGO);
		pointLightGO->setPosition(vec3(0.0f, 10.0f, 0.0f), WORLD);

		LightComponent* pointlight = new PositionLightComponent(GLFW_KEY_P);
		pointlight->setAmbientColor(vec4(0.15f, 0.15f, 0.15f, 1.0f));
		pointlight->setDiffuseColor(vec4(1.0f, 1.0f, 1.0f, 1.0f));
		pointlight->setSpecularColor(0.5f * vec4(1.0f, 1.0f, 1.0f, 1.0f));
		pointlight->setEnable(true);
		pointLightGO->addComponent(pointlight);

		// ****** Directional light *********

		GameObject* directionaLightGO = new GameObject();
		addChildGameObject(directionaLightGO);
		directionaLightGO->rotateTo(vec3(-1.0f, -1.0f, -1.0f), WORLD);

		DirectionalLightComponent* dlc = new DirectionalLightComponent(GLFW_KEY_D);
		dlc->setAmbientColor(vec4(0.15f, 0.15f, 0.15f, 1.0f));
		dlc->setDiffuseColor(vec4(1.0f, 1.0f, 1.0f, 1.0f));
		dlc->setSpecularColor(vec4(1.0f, 1.0f, 1.0f, 1.0f));
		dlc->setEnable(true);
		directionaLightGO->addComponent(dlc);


		// ****** sphereGameObject *********

		//Add a balloon
		GameObject* balloonGameObject = new GameObject();
		this->addChildGameObject(balloonGameObject);
		//Spawn the balloon
		balloonGameObject->setPosition(vec3(50.0f, 0.0f, -200.0f), WORLD);

		//set spaceship's model, scale, sound listener and steering component
		balloonGameObject->setScale(vec3(0.01f, 0.01f, 0.01f), LOCAL);
		balloonGameObject->setRotation(glm::rotate(-PI_OVER_2, UNIT_X_V3));

		ModelMeshComponent* balloonMesh = new ModelMeshComponent("Assets/balloon/11809_Hot_air_balloon_l2.obj", shaderProgram);
		balloonGameObject->addComponent(balloonMesh);
		
		RigidBodyComponent* rigidBody = new RigidBodyComponent(balloonMesh, DYNAMIC);
		rigidBody->setGravityOn(false);

		balloonGameObject->addComponent(rigidBody);

		balloonGameObject->addComponent(new CollisionComponent());

		balloonGameObject->gameObjectName = "balloon - DYNAMIC";
		//sphereGameObject->addComponent(new CollisionComponent());

		// ****** sphereGameObject2 *********

		GameObject* sphereGameObject2 = new GameObject();
		this->addChildGameObject(sphereGameObject2);
		sphereGameObject2->setPosition(vec3(0.0f, 0.0f, -50.0f), WORLD);

		Material sphereMat2;
		sphereMat2.setDiffuseMat(vec4(0.0f, 0.0f, 1.0f, 1.0f));
		sphereMat2.setDiffuseTexture(Texture::GetTexture("Textures/earthmap.jpg")->getTextureObject());
		SphereMeshComponent* sphereMesh2 = new SphereMeshComponent(shaderProgram, sphereMat2, 5.0f, 24, 32);
		sphereGameObject2->addComponent(sphereMesh2);

		RigidBodyComponent* sgorg2 = new RigidBodyComponent(sphereMesh2, DYNAMIC, 1000.0f);
		sgorg2->setGravityOn(false);
		sphereGameObject2->addComponent(sgorg2);

		sphereGameObject2->addComponent(new CollisionComponent());

		sphereGameObject2->gameObjectName = "blue sphere - DYNAMIC";
		//sphereGameObject2->addComponent(new CollisionComponent());

		// ****** Cameras *********

		GameObject* cameraGameObject = new GameObject();
		this->addChildGameObject(cameraGameObject);
		cameraGameObject->setPosition(vec3(0.0f, 0.0f, 100.0f), WORLD);

		CameraComponent* camera = new CameraComponent();
		camera->setViewPort(0.0f, 0.0f, 1.0f, 1.0f);
		cameraGameObject->addComponent(camera);

		SkyBoxComponent* skybox = new SkyBoxComponent();
		cameraGameObject->addComponent(skybox);
		camera->setSkyBox(skybox);

		GameObject* cameraGameObject2 = new GameObject();
		this->addChildGameObject(cameraGameObject2);
		cameraGameObject2->setPosition(vec3(0.0f, 300.0f, 0.0f), WORLD);
		cameraGameObject2->setRotation(glm::rotate(-PI_OVER_2, UNIT_X_V3));

		CameraComponent* camera2 = new CameraComponent(0, 90.0f, 100);
		camera2->setViewPort(0.6f, 0.55f, 0.35f, 0.4f);
		cameraGameObject2->addComponent(camera2);

		//SkyBoxComponent* skybox2 = new SkyBoxComponent(150);
		//cameraGameObject2->addComponent(skybox2);
		//camera2->setSkyBox(skybox2);

		// ****** Jet *********
		GameObject* jetGameObject = new GameObject();

		this->addChildGameObject(jetGameObject);
		jetGameObject->setPosition(vec3(0.0f, 0.0f, 0.0f), WORLD);
		jetGameObject->setScale(vec3(0.25f, 0.25f, 0.25f), LOCAL);

		ModelMeshComponent* jetMesh = new ModelMeshComponent("Assets/jet_models/F-15C_Eagle.obj", shaderProgram);
		jetGameObject->addComponent(jetMesh);

		jetGameObject->gameObjectName = "jet - KINEMATIC";
		//jetGameObject->addComponent(new CollisionComponent());


		//SteeringComponent* stComp = new SteeringComponent(waypoints);

		UserSteeringComponent* stComp = new UserSteeringComponent();
		jetGameObject->addComponent(stComp);

		SoundSourceComponent* jetSound = new SoundSourceComponent("Assets/airplane-interior-2.wav", true);
		jetSound->setLooping(true);
		jetSound->play();
		jetGameObject->addComponent(jetSound);

		jetGameObject->addComponent(new RigidBodyComponent(jetMesh, KINEMATIC));
		
		jetGameObject->addComponent(new ModelMakerComponent(shaderProgram, "Assets/Dinosaur/Trex.obj"));

		SoundListenerComponent* listen = new SoundListenerComponent();
		jetGameObject->addComponent(listen);

		// Add a spot light
		GameObject* spotLightGO = new GameObject();
		jetGameObject->addChildGameObject(spotLightGO);
		spotLightGO->setPosition(vec3(0.0f, 0.0f, -8.0f), LOCAL);

		SpotLightComponent* splc = new SpotLightComponent(GLFW_KEY_S);
		splc->setAmbientColor(vec4(0.15f, 0.15f, 0.15f, 1.0f));
		splc->setDiffuseColor(vec4(1.0f, 1.0f, 1.0f, 1.0f));
		splc->setSpecularColor(vec4(1.0f, 1.0f, 1.0f, 1.0f));
		splc->setEnable(true);
		splc->setCutoffAngleInDegrees(15.0f);
		spotLightGO->addComponent(splc);

		// ****** Follow Camera *********

		GameObject* followCameraGameObject = new GameObject();
		jetGameObject->addChildGameObject(followCameraGameObject);
		followCameraGameObject->setPosition(vec3(0.0f, 3.0f, 30.0f), LOCAL);

		CameraComponent* followCamera = new CameraComponent();
		followCamera->setViewPort(0.1f, 0.6f, 0.3f, 0.3f);
		followCameraGameObject->addComponent(followCamera);

		SkyBoxComponent* skybox1 = new SkyBoxComponent();
		followCameraGameObject->addComponent(skybox1);
		followCamera->setSkyBox(skybox1);

	} // end loadScene
};
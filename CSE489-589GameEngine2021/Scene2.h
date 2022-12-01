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

		//set waypoints for earth to move
		GameObject* location1 = new GameObject();
		GameObject* location2 = new GameObject();
		this->addChildGameObject(location1);
		this->addChildGameObject(location2);
		location1->setPosition(vec3(50.0f, 0.0f, -50.0f), WORLD);
		location2->setPosition(vec3(-100.0f, 0.0f, -100.0f), WORLD);

		std::vector<GameObject*> waypoints = { location1, location2 };

		// ****** balloonGameObject *********
		//Add a balloon
		GameObject* balloonGameObject = new GameObject();
		this->addChildGameObject(balloonGameObject);
		//Spawn the balloon
		balloonGameObject->setPosition(vec3(50.0f, 0.0f, -200.0f), WORLD);

		//set balloon properties
		balloonGameObject->setScale(vec3(0.01f, 0.01f, 0.01f), LOCAL);
		balloonGameObject->setRotation(glm::rotate(-PI_OVER_2, UNIT_X_V3));

		ModelMeshComponent* balloonMesh = new ModelMeshComponent("Assets/balloon/11809_Hot_air_balloon_l2.obj", shaderProgram);
		balloonGameObject->addComponent(balloonMesh);
		
		RigidBodyComponent* rigidBody = new RigidBodyComponent(balloonMesh, DYNAMIC);
		rigidBody->setGravityOn(false);

		balloonGameObject->addComponent(rigidBody);

		balloonGameObject->addComponent(new CollisionComponent());

		//balloon song
		SoundSourceComponent* balloonSound = new SoundSourceComponent("Sounds/sweet-child-o-mine.wav", true);
		balloonSound->setMaxDistance(500.0f);
		balloonSound->setReferenceDistance(200.0f);
		balloonSound->setLooping(true);
		balloonSound->play();
		balloonGameObject->addComponent(balloonSound);

		balloonGameObject->gameObjectName = "balloon - DYNAMIC";

		// ****** tennisGameObject *********

		//Add a tennis falling from the sky
		GameObject* tennisGameObject = new GameObject();
		this->addChildGameObject(tennisGameObject);
		//Spawn the balloon
		tennisGameObject->setPosition(vec3(-50.0f, 50.0f, -200.0f), WORLD);

		//set the tennis ball properties
		tennisGameObject->setScale(vec3(0.05f, 0.05f, 0.05f), LOCAL);

		ModelMeshComponent* tennisMesh = new ModelMeshComponent("Assets/Dinosaur/tennisball.obj", shaderProgram);
		tennisGameObject->addComponent(tennisMesh);

		RigidBodyComponent* tennisrigidBody = new RigidBodyComponent(tennisMesh, DYNAMIC, 0.05f);
		tennisrigidBody->setGravityOn(true);

		tennisGameObject->addComponent(tennisrigidBody);

		tennisGameObject->addComponent(new CollisionComponent());

		tennisGameObject->gameObjectName = "tennis - DYNAMIC";

		// ****** earthGameObject *********

		GameObject* earthGameObject = new GameObject();
		this->addChildGameObject(earthGameObject);
		earthGameObject->setPosition(vec3(0.0f, 0.0f, -100.0f), WORLD);

		Material sphereMat2;
		sphereMat2.setDiffuseMat(vec4(0.0f, 0.0f, 1.0f, 1.0f));
		sphereMat2.setDiffuseTexture(Texture::GetTexture("Textures/earthmap.jpg")->getTextureObject());
		SphereMeshComponent* sphereMesh2 = new SphereMeshComponent(shaderProgram, sphereMat2, 5.0f, 24, 32);
		earthGameObject->addComponent(sphereMesh2);

		RigidBodyComponent* sgorg2 = new RigidBodyComponent(sphereMesh2, DYNAMIC);
		sgorg2->setGravityOn(false);
		earthGameObject->addComponent(sgorg2);

		earthGameObject->addComponent(new CollisionComponent());

		earthGameObject->addComponent(new SteeringComponent(waypoints));

		earthGameObject->gameObjectName = "blue sphere - DYNAMIC";

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

		// ****** Jet *********
		
		//add a jet and its properties
		GameObject* jetGameObject = new GameObject();

		this->addChildGameObject(jetGameObject);
		jetGameObject->setPosition(vec3(0.0f, 0.0f, 0.0f), WORLD);
		jetGameObject->setScale(vec3(0.25f, 0.25f, 0.25f), LOCAL);

		ModelMeshComponent* jetMesh = new ModelMeshComponent("Assets/jet_models/F-15C_Eagle.obj", shaderProgram);
		jetGameObject->addComponent(jetMesh);

		jetGameObject->gameObjectName = "jet - KINEMATIC";

		//custome steering
		UserSteeringComponent* stComp = new UserSteeringComponent();
		jetGameObject->addComponent(stComp);

		//jet song
		SoundSourceComponent* jetSound = new SoundSourceComponent("Sounds/21 Guns.wav", true);
		jetSound->setGain(2.0f);
		jetSound->setLooping(true);
		jetSound->play();
		jetGameObject->addComponent(jetSound);

		jetGameObject->addComponent(new RigidBodyComponent(jetMesh, KINEMATIC));
		
		jetGameObject->addComponent(new ModelMakerComponent(shaderProgram, "Assets/Dinosaur/Trex.obj"));

		//listener
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
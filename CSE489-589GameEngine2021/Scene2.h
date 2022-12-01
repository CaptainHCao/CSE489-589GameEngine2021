#pragma once

#include "GameEngine.h"

class Scene2 : public Game
{

	void loadScene() override
	{
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

		// ****** boxGameObject *********

		GameObject* boxGameObject = new GameObject();
		this->addChildGameObject(boxGameObject);
		boxGameObject->setPosition(vec3(100.0f, 0.0f, 0.0f), WORLD);

		Material boxMat;
		boxMat.setDiffuseMat(vec4(1.0f, 0.0f, 1.0f, 1.0f));
		BoxMeshComponent* boxMesh = new BoxMeshComponent(boxMat, shaderProgram, 10, 10, 10);

		boxGameObject->addComponent(boxMesh);
		boxGameObject->addComponent(new RigidBodyComponent(boxMesh, STATIONARY));

		boxGameObject->gameObjectName = "box - STATIONARY";
		//boxGameObject->addComponent(new CollisionComponent());

		// ****** sphereGameObject *********

		GameObject* sphereGameObject = new GameObject();
		this->addChildGameObject(sphereGameObject);
		sphereGameObject->setPosition(vec3(-100.0f, 0.0f, 0.0f), WORLD);

		Material sphereMat;
		sphereMat.setDiffuseTexture(Texture::GetTexture("Textures/earthmap.jpg")->getTextureObject());
		SphereMeshComponent* sphereMesh = new SphereMeshComponent(shaderProgram, sphereMat, 5.0f);
		sphereGameObject->addComponent(sphereMesh);

		RigidBodyComponent* rigidBody = new RigidBodyComponent(sphereMesh, STATIONARY);
		sphereGameObject->addComponent(rigidBody);

		sphereGameObject->gameObjectName = "earth - STATIONARY";
		//sphereGameObject->addComponent(new CollisionComponent());

		// ****** sphereGameObject2 *********

		GameObject* sphereGameObject2 = new GameObject();
		this->addChildGameObject(sphereGameObject2);
		sphereGameObject2->setPosition(vec3(0.0f, 0.0f, -50.0f), WORLD);

		Material sphereMat2;
		sphereMat2.setDiffuseMat(vec4(0.0f, 0.0f, 1.0f, 1.0f));
		//sphereMat2.setDiffuseTexture(Texture::GetTexture("Textures/earthmap.jpg")->getTextureObject());
		SphereMeshComponent* sphereMesh2 = new SphereMeshComponent(shaderProgram, sphereMat2, 5.0f, 24, 32);
		sphereGameObject2->addComponent(sphereMesh2);

		RigidBodyComponent* sgorg2 = new RigidBodyComponent(sphereMesh2, DYNAMIC, 1000.0f);
		sgorg2->setGravityOn(false);
		sphereGameObject2->addComponent(sgorg2);

		sphereGameObject2->gameObjectName = "blue sphere - DYNAMIC";
		//sphereGameObject2->addComponent(new CollisionComponent());

		// ****** cylinderGameObject *********

		GameObject* cylinderGameObject = new GameObject();
		this->addChildGameObject(cylinderGameObject);
		cylinderGameObject->setPosition(vec3(0.0f, 0.0f, 100.0f), WORLD);

		Material sphereMat3;
		sphereMat3.setDiffuseMat(vec4(1.0f, 1.0f, 0.0f, 1.0f));
		CylinderMeshComponent* cylinder = new CylinderMeshComponent(shaderProgram, sphereMat3, 2.0f, 4.0f);
		cylinderGameObject->addComponent(cylinder);

		cylinderGameObject->gameObjectName = "cylinder without a rigid body";
		//ylinderGameObject->addComponent(new CollisionComponent());

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

		SoundListenerComponent* listen = new SoundListenerComponent();
		cameraGameObject->addComponent(listen);

		GameObject* cameraGameObject2 = new GameObject();
		this->addChildGameObject(cameraGameObject2);
		cameraGameObject2->setPosition(vec3(0.0f, 250.0f, 0.0f), WORLD);
		cameraGameObject2->setRotation(glm::rotate(-PI_OVER_2, UNIT_X_V3));

		CameraComponent* camera2 = new CameraComponent(150);
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

		std::vector<GameObject*> waypoints = { boxGameObject, cylinderGameObject, sphereGameObject, sphereGameObject2 };

		//SteeringComponent* stComp = new SteeringComponent(waypoints);

		UserSteeringComponent* stComp = new UserSteeringComponent();
		jetGameObject->addComponent(stComp);

		SoundSourceComponent* jetSound = new SoundSourceComponent("Assets/airplane-interior-2.wav", true);
		jetSound->setLooping(true);
		jetSound->play();
		jetGameObject->addComponent(jetSound);

		jetGameObject->addComponent(new RigidBodyComponent(jetMesh, KINEMATIC));
		
		jetGameObject->addComponent(new ModelMakerComponent(shaderProgram, "Assets/Dinosaur/Trex.obj"));

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
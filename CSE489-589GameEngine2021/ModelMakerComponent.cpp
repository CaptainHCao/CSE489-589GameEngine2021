#include "ModelMakerComponent.h"
#include "RemoveComponent.h"
#include "RigidBodyComponent.h"

#define VERBOSE true

ModelMakerComponent::ModelMakerComponent(GLuint shaderProgram, string pathAndFileName)
	: pathAndFileName(pathAndFileName), shaderProgram(shaderProgram)
{

}

ModelMakerComponent::~ModelMakerComponent()
{
}

void ModelMakerComponent::initialize()

{
	// Add a "paused" game object with an a mesh attached (will not be rendered)
	// to avoid delays in loading at run time.

	pausedObject = new GameObject();
	pausedObject->setState(PAUSED);
	this->owningGameObject->addChildGameObject(pausedObject);

	ModelMeshComponent* baseMesh = new ModelMeshComponent(this->pathAndFileName, this->shaderProgram);
	pausedObject->addComponent(baseMesh);
}



void ModelMakerComponent::processInput()
{
	if (glfwGetKey(glfwGetCurrentContext(), GLFW_KEY_J) && j_key_up == true) {

		GameObject* gameObject = new GameObject();
		owningGameObject->addChildGameObject(gameObject);

		gameObject->setPosition(owningGameObject->getFowardDirection(LOCAL) * 20.0f, LOCAL);
		gameObject->addComponent(new RemoveComponent(10.0));
		
		// Create empty game object to rotate the model to face -Z i.e. "forward"
		// Only necessary with some models
		GameObject* emptyGameObject = new GameObject();
		gameObject->addChildGameObject(emptyGameObject);
		emptyGameObject->setRotation(glm::rotate(PI, UNIT_Y_V3), LOCAL);

		ModelMeshComponent* model = new ModelMeshComponent(this->pathAndFileName, this->shaderProgram);
		emptyGameObject->addComponent(model);
		RigidBodyComponent* rgb = new RigidBodyComponent(model, DYNAMIC);

		emptyGameObject->addComponent(rgb);
		rgb->setVelocity(owningGameObject->getFowardDirection(WORLD) * 100.0f);

		j_key_up = false;
	}
	if (!glfwGetKey(glfwGetCurrentContext(), GLFW_KEY_J)) {

		j_key_up = true;
	}

}
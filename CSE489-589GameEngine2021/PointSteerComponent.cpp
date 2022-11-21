#include "PointSteerComponent.h"

void PointSteerComponent::initialize()
{
	owningGameObject->setPosition( positions[0]+vec3(50.0f, 0.0f, 0.0f) );

	currentTarget = positions[0];

}


void PointSteerComponent::update(const float& deltaTime)
{
	vec3 curDirection = owningGameObject->getFowardDirection();

	if (glm::length(currentTarget - owningGameObject->getPosition()) > 0.0f) {

		vec3 direction = glm::normalize(currentTarget - owningGameObject->getPosition());

		vec3 curDirection = glm::normalize(glm::lerp(owningGameObject->getFowardDirection(), direction, deltaTime * rotationMultiplier));

		owningGameObject->rotateTo(curDirection);

	}

	owningGameObject->setPosition(owningGameObject->getPosition() + speed * curDirection * deltaTime);

}

void PointSteerComponent::processInput()
{
	if (glfwGetKey(glfwGetCurrentContext(), GLFW_KEY_1) && e_key_up == true) {

		targetInx++;

		currentTarget = positions[(targetInx) % 4];

		cout << targetInx << " " << currentTarget << endl;
		
		e_key_up = false;

	}
	if (!glfwGetKey(glfwGetCurrentContext(), GLFW_KEY_1)) {

		e_key_up = true;
	}

}

void PointSteerComponent::setSpeed(float spd)
{
	speed = spd;

}

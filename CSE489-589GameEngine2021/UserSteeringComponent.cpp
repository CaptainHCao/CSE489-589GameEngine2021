#include "UserSteeringComponent.h"
#define VERBOSE false

UserSteeringComponent::UserSteeringComponent(float speed, int updateOrder)
	: Component(updateOrder), speed(speed)
{
	componentType = MOVE;
}

void UserSteeringComponent::initialize()
{

}

void UserSteeringComponent::processInput()
{




}


void UserSteeringComponent::update(const float& deltaTime)
{	
	/***************Turning**************/
	mat4 rot = owningGameObject->getRotation();

	//// Figure out direction to next waypoint
	vec3 desiredDirection = owningGameObject->getFowardDirection();

	if (glfwGetKey(glfwGetCurrentContext(), GLFW_KEY_LEFT)) {
		
		rot *= glm::rotate(ROTATION * deltaTime, UNIT_Y_V3);
		desiredDirection = vec3(-owningGameObject->getRightDirection().x, 0.0f, 0.0f);
	}
	else if (glfwGetKey(glfwGetCurrentContext(), GLFW_KEY_RIGHT)) {
		
		rot *= glm::rotate(-ROTATION * deltaTime, UNIT_Y_V3);
		desiredDirection = vec3(owningGameObject->getRightDirection().x, 0.0f, 0.0f);

	}

	if (glfwGetKey(glfwGetCurrentContext(), GLFW_KEY_DOWN)) {
		
		rot *= glm::rotate(-ROTATION * deltaTime, UNIT_X_V3);

	}
	else if (glfwGetKey(glfwGetCurrentContext(), GLFW_KEY_UP)) {
		
		rot *= glm::rotate(ROTATION * deltaTime, UNIT_X_V3);

	}

	else {
		desiredDirection = owningGameObject->getFowardDirection();
	}

	this->owningGameObject->setRotation(rot);


	/**************Steering*******************/
	//// Get the current facing direction
	vec3 facingDirection = owningGameObject->getFowardDirection(WORLD);

	//// Calculate current yaw angle
	float currentYaw = (float)atan2(-facingDirection.x, -facingDirection.z);

	//// Calculate current pitch angle
	float currentPitch = (float)atan2(facingDirection.y, glm::sqrt(glm::pow(facingDirection.x, 2) + pow(facingDirection.z, 2)));

	//// Get heading angle to the next waypoint
	float desiredYaw = (float)atan2(-desiredDirection.x, -desiredDirection.z);

	// Account for the "branch cut" in yaw angles
	if (fabs(currentYaw - desiredYaw) > PI) {

	if (currentYaw < desiredYaw) {

			currentYaw += 2 * PI;
		}
		else {
			desiredYaw += 2 * PI;
		}
	}

	//// Determine the desired pitch
	float desiredPitch = (float)atan2(desiredDirection.y, glm::sqrt(glm::pow(desiredDirection.x, 2) + pow(desiredDirection.z, 2)));

	float desiredRoll = 0;

	//// Determine the turn direction and set the roll angle accordingly
	if (desiredYaw < currentYaw) {
	//	// Left turn
		desiredRoll = fabs(desiredYaw - currentYaw);
	}
	else {
	//	// Right turn
		desiredRoll = -fabs(desiredYaw - currentYaw);
	}

	//// Interpolate towards desired roll, pitch, and yaw angles
	currentRoll = glm::lerp(currentRoll, desiredRoll, deltaTime);
	float yaw = glm::lerp(currentYaw, desiredYaw, deltaTime);
	float pitch = glm::lerp(currentPitch, desiredPitch, deltaTime / 2);

	//// Apply Euler angles
	//mat3 eulerRotation = glm::rotate(yaw, UP) * glm::rotate(pitch, RIGHT) * glm::rotate(currentRoll, FORWARD);

	mat4 eulerRotation = glm::yawPitchRoll(yaw, pitch, -currentRoll);

	//// Set the orientation of the owning game object
	owningGameObject->setRotation(eulerRotation, WORLD);


	/*****************Translate***************/
	vec3 newFacingDirection = owningGameObject->getFowardDirection(WORLD);

	// Get the current position
	vec3 position = owningGameObject->getPosition(WORLD);

	// Determine the new position based on the speed
	position = position + newFacingDirection * speed * deltaTime;

	if (VERBOSE) cout << position << endl;

	// Update the position of the owning game object
	this->owningGameObject->setPosition(position, WORLD);

}

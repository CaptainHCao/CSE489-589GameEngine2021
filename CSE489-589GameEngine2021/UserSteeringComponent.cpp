#include "UserSteeringComponent.h"

#define VERBOSE false


UserSteeringComponent::UserSteeringComponent(vec3 velocity, int updateOrder)
	: Component(updateOrder), velocity(velocity)
{

}

void UserSteeringComponent::update(const float& deltaTime)
{
	float speed = glm::length(velocity);

	// Get the current facing direction
	vec3 facingDirection = owningGameObject->getFowardDirection(WORLD);

	// Calculate current yaw angle
	float currentYaw = (float)atan2(-facingDirection.x, -facingDirection.z);

	// Calculate current pitch angle
	float currentPitch = (float)atan2(facingDirection.y, glm::sqrt(glm::pow(facingDirection.x, 2) + pow(facingDirection.z, 2)));

	vec3 desiredDirection = owningGameObject->getFowardDirection(WORLD);

	// Figure out direction to next waypoint
	if (glfwGetKey(glfwGetCurrentContext(), GLFW_KEY_LEFT)) {
		desiredDirection = vec3(-owningGameObject->getRightDirection(LOCAL).x, 0.0f, 0.0f);

	}
	else if (glfwGetKey(glfwGetCurrentContext(), GLFW_KEY_RIGHT)) {
		desiredDirection = vec3(owningGameObject->getRightDirection(LOCAL).x, 0.0f, 0.0f);
	}
	
	owningGameObject->rotateTo(desiredDirection, LOCAL);

	// Get heading angle to the next waypoint
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

	// Determine the desired pitch
	float desiredPitch = (float)atan2(desiredDirection.y, glm::sqrt(glm::pow(desiredDirection.x, 2) + pow(desiredDirection.z, 2)));

	float desiredRoll = 0;

	// Determine the turn direction and set the roll angle accordingly
	if (desiredYaw < currentYaw) {
		// Left turn
		desiredRoll = fabs(desiredYaw - currentYaw);
	}
	else {
		// Right turn
		desiredRoll = -fabs(desiredYaw - currentYaw);
	}

	// Interpolate towards desired roll, pitch, and yaw angles
	currentRoll = glm::lerp(currentRoll, desiredRoll, deltaTime);
	float yaw = glm::lerp(currentYaw, desiredYaw, deltaTime);
	float pitch = glm::lerp(currentPitch, desiredPitch, deltaTime / 2);

	mat4 eulerRotation = glm::yawPitchRoll(yaw, pitch, -currentRoll);

	// Set the orientation of the owning game object
	owningGameObject->setRotation(eulerRotation, WORLD);

	// Get the new facing direction after the orientation has been applied
	vec3 newFacingDirection = owningGameObject->getFowardDirection(WORLD);

	// Get the current position
	vec3 position = owningGameObject->getPosition(WORLD);

	// Determine the new position based on the speed
	position = position + newFacingDirection * speed * deltaTime;

	if (VERBOSE) cout << position << endl;

	// Update the position of the owning game object
	this->owningGameObject->setPosition(position, WORLD);

} // end update



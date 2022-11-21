#include "PhysicsDemonstrationComponent.h"


void PhysicsDemonstrationComponent::update(const float & deltaTime)
{
	velocity += (acceleration + pdcGRAVITY) * deltaTime;

	vec3 position = owningGameObject->getPosition();

	position += velocity * deltaTime;

	owningGameObject->setPosition(position);

	//owningGameObject->rotateTo(velocity);

} // end update

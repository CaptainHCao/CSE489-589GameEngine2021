#include "SimplePhysicsEngine.h"
#include "SimpleRigidBodyComponent.h"

std::vector<class SimpleRigidBodyComponent*> SimplePhysicsEngine::rigidBodies;

void SimplePhysicsEngine::Update(const float& deltaTime)
{
	for (auto rb0 : SimplePhysicsEngine::rigidBodies) {

		if (rb0->dynamicsState == DYNAMIC) {

			if (rb0->applyGravity == true) {

				rb0->velocity += (rb0->acceleration + GRAVITY) * deltaTime;
			}
			else {

				rb0->velocity += (rb0->acceleration * deltaTime);
			}

			vec3 position = rb0->owningGameObject->getPosition();

			position += rb0->velocity * deltaTime;

			rb0->owningGameObject->setPosition(position);
		}
	}

	for (auto rb1 : SimplePhysicsEngine::rigidBodies) {

		for (auto rb2 : SimplePhysicsEngine::rigidBodies) {

			if (rb1 != rb2 && glm::distance(rb1->owningGameObject->getPosition(), rb2->owningGameObject->getPosition()) <
				rb1->collisionRadius + rb2->collisionRadius ) {

				for (auto* comp : rb1->owningGameObject->getComponents()) {

					comp->collision(rb2);
				}
			}
		}
	}




}
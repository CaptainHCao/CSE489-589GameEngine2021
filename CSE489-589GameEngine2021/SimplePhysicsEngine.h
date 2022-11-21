#pragma once

#include "MathLibsConstsFuncs.h"


/** @brief	(Immutable) gravitational acceleration */
static const vec3 GRAVITY(0.0f, -9.8f, 0.0);

class SimplePhysicsEngine
{
public:

	/**
	 * Initialize the engine.
	 */
	static bool Init() { return true;  }

	/**
	 * Updates all the position of only rigid bodies designated as DYNAMIC 
	 * based on the acceleration and velocity. Gravity is only applied 
	 * to those rigid bodies for which applyGravity is equal to true.
	 * 
	 * Checks each Rigid body  for collision against ALL other rigid bodies
	 * by determining if spherical bounding volumes overlap. Component::collision 
	 * is called on all the Components that are attached to the same
	 * GameObject as the rigid body that was hit. It is passed a pointer
	 * to the other rigid body that is involved in the collision.
	 * 
	 * Component subclasses override the Component::collision in order
	 * to create custom responses to collisions.
	 * 
	 * Collision response modeling is not expected.
	 * 
	 * @param deltaTime - time in seconds since the last update.
	 */
	static void Update(const float& deltaTime = 0.0f);

	/**
	 * Stop the engine. Call when closing down. 
	 */
	static void Stop() {};

	static std::vector<class SimpleRigidBodyComponent *> rigidBodies;

}; // end SimplePhysicsEngine



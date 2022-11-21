#pragma once
#include "Component.h"
#include "SimplePhysicsEngine.h"

class SimpleRigidBodyComponent : public Component
{
public:

    SimpleRigidBodyComponent() : Component(100) { componentType = RIGID_BODY; }

    /**
     * @fn	virtual void RigidBodyComponent::initialize() override
     *
     * @brief	Initializes this object by adding the rigid body to
     * 			the vector containing all rigid bodies that the 
     * 			physics engine should be aware of. The position of
     * 			rigid bodies is monitored by the physics engine. 
     * 			The positions of DYNAMIC rigid bodies are set by 
     * 			the physics engine. The physics engine checks all
     * 			rigid bodies for collision with other rigid bodies.
     */
    virtual void initialize() override
    {
        SimplePhysicsEngine::rigidBodies.push_back(this);
    }

    /** @brief	The mass of the rigid body.*/
    float Mass = 1.0f;

    /** @brief	The velocity of the rigid body.*/
    vec3 velocity = ZERO_V3;

    /** @brief	The acceleration of the rigid body.*/
    vec3 acceleration = ZERO_V3;

    /** @brief	radius of the spherical bounding volume 
    associated the rigid body. If the bounding volumes of 
    two rigid bodies overlap, they have collided. */
    float collisionRadius = 1.0f;

    /** @brief	If the rigid body is DYNAMIC and applyGravity    
    is true, gravitational acceleration is applied to 
    the rigid body. If is false, then gravitational acceleration
    is not not applied. */
    bool applyGravity = true;

    /** @brief	Dynamics state of the rigid body. The physics  
    engine updates the position of DYNAMIC rigid bodies   
    based on acceleration and velocity. 
    */
    DynamicsState dynamicsState = DYNAMIC;

}; // RigidBody


#pragma once
#include "Component.h"
class UserSteeringComponent :
    public Component
{
public:
	UserSteeringComponent(vec3 velocity = vec3(20, 0, 0), int updateOrder = 100);

	virtual void update(const float& deltaTime) override;

	float currentRoll = 0;

protected:
	vec3 velocity;
};



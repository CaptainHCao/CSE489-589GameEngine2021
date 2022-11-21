#pragma once
#include "Component.h"

class PointSteerComponent : public Component
{
public:

	virtual void update(const float& deltaTime) override;

	virtual void processInput() override;

	virtual void initialize() override;

	virtual void setSpeed(float spd);
	
protected:

	float speed = 0.0f;

	float rotationMultiplier = 5.0f;

	vec3 positions[4] = { vec3(0.0f, 0.0f, 0.0f), vec3(0.0f, 20.0f, 200.0f),  vec3(-50.0f, -20.0f, 0.0f), vec3(0.0f, 0.0f, -30.0f)  };

	vec3 currentTarget = positions[0];

	bool e_key_up = true;
	int targetInx = 0;

};


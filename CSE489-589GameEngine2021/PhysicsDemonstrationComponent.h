#pragma once
#include "Component.h"


static const vec3 pdcGRAVITY(0.0f, -9.8f, 0.0f);

class PhysicsDemonstrationComponent : public Component
{
public:

	virtual void update(const float& deltaTime) override;

    vec3 velocity = ZERO_V3;

    vec3 acceleration = ZERO_V3;

};


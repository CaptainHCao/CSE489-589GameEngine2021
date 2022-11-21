#pragma once
#include "LightComponent.h"

class PositionLightComponent : public LightComponent
{
public:

	PositionLightComponent(int controlKey) : LightComponent(controlKey) {}

	/**
	 * @fn	virtual void PositionLightComponent::update(float deltaTime) override;
	 *
	 * @brief	Updates the position of the point light.
	 *
	 * @param 	deltaTime	The delta time.
	 */
	virtual void update(const float& deltaTime) override;


};


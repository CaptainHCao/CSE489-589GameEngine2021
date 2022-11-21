#include "SoundBaseComponent.h"

SoundBaseComponent::SoundBaseComponent( int updateOrder)
	:Component( updateOrder )
{
}

void SoundBaseComponent::initialize()
{
	lastPosition = this->owningGameObject->getPosition(WORLD);
}

void SoundBaseComponent::update(const float& deltaTime)
{
	// Get current position
	soundPosition = this->owningGameObject->getPosition( WORLD );
	soundForward = this->owningGameObject->getFowardDirection( WORLD );
	soundUp = this->owningGameObject->getUpDirection( WORLD );

	// Calculate the velocity 
	if( deltaTime > 0.0f ) {

		soundVelocity = (soundPosition - lastPosition) / deltaTime;
	}

	// Save position for next update
	lastPosition = soundPosition;

} // end soundUpdate


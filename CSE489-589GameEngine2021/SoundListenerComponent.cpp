#include "SoundListenerComponent.h"

#define VERBOSE false

SoundListenerComponent::SoundListenerComponent( int updateOrder )
	:SoundBaseComponent( updateOrder )
{

}

void SoundListenerComponent::initialize()
{
	SoundBaseComponent::update(0.0f);
}


void SoundListenerComponent::update(const float& deltaTime)
{
	// Calculate the World position, velocity, and orientation 
	SoundBaseComponent::update( deltaTime );

	ALfloat listenerOri[] = { soundForward.x, soundForward.y, soundForward.z, soundUp.x, soundUp.y, soundUp.z };

	alListener3f(AL_POSITION, soundPosition.x, soundPosition.y, soundPosition.z);

	alListener3f(AL_VELOCITY, soundVelocity.x, soundVelocity.y, soundVelocity.z);
	
	alListenerfv(AL_ORIENTATION, listenerOri);
	
	// check for errors
	SoundEngine::check_al_errors();

	if (VERBOSE) {

		cout << "listener position " << soundPosition << endl;

	}

} // end update

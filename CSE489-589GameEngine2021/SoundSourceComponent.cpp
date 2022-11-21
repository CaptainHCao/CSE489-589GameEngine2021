#include "SoundSourceComponent.h"

#define VERBOSE false


SoundSourceComponent::SoundSourceComponent( std::string soundFileName, float refDistance,
	float rollOffFactor, float maxDistance, int updateOrder)
	: SoundBaseComponent( updateOrder )
{
	// Load the sound file into a buffer.
	buffer = alutCreateBufferFromFile(soundFileName.c_str());
	SoundEngine::check_alut_errors();

	// Create a source
	alGenSources((ALuint)1, &source);
	SoundEngine::check_al_errors();

	// Associate the buffer with the source
	alSourcei(source, AL_BUFFER, buffer);
	SoundEngine::check_al_errors();

	// Set the soud properties.
	setReferenceDistance(refDistance);
	setRollOffFactor(rollOffFactor);
	setMaxDistance(maxDistance);
	SoundEngine::check_al_errors();

} // SoundSource constructor


SoundSourceComponent::~SoundSourceComponent( )
{
	this->stop();

	if (VERBOSE) cout << "~SoundSourceComponent" << endl;

	alDeleteSources(1, &source);
	alDeleteBuffers(1, &buffer);

} // end SoundSource destructor

void SoundSourceComponent::initialize()
{
	SoundBaseComponent::update(0.0f);

	if (preInitPlay == true) {

		alSourcePlay(source);

		SoundEngine::check_al_errors();

		if (VERBOSE) cout << "play called in initilize" << endl;
	}
	
}

void SoundSourceComponent::update(const float& deltaTime)
{
	if (this->isPlaying()) {

		SoundBaseComponent::update(deltaTime);

		alSource3f(source, AL_DIRECTION, soundForward.x, soundForward.y, soundForward.z);

		alSource3f(source, AL_POSITION, soundPosition.x, soundPosition.y, soundPosition.z);
		// check for errors
		alSource3f(source, AL_VELOCITY, soundVelocity.x, soundVelocity.y, soundVelocity.z);

		SoundEngine::check_al_errors();
	}
} // end update

 // Start playing the sound
void SoundSourceComponent::play()
{
	if (owningGameObject->getOwningGame()->gameIsInitialized() == false) {

		preInitPlay = true;
		
		if (VERBOSE) cout << "delayed play" << endl;

	}
	else { // Game has been initilized. Play the sound now.

		// Play the source
		alSourcePlay(source);

		SoundEngine::check_al_errors();
	}

} // end play

// Stop the sound. 
void SoundSourceComponent::stop( )
{
	// Stop playing the source
	alSourceStop(source);

	SoundEngine::check_al_errors();

} // end stop

  // Pause the sound. When played again, it will start from the point 
  // where is was paused.
void SoundSourceComponent::pause( )
{
	// Pause the source
	alSourcePause(source);

	SoundEngine::check_al_errors();

} // end pause

void SoundSourceComponent::continuePlaying()
{
	// Continue playing the source
	alSourcePlay(source);

	SoundEngine::check_al_errors();

} // end continuePlaying

void SoundSourceComponent::setLooping( bool loopingOn )
{
	alSourcei(source, AL_LOOPING, loopingOn);

	SoundEngine::check_al_errors();

} // end setLooping

// Check to see if a sound it actively playing
bool SoundSourceComponent::isPlaying( )
{
	ALint state;
	alGetSourcei(source, AL_SOURCE_STATE, &state); 

	SoundEngine::check_al_errors();

	return (state == AL_PLAYING);

} // end isPlaying

bool SoundSourceComponent::isPaused()
{
	ALint state;
	alGetSourcei(source, AL_SOURCE_STATE, &state);

	SoundEngine::check_al_errors();

	return (state == AL_PAUSED);

} // end isPlaying

void SoundSourceComponent::setGain( float volume )
{
	alSourcef(source, AL_GAIN, volume);

	SoundEngine::check_al_errors();

} // end setGain

float SoundSourceComponent::getGain( )
{
	float gain;
	alGetSourcef(source, AL_GAIN, &gain);

	SoundEngine::check_al_errors();

	return gain;

} // end getGain

void SoundSourceComponent::setReferenceDistance(float refDistance)
{
	alSourcef(source, AL_REFERENCE_DISTANCE, refDistance);

	SoundEngine::check_al_errors();

} // end setReferenceDistance

void SoundSourceComponent::setRollOffFactor(float rollOffFactor)
{
	alSourcef(source, AL_ROLLOFF_FACTOR, rollOffFactor); // 0 no roll off fac

	SoundEngine::check_al_errors();

} // end setRollOffFactor

void SoundSourceComponent::setMaxDistance(float maxDistance)
{
	alSourcef(source, AL_MAX_DISTANCE, maxDistance);

	SoundEngine::check_al_errors();

} // end setMaxDistance


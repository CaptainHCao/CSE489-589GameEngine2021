#include "Component.h"

#define VERBOSE false

Component::Component(int updateOrder) 
	: updateOrder(updateOrder) 
{}

void Component::initialize() 
{}

Component::~Component()
{
	if (VERBOSE) cout << "Component destructor called " << endl;

	owningGameObject->removeComponent(this);
}

void Component::update(const float& deltaTime)
{}
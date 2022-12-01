#include "SynceComponent.h"

SynceComponent::SynceComponent(GameObject* followedGameObject, int updateOrder) : Component(updateOrder), followedGameObject(followedGameObject)
{

}

void SynceComponent::update(const float& deltaTime)
{

}
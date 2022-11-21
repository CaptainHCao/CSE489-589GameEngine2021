#include "PositionLightComponent.h"


void PositionLightComponent::update(const float& deltaTime)
{
    vec3 position = owningGameObject->getPosition(WORLD);

    SharedLighting::setPositionOrDirection(lightIndex, vec4(position, 1.0f));


}
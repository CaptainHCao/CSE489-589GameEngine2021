#include "SpotLightComponent.h"

void SpotLightComponent::initialize()
{
    SharedLighting::setIsSpot(lightIndex, true);
}

void SpotLightComponent::update(const float& deltaTime)
{
    vec3 position = owningGameObject->getPosition(WORLD);

    SharedLighting::setPositionOrDirection(lightIndex, vec4(position, 1.0f));

    vec3 direction = owningGameObject->getFowardDirection(WORLD);

    SharedLighting::setSpotDirection(lightIndex, direction);
}

void SpotLightComponent::setCutoffAngleInDegrees(float cutOffInDegrees)
{
    SharedLighting::setSpotCutoffCos(lightIndex, glm::cos(glm::radians(cutOffInDegrees)));
}
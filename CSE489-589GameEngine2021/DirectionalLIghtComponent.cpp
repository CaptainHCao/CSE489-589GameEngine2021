#include "DirectionalLIghtComponent.h"

void DirectionalLightComponent::update(const float& deltaTime)
{
    vec3 direction = owningGameObject->getFowardDirection(WORLD);

    SharedLighting::setPositionOrDirection(lightIndex, vec4(-direction, 0.0f));

}

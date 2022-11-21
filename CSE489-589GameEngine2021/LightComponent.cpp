#include "LightComponent.h"

LightComponent::LightComponent(int controlKey)
	:toggleKey(controlKey)
{
	componentType = LIGHT;

	for (int i = 0; i < MAX_LIGHTS; i++ ) {

		if (SharedLighting::lights[i].inUse == false) {
			lightIndex = i;
			SharedLighting::lights[i].inUse = true;
			break;
		}
	}
}

bool LightComponent::getEnable()
{
	return SharedLighting::getEnabled(lightIndex);
}

LightComponent::~LightComponent()
{
	SharedLighting::initilizeAttributes(lightIndex);
	SharedLighting::lights[lightIndex].inUse = false;
}

void LightComponent::setEnable(bool onOff)
{
	SharedLighting::setEnabled(lightIndex, onOff);
}

void LightComponent::processInput()
{
	if (glfwGetKey(glfwGetCurrentContext(), toggleKey) && toggleKeyDown == false) {

		SharedLighting::setEnabled(lightIndex, !SharedLighting::getEnabled(lightIndex));

		toggleKeyDown = true;
	}
	else if (!glfwGetKey(glfwGetCurrentContext(), toggleKey) && toggleKeyDown == true) {
		toggleKeyDown = false;
	}
}



void LightComponent::setAmbientColor(vec4 ambColor)
{
	SharedLighting::setAmbientColor(lightIndex, ambColor);
}

void LightComponent::setDiffuseColor(vec4 difColor)
{
	SharedLighting::setDiffuseColor(lightIndex, difColor);
}

void LightComponent::setSpecularColor(vec4 specColor)
{
	SharedLighting::setSpecularColor(lightIndex, specColor);
}

void LightComponent::setAttenuationFactors(float constant, float linear, float quadratic)
{
	SharedLighting::setConstantAttenuation(lightIndex, constant);

	SharedLighting::setLinearAttenuation(lightIndex, linear);

	SharedLighting::setQuadraticAttenuation(lightIndex, quadratic);

} 
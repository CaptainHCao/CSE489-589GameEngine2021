#pragma once
#include "Component.h"

#include "SharedLighting.h"

/**
 * @class	LightComponent - Supports all properties that are shared by all
 * 			types of light light sources. Super class for PointLightComponent,
 * 			DirectionalLightComponent, and SpotLightComponenet.
 *
 * @brief	A light component.
 */
class LightComponent : public Component
{
public:

    LightComponent(int controlKey);

    ~LightComponent();

    void setAmbientColor(vec4 ambColor);

    void setDiffuseColor(vec4 difColor);

    void setSpecularColor(vec4 specColor);

    void setEnable(bool onOff);

    void setAttenuationFactors(float constant = 1.0f, float linear = 2.0f, float quadratic = 2.0f);

    bool getEnable();
    
    virtual void processInput() override;

    protected:

    /** @brief	Zero-based index of the specific light */
    int lightIndex;

    /** @brief	The key used to toggle the light on and off*/
    int toggleKey;

    /** @brief	Tracks state of the toggle key to detect each press once.*/
    bool toggleKeyDown = false;

};


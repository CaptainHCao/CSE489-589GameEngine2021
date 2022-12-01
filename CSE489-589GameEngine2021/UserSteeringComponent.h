#pragma once
#include "Component.h"

class UserSteeringComponent :
    public Component
{
public:
    UserSteeringComponent(float speed = 20.0f, int updateOrder = 100);

    virtual void initialize() override;

    virtual void processInput() override;

    virtual void update(const float& deltaTime) override;

    //virtual vec3 desiredDirection();

protected:

    float rotationY = 0.0f, rotationX = 0.0f;

    float ROTATION_INC = glm::radians(20.0f);

    float ROTATION = glm::radians(20.0f);

    float speed;

    float currentRoll = 0;
};


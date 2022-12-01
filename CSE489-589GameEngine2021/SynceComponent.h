#pragma once
#include "Component.h"
class SynceComponent :
    public Component
{
public:
   SynceComponent(GameObject* followedGameObject, int updateOrder = 100);

   virtual void update(const float& deltaTime) override;

protected:
    GameObject* followedGameObject;
};


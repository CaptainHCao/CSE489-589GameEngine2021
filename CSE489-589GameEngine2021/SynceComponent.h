#pragma once
#include "Component.h"
class SynceComponent :
    public Component
{
   SynceComponent(GameObject* followedGameObject, int updateEngine);

   virtual void update(const float& deltaTime) override;
};


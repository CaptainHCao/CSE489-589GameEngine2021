#pragma once
#include "Component.h"
class CollisionComponent :
    public Component
{
public:
	CollisionComponent() : Component(100) {}

	virtual void collisionEnter(const class RigidBodyComponent* collisionData)
	{
		cout << "collision enter " << owningGameObject->gameObjectName << endl; 
	};

	virtual void collisionStay(const class RigidBodyComponent* collisionData)
	{ 
		cout << "collision stay " << owningGameObject->gameObjectName << endl;
	};

	virtual void collisionExit(const class RigidBodyComponent* collisionData)
	{ 
		cout << "collision exit " << owningGameObject->gameObjectName << endl;

		//owningGameObject->removeAndDelete();
	};




};


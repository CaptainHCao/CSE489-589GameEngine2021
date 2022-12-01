#pragma once
#include "Component.h"
#include "PointCounter.h"

class CollisionComponent :
    public Component
{
public:
	CollisionComponent() : Component(100) {}
	int points;

	virtual void collisionEnter(const class RigidBodyComponent* collisionData)
	{
		cout << "collision enter " << owningGameObject->gameObjectName << endl; 
		owningGameObject->removeAndDelete();
		cout << "points: " << ++PointCounter::points << endl;
	};

	virtual void collisionStay(const class RigidBodyComponent* collisionData)
	{ 
		cout << "collision stay " << owningGameObject->gameObjectName << endl;
		owningGameObject->removeAndDelete();
	};

	virtual void collisionExit(const class RigidBodyComponent* collisionData)
	{ 
		cout << "collision exit " << owningGameObject->gameObjectName << endl;

		//owningGameObject->removeAndDelete();
	};
};


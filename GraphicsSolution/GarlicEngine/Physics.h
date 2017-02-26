#pragma once
#include "GameObject.h"
#include "WeakPointer.h"
namespace Physics
{
	class PhysicsInfo {
	public:
		PhysicsInfo(StrongPointer<GameObject> i_pStrPtr);
		~PhysicsInfo();

		WeakPointer<GameObject> m_pWeakPtr;
		float m_Mass;
		float m_Drag;
	};

	Vector2D GetDragForce(PhysicsInfo *i_pPhysicsInfo, Vector2D i_drivingForce);
	void Update(PhysicsInfo & i_physicsInfo, Vector2D & i_Forces, float i_dt);
	void Run(float deltaTime);
};


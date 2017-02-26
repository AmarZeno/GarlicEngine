#include "Physics.h"
#include "Engine.h"
#include <windows.h>

namespace Physics
{
	constexpr float MAX_CHARACTER_VELOCITY = 0.002000f;
}


Physics::PhysicsInfo::PhysicsInfo(StrongPointer<GameObject> i_pStrPtr)
{
	m_pWeakPtr = i_pStrPtr;
	Engine::GetMasterObjects()->GetPhysicsInfoList().push_back(m_pWeakPtr);
}

Physics::PhysicsInfo::~PhysicsInfo()
{
}

Vector2D Physics::GetDragForce(PhysicsInfo *i_pPhysicsInfo, Vector2D i_drivingForce)
{
	// Coefficient = constant
	// drag * velocity2 * mass
	//TODO work in progress
	return (i_drivingForce * i_drivingForce) * (i_pPhysicsInfo->m_Mass * i_pPhysicsInfo->m_Drag);
}

void Physics::Update(PhysicsInfo & i_physicsInfo, Vector2D & i_Forces, float i_dt)
{
	if (i_physicsInfo.m_pWeakPtr.IsValid()) {
		//Vector2D
		if (i_Forces.x() == 0 && i_Forces.y() == 0) {
			// Reset velocity
			i_physicsInfo.m_pWeakPtr.Acquire()->SetVelocity(Vector2D(0, 0));
			return;
		}

		Vector2D initialVelocity = i_physicsInfo.m_pWeakPtr.Acquire()->GetVelocity();

		Vector2D Acceleration = i_Forces / i_physicsInfo.m_Mass;

		Vector2D finalVelocity = Vector2D(0, 0);

		Vector2D finalVelocityVector = (i_physicsInfo.m_pWeakPtr.Acquire()->GetVelocity() + (Acceleration * i_dt));

		if (finalVelocity.x() <= MAX_CHARACTER_VELOCITY) {

			finalVelocity.x(finalVelocityVector.x());
		}
		if (finalVelocity.y() <= MAX_CHARACTER_VELOCITY) {
			finalVelocity.y(finalVelocityVector.y());
		}

		i_physicsInfo.m_pWeakPtr.Acquire()->SetVelocity(finalVelocity);

		//	Vector2D dragForce = (((finalVelocity * finalVelocity) * i_physicsInfo.m_Drag) * i_physicsInfo.m_Mass);

		Vector2D finalPosition = i_physicsInfo.m_pWeakPtr.Acquire()->GetPosition() + (((finalVelocity + initialVelocity) / 2.0f) * i_dt);


		//i_physicsInfo.m_pObject->SetVelocity(&(i_physicsInfo.m_pObject->GetVelocity() + (Acceleration * i_dt)));
		i_physicsInfo.m_pWeakPtr.Acquire()->SetPosition(finalPosition);
	}
}

void Physics::Run(float deltaTime)
{

}



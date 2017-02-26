#pragma once
#include "GameObject.h"
#include "StrongPointer.h"
#include "WeakPointer.h"
#include <vector>

class MasterObjects
{
public:
	MasterObjects();
	~MasterObjects();

	std::vector<StrongPointer<GameObject>> GetMasterObjectList();
	std::vector<WeakPointer<GameObject>> GetPhysicsInfoList();
	std::vector<WeakPointer<GameObject>> GetRendererInfoList();

private:
	std::vector<StrongPointer<GameObject>> m_MasterObjectList;
	std::vector<WeakPointer<GameObject>> m_PhysicsInfoObjectList;
	std::vector<WeakPointer<GameObject>> m_RendererInfoObjectList;
};


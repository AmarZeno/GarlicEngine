#include "MasterObjects.h"



MasterObjects::MasterObjects()
{
}


MasterObjects::~MasterObjects()
{
}

std::vector<StrongPointer<GameObject>> MasterObjects::GetMasterObjectList()
{
	return m_MasterObjectList;
}

std::vector<WeakPointer<GameObject>> MasterObjects::GetPhysicsInfoList()
{
	return m_PhysicsInfoObjectList;
}

std::vector<WeakPointer<GameObject>> MasterObjects::GetRendererInfoList()
{
	return m_RendererInfoObjectList;
}

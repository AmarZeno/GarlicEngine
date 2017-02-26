#pragma once
#include "HeapAllocator.h"
#include "MasterObjects.h"

namespace Engine
{
	bool Startup();
	bool Shutdown();

	MasterObjects * GetMasterObjects();
};


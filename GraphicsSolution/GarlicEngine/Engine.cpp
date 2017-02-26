#include "Engine.h"
#include "Timing.h"
#include "Input.h"
#include "AI.h"
#include "Physics.h"
#include "Renderer.h"
#include <windows.h>

using namespace HeapAllocatorProxy;

namespace Engine
{
	// Memory constants
	const size_t SIZE_HEAP = 1024 * 1024;
	const guint_t NUM_OF_DESCRIPTORS = 93;

	void * pHeapMemory;

	static MasterObjects *m_MasterObjects;

	bool Startup()
	{
		// Allocate memory for my heap.
		pHeapMemory = _aligned_malloc(SIZE_HEAP, DEFAULT_MEM_ALIGNMENT);
		// Initialize MemoryAllocator
		HeapAllocator *heapAllocator = HeapAllocatorProxy::Initialize(pHeapMemory, SIZE_HEAP, NUM_OF_DESCRIPTORS);

		// Reserve storage for master object list and vector list
		m_MasterObjects = new MasterObjects();
		const int reservedVectorSpace = 100;
		m_MasterObjects->GetMasterObjectList().reserve(reservedVectorSpace);
		m_MasterObjects->GetPhysicsInfoList().reserve(reservedVectorSpace);
		m_MasterObjects->GetRendererInfoList().reserve(reservedVectorSpace);

		return heapAllocator != nullptr ? true : false;
	}

	bool Shutdown()
	{
		if(pHeapMemory != nullptr)
			_aligned_free(pHeapMemory);
		return true;
	}

	void Run() {
		// How long has it been since our last run through?
		float dt = Timing::CalcLastFrameTime();

		Input::Read(); // Get Input from user
		AI::Run(dt); // Run AI //(also apply user input)
		Physics::Run(dt);  // Run Physics + Detect/Handle Collision

		//Renderer::Draw(new Renderer::RendererInfo()); // Present scene to user
	}

	MasterObjects * GetMasterObjects() {
		return m_MasterObjects;
	}

}


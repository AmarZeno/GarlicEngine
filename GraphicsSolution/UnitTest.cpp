#include "UnitTest.h"
#include "Vector2D.h"
#include "Diagnostics.h"
#include "HeapAllocator.h"
#include "Zstring.h"
#include "LinkedListTest.h"
#include "FixedSizeAllocator.h"
#include "GameObject.h"

#define _
#define __SHOW_FREE_BLOCKS
#define __SHOW_ALLOCATIONS
#define	TEST_GARBAGE_COLLECTION
#define SUPPORT_ALIGNMENT

using namespace HeapAllocatorProxy;
using namespace UnitTestProxy;

namespace UnitTestProxy {

	void CheckVector2DConst()
	{
		DEBUG_SYSTEM_INFO
		DEBUG_FUNCTION_NAME
		DEBUG_LOCATION

		const Vector2D A(0.0f, 0.0f);
		const Vector2D B(1.0f, 1.0f);

		// These should work
		Vector2D C = A + B;

		C.x(A.x());

		C = A + B;

		// These should not work
		//A.x(1.0f);
		//A = C + B;
		//A += C;
	}

	void CheckHeapAllocator(HeapAllocator * i_pHeapAllocator)
	{
		char *abc = new TRACK_NEW char[MAX_CHAR_BUFFER];
		Zstrcpy(abc, "abc");

		delete[] abc;

#ifdef __SHOW_FREE_BLOCKS
		size_t count2 = ShowFreeBlocks(i_pHeapAllocator);
#endif // __SHOW_FREE_BLOCKS
#ifdef __SHOW_ALLOCATIONS
		size_t count = ShowAllocatedBlocks(i_pHeapAllocator);
#endif // __SHOW_ALLOCATIONS

		char *abc2 = new TRACK_NEW char[MAX_CHAR_BUFFER];
		Zstrcpy(abc2, "abc2");

#ifdef __SHOW_FREE_BLOCKS
		size_t count4 = ShowFreeBlocks(i_pHeapAllocator);
#endif // __SHOW_FREE_BLOCKS
#ifdef __SHOW_ALLOCATIONS
		size_t count3 = ShowAllocatedBlocks(i_pHeapAllocator);
#endif // __SHOW_ALLOCATIONS

		char *abc3 = new TRACK_NEW char[MAX_CHAR_BUFFER];
		Zstrcpy(abc3, "abc3");

#ifdef __SHOW_FREE_BLOCKS
		size_t count6 = ShowFreeBlocks(i_pHeapAllocator);
#endif // __SHOW_FREE_BLOCKS
#ifdef __SHOW_ALLOCATIONS
		size_t count5 = ShowAllocatedBlocks(i_pHeapAllocator);
#endif // __SHOW_ALLOCATIONS

		// Assert if circular linked list exists
		bool circularRef = IsCircular(i_pHeapAllocator->pFreeDescriptorList);
		assert(circularRef);

		delete abc3;

		//Deallocate(abc3, heapAllocator);

		size_t count7 = ShowAllocatedBlocks(i_pHeapAllocator);
		size_t count8 = ShowFreeBlocks(i_pHeapAllocator);

#ifdef TEST_GARBAGE_COLLECTION
		GarbageCollect(i_pHeapAllocator);
		char *abc4 = new TRACK_NEW char[MAX_CHAR_BUFFER];
		Zstrcpy(abc4, "abc4");
#endif // __TEST_GARBAGE_COLLECTION

}

	void CheckFixedBlockAllocator() {
		float *test1 = new float; // Assign memory using 8 byte FSA

		float *test2 = new float; // Assign memory using 8 byte FSA

		float *test3 = new float[3]; // Assign memory using 16 byte FSA

		float *test4 = new float[5]; // Assign memory using 32 byte FSA

		delete test2;			  // Delete the memory allocated using 8 byte FSA

		float *test5 = new float; // Assign a new memory using 8 byte FSA and observe the same address as the deleted one

		delete test3;

		float *test6 = new float[3]; // Assign a new memory using 16 byte FSA and observe the same address as the deleted one
	}

	void CheckGameObjectCopyAndMoveConstructor() {
		GameObject test1("test1", new Vector2D(0, 0));

		// copy constructor

		GameObject test2 = test1;

		// copy assignment

		GameObject test3("test2", new Vector2D(0, 0));

		test3 = test1;

		// move constructor

		GameObject test4 = std::move(test2);

		// move assignment

		GameObject test5("test3", new Vector2D(0, 0));

		test5 = std::move(test4);
	}

}
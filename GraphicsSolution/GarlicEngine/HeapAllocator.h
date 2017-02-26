#pragma once
#include "EngineTypes.h"
#include "FixedSizeAllocator.h"
#include "BitArray.h"

class HeapAllocator;

// Tracking in Debug build
#if defined(_DEBUG)
#define TRACK_NEW (__FILE__, __LINE__)
#else
#define TRACK_NEW
#endif // DEBUG

enum NewAlignment
{
	NEW_ALIGN_DEFAULT = 4,
	NEW_ALIGN_8 = 8,
	NEW_ALIGN_16 = 16,
	NEW_ALIGN_32 = 32
};

struct BlockDescriptor
{
	void *pHeap;
	size_t iSize;
	int iSignature;
	BlockDescriptor *pNext;
	BlockDescriptor *pPrev;
};

class HeapAllocator
{
public:
	HeapAllocator();
	~HeapAllocator();

	void *pFreeMemoryList;
	BlockDescriptor *pOutstandingAllocatedList;
	BlockDescriptor *pFreeDescriptorList;

	void * pHeapMemSystem;
	size_t memSystemSize;
	guint_t numberOfDescriptors;

	FixedSizeAllocator *staticFixedSizeAllocator_8Byte;
	FixedSizeAllocator *staticFixedSizeAllocator_16Byte;
	FixedSizeAllocator *staticFixedSizeAllocator_32Byte;

	BitArray *bitArray_8Byte;
	BitArray *bitArray_16Byte;
	BitArray *bitArray_32Byte;

	bool isHeapAllocatorReady = false;
};


namespace HeapAllocatorProxy {

	HeapAllocator * Initialize(void * i_pHeapMemory, const size_t i_SizeHeap, const guint_t i_NoOfDescriptors);
	// Parameter cannot be const as the values are modified
	void AssignStaticHeapAllocator(HeapAllocator * i_HeapAllocator);
	HeapAllocator * GetHeapAllocator();
	extern void * HAAllocate(const size_t i_Size, HeapAllocator * i_pHeapAllocator, NewAlignment i_Align);
	void * Deallocate(void * i_pDeallocatePointer, const HeapAllocator * i_pHeapAllocator);
	void AddToFreeBlocks(BlockDescriptor * , HeapAllocator *);

	inline size_t GetAvailableSize(const HeapAllocator * i_pHeapAllocator);
	inline size_t GetUsedSize(const HeapAllocator * i_pHeapAllocator);

	inline size_t ShowFreeBlocks(const HeapAllocator * i_pHeapAllocator);
	inline size_t ShowAllocatedBlocks(const HeapAllocator * i_pHeapAllocator);

	// Parameter cannot be const as the values are modified
	inline void ResetFreeDescriptorList(HeapAllocator * i_pHeapAllocator);
	size_t GetAlignedSize(size_t i_Size, NewAlignment i_Align);
	uintptr_t GetAlignedAddress(uintptr_t i_memAddress, NewAlignment i_align);

	void GarbageCollect(HeapAllocator * i_pHeapAllocator);

	// FSA Functions
	void InitializeFSA(HeapAllocator *allocator);
	void * CanFSAHandle(size_t i_size);
	bool CanFSADeallocate(void *i_pDeallocateAddress);

	// Bit array functions
	void InitializeBitArray(HeapAllocator *allocator);

	static HeapAllocator *staticHeapAllocator;

	// Constants
	const int INITIAL_BLOCK_DESCRIPTOR = 1;
	const int INITIAL_DESCRIPTOR_COUNT_OFFSET = 1;
	const int DEFAULT_MEM_ALIGNMENT = 4;
	const int FSA_SEGMENT_COUNT = 80;
	const int BASE_ZERO = 0;
}

void * operator new(size_t i_size);
void * operator new[](size_t i_size);
void * operator new(size_t i_size, NewAlignment i_Align);
void * operator new(size_t i_size, const char * i_pFile, unsigned int i_Line);

void operator delete(void * i_ptr);
void operator delete[](void * i_ptr);
void operator delete(void * i_ptr, const char * i_pFile, unsigned int i_Line);

#include "HeapAllocator-inl.h"


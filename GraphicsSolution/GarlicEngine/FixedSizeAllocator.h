#pragma once
#include "EngineTypes.h"
#include "BitArray.h"

enum FSAType
{
	FSA_TYPE_8_BYTE = 8,
	FSA_TYPE_16_BYTE = 16,
	FSA_TYPE_32_BYTE = 32,
};

class FixedSizeAllocator
{
public:
	FixedSizeAllocator();
	~FixedSizeAllocator();

	void * pStartFSAAddress;
	size_t totalMemSystemSegments;
	void * pEndFSAAddress;
	BitArray * bitArray;
};

namespace FixedSizeAllocatorProxy {
	FixedSizeAllocator * FSAInitialize(void * i_pHeapMemory, const size_t i_NumOfSegments, const FSAType i_FSAType, BitArray *bitArray);
	void * FSAAllocate(const size_t i_Size, const FixedSizeAllocator * i_pFSAAllocator, const FSAType i_FSAType);
	void * FSADeallocate(void * i_pDeallocatePointer, const FixedSizeAllocator * i_pFSAAllocator, const FSAType i_FSAType);
	size_t GetFSAStorageSize(const size_t i_NumOfSegments, const FSAType i_FSAType);
	size_t GetBytesFromSegmentCount(const size_t i_numOfSegments);

	// Constants
	const static size_t bitsPerBytes = 8;
}

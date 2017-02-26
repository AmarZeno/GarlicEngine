#include "FixedSizeAllocator.h"

FixedSizeAllocator::FixedSizeAllocator()
{
}

FixedSizeAllocator::~FixedSizeAllocator()
{
	bool success;
	size_t firstClearBit = bitArray->GetFirstClearBit(success);
	if (firstClearBit == bitArray->GetFullSkipValue() || success == false) {
		DEBUG_PRINT("Bit array has some allocations. Memory leaks warning.");
	}
}

namespace FixedSizeAllocatorProxy {
	FixedSizeAllocator * FSAInitialize(void * i_pHeapMemory, const size_t i_NumOfSegments, const FSAType i_FSAType, BitArray *bitArray)
	{
		// FSA Initialization
		FixedSizeAllocator *allocator = reinterpret_cast<FixedSizeAllocator *>(i_pHeapMemory);
		allocator->pStartFSAAddress = reinterpret_cast<void *>(reinterpret_cast<uintptr_t>(i_pHeapMemory) + sizeof(FixedSizeAllocator));
		allocator->totalMemSystemSegments = i_NumOfSegments;
		allocator->pEndFSAAddress = reinterpret_cast<void *>(reinterpret_cast<uintptr_t>(allocator->pStartFSAAddress) +  (i_NumOfSegments * i_FSAType));
		allocator->bitArray = bitArray;
		return allocator;
	}

	void * FSAAllocate(const size_t i_Size, const FixedSizeAllocator * i_pFSAAllocator, const FSAType i_FSAType)
	{
		bool success;
		size_t availableBitIndex = i_pFSAAllocator->bitArray->GetFirstClearBit(success);
		if (success == false) {
			return nullptr;
		}
		i_pFSAAllocator->bitArray->SetBit(availableBitIndex);
		return reinterpret_cast<void *>(reinterpret_cast<uintptr_t>(i_pFSAAllocator->pStartFSAAddress) + (availableBitIndex * i_FSAType));
	}

	void * FSADeallocate(void * i_pDeallocatePointer, const FixedSizeAllocator * i_pFSAAllocator, const FSAType i_FSAType) {
		size_t addressDifference = reinterpret_cast<uint8_t*>(i_pDeallocatePointer) - reinterpret_cast<uint8_t*>(i_pFSAAllocator->pStartFSAAddress);
		size_t storedBitIndex = addressDifference / i_FSAType;
		i_pFSAAllocator->bitArray->ClearBit(storedBitIndex);
		return nullptr;
	}

	size_t GetFSAStorageSize(const size_t i_NumOfSegments, const FSAType i_FSAType) {
		return (i_NumOfSegments * i_FSAType) + sizeof(FixedSizeAllocator);
	}

	size_t GetBytesFromSegmentCount(const size_t i_numOfSegments) {
		return i_numOfSegments / bitsPerBytes;
	}
}


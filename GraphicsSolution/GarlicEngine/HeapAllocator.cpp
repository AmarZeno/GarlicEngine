#include "HeapAllocator.h"
#include "Diagnostics.h"
#include <string>
#include <iostream>
#include <stdint.h>
#include <stdlib.h>


#ifdef __GUARDBAND
const uint8_t guardbandValues[] = { 0xde, 0xad, 0xbe, 0xef }; // 0xdeadbeef
const size_t guardbandBytes = sizeof(guardbandValues);
#endif // __GUARDBAND

using namespace HeapAllocatorProxy;

using namespace FixedSizeAllocatorProxy;

#define MEMSYSTEM_SIGNATURE 0xDEADC0DE
#define MEMSYSTEM_ENDMARKER 0xE00DC0DE

enum GarbageCollectionType
{
	DescriptorFix,
	StorageFix
};

HeapAllocator::HeapAllocator()
{
}

HeapAllocator::~HeapAllocator()
{
}

namespace HeapAllocatorProxy {

	HeapAllocator * Initialize(void * i_pHeapMemory, const size_t i_SizeHeap, const guint_t i_NoOfDescriptors)
	{
		HeapAllocator *allocator = reinterpret_cast<HeapAllocator *>(i_pHeapMemory);

		// Push the pHeapMemory forward
		i_pHeapMemory = reinterpret_cast<void *>(reinterpret_cast<uintptr_t>(i_pHeapMemory) + sizeof(HeapAllocator) + sizeof(int));
		// Making a local reference for all the passed values

		allocator->pHeapMemSystem = i_pHeapMemory;
		allocator->memSystemSize = i_SizeHeap;
		allocator->numberOfDescriptors = i_NoOfDescriptors;

		allocator->pFreeMemoryList = i_pHeapMemory;

		// Char sizes to 1. Instantiating the struct pointer within the available space

		size_t  requiredDescriptorStorage = i_NoOfDescriptors * sizeof(BlockDescriptor);

		allocator->pFreeDescriptorList = reinterpret_cast<BlockDescriptor *>(reinterpret_cast<uintptr_t>(allocator->pFreeMemoryList) + i_SizeHeap) - requiredDescriptorStorage;
		allocator->pFreeDescriptorList->pNext = allocator->pFreeDescriptorList + sizeof(BlockDescriptor);
		allocator->pFreeDescriptorList->pPrev = nullptr;
		allocator->pFreeDescriptorList->iSize = BASE_ZERO;
		size_t currentBlockDescriptor = INITIAL_BLOCK_DESCRIPTOR;
		while (true)
		{
			if (currentBlockDescriptor == allocator->numberOfDescriptors) {
				break;
			}
			allocator->pFreeDescriptorList = allocator->pFreeDescriptorList->pNext;
			if (currentBlockDescriptor == allocator->numberOfDescriptors - INITIAL_DESCRIPTOR_COUNT_OFFSET) {
				allocator->pFreeDescriptorList->pNext = nullptr;
			}
			else {
				allocator->pFreeDescriptorList->pNext = allocator->pFreeDescriptorList + sizeof(BlockDescriptor);
			}

			allocator->pFreeDescriptorList->pPrev = allocator->pFreeDescriptorList - sizeof(BlockDescriptor);
			allocator->pFreeDescriptorList->iSize = BASE_ZERO;
			currentBlockDescriptor++;
		}

		allocator->pFreeDescriptorList->pHeap = allocator->pFreeMemoryList;

		allocator->pFreeDescriptorList->iSize = i_SizeHeap - (i_NoOfDescriptors * sizeof(BlockDescriptor));

		allocator->pOutstandingAllocatedList = nullptr;

		// Assign static heap allocator
		AssignStaticHeapAllocator(allocator);

		InitializeBitArray(allocator);
		InitializeFSA(allocator);
		
		allocator->isHeapAllocatorReady = true;

		return allocator;
	}

	void InitializeBitArray(HeapAllocator *allocator)
	{
		// Bit Array Initialization
		allocator->bitArray_8Byte = reinterpret_cast<BitArray*>(HAAllocate(GetBitArrayHeapSize(FSA_SEGMENT_COUNT), allocator, NewAlignment::NEW_ALIGN_8));
		allocator->bitArray_16Byte = reinterpret_cast<BitArray*>(HAAllocate(GetBitArrayHeapSize(FSA_SEGMENT_COUNT), allocator, NewAlignment::NEW_ALIGN_16));
		allocator->bitArray_32Byte = reinterpret_cast<BitArray*>(HAAllocate(GetBitArrayHeapSize(FSA_SEGMENT_COUNT), allocator, NewAlignment::NEW_ALIGN_32));
		allocator->bitArray_8Byte->Create((FSA_SEGMENT_COUNT * FSAType::FSA_TYPE_8_BYTE), true, allocator->bitArray_8Byte);
		allocator->bitArray_16Byte->Create((FSA_SEGMENT_COUNT * FSAType::FSA_TYPE_16_BYTE), true, allocator->bitArray_16Byte);
		allocator->bitArray_32Byte->Create((FSA_SEGMENT_COUNT * FSAType::FSA_TYPE_32_BYTE), true, allocator->bitArray_32Byte);
	}

	void InitializeFSA(HeapAllocator *allocator) {
		// FSA Initialization
		allocator->staticFixedSizeAllocator_8Byte = reinterpret_cast<FixedSizeAllocator*>(HAAllocate(GetFSAStorageSize(FSA_SEGMENT_COUNT, FSAType::FSA_TYPE_8_BYTE), allocator, NewAlignment::NEW_ALIGN_8));
		allocator->staticFixedSizeAllocator_16Byte = reinterpret_cast<FixedSizeAllocator*>(HAAllocate(GetFSAStorageSize(FSA_SEGMENT_COUNT, FSAType::FSA_TYPE_16_BYTE), allocator, NewAlignment::NEW_ALIGN_16));
		allocator->staticFixedSizeAllocator_32Byte = reinterpret_cast<FixedSizeAllocator*>(HAAllocate(GetFSAStorageSize(FSA_SEGMENT_COUNT, FSAType::FSA_TYPE_32_BYTE), allocator, NewAlignment::NEW_ALIGN_32));

		FSAInitialize(allocator->staticFixedSizeAllocator_8Byte, FSA_SEGMENT_COUNT, FSAType::FSA_TYPE_8_BYTE, allocator->bitArray_8Byte);
		FSAInitialize(allocator->staticFixedSizeAllocator_16Byte, FSA_SEGMENT_COUNT, FSAType::FSA_TYPE_16_BYTE, allocator->bitArray_16Byte);
		FSAInitialize(allocator->staticFixedSizeAllocator_32Byte, FSA_SEGMENT_COUNT, FSAType::FSA_TYPE_32_BYTE, allocator->bitArray_32Byte);
	}

	// Setters and getters for static heap allocator
	void AssignStaticHeapAllocator(HeapAllocator * i_pHeapAllocator)
	{
		staticHeapAllocator = i_pHeapAllocator;
	}

	HeapAllocator * GetHeapAllocator() 
	{
		return staticHeapAllocator;
	}

	void * HAAllocate(const size_t i_Size, HeapAllocator * i_pHeapAllocator, NewAlignment i_Align)
	{
		// Moving the free memory pointer after the requested size
		size_t iRequestedBytes = i_Size;

		// Check if FSA can handle the incoming request and respond accordingly
		void * canFSAHandle = CanFSAHandle(i_Size);
		if (canFSAHandle != nullptr) {
			// Returns the address given by FSA
			return canFSAHandle;
		}

#ifdef __GUARDBAND
		// account for end guardband
		iRequestedBytes += 2 * guardbandBytes;
#endif

		size_t availableSize = GetAvailableSize(i_pHeapAllocator);

		// Safety check and garbage collection if required
		if (i_Size == BASE_ZERO) {
			return NULL;
		}

		if (iRequestedBytes > availableSize) {
			return NULL;
		}

		// Check for available size descriptor
		while (true)
		{
			if (iRequestedBytes < i_pHeapAllocator->pFreeDescriptorList->iSize) {
				// Available size found in the matching block
				// Do nothing
				break;
			}
			else {
				// Move to the previous available block
				i_pHeapAllocator->pFreeDescriptorList = i_pHeapAllocator->pFreeDescriptorList->pPrev;
			}

			if (i_pHeapAllocator->pFreeDescriptorList->pPrev == nullptr) {
				// garbage collection
				GarbageCollect(i_pHeapAllocator);
				break;
			}
		}

		/*Current Allocation Block*/
		// Steps for new allocation
		// 1) Update allocation for the first entry
		// 2) Update allocation for the following entry

		if (i_pHeapAllocator->pOutstandingAllocatedList == nullptr) {
			i_pHeapAllocator->pOutstandingAllocatedList = i_pHeapAllocator->pFreeDescriptorList;

			i_pHeapAllocator->pOutstandingAllocatedList->pHeap = i_pHeapAllocator->pFreeDescriptorList->pHeap;
			// Move the free block descriptor for the next allocation
			i_pHeapAllocator->pFreeDescriptorList = i_pHeapAllocator->pFreeDescriptorList->pPrev;

			i_pHeapAllocator->pOutstandingAllocatedList->pNext = nullptr;
			i_pHeapAllocator->pOutstandingAllocatedList->pPrev = nullptr;

			i_pHeapAllocator->pFreeDescriptorList->pNext = nullptr;
		}
		else {

			BlockDescriptor *prevAllocatedList = i_pHeapAllocator->pOutstandingAllocatedList;
			i_pHeapAllocator->pOutstandingAllocatedList = i_pHeapAllocator->pFreeDescriptorList;

			// Check if pheap reached last, then do garbage collect


			i_pHeapAllocator->pOutstandingAllocatedList->pHeap = i_pHeapAllocator->pFreeDescriptorList->pHeap;
			// Move the free block descriptor for the next allocation
			i_pHeapAllocator->pFreeDescriptorList = i_pHeapAllocator->pFreeDescriptorList->pPrev;
			i_pHeapAllocator->pFreeDescriptorList->pNext = nullptr;

			i_pHeapAllocator->pOutstandingAllocatedList->pNext = prevAllocatedList;
			i_pHeapAllocator->pOutstandingAllocatedList->pPrev = nullptr;

			i_pHeapAllocator->pOutstandingAllocatedList->pNext->pPrev = i_pHeapAllocator->pOutstandingAllocatedList;
		}

		i_pHeapAllocator->pOutstandingAllocatedList->iSize = iRequestedBytes;

		// Aligned address
		i_pHeapAllocator->pOutstandingAllocatedList->pHeap = reinterpret_cast<void *>(GetAlignedAddress(reinterpret_cast<uintptr_t>(i_pHeapAllocator->pOutstandingAllocatedList->pHeap), i_Align));

		if (i_pHeapAllocator->pFreeDescriptorList->pPrev == nullptr) {

		}
		else {
			/*Next allocation Block*/
			// Fetching previous block to update the next heap pointer
			i_pHeapAllocator->pFreeDescriptorList->pHeap = reinterpret_cast<void*>(reinterpret_cast<uintptr_t>(i_pHeapAllocator->pOutstandingAllocatedList->pHeap) + i_pHeapAllocator->pOutstandingAllocatedList->iSize);
			// Subtracting available memory with used Memory
			i_pHeapAllocator->pFreeDescriptorList->iSize = i_pHeapAllocator->memSystemSize - (i_pHeapAllocator->numberOfDescriptors * sizeof(BlockDescriptor)) - GetUsedSize(i_pHeapAllocator);
			//pFreeDescriptorList->pPrev = pFreeDescriptorList + sizeof(BlockDescriptor);
		}

		//ResetFreeDescriptorList();

#ifdef __GUARDBAND
		uint8_t * pStartGuardband = reinterpret_cast<uint8_t*>(i_pHeapAllocator->pOutstandingAllocatedList->pHeap);

		// write the start of block guardband
		pStartGuardband[0] = guardbandValues[0];
		pStartGuardband[1] = guardbandValues[1];
		pStartGuardband[2] = guardbandValues[2];
		pStartGuardband[3] = guardbandValues[3];

		// Add offset for guardband
		uint8_t * o_pUserStartAddress = reinterpret_cast<uint8_t*>(i_pHeapAllocator->pOutstandingAllocatedList->pHeap) + guardbandBytes;

		// write the end of block guardband
		uint8_t * pEndGuardband = o_pUserStartAddress + i_Size;
		pEndGuardband[0] = guardbandValues[0];
		pEndGuardband[1] = guardbandValues[1];
		pEndGuardband[2] = guardbandValues[2];
		pEndGuardband[3] = guardbandValues[3];

		// When guardbands are used, return the address offseted by the guardbands
		return o_pUserStartAddress;
#endif

		return i_pHeapAllocator->pOutstandingAllocatedList->pHeap;
	}

	void * CanFSAHandle(size_t i_size) {
		HeapAllocator *heapAllocator = GetHeapAllocator();
		if (heapAllocator->isHeapAllocatorReady == true) {
			if (i_size <= 8 && i_size > 0) {
				return FSAAllocate(i_size, heapAllocator->staticFixedSizeAllocator_8Byte, FSAType::FSA_TYPE_8_BYTE);
			}
			else if (i_size <= 16 && i_size > 8) {
				return FSAAllocate(i_size, heapAllocator->staticFixedSizeAllocator_16Byte, FSAType::FSA_TYPE_16_BYTE);
			}
			else if (i_size <= 32 && i_size > 16) {
				return FSAAllocate(i_size, heapAllocator->staticFixedSizeAllocator_32Byte, FSAType::FSA_TYPE_32_BYTE);
			}
		}
		return nullptr;
	}

	size_t GetAlignedSize(size_t i_size, NewAlignment i_align) {
		return i_size + (i_size % i_align);
	}

	uintptr_t GetAlignedAddress(uintptr_t i_memAddress, NewAlignment i_align)
	{
		uintptr_t localMemAddress = i_memAddress;
		if (localMemAddress % i_align != 0) {
			localMemAddress += i_align - localMemAddress % i_align;
		}
		return localMemAddress;
	}

	void * Deallocate(void * i_pDeallocatePointer, HeapAllocator * i_pHeapAllocator)
	{
		// Check if FSA can deallocate
		if (CanFSADeallocate(i_pDeallocatePointer)) {
			return nullptr;
		}

		#ifdef __GUARDBAND
			i_pDeallocatePointer = reinterpret_cast<uint8_t*>(i_pDeallocatePointer) - guardbandBytes;
		#endif

		BlockDescriptor *tempOutstandingAllocatedList = i_pHeapAllocator->pOutstandingAllocatedList;
		while (true)
		{
			if (i_pDeallocatePointer == tempOutstandingAllocatedList->pHeap) {

				BlockDescriptor *deallocatedPointerRef = tempOutstandingAllocatedList;
				//BlockDescriptor *swapPreviousFreeList = pFreeDescriptorList->pPrev;
				//pFreeDescriptorList->pPrev = tempOutstandingAllocatedList;
				//pFreeDescriptorList->pPrev->pPrev = swapPreviousFreeList;

				/*Cases to connect Allocated list*/
				// Case 1) When previous and next are null. Single block entry.
				if (tempOutstandingAllocatedList->pPrev == nullptr && tempOutstandingAllocatedList->pNext == nullptr) {
					i_pHeapAllocator->pOutstandingAllocatedList = nullptr;
				}

				// Case 2) When previous alone is nullptr. Multiple block entry. head at first position 
				if (tempOutstandingAllocatedList->pPrev == nullptr && tempOutstandingAllocatedList->pNext != nullptr) {
					//	pOutstandingAllocatedList->pNext->pPrev = nullptr;
					i_pHeapAllocator->pOutstandingAllocatedList = i_pHeapAllocator->pOutstandingAllocatedList->pNext;
					i_pHeapAllocator->pOutstandingAllocatedList->pPrev = nullptr;
				}

				// Case 3) When next alone is nullptr. Multiple block entry. removal at last position
				if (tempOutstandingAllocatedList->pPrev != nullptr && tempOutstandingAllocatedList->pNext == nullptr) {
					tempOutstandingAllocatedList->pPrev->pNext = nullptr;
				}

				// Case 4) When previous and next are not null. Multiple block entry. Removal from centre of a contigous block.
				if (tempOutstandingAllocatedList->pPrev != nullptr && tempOutstandingAllocatedList->pNext != nullptr) {
					tempOutstandingAllocatedList->pPrev->pNext = tempOutstandingAllocatedList->pNext;
					tempOutstandingAllocatedList->pNext->pPrev = tempOutstandingAllocatedList->pPrev;
				}

				// Add the removed allocated element to the free list
				/*tempOutstandingAllocatedList->pNext = pFreeDescriptorList;
				tempOutstandingAllocatedList->pPrev = pFreeDescriptorList->pPrev;
				pFreeDescriptorList->pPrev->pNext = tempOutstandingAllocatedList;
				pFreeDescriptorList->pPrev = tempOutstandingAllocatedList;*/
				AddToFreeBlocks(deallocatedPointerRef, i_pHeapAllocator);

				i_pDeallocatePointer = nullptr;
				break;
			}
			tempOutstandingAllocatedList = tempOutstandingAllocatedList->pNext;
			if (tempOutstandingAllocatedList == nullptr) {
				break;
			}
		}

		return nullptr;
	}

	void AddToFreeBlocks(BlockDescriptor *i_pFreedBlock, HeapAllocator * i_pHeapAllocator)
	{
		// Start from beginning of free list
		BlockDescriptor * pfreeDescriptorCurrentRef = i_pHeapAllocator->pFreeDescriptorList;
		while (true)
		{
			if (i_pFreedBlock < pfreeDescriptorCurrentRef && pfreeDescriptorCurrentRef->pPrev == nullptr) {
				// add the block before the head
				i_pFreedBlock->pPrev = nullptr;
				i_pFreedBlock->pNext = pfreeDescriptorCurrentRef;
				pfreeDescriptorCurrentRef->pPrev = i_pFreedBlock;
				break;
			}
			else  if (i_pFreedBlock > pfreeDescriptorCurrentRef && pfreeDescriptorCurrentRef->pNext == nullptr) {
				// add the block at the tail
				i_pFreedBlock->pNext = nullptr;
				i_pFreedBlock->pPrev = pfreeDescriptorCurrentRef;
				pfreeDescriptorCurrentRef->pNext = i_pFreedBlock;

				// Reassign free descriptor to the last block
				i_pHeapAllocator->pFreeDescriptorList = i_pFreedBlock;
				break;
			}
			else if (i_pFreedBlock > pfreeDescriptorCurrentRef && i_pFreedBlock < pfreeDescriptorCurrentRef->pNext) {
				// if address is greater than the current element and lesser than the next element
				i_pFreedBlock->pNext = pfreeDescriptorCurrentRef->pNext;
				i_pFreedBlock->pPrev = pfreeDescriptorCurrentRef;
				pfreeDescriptorCurrentRef->pNext = i_pFreedBlock;
				i_pFreedBlock->pNext->pPrev = i_pFreedBlock;
				break;
			}
			if (pfreeDescriptorCurrentRef->pPrev == nullptr) {
				// Break if it reaches the end of the linked list
				break;
			}
			pfreeDescriptorCurrentRef = pfreeDescriptorCurrentRef->pPrev;
		}
	}

	bool CanFSADeallocate(void *i_pDeallocateAddress) {
		bool canFSADeallocate = false;
		HeapAllocator *heapAllocator = GetHeapAllocator();
		if (i_pDeallocateAddress > heapAllocator->staticFixedSizeAllocator_8Byte && i_pDeallocateAddress <= heapAllocator->staticFixedSizeAllocator_16Byte) {
			FSADeallocate(i_pDeallocateAddress, heapAllocator->staticFixedSizeAllocator_8Byte, FSAType::FSA_TYPE_8_BYTE);
			canFSADeallocate = true;
		}
		else if (i_pDeallocateAddress > heapAllocator->staticFixedSizeAllocator_16Byte && i_pDeallocateAddress <= heapAllocator->staticFixedSizeAllocator_32Byte) {
			FSADeallocate(i_pDeallocateAddress, heapAllocator->staticFixedSizeAllocator_16Byte, FSAType::FSA_TYPE_16_BYTE);
			canFSADeallocate = true;
		}
		else if (i_pDeallocateAddress > heapAllocator->staticFixedSizeAllocator_32Byte && i_pDeallocateAddress < heapAllocator->bitArray_8Byte) {
			FSADeallocate(i_pDeallocateAddress, heapAllocator->staticFixedSizeAllocator_32Byte, FSAType::FSA_TYPE_32_BYTE);
			canFSADeallocate = true;
		}
		return canFSADeallocate;
	}


	void GarbageCollect(HeapAllocator * i_pHeapAllocator)
	{
		ResetFreeDescriptorList(i_pHeapAllocator);

		while (true)
		{
			if (reinterpret_cast<uintptr_t>(i_pHeapAllocator->pFreeDescriptorList->pHeap) + i_pHeapAllocator->pFreeDescriptorList->iSize == reinterpret_cast<uintptr_t>(i_pHeapAllocator->pFreeDescriptorList->pPrev->pHeap)) {
				if (i_pHeapAllocator->pFreeDescriptorList->pHeap < i_pHeapAllocator->pFreeDescriptorList->pPrev->pHeap) {
					i_pHeapAllocator->pFreeDescriptorList->iSize += i_pHeapAllocator->pFreeDescriptorList->pPrev->iSize;
				}
				else {
					i_pHeapAllocator->pFreeDescriptorList->pHeap = i_pHeapAllocator->pFreeDescriptorList->pPrev->pHeap;
					i_pHeapAllocator->pFreeDescriptorList->iSize += i_pHeapAllocator->pFreeDescriptorList->pPrev->iSize;
				}
			}
			else
			{
				if (i_pHeapAllocator->pFreeDescriptorList->pPrev) {
					break;
				}
				i_pHeapAllocator->pFreeDescriptorList = i_pHeapAllocator->pFreeDescriptorList->pPrev;
			}
		}

		ResetFreeDescriptorList(i_pHeapAllocator);
	}
}

// Overloading new and delete

void * operator new(size_t i_size)
{
	HeapAllocator *heapAllocator = GetHeapAllocator();
	return HAAllocate(i_size, heapAllocator, NewAlignment::NEW_ALIGN_DEFAULT);
}

void * operator new[](size_t i_size) {
	HeapAllocator *heapAllocator = GetHeapAllocator();
	return HAAllocate(i_size, heapAllocator, NewAlignment::NEW_ALIGN_DEFAULT);
}

void * operator new(size_t i_size, NewAlignment i_Align) {
	HeapAllocator *heapAllocator = GetHeapAllocator();
	switch (i_Align)
	{
	case NEW_ALIGN_16:
		return HAAllocate(i_size, heapAllocator, NewAlignment::NEW_ALIGN_16);
		break;
	case NEW_ALIGN_32:
		return HAAllocate(i_size, heapAllocator, NewAlignment::NEW_ALIGN_32);
		break;
	default:
		return HAAllocate(i_size, heapAllocator, NewAlignment::NEW_ALIGN_DEFAULT);
		break;
	}
}

void * operator new(size_t i_size, const char * i_pFile, unsigned int i_Line) {
	// Print the tracked information
	DEBUG_FUNCTION_NAME
	EmitLocation(i_Line, i_pFile);

	HeapAllocator *heapAllocator = GetHeapAllocator();
	return HAAllocate(i_size, heapAllocator, NewAlignment::NEW_ALIGN_DEFAULT);
}

void operator delete(void * i_ptr)
{
	HeapAllocator * heapAllocator = GetHeapAllocator();
	if (i_ptr) {
		Deallocate(i_ptr, heapAllocator);
	}
	return;
}

void operator delete[](void * i_ptr)
{
	HeapAllocator * heapAllocator = GetHeapAllocator();
	if (i_ptr) {
		Deallocate(i_ptr, heapAllocator);
	}
	return;
}

void operator delete(void * i_ptr, const char * i_pFile, unsigned int i_Line) {
	HeapAllocator * heapAllocator = GetHeapAllocator();
	if (i_ptr) {
		Deallocate(i_ptr, heapAllocator);
	}
	return;
}


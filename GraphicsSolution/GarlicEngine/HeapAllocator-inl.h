#pragma once

namespace HeapAllocatorProxy {

	size_t GetAvailableSize(const HeapAllocator * i_pHeapAllocator)
	{
		BlockDescriptor *tempFreeDescriptorList = i_pHeapAllocator->pFreeDescriptorList;
		size_t availableSize = BASE_ZERO;

		while (tempFreeDescriptorList != nullptr)
		{
			availableSize += tempFreeDescriptorList->iSize;
			tempFreeDescriptorList = tempFreeDescriptorList->pPrev;
		}

		return availableSize;
	}

	size_t GetUsedSize(const HeapAllocator * i_pHeapAllocator) {
		BlockDescriptor *tempAllocatedDescriptorList = i_pHeapAllocator->pOutstandingAllocatedList;
		size_t usedSize = BASE_ZERO;

		while (tempAllocatedDescriptorList != nullptr)
		{
			usedSize += tempAllocatedDescriptorList->iSize;
			tempAllocatedDescriptorList = tempAllocatedDescriptorList->pPrev;
		}

		return usedSize;
	}

	inline size_t ShowFreeBlocks(const HeapAllocator * i_pHeapAllocator) 
	{
		size_t freeDescriptors = i_pHeapAllocator->numberOfDescriptors - ShowAllocatedBlocks(i_pHeapAllocator);
		return freeDescriptors;
	}

	inline size_t ShowAllocatedBlocks(const HeapAllocator * i_pHeapAllocator) 
	{
		BlockDescriptor *tempOutstandingAllocatedList = i_pHeapAllocator->pOutstandingAllocatedList;
		size_t allocatedListCount = BASE_ZERO;
		do
		{
			// For checking the first entry
			if (tempOutstandingAllocatedList == nullptr) {
				break;
			}
			allocatedListCount++;
			tempOutstandingAllocatedList = tempOutstandingAllocatedList->pNext;
		} while (tempOutstandingAllocatedList != nullptr);
		return allocatedListCount;
	}

	// Resets pFreeDescriptorList to the first block
	inline void ResetFreeDescriptorList(HeapAllocator * i_pHeapAllocator)
	{
		while (i_pHeapAllocator->pFreeDescriptorList->pNext != nullptr) {
			i_pHeapAllocator->pFreeDescriptorList = i_pHeapAllocator->pFreeDescriptorList->pNext;
		}
	}
}
#pragma once
class HeapAllocator;

namespace UnitTestProxy {
	// Constants
	const int MAX_CHAR_BUFFER = 100;

	void CheckVector2DConst();
	void CheckHeapAllocator(HeapAllocator *i_pHeapAllocator);
	void CheckFixedBlockAllocator();
	void CheckGameObjectCopyAndMoveConstructor();
}


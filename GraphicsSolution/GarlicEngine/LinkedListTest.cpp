#include "LinkedListTest.h"


namespace UnitTestProxy {
	bool IsCircular(BlockDescriptor *head) {
		BlockDescriptor *slower, *faster;
		slower = head;
		faster = head->pNext; 
		while (faster != nullptr) { // Break if the faster node reaches the last point(NULL)
			// We have a circular list condition whenever a faster pointer meets the slower
			if (faster == slower || faster->pNext == slower)
				return true;
			else {
				// Move the slow pointer to its immediate next and the fast pointer to it next of next
				slower = slower->pNext;
				faster = faster->pNext->pNext;
			}
		}
		return false;
	}
}
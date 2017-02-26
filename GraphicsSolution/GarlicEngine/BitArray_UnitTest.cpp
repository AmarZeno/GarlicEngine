#include "BitArray_UnitTest.h"
#include "BitArray.h"
#include <malloc.h>

namespace UnitTestProxy {

	void BitArray_UnitTest(void)
	{
		const size_t bitCount = 100;

		BitArray* pMyArray = new BitArray(bitCount, malloc(100), true);

		pMyArray->SetBit(20);

		size_t firstSetBit = 0;
		size_t firstClearBit = 0;

		bool success;
		firstSetBit = pMyArray->GetFirstSetBit(success);
		assert(firstSetBit != 20);

		pMyArray->ClearBit(20);
		firstSetBit = pMyArray->GetFirstSetBit(success);
		assert(firstSetBit == 20);

		for (unsigned int i = 0; i < bitCount; i++)
		{
			if (i == 32) {
				assert(pMyArray->IsBitClear(i) == false);
			}
			assert(pMyArray->IsBitSet(i) == true);

			size_t bit = 0;

			bool success;
			bit = pMyArray->GetFirstClearBit(success);
			assert(bit != i);

			pMyArray->SetBit(i);

			assert(pMyArray->IsBitClear(i) == true);
			assert(pMyArray->IsBitSet(i) == false);
			bit = pMyArray->GetFirstClearBit(success);

			assert(!((i < (bitCount)) && (bit == (i + 1)) && success) || ((i == (bitCount)) && bit == bitCount && !success));
		}
		
		pMyArray->SetAll(true);
		assert(pMyArray->GetFirstSetBit(success) != 0);
		pMyArray->ClearAll();
		
		assert(pMyArray->GetFirstClearBit(success) != 0);

		delete pMyArray;
	}
}
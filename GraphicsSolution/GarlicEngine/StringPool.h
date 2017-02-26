#pragma once
#include "HeapAllocator.h"
#include <stdint.h>

namespace StringPoolProxy
{
	struct PoolSpec {
		uint8_t * m_pStartBuffer;
		uint8_t * m_pEndStrings;
		uint8_t * m_pEndBuffer;
	};

	class StringPool
	{
	public:
		static StringPool * Create(size_t i_bytesInPool);
		~StringPool();

		// find string in pool - adding it if it's not there
		const char * add(const char * i_pString);
		// find string in pool - don't add if it's not there
		const char * find(const char * i_pString);
	private:
		StringPool(uint8_t * i_pPool, size_t i_sizePool);
		PoolSpec poolSpec;
	};



	void InitializeStringPool();
	StringPool * GetStaticGlobalStringPool();
	static StringPool * gGlobalStringPool;
};


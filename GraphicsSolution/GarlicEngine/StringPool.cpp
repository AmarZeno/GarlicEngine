#include "StringPool.h"
#include "Zstring.h"
#include <stdlib.h>
#include <String.h>

namespace StringPoolProxy
{
	StringPool * StringPool::Create(size_t i_bytesInPool)
	{
		uint8_t * i_pPool = new uint8_t[i_bytesInPool];
		return (new StringPool(i_pPool, i_bytesInPool));
	}

	StringPool::StringPool(uint8_t * i_pPool, size_t i_sizePool)
	{
		poolSpec.m_pStartBuffer = i_pPool;
		poolSpec.m_pEndStrings = poolSpec.m_pStartBuffer;
		poolSpec.m_pEndBuffer = reinterpret_cast<uint8_t *>(reinterpret_cast<char *>(poolSpec.m_pStartBuffer) + i_sizePool);
	}

	StringPool::~StringPool()
	{
	}

	const char * StringPool::add(const char * i_pString)
	{
		constexpr int nullCharacterLength = 1;
		size_t stringLength = strlen(i_pString) + nullCharacterLength;
		char * stringLengthBuffer = new char[sizeof(size_t)];
		snprintf(stringLengthBuffer, 12, "%d", stringLength);
		if (find(i_pString) == nullptr) {
			Zstrcpy(reinterpret_cast<char *>(poolSpec.m_pEndStrings), stringLengthBuffer);
			poolSpec.m_pEndStrings += sizeof(size_t);
			Zstrcpy(reinterpret_cast<char *>(poolSpec.m_pEndStrings), i_pString);
			poolSpec.m_pEndStrings += stringLength;
		}
		return i_pString;
	}

	const char * StringPool::find(const char * i_pString)
	{
		constexpr int nullCharacterLength = 1;
		uint8_t * m_pCurrentRef = poolSpec.m_pStartBuffer;
		while (m_pCurrentRef != poolSpec.m_pEndStrings) {
			size_t sizeToJump = atoi(reinterpret_cast<const char *>(m_pCurrentRef));
			char * availableString = reinterpret_cast<char *>(m_pCurrentRef + sizeof(size_t));
			// strcmp returns 0 if both strings are equal
			if (strcmp(availableString, i_pString) == 0) {
				return availableString;
			}
			else {
				m_pCurrentRef += (sizeToJump + sizeof(size_t));
			}
		}
		return nullptr;
	}

	void InitializeStringPool() {
		constexpr size_t poolSize = 1000;
		gGlobalStringPool = StringPool::Create(poolSize);
		return;
	}

	StringPool * GetStaticGlobalStringPool()
	{
		return gGlobalStringPool;
	}
}
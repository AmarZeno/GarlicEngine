#include "HashedString.h"
#include <stdint.h>
#include <string.h>


HashedString::HashedString()
{
}


HashedString::~HashedString()
{
}

unsigned int HashedString::Hash(const char * i_string)
{
	return Hash(reinterpret_cast<void *>(const_cast<char *>(i_string)), strlen(i_string));
}

unsigned int HashedString::Hash(const void * i_bytes, unsigned int i_bytecount)
{
	const uint8_t * p = static_cast<const uint8_t *>(i_bytes);
	unsigned int hash = 2166136261;

	for (unsigned int i = 0; i < i_bytecount; ++i)
		hash = 16777619 * (hash ^ p[i]);

	return hash ^ (hash >> 16);
}

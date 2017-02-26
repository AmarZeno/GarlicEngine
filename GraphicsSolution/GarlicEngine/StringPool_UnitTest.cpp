#include "StringPool_UnitTest.h"
#include "PooledString.h"
#include "StringPool.h"

using namespace StringPoolProxy;


StringPool_UnitTest::StringPool_UnitTest()
{
	// Initialization of string pool
	InitializeStringPool();
	char *a = new char[100];
	a = "abc";

	char *b = new char[100];
	b = "def";

	// Adding the new char into a pooled string. Since the string does not exist in the pool, it gets added
	PooledString *pooledString = new PooledString(a);

	// Adding another char into a pooled string. Since none of the existing string matches the current string, it gets added
	PooledString *pooledString2 = new PooledString(b);

	// Adding an existing char into a pooled string. Since the string exists, the pool does not create a duplicate for the same string
	PooledString *pooledString3 = new PooledString(a);
}


StringPool_UnitTest::~StringPool_UnitTest()
{
}

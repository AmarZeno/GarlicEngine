#pragma once
#include "StringPool.h"
#include <string.h>

using namespace StringPoolProxy;

class PooledString
{
public:
	explicit PooledString(const char * i_pString) :
		m_pString(GetStaticGlobalStringPool()->add(i_pString))
	{}

	PooledString(const PooledString & i_other) :
		m_pString(i_other.m_pString)
	{}

	bool operator==(const PooledString & i_other) {
		return m_pString == i_other.m_pString;
	}

	operator bool() const
	{
		return m_pString != nullptr;
	}

	char * duplicate() const {
		return m_pString ? _strdup(m_pString) : nullptr;
	}

	const char * get() const {
		return m_pString;
	}

	~PooledString() {
	}

private:
	const char * m_pString;
};


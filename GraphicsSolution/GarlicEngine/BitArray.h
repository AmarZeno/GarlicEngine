#pragma once
#include "Diagnostics.h"
#include "EngineTypes.h"
#include <stdint.h>
class FixedSizeAllocator;

class BitArray
{
public:
	BitArray(const size_t i_numBits, void * pBitArrayStartAddress, const bool i_bInitToZero);
	~BitArray();

	BitArray * Create(const size_t i_numBytes, const bool i_initToZero, void * pBitArrayStartAddress);
	void ClearAll(void);
	void SetAll(bool i_setOccupied);

	bool AreAllClear(void) const;
	bool AreAllSet(void) const;

	bool IsBitSet(const size_t i_bitNumber) const;
	bool IsBitClear(const size_t i_bitNumber) const;

	void SetBit(const size_t i_bitNumber);
	void ClearBit(const size_t i_bitNumber);

	size_t GetFirstClearBit(bool &success) const;
	size_t GetFirstSetBit(bool &success) const;
	size_t GetFullSkipValue() const;
	void InitializeConstants();

	bool operator[](const size_t &i_index) const;

private:
	// Constants
	size_t bitsPerBytes;
	size_t extraMemoryOffset; // Add this to indicate the end of the required bit set

	 size_t bitsRegistersProcess;
	 size_t m_numBits;
	 size_t m_offsetedNumBits;
	 guint_t * m_pBytes;
	 guint_t leftShiftConstant;
};

size_t GetBitsRegistersProcess();
size_t GetBitArrayStorageSize(const size_t i_numOfSegments);
size_t GetBitArrayHeapSize(const size_t i_numOfSegments);

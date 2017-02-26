#include "BitArray.h"
#include <intrin.h>
#include <string.h>
#include <math.h> 

BitArray::BitArray(const size_t i_numBits, void * pBitArrayStartAddress, const bool i_bInitToZero)
{
	Create(i_numBits, i_bInitToZero, pBitArrayStartAddress);
}

BitArray::~BitArray()
{
}

BitArray * BitArray::Create(const size_t i_numBits, const bool i_initToZero, void * pBitArrayStartAddress)
{
	InitializeConstants();
	bitsRegistersProcess = GetBitsRegistersProcess();
	m_numBits = i_numBits;
	m_offsetedNumBits = GetBitArrayStorageSize(i_numBits) * bitsPerBytes;
	m_pBytes = reinterpret_cast<guint_t*>(reinterpret_cast<uintptr_t>(pBitArrayStartAddress) + sizeof(BitArray));
	assert(m_pBytes == nullptr ? true:false);
	memset(m_pBytes, i_initToZero ? 0 : ~0, m_offsetedNumBits/ bitsPerBytes);
	return nullptr;
}

void BitArray::InitializeConstants()
{
	leftShiftConstant = 1i64;
	bitsPerBytes = 8;
	extraMemoryOffset = 2;
}

void BitArray::ClearAll(void)
{
	SetAll(false);
}

void BitArray::SetAll(bool i_setOccupied)
{
	memset(m_pBytes, i_setOccupied == true? ~0 : 0, m_offsetedNumBits/ bitsPerBytes);
}

bool BitArray::AreAllClear(void) const
{
	size_t m_emptyBitSetCount = 0;
	size_t m_numBytes = m_numBits / bitsRegistersProcess;

	// quick skip bytes where no bits are set   
	guint_t checkValue;
#ifdef _WIN64
	checkValue = 0x0000000000000000;
#else
	checkValue = 0x00000000;
#endif

	while ((m_pBytes[m_emptyBitSetCount] == checkValue) && (m_emptyBitSetCount < m_numBytes)) {
		m_emptyBitSetCount++;
	}

	return m_emptyBitSetCount < m_numBytes ? false : true;
}

bool BitArray::AreAllSet(void) const
{
	size_t m_filledBitSetCount = 0;
	size_t m_numBytes = m_numBits / bitsRegistersProcess;

	// Faster skip at byte level  
	guint_t checkValue;
#ifdef _WIN64
	checkValue = UINT64_MAX;
#else
	checkValue = UINT32_MAX;
#endif

	while ((m_pBytes[m_filledBitSetCount] == checkValue) && (m_filledBitSetCount < m_numBytes)) {
		m_filledBitSetCount++;
	}

	return m_filledBitSetCount < m_numBytes? false : true;
}

bool BitArray::IsBitSet(const size_t i_bitNumber) const
{
	size_t numBytes = i_bitNumber / bitsRegistersProcess;
	if (m_pBytes[numBytes] & leftShiftConstant << (i_bitNumber - (numBytes * bitsRegistersProcess))) {
		return true;
	}
	return false;
}

bool BitArray::IsBitClear(const size_t i_bitNumber) const
{
	return !IsBitSet(i_bitNumber);
}

void BitArray::SetBit(const size_t i_bitNumber)
{
	size_t numBytes = i_bitNumber / bitsRegistersProcess;
	// Offset the bitnumber in the order of 0 - 7
	// Set the requested bit
	m_pBytes[numBytes] |= leftShiftConstant << (i_bitNumber - (numBytes * bitsRegistersProcess));
}

void BitArray::ClearBit(const size_t i_bitNumber)
{
	size_t numBytes = i_bitNumber / bitsRegistersProcess;
	// Offset the bitnumber in the order of 0 - 7
	// Clear the requested bit
	m_pBytes[numBytes] &= ~(leftShiftConstant << (i_bitNumber - (numBytes * bitsRegistersProcess)));
}

size_t BitArray::GetFirstClearBit(bool &success) const
{
	success = false;
	size_t m_filledBitSetCount = 0;
	size_t m_numBytes = m_offsetedNumBits / bitsRegistersProcess;
	// Faster skip at byte level  
	guint_t checkValue;
#ifdef _WIN64
	checkValue = UINT64_MAX;
#else
	checkValue = UINT32_MAX;
#endif
	while ((m_pBytes[m_filledBitSetCount] == checkValue) && (m_filledBitSetCount < m_numBytes)) {
		m_filledBitSetCount++;
	}
	size_t bitSpot = 0;
	if (m_filledBitSetCount < (static_cast<float>(m_offsetedNumBits) / static_cast<float>(bitsRegistersProcess))) { // Finding bits in full bytes
		unsigned long bit = 0;

		// X-OR for BitScanForward to find the first clear bit
		guint_t reverseMask = checkValue;
		
#ifdef _WIN64
		_BitScanForward64(&bit, m_pBytes[m_filledBitSetCount] ^ reverseMask);
#else
		_BitScanForward(&bit, m_pBytes[m_filledBitSetCount] ^ reverseMask);
#endif

		// Calculate bitSpot based on byte and the bit
		bitSpot = m_filledBitSetCount * bitsRegistersProcess + bit;
		success = true;
	}
	else if (m_filledBitSetCount == (static_cast<float>(m_offsetedNumBits) / static_cast<float>(bitsRegistersProcess))) {
		bitSpot = m_numBits + extraMemoryOffset; // Extra offset of 2 from the original numbits
		success = false;
	}
	return bitSpot; // All bit sets are full
}

size_t BitArray::GetFirstSetBit(bool &success) const
{
	success = false;
	size_t m_emptyBitSetCount = 0;
	size_t m_numBytes = m_offsetedNumBits /bitsRegistersProcess;

	// Faster skip at byte level 
	int checkValue;
#ifdef _WIN64
	checkValue = 0x0000000000000000;
#else
	checkValue = 0x00000000;
#endif

	while ((m_pBytes[m_emptyBitSetCount] == checkValue) && (m_emptyBitSetCount < m_numBytes)) {
		m_emptyBitSetCount++;
	}

	size_t bitSpot = 0;
	if (m_emptyBitSetCount < (static_cast<float>(m_offsetedNumBits) / static_cast<float>(bitsRegistersProcess))) {
		
		unsigned long bit;
#ifdef _WIN64
		_BitScanForward64(&bit, m_pBytes[m_emptyBitSetCount]);
#else
		_BitScanForward(&bit, m_pBytes[m_emptyBitSetCount]);
#endif
		// Calculate bitSpot based on byte and the bit
		bitSpot = m_emptyBitSetCount * bitsRegistersProcess + bit;
		success = true;
	}
	else if (m_emptyBitSetCount == (static_cast<float>(m_offsetedNumBits) / static_cast<float>(bitsRegistersProcess))) {
		bitSpot = m_numBits + extraMemoryOffset; // Extra offset of 2 from the original numbits
		success = false;
	}
	return bitSpot; // All bit sets are empty
}

bool BitArray::operator[](const size_t &i_index) const
{
	size_t numBytes = i_index / bitsRegistersProcess;
	if (m_pBytes[numBytes] & leftShiftConstant << (i_index - (numBytes * bitsRegistersProcess))) {
		return true;
	}
	return false;
}

size_t BitArray::GetFullSkipValue() const
{
	return m_numBits + extraMemoryOffset;
}

size_t GetBitsRegistersProcess() {
#ifdef _WIN64
	return 64;
#endif
	return 32;
}

size_t GetBitArrayStorageSize(const size_t i_numOfSegments) {
	size_t bitsPerBytes = 8;
	size_t bitsRegistersProcess = GetBitsRegistersProcess();
	size_t numBytesCeiled = static_cast<size_t>(ceil(static_cast<float>(i_numOfSegments) / static_cast<float>(bitsRegistersProcess)));
	return (numBytesCeiled * bitsRegistersProcess) / bitsPerBytes;
}

size_t GetBitArrayHeapSize(const size_t i_numOfSegments) {
	return GetBitArrayStorageSize(i_numOfSegments) + sizeof(BitArray);
}


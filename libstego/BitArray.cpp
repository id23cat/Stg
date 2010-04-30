#ifdef _WIN32
#include "stdafx.h"
#endif
#include "BitArray.h"

BitArray::BitArray(void)
{
	arrayBitLength = 0;
	maxByteIndex = 0;
	maxBitIndex = 0;
}

BitArray::BitArray(size_t len):Array(len)
{
	arrayBitLength = len*8;
	maxByteIndex = len-1;
	maxBitIndex = 7;
}

BitArray::~BitArray(void)
{
}

void BitArray::SetArray(BYTE *ar, size_t len)
{
	maxByteIndex = len-1;
	maxBitIndex = 7;
	Array::SetArray(ar, len);
}

BYTE BitArray::GetBit(size_t byte, size_t bit) throw (...)
{
	if(byte>maxByteIndex || bit>maxBitIndex)
		throw Exception("Index out of range");
	BYTE c = Array::array[byte];
	c = c << bit;
	c = (c & 0x80)>>7;
	return c;
}

void BitArray::SetBit(size_t byte, size_t bit, BYTE b) throw (...)
{
	if(byte>maxByteIndex || bit>maxBitIndex)
		throw Exception("Index out of range");

	BYTE c = Array::array[byte];
	BYTE bb = b << 7-bit;
	c = c | bb;
	Array::array[byte] = c;
}

BitArray::BitArrayIterator& BitArray::Begin()
{
	//BitArrayIterator it(0,0,this);
	return *new BitArrayIterator(0,0,this);
}

BitArray::BitArrayIterator& BitArray::End()
{
	//BitArrayIterator it(maxByteIndex, maxBitIndex,this);
	return *new BitArrayIterator(maxByteIndex, maxBitIndex,this);
}

/////////////BitArrayIterator//////////
BitArray::BitArrayIterator::BitArrayIterator()
{
	byteIndex = 0;
	bitIndex = 0;
	pArray = NULL;
}

BitArray::BitArrayIterator::BitArrayIterator(size_t byte, size_t bit, BitArray
*par) throw (...)
{
	byteIndex = byte;
	bitIndex = bit;
	pArray = par;
	if(byteIndex > pArray->maxByteIndex || bitIndex > pArray->maxBitIndex)
		throw Exception("Out of array range");
}

BitArray::BitArrayIterator::~BitArrayIterator()
{
}

void BitArray::BitArrayIterator::SetBitArray(BitArray *bar) throw (...)
{
	byteIndex = 0;
	bitIndex = 0;
	pArray = bar;
	if(byteIndex > pArray->maxByteIndex || bitIndex > pArray->maxBitIndex)
		throw Exception("Out of array range");
}

BitArray::BitArrayIterator &BitArray::BitArrayIterator::operator[](size_t
index)throw (...)
{
	byteIndex = (size_t)index/8;
	bitIndex = index%8;
	if(byteIndex > pArray->maxByteIndex)
		throw Exception("Out of array range");
	return *this;
}

BitArray::BitArrayIterator &BitArray::BitArrayIterator::operator=(BYTE bit)
{
	pArray->SetBit(byteIndex, bitIndex, bit);
	pArray->actualArrayLength = byteIndex;
	return *this;
}

BitArray::BitArrayIterator &BitArray::BitArrayIterator::operator=(BitArrayIterator &it)
{
	byteIndex = it.byteIndex;
	bitIndex = it.bitIndex;
	pArray = it.pArray;
	return *this;
}



bool BitArray::BitArrayIterator::operator<(BitArrayIterator it)
{
	if(byteIndex < it.byteIndex)
		return true;
	if(byteIndex==it.byteIndex && bitIndex < it.bitIndex)
		return true;
	return false;
}

BitArray::BitArrayIterator &BitArray::BitArrayIterator::operator++()throw
(...)
{
	if(bitIndex < pArray->maxBitIndex)
		bitIndex++;
	else if(byteIndex < pArray->maxByteIndex)
	{
		bitIndex=0;
		byteIndex++;
	}
	else
		throw Exception("operator ++: out of range");
	return *this;
}

BitArray::BitArrayIterator &BitArray::BitArrayIterator::operator++(int i) throw
(...)
{
	if(bitIndex < pArray->maxBitIndex)
		bitIndex++;
	else if(byteIndex < pArray->maxByteIndex)
	{
		bitIndex=0;
		byteIndex++;
	}
	else
		throw Exception("operator ++: out of range");
	return *this;
}

size_t BitArray::BitArrayIterator::operator-(BitArrayIterator &it)
{
	return (byteIndex-it.byteIndex)*8 + (bitIndex-it.bitIndex);
}

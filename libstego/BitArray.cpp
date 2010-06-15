#ifdef _WIN32

#endif
#include "stdafx.h"
#include "BitArray.h"
#include "iostream"
using namespace std;

BitArray::BitArray(void)
{
	//arrayBitLength = 0;
	//maxByteIndex = 0;
	//maxBitIndex = 0;
}

BitArray::BitArray(size_t len):Array(len)
{
	//arrayBitLength = len*8;
	//maxByteIndex = len-1;
	//maxBitIndex = 7;
}

BitArray::~BitArray(void)
{
}

//void BitArray::SetArray(BYTE *ar, size_t len)
//{
//	//maxByteIndex = len-1;
//	//maxBitIndex = 7;
//	Array::SetArray(ar, len);
//}

/*
void BitArray::AddArray(BYTE *ar, size_t len)
{
	//maxByteIndex += len;
	Array::AddArray(ar,len);
}
*/

//void BitArray::AddArrayToBegin(BYTE *ar, size_t len)
//{
//	maxByteIndex += len;
//}

BYTE BitArray::GetBit(size_t byte, size_t bit) throw (...)
{
	if(((byte>=arrayLength) && arrayLength) || bit>7)
		throw Exception("Index out of range");
	BYTE c = Array::array[byte];
	c = c << bit;
	c = (c & 0x80)>>7;

	//debug
#ifdef _DEBUG
	cerr << (int)c;
#endif
	return c;
}

/************!!!!!!!!!!!!!!!!!!!*****************/
void BitArray::SetBit(size_t byte, size_t bit, BYTE b) throw (...)
{
		//debug
//#ifdef _DEBUG
//	cerr << (int)b;
//#endif
	/*if(byte>maxByteIndex || bit>maxBitIndex)*/
	if((byte==arrayLength) || !arrayLength)
		reAllocate(1);
	if(((byte>=arrayLength) && arrayLength) || bit>7)
		throw Exception("Index out of range");

	BYTE c = Array::array[byte];
	BYTE bb = b << 7-bit;
	c = c | bb;
	Array::array[byte] = c;
	//if is the last bit of the last byte
	//if(freeLength==0&&bit==7)
	//{
	//	Array::reAllocate(16);			//Add to the array new 16 bytes
	//	//maxByteIndex++;
	//}
}
/************!!!!!!!!!!!!!!!!!!!*****************/

BitArray::BitArrayIterator& BitArray::Begin()
{
	//BitArrayIterator it(0,0,this);
	return *new BitArrayIterator(0,0,this);
}

BitArray::BitArrayIterator& BitArray::End()
{
	//return *new BitArrayIterator(maxByteIndex, maxBitIndex,this);
	return *new BitArrayIterator(arrayLength-1, 7,this);
}

/////////////BitArrayIterator//////////
BitArray::BitArrayIterator::BitArrayIterator()
{
	byteIndex = 0;
	bitIndex = 0;
	pArray = NULL;
}

BitArray::BitArrayIterator::BitArrayIterator(size_t byte, size_t bit, BitArray  *par) throw (...)
{
	byteIndex = byte;
	bitIndex = bit;
	pArray = par;
	if(((byteIndex >= pArray->arrayLength) && pArray->arrayLength) || bitIndex > 7)
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
	if(((byteIndex >= pArray->arrayLength) && pArray->arrayLength) || bitIndex > 7)
		throw Exception("Out of array range");
}

BitArray::BitArrayIterator &BitArray::BitArrayIterator::operator[](size_t index)throw (...)
{
	if(index >= pArray->arrayBitLength())
		throw Exception("Out of array range");
	byteIndex = (size_t)index/8;
	bitIndex = index%8;
	return *this;
}

BitArray::BitArrayIterator &BitArray::BitArrayIterator::operator=(BYTE bit)
{
	pArray->SetBit(byteIndex, bitIndex, bit);
	if(bitIndex==0)
		pArray->freeLength--;
	//pArray->actualArrayLength = byteIndex;
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

BitArray::BitArrayIterator &BitArray::BitArrayIterator::operator++()throw (...)
{
	//if(bitIndex < pArray->maxBitIndex)
	if(bitIndex < 7)
		bitIndex++;
	/*else if(byteIndex < pArray->maxByteIndex)*/
	else if(byteIndex < pArray->arrayLength)
	{
		bitIndex=0;
		byteIndex++;
		//pArray->freeLength--;
	}
	else if(byteIndex == pArray->arrayLength)
	{
		pArray->reAllocate(1);
		bitIndex=0;
		byteIndex++;
		//pArray->freeLength--;
	}
	else
		throw Exception("operator ++: out of range");
	return *this;
}

//BitArray::BitArrayIterator &BitArray::BitArrayIterator::operator++(int i) throw
//(...)
//{
//	if(bitIndex < 7)
//		bitIndex++;
//	else if(byteIndex < pArray->arrayLength-1)
//	{
//		bitIndex=0;
//		byteIndex++;
//		pArray->freeLength--;
//	}
//	else
//		throw Exception("operator ++: out of range");
//	return *this;
//}

size_t BitArray::BitArrayIterator::operator-(BitArrayIterator &it)
{
	return (byteIndex-it.byteIndex)*8 + (bitIndex-it.bitIndex);
}

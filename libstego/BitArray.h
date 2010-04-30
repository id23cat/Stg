#pragma once
#include "Array.h"

#ifdef _WIN32
//#ifdef LIBOGGSTEGODLL_EXPORTS
//#define LIBOGGSTEGODLL __declspec(dllexport)
//#else
//#define LIBOGGSTEGODLL __declspec(dllimport)
//#endif
//#endif
//
//
//#ifdef _WIN32
//class LIBOGGSTEGODLL BitArray: public Array
//#else
class BitArray: public Array
#endif
{
public:
	class BitArrayIterator
	{
		BitArray *pArray;
		size_t byteIndex;
		size_t bitIndex;
	public:
		BitArrayIterator();
		BitArrayIterator(size_t byte, size_t bit, BitArray *par) throw(...);
		~BitArrayIterator();
		void SetBitArray(BitArray *bar) throw(...);
		BitArrayIterator &operator[](size_t index)  throw(...);
		BitArrayIterator &operator=(BYTE bit);
		BitArrayIterator &operator=(BitArrayIterator &it);
		bool operator<(BitArrayIterator it);
		operator BYTE()
		{
			return this->pArray->GetBit(this->byteIndex, this->bitIndex);
		};
		BitArrayIterator &operator++()  throw(...);
		BitArrayIterator &operator++(int i) throw(...);
		size_t operator-(BitArrayIterator &it);

	};

private:
	BYTE selectedByte[8];
	size_t arrayBitLength;
	size_t maxByteIndex;
	size_t maxBitIndex;

public:
	BitArray(void);
	BitArray(size_t len);
	~BitArray(void);
	void SetArray(BYTE *ar, size_t len);
	BYTE GetBit(size_t byte, size_t bit) throw(...);
	void SetBit(size_t byte, size_t bit, BYTE b) throw(...);
	BitArrayIterator& Begin();
	BitArrayIterator& End();
};

/*
BYTE& operator=(BYTE &bit, const BitArray::BitArrayIterator &it)
{
	//bit = it.pArray->GetBit(it.byteIndex, it.bitIndex);
	bit = 1;
	return bit;
}
*/

/*
class MyClass
{

public:
	int xxx;
};

int& operator=(int& left, const MyClass& obj)
{

	left = obj.xxx;
	return left;
}
*/

//BYTE TestFun()
//{
//
//BitArray TST;
//BYTE TST2;
//
//TST2 = TST.Begin();
//
//return TST2;
//}
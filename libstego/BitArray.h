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
	protected:
		BitArray *pArray;
		size_t byteIndex;		//index in the BitArray object [0 ... length]
		size_t bitIndex;		//index in the each BYTE of BitArray object [0...7]
		bool enableOORException;	//allows to throw OutOfRangeException
									//if OutOfRangeException not allowed,
									//then bit array will be shifted to left on 1 bit
									//with each calling operator++()
									//true by default -- uses for encoders
	public:
		BitArrayIterator();
		BitArrayIterator(size_t byte, size_t bit, BitArray *par) throw(OutOfRangeException);	//byte -> byteIndex,
																				//bit -> bitIndex,
																				//par -> pArray
		~BitArrayIterator();
		virtual void SetBitArray(BitArray *bar) throw(OutOfRangeException);	//sets pointer to object
		virtual BitArrayIterator &operator[](size_t index)  throw(OutOfRangeException);	//index by bits in BitArray
		virtual BitArrayIterator &operator=(BYTE bit) throw(Exception, OutOfRangeException); //sets bit to current position in BitArray object
		virtual BitArrayIterator &operator=(BitArrayIterator &it);		//copy constructor
		virtual bool operator<(BitArrayIterator it);					//compare two iterators
		virtual operator BYTE()											//convertion to BYTE
		{
			return this->pArray->GetBit(this->byteIndex, this->bitIndex);
		};
		virtual BitArrayIterator &operator++() throw (OutOfRangeException);				//increment iterator index
		//virtual BitArrayIterator &operator++(int i) throw(...);
		virtual size_t operator-(BitArrayIterator &it);
		void SetOutOfRangeExceptionStatus(bool b) {enableOORException = b;};	//Sets OutOfRangeException status
	};

private:
	BYTE selectedByte[8];						//disintegrate BYTE in bits
	//size_t arrayBitLength;						//length in bits
	/*size_t maxByteIndex;						
	size_t maxBitIndex;*/
	size_t arrayBitLength(){return busyArrayLength()*8;};
	void shiftLeft();						//shifts to the left on one bit
	
public:
	BitArray(void);
	BitArray(size_t len);
	~BitArray(void);
	//void SetArray(BYTE *ar, size_t len);		
	//void AddArray(BYTE *ar, size_t len);
	//void AddArrayToBegin(BYTE *ar, size_t len);
	BYTE GetBit(size_t byte, size_t bit) throw(Exception, OutOfRangeException);
	virtual void SetBit(size_t byte, size_t bit, BYTE b) throw(Exception, OutOfRangeException);
	BitArrayIterator& Begin() throw(OutOfRangeException);		//returns iterator pointed to the begin of the Array
	BitArrayIterator& End() throw(OutOfRangeException);					//returns iterator pointed to the end of the Array
	size_t ArrayBitLength(){return busyArrayLength()*8;};
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
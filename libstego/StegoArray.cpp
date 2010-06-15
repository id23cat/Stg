#include "StdAfx.h"
#include "StegoArray.h"
#include "AP_CRC16.h"

StegoArray::StegoArray(void)
{
	beginFlag = BEG_FLAG;
	endFlag = END_FLAG;
}

StegoArray::StegoArray(size_t len):BitArray(len)
{
	beginFlag=BEG_FLAG;
	endFlag=END_FLAG;
}

StegoArray::~StegoArray(void)
{
}

void StegoArray::SetMessage(BYTE *mes, size_t len)
{
	unsigned short int crcCode;
	AddArray((BYTE*)beginFlag, BEG_LEN);
	AddArray((BYTE*)&len, LEN_LEN);
	AddArray(mes, len);
	messageLength = len;
	//calculate CRC16 code for message
	{
		APCRC16 crc16;
		crc16.InitializeCRC16System();
		crc16.ComputeCRC16_ByteArray(mes,len,crcCode);
		crc16.CleanupCRC16System();
	}
	AddArray((BYTE*)&crcCode, CRC_LEN);
	AddArray((BYTE*)endFlag, END_LEN);

	//SetArray(&len, 4);
}
BYTE* StegoArray::GetMessage(size_t& len)
{
	len = messageLength;
	return array + BEG_LEN + LEN_LEN;
}


void StegoArray::SetBit(size_t byte, size_t bit, BYTE b) throw (...)
{
	BitArray::SetBit(byte, bit, b);

	if(byte==BEG_LEN-1 && bit==7)			//checking of the  flag of the begining
	{
		BYTE *ar;
		//pArray->GetArray(ar);
		//if(strncmp((char*)ar, ((StegoArray*)pArray)->beginFlag, 4))
		if(strncmp((char*)array, beginFlag, BEG_LEN))
		{
			throw (int)1;
		}
	}
	if(byte==BEG_LEN+LEN_LEN-1 && bit==7)			//getting message length
	{
		memcpy(&messageLength,array+BEG_LEN, LEN_LEN);
		//size_t *psize = (size_t*)array+4;
		//messageLength = *psize;
	}
	if(byte==BEG_LEN+LEN_LEN+messageLength+CRC_LEN-1 && bit==7)		//check crc code
	{
		BYTE *t;
		t=array+BEG_LEN+LEN_LEN+messageLength;
		unsigned short int crc1, *crcptr;
		crcptr = (unsigned short*)t;
		crc1 = *crcptr;
		
		unsigned short int crc2;
		APCRC16 crc16;
		crc16.InitializeCRC16System();
		crc16.ComputeCRC16_ByteArray(array+BEG_LEN+LEN_LEN,messageLength,crc2);
		crc16.CleanupCRC16System();
		if(crc1 != crc2)
			throw (int)1;
	}
}

StegoArray::StegoArrayIterator& StegoArray::Begin()
{
	//BitArrayIterator it(0,0,this);
	return *new StegoArrayIterator(0,0,this);
}

StegoArray::StegoArrayIterator& StegoArray::End()
{
	//return *new BitArrayIterator(maxByteIndex, maxBitIndex,this);
	return *new StegoArrayIterator(arrayLength-1, 7,this);
}



/// StegoArrayIterator
StegoArray::StegoArrayIterator::StegoArrayIterator()
{
}

StegoArray::StegoArrayIterator::StegoArrayIterator(size_t byte, size_t bit, StegoArray  *par)
:BitArray::BitArrayIterator(byte,bit,par)		
{
	
}

StegoArray::StegoArrayIterator &StegoArray::StegoArrayIterator::operator[](size_t index)  throw(...)
{
	BitArrayIterator::operator[](index);
	return *this;
}

StegoArray::StegoArrayIterator &StegoArray::StegoArrayIterator::operator=(BYTE bit)
{
	BitArrayIterator::operator=(bit);
	return *this;
}

StegoArray::StegoArrayIterator &StegoArray::StegoArrayIterator::operator=(StegoArrayIterator &it)
{
	BitArrayIterator::operator=(it);
	return *this;
}

StegoArray::StegoArrayIterator &StegoArray::StegoArrayIterator::operator++()
{
	BitArrayIterator::operator++();
	return *this;
}

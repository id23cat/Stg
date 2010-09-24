#include "StdAfx.h"
#include "StegoArray.h"
#include "AP_CRC16.h"
#ifdef _DEBUG
#include "iostream"
using namespace std;
#endif

StegoArray::StegoArray(void)
{
	beginFlag = BEG_FLAG;
	endFlag = END_FLAG;
	messageLength = 0;
	allocate(BEG_LEN);
}

StegoArray::StegoArray(size_t len):BitArray(len)
{
	beginFlag=BEG_FLAG;
	endFlag=END_FLAG;
	messageLength = 0;
	allocate(BEG_LEN);
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


void StegoArray::SetBit(size_t byte, size_t bit, BYTE b)  throw(Exception,
																OutOfRangeException,
																DamagedMessageException,
																EndOfMessageException)
{
	BitArray::SetBit(byte, bit, b);

	if(byte==BEG_LEN-1 && bit==7)			//checking flag of the begining
	{
		//BYTE *ar;
		//pArray->GetArray(ar);
		//if(strncmp((char*)ar, ((StegoArray*)pArray)->beginFlag, 4))
		if(!strncmp((char*)array, beginFlag, BEG_LEN))
		{
			addMemory(BEG_LEN + LEN_LEN);
			//cout << "begins\n";
			//throw (int)1;								// getting begins
		}
	}
	else if(byte==BEG_LEN+LEN_LEN-1 && bit==7)			//getting message length
	{
		memcpy(&messageLength,array+BEG_LEN, LEN_LEN);
		addMemory(BEG_LEN + LEN_LEN + messageLength + CRC_LEN);
		//size_t *psize = (size_t*)array+4;
		//messageLength = *psize;
	}
	else if(byte==BEG_LEN+LEN_LEN+messageLength+CRC_LEN-1 && bit==7)		//check crc code
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
			throw DamagedMessageException("CRC checksum not equal",array + BEG_LEN + LEN_LEN,messageLength);
	}
	else if(byte==BEG_LEN+LEN_LEN+messageLength+CRC_LEN+END_LEN-1 && bit==7)			//checking of the  flag of the begining
	{
		//BYTE *ar;
		//pArray->GetArray(ar);
		//if(strncmp((char*)ar, ((StegoArray*)pArray)->beginFlag, 4))
		char *endf = (char*)array+BEG_LEN+LEN_LEN+messageLength+CRC_LEN;
		if(!strncmp((char*)endf, endFlag, END_LEN))
		{
			//allocate(BEG_LEN);
			throw EndOfMessageException("Getting finished!", messageLength);								// getting finished
		}
		else
			throw DamagedMessageException("End flag may be damaged",array + BEG_LEN + LEN_LEN,messageLength);
	}
}

void StegoArray::Initialize() throw(Exception)
{
	allocate(BEG_LEN);
	messageLength = 0;
}

#ifdef _DEBUG
void StegoArray::DEBUG_Print_Array()
{
	cerr << "DEBUG ARRAY:\n";
	for(int i=0;i<arrayLength;i++)
		cerr << array[i] << " ";
	cerr << endl;
}

void StegoArray::DEBUG_Print_Bits()
{
	cerr << "DEBUG BITS:\n";
	for(int i=0;i<arrayLength;i++)
	{
		for (int j=0;j<8;j++)
			cerr << (int)GetBit(i,j);
		cerr << " ";
	}
	cerr << endl;
}
#endif

//StegoArray::StegoArrayIterator& StegoArray::Begin()
//{
//	//BitArrayIterator it(0,0,this);
//	//return *new StegoArrayIterator(0,0,this);
//
//}
//
//StegoArray::StegoArrayIterator& StegoArray::End()
//{
//	//return *new BitArrayIterator(maxByteIndex, maxBitIndex,this);
//	return *new StegoArrayIterator(arrayLength-1, 7,this);
//}



///// StegoArrayIterator
//StegoArray::StegoArrayIterator::StegoArrayIterator()
//{
//}
//
//StegoArray::StegoArrayIterator::StegoArrayIterator(size_t byte, size_t bit, StegoArray  *par)
//:BitArray::BitArrayIterator(byte,bit,par)		
//{
//	
//}
//
//StegoArray::StegoArrayIterator &StegoArray::StegoArrayIterator::operator[](size_t index)  throw(...)
//{
//	BitArrayIterator::operator[](index);
//	return *this;
//}
//
//StegoArray::StegoArrayIterator &StegoArray::StegoArrayIterator::operator=(BYTE bit)
//{
//	BitArrayIterator::operator=(bit);
//	return *this;
//}
//
//StegoArray::StegoArrayIterator &StegoArray::StegoArrayIterator::operator=(StegoArrayIterator &it)
//{
//	BitArrayIterator::operator=(it);
//	return *this;
//}
//
//StegoArray::StegoArrayIterator &StegoArray::StegoArrayIterator::operator++()
//{
//	BitArrayIterator::operator++();
//	return *this;
//}

#ifdef _WIN32

#endif
#include "stdafx.h"
#include "Array.h"
#include <stdio.h>
#include <stdlib.h>

void Array::allocate(size_t len) throw(Exception)
{
	if(array)
		delete array;
	arrayLength = len;
	array = new BYTE[arrayLength];
	if(!array)
	{
		arrayLength=0;
		throw Exception("Can not allocate memory in  Array::Allocate(size_t len)");		
	}
	memset(array,0,arrayLength);
	freeLength=arrayLength;
}

void Array::addMemory(size_t len) throw(Exception)
{
	if(!array)
		allocate(len);
	else
	{
		arrayLength+=len;
		BYTE *temp = new BYTE[arrayLength];
		if(!temp)
		{
			arrayLength-=len;
			throw Exception("Can not add memory in  Array::addMemory(size_t len)");		
		}
		freeLength = len+freeLength;
		memcpy(temp,array, arrayLength-len);
		delete array;
		array = temp;
		memset(ptrToFreeArray(),0,freeLength);
	}
}

BYTE *Array::ptrToFreeArray()
{
	return array+arrayLength-freeLength;
}

size_t Array::busyArrayLength()
{
	return arrayLength-freeLength;
}

Array::Array(void)
{
	array=NULL;
	arrayLength=0;
	freeLength=0;
	//totalArrayLength = 0;
	//lenLength = sizeof(size_t);
	//crcLength = 4;				//crc length
}

Array::Array(size_t len) throw(Exception)
{
	allocate(len);
	memset(array,0,arrayLength);
	freeLength = arrayLength;
}

Array::~Array(void)
{
	if(array)
		delete array;
}

//void Array::SetArray(BYTE *ar, size_t len) throw (...)
//{
//	allocate(len);
//	if(ar)
//	{
//		memcpy(array, ar, arrayLength);
//		freeLength = 0;		
//	}else{
//		memset(array,0,arrayLength);
//		freeLength = arrayLength;
//	}
//}

void Array::AddArray(BYTE *ar, size_t len) throw(Exception)
{
	if(freeLength<len)
		addMemory(len);
	if ((ar) || (len > 0))
	{		
		memcpy(ptrToFreeArray(), ar, len);
		freeLength -= len;
	}else{
		memset(ptrToFreeArray(),0,len);		
	}
}

size_t Array::GetArray(BYTE *ar)
{
	memcpy(ar, array, arrayLength);
	return busyArrayLength();
}

size_t Array::GetArrayLength()
{
	return busyArrayLength();
}

void Array::Initialize()
{
	delete array;
	array=NULL;
	arrayLength=0;
	freeLength=0;	
}

void Array::AddMem(size_t length)
{
	addMemory(length);	
}

bool Array::IsArraySet()
{
	if(array && arrayLength!=freeLength)
		return true;
	return false;
}

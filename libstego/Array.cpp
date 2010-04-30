#ifdef _WIN32
#include "stdafx.h"
#endif
#include "Array.h"
#include <stdio.h>
#include <stdlib.h>

Array::Array(void)
{
	array=NULL;
	arrayLength=0;
	actualArrayLength = 0;
}

Array::Array(size_t len) throw(...)
{
	array = new BYTE[len];
	if(!array)
	{
		throw Exception("Can not allocate memory in constructor Array::Array(size_t len)");
		//exit(1);
	}
	arrayLength = len;
	memset(array,0,arrayLength);
	actualArrayLength = 0;
}

Array::~Array(void)
{
	if(array)
		delete array;
}

void Array::SetArray(BYTE *ar, size_t len) throw (...)
{
	array = new BYTE[len];
	if(!array)
	{
		throw Exception("Can not allocate memory in  Array::SetArray(BYTE *ar, size_t len) function");
		//exit(1);
	}
	arrayLength = len;
	if(ar)
	{
		memcpy(array, ar, arrayLength);
		actualArrayLength = arrayLength;
	}else{
		memset(array,0,arrayLength);
		actualArrayLength = 0;
	}
}

size_t Array::GetArray(BYTE *ar)
{
	memcpy(ar, array, arrayLength);
	return actualArrayLength;
}

size_t Array::GetArrayLength()
{
	return arrayLength;
}

bool Array::IsArraySet()
{
	if(array && arrayLength)
		return true;
	return false;
}

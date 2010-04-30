#pragma once
#include <memory.h>
#include "Exception.h"
#ifndef _WIN32
#include "stddef.h"
#endif

//#ifdef _WIN32
//#ifdef LIBOGGSTEGODLL_EXPORTS
//#define LIBOGGSTEGODLL __declspec(dllexport)
//#else
//#define LIBOGGSTEGODLL __declspec(dllimport)
//#endif
//#endif
//#ifdef _WIN32
//typedef LIBOGGSTEGODLL unsigned char BYTE;
//#else
typedef unsigned char BYTE;
//#endif
//
//
//#ifdef _WIN32
//class LIBOGGSTEGODLL Array
//#else
class Array
//#endif
{
protected:
	BYTE *array;
	size_t arrayLength;
	size_t actualArrayLength;
public:
	Array(void);
	Array(size_t len) throw(...);
	~Array(void);
	void SetArray(BYTE *ar, size_t len) throw(...);
	size_t GetArray(BYTE *ar);
	size_t GetArrayLength();
	bool IsArraySet();
};

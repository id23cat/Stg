#pragma once
#include <memory.h>
#include "Exception.h"
#ifndef _WIN32
#include "stddef.h"
#endif

#ifdef _DEBUG
	//#include "iostream"
	//using namespace std;
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
	size_t arrayLength;			//total length of array
	size_t freeLength;			//count of free bytes
	//size_t messageLength;
	//size_t lenLength;			//size_t type length in bytes
	//size_t crcLength;
protected:
	void allocate(size_t len) throw(Exception);		//allocate new array
	void addMemory(size_t len) throw (Exception);	//add to the existing array _len_ bytes
	BYTE *ptrToFreeArray();
	size_t busyArrayLength();
public:
	Array(void);
	Array(size_t len) throw(Exception);
	~Array(void);
	//void SetArray(BYTE *ar, size_t len) throw(...);		//create new array and copy data from *ar to it
	void AddArray(BYTE *ar, size_t len) throw(Exception);		//create as SetArray or add to existing array copy of *ar
	size_t GetArray(BYTE *ar);								//returns length of array and copyes array to ar
	size_t GetArrayLength();								//returns length of array
	virtual void Initialize() throw(Exception);				//first initializing of the object
	virtual void AddMem(size_t length) throw(Exception);	//adding memory in bytes
	bool IsArraySet();										//terurns true if array not null
};

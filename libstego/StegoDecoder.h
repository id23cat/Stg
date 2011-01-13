#pragma once
#include "StegoArray.h"
#include "LibInclude.h"
#include "stdio.h"

#ifdef _WIN32
//#ifdef LIBSTEGODLL_EXPORTS
//#define LIBSTEGODLL __declspec(dllexport)
//#else
//#define LIBSTEGODLL __declspec(dllimport)
//#endif
//#endif
//
//#ifdef _WIN32
//class LIBSTEGODLL StegoDecoder
//#else
class  StegoDecoder
	//!OutOfRangeException = false;
#endif
{
protected:
	StegoArray mesArray;
	//BitArray lenArray;
	BitArray::BitArrayIterator mit;
	//BitArray::BitArrayIterator lit;
	bool get_message;
	//bool get_length;
	//size_t mes_len;
	//int we;
public:
	StegoDecoder(void);
	virtual ~StegoDecoder(void);
	size_t GetMessageLength();
	size_t GetMessageP(BYTE *ptr);
	void SaveMessageToFile(char *mesFile) throw(...);
	void DecodeMessage(bool b);		// set the flag which couse to get message from the stream
	bool IsGetMessage(){return get_message;};
	//bool IsGetLength(){return get_length;};
public:
	//virtual int Decode(bool pasteMes=false)=0;
	virtual int Decode(char *infile, char *outfile, bool getMes=false)=0;
};

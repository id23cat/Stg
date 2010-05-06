#pragma once
#include "BitArray.h"
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
#endif
{
protected:
	BitArray mesArray;
	BitArray lenArray;
	BitArray::BitArrayIterator mit;
	BitArray::BitArrayIterator lit;
	bool get_message;
	bool get_length;
	size_t mes_len;
	int we;
public:
	StegoDecoder(void);
	~StegoDecoder(void);
	size_t GetMessageLength();
	size_t GetMessageP(BYTE *ptr);
	void SetMessageFile(char *mesFile) throw(...);
	void DecodeMessage(bool b);		// set the flag which couse to get message from the stream
	bool IsGetMessage(){return get_message;};
	bool IsGetLength(){return get_length;};
};

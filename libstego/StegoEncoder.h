/*
 * StegoEncoder.h
 *
 *  Created on: 21.01.2010
 *      Author: id23cat
 */

#ifndef STEGOENCODER_H_
#define STEGOENCODER_H_
#pragma once
#include "BitArray.h"
#include "LibInclude.h"

#include "iostream"
using namespace std;

#ifdef _WIN32
//#ifdef LIBSTEGODLL_EXPORTS
//#define LIBSTEGODLL __declspec(dllexport)
//#else
//#define LIBSTEGODLL __declspec(dllimport)
//#endif
//class LIBSTEGODLL StegoEncoder
//#else

class StegoEncoder
#endif
{
protected:
	BitArray mesArray;
	BitArray lenArray;
	BitArray::BitArrayIterator mit;
	BitArray::BitArrayIterator lit;
	bool paste_message;
	bool paste_length;
	char *infile;
	char *outfile;
public:
	StegoEncoder(void);
	~StegoEncoder(void);
	void SetMessage(BYTE *mes, size_t len) throw(...);
	//void SetMessageFile(wchar_t *mesFile) throw(...);
	void SetMessageFile(char *mesFile) throw(...);
	bool IsPasteMessage(){return paste_message;};			
	bool IsPasteLength(){return paste_length;};				
	void SetInFile(char *inf);							// set source media file for stego
	void SetOutFile(char *outf);						// set destination media file for stego
public:
	virtual int Encode(bool pasteMes=false)=0;
};

#endif /* STEGOENCODER_H_ */

/*
 * StegoEncoder.h
 *
 *  Created on: 21.01.2010
 *      Author: id23cat
 */

#ifndef STEGOENCODER_H_
#define STEGOENCODER_H_
#pragma once
#include "StegoArray.h"
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
#define PATH_LEN 512

class StegoEncoder
	//!OutOfRangeException = true;
#endif
{
	//FILE *outstream;
	FILE *errstream;	//stream for reopen stderr
	char **srcfiles;	//list of source files
	char **dstfiles;	//list of destination files
	int filecount;		//count of file to open
	char *dstdir;		//destination directory for result steganos
	
protected:
	StegoArray mesArray;
	BitArray::BitArrayIterator mit;
	
	bool paste_message;
	size_t capacityBit;				//capacity of container in bits;
	void genOutFileName(char *in, char *out, char *dir, int num=-1, int count=0);//
			//generates name for destination file
			//in--pointer to the input file name
			//out--pointer to string for destination file name
			//dir--destination directory
			//num--number of file in file list
			//count--total count of files

public:
	StegoEncoder(void);
	virtual ~StegoEncoder(void);
	void SetMessage(BYTE *mes, size_t len) throw(...);
	void SetMessageFile(char *mesFile) throw(...);
	bool IsPasteMessage(){return paste_message;};			
	//void AddContainer(char *file);
	void SetDstDir(char *dir);
public:
	virtual int Encode(char *infile, char *outfile, bool pasteMes=false)=0;
	//virtual int Encode(char **infiles, int count, char *dstdir=NULL, bool pasteMes=false)=0;
	virtual size_t Test(char *infile, bool wrtLog=false)=0;				//count capacity of container in bytes
};

#endif /* STEGOENCODER_H_ */

#pragma once
#if !defined(LSBSTEGOENCODER_H)
#define LSBSTEGOENCODER_H
#include "StegoEncoder.h"
#include <BMPimage.h>

class LSBStegoEncoder :
	public StegoEncoder
{
	BMP bmpSrc;
	//BMPimage bmpDst;
public:
	LSBStegoEncoder(void);
	~LSBStegoEncoder(void);

	void StegoHideMessage(BYTE *data, int len);	//callback function
	//void StegoHideLength(BYTE *data, int len);	//callback function

	void StegoHide(BYTE *vector, int len);
	int Encode(char *infile, char *outfile, bool pasteMes=false);
	//int Encode(bool pasteMes);
	size_t Test(char *infile,bool wrtLog=false);
};
#endif

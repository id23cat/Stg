#pragma once
#if !defined(LSBSTEGODECODER_H)
#define LSBSTEGODECODER_H
#include "StegoDecoder.h"
#include <BMPimage.h>

class LSBStegoDecoder :
	public StegoDecoder
{
	BMP bmpSrc;
public:
	LSBStegoDecoder(void);
	~LSBStegoDecoder(void);

	//void StegoGet(BYTE *vector, int len);
	//void StegoGetMessage(void *vb, BYTE *data, int len);	//callback function
	//void StegoGetLength(void *vb, BYTE *data, int len);	//callback function
	void StegoGet(void *vb, BYTE *data, int len);	//callback function
	int Decode(char *infile, char *outfile=NULL, bool getMes=true);
};
#endif

#pragma once
#include "StegoDecoder.h"

class LSBStegoDecoder :
	public StegoDecoder
{
public:
	LSBStegoDecoder(void);
	~LSBStegoDecoder(void);

	void StegoGetMessage(void *vb, BYTE *data, int len);	//callback function
	void StegoGetLength(void *vb, BYTE *data, int len);	//callback function
	void StegoGet(void *vb, BYTE *data, int len);	//callback function
};

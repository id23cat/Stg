#pragma once
#include "StegoEncoder.h"

class LSBStegoEncoder :
	public StegoEncoder
{
public:
	LSBStegoEncoder(void);
	~LSBStegoEncoder(void);

	void StegoHideMessage(void *vb, BYTE *vector, int len);	//callback function
	void StegoHideLength(void *vb, BYTE *vector, int len);	//callback function

	void StegoHide(void *vb, BYTE *vector, int len);
};

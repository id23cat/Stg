#pragma once
#include "StegoEncoder.h"
#include "BMPimage.h"

class LSBStegoEncoder :
	public StegoEncoder
{
	BMPimage bmpSrc;
	BMPimage bmpDst;
public:
	LSBStegoEncoder(void);
	~LSBStegoEncoder(void);

	void StegoHideMessage(BYTE *data, int len);	//callback function
	void StegoHideLength(BYTE *data, int len);	//callback function

	void StegoHide(BYTE *vector, int len);
	int Encode(bool pasteMes);
};

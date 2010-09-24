#pragma once
#include "StegoDecoder.h"
#include "constants.h"
extern "C" {	
	#include "..\jpeg-8a\jpeglib.h"
	#include "..\jpeg-8a\jpegtran.h"
	#include "..\jpeg-8a\cdjpeg.h"
}

//typedef struct JDPointerStruct
//{
//	//int notStego;		//boolean parameter, 0 if we mast call stego function, else NOT 0
//	void (*CallbackFunction)(j_decompress_ptr cinfo, JBLOCKROW *MCU_data);	//Stego function
//	void *stegoObjPtr;						//pointer to the stego ooject
//}JpegDCallbackData;

class JpegStegoDecoder : public StegoDecoder
	//!OutOfRangeException = false;
{
	StegoData sData;
	
private:
	void InitJpegStego(bool decodeMessage);
	int selectPosition(JCOEF *coef);				//select position in block of coefficients (MCU)
	int selectSign(JCOEF *coef, int position);		//select sign by current coefficients

public:
	//static void StegoGetMessage(j_decompress_ptr cinfo, JBLOCKROW *MCU_data);	//callback function	
	static void StegoGetMessage(void *cinfo, JBLOCKROW *MCU_data);	//callback function
public:
	JpegStegoDecoder(void);
	~JpegStegoDecoder(void);

	int Decode(char *infile, char *outfile=NULL, bool getMes=true);
};

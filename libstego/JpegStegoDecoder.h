#pragma once
#include "StegoDecoder.h"
extern "C" {	
	#include "..\jpeg-8a\jpeglib.h"
}

typedef struct JDPointerStruct
{
	int notStego;		//boolean parameter, 0 if we mast call stego function, else NOT 0
	void (*CallbackFunction)(j_decompress_ptr cinfo, JBLOCKROW *MCU_data);	//Stego function
	void *stegoObjPtr;						//pointer to the stego ooject
}JpegDCallbackData;

class JpegStegoDecoder :
	public StegoDecoder
{
public:
	static void StegoGetMessage(j_decompress_ptr cinfo, JBLOCKROW *MCU_data);	//callback function
	static void StegoGetLength(j_decompress_ptr cinfo, JBLOCKROW *MCU_data);		//callback function
public:
	JpegStegoDecoder(void);
	~JpegStegoDecoder(void);
};

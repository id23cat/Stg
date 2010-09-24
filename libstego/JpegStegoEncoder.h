#pragma once
#include "StegoEncoder.h"
#include "constants.h"
//#include <$(SolutionDir)\libbmp\BMPimage.h>
#include "..\libbmp\BMPimage.h"
//#include "BMPimage.h"
extern "C" {
	#include "..\jpeg-8a\jpeglib.h"
	#include "..\jpeg-8a\jpegtran.h"
	#include "..\jpeg-8a\cdjpeg.h"
}

#if !defined(JPEGSTEGOENCODER_H)
#define JPEGSTEGOENCODER_H

class JpegStegoEncoder :
	public StegoEncoder
{
	//!OutOfRangeException = true;
	StegoData sData;
	
	/*size_t width;
	size_t height;
	BYTE *rgb;*/
private:
	void InitJpegStego(bool encodeMessage);
	int selectPosition(JCOEF *coef);				//select position in block of coefficients (MCU)
	int selectSign(JCOEF *coef, int position);		//select sign by current coefficients
	//bool readBMP(char *fname);
	

protected:
	//static void StegoHideMessage(j_compress_ptr cinfo, JBLOCKROW *MCU_data);	//callback function
	//static void StegoTestContainer(j_compress_ptr cinfo, JBLOCKROW *MCU_data);	//callback function	

	static void StegoHideMessage(void *cinfo, JBLOCKROW *MCU_data);		//callback function
	static void StegoTestContainer(void *cinfo, JBLOCKROW *MCU_data);	//callback function	
public:
	JpegStegoEncoder(void);
	~JpegStegoEncoder(void);
public:
	int Encode(char *infile, char *outfile, bool pasteMes=false);
	//int Encode(char **infiles, int count, char *dstdir=NULL, bool pasteMes=false);
	size_t Test(char *infile);
	int startJpegToJpeg(char *inf, char *outf);
	int startBmpToJpeg(char *inf, char *outf);	
	void SetNextContainer(char *file){};
};
#endif //JPEGSTEGOENCODER_H
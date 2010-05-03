#include "StegoEncoder.h"
#include "constants.h"
//#include <$(SolutionDir)\libbmp\BMPimage.h>
//#include "..\libbmp\BMPimage.h"
#include "BMPimage.h"
extern "C" {
	#include "..\jpeg-8a\jpeglib.h"
}

#if !defined(JPEGSTEGOENCODER_H)
#define JPEGSTEGOENCODER_H

class JpegStegoEncoder :
	public StegoEncoder
{
	StegoCData cData;
	StegoDData dData;
	size_t width;
	size_t height;
	BYTE *rgb;
private:
	int selectPosition(JCOEF *coef);				//select position in block of coefficients (MCU)
	int selectSign(JCOEF *coef, int position);		//select sign by current coefficients
	bool readBMP(char *fname);

protected:
	//static void StegoHideMessage(void *vb, JBLOCKROW *MCU_data, int blocks_in_MCU);	//callback function
	//static void StegoHideLength(void *vb, JBLOCKROW *MCU_data, int blocks_in_MCU);	//callback function
	static void StegoHideMessage(j_compress_ptr cinfo, JBLOCKROW *MCU_data);	//callback function
	static void StegoHideLength(j_compress_ptr cinfo, JBLOCKROW *MCU_data);	//callback function
	static void CountLength(j_compress_ptr cinfo, JBLOCKROW *MCU_data);
public:
	JpegStegoEncoder(void);
	//JpegStegoEncoder(size_t dctS2);
	~JpegStegoEncoder(void);
public:
	int startHiding(char *inf, char *outf);
	int startJpegToJpeg(char *inf, char *outf);
	int startBmpToJpeg(char *inf, char *outf);
	size_t CountLength(char *inf);
};
#endif //JPEGSTEGOENCODER_H
#pragma once
#if !defined(JPEGSTEGOENCODER_H)
#define JPEGSTEGOENCODER_H
#include "StegoEncoder.h"
#include "JpegStegoLog.h"
//#include "constants.h"
#include "Methods.h"
//#include <$(SolutionDir)\libbmp\BMPimage.h>
#include "..\libbmp\BMPimage.h"
//#include "BMPimage.h"
extern "C" {
	#include "..\jpeg-8a\jpeglib.h"
	#include "..\jpeg-8a\jpegtran.h"
	#include "..\jpeg-8a\cdjpeg.h"
}

#define p0		0
#define P25		1
#define P50		2
#define P75		3
#define P100	4


class JpegStegoEncoder :
	public StegoEncoder
{
	//!OutOfRangeException = true;
protected:
	
	
	JpegStegoLog *slog1;
	JpegStegoLog *slog2;
	JStegoData sData;
	
	BYTE perc;				//percent parameter
	BYTE Pi;					//счетчик процентов
	
	
	
	/*size_t width;
	size_t height;
	BYTE *rgb;*/
//private:
	virtual void InitJpegStego(bool encodeMessage);
	void InitPercent();
	//int selectPosition(JCOEF *coef);				//select position in block of coefficients (MCU)
	//int selectSign(JCOEF *coef, int position);		//select sign by current coefficients
	//bool readBMP(char *fname);
	

protected:
	//static void StegoHideMessage(j_compress_ptr cinfo, JBLOCKROW *MCU_data);	//callback function
	//static void StegoTestContainer(j_compress_ptr cinfo, JBLOCKROW *MCU_data);	//callback function	

	static void StegoHideMessage(void *cinfo, JBLOCKROW *MCU_data);		//callback function
	static void StegoKochZhaoHide(void *cinfo, JBLOCKROW *MCU_data);	//callback function
	static void StegoKochZhaoTest(void *cinfo, JBLOCKROW *MCU_data);	//callback function
	static void StegoTestContainer(void *cinfo, JBLOCKROW *MCU_data);	//callback function	
	static void StegoTestContainer2(void* cinfo, int workspace[64],int blkn);		//callback function	
public:
	JpegStegoEncoder(void);
	~JpegStegoEncoder(void);
public:
	int Encode(char *infile, char *outfile, bool pasteMes=false);
	//int Encode(char **infiles, int count, char *dstdir=NULL, bool pasteMes=false);
	size_t Test(char *infile, bool wrtLog=false, char *outfile=NULL);
	int startJpegToJpeg(char *inf, char *outf);
	int startBmpToJpeg(char *inf, char *outf);	
	//void SetNextContainer(char *file){};
	void LOG(bool log, bool bin=false){blog = log; binary = bin;};
	void Koch(bool k=false){koch = k;};

public:
	bool blog;				//write text log
	char *logfile;			// file name
	bool binary;			//write binary log
	bool koch;				//write with Koch-Zhao algorithm
	bool random;			// write randomly
	int D;					//difference
	int percent;			//percent capacity
	BYTE quality;			//
	int work_component;		//(All,Y,Cb,Cr) компонент в который встраиваются данные
	int bit;				//{0,1} -- бит для тестирования Коха-Жао
};
#endif //JPEGSTEGOENCODER_H
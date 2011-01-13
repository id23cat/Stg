#pragma once
#if !defined(JPEGSTEGOANALYSIS_H)
#define JPEGSTEGOANALYSIS_H

#include "JpegStegoEncoder.h"
//#include "..\libbmp\BMPimage.h"
//#include "BMPimage.h"
//extern "C" {
//#include "..\jpeg-8a\jpeglib.h"
//#include "..\jpeg-8a\jpegtran.h"
//#include "..\jpeg-8a\cdjpeg.h"
//}



class JpegStegoAnalysis:public JpegStegoEncoder
{
	//StegoData sData;
	char *outfile;
	FILE *fout;

	void InitJpegStego(bool analyseMessage);
	static void StegoAnalyseContainer(void *cinfo, int *block,int blkn);	//callback function
public:
	JpegStegoAnalysis(void);
	~JpegStegoAnalysis(void);
	void SetExcelFile(char* file);
};

#endif //JPEGSTEGOANALYSIS
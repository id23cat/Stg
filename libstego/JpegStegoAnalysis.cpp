#include "StdAfx.h"
#include "JpegStegoAnalysis.h"

JpegStegoAnalysis::JpegStegoAnalysis(void)
{
}

JpegStegoAnalysis::~JpegStegoAnalysis(void)
{
}

void JpegStegoAnalysis::InitJpegStego(bool analyseMes)
{
	if(analyseMes)
	{
		sData.CallbackFunction2 = &StegoAnalyseContainer;
		sData.isStegoAnalysis = 1;
	}else
		sData.isStegoAnalysis = 0;

	sData.stegoObjPtr = this;

	/*dData.CallbackFunction = NULL;
	dData.notStego = 1;
	dData.stegoObjPtr = this;*/
}

void JpegStegoAnalysis::StegoAnalyseContainer(void *cinfo, int *block,int blkn)	//callback function
{
	j_compress_ptr *ccinfop = static_cast<j_compress_ptr*>(cinfo);
	j_compress_ptr ccinfo = *ccinfop;
	JpegStegoAnalysis *pJSE = static_cast<JpegStegoAnalysis*>(ccinfo->stego.stegoObjPtr);
}

void JpegStegoAnalysis::SetExcelFile(char* file)
{
	if( fopen_s( &fout, file, "w" ) != 0 )
	{
		char str[256]={0};
		sprintf(str,"Can not open file %s\n",file);
		throw Exception(str);		
	};
}
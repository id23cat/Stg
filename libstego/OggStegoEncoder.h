#pragma once
#include "StegoEncoder.h"

#include "iostream"
using namespace std;

extern "C"
{
#include <vorbis/vorbisenc.h>
//#include <vorbis/vorbisdec.h>
#include <vorbis/codec.h>
}

#ifdef _WIN32
//#ifdef LIBOGGSTEGODLL_EXPORTS
//#define LIBOGGSTEGODLL __declspec(dllexport)
//#else
//#define LIBOGGSTEGODLL __declspec(dllimport)
//#endif
//
//class LIBOGGSTEGODLL OggStegoEncoder: public StegoEncoder
//#else
class OggStegoEncoder: public StegoEncoder
#endif
{
#if _DEBUG
	FILE *cVec;
	FILE *cCoef;
	FILE *total;
	size_t totC;
#endif
public:
// 	static void StegoHideMessage(struct vorbis_block *vb, float *vector, int len);	//callback function
// 	static void StegoHideLength(struct vorbis_block *vb, float *vector, int len);	//callback function
	static void StegoHideMessage(void *vb, float *vector, int len);	//callback function
	static void StegoHideLength(void *vb, float *vector, int len);	//callback function
public:
	OggStegoEncoder(void);
	~OggStegoEncoder(void);

	//size_t Encode(FILE *instream, FILE *outstream, bool encMes=false);
	//size_t ReEncode(FILE *instream, FILE *outstream, bool encMes, OggStegoDecoder *dec);
	size_t ReEncode(FILE *instream, FILE *outstream, bool encMes);

#if _DEBUG
	void SetLogs(FILE *v, FILE *c, FILE *t);
	//static void StegoTest(struct vorbis_block *vb, float *vector, int len);	//callback function
	static void StegoTest(void *vb, float *vector, int len);	//callback function
	size_t GetTotal();
#endif
};

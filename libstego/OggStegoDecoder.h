#pragma once
#include "BitArray.h"
#include "stdio.h"
#include "StegoDecoder.h"

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
//#endif
//
//#ifdef _WIN32
//class LIBOGGSTEGODLL OggStegoDecoder: public StegoDecoder
//#else
class OggStegoDecoder: public StegoDecoder
#endif
{

public:
// 	static void StegoGetMessage(struct vorbis_block *vb, float *vector, int len);	//callback function
// 	static void StegoGetLength(struct vorbis_block *vb, float *vector, int len);	//callback function
	static void StegoGetMessage(void *vb, float *vector, int len);	//callback function
	static void StegoGetLength(void *vb, float *vector, int len);	//callback function
public:
	OggStegoDecoder(void);
	~OggStegoDecoder(void);
	//size_t Decode(FILE *instream, FILE *outstream, bool decMes=false);
};

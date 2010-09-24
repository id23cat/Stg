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

#ifdef _DEBUG
static size_t ticD=0;
#define LOWD (0x1F0+8)*8+3
#define HIGD (0x1F1+8)*8
#endif

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
	//!OutOfRangeException = false;
#endif
{
	void InitVorbisStego(vorbis_block *vb, bool decodeMessage);

public:
	static void StegoGetMessage(void *vb, float *vector, int len);	//callback function
public:
	OggStegoDecoder(void);
	~OggStegoDecoder(void);

	int Decode(char *infile, char *outfile=NULL, bool getMes=true);
	size_t startDecodeOgg(FILE *instream, FILE *outstream, bool getMes=true);
};

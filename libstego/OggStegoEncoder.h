#pragma once
#include "StegoEncoder.h"
#include "iostream"
extern "C"
{	
	//#include <..\libvorbis-1.2.3\include\vorbis/codec.h>
	#include <vorbis/codec.h>
	#include <vorbis/vorbisenc.h>
}
using namespace std;

#ifdef _DEBUG
static size_t ticE=0;
#define LOWE (0x1F0+8)*8+3
#define HIGE (0x1F1+8)*8
#endif

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
	//!OutOfRangeException = true;
#endif
{
	void InitVorbisStego(vorbis_block *vb, bool encodeMessage);		//Initialize stego encoder/decoder
public:
	static void StegoHideMessage(void *vb, float *vector, int len);	//callback function
	static void StegoTestContainer(void *vb, float *vector, int len);	//test container
public:
	OggStegoEncoder(void);
	~OggStegoEncoder(void);	

	int Encode(char *infile, char *outfile, bool pasteMes=false);
	int Encode(char **infiles, int count, char *dstdir=NULL, bool pasteMes=false){return 0;};
	size_t Test(char *infile);
	size_t startWavToOgg(FILE *instream, FILE *outstream, bool encMes=false);
	size_t startOggToOgg(FILE *instream, FILE *outstream, bool encMes=false);
};

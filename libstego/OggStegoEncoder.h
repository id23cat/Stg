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
	void InitVorbisStego(vorbis_block *vb, bool encMes);
public:
// 	static void StegoHideMessage(struct vorbis_block *vb, float *vector, int len);	//callback function
// 	static void StegoHideLength(struct vorbis_block *vb, float *vector, int len);	//callback function
	static void StegoHideMessage(void *vb, float *vector, int len);	//callback function
	static void StegoHideLength(void *vb, float *vector, int len);	//callback function
public:
	OggStegoEncoder(void);
	~OggStegoEncoder(void);

	size_t Encode(FILE *instream, FILE *outstream, bool encMes=false);
	size_t ReEncode(FILE *instream, FILE *outstream, bool encMes=false);
};

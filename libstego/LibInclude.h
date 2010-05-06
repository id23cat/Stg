#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <math.h>
#include <iostream>
using namespace std;

// extern "C"
// {
// #include <vorbis/vorbisenc.h>
// //#include <vorbis/vorbisdec.h>
// #include <vorbis/codec.h>
// }

#ifdef _WIN32 /* We need the following two to set stdin/stdout to binary */
#include <io.h>
#include <fcntl.h>
#endif

#if defined(__MACOS__) && defined(__MWERKS__)
#include <console.h>      /* CodeWarrior's Mac "command-line" support */
#endif

#define READ 1024

#pragma  once
typedef struct OPointerStruct
{
	int isStego;		//boolean parameter, 0 if we mast call stego function, else NOT 0
	void (*CallbackFunction)(void *vb, float *vector, int len);	//Stego function
	void *stegoObjPtr;						//pointer to the stego ooject
}OggCallbackData;



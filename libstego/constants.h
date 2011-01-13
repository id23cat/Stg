#pragma once
#if !defined(CONSTANTS_H)
#define CONSTANTS_H

extern "C" {
	#include "..\jpeg-8a\jpeglib.h"
}


//typedef enum {
//	JCOPYOPT_NONE,		/* copy no optional markers */
//	JCOPYOPT_COMMENTS,	/* copy only comment (COM) markers */
//	JCOPYOPT_ALL		/* copy all optional markers */
//} JCOPY_OPTION;



#define	Y	1
#define Cb	2
#define Cr	3
#define ALL	0

typedef char SBYTE;
//typedef BYTE SBYTE;

#define QT_PROJECT
#define FNAME_LEN	256

/*
 * jpeg_natural_order[i] is the natural-order position of the i'th element
 * of zigzag order.
 *
 * When reading corrupted data, the Huffman decoders could attempt
 * to reference an entry beyond the end of this array (if the decoded
 * zero run length reaches past the end of the block).  To prevent
 * wild stores without adding an inner-loop test, we put some extra
 * "63"s after the real entries.  This will cause the extra coefficient
 * to be stored in location 63 of the block, not somewhere random.
 * The worst case would be a run-length of 15, which means we need 16
 * fake entries.
 */
const int jpeg_natural_order[DCTSIZE2+16] = {
  0,  1,  8, 16,  9,  2,  3, 10,
 17, 24, 32, 25, 18, 11,  4,  5,
 12, 19, 26, 33, 40, 48, 41, 34,
 27, 20, 13,  6,  7, 14, 21, 28,
 35, 42, 49, 56, 57, 50, 43, 36,
 29, 22, 15, 23, 30, 37, 44, 51,
 58, 59, 52, 45, 38, 31, 39, 46,
 53, 60, 61, 54, 47, 55, 62, 63,
 63, 63, 63, 63, 63, 63, 63, 63, /* extra entries for safety in decoder */
 63, 63, 63, 63, 63, 63, 63, 63
};

/*
 * jpeg_zigzag_order[i] is the zigzag-order position of the i'th element
 * of a DCT block read in natural order (left to right, top to bottom).
 */
const int jpeg_zigzag_order[DCTSIZE2] = {
   0,  1,  5,  6, 14, 15, 27, 28,
   2,  4,  7, 13, 16, 26, 29, 42,
   3,  8, 12, 17, 25, 30, 41, 43,
   9, 11, 18, 24, 31, 40, 44, 53,
  10, 19, 23, 32, 39, 45, 52, 54,
  20, 22, 33, 38, 46, 51, 55, 60,
  21, 34, 37, 47, 50, 56, 59, 61,
  35, 36, 48, 49, 57, 58, 62, 63
};

#define ALLOW 16
#define  nt(i) jpeg_natural_order[i]
#define  zt(i) jpeg_zigzag_order[i]

// Допустимые коэффициенты для встраивания
const int allowable_position[ALLOW] = {
	nt(4),	nt(7),	nt(8),	nt(11),
	nt(12),	nt(13),	nt(16),	nt(17),
	nt(18),	nt(19),	nt(23),	nt(24),
	nt(25),	nt(31),	nt(32),	nt(39)
};

// Допустимые позиции для Коха-Жао
#define ALLOW_KOCH 18
struct LKoch
{
	int l1, l2,l3;
};

const int KochZhaoPositions[ALLOW_KOCH][3] = {
	{zt(2), zt(9), zt(10)},
	{zt(9), zt(2), zt(10)},
	{zt(3), zt(10), zt(11)},
	{zt(10), zt(3), zt(11)},
	{zt(9), zt(2), zt(10)},
	{zt(2), zt(9), zt(10)},
	{zt(9), zt(16), zt(2)},
	{zt(16), zt(9), zt(2)},
	{zt(2), zt(9), zt(16)},
	{zt(9), zt(2), zt(16)},
	{zt(10), zt(17), zt(3)},
	{zt(17), zt(10), zt(3)},
	{zt(10), zt(3), zt(17)},
	{zt(3), zt(10), zt(17)},
	{zt(9), zt(16), zt(17)},
	{zt(16), zt(9), zt(17)},
	{zt(10), zt(17), zt(18)},
	{zt(17), zt(10), zt(18)}
};



typedef union BYTE_B
{
	BYTE byte;
	struct BIT
	{
		unsigned _7: 1;
		unsigned _6: 1;
		unsigned _5: 1;
		unsigned _4: 1;
		unsigned _3: 1;
		unsigned _2: 1;
		unsigned _1: 1;
		unsigned _0: 1;
	} bit;
}BYTE_b;

//#define LOGFILEC "Cstatistic.log"
//#define LOGFILED "Dstatistic.log"

//typedef struct JEPointerStruct
//{
//	int notStego;		//boolean parameter, 0 if we mast call stego function, else NOT 0
//	void (*CallbackFunction)(j_compress_ptr cinfo, JBLOCKROW *MCU_data);	//Stego function
//	void *stegoObjPtr;						//pointer to the stego oject
//}JpegECallbackData;
//
//typedef struct JDPointerStruct
//{
//	int notStego;		//boolean parameter, 0 if we mast call stego function, else NOT 0
//	void (*CallbackFunction)(j_decompress_ptr cinfo, JBLOCKROW *MCU_data);	//Stego function
//	void *stegoObjPtr;						//pointer to the stego oject
//}JpegDCallbackData;

#endif	//!defined(CONSTANTS_H)
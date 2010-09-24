#pragma once
#include "Exception.h"
#include "EasyBMP.h"
//#include <fstream>
//#include <stdexcept>
using namespace std;

#if !defined(BMPIMAGE_H)
#define BMPIMAGE_H

typedef unsigned char BYTE;
typedef unsigned short WORD;
typedef unsigned long DWORD;
typedef long LONG;



class BMPimage: protected BMP
{
	//typedef struct tagBITMAPFILEHEADER1 {
	//		WORD    bfType;
	//		DWORD   bfSize;
	//		WORD    bfReserved1;
	//		WORD    bfReserved2;
	//		DWORD   bfOffBits;
	//} BITMAPFILEHEADER;

	//typedef struct tagBITMAPINFOHEADER{
	//		DWORD      biSize;
	//		LONG       biWidth;
	//		LONG       biHeight;
	//		WORD       biPlanes;
	//		WORD       biBitCount;
	//		DWORD      biCompression;
	//		DWORD      biSizeImage;
	//		LONG       biXPelsPerMeter;
	//		LONG       biYPelsPerMeter;
	//		DWORD      biClrUsed;
	//		DWORD      biClrImportant;
	//} BITMAPINFOHEADER;

	//typedef struct tagRGBTRIPLE
	//{
	//	BYTE    rgbBlue;
	//	BYTE    rgbGreen;
	//	BYTE    rgbRed;	
	//} RGBTRIPLE;

	//struct DESCR
	//{
	//	char *fname;
	//	BITMAPFILEHEADER filHead;
	//	BITMAPINFOHEADER infHead;
	//	int width;
	//	int height;
	//	BYTE *GRAY;
	//	BYTE *R;
	//	BYTE *G;
	//	BYTE *B;
	//	BYTE type;
	//};

	//BITMAPINFOHEADER infHead;
	//BITMAPFILEHEADER filHead;

	BYTE *rgb;
//	//BYTE *B;
//	//BYTE *G;
//	//BYTE *R;
//	size_t rgblen;
//	size_t width, height;
//	//void *pSObj;
//private:
//	int ReadHeader(fstream &in)throw(...);
//	int ReadData(fstream &in)throw(...);
//	int WriteHeader(fstream &out)throw(...);
//	int writeData(fstream &out)throw(...);
	void TopToBottom();
	void BGRtoRGB(BYTE *bgr);
public:
	BMPimage(void);
	~BMPimage(void);
public:
	int ReadRGB(char *fname)throw(...);		//reading RGB from  BMP image specified in string fname
	BYTE * GetRGBptr(size_t &height, size_t &width);	//get pointer to RGB
	void ObjPtr(void *prt);
	//int ReadBMPheader(char *fname, size_t &h, size_t &w)throw(...);	// reading header of BMP file specified in fname
	int WriteRGB(char *fname)throw(...);		//save RGB to specified file, width & height mast have actual values
	//int ReWriteRGB(char *fname)throw(...);	//rewrite BMP file with using callback stego function
};

#endif //BMPIMAGE_H
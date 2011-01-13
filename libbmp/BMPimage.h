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



class BMPimage: public BMP
{


	BYTE *rgb;

	void TopToBottom();
	void BGRtoRGB(BYTE *bgr);
public:
	BMPimage(void);
	~BMPimage(void);
public:
	//RGBApixel* operator()(int i,int j){return BMP::operator ()(i,j);};
	int ReadRGB(char *fname)throw(...);		//reading RGB from  BMP image specified in string fname
	BYTE * GetRGBptr(size_t &height, size_t &width);	//get pointer to RGB
	void ObjPtr(void *prt);
	//int ReadBMPheader(char *fname, size_t &h, size_t &w)throw(...);	// reading header of BMP file specified in fname
	int WriteRGB(char *fname)throw(...);		//save RGB to specified file, width & height mast have actual values
	//int ReWriteRGB(char *fname)throw(...);	//rewrite BMP file with using callback stego function
};

#endif //BMPIMAGE_H
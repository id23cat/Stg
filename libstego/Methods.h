#pragma once
#if !defined(METHODS_H)
#define METHODS_H

#include "constants.h"
//#include "JpegStegoEncoder.h"
//extern "C" {
//#include "..\jpeg-8a\jmorecfg.h"
////#include "..\jpeg-8a\jpeglib.h"
////#include "..\jpeg-8a\jpegtran.h"
////#include "..\jpeg-8a\cdjpeg.h"
//}

//void DemiHide(j_compress_ptr ccinfo, JpegStegoEncoder *pJSE, JBLOCKROW *MCU_data);
int selectPosition(JCOEF *coef);
int selectSign(JCOEF *coef, int position);

LKoch KochZhaoPosition(int pos);

#endif //METHODS_H
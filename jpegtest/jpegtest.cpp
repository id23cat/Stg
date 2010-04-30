// jpegtest.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "JpegStegoEncoder.h"



int _tmain(int argc, _TCHAR* argv[])
{
	JpegStegoEncoder jse;
	//jse.startHiding("1.jpg","11.jpg");
	jse.startHiding("1.bmp","11.jpg");
	
	return 0;
}


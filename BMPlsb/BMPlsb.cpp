// BMPlsb.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "conio.h"
#include "LSBStegoDecoder.h"
#include "LSBStegoEncoder.h"
#include <BMPimage.h>
#include<iostream>
using namespace std;


int _tmain(int argc, _TCHAR* argv[])
{
	try{
	LSBStegoDecoder de;
	LSBStegoEncoder en;
	BMPimage bmp;
	size_t height=0;
	size_t width=0;

	bmp.ReadRGB("1.bmp");
	BYTE *rgb = bmp.GetRGBptr(height,width);
	if(!height ||!width)
		cerr << "failed to open BMP";

	BYTE ar[64];
	memset(ar,0,64);
	BYTE stg[]={1,2,3,4};
	for(int i=0;i<4;i++)
		cout <<(int) stg[i]<<' ';
	cout << endl;
	en.SetMessage(stg,4);
	en.StegoHide(0,ar,64);
	de.StegoGet(0,ar,64);
	de.GetMessageP(stg);
	for(int i=0;i<4;i++)
		cout << (int)stg[i]<<' ';
	cout << endl;
	}catch(Exception exc)
	{
		cout << exc.getMessage();
	}
	getch();
	return 0;
}


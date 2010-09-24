// BMPlsb.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "conio.h"
#include "LSBStegoDecoder.h"
#include "LSBStegoEncoder.h"
#include <BMPimage.h>
#include <iostream>
#include "StegoArray.h"
using namespace std;




int _tmain(int argc, _TCHAR* argv[])
{
	/*try{
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
	en.StegoHide(ar,64);
	de.StegoGet(0,ar,64);
	de.GetMessageP(stg);
	for(int i=0;i<4;i++)
		cout << (int)stg[i]<<' ';
	cout << endl;
	}catch(Exception exc)
	{
		cout << exc.getMessage();
	}
	getch();*/

	//StegoArray sar1;
	//sar1.SetMessage((BYTE*)"asdf",4);
	////sar1.DEBUG_Print_Array();
	////sar1.DEBUG_Print_Bits();
	//StegoArray sar2;
	//StegoArray sar3;

	//BitArray::BitArrayIterator it1=sar1.Begin();
	//BitArray::BitArrayIterator it2=sar2.Begin();
	//BitArray::BitArrayIterator it3=sar3.Begin();


	//BYTE b=0;
	//try{
	//	cerr << "1st test:"<<endl;
	//	it2.SetOutOfRangeExceptionStatus(false);
	//	try{
	//		while(1)
	//		{
	//			b=it1;
	//			it2=b;
	//			
	//			it1++; 
	//			it2++;
	//		}
	//	}catch(OutOfRangeException exc)
	//	{
	//		cerr << exc.getMessage()<<endl;
	//		cerr << "2: ";
	//		//sar2.DEBUG_Print_Array();
	//		//sar2.DEBUG_Print_Bits();

	//	}catch(EndOfMessageException exc)
	//	{
	//		cerr << exc.getMessage()<<endl;
	//		cerr << "2: ";
	//		//sar2.DEBUG_Print_Array();
	//		//sar2.DEBUG_Print_Bits();
	//	}

	//	it2.SetOutOfRangeExceptionStatus(true);
	//	it3.SetOutOfRangeExceptionStatus(false);
	//	it2 = sar2.Begin();
	//	cerr <<"2nd test:" << endl;
	//	try{
	//		for(int i=64;i>=0;i--)
	//		{	it3=1; it3++;	}

	//		while(1)
	//		{
	//			b=it2;
	//			it3=b;
	//			//cerr<<" ";
	//			it3++;
	//			it2++;
	//		}
	//	}catch(OutOfRangeException exc)
	//	{
	//		cerr << exc.getMessage()<<endl;		
	//		cerr << "3: ";
	//		//sar3.DEBUG_Print_Array();
	//		//sar3.DEBUG_Print_Bits();
	//	}catch(EndOfMessageException exc)
	//	{
	//		cerr << exc.getMessage()<<endl;
	//		cerr << "3: ";
	//		//sar3.DEBUG_Print_Array();
	//		//sar3.DEBUG_Print_Bits();
	//	}



	//BYTE *str;
	//size_t len;
	//str = sar2.GetMessage(len);
	//system("pause");
	//}catch(Exception ex)
	//{
	//	cout << ex.getMessage()<<endl;
	//}
	//catch(DamagedMessageException exc)
	//{
	//	cout << exc.getMessage()<<endl;
	//}


	//for(int i=0; i<1000; i++)
	//	cout << (BYTE) 0xFF;

	BMPimage bim;
	//bim.


	system("pause");
	


	return 0;
}


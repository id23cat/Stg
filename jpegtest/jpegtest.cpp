// jpegtest.cpp : Defines the entry point for the console application.
//
#include "stdafx.h"
#include "JpegStegoEncoder.h"
#include "JpegStegoDecoder.h"

int main(int argc, char* argv[])
{
	try
	{
		JpegStegoEncoder jse;
		cerr << "Encoding..."<<endl;
		jse.SetMessageFile(argv[3]);
		cerr<<jse.Test(argv[1])<<" bytes"<<endl;
		jse.Encode(argv[1],argv[2],true);
		//jse.Encode(&argv[1],1,".",true); 
		cerr << "\nDone\n";
		JpegStegoDecoder jsd;
		cerr << "Decoding..."<<endl;
		jsd.Decode(argv[2]);
		jsd.SaveMessageToFile(argv[4]);
		cerr << "\ndone";
	}catch(Exception exc)
	{
		cerr << exc.getMessage();
	}
	cerr << "all done\n";
	system("pause");
	
	return 0;
}


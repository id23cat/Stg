// jpegtest.cpp : Defines the entry point for the console application.
//
#include "stdafx.h"
#include "JpegStegoEncoder.h"
#include "JpegStegoDecoder.h"

#define ENC 0x80	//10000000b
#define TST 0x40	//01000000b
#define LOG 0x20	//00100000b


BYTE ParseArgs(int argc, char* argv[]);
void PrintUsage();

int main(int argc, char* argv[])
{
	try
	{
		JpegStegoEncoder jse;
		cerr << "Encoding..."<<endl;
		jse.SetMessageFile(argv[5]);
		cerr<<(jse.Test(argv[1]) /*+ jse.Test(argv[3])*/)<<" bytes"<<endl;
		jse.Encode(argv[1],argv[2],true);
		//jse.Encode(argv[3],argv[4],true);
		//jse.Encode(&argv[1],1,".",true); 
		cerr << "\nDone\n";
		JpegStegoDecoder jsd;
		cerr << "Decoding..."<<endl;
		jsd.Decode(argv[2]);
		//jsd.Decode(argv[4]);
		jsd.SaveMessageToFile(argv[6]);
		cerr << "\ndone";
	}catch(Exception exc)
	{
		cerr << exc.getMessage();
	}
	cerr << "all done\n";
	system("pause");
	
	return 0;
}

// return parameter (BYTE):
// [|encode(e)(d)|test(t)|log(l)|...]

BYTE ParseArgs(int argc, char* argv[])
{
	BYTE retargs = 0;
	for(int i=0; i<argc; i++)
	{
		if(argv[i][0]=='-')
		{
			for(int j=1; j<strlen(argv[i]); j++)
			{
				switch(argv[i][j])
				{
				case 'e':
					retargs = retargs|ENC;
				case 'd':
					retargs = retargs|(~ENC);
				case 't':
					retargs = retargs|TST;
				case 'l':
					retargs = retargs|LOG;
				default:
					PrintUsage();
					exit(0);
				}				
			}
			return retargs;
		}
	}
	PrintUsage();
	exit(0);
}

void PrintUsage()
{
	cerr << "Usage: jpegtest -[edtl] input_file.[jpg|bmp] [output_file.jpg] [message_file]"<<endl;
	cerr << "Supported flags:"<<endl;
	cerr << " e		Encoding"<<endl;
	cerr << " d		Decoding"<<endl;
	cerr << " t		Testing capacity"<<endl;
	cerr << " l		Write coefficients to log file"<<endl;
	return;
}
// jpegtest.cpp : Defines the entry point for the console application.
//
#include "stdafx.h"
#include "JpegStegoEncoder.h"
#include "JpegStegoDecoder.h"

#define ENC 0x80	//10000000b
#define TST 0x40	//01000000b
#define LOG 0x20	//00100000b

struct ARGS
{
	bool enc;
	bool dec;
	bool tst;
	bool log;
	bool bin;
	bool koch;
	BYTE qual;

};


ARGS ParseArgs(int argc, char* argv[]);
void PrintUsage();


//	Usage: jpegtest -[{edtlbk}l] input_file.[jpg|bmp] [output_file.jpg] [message_file] [difference(1-20)]

int main(int argc, char* argv[])
{
	ARGS args;
	cout << argc <<" ";
	args = ParseArgs(argc,argv);
	try
	{
		//if((args & ENC) != 0)		//then encoding
		if(args.enc)						//then encoding
		{
			JpegStegoEncoder jse;
			cout << "Encoding..."<<endl;
			cout << argv[2] <<" to "<<argv[3];
			jse.koch = args.koch;
			jse.blog = args.log;
			jse.binary = args.bin;
			if(argc>=6)
				jse.D = atoi(argv[5]);
			if(argc>=7)
				jse.percent = atoi(argv[6]);
			if(args.qual)
				jse.quality = args.qual;
			if(argc>=5)
			{
				cout <<" with "<<argv[4]<<endl;
				jse.SetMessageFile(argv[4]);		
			}
			jse.Encode(argv[2],argv[3],true);			
			cout << "\nDone\n";			
			if(args.dec)
			{
				JpegStegoDecoder jsd;				
				cout << "Decoding..."<<endl;
				cout << "from "<< argv[3]<<" to "<<argv[4]<<endl;
				jsd.Decode(argv[3]);
				//jsd.Decode(argv[4]);
				jsd.SaveMessageToFile(argv[4]);
				cout << "\ndone";		
			}

		//}else if((~args & ENC) != 0)		//then decoding
		}else if(args.dec)	//then decoding
		{
			JpegStegoDecoder jsd;
			cout << "Decoding..."<<endl;
			cout << "from "<< argv[3]<<" to "<<argv[3]<<endl;
			jsd.Decode(argv[2]);
			//jsd.Decode(argv[4]);
			jsd.SaveMessageToFile(argv[3]);
			cout << "\ndone";		
		//}else if((args & TST) != 0)		//then testing
		}else if(args.tst)
		{
			JpegStegoEncoder jse;
			cout << "Testing..."<<endl;
			cout << argv[2]<<endl;
			jse.koch = args.koch;
			jse.blog = args.log;
			jse.binary = args.bin;
			cout <<(jse.Test(argv[2]) /*+ jse.Test(argv[3])*/)<<" bytes"<<endl;
			cout << "\nDone\n";			
		}		
	}catch(Exception exc)
	{
		cout << exc.getMessage();
		system("pause");
	}
	//cout << "all done\n";
	//system("pause");
	
	return 0;
}

// return parameter (BYTE):
// [|encode(e)(d)|test(t)|log(l)|...]

ARGS ParseArgs(int argc, char* argv[])
{
	ARGS retargs;
	memset(&retargs,0,sizeof(ARGS));
	for(int i=0; i<argc; i++)
	{
		if(argv[i][0]=='-')
		{
			for(int j=1; j<strlen(argv[i]); j++)
			{
				switch(argv[i][j])
				{
				case 'e':
					//retargs = retargs|ENC;
					retargs.enc=true;
					if(argc <4)
					{
						PrintUsage();
						exit(0);
					}
					break;
				case 'd':
					//retargs = retargs &(~ENC);
					retargs.dec = true;
					if(argc <4)
					{
						PrintUsage();
						exit(0);
					}
					break;
				case 't':
					//retargs = retargs|TST;
					retargs.tst = true;
					if(argc <3)
					{
						PrintUsage();
						exit(0);
					}
					break;
				case 'l':
					//retargs = retargs|LOG;
					retargs.log = true;
					break;
				case 'b':
					//retargs = retargs|LOG;
					retargs.bin = true;
					break;
				case 'k':
					//retargs = retargs|LOG;
					retargs.koch = true;
					break;
				case 'q':
					//retargs = retargs|LOG;
					retargs.qual = atoi(argv[i+1]);
					break;
				//case 'p':
				//	//retargs = retargs|LOG;
				//	retargs.percent = true;
				//	break;
				default:
					PrintUsage();
					exit(0);
				}				
			}
			
		}
	}
	return retargs;
	/*PrintUsage();
	exit(0);*/
}

void PrintUsage()
{
	cout << "Usage: jpegtest -[edtlbkp] input_file.[jpg|bmp] [output_file.jpg] [message_file] [difference(1-20)] "
		<<"[% bloks to write{25,50,75,100}] [-q "<<endl;
	cout << "Supported flags:"<<endl;
	cout << " e		Encoding"<<endl;
	cout << " d		Decoding"<<endl;
	cout << " t		Testing capacity"<<endl;
	cout << " l		Write coefficients to log file"<<endl;
	cout << " b		Write coefficients to log file in binary mode"<<endl;
	cout << " k		Koch-Zhao encoding/decoding"<<endl;
	cout << "-q		N[,...]   Compression quality (0..100; 5-95 is useful range)\n";
	/*cout << " p		Capcity percent"<<endl;*/
	return;
}
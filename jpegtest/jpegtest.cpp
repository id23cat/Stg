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
	bool enc;		// encoding
	bool dec;		// decoding
	bool tst;		// test
	char *in;	// input file
	char *out;	// output file
	char *mes;	// message file
	bool log;		// write log in text mode
	char *logf;	// log file
	bool bin;		// write log in binary mode
	BYTE D;			// threshold
	BYTE perc;		// percentage of completion
	BYTE qual;		// quality of JPEG compressing
	bool koch;		// write with Koch-Zhao algorithm
	bool rnd;		// randomly
	bool y;			// Y component
	bool cb;			// Cb component
	bool cr;			// Cr component

};


ARGS ParseArgs(int argc, char* argv[]);
void PrintUsage();


//	Usage: jpegtest -[{edtlbk}l] -{e|d|t} input_file.[jpg|bmp] -o [output_file.jpg] -m [message_file] [difference(1-20)]

int main(int argc, char* argv[])
{
	ARGS args;
	//cout << argc <<" ";
	args = ParseArgs(argc,argv);
	try
	{
		//if((args & ENC) != 0)		//then encoding
		if(args.enc)						//then encoding
		{			
			JpegStegoEncoder jse;
			jse.koch = args.koch;			
			jse.D = args.D;
			if(args.perc)
				jse.percent = args.perc;
			if(args.qual)				// compress with specified quality
				jse.quality = args.qual;
			if(args.y)
				jse.work_component=Y;
			else if(args.cb)
				jse.work_component=Cb;
			else if(args.cr)
				jse.work_component=Cr;
			else
				jse.work_component=ALL;	
			jse.binary = args.bin;
			jse.logfile = args.logf;
			jse.random = args.rnd;

			// if testing...
			int cap0=0;		// capacity for bit '0'
			int cap1=0;		// capacity for bit '1'
			if(args.tst)
			{
				/*if(args.log)
				{
					cout << jse.D << ' '<<jse.percent<<endl;
					jse.bit=0;
					cap0 = jse.Test(args.in,true,args.logf);
					cout <<"0: "<< cap0 <<" bytes"<<endl;
					if(jse.koch)
					{
						jse.bit=1;
						cap1 = jse.Test(args.in,true,args.logf);
						cout <<"1: "<< cap1 <<" bytes"<<endl;
					}				
				}
				else*/{				
					cout << '\n'<<jse.D << ' '<<jse.percent<<endl;
					jse.bit=0;
					cap0 = jse.Test(args.in,true);
					cout <<"0: "<< cap0 <<" bytes"<<endl;
					if(jse.koch)
					{
						jse.bit=1;
						cap1 = jse.Test(args.in,true);
						cout <<"1: "<< cap1 <<" bytes"<<endl;
					}				
				}
			}

			jse.blog = args.log;
			//cout << "Encoding..."<<endl;
			cout << args.in <<" to "<<args.out;
						
			if(args.mes)
			{
				//cout <<" with "<<args.mes<<endl;
				if(!cap0 || !cap1 || args.rnd )
					jse.SetMessageFile(args.mes);		
				else
				{
					size_t len = (size_t)((cap0+cap1)/2/100)*jse.percent;
					jse.SetMessageFile(args.mes, len);

				}
			}
			jse.Encode(args.in,args.out,true);	
			//cout << "\nDone\n";
			if(args.dec)
			{
				JpegStegoDecoder jsd;				
				cout << "Decoding..."<<endl;
				cout << "from "<< args.in<<" to "<<args.mes<<endl;
				jsd.Decode(args.in);
				jsd.SaveMessageToFile(args.mes);
				cout << "\ndone";	
			}
					
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
			//cout << "Testing..."<<endl;
			//cout << argv[2]<<endl;
			jse.koch = args.koch;
			jse.blog = args.log;
			jse.binary = args.bin;
			jse.D = args.D;
			jse.random = args.rnd;
			if(args.perc)
				jse.percent = args.perc;
			if(args.qual)				// compress with specified quality
				jse.quality = args.qual;
			if(args.y)
				jse.work_component=Y;
			else if(args.cb)
				jse.work_component=Cb;
			else if(args.cr)
				jse.work_component=Cr;
			else
				jse.work_component=ALL;

			if(args.log)
			{
				cout << jse.D << ' '<<jse.percent<<endl;
				jse.bit=0;
				cout <<"0: "<<(jse.Test(args.in,true,args.logf) /*+ jse.Test(argv[3])*/)<<" bytes"<<endl;
				if(jse.koch)
				{
					jse.bit=1;
					cout <<"1: "<<(jse.Test(args.in,true,args.logf) /*+ jse.Test(argv[3])*/)<<" bytes"<<endl;
				}				
			}
			else{				
				cout << jse.D << ' '<<jse.percent<<endl;
				jse.bit=0;
				cout <<"0: "<<(jse.Test(args.in,true) /*+ jse.Test(argv[3])*/)<<" bytes"<<endl;
				if(jse.koch)
				{
					jse.bit=1;
					cout <<"1: "<<(jse.Test(args.in,true) /*+ jse.Test(argv[3])*/)<<" bytes"<<endl;
				}				
			}
			//cout << "\nDone\n";			
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
	int r=0;
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
					retargs.enc = true;
					retargs.in = argv[i+1];
					/*if(argc <4)
					{
						PrintUsage();
						exit(0);
					}*/
					break;
				case 'd':
					retargs.dec = true;
					retargs.in = argv[i+1];
					/*if(argc <4)
					{
						PrintUsage();
						exit(0);
					}*/
					break;
				case 't':
					retargs.tst = true;
					retargs.in = argv[i+1];
					/*if(argc <3)
					{
						PrintUsage();
						exit(0);
					}*/
					break;
				case 'o':
					retargs.out = argv[i+1];
					break;
				case 'm':
					retargs.mes = argv[i+1];					
					break;
				case 'l':
					retargs.log = true;
					retargs.logf = argv[i+1];
					break;
				case 'b':					
					retargs.bin = true;
					break;
				case 'D':					
					retargs.D = atoi(argv[i+1]);
					break;
				case 'p':					
					retargs.perc = atoi(argv[i+1]);
					break;
				case 'q':					
					retargs.qual = atoi(argv[i+1]);
					break;
				case 'k':					
					retargs.koch = true;
					break;
				case 'r':
					r = atoi(argv[i+1]);
					if(r)
						retargs.rnd = true;
					else
						retargs.rnd = false;
					break;
				case 'Y':
					retargs.y=true;
					break;
				case 'B':
					retargs.cb=true;
					break;
				case 'R':
					retargs.cr=true;
					break;
				case 'A':					
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
	cout << "Usage: jpegtest.exe -[kYBRA] -{e|d|t} input_file.{jpg|bmp} [-o output_file.jpg] "<<
		"[-m message_file.*] [-lb log_file.log] [-D threshold(1..20)] "<<
		"[-p percentage_of_completion{25,50,75,100}] [-L length] [-r {0|1}] [-q quality] "<<endl;
	cout << "Supported flags:"<<endl;
	cout << " -e		Encoding file input_file.{jpg|bmp}"<<endl;
	cout << " -d		Decoding input_file.jpg"<<endl;	
	cout << " -t		Testing capacity of input_file.{jpg|bmp}"<<endl;
	cout << " -o		Write result to output_file.jpg"<<endl;
	cout << " -m		Read/Write message from/to message_file.*"<<endl;
	cout << " -l		Write coefficients to log file"<<endl;
	cout << " -b		Write coefficients to log file in binary mode"<<endl;
	cout << " -D		threshold from 1 to 20"<<endl;
	cout << " -p		percentage_of_completion{25,50,75,90,100}"<<endl;
	cout << " -L		length in bytes that will be written from message file"<<endl;
	cout << " -q		N[...]   Compression quality (0..100; 5-95 is useful range)\n";
	cout << " -k		Koch-Zhao encoding/decoding"<<endl;
	cout << " -r		uniform writing to container (equals to random distribution)'1', if -r not set or '0' -- consistently by default"<<endl;
	cout << " -Y		Write in Y component"<<endl;
	cout << " -B		Write in Cb component"<<endl;
	cout << " -R		Write in Cr component"<<endl;
	cout << " -A		Write in ALL components (default)"<<endl;
	
	cout << "Examples:"<<endl;
	cout << "\tjpegtest.exe -e image.bmp image.jpg -- just compress"<<endl;
	cout << "\tjpegtest.exe -tklb image.bmp -- testing with binary loging for Koch-Zhao algorithm"<<endl;
	cout << "\tjpegtest.exe -tk image.bmp 1 -- testing for Koch-Zhao algorithm with  differnce factor is equal to 1"<<endl;
	cout << "\tjpegtest.exe -tklb image.bmp image.log 2 -- testing with binary loging to file image.log for Koch-Zhao algorithm, differnce factor is equal to 2"<<endl;
	cout << "\tjpegtest.exe -ekbl image.bmp image.jpg mes.txt 5 100 -- compress whith writing message from mes.txt whith Koch-Zhao and 100% filing, differnce factor is equal to 5"<<endl;
	
	/*cout << " p		Capcity percent"<<endl;*/
	return;
}
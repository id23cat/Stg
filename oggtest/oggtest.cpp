// oggtest.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "OggStegoEncoder.h"
#include "OggStegoDecoder.h"
#include <stdio.h>
#include <conio.h>



//int _tmain(int argc, _TCHAR* argv[])
//	0	   1	2			3			4
//encoding:
//oggtest -e <infile> [outfile] [messgae file]
//decoding:
//oggtest -d <infile> [message file]
//testing:
//oggtest -t <infile>
void Using()
{
	cerr << "\nДля встраивания сообщения:\n";
	cerr << "oggtest -e <input_file_(.ogg_or_.wav)> [output_file_(.ogg)] [messgae_file]\n";
	cerr << "Для извлечения сообщения\n";
	cerr << "oggtest -d <input_file_(.ogg)> [message_file]\n";
	cerr << "Для тестирования контейнера:\n";
	cerr << "oggtest -t <input_file_(.ogg_or_.wav)>";
}

int main(int argc, char* argv[])
{
	setlocale ( LC_ALL, "Russian" );
	if(argc<3)
	{
		Using();
		return 1;
	}
	try{
		switch(argv[1][1])
		{
		case 'e':
			{
				OggStegoEncoder enc;
				cerr << "\nEncoding message...\n";
				if(argc>=5)
					enc.SetMessageFile(argv[4]);
				enc.Encode(argv[2],argv[3],true);
				cerr << "\nDone\n";
				break;
			}
			
		case 'd':
			{
				OggStegoDecoder dec;
				cout << "\nDecoding message...\n";
				dec.Decode(argv[2],NULL,true);
				if(argc>=4)
					dec.SaveMessageToFile(argv[3]);
				cerr << "\nDone\n";
				break;
			}
		case 't':
			{
				OggStegoEncoder enc;
				cout << "\nTesting container...\n";
				cerr << enc.Test(argv[2])<<" bytes\n";
				break;
			}
		default:
			{
				Using();

			}
		}
	}catch(Exception exc)
	{
		cerr << exc.getMessage();
		Using();
		return 1;
	}
	
	//OggStegoEncoder enc;
	//OggStegoDecoder dec;
	//BYTE mes[3];
	//mes[0]=1;mes[1]=2;mes[2]=3;
	//try{
	//	cout << "encoding...";
	//	enc.SetMessageFile(argv[4]);
	//	//enc.SetMessage(mes,3);
	//	//enc.Encode(instream,outstream,1);
	//	enc.Encode(argv[2],argv[3],1);
	//	/*enc.ReEncode(*/
	//	cout << " finished\n" << "decoding...";
	//	//fclose(instream);
	//	//fclose(outstream);
	//	//if( (instream = _fsopen( argv[3], ("r+b"), _SH_DENYNO)) == NULL )
	//	//{
	//	//	/*fwprintf(stderr,_T("Can not open file %s\n"), argv[2]);*/
	//	//	fprintf(stderr,("Can not open file %s\n"), argv[3]);
	//	//	system("pause");
	//	//	exit(1);
	//	//};
	//	//dec.Decode(instream,nullstream,1);
	//	dec.Decode(argv[3],NULL,1);
	//	dec.SaveMessageToFile(argv[5]);

	//	//size_t len = dec.GetMessageLength();
	//	//system("pause");
	//	//dec.GetMessageP(mes);
	//	//cout << mes[0] << mes[1] << mes[2];
	//	cout << "\n finished";
	//}catch(Exception exc){
	//	cout << exc.getMessage();
	//}


	////char buffer[1];
	////char str[]="123";
	////for(int i=0;i<3;i++)
	////{
	////	fread(buffer,1,1,instream);
	////	cout << buffer[0];
	////	fwrite(str+i,1,1,outstream);
	////}
	////fclose(instream);
	////fclose(nullstream);
	//system("pause");
	return 0;
}


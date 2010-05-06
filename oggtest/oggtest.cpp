// oggtest.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "OggStegoEncoder.h"
#include "OggStegoDecoder.h"
#include <stdio.h>
#include <conio.h>



//int _tmain(int argc, _TCHAR* argv[])
int main(int argc, char* argv[])
{
	FILE *instream, *outstream, *nullstream;

	/*if( (instream = _wfsopen( argv[2], _T("r+b"), _SH_DENYNO)) == NULL )*/
	if( (instream = _fsopen( argv[2], ("r+b"), _SH_DENYNO)) == NULL )
	{
		/*fwprintf(stderr,_T("Can not open file %s\n"), argv[2]);*/
		fprintf(stderr,("Can not open file %s\n"), argv[2]);
		system("pause");
		exit(1);
	};
	/*if( (outstream = _wfsopen( argv[3], _T("r+b"), _SH_DENYNO)) == NULL )*/
	if( (outstream = _fsopen( argv[3], ("w+b"), _SH_DENYNO)) == NULL )
	{
		/*fwprintf(stderr,_T("Can not open file %s\n"), argv[3]);*/
		fprintf(stderr,("Can not open file %s\n"), argv[3]);
		system("pause");
		exit(1);
	}

	if( (nullstream = _fsopen( "nul", ("wb"), _SH_DENYNO)) == NULL )
	{
		/*fwprintf(stderr,_T("Can not open file %s\n"), argv[3]);*/
		fprintf(stderr,("Can not open file %s\n"), "NULL:");
		system("pause");
		exit(1);
	}
	
	OggStegoEncoder enc;
	OggStegoDecoder dec;
	BYTE mes[3];
	mes[0]=1;mes[1]=2;mes[2]=3;
	try{
		cout << "encoding...";
		enc.SetMessageFile(argv[4]);
		//enc.SetMessage(mes,3);
		enc.ReEncode(instream,outstream,1);
		cout << " finished\n" << "decoding...";
		fclose(instream);
		fclose(outstream);
		if( (instream = _fsopen( argv[3], ("r+b"), _SH_DENYNO)) == NULL )
		{
			/*fwprintf(stderr,_T("Can not open file %s\n"), argv[2]);*/
			fprintf(stderr,("Can not open file %s\n"), argv[3]);
			system("pause");
			exit(1);
		};
		dec.Decode(instream,nullstream,1);
		dec.SetMessageFile(argv[5]);
		//size_t len = dec.GetMessageLength();
		//system("pause");
		//dec.GetMessageP(mes);
		//cout << mes[0] << mes[1] << mes[2];
		cout << "/n finished";
	}catch(Exception exc){
		cout << exc.getMessage();
	}


	//char buffer[1];
	//char str[]="123";
	//for(int i=0;i<3;i++)
	//{
	//	fread(buffer,1,1,instream);
	//	cout << buffer[0];
	//	fwrite(str+i,1,1,outstream);
	//}
	fclose(instream);
	fclose(nullstream);
	system("pause");
	return 0;
}


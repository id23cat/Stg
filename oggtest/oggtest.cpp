// oggtest.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "OggStegoEncoder.h"
#include <stdio.h>
#include <conio.h>



int _tmain(int argc, _TCHAR* argv[])
{
	FILE *instream, *outstream, *mes;
	//if( (instream = _wfsopen( argv[2], _T("r+"), _SH_DENYNO)) == NULL )
	if( (instream = _wfsopen( argv[2], _T("r"), _SH_DENYNO)) == NULL )
	{
		fwprintf(stderr,_T("Can not open file %s\n"), argv[2]);
		getch();
		exit(1);
	};
	//if( (outstream = _wfsopen( argv[3], _T("r+"), _SH_DENYNO)) == NULL )
	if( (outstream = _wfsopen( argv[3], _T("w"), _SH_DENYNO)) == NULL )
	{
		fwprintf(stderr,_T("Can not open file %s\n"), argv[3]);
		getch();
		exit(1);
	}
	
	OggStegoEncoder enc;
	try{
		enc.Encode(instream,outstream,1);
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
	fclose(outstream);
	return 0;
}


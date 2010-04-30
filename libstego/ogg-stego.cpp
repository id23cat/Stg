// ogg-stego.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "OggStegoEncoder.hpp"
#include "OggStegoDecoder.hpp"
#include "conio.h"
#include <stdio.h>
#include <wchar.h>
#include "iostream"
using namespace std;

BYTE* ReadMessage(_TCHAR *fname, size_t *len);
void WriteMessage(_TCHAR *fname, BYTE * mes, size_t len);

int _tmain(int argc, _TCHAR* argv[])
{
	BitArray ar1;
	BitArray ar2(16);
	BYTE str1[16] = {0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15};
	size_t len = 16;
	BYTE str2[16];

	/*for(int q=0;q<16;q++)
		cout << (int)str1[q] << ' ';*/

	FILE *instream, *outstream, *mes;
	//size_t len = 0;
	
	switch(argv[1][0]){
/*********** encoding ********************/
		case _T('e'):
			if( _wfopen_s( &instream, argv[2], _T("rb") ) != 0 )
			{
				fwprintf(stderr,_T("Can not open file %s\n"), argv[2]);
				getch();
				exit(1);
			};
			if( _wfopen_s( &outstream, argv[3], _T("wb") ) != 0 )
			{
				fwprintf(stderr,_T("Can not open file %s\n"), argv[3]);
				getch();
				exit(1);
			}
			/*if( fopen_s( &mes, argv[4], "rb" ) != 0 )
			{
				fprintf(stderr,"Can not open file %s\n", argv[4]);
				getch();
				exit(1);
			};*/
			cout << "Start encoding\n";
			{
				OggStegoEncoder enc;
#if _DEBUG
				_TCHAR vec[256] = {0};
				_TCHAR coef[256] = {0};
				_TCHAR tot[256] = {0};
				_TCHAR lem[256] = {0};
				if(argc<5)
				{
					wcscpy(vec, _T(".\\false\\"));
					wcscpy(coef, _T(".\\false\\"));
					wcscpy(tot, _T(".\\false\\"));				
					wcscpy(lem, _T(".\\false\\"));				
				}
				else
				{
					wcscpy(vec, _T(".\\true\\"));
					wcscpy(coef, _T(".\\true\\"));
					wcscpy(tot, _T(".\\true\\"));
					wcscpy(lem, _T(".\\true\\"));
				}
				wcscat(vec, argv[2]);
				wcscat(coef, argv[2]);
				wcscat(tot, argv[2]);
				wcscat(lem, argv[2]);

				wcscat(vec, _T(".vec"));
				wcscat(coef, _T(".coe"));
				wcscat(tot, _T(".tot"));
				wcscat(lem, _T(".lel"));
				
				
				FILE *vector, *coefficient, *total, *lenm;
				if( _wfopen_s( &vector, vec, _T("wb") ) != 0 )
				{
					fwprintf(stderr,_T("Can not open file %s\n"), argv[3]);
					getch();
					exit(1);
				}
				if( _wfopen_s( &coefficient, coef, _T("wb") ) != 0 )
				{
					fwprintf(stderr,_T("Can not open file %s\n"), argv[3]);
					getch();
					exit(1);
				}
				if( _wfopen_s( &total, tot, _T("wb") ) != 0 )
				{
					fwprintf(stderr,_T("Can not open file %s\n"), argv[3]);
					getch();
					exit(1);
				}
				if( _wfopen_s( &lenm, lem, _T("wb") ) != 0 )
				{
					fwprintf(stderr,_T("Can not open file %s\n"), argv[3]);
					getch();
					exit(1);
				}
				enc.SetLogs(vector, coefficient, total);
#endif
				if(argc<5)
				{
					
					size_t ret = enc.Encode(instream, outstream, false);
				}
				else
				{

					BYTE *message = ReadMessage(argv[4], &len);
					enc.SetMessage(message, len);
					//enc.SetMessage(str1, len);
					size_t ret = enc.Encode(instream, outstream, true);
					cout << ret<<endl;
#if _DEBUG
					size_t ll = enc.GetTotal()/8;
					cout << ll <<endl;
					fwrite(&ll, sizeof(size_t), 1, lenm);
#endif
				}
				cout << "Encoding finished\n";
				fclose(instream);
				fclose(outstream);
#if _DEBUG
				fclose(vector);
				fclose(coefficient);
				fclose(total);
#endif
			}
			break;

/********** decoding **************************/
		case _T('d'):
			if( _wfopen_s( &instream, argv[2], _T("rb") ) != 0 )
			{
				fwprintf(stderr,_T("Can not open file %s\n"), argv[2]);
				getch();
				exit(1);
			};
			if( _wfopen_s( &outstream, argv[3], _T("wb") ) != 0 )
			{
				fwprintf(stderr,_T("Can not open file %s\n"), argv[3]);
				getch();
				exit(1);
			}
			cout << "Start decoding\n";
			{
				OggStegoDecoder dec;
				dec.DecodeMessage(true);
				
				if(argc<5)
					dec.Decode(instream, outstream, false);
				else
				{
					size_t ret = dec.Decode(instream, outstream, true);
					cout << ret << endl;
					len = dec.GetMessageLength();
					cout << len << endl;
					BYTE *message = new BYTE[len];
					len = dec.GetMessage(message);
					cout << len << endl;
					WriteMessage(argv[4], message, len);
				}
			}
			cout << "Decoding finished\n";
			fclose(instream);
			fclose(outstream);
	}

	
	//getch();

	return 0;
}

BYTE* ReadMessage(_TCHAR *fname, size_t *len)
{
	FILE *instream;
	if( _wfopen_s( &instream, fname, _T("rb") ) != 0 )
	{
		fwprintf(stderr,_T("Can not open file %s\n"), fname);
		getch();
		exit(1);
	};
	fseek(instream, 0, SEEK_END);
	*len = ftell(instream);
	BYTE * mes = new BYTE[*len];
	fseek(instream, 0, SEEK_SET);
#if _DEBUG
	cout << *len*8 << endl;	
#endif
	*len = fread(mes, sizeof(BYTE), *len, instream);
	//cout << *len << endl;	
	//cout << (char *) mes;
	fclose(instream);
	return mes;

}

void WriteMessage(_TCHAR *fname, BYTE * mes, size_t len)
{
	FILE *outstream;
	if( _wfopen_s( &outstream, fname, _T("wb") ) != 0 )
	{
		fwprintf(stderr,_T("Can not open file %s\n"), fname);
		getch();
		exit(1);
	};
	fwrite(mes, sizeof(BYTE), len, outstream);
	fclose(outstream);
}

#ifdef _WIN32
#include "stdafx.h"
#endif
#include <stdio.h>

#include "LibInclude.h"
#include "StegoEncoder.h"

StegoEncoder::StegoEncoder(void)
{
	paste_message = false;
	paste_length = false;
#if _DEBUG
	/*totC = 0;*/
#endif
}

StegoEncoder::~StegoEncoder(void)
{
}

void StegoEncoder::SetMessage(BYTE *mes, size_t len)  throw(...)
{
	mesArray.SetArray(mes, len);
	mit = mesArray.Begin();
	paste_message = true;

	lenArray.SetArray((BYTE*)&len, sizeof(size_t));
	size_t l;
	lenArray.GetArray((BYTE*)&l);
	lit = lenArray.Begin();
	paste_length = true;
}

//void StegoEncoder::SetMessageFile(wchar_t *mesFile) throw(...)
//{
//	FILE *instream;
//	size_t len;
//	if( _wfopen_s( &instream, mesFile, _T("rb") ) != 0 )
//	{
//		char str[256]={0};
//		sprintf(str,"Can not open file %s\n",mesFile);
//		throw Exception(str);		
//	};
//	fseek(instream, 0, SEEK_END);
//	len = ftell(instream);
//	BYTE * mes = new BYTE[len];
//	fseek(instream, 0, SEEK_SET);
//	len = fread(mes, sizeof(BYTE), len, instream);
//	//cout << *len << endl;
//	//cout << (char *) mes;
//	fclose(instream);
//	SetMessage(mes,len);
//	delete mes;
//}

void StegoEncoder::SetMessageFile(char *mesFile) throw(...)
{
	FILE *instream;
	size_t len;
	if( fopen_s( &instream, mesFile, "rb" ) != 0 )
	{
		char str[256]={0};
		sprintf(str,"Can not open file %s\n",mesFile);
		throw Exception(str);		
	};
	fseek(instream, 0, SEEK_END);
	len = ftell(instream);
	BYTE * mes = new BYTE[len];
	fseek(instream, 0, SEEK_SET);
	len = fread(mes, sizeof(BYTE), len, instream);
	//cout << *len << endl;
	//cout << (char *) mes;
	fclose(instream);
	SetMessage(mes,len);
	delete mes;
}

#ifdef _WIN32
#endif
#include "stdafx.h"
#include "StegoDecoder.h"

StegoDecoder::StegoDecoder()
{
	get_message = true;
	//get_length = true;
	//mes_len = 0;
	//we=1;
	//lenArray.SetArray(NULL, sizeof(size_t));
	//lenArray.AddArray(NULL, sizeof(size_t));
	//lit = lenArray.Begin();
	mit = mesArray.Begin();
	mit.SetOutOfRangeExceptionStatus(false);
}

StegoDecoder::~StegoDecoder(void)
{
}

size_t StegoDecoder::GetMessageLength()
{
	return mesArray.GetArrayLength();
}

size_t StegoDecoder::GetMessageP(BYTE *ptr)
{
	return mesArray.GetArray(ptr);
}

void StegoDecoder::SaveMessageToFile(char *mesFile) throw(...)
{
	FILE *instream;
	if( fopen_s( &instream, mesFile, "wb" ) != 0 )
	{
		char str[256]={0};
		sprintf(str,"Can not open file %s\n",mesFile);
		throw Exception(str);		
	};
	size_t len;// = GetMessageLength();
	BYTE *mes;// = new BYTE[len];
	//len = GetMessageP(mes);

	mes = mesArray.GetMessage(len);
	len = fwrite(mes, sizeof(BYTE), len, instream);
	fclose(instream);	
	//delete mes;
}

void StegoDecoder::DecodeMessage(bool b)
{
	if(b)
	{
		//lenArray.SetArray(NULL, sizeof(size_t));
		//lenArray.AddArray(NULL, sizeof(size_t));
		//lit = lenArray.Begin();
	}
	//get_message = b;
	//get_length = b;
}

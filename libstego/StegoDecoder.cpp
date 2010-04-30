#ifdef _WIN32
#include "stdafx.h"
#endif
#include "StegoDecoder.h"

StegoDecoder::StegoDecoder()
{
	get_message = false;
	get_length = false;
	mes_len = 0;
	we=1;
	lenArray.SetArray(NULL, sizeof(size_t));
	lit = lenArray.Begin();
	mit = mesArray.Begin();
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

void StegoDecoder::DecodeMessage(bool b)
{
	if(b)
	{
		lenArray.SetArray(NULL, sizeof(size_t));
		lit = lenArray.Begin();
	}
	get_message = b;
	get_length = b;
}

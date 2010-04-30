#ifdef _WIN32
#include "stdafx.h"
#endif
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

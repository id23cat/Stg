#include "StdAfx.h"
#include "LSBStegoDecoder.h"

LSBStegoDecoder::LSBStegoDecoder(void)
{
}

LSBStegoDecoder::~LSBStegoDecoder(void)
{
}

void LSBStegoDecoder::StegoGetMessage(void *vb, BYTE *vector, int len)
{
}

void LSBStegoDecoder::StegoGetLength(void *vb, BYTE *vector, int len)
{
}

void LSBStegoDecoder::StegoGet(void *vb, BYTE *vector, int len)
{
	int i=0;
	BYTE bit=0;
	try
	{
		for(i=0;i<len;i++)
		{
			bit=vector[i]&1;
			lit = bit;
			lit++;
		}
	}catch(Exception exc)
	{
		size_t len;
		lenArray.GetArray((BYTE*)&len);
		mesArray.SetArray(NULL, len);
		mit = mesArray.Begin();
	}
	try
	{
		for(i;i<len;i++)
		{
			bit=vector[i]&1;
			mit = bit;
			mes_len++;
			mit++;
		}
	}catch(Exception exc)
	{
	}
}

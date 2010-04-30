#include "StdAfx.h"
#include "LSBStegoEncoder.h"

LSBStegoEncoder::LSBStegoEncoder(void)
{
}

LSBStegoEncoder::~LSBStegoEncoder(void)
{
}

void LSBStegoEncoder::StegoHideMessage(void *vb, BYTE *vector, int len)
{
}

void LSBStegoEncoder::StegoHideLength(void *vb, BYTE *vector, int len)
{
}

void LSBStegoEncoder::StegoHide(void *vb, BYTE *vector, int len)
{
	int i;
	BYTE bit=0;
	BYTE temp=0;
	
	try
	{
		for(i=0;i<len;i++)
		{
			bit = lit;
			temp=vector[i]>>1;
			temp=temp<<1;
			vector[i]=temp|bit;
			lit++;
		}
	}catch(Exception exc)
	{
		/*pOCD->CallbackFunction = &StegoHideMessage;
		pOSE->paste_length = false;*/
	}
	try
	{
		for(i;i<len;i++)
		{
			bit = mit;
			temp=vector[i]>>1;
			temp=temp<<1;
			vector[i]=temp|bit;
			mit++;
		}
	}catch(Exception exc)
	{
		/*pOCD->CallbackFunction = &StegoHideMessage;
		pOSE->paste_length = false;*/
	}

}
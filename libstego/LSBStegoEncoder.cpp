#include "StdAfx.h"
#include "LSBStegoEncoder.h"

LSBStegoEncoder::LSBStegoEncoder(void)
{
}

LSBStegoEncoder::~LSBStegoEncoder(void)
{
}

//void LSBStegoEncoder::StegoHideMessage(void *vb, BYTE *vector, int len)
//{
//}

void LSBStegoEncoder::StegoHideLength(BYTE *data, int len)
{
	int i;
	BYTE bit=0;
	BYTE temp=0;
	
	try
	{
		for(i=0;i<len;i++)
		{
			bit = lit;
			temp=data[i]>>1;
			temp=temp<<1;
			data[i]=temp|bit;
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
			temp=data[i]>>1;
			temp=temp<<1;
			data[i]=temp|bit;
			mit++;
		}
	}catch(Exception exc)
	{
		/*pOCD->CallbackFunction = &StegoHideMessage;
		pOSE->paste_length = false;*/
	}
}

void LSBStegoEncoder::StegoHide(BYTE *data, int len)
{

	//if(lit

}

int LSBStegoEncoder::Encode(bool pasteMes)
{
	//if(
	if(pasteMes)
	{
		bmpSrc.ReadRGB(infile);
	}
	return 0;
}
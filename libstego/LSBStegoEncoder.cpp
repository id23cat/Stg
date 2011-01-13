#include "StdAfx.h"
#include "LSBStegoEncoder.h"

LSBStegoEncoder::LSBStegoEncoder(void)
{
}

LSBStegoEncoder::~LSBStegoEncoder(void)
{
}

void LSBStegoEncoder::StegoHideMessage(BYTE *data, int len)
{
}

//void LSBStegoEncoder::StegoHideLength(BYTE *data, int len)
//{
//	int i;
//	BYTE bit=0;
//	BYTE temp=0;
//	
//	try
//	{
//		for(i=0;i<len;i++)
//		{
//			bit = lit;
//			temp=data[i]>>1;
//			temp=temp<<1;
//			data[i]=temp|bit;
//			lit++;
//		}
//	}catch(Exception exc)
//	{
//		/*pOCD->CallbackFunction = &StegoHideMessage;
//		pOSE->paste_length = false;*/
//	}
//	try
//	{
//		for(i;i<len;i++)
//		{
//			bit = mit;
//			temp=data[i]>>1;
//			temp=temp<<1;
//			data[i]=temp|bit;
//			mit++;
//		}
//	}catch(Exception exc)
//	{
//		/*pOCD->CallbackFunction = &StegoHideMessage;
//		pOSE->paste_length = false;*/
//	}
//}

void LSBStegoEncoder::StegoHide(BYTE *data, int len)
{

	try{
		for( int j=0 ; j < bmpSrc.TellHeight() ; j++)
		{
			for( int i=0 ; i < bmpSrc.TellWidth() ; i++)
			{
				BYTE b = bmpSrc(i,j)->Blue;
				b = b>>1;
				b = b<<1;
				b = b|(BYTE)mit;
				bmpSrc(i,j)->Blue = b;
				mit++;
				capacityBit++;
			}
		}
	}catch(OutOfRangeException oorExc)
	{		
	}
	catch(Exception exc)
	{
		cerr << exc.getMessage();
	}

}


int LSBStegoEncoder::Encode(char *infile, char *outfile, bool pasteMes)
{
	//bmpSrc = new BMPimage;
	bmpSrc.ReadFromFile(infile);
	//bmpDst = bmpSrc;
	if(pasteMes)
	{		
		StegoHide(NULL, 0);		
	}
	bmpSrc.WriteToFile(outfile);
	
	return ((size_t) capacityBit/8) - BEG_LEN - LEN_LEN - CRC_LEN - END_LEN; 
};

size_t LSBStegoEncoder::Test(char *infile,bool wrtLog)
{
	bmpSrc.ReadFromFile(infile);
	return bmpSrc.TellHeight()*bmpSrc.TellWidth()/8/2 - BEG_LEN - LEN_LEN - CRC_LEN - END_LEN;
}

//int LSBStegoEncoder::Encode(bool pasteMes)
//{
//	//if(
//	/*if(pasteMes)
//	{
//		bmpSrc.ReadRGB(infile);
//	}*/
//	return 0;
//}
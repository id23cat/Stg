#include "StdAfx.h"
#include "LSBStegoDecoder.h"

LSBStegoDecoder::LSBStegoDecoder(void)
{
}

LSBStegoDecoder::~LSBStegoDecoder(void)
{
}

//void LSBStegoDecoder::StegoGetMessage(void *vb, BYTE *vector, int len)
//{
//}



void LSBStegoDecoder::StegoGet(void *vb, BYTE *vector, int len)
{
	try{
		for( int j=0 ; j < bmpSrc.TellHeight() ; j++)
		{
			for( int i=0 ; i < bmpSrc.TellWidth() ; i++)
			{
				BYTE b = bmpSrc(i,j)->Blue;
				mit = b&1;
				mit++;				
			}
		}
	}catch(EndOfMessageException exc)
	{
		//cerr << exc.getMessage()<<endl;
		
	}catch(DamagedMessageException exc)
	{
		cerr << exc.getMessage()<<endl;
		
	}catch(Exception exc)
	{		
		cerr << exc.getMessage()<<endl;		
	}
}

int LSBStegoDecoder::Decode(char *infile, char *outfile, bool getMes)
{
	bmpSrc.ReadFromFile(infile);
	if(getMes)
	{		
		StegoGet(NULL, NULL, 0);		
	}
	return 0;
}

#include "StdAfx.h"
#include "JpegStegoDecoder.h"

JpegStegoDecoder::JpegStegoDecoder(void)
{
	/*cData.CallbackFunction = NULL;
	cData.isStego = 0;
	cData.stegoObjPtr = this;

	dData.CallbackFunction = &(this->StegoGetMessage);
	dData.isStego = 1;
	dData.stegoObjPtr = this;*/
}

JpegStegoDecoder::~JpegStegoDecoder(void)
{
}

void JpegStegoDecoder::InitJpegStego(bool decMes)
{
	if(decMes && get_message)											//
	{																	//
		sData.CallbackFunction = &StegoGetMessage;					//
		sData.isStego = 1;											//		
	}else																//
		sData.isStego = 0;											//
	sData.stegoObjPtr = this;		
}

int JpegStegoDecoder::selectPosition(JCOEF *coef)
{
	for(size_t i=0; i<ALLOW; i++)
	{
		switch(coef[allowable_position[i]])
			case 0:
			case 1:
			case -1:{
				return allowable_position[i];
					}
	}
	return -1;
}

//void JpegStegoDecoder::StegoGetMessage(j_decompress_ptr cinfo, JBLOCKROW *MCU_data)
//{
//	/*JpegDCallbackData *pJCD = static_cast<JpegDCallbackData*>(cinfo->stegoDecoderData.stegoObjPtr);
//	JpegStegoDecoder *pJSD = static_cast<JpegStegoDecoder*>(pJCD->stegoObjPtr);*/
//	JpegStegoDecoder *pJSD = static_cast<JpegStegoDecoder*>(cinfo->stegoDecoderData.stegoObjPtr);
//	if(!pJSD->get_message)// || !pJSD->mesArray.IsArraySet())
//		return;
//	try
//	{
//		jpeg_component_info *compptr;
//		int DCT_pos;
//
//		for (int blkn = 0; blkn < cinfo->blocks_in_MCU; blkn++)
//		{
//			compptr = cinfo->cur_comp_info[cinfo->MCU_membership[blkn]];
//			if(pJSD->get_message)
//			{
//				DCT_pos = pJSD->selectPosition(MCU_data[blkn][0]);
//				BYTE bit = abs(MCU_data[blkn][0][DCT_pos]);
//				if(DCT_pos != -1 && (bit==1 ||bit==0) )
//				{
//					pJSD->mit = bit;
//					pJSD->mit++;
//				}					
//			}
//			pJSD->mit++;
//		}
//
//
//	}catch(EndOfMessageException exc)
//	{
//		cerr << exc.getMessage()<<endl;
//		pJSD->dData.isStego = 0;
//		pJSD->get_message = false;
//	}catch(DamagedMessageException exc)
//	{
//		cerr << exc.getMessage()<<endl;
//		pJSD->dData.isStego = 0;
//		pJSD->get_message = false;	
//	}catch(Exception exc)
//	{		
//		cerr << exc.getMessage()<<endl;		
//	}
//}

void JpegStegoDecoder::StegoGetMessage(void *cinfo, JBLOCKROW *MCU_data)
{
	/*JpegDCallbackData *pJCD = static_cast<JpegDCallbackData*>(cinfo->stegoDecoderData.stegoObjPtr);
	JpegStegoDecoder *pJSD = static_cast<JpegStegoDecoder*>(pJCD->stegoObjPtr);*/
	j_decompress_ptr *dcinfop = static_cast<j_decompress_ptr*>(cinfo);
	j_decompress_ptr dcinfo = *dcinfop;
	JpegStegoDecoder *pJSD = static_cast<JpegStegoDecoder*>(dcinfo->stego.stegoObjPtr);
	if(!pJSD->get_message)// || !pJSD->mesArray.IsArraySet())
		return;
	try
	{
		jpeg_component_info *compptr;
		int DCT_pos;

		for (int blkn = 0; blkn < dcinfo->blocks_in_MCU; blkn++)
		{
			compptr = dcinfo->cur_comp_info[dcinfo->MCU_membership[blkn]];
			if(pJSD->get_message)
			{
				DCT_pos = pJSD->selectPosition(MCU_data[blkn][0]);
				BYTE bit = abs(MCU_data[blkn][0][DCT_pos]);
				//cerr << (int)bit;
				if(DCT_pos != -1 && (bit==1 ||bit==0) )
				{
					pJSD->mit = bit;
					pJSD->mit++;
				}					
			}
			//pJSD->mit++;
		}


	}catch(EndOfMessageException exc)
	{
		cerr << exc.getMessage()<<endl;
		//pJSD->sData.isStego = 0;
		dcinfo->stego.isStego=0;
		pJSD->get_message = false;
	}catch(DamagedMessageException exc)
	{
		cerr << exc.getMessage()<<endl;
		//pJSD->sData.isStego = 0;
		dcinfo->stego.isStego=0;
		pJSD->get_message = false;	
	}catch(Exception exc)
	{		
		cerr << exc.getMessage()<<endl;		
	}
}

//void JpegStegoDecoder::StegoGetLength(j_decompress_ptr cinfo, JBLOCKROW *MCU_data)
//{
//	JpegDCallbackData *pJCD = static_cast<JpegDCallbackData*>(cinfo->stegoDecoderData.stegoObjPtr);
//	JpegStegoDecoder *pJSD = static_cast<JpegStegoDecoder*>(pJCD->stegoObjPtr);
//	if(!pJSD->get_length || !pJSD->lenArray.IsArraySet())
//		return;
//	try
//	{
//
//	}catch(Exception exc)
//	{
//	}
//}

int JpegStegoDecoder::Decode(char *infile, char *outfile, bool getMes)
{
	if(!outfile)
		outfile = "nul";

	// test for existing input and output files
	FILE *fp;
	if ((fp = fopen(infile, READ_BINARY)) == NULL) {
		throw FileNotFoundException("File not found\n",infile);
	}
	fclose(fp);
	if ((fp = fopen(outfile, WRITE_BINARY)) == NULL) {
		throw FileNotFoundException("File not found\n",outfile);
	}
	fclose(fp);

	InitJpegStego(true);

	int argc = 3;
	char *argv[3];
	char name[]="djpeg";
	
	argv[0]=name;
	argv[1]=infile;
	argv[2]=outfile;
	

	return main_djpeg(argc, argv, sData);
}

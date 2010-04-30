#include "StdAfx.h"
#include "JpegStegoDecoder.h"

JpegStegoDecoder::JpegStegoDecoder(void)
{
}

JpegStegoDecoder::~JpegStegoDecoder(void)
{
}

void JpegStegoDecoder::StegoGetMessage(j_decompress_ptr cinfo, JBLOCKROW *MCU_data)
{
	JpegDCallbackData *pJCD = static_cast<JpegDCallbackData*>(cinfo->stegoDecoderData.stegoObjPtr);
	JpegStegoDecoder *pJSD = static_cast<JpegStegoDecoder*>(pJCD->stegoObjPtr);
	if(!pJSD->get_message || !pJSD->lenArray.IsArraySet())
		return;
	try
	{

	}catch(Exception exc)
	{
	}
}

void JpegStegoDecoder::StegoGetLength(j_decompress_ptr cinfo, JBLOCKROW *MCU_data)
{
	JpegDCallbackData *pJCD = static_cast<JpegDCallbackData*>(cinfo->stegoDecoderData.stegoObjPtr);
	JpegStegoDecoder *pJSD = static_cast<JpegStegoDecoder*>(pJCD->stegoObjPtr);
	if(!pJSD->get_length || !pJSD->lenArray.IsArraySet())
		return;
	try
	{

	}catch(Exception exc)
	{
	}
}
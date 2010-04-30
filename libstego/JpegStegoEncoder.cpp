#include "StdAfx.h"
#include "JpegStegoEncoder.h"

extern "C" {
	#include "..\jpeg-8a\jpegtran.h"
}


JpegStegoEncoder::JpegStegoEncoder(void)
{
	//jEdata.CallbackFunction = &(this->StegoHideLength);
	//jEdata.notStego = 0;
	//jEdata.stegoObjPtr = this;

	////jDdata.CallbackFunction = &(this->CountLength);
	//jDdata.notStego = 0;
	//jDdata.stegoObjPtr = this;
	cData.CallbackFunction = &(this->StegoHideLength);
	cData.notStego = 0;
	cData.stegoObjPtr = this;

	//dData.CallbackFunction = &(this->CountLength);
	dData.notStego = 0;
	dData.stegoObjPtr = this;
}
//JpegStegoEncoder::JpegStegoEncoder(size_t dctS2)
//{
//	DCTSIZE2 = dctS2;
//}

JpegStegoEncoder::~JpegStegoEncoder(void)
{
}

int JpegStegoEncoder::selectPosition(JCOEF *coef)
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

int JpegStegoEncoder::selectSign(JCOEF *coef, int position)
{
	if(coef[position]==1)
		return 1;
	else if(coef[position]==-1)
		return -1;
	else if(coef[position]==0)
	{
		int tmp = coef[position-7];
		tmp += coef[position+7];
		tmp += coef[position-8];
		tmp += coef[position+8];
		if(tmp<0)
			return -1;
		else
			return 1;
	}
	return 1;
}

bool JpegStegoEncoder::readBMP(char *fname)
{
	BMPimage bmpImg;
	bmpImg.ReadRGB(fname);
	rgb = bmpImg.GetRGBptr(height,width);
	return true;
}

void JpegStegoEncoder::StegoHideLength(j_compress_ptr cinfo, JBLOCKROW *MCU_data)
{
	JpegECallbackData *pJCD = static_cast<JpegECallbackData*>(cinfo->stegoEncoderData.stegoObjPtr);
	JpegStegoEncoder *pJSE = static_cast<JpegStegoEncoder*>(pJCD->stegoObjPtr);
	if(!pJSE->paste_length || !pJSE->lenArray.IsArraySet())
		return;
	try
	{
		int DCT_pos;
		jpeg_component_info *compptr;

		for (int blkn = 0; blkn < cinfo->blocks_in_MCU; blkn++)
		{
			compptr = cinfo->cur_comp_info[cinfo->MCU_membership[blkn]];
			if(pJSE->paste_message)
			{
				DCT_pos = pJSE->selectPosition(MCU_data[blkn][0]);
				if(DCT_pos != -1)
					MCU_data[blkn][0][DCT_pos] = pJSE->lit *				//set bit with sign
												pJSE->selectSign(MCU_data[blkn][0],DCT_pos);	//
			}
			pJSE->lit++;
		}

	}catch(Exception exc)
	{
		pJCD->CallbackFunction = &StegoHideMessage;
		pJSE->paste_length = false;
	}
}

void JpegStegoEncoder::StegoHideMessage(j_compress_ptr cinfo, JBLOCKROW *MCU_data)
{
	JpegECallbackData *pJCD = static_cast<JpegECallbackData*>(cinfo->stegoEncoderData.stegoObjPtr);
	JpegStegoEncoder *pJSE = static_cast<JpegStegoEncoder*>(pJCD->stegoObjPtr);
	if(!pJSE->paste_message || !pJSE->mesArray.IsArraySet())
		return;
	try
	{
		int DCT_pos;
		jpeg_component_info *compptr;

		for (int blkn = 0; blkn < cinfo->blocks_in_MCU; blkn++)
		{
			compptr = cinfo->cur_comp_info[cinfo->MCU_membership[blkn]];
			if(pJSE->paste_message)
			{
				DCT_pos = pJSE->selectPosition(MCU_data[blkn][0]);
				if(DCT_pos != -1)
					MCU_data[blkn][0][DCT_pos] = pJSE->mit *				//set bit with sign
												pJSE->selectSign(MCU_data[blkn][0],DCT_pos);	//
			}
			pJSE->mit++;
		}

	}catch(Exception exc)
	{
		pJCD->notStego = 1;
	}
}

void JpegStegoEncoder::CountLength(j_compress_ptr cinfo, JBLOCKROW *MCU_data)
{
}

int JpegStegoEncoder::startHiding(char *inf, char *outf)
{
	size_t len = strlen(inf);
	char *ext = inf+(len-3);
	char extl[4]={0};
	for(int i=0;i<3;i++)
		extl[i] = tolower(ext[i]);
	if(!strcmp(extl,"bmp\0"))
		startBmpToJpeg(inf,outf);
	else if(!strcmp(extl,"jpg\0"))
		startJpegToJpeg(inf,outf);
	else
		return -1;
}

int JpegStegoEncoder::startJpegToJpeg(char *inf, char *outf)
{
	int argc = 3;
	char *argv[3];
	char name[]="jpegtran";
	
	argv[0]=name;
	argv[1]=inf;
	argv[2]=outf;

	return main_tran(argc, argv, cData, dData);
}

int JpegStegoEncoder::startBmpToJpeg(char *inf, char *outf)
{
	readBMP(inf);
	struct jpeg_compress_struct cinfo;
	struct jpeg_error_mgr jerr;
	FILE* dst_jpeg;
	JSAMPLE *image_buffer = (JSAMPLE*)rgb;

	cinfo.err = jpeg_std_error(&jerr);
	jpeg_create_compress(&cinfo);

	if (fopen_s(&dst_jpeg, outf, "wb")!=0)
	{
		Exception exc;
		char strerr[STR_LEN];
		sprintf_s(strerr, STR_LEN, "can't open %s\n", outf);
		exc.setMessage(strerr);
		throw exc;
	}

		jpeg_stdio_dest(&cinfo, dst_jpeg);
		cinfo.image_width = width; 	/* image width and height, in pixels */
		cinfo.image_height = height;
		cinfo.input_components = 3;		/* # of color components per pixel */
		cinfo.in_color_space = JCS_RGB; 	/* colorspace of input image */

	JSAMPROW row_pointer[1];	/* pointer to JSAMPLE row[s] */
	int row_stride;		/* physical row width in image buffer */

	jpeg_set_defaults(&cinfo);
	
	jpeg_set_quality(&cinfo, 100, TRUE /* limit to baseline-JPEG values */);
	jpeg_start_compress(&cinfo, TRUE);

	row_stride = width * 3;	/* JSAMPLEs per row in image_buffer */

/**************************************/
	cinfo.stegoEncoderData = cData;
/**************************************/
		
	while (cinfo.next_scanline < cinfo.image_height)
	{
		row_pointer[0] = & image_buffer[cinfo.next_scanline * row_stride];
		(void) jpeg_write_scanlines(&cinfo, row_pointer, 1);
	}
	jpeg_finish_compress(&cinfo);

	fclose(dst_jpeg);
	return 1;
}

size_t JpegStegoEncoder::CountLength(char *inf)
{
	//struct jpeg_decompress_struct cinfo;
	//struct jpeg_error_mgr jerr;

	//cinfo.err = jpeg_std_error(&jerr);
	//jpeg_create_compress(&cinfo);
	//cinfo.stegoDecoderData.stegoObjPtr = (void*) this;
	///*memset(stego_fname, 0, FNAME_LEN);
	//stego_short_name=NULL;*/

	return 1;
}

//{
//	char * outfilename;
//	struct jpeg_decompress_struct srcinfo;
//	struct jpeg_compress_struct dstinfo;
//	struct jpeg_error_mgr jsrcerr, jdsterr;
//#ifdef PROGRESS_REPORT
//	struct cdjpeg_progress_mgr progress;
//#endif
//	jvirt_barray_ptr * src_coef_arrays;
//	jvirt_barray_ptr * dst_coef_arrays;
//	
//	/* We assume all-in-memory processing and can therefore use only a
//	* single file pointer for sequential input and output operation. 
//	*/
//	FILE * fp;
//	/* Initialize the JPEG decompression object with default error handling. */
//	srcinfo.err = jpeg_std_error(&jsrcerr);
//	jpeg_create_decompress(&srcinfo);
//	/* Initialize the JPEG compression object with default error handling. */
//	dstinfo.err = jpeg_std_error(&jdsterr);
//	jpeg_create_compress(&dstinfo);
//	
//	///* Set up default JPEG parameters. */
//	jpeg_transform_info transformoption; /* image transformation options */
//	//boolean simple_progressive = FALSE;
//	//char *outfilename = NULL;
//	//char *scaleoption = NULL;
//	JCOPY_OPTION copyoption = JCOPYOPT_DEFAULT;
//	transformoption.transform = JXFORM_NONE;
//	transformoption.perfect = FALSE;
//	transformoption.trim = FALSE;
//	transformoption.force_grayscale = FALSE;
//	transformoption.crop = FALSE;
//	//cinfo->err->trace_level = 0;
//
//	jsrcerr.trace_level = jdsterr.trace_level = 0;
//	srcinfo.mem->max_memory_to_use = dstinfo.mem->max_memory_to_use;
//	outfilename = outf;
//
//	/* Open the input file. */
//	if ((fp = fopen(inf, "rb")) == NULL) {
//      fprintf(stderr, "%s: can't open %s for reading\n", "JpegStego", inf);
//      exit(EXIT_FAILURE);
//	}
//	//else {
// //   /* default input file is stdin */
//	//	fp = read_stdin();
//	//}
//#ifdef PROGRESS_REPORT
//	start_progress_monitor((j_common_ptr) &dstinfo, &progress);
//#endif
//	/* Specify data source for decompression */
//	jpeg_stdio_src(&srcinfo, fp);
//
//	/* Enable saving of extra markers that we want to copy */
//	jcopy_markers_setup(&srcinfo, copyoption);
//
//	/* Read file header */
//	(void) jpeg_read_header(&srcinfo, TRUE);
//
//	/* Read source file as DCT coefficients */
//	src_coef_arrays = jpeg_read_coefficients(&srcinfo);
//
//	/* Initialize destination compression parameters from source values */
//	jpeg_copy_critical_parameters(&srcinfo, &dstinfo);
//
//	/* Adjust destination parameters if required by transform options;
//	* also find out which set of coefficient arrays will hold the output.
//	*/
//
//	dst_coef_arrays = jtransform_adjust_parameters(&srcinfo, &dstinfo,
//						 src_coef_arrays,
//						 &transformoption);
//	 /* Close input file, if we opened it.
//	* Note: we assume that jpeg_read_coefficients consumed all input
//	* until JPEG_REACHED_EOI, and that jpeg_finish_decompress will
//	* only consume more while (! cinfo->inputctl->eoi_reached).
//	* We cannot call jpeg_finish_decompress here since we still need the
//	* virtual arrays allocated from the source object for processing.
//	*/
//	if (fp != stdin)
//		fclose(fp);
//	/* Open the output file. */
//	if ((fp = fopen(outfilename, "wb")) == NULL) {
//      fprintf(stderr, "%s: can't open %s for writing\n", "JpegStego", outfilename);
//      exit(EXIT_FAILURE);
//    } 
//	//else {
// //   /* default output file is stdout */
//	//	fp = write_stdout();
//	//}
//	 /* Adjust default compression parameters by re-parsing the options */
//	//file_index = parse_switches(&dstinfo, argc, argv, 0, TRUE);
//
//	/* Specify data destination for compression */
//	//jpeg_stdio_dest(&dstinfo, fp);
//
//	/* Start compressor (note no image data is actually written here) */
//	jpeg_write_coefficients(&dstinfo, dst_coef_arrays);
//
//	/* Copy to the output file any extra markers that we want to preserve */
//	//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!jcopy_markers_execute(&srcinfo, &dstinfo, copyoption);
//	
//	/* Finish compression and release memory */
//	jpeg_finish_compress(&dstinfo);
//	jpeg_destroy_compress(&dstinfo);
//	(void) jpeg_finish_decompress(&srcinfo);
//	jpeg_destroy_decompress(&srcinfo);
//
//	/* Close output file, if we opened it */
//	if (fp != stdout)
//		fclose(fp);
//
//#ifdef PROGRESS_REPORT
//	end_progress_monitor((j_common_ptr) &dstinfo);
//#endif
//	/* All done. */
//	//exit(jsrcerr.num_warnings + jdsterr.num_warnings ?EXIT_WARNING:EXIT_SUCCESS);
//	return 0;			/* suppress no-return-value warnings */	
//}
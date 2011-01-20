#include "StdAfx.h"
#include "JpegStegoEncoder.h"

extern "C" {
	
}


JpegStegoEncoder::JpegStegoEncoder(void)
{
	blog = false;
	binary = false;
	koch = false;
	percent = 100;
	D = 10;
	Pi=1;
	quality = 0;
	slog1 = NULL;
	slog2 = NULL;
	work_component;
///********************************************************/
//	//cData.CallbackFunction = &(this->StegoHideLength);
//	cData.CallbackFunction = &(this->StegoHideMessage);
//	cData.isStego = 0;
//	cData.stegoObjPtr = this;
//
//	/*dData.CallbackFunction = NULL;
//	dData.notStego = 1;
//	dData.stegoObjPtr = this;*/
///********************************************************/
}

JpegStegoEncoder::~JpegStegoEncoder(void)
{
}

void JpegStegoEncoder::InitPercent()
{
	Pi=1;
	switch(percent)
	{case 100:
		perc = P100;
		break;
	case 75:
		perc = P75;
		break;
	case 50:
		perc = P50;
		break;
	case 25:
		perc = P25;
		break;
	default:
		perc = P100;
	}
}

void JpegStegoEncoder::InitJpegStego(bool encMes)
{	
	InitPercent();	
	if(encMes && paste_message)
	{							
		sData.CallbackFunction = &StegoHideMessage;
		if(koch)
			sData.CallbackFunction = &StegoKochZhaoHide;
		sData.isStego = 1;
	}else if(!encMes)													//
	{
		sData.CallbackFunction = &StegoTestContainer;
		sData.isStego = 1;
		sData.CallbackFunction2 = &StegoTestContainer2;
		sData.isStegoAnalysis = 1;
		
	}else
		sData.isStego = 0;

	sData.stegoObjPtr = this;

	/*dData.CallbackFunction = NULL;
	dData.notStego = 1;
	dData.stegoObjPtr = this;*/

}

//int JpegStegoEncoder::selectPosition(JCOEF *coef)
//{
//	for(size_t i=0; i<ALLOW; i++)
//	{
//		switch(coef[allowable_position[i]])
//			case 0:
//			case 1:
//			case -1:{
//				return allowable_position[i];
//			}
//	}
//	return -1;
//}

//int JpegStegoEncoder::selectSign(JCOEF *coef, int position)
//{
//	if(coef[position]==1)
//		return 1;
//	else if(coef[position]==-1)
//		return -1;
//	else if(coef[position]==0)
//	{
//		int tmp = coef[position-7];
//		tmp += coef[position+7];
//		tmp += coef[position-8];
//		tmp += coef[position+8];
//		if(tmp<0)
//			return -1;
//		else
//			return 1;
//	}
//	return 1;
//}

//bool JpegStegoEncoder::readBMP(char *fname)
//{
//	BMPimage bmpImg;
//	bmpImg.ReadRGB(fname);
//	rgb = bmpImg.GetRGBptr(height,width);
//	return true;
//}


//void JpegStegoEncoder::StegoHideMessage(j_compress_ptr cinfo, JBLOCKROW *MCU_data)
//{
//	//JpegECallbackData *pJCD = static_cast<JpegECallbackData*>(cinfo->stegoEncoderData/*.stegoObjPtr*/);
//	//JpegStegoEncoder *pJSE = static_cast<JpegStegoEncoder*>(pJCD->stegoObjPtr);
//	//StegoCData *pJCD = static_cast<StegoCData*>(cinfo->stegoEncoderData/*.stegoObjPtr*/);
//	JpegStegoEncoder *pJSE = static_cast<JpegStegoEncoder*>(cinfo->stegoEncoderData.stegoObjPtr);
//	if(!pJSE->paste_message || !pJSE->mesArray.IsArraySet())
//		return;
//	try
//	{
//		int DCT_pos;
//		jpeg_component_info *compptr;
//
//		for (int blkn = 0; blkn < cinfo->blocks_in_MCU; blkn++)
//		{
//			compptr = cinfo->cur_comp_info[cinfo->MCU_membership[blkn]];
//			if(pJSE->paste_message)
//			{
//				DCT_pos = pJSE->selectPosition(MCU_data[blkn][0]);
//				if(DCT_pos != -1)
//					MCU_data[blkn][0][DCT_pos] = pJSE->mit *				//set bit with sign
//												pJSE->selectSign(MCU_data[blkn][0],DCT_pos);	//
//			}
//			pJSE->mit++;
//		}
//
//	}catch(OutOfRangeException oorExc)
//	{
//		//cerr<<oorExc.getMessage();
//		pJSE->cData.isStego = 0;
//	}
//	catch(Exception exc)
//	{
//		cerr << exc.getMessage();
//		//pOCD->isStego = 0;
//	}
//}
//
//
//
//void JpegStegoEncoder::StegoTestContainer(j_compress_ptr cinfo, JBLOCKROW *MCU_data)
//{
//	/*JpegECallbackData *pJCD = static_cast<JpegECallbackData*>(cinfo->stegoEncoderData.stegoObjPtr);
//	JpegStegoEncoder *pJSE = static_cast<JpegStegoEncoder*>(pJCD->stegoObjPtr);*/
//	JpegStegoEncoder *pJSE = static_cast<JpegStegoEncoder*>(cinfo->stegoEncoderData.stegoObjPtr);
//	
//	try
//	{
//		int DCT_pos;
//		jpeg_component_info *compptr;
//
//		for (int blkn = 0; blkn < cinfo->blocks_in_MCU; blkn++)
//		{
//			compptr = cinfo->cur_comp_info[cinfo->MCU_membership[blkn]];
//			DCT_pos = pJSE->selectPosition(MCU_data[blkn][0]);
//			if(DCT_pos != -1)
//				pJSE->capacityBit++;			
//		}
//	}catch(Exception exc)
//	{
//		pJSE->cData.isStego = 0;
//	}
//}

void JpegStegoEncoder::StegoHideMessage(void *cinfo, JBLOCKROW *MCU_data)
{
	//JpegECallbackData *pJCD = static_cast<JpegECallbackData*>(cinfo->stegoEncoderData/*.stegoObjPtr*/);
	//JpegStegoEncoder *pJSE = static_cast<JpegStegoEncoder*>(pJCD->stegoObjPtr);
	//StegoCData *pJCD = static_cast<StegoCData*>(cinfo->stegoEncoderData/*.stegoObjPtr*/);
	j_compress_ptr *ccinfop = static_cast<j_compress_ptr*>(cinfo);
	j_compress_ptr ccinfo = *ccinfop;
	JpegStegoEncoder *pJSE = static_cast<JpegStegoEncoder*>(ccinfo->stego.stegoObjPtr);
	
	
	/*if(!pJSE->paste_message || !pJSE->mesArray.IsArraySet())
		return;*/
	try
	{

		BYTE bit;
		int DCT_pos;
		jpeg_component_info *compptr;

		for (int blkn = 0; blkn < ccinfo->blocks_in_MCU; blkn++)
		{
			
			compptr = ccinfo->cur_comp_info[ccinfo->MCU_membership[blkn]];
			if(pJSE->paste_message && pJSE->mesArray.IsArraySet())
			{
				if(pJSE->paste_message)
				{
					DCT_pos = /*pJSE->*/selectPosition(MCU_data[blkn][0]);
					if(DCT_pos != -1)
					{
						bit = pJSE->mit;
						//cerr << (int)bit;
						MCU_data[blkn][0][DCT_pos] = bit *				//set bit with sign
						/*pJSE->*/selectSign(MCU_data[blkn][0],DCT_pos);	//
						pJSE->mit++;
					}
				}
			}
			if(pJSE->blog) 
			{
				pJSE->slog1->writeData(compptr->component_id);
				for(int i=0; i<8; i++)
				{
					for(int j=0; j<8; j++)
					{						
						pJSE->slog1->writeCoef(MCU_data[blkn][0][i*8+j]);
					}
					pJSE->slog1->endStr();
				}				
				pJSE->slog1->endBlock();
			}			
		}

	}catch(OutOfRangeException oorExc)
	{
		//cerr<<oorExc.getMessage();
		ccinfo->stego.isStego=0;
		//pJSE->sData.isStego = 0;
	}
	catch(Exception exc)
	{
		cerr << exc.getMessage();
		//pOCD->isStego = 0;
	}
}

void JpegStegoEncoder::StegoKochZhaoHide(void *cinfo, JBLOCKROW *MCU_data)
{
	//JpegECallbackData *pJCD = static_cast<JpegECallbackData*>(cinfo->stegoEncoderData/*.stegoObjPtr*/);
	//JpegStegoEncoder *pJSE = static_cast<JpegStegoEncoder*>(pJCD->stegoObjPtr);
	//StegoCData *pJCD = static_cast<StegoCData*>(cinfo->stegoEncoderData/*.stegoObjPtr*/);
	j_compress_ptr *ccinfop = static_cast<j_compress_ptr*>(cinfo);
	j_compress_ptr ccinfo = *ccinfop;
	JpegStegoEncoder *pJSE = static_cast<JpegStegoEncoder*>(ccinfo->stego.stegoObjPtr);


	/*if(!pJSE->paste_message || !pJSE->mesArray.IsArraySet())
		return;*/
	try
	{

		BYTE bit;
		LKoch DCT_pos;
		jpeg_component_info *compptr;
		int D = pJSE->D;
		int d = D>>1;
		int ii=0;

		for (int blkn = 0; blkn < ccinfo->blocks_in_MCU; blkn++)
		{
			compptr = ccinfo->cur_comp_info[ccinfo->MCU_membership[blkn]];
			if(pJSE->paste_message && pJSE->mesArray.IsArraySet())
			{
				if(pJSE->paste_message && pJSE->perc>=pJSE->Pi)
				{				
					bit = pJSE->mit;
					for(int i=0;i<KOCH_MAX; i++)
					{
						ii = (((double) rand() / 
							 (double) RAND_MAX) * (KOCH_MAX-1) + 0);

						DCT_pos = /*pJSE->*/KochZhaoPosition(ii);
						
						if(bit==1)
						{
							if(min(MCU_data[blkn][0][DCT_pos.l1],MCU_data[blkn][0][DCT_pos.l2])+D
								< MCU_data[blkn][0][DCT_pos.l3])
							{
								// here must be modification
								MCU_data[blkn][0][DCT_pos.l1]=MCU_data[blkn][0][DCT_pos.l3]-d;
								MCU_data[blkn][0][DCT_pos.l2]=MCU_data[blkn][0][DCT_pos.l3]+d;
																	
								continue;
							}
							
							MCU_data[blkn][0][DCT_pos.l3]-=d;
							MCU_data[blkn][0][DCT_pos.l1]+=d/*+1*/;						
							MCU_data[blkn][0][DCT_pos.l2]+=d/*+1*/;						
							pJSE->mit++;
							break;
						}else
						{
							if(max(MCU_data[blkn][0][DCT_pos.l1],MCU_data[blkn][0][DCT_pos.l2]) > 
								MCU_data[blkn][0][DCT_pos.l3]+D/*-1*/)
							{
								// here must be modification
								MCU_data[blkn][0][DCT_pos.l1]=MCU_data[blkn][0][DCT_pos.l3]-d;
								MCU_data[blkn][0][DCT_pos.l2]=MCU_data[blkn][0][DCT_pos.l3]+d;
								continue;
							}

							MCU_data[blkn][0][DCT_pos.l3]+=d/*+1*/;
							MCU_data[blkn][0][DCT_pos.l1]-=d;
							MCU_data[blkn][0][DCT_pos.l2]-=d;						
							pJSE->mit++;
							break;
						}
						
						MCU_data[blkn][0][DCT_pos.l1]=100;
						MCU_data[blkn][0][DCT_pos.l2]=200;
						MCU_data[blkn][0][DCT_pos.l3]=300;
					}
					//pJSE->mit++;
					//if(DCT_pos != -1)
					//{
					//	bit = pJSE->mit;
					//	//cerr << (int)bit;
					//	MCU_data[blkn][0][DCT_pos] = bit *				//set bit with sign
					//		/*pJSE->*/selectSign(MCU_data[blkn][0],DCT_pos);	//
					//	pJSE->mit++;
					//}
				}
				
				pJSE->Pi++;
					if(pJSE->Pi>4)
						pJSE->Pi=1;
			}

			if(pJSE->blog) 
			{
				pJSE->slog1->writeData(compptr->component_id);
				for(int i=0; i<8; i++)
				{
					for(int j=0; j<8; j++)
					{						
						pJSE->slog1->writeCoef(MCU_data[blkn][0][i*8+j]);
					}
					pJSE->slog1->endStr();
				}				
				pJSE->slog1->endBlock();
			}

		}

	}catch(OutOfRangeException oorExc)
	{
		//cerr<<oorExc.getMessage();
		ccinfo->stego.isStego=0;
		//pJSE->sData.isStego = 0;
	}
	catch(Exception exc)
	{
		cerr << exc.getMessage();
		//pOCD->isStego = 0;
	}
}

void JpegStegoEncoder::StegoTestContainer(void *cinfo, JBLOCKROW *MCU_data)
{
	/*JpegECallbackData *pJCD = static_cast<JpegECallbackData*>(cinfo->stegoEncoderData.stegoObjPtr);
	JpegStegoEncoder *pJSE = static_cast<JpegStegoEncoder*>(pJCD->stegoObjPtr);*/
	j_compress_ptr *ccinfop = static_cast<j_compress_ptr*>(cinfo);
	j_compress_ptr ccinfo = *ccinfop;
	JpegStegoEncoder *pJSE = static_cast<JpegStegoEncoder*>(ccinfo->stego.stegoObjPtr);

	try
	{
		int DCT_pos;
		jpeg_component_info *compptr;

		int blkn;
		for (blkn = 0; blkn < ccinfo->blocks_in_MCU; blkn++)
		{
			compptr = ccinfo->cur_comp_info[ccinfo->MCU_membership[blkn]];
			DCT_pos = /*pJSE->*/selectPosition(MCU_data[blkn][0]);
			if(DCT_pos != -1)
				pJSE->capacityBit++;

			//Writing LOG
			if(pJSE->blog) 
			{
				pJSE->slog1->writeData(compptr->component_id);
				for(int i=0; i<8; i++)
				{
					for(int j=0; j<8; j++)
					{						
						pJSE->slog1->writeCoef(MCU_data[blkn][0][i*8+j]);
					}
					pJSE->slog1->endStr();
				}				
				pJSE->slog1->endBlock();
			}
		}
		
	}catch(Exception exc)
	{
		pJSE->sData.isStego = 0;
	}
}

void JpegStegoEncoder::StegoTestContainer2(void* cinfo, int workspace[64],int blkn)
{
	j_compress_ptr *ccinfop = static_cast<j_compress_ptr*>(cinfo);
	j_compress_ptr ccinfo = *ccinfop;
	JpegStegoEncoder *pJSE = static_cast<JpegStegoEncoder*>(ccinfo->stego.stegoObjPtr);

	try
	{
		int DCT_pos;
		jpeg_component_info *compptr;

		//int blkn;

		compptr = ccinfo->cur_comp_info[ccinfo->MCU_membership[blkn]];
		if(pJSE->blog) 
		{
			pJSE->slog2->writeData(compptr->component_id);
			for(int i=0; i<8; i++)
			{
				for(int j=0; j<8; j++)
				{						
					pJSE->slog2->writeCoef(workspace[i*8+j]);
				}
				pJSE->slog2->endStr();
			}				
			pJSE->slog2->endBlock();
		}
		//for (blkn = 0; blkn < ccinfo->blocks_in_MCU; blkn++)
		//{
		//	
		//	DCT_pos = pJSE->selectPosition(MCU_data[blkn][0]);
		//	if(DCT_pos != -1)
		//		pJSE->capacityBit++;

		//	//Writing LOG
		//	
		//}

	}catch(Exception exc)
	{
		pJSE->sData.isStegoAnalysis = 0;
	}
}


//int  JpegStegoEncoder::Encode(char **infiles, int count, char *dstdir, bool pasteMes)
//{
//	if(count <1)
//		throw Exception("Must be one or more input files");
//	char outfile[512];
//	int len=0;
//	for(int i=0; i<count; i++)
//	{
//		memset(outfile,0,512);
//		genOutFileName(infiles[i],outfile,dstdir,i,count);
//		len = strlen(outfile);
//		outfile[len-3]='j';
//		outfile[len-2]='p';
//		outfile[len-1]='g';
//		Encode(infiles[i], outfile, pasteMes);
//	}	
//}

int JpegStegoEncoder::Encode(char *infile, char *outfile, bool pasteMes)
{
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

	if(blog)
	{		
		slog1 = new JpegStegoLog;
		slog1->Bin(binary);
		slog1->setLogFileName(outfile);
		slog1->openLOG(ALL);
		/*slog2 = new JpegStegoLog;
		slog2->setLogFileName(infile,"DCT");
		slog2->openLOG(ALL);*/
	}

	InitJpegStego(true);
	size_t len = strlen(infile);
	char *ext = infile+(len-3);
	char extl[4]={0};
	for(int i=0;i<3;i++)
		extl[i] = tolower(ext[i]);
	if(!strcmp(extl,"bmp\0"))
		startBmpToJpeg(infile,outfile);
	else if(!strcmp(extl,"jpg\0"))
		startJpegToJpeg(infile,outfile);
	else
		throw Exception("Unsupported file type");
	return 0;
}

size_t JpegStegoEncoder::Test(char *infile, bool wrtLog)
{
	// test for existing input file
	//blog = wrtLog;
	FILE *fp;
	if ((fp = fopen(infile, READ_BINARY)) == NULL) {
		throw FileNotFoundException("File not found\n",infile);
	}
	fclose(fp);
	if ((fp = fopen("nul", READ_BINARY)) == NULL) {
		throw FileNotFoundException("File not found\n","\Device\Null");
	}
	fclose(fp);

	if(blog)
	{		
		slog1 = new JpegStegoLog;
		slog1->Bin(binary);
		slog1->setLogFileName(infile);
		slog1->openLOG(ALL);	
	}

	InitJpegStego(false);
	size_t len = strlen(infile);
	char *ext = infile+(len-3);
	char extl[4]={0};
	for(int i=0;i<3;i++)
		extl[i] = tolower(ext[i]);
	if(!strcmp(extl,"bmp\0"))
		startBmpToJpeg(infile,"nul");
	else if(!strcmp(extl,"jpg\0"))
		startJpegToJpeg(infile,"nul");
	else
		throw Exception("Unsupported file type");
	if(wrtLog)
	{
		slog1->close();
		delete slog1;
		/*slog2->close();
		delete slog2;*/
	}
	if(capacityBit/8 <= BEG_LEN + LEN_LEN + CRC_LEN + END_LEN)
		return 0;
	return ((size_t) capacityBit/8) - BEG_LEN - LEN_LEN - CRC_LEN - END_LEN;
}

//int JpegStegoEncoder::startHiding(char *inf, char *outf)
//{
//	size_t len = strlen(inf);
//	char *ext = inf+(len-3);
//	char extl[4]={0};
//	for(int i=0;i<3;i++)
//		extl[i] = tolower(ext[i]);
//	if(!strcmp(extl,"bmp\0"))
//		startBmpToJpeg(inf,outf);
//	else if(!strcmp(extl,"jpg\0"))
//		startJpegToJpeg(inf,outf);
//	else
//		return -1;
//}

int JpegStegoEncoder::startJpegToJpeg(char *inf, char *outf)
{
	int argc = 3;
	char *argv[3];
	char name[]="jpegtran";
	
	argv[0]=name;
	argv[1]=inf;
	argv[2]=outf;

	return main_tran(argc, argv, sData);
}

int JpegStegoEncoder::startBmpToJpeg(char *inf, char *outf)
{
	int argc = 3;
	char *argv[5];
	char name[]="cjpeg";
	char qua[3];

	argv[0]=name;
	argv[1]=inf;
	argv[2]=outf;
	if(quality)
	{
		argc = 5;
		argv[3]= "-quality";
		itoa(quality,qua,10);
		argv[4]= qua;
	}

	return main_cjpeg(argc, argv, sData);
//	readBMP(inf);
//	struct jpeg_compress_struct cinfo;
//	struct jpeg_error_mgr jerr;
//	FILE* dst_jpeg;
//	JSAMPLE *image_buffer = (JSAMPLE*)rgb;
//
//	cinfo.err = jpeg_std_error(&jerr);
//	jpeg_create_compress(&cinfo);
//
//	if (fopen_s(&dst_jpeg, outf, "wb")!=0)
//	{
//		Exception exc;
//		char strerr[STR_LEN];
//		sprintf_s(strerr, STR_LEN, "can't open %s\n", outf);
//		exc.setMessage(strerr);
//		throw exc;
//	}
//
//		jpeg_stdio_dest(&cinfo, dst_jpeg);
//		cinfo.image_width = width; 	/* image width and height, in pixels */
//		cinfo.image_height = height;
//		cinfo.input_components = 3;		/* # of color components per pixel */
//		cinfo.in_color_space = JCS_RGB; 	/* colorspace of input image */
//
//	JSAMPROW row_pointer[1];	/* pointer to JSAMPLE row[s] */
//	int row_stride;		/* physical row width in image buffer */
//
//	jpeg_set_defaults(&cinfo);
//	
//	jpeg_set_quality(&cinfo, 100, TRUE /* limit to baseline-JPEG values */);
//	jpeg_start_compress(&cinfo, TRUE);
//
//	row_stride = width * 3;	/* JSAMPLEs per row in image_buffer */
//
///**************************************/
//	cinfo.stego = sData;
///**************************************/
//		
//	while (cinfo.next_scanline < cinfo.image_height)
//	{
//		row_pointer[0] = & image_buffer[cinfo.next_scanline * row_stride];
//		(void) jpeg_write_scanlines(&cinfo, row_pointer, 1);
//	}
//	jpeg_finish_compress(&cinfo);
//
//	fclose(dst_jpeg);
	return 1;
}

//size_t JpegStegoEncoder::CountLength(char *inf)
//{
//	//struct jpeg_decompress_struct cinfo;
//	//struct jpeg_error_mgr jerr;
//
//	//cinfo.err = jpeg_std_error(&jerr);
//	//jpeg_create_compress(&cinfo);
//	//cinfo.stegoDecoderData.stegoObjPtr = (void*) this;
//	///*memset(stego_fname, 0, FNAME_LEN);
//	//stego_short_name=NULL;*/
//
//	return 1;
//}

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
#ifdef _WIN32

#endif
#include "stdafx.h"
#include "OggStegoEncoder.h"

/* takes a stereo 16bit 44.1kHz WAV file from stdin and encodes it into
   a Vorbis bitstream */

/* Note that this is POSIX, not ANSI, code */

OggStegoEncoder::OggStegoEncoder(void)
{
}

OggStegoEncoder::~OggStegoEncoder(void)
{
}

void OggStegoEncoder::InitVorbisStego(vorbis_block *vb, bool encMes)
{
	if(encMes && paste_message)
	{																	//
		vb->sData.CallbackFunction = &StegoHideMessage;					//
		vb->sData.isStego = 1;											//		
	}else if(!encMes)													//
	{
		vb->sData.CallbackFunction = &StegoTestContainer;				//
		vb->sData.isStego = 1;											//		
	}else
		vb->sData.isStego = 0;											//
	vb->sData.stegoObjPtr = (void*) this;
}


void OggStegoEncoder::StegoHideMessage(void *vb, float *vector, int len)
{
	OggCallbackData *pOCD = static_cast<OggCallbackData*>(vb);
	OggStegoEncoder *pOSE = static_cast<OggStegoEncoder*>(pOCD->stegoObjPtr);
	if(!pOSE->paste_message || !pOSE->mesArray.IsArraySet())
		return;

	try
	{
		for(int i=1; i<len; i++)
		{
			if(vector[i]==1 || vector[i]==2)
			{
				BYTE bit = pOSE->mit;
#ifdef _DEBUG
		ticE++;
		if(ticE>=LOWE && ticE<HIGE)
			cout << int(bit);
#endif
				vector[i] = (float)1+bit;
				pOSE->mit++;
				break;
			}else if(vector[i]==-1 || vector[i]==-2)
			{
				BYTE bit = pOSE->mit;
#ifdef _DEBUG
		ticE++;
		if(ticE>=LOWE && ticE<HIGE)
			cout << int(bit);
#endif
				vector[i] = (float)-(1+bit);
				pOSE->mit++;
				break;
			}
		}
	}
	catch(OutOfRangeException oorExc)
	{
		//cerr<<oorExc.getMessage();
		pOCD->isStego = 0;
	}
	catch(Exception exc)
	{
		cerr << exc.getMessage();
		//pOCD->isStego = 0;
	}
}

void OggStegoEncoder::StegoTestContainer(void *vb, float *vector, int len)
{
	OggCallbackData *pOCD = static_cast<OggCallbackData*>(vb);
	OggStegoEncoder *pOSE = static_cast<OggStegoEncoder*>(pOCD->stegoObjPtr);
	
	try
	{
		for(int i=1; i<len; i++)
		{
			if(vector[i]==1 || vector[i]==2)
			{
				pOSE->capacityBit++;
				break;
			}else if(vector[i]==-1 || vector[i]==-2)
			{
				pOSE->capacityBit++;
				break;
			}
		}
	}	
	catch(Exception exc)
	{
		cerr<<exc.getMessage();
		pOCD->isStego = 0;
	}
}

int OggStegoEncoder::Encode(char *infile, char *outfile, bool pasteMes)
{
	size_t len = strlen(infile);
	char *ext = infile+(len-3);
	char extl[4]={0};
	for(int i=0;i<3;i++)
		extl[i] = tolower(ext[i]);
	FILE *instream, *outstream;
	if( (instream = _fsopen( infile, ("r+b"), _SH_DENYNO)) == NULL )
	{
		char buf[256];
		sprintf(buf,("Can not open file %s\n"), infile);
		throw Exception(buf);		
	};
	/*if( (outstream = _wfsopen( argv[3], _T("r+b"), _SH_DENYNO)) == NULL )*/
	if( (outstream = _fsopen( outfile, ("w+b"), _SH_DENYNO)) == NULL )
	{
		char buf[256];
		sprintf(buf,("Can not open file %s\n"), outfile);
		throw Exception(buf);		
	}
	if(!strcmp(extl,"wav\0"))
	{

		startWavToOgg(instream,outstream,pasteMes);
	}
	else if(!strcmp(extl,"ogg\0"))
	{
		startOggToOgg(instream,outstream,pasteMes);
	}
	else
		return -1;
	fclose(instream);
	fclose(outstream);
	return 0;
}

size_t OggStegoEncoder::Test(char *infile)
{
	size_t len = strlen(infile);
	char *ext = infile+(len-3);
	char extl[4]={0};
	for(int i=0;i<3;i++)
		extl[i] = tolower(ext[i]);
	FILE *instream, *outstream;
	if( (instream = _fsopen( infile, ("r+b"), _SH_DENYNO)) == NULL )
	{
		char buf[256];
		sprintf(buf,("Can not open file %s\n"), infile);
		throw Exception(buf);		
	};
	/*if( (outstream = _wfsopen( argv[3], _T("r+b"), _SH_DENYNO)) == NULL )*/
	if( (outstream = _fsopen( "nul", ("wb"), _SH_DENYNO)) == NULL )
	{		
		char buf[256];
		sprintf(buf,("Can not open file %s\n"), "NULL:");
		throw Exception(buf);
	}
	if(!strcmp(extl,"wav\0"))
	{

		startWavToOgg(instream,outstream,false);
	}
	else if(!strcmp(extl,"ogg\0"))
	{
		startOggToOgg(instream,outstream,false);
	}
	else
		return -1;
	fclose(instream);
	fclose(outstream);
	return ((size_t) capacityBit/8) - BEG_LEN - LEN_LEN - CRC_LEN - END_LEN;
}

size_t OggStegoEncoder::startOggToOgg(FILE *instream, FILE *outstream, bool encMes)
{
	ogg_int16_t convbuffer[4096]; /* take 8k out of the data segment, not the stack */
	signed char readbuffer[READ*4+44]; /* out of the data segment, not the stack */
	//signed char *readbuffer;
	int convsize=4096;

	ogg_sync_state oy; /* sync and verify incoming physical bitstream */
	ogg_stream_state os;/* take physical pages, weld into a logical
						stream of packets */
	ogg_stream_state ose;
	ogg_page og; /* one Ogg bitstream page. Vorbis packets are inside */
	ogg_page oge;
	ogg_packet op; /* one raw packet of data for decode */
	ogg_packet ope;

	vorbis_info vi;/* struct that stores all the static vorbis bitstream
				   settings */
	vorbis_info vie;

	vorbis_comment vc; /* struct that stores all the bitstream user comments */
	vorbis_comment vce;
	vorbis_dsp_state vd; /* central working state for the packet->PCM decoder */
	vorbis_dsp_state vde;
	vorbis_block vb; /* local working space for packet->PCM decode */
	vorbis_block vbe;

	char *buffer;
	int bytes;
	/**encoder's**/
	int eos=0,ret,i,founddata;

	//#ifdef _WIN32 /* We need to set stdin/stdout to binary mode. Damn windows. */
	// /* Beware the evil ifdef. We avoid these where we can, but this one we
	// cannot. Don't add any more, you'll probably go to hell if you do. */
	// //_setmode( _fileno( stdin ), _O_BINARY );
	// //_setmode( _fileno( stdout ), _O_BINARY );
	//#endif

#if defined(macintosh) && defined(__MWERKS__)
	{
		int argc;
		char **argv;
		argc=ccommand(&argv); /* get a "command line" from the Mac user */
		/* this also lets the user set stdin and stdout */
	}
#endif

	ogg_sync_init(&oy); /* Now we can read pages */

	while(1){ /* we repeat if the bitstream is chained */
			/********** Decode setup ************/
		{
			int eos=0;
			int i;

			/* grab some data at the head of the stream. We want the first page
			(which is guaranteed to be small and only contain the Vorbis
			stream initial header) We need the first page to get the stream
			serialno. */

			/* submit a 4k block to libvorbis' Ogg layer */
			buffer=ogg_sync_buffer(&oy,4096);
			//bytes=fread(buffer,1,4096,stdin);
			bytes=fread(buffer,1,4096,instream);
			ogg_sync_wrote(&oy,bytes);

			/* Get the first page. */
			if(ogg_sync_pageout(&oy,&og)!=1){
				/* have we simply run out of data? If so, we're done. */
				if(bytes<4096)break;

				/* error case. Must not be Vorbis data */
				throw(Exception("OggStegoEncoder: Input does not appear to be an Ogg bitstream.\n"));				
			}

			/* Get the serial number and set up the rest of decode. */
			/* serialno first; use it to set up a logical stream */
			ogg_stream_init(&os,ogg_page_serialno(&og));

			/* extract the initial header from the first page and verify that the
			Ogg bitstream is in fact Vorbis data */

			/* I handle the initial header first instead of just having the code
			read all three Vorbis headers at once because reading the initial
			header is an easy way to identify a Vorbis bitstream and it's
			useful to see that functionality seperated out. */

			vorbis_info_init(&vi);
			vorbis_comment_init(&vc);
			if(ogg_stream_pagein(&os,&og)<0){
				/* error; stream version mismatch perhaps */
				throw(Exception("OggStegoEncoder:Error reading first page of Ogg bitstream data.\n"));				
			}

			if(ogg_stream_packetout(&os,&op)!=1){
				/* no page? must not be vorbis */
				throw(Exception("OggStegoEncoder:Error reading initial header packet.\n"));				
			}

			if(vorbis_synthesis_headerin(&vi,&vc,&op)<0){
				/* error case; not a vorbis header */
				throw(Exception("OggStegoEncoder:This Ogg bitstream does not contain Vorbis "
					"audio data.\n"));				
			}



			/* At this point, we're sure we're Vorbis. We've set up the logical
			(Ogg) bitstream decoder. Get the comment and codebook headers and
			set up the Vorbis decoder */

			/* The next two packets in order are the comment and codebook headers.
			They're likely large and may span multiple pages. Thus we read
			and submit data until we get our two packets, watching that no
			pages are missing. If a page is missing, error out; losing a
			header page is the only place where missing data is fatal. */

			i=0;
			while(i<2){
				while(i<2){
					int result=ogg_sync_pageout(&oy,&og);
					if(result==0)break;/* Need more data */
					/* Don't complain about missing or corrupt data yet. We'll
					catch it at the packet output phase */
					if(result==1){
						ogg_stream_pagein(&os,&og);/* we can ignore any errors here
												   as they'll also become apparent
												   at packetout */
						while(i<2){
							result=ogg_stream_packetout(&os,&op);
							if(result==0)break;
							if(result<0){
								/* Uh oh; data at some point was corrupted or missing!
								We can't tolerate that in a header. Die. */
								throw(Exception("OggStegoEncoder:Corrupt secondary header. Exiting.\n"));
							}
							result=vorbis_synthesis_headerin(&vi,&vc,&op);
							if(result<0){
								throw Exception("OggStegoEncoder:Corrupt secondary header. Exiting.\n");
							}
							i++;
						}
					}
				}
				/* no harm in not checking before adding more */
				buffer=ogg_sync_buffer(&oy,4096);
				//bytes=fread(buffer,1,4096,stdin);
				bytes=fread(buffer,1,4096,instream);
				if(bytes==0 && i<2){
					throw Exception("OggStegoEncoder:End of file before finding all Vorbis headers!\n");					
				}
				ogg_sync_wrote(&oy,bytes);
			}


			/* Throw the comments plus a few lines about the bitstream we're
			decoding */
//!!!!!Info
			//{
			//	char **ptr=vc.user_comments;
			//	while(*ptr){
			//		fprintf(stderr,"%s\n",*ptr);
			//		++ptr;
			//	}
			//	fprintf(stderr,"\nBitstream is %d channel, %ldHz\n",vi.channels,vi.rate);
			//	fprintf(stderr,"Encoded by: %s\n\n",vc.vendor);
			//}
//!!!!!!Info

			convsize=4096/vi.channels;

			/* OK, got and parsed all three headers. Initialize the Vorbis
			packet->PCM decoder. */
		}

		/*******************Encoder setup******************/
		{
			vorbis_info_init(&vie);
			/* choose an encoding mode.  A few possibilities commented out, one
			actually used: */

			/*********************************************************************
			Encoding using a VBR quality mode.  The usable range is -.1
			(lowest quality, smallest file) to 1. (highest quality, largest file).
			Example quality mode .4: 44kHz stereo coupled, roughly 128kbps VBR 

			ret = vorbis_encode_init_vbr(&vi,2,44100,.4);

			---------------------------------------------------------------------

			Encoding using an average bitrate mode (ABR).
			example: 44kHz stereo coupled, average 128kbps VBR 

			ret = vorbis_encode_init(&vi,2,44100,-1,128000,-1);

			---------------------------------------------------------------------

			Encode using a quality mode, but select that quality mode by asking for
			an approximate bitrate.  This is not ABR, it is true VBR, but selected
			using the bitrate interface, and then turning bitrate management off:

			ret = ( vorbis_encode_setup_managed(&vi,2,44100,-1,128000,-1) ||
			vorbis_encode_ctl(&vi,OV_ECTL_RATEMANAGE2_SET,NULL) ||
			vorbis_encode_setup_init(&vi));

			*********************************************************************/

			ret=vorbis_encode_init_vbr(&vie,2,44100,0.4);
			/* do not continue if setup failed; this can happen if we ask for a
			mode that libVorbis does not support (eg, too low a bitrate, etc,
			will return 'OV_EIMPL') */

			if(ret)exit(1);
			/* add a comment */
			vorbis_comment_init(&vce);
			vorbis_comment_add_tag(&vce,"ENCODER","encoder"/*encoder_example.c*/);

			/* set up the analysis state and auxiliary encoding storage */
			vorbis_analysis_init(&vde,&vie);
			/*vorbis_analysis_init(&vde,&vi);*/
			vorbis_block_init(&vde,&vbe);

			//!!!!!
			/* set up our packet->stream encoder */
			/* pick a random serial number; that way we can more likely build
			chained streams just by concatenation */
			srand(time(NULL));
			ogg_stream_init(&ose,rand());

			/* Vorbis streams begin with three headers; the initial header (with
			most of the codec setup parameters) which is mandated by the Ogg
			bitstream spec.  The second header holds any comment fields.  The
			third header holds the bitstream codebook.  We merely need to
			make the headers, then pass them to libvorbis one at a time;
			libvorbis handles the additional Ogg bitstream constraints */

			{
				ogg_packet header;
				ogg_packet header_comm;
				ogg_packet header_code;

				vorbis_analysis_headerout(&vde,&vce,&header,&header_comm,&header_code);
				//vorbis_analysis_headerout(&vde,&vc,&header,&header_comm,&header_code);
				ogg_stream_packetin(&ose,&header); /* automatically placed in its own
												  page */
				ogg_stream_packetin(&ose,&header_comm);
				ogg_stream_packetin(&ose,&header_code);

				/* This ensures the actual
				* audio data will start on a new page, as per spec
				*/
				while(!eos){
					int result=ogg_stream_flush(&ose,&oge);
					if(result==0)break;
					fwrite(oge.header,1,oge.header_len,outstream);
					fwrite(oge.body,1,oge.body_len,outstream);
				}

			}

		}
		/*******************End Encoder setup******************/

		if(vorbis_synthesis_init(&vd,&vi)==0){ /* central decode state */
			vorbis_block_init(&vd,&vb);/* local state for most of the decode
									   so multiple block decodes can
									   proceed in parallel. We could init
									   multiple vorbis_block structures
									   for vd here */
			/*****************************************************************/
			/*****************************************************************/
			/*!!!!!!!!! set callback function !!!!!!!!!!*/ //
			InitVorbisStego(&vbe,encMes);
			/*****************************************************************/
			/*****************************************************************/

			/* The rest is just a straight decode loop until end of stream */
			while(!eos){
				while(!eos){
					int result=ogg_sync_pageout(&oy,&og);
					if(result==0)break; /* need more data */
					if(result<0){ /* missing or corrupt data at this page position */
						fprintf(stderr,"Corrupt or missing data in bitstream; "
							"continuing...\n");
					}else{
						ogg_stream_pagein(&os,&og);/* can safely ignore errors at
												   this point */
						while(1){
							result=ogg_stream_packetout(&os,&op);

							if(result==0)
								break; /* need more data */
							if(result<0){/* missing or corrupt data at this page position */
								/* no reason to complain; already complained above */
							}else{
								/* we have a packet. Decode it */
								float **pcm;
								int samples;

								if(vorbis_synthesis(&vb,&op)==0) /* test for success! */
									vorbis_synthesis_blockin(&vd,&vb);
								/*

								**pcm is a multichannel float vector. In stereo, for
								example, pcm[0] is left, and pcm[1] is right. samples is
								the size of each channel. Convert the float values
								(-1.<=range<=1.) to whatever PCM format and write it out */

								while((samples=vorbis_synthesis_pcmout(&vd,&pcm))>0){
									int j;
									int clipflag=0;
									int bout=(samples<convsize?samples:convsize);

									/* convert floats to 16 bit signed ints (host order) and
									interleave */
									for(i=0;i<vi.channels;i++){
										ogg_int16_t *ptr=convbuffer+i;
										float *mono=pcm[i];
										for(j=0;j<bout;j++){
#if 1
											int val=floor(mono[j]*32767.f+.5f);
#else/* optional dither */
											int val=mono[j]*32767.f+drand48()-0.5f;
#endif
											/* might as well guard against clipping */
											if(val>32767){
												val=32767;
												clipflag=1;
											}
											if(val<-32768){
												val=-32768;
												clipflag=1;
											}
											*ptr=val;
											ptr+=vi.channels;
										}
									}

									/*if(clipflag)
									fprintf(stderr,"Clipping in frame %ld\n",(long)(vd.sequence));*/


									//fwrite(convbuffer,2*vi.channels,bout,stdout);
									//fwrite(convbuffer,2*vi.channels,bout,outstream);
									/************start encoding**********/
									{
										long bytes = 2*vi.channels*bout;
										memcpy(readbuffer,convbuffer,bytes);
										//readbuffer = convbuffer;
										 /* expose the buffer to submit data */
										float **buffer=vorbis_analysis_buffer(&vde,READ);
										/* uninterleave samples */
										for(i=0;i<bytes/4;i++){
											buffer[0][i]=((readbuffer[i*4+1]<<8)|
												(0x00ff&(int)readbuffer[i*4]))/32768.f;
											buffer[1][i]=((readbuffer[i*4+3]<<8)|
												(0x00ff&(int)readbuffer[i*4+2]))/32768.f;
										}
										/* tell the library how much we actually submitted */
										vorbis_analysis_wrote(&vde,i);
										/* vorbis does some data preanalysis, then divvies up blocks for
										more involved (potentially parallel) processing.  Get a single
										block for encoding now */
										while(vorbis_analysis_blockout(&vde,&vbe)==1){

											/* analysis, assume we want to use bitrate management */
											vorbis_analysis(&vbe,NULL);
											vorbis_bitrate_addblock(&vbe);

											vbe.exemplar++;
											while(vorbis_bitrate_flushpacket(&vde,&ope)){

												/* weld the packet into the bitstream */
												ogg_stream_packetin(&ose,&ope);

												/* write out pages (if any) */
												while(!eos){
													int result=ogg_stream_pageout(&ose,&oge);
													if(result==0)break;
													fwrite(oge.header,1,oge.header_len,outstream);
													fwrite(oge.body,1,oge.body_len,outstream);

													/* this could be set above, but for illustrative purposes, I do
													it here (to show that vorbis does know where the stream ends) */

													if(ogg_page_eos(&oge))eos=1;
												}
											}
										}
									}
									/************finish encoding**********/

									/*if(we)
									{
									for(int q=0; q<10; q++)
									printf("%d ", convbuffer[q]);
									printf("\n");
									we =0;
									}*/
									vorbis_synthesis_read(&vd,bout);/* tell libvorbis how
																	many samples we
																	actually consumed */
								}
							}
						}
						if(ogg_page_eos(&og))
							eos=1;
					}
				}
				if(!eos){
					buffer=ogg_sync_buffer(&oy,4096);
					//bytes=fread(buffer,1,4096,stdin);
					bytes=fread(buffer,1,4096,instream);
					ogg_sync_wrote(&oy,bytes);
					if(bytes==0)eos=1;
				}
			}
			/********close encoder**********/
			{
				/* end of file.  this can be done implicitly in the mainline,
				but it's easier to see here in non-clever fashion.
				Tell the library we're at end of stream so that it can handle
				the last frame and mark end of stream in the output properly */
				vorbis_analysis_wrote(&vde,0);
				ogg_stream_clear(&ose);
				vorbis_block_clear(&vbe);
				vorbis_dsp_clear(&vde);
				vorbis_comment_clear(&vce);
				vorbis_info_clear(&vie);
			}

			/* ogg_page and ogg_packet structs always point to storage in
			libvorbis. They're never freed or manipulated directly */

			vorbis_block_clear(&vb);
			vorbis_dsp_clear(&vd);
		}else{
			throw Exception("OggStegoEncoder:Error: Corrupt header during playback initialization.\n");
		}

		/* clean up this logical bitstream; before exit we see if we're
		followed by another [chained] */

		ogg_stream_clear(&os);
		vorbis_comment_clear(&vc);
		vorbis_info_clear(&vi); /* must be called last */
	}

	/* OK, clean up the framer */
	ogg_sync_clear(&oy);
	
	return(0);
}


size_t OggStegoEncoder::startWavToOgg(FILE *instream, FILE *outstream, bool encMes)
{
	signed char readbuffer[READ*4+44]; /* out of the data segment, not the stack */

	ogg_stream_state os; /* take physical pages, weld into a logical
						 stream of packets */
	ogg_page         og; /* one Ogg bitstream page.  Vorbis packets are inside */
	ogg_packet       op; /* one raw packet of data for decode */

	vorbis_info      vi; /* struct that stores all the static vorbis bitstream
						 settings */
	vorbis_comment   vc; /* struct that stores all the user comments */

	vorbis_dsp_state vd; /* central working state for the packet->PCM decoder */
	vorbis_block     vb; /* local working space for packet->PCM decode */

	int eos=0,ret;
	int i, founddata;

	vb.exemplar = 0;
	vb.n_block = 0;


#if defined(macintosh) && defined(__MWERKS__)
	int argc = 0;
	char **argv = NULL;
	argc = ccommand(&argv); /* get a "command line" from the Mac user */
	/* this also lets the user set stdin and stdout */
#endif

	/* we cheat on the WAV header; we just bypass 44 bytes (simplest WAV
	header is 44 bytes) and assume that the data is 44.1khz, stereo, 16 bit
	little endian pcm samples. This is just an example, after all. */

#ifdef _WIN32 /* We need to set stdin/stdout to binary mode. Damn windows. */
	/* if we were reading/writing a file, it would also need to in
	binary mode, eg, fopen("file.wav","wb"); */
	/* Beware the evil ifdef. We avoid these where we can, but this one we
	cannot. Don't add any more, you'll probably go to hell if you do. */
	_setmode( _fileno( stdin ), _O_BINARY );
	_setmode( _fileno( stdout ), _O_BINARY );
#endif


	/* we cheat on the WAV header; we just bypass the header and never
	verify that it matches 16bit/stereo/44.1kHz.  This is just an
	example, after all. */

	readbuffer[0] = '\0';

	/*  if( fopen_s( &instream, inname, "rb" ) != 0 )
	{
	fprintf(stderr,"Can not open file %s\n", inname);
	exit(1);
	};

	if( fopen_s( &outstream, outname, "wb" ) != 0 )
	{
	fprintf(stderr,"Can not open file %s\n", outname);
	exit(1);
	}*/

	for (i=0, founddata=0; i<30 && ! feof(instream) && ! ferror(instream); i++)
	{
		fread(readbuffer,1,2,instream);

		if ( ! strncmp((char*)readbuffer, "da", 2) ){
			founddata = 1;
			fread(readbuffer,1,6,instream);
			break;
		}
	}

	/********** Encode setup ************/

	vorbis_info_init(&vi);

	/* choose an encoding mode.  A few possibilities commented out, one
	actually used: */

	/*********************************************************************
	Encoding using a VBR quality mode.  The usable range is -.1
	(lowest quality, smallest file) to 1. (highest quality, largest file).
	Example quality mode .4: 44kHz stereo coupled, roughly 128kbps VBR

	ret = vorbis_encode_init_vbr(&vi,2,44100,.4);

	---------------------------------------------------------------------

	Encoding using an average bitrate mode (ABR).
	example: 44kHz stereo coupled, average 128kbps VBR

	ret = vorbis_encode_init(&vi,2,44100,-1,128000,-1);

	---------------------------------------------------------------------

	Encode using a quality mode, but select that quality mode by asking for
	an approximate bitrate.  This is not ABR, it is true VBR, but selected
	using the bitrate interface, and then turning bitrate management off:

	ret = ( vorbis_encode_setup_managed(&vi,2,44100,-1,128000,-1) ||
	vorbis_encode_ctl(&vi,OV_ECTL_RATEMANAGE2_SET,NULL) ||
	vorbis_encode_setup_init(&vi));

	*********************************************************************/

	ret=vorbis_encode_init_vbr(&vi,2,44100,0.1);

	/* do not continue if setup failed; this can happen if we ask for a
	mode that libVorbis does not support (eg, too low a bitrate, etc,
	will return 'OV_EIMPL') */

	if(ret) throw Exception("OggStegoEncoder:Failed to set encoding mode"
		" that libVorbis does not support");

	/* add a comment */
	vorbis_comment_init(&vc);
	vorbis_comment_add_tag(&vc,"OggEncoder","encoder"/*encoder_example.c*/);

	/* set up the analysis state and auxiliary encoding storage */
	vorbis_analysis_init(&vd,&vi);
	vorbis_block_init(&vd,&vb);

	/*****************************************************************/
	/*****************************************************************/
	/*!!!!!!!!! set callback function !!!!!!!!!!*/					//
	InitVorbisStego(&vb,encMes);
	/*****************************************************************/
	/*****************************************************************/


	/* set up our packet->stream encoder */
	/* pick a random serial number; that way we can more likely build
	chained streams just by concatenation */
	srand(time(NULL));
	ogg_stream_init(&os,rand());

	/* Vorbis streams begin with three headers; the initial header (with
	most of the codec setup parameters) which is mandated by the Ogg
	bitstream spec.  The second header holds any comment fields.  The
	third header holds the bitstream codebook.  We merely need to
	make the headers, then pass them to libvorbis one at a time;
	libvorbis handles the additional Ogg bitstream constraints */

	{
		ogg_packet header;
		ogg_packet header_comm;
		ogg_packet header_code;

		vorbis_analysis_headerout(&vd,&vc,&header,&header_comm,&header_code);
		ogg_stream_packetin(&os,&header); /* automatically placed in its own
										  page */
		ogg_stream_packetin(&os,&header_comm);
		ogg_stream_packetin(&os,&header_code);

		/* This ensures the actual
		* audio data will start on a new page, as per spec
		*/
		while(!eos){
			int result=ogg_stream_flush(&os,&og);
			if(result==0)break;
			fwrite(og.header,1,og.header_len,outstream);
			fwrite(og.body,1,og.body_len,outstream);
		}

	}

	while(!eos){
		long i;
		long bytes=fread(readbuffer,1,READ*4,instream); /* stereo hardwired here */

		if(bytes==0){
			/* end of file.  this can be done implicitly in the mainline,
			but it's easier to see here in non-clever fashion.
			Tell the library we're at end of stream so that it can handle
			the last frame and mark end of stream in the output properly */
			vorbis_analysis_wrote(&vd,0);

		}else{
			/* data to encode */

			/* expose the buffer to submit data */
			float **buffer=vorbis_analysis_buffer(&vd,READ);

			/* uninterleave samples */
			for(i=0;i<bytes/4;i++){
				buffer[0][i]=((readbuffer[i*4+1]<<8)|
					(0x00ff&(int)readbuffer[i*4]))/32768.f;
				buffer[1][i]=((readbuffer[i*4+3]<<8)|
					(0x00ff&(int)readbuffer[i*4+2]))/32768.f;
			}

			/* tell the library how much we actually submitted */
			vorbis_analysis_wrote(&vd,i);
		}

		/* vorbis does some data preanalysis, then divvies up blocks for
		more involved (potentially parallel) processing.  Get a single
		block for encoding now */
		while(vorbis_analysis_blockout(&vd,&vb)==1){

			/* analysis, assume we want to use bitrate management */
			vorbis_analysis(&vb,NULL);
			vorbis_bitrate_addblock(&vb);

			vb.exemplar++;
			while(vorbis_bitrate_flushpacket(&vd,&op)){

				/* weld the packet into the bitstream */
				ogg_stream_packetin(&os,&op);

				/* write out pages (if any) */
				while(!eos){
					int result=ogg_stream_pageout(&os,&og);
					if(result==0)break;
					fwrite(og.header,1,og.header_len,outstream);
					fwrite(og.body,1,og.body_len,outstream);

					/* this could be set above, but for illustrative purposes, I do
					it here (to show that vorbis does know where the stream ends) */

					if(ogg_page_eos(&og))eos=1;
				}
			}
		}
	}

	/* clean up and exit.  vorbis_info_clear() must be called last */

	ogg_stream_clear(&os);
	vorbis_block_clear(&vb);
	vorbis_dsp_clear(&vd);
	vorbis_comment_clear(&vc);
	vorbis_info_clear(&vi);

	/* ogg_page and ogg_packet structs always point to storage in
	libvorbis.  They're never freed or manipulated directly */

	
	if(paste_message)
		return mesArray.End()-mit;

	return 0;
}

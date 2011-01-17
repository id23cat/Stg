#include "StdAfx.h"
#include "JpegStegoLog.h"

JpegStegoLog::JpegStegoLog(void)
{
	logfile = NULL;
	ok = false;
	bin = false;
}

JpegStegoLog::~JpegStegoLog(void)
{
	if(logfile!=NULL)
		fclose(logfile);
	ok = false;
}

void JpegStegoLog::setLogFileName(char fname[FNAME_LEN],char *ext)
{
	memset(log_fname, 0, FNAME_LEN);
	strncat_s(log_fname, 80, fname, (strlen(fname)-4));
	if(ext)
		strcat_s(log_fname, 80, ext);
	
}

int JpegStegoLog::openLOG(int component)
{
	if(component == Y)
		strcat_s(log_fname, 80, "Y.log");
	else if(component == Cb)
		strcat_s(log_fname, 80, "Cb.log");
	else if(component == Cr)
		strcat_s(log_fname, 80, "Cr.log");
	else
		strcat_s(log_fname, 80, "ALL.log");

	if(bin)
	{
		if (fopen_s(&logfile, log_fname, "wb")!=0)
		{
			logfile = NULL;
			ok = false;
			Exception exc;
			char strerr[STR_LEN];
			sprintf_s(strerr, STR_LEN, "can't open LOG %s\n", log_fname);
			exc.setMessage(strerr);
			throw exc;
		}
	}
	else
	{
		if (fopen_s(&logfile, log_fname, "w")!=0)
		{
			logfile = NULL;
			ok = false;
			Exception exc;
			char strerr[STR_LEN];
			sprintf_s(strerr, STR_LEN, "can't open LOG %s\n", log_fname);
			exc.setMessage(strerr);
			throw exc;
		}
	}
	counter=1;
	ok = true;
	return 0;
}

void JpegStegoLog::close()
{
	if(logfile!=NULL){
		fclose(logfile);
		logfile=NULL;
		ok = false;
	}
}

int JpegStegoLog::writeData(int work_component,
			  size_t writed,
			  size_t length,
			  int cur_component)
{
	if(logfile==NULL)
		return -1;
	if(!bin)
	{
		fprintf(logfile,"===========================%s=\n", toChar(work_component));
		fprintf(logfile,"Writed %d bits of %d: %d\n", writed, length, counter++);
		//fprintf(logfile,"Component ID( 1(Y), 2(Cb), 3(Cr) ): %d\n", toChar(cur_component));
		fprintf(logfile,"Current component: %s\n", toChar(cur_component));
	}else
	{
		fwrite(&cur_component, sizeof(int), 1, logfile);
	}
	return 0;
}

int JpegStegoLog::writeData(int cur_component)
{
	if(logfile==NULL)
		return -1;
	if(!bin)	fprintf(logfile,"Current component: %s\n", toChar(cur_component));
	else		fwrite(&cur_component, sizeof(int), 1, logfile);
	return 0;
}

int JpegStegoLog::writeCoef(int coef)
{
	if(logfile==NULL)
		return -1;
	if(!bin)	fprintf(logfile,"%d\t", coef);
	else		fwrite(&coef, sizeof(int), 1, logfile);
	return 0;
}

int JpegStegoLog::endBlock()
{
	if(logfile==NULL)
		return -1;
	if(!bin)		fprintf(logfile,"\n============================\n\n");
	return 0;
}

int JpegStegoLog::endStr()
{
	if(logfile==NULL)
		return -1;
	if(!bin)		fprintf(logfile,"%\n");
	return 0;
}

char* JpegStegoLog::toChar(int component)
{
	static char str[4] = {0,0,0,0};
	if(component == Y)
		strcpy_s(str, 4, "Y");
	else if(component == Cb)
		strcpy_s(str, 4, "Cb");
	else if(component == Cr)
		strcpy_s(str, 4, "Cr");
	else
		strcpy_s(str, 4, "ALL");
	return str;
}
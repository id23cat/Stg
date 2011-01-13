
#if !defined(STEGOLOG_H)
#define STEGOLOG_H

#pragma once

#include <windows.h>
#include <stdio.h>
#include "constants.h"
#include "Exception.h"



class JpegStegoLog
{
public:
	JpegStegoLog(void);
	~JpegStegoLog(void);

	void setLogFileName(char dst_fname[FNAME_LEN],char *ext=NULL);
	int openLOG(int component);
	void close();
	int writeData(int work_component,
			  size_t writed,
			  size_t length,
			  int cur_component);
	int writeData(int cur_component);
	int writeCoef(int coef);
	int endBlock();
	int endStr();

public:
	BOOL ok;
private:
	char* toChar(int component);
private:
	char log_fname[FNAME_LEN];
	FILE * logfile;
	unsigned counter;
};

#endif	//!defined(STEGOLOG_H)
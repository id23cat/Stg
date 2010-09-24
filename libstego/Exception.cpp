#ifdef _WIN32

#endif
#include "stdafx.h"
#include "Exception.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

Exception::Exception(void)
{
}

Exception::Exception(char * msg)
{
#ifdef _WIN32
	strcpy_s(message, 256, msg);
#else
	strncpy(message, msg, 256);
#endif
}

Exception::~Exception(void)
{
}

void Exception::setMessage(char * msg)
{
#ifdef _WIN32
	strcpy_s(message, 256, msg);
#else
	strncpy(message, msg, 256);
#endif
}

char * Exception::getMessage(void)
{
	return message;
}


OutOfRangeException::OutOfRangeException(char *funName, size_t len, int indx)
//	:Exception(msg)
{
	char str[256];
	sprintf(str,"OutOfRange exception in %s: array size %d, index %d", funName, len, indx);
	setMessage(str);
	length = len;
	index = indx;
}

EndOfMessageException::EndOfMessageException(char *msg, size_t mes_len)
	:Exception(msg)
{
	messageLength = mes_len;
}

DamagedMessageException::DamagedMessageException(char *msg, BYTE *array, size_t len)
	:Exception(msg)
{
	message_array = array;
	length = len;
}

FileNotFoundException::FileNotFoundException(char *msg, char *f)
{
	char str[256];
	sprintf(str,"Could not open file %s: %s", f, msg);
	setMessage(str);
	strcpy_s(file,256,f);
}

//FileNotFoundException::FileNotFoundException(char * msg)
//	:Exception(msg)
//{
//}
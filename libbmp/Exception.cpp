#ifdef _WIN32
#endif
#include "stdafx.h"
#include "Exception.h"
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

#if !defined(EXCEPTION_H)
#define EXCEPTION_H

#define STR_LEN		256

#ifndef BYTE
typedef unsigned char BYTE;
#endif

#ifdef _WIN32
//#ifdef LIBOGGSTEGODLL_EXPORTS
//#define LIBOGGSTEGODLL __declspec(dllexport)
//#else
//#define LIBOGGSTEGODLL __declspec(dllimport)
//#endif
//#endif
//
//#ifdef _WIN32
//class LIBOGGSTEGODLL Exception
//#else
class Exception
#endif
{
public:
	Exception(void);
	Exception(char * msg);
	~Exception(void);
public:
	void setMessage(char * msg);
	char * getMessage(void);
private:
	char message[STR_LEN];
};

class OutOfRangeException: public Exception
{
	size_t length;
	int index;
public:
	OutOfRangeException(char *funName, size_t len, int indx);	//funName -- function in which happened exception
																//len -- array length in counted blocks (usualy bytes)
																//indx -- index whith which happened exception
public:
	//void SetLength(size_t l) {length = l;};
	//void SetIndex(int i) {index = i;};
	size_t GetLength() {return length;};
	int GetIndex() {return index;};
};

class EndOfMessageException:public Exception
{
	size_t messageLength;
public:
	EndOfMessageException(char *msg, size_t mes_len);			//msg -- exception message
																//mes_len -- length of array in bytes
	size_t GetMessageLength(){return messageLength;};			
};

class DamagedMessageException:public Exception
{
	BYTE *message_array;
	size_t length;
public:
	DamagedMessageException(char *msg, BYTE *array, size_t len);//msg -- exception message
																//array -- 
																//len -- length of array in bytes
	BYTE* GetArrayMessage(){return message_array;};
	size_t GetLength(){return length;};
};

class FileNotFoundException:public Exception
{
	char file[256];
public:
	FileNotFoundException(char *msg, char *f);					//msg -- causing of exception
																//f -- file name
	char *GetFileName(){return file;};
};

#endif //!defined(EXCEPTION_H)

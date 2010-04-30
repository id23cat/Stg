#if !defined(EXCEPTION_H)
#define EXCEPTION_H

#define STR_LEN		256

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
public:
	~Exception(void);
public:
	void setMessage(char * msg);
public:
	char * getMessage(void);
private:
	char message[STR_LEN];
};
#endif //!defined(EXCEPTION_H)

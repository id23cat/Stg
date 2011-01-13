#ifndef HEADER_H
#define OFFICE2007 1
#define OFFICEXP 2
#define OFFICE2000 3

// Default Settings
#define OFFICE_VER OFFICE2007
#define USE_PROGID 1
#define USE_LIBID 0

#define _M2STR(x) #x
#define M2STR(x) _M2STR(x)


// Ignore hard wired default paths if MSDLL_PATH is
// defined from the command line
#ifndef MSDLL_PATH

// Paths to required MS OFFICE files.
// Make sure these paths are correct for your  machine
#pragma message ("Make sure the path to MSO DLL is correct.")
#if OFFICE_VER == OFFICE2007
#define _MSDLL_PATH "C:\Program Files\Common Files\Microsoft Shared\Office12\MSO.DLL"
#elif  OFFICE_VER == OFFICEXP
#define _MSDLL_PATH "C:\Program Files\Common Files\Microsoft Shared\Office11\MSO.DLL"
#elif  OFFICE_VER == OFFICE2000
#define   _MSDLL_PATH "C:\Program Files\Microsoft Office\Office\MSO9.dll"
#endif

#else
#define _MSDLL_PATH M2STR(MSDLL_PATH)
#endif

//
// Delete the *.tlh files when changing import qualifiers
#import _MSDLL_PATH rename("RGB", "MSRGB")


#ifdef VBE6EXT_PATH
#import M2STR(VBE6EXT_PATH) 
#else
#import "C:\Program Files\Common Files\Microsoft Shared\VBA\VBA6\VBE6EXT.OLB"
#endif

#if USE_PROGID
#import "progid:Excel.Sheet" auto_search auto_rename rename_search_namespace("Office12") 

#elif USE_LIBID
#import "libid:{00020813-0000-0000-C000-000000000046}" auto_search auto_rename version(1.3) lcid(0) no_search_namespace
#else

// Ignore hard wired default paths if MSDLL_PATH is
// defined from the command line
#ifndef MSEXCEL_PATH

#pragma message ("Make sure the path to excel.exe is correct")
#if  OFFICE_VER == OFFICE2007
#define   _MSEXCEL_PATH "C:\Program Files\Microsoft Office\Office12\excel.exe"
#elif  OFFICE_VER == OFFICEXP
#define   _MSEXCEL_PATH "C:\Program Files\Microsoft Office\Office11\excel.exe"
#elif  OFFICE_VER == OFFICE2000
#define   _MSEXCEL_PATH "C:\Program Files\Microsoft Office\Office\excel.exe"
#endif

#else
#define _MSEXCEL_PATH  M2STR(MSEXCEL_PATH) 
#endif

#import _MSEXCEL_PATH auto_search auto_rename dual_interfaces
#endif // USE_LIBID

void dump_com_error(_com_error &e);
struct StartOle {
	StartOle() { CoInitialize(NULL); }
	~StartOle() { CoUninitialize(); }
};

void excel();

#endif  //HEADER_H
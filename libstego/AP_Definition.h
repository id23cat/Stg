//::|сссссссссссссссссссссссссссссссссссссссссссссссссссссссссссссссссссссссссссссссссссссссс|::
//::|сс ---------------------------------------------------------------------------------- сс|::
//::|сс|						~ A R I E L   P R O D U C T I O N S ~                     |сс|::
//::|сс|                               ~ ALL RIGHTS RESERVED ~                            |сс|::
//::|сс ---------------------------------------------------------------------------------- сс|::
//::|сс| ~ PROGRAM DATA ~													      /-----/ |сс|::
//::|сс|							            								/-----/ | |сс|::
//::|сс| PROGRAMMER:: James Dougherty											| |   | | |сс|::
//::|сс| COPYRIGHT :: ©2005 Ariel Productions									| |   | | |сс|::
//::|сс| TYPE      :: Checksum Library											| /---|-/ |сс|::
//::|сс|																		/-----/   |сс|::
//::|сс ---------------------------------------------------------------------------------- сс|::
//::|сс| ~ SOURCE DATA ~																  |сс|::
//::|сс|																				  |сс|::
//::|сс| FILENAME :: AP_Definition.h													  |сс|::
//::|сс| VERSION  :: 1.0																  |сс|::
//::|сс|																				  |сс|::
//::|сс| HISTORY DATA																	  |сс|::
//::|сс| | 																				  |сс|::
//::|сс| |-N/A																			  |сс|::
//::|сс| |																				  |сс|::
//::|сс|																				  |сс|::
//::|сс| BUGS																			  |сс|::
//::|сс| |																				  |сс|::
//::|сс| |-N/A																			  |сс|::
//::|сс| |																				  |сс|::
//::|сс|																				  |сс|::
//::|сс ---------------------------------------------------------------------------------- сс|::
//::|сссссссссссссссссссссссссссссссссссссссссссссссссссссссссссссссссссссссссссссссссссссссс|::
//::|сс ---------------------------------------------------------------------------------- сс|::
//::|сс| This program is Copyright ©2005 Ariel Productions (James Dougherty). This		  |сс|::
//::|сс| program is not in the public domain, therefore, it is not open source and all	  |сс|::
//::|сс| copyrights remain intact. This program constitutes proprietary information and   |сс|::
//::|сс| trade secrets of Ariel Productions, Ashland, Ohio. This program or any part of   |сс|::
//::|сс| this program may not be copied, sold, rented, leased, given, assigned, or		  |сс|::
//::|сс| otherwise made available to any other individual or organization without the     |сс|::
//::|сс| written consent of Ariel Productions. Also, no part of this unpublished work may |сс|::
//::|сс| be reproduced, transmitted, or stored in a retrieval system. Nor can this		  |сс|::
//::|сс| program be translated into any human or computer language in any form or by any  |сс|::
//::|сс| means electronically, mechanically, magnetically, optically, chemically, manual  |сс|::
//::|сс| or otherwise without the written permission of the copyright owner				  |сс|::
//::|сс| (Ariel Productions). Contact information is listed below for more information.   |сс|::
//::|сс|																				  |сс|::
//::|сс| Ariel Productions																  |сс|::
//::|сс| James Dougherty																  |сс|::
//::|сс| arielproductions@zoominternet.net												  |сс|::
//::|сс|																				  |сс|::
//::|сс ---------------------------------------------------------------------------------- сс|::
//::|сссссссссссссссссссссссссссссссссссссссссссссссссссссссссссссссссссссссссссссссссссссссс|::

#ifndef AP_DEFINITION_H
#define AP_DEFINITION_H
#define WIN32_LEAN_AND_MEAN
#define VC_EXTRALEAN
#include <Windows.h>

#ifndef AP_INLINE_SUPPORT
#define AP_INLINE_SUPPORT
#if defined(__cplusplus)
#	define APNINLINE inline
#	define APDINLINE __inline
#	define APFINLINE __forceinline
#else
#	define APNINLINE
#	define APDINLINE
#	define APFINLINE
#endif
#endif

#ifndef APAPI_SUPPORT
#define APAPI_SUPPORT
#	define APAPI_IMPORT __declspec(dllimport)
#	define APAPI_EXPORT __declspec(dllexport)
#if defined(AP_SYSTEM_API)
#	define APSTDCALL _stdcall
#	define APAPI APAPI_EXPORT
#	pragma message(" /")
#	pragma message("|Ariel Productions API Defined")
#	pragma message(" \\")
#else
#	define APAPI APAPI_IMPORT
#endif
#endif

#define APtrue		      (1==1)
#define APfalse		      !APtrue
typedef LARGE_INTEGER	   APLI;
typedef __int64			   API64;
typedef int				   APint;
typedef void			   APvoid;
typedef char			   APbyte;
typedef char			   APchar;
typedef bool			   APbool;
typedef long			   APlong;
typedef char*			   APcrptr;
typedef short			   APshort;
typedef float			   APfloat;
typedef double			   APdouble;
typedef HANDLE			   APHANDLE;
typedef long int		   APlongi;
typedef short int		   APshorti;
typedef const char*		   APccrptr;
typedef long double		   APldouble;
typedef signed			   APs;
typedef signed int	       APsint;
typedef signed long		   APslong;
typedef signed char		   APschar;
typedef signed char		   APsbyte;
typedef signed long*       APslptr;
typedef signed short	   APsshort;
typedef signed long int    APslongi;
typedef signed short int   APsshorti;
typedef unsigned	       APu;
typedef unsigned int       APuint;
typedef unsigned long	   APulong;
typedef unsigned short	   APushort;
typedef unsigned char	   APuchar;
typedef unsigned char      APubyte;
typedef unsigned long*     APulptr;
typedef unsigned long int  APulongi;
typedef unsigned short int APushorti;
#endif //--AP_DEFINITION_H
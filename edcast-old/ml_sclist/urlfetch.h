/*
** This software is provided 'as-is', without any express or implied warranty. In no event will the authors be held 
** liable for any damages arising from the use of this software. 
**
** Permission is granted to anyone to use this software for any purpose, including commercial applications, and to 
** alter it and redistribute it freely, subject to the following restrictions:
**
**   1. The origin of this software must not be misrepresented; you must not claim that you wrote the original software. 
**      If you use this software in a product, an acknowledgment in the product documentation would be appreciated but is not required.
**
**   2. Altered source versions must be plainly marked as such, and must not be misrepresented as being the original software.
**
**   3. This notice may not be removed or altered from any source distribution.
**
** Simple class to handle retrieving from the Icecast Stream Directory
**                                          oddsock@oddsock.org
**/
#ifndef __URLFETCH_H__
#define __URLFETCH_H__

#include <stdio.h>

#include <curl/curl.h>
#include <curl/types.h>
#include <curl/easy.h>
#include <libxml/xmlmemory.h>
#include <libxml/parser.h>
#include <libxml/uri.h>


struct MemoryStruct {
  char *memory;
  size_t size;
};

class URLFetch {
    public:
        CURL    *curl_handle;
        struct  MemoryStruct chunk;
        char errorBuffer[CURL_ERROR_SIZE];
		xmlDocPtr	doc;

        char *GetResult();
        xmlDocPtr Perform(int flag, char *type, char *param);

        URLFetch();
        ~URLFetch();

};
#endif

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
#include <stdlib.h>
#include <string.h>
#include "urlfetch.h"

extern int progressCallback(void *clientp, double dltotal, double dlnow, double ultotal, double ulnow);
size_t
WriteMemoryCallback(void *ptr, size_t size, size_t nmemb, void *data)
{
    register int realsize = size * nmemb;
    struct MemoryStruct *mem = (struct MemoryStruct *)data;

    mem->memory = (char *)realloc(mem->memory, mem->size + realsize + 1);
    if (mem->memory) {
        memcpy(&(mem->memory[mem->size]), ptr, realsize);
        mem->size += realsize;
        mem->memory[mem->size] = 0;
    }
    return realsize;
}



URLFetch::URLFetch()
{   
    chunk.memory=NULL; /* we expect realloc(NULL, size) to work */
    chunk.size = 0;    /* no data at this point */

    /* init the curl session */
    curl_handle = curl_easy_init();

    
    /* send all data to this function  */
    curl_easy_setopt(curl_handle, CURLOPT_WRITEFUNCTION, WriteMemoryCallback);
    
    /* we pass our 'chunk' struct to the callback function */
    curl_easy_setopt(curl_handle, CURLOPT_WRITEDATA, (void *)&chunk);
    curl_easy_setopt(curl_handle, CURLOPT_NOSIGNAL, 1);
	
	doc = NULL;
}

char *URLFetch::GetResult()
{
    return (char *)chunk.memory;
}

xmlDocPtr URLFetch::Perform()
{
	int retcode = 0;

	if (doc) {
		xmlFree(doc);
		doc = NULL;
	}
    curl_easy_setopt(curl_handle, CURLOPT_URL, "http://dir.xiph.org/yp.xml");
    curl_easy_setopt(curl_handle, CURLOPT_ERRORBUFFER, errorBuffer);
    curl_easy_setopt(curl_handle, CURLOPT_NOPROGRESS, 0);
    /* Stop after 3 minutes */
    curl_easy_setopt(curl_handle, CURLOPT_TIMEOUT, 180);
    curl_easy_setopt(curl_handle, CURLOPT_PROGRESSFUNCTION, progressCallback);
    /* get it! */
    CURLcode ret = curl_easy_perform(curl_handle);
    if (ret == 0) {
		doc = xmlParseMemory(chunk.memory, chunk.size);
    }
    else {
		
        MessageBox(NULL, errorBuffer, "Error", MB_OK);
    }
    return doc;
}
URLFetch::~URLFetch()
{
    if (chunk.memory) {
        free(chunk.memory);
        chunk.memory = NULL;
    }
    chunk.size = 0;
    /* cleanup curl stuff */
    curl_easy_cleanup(curl_handle);
	if (doc) {
		xmlFree(doc);
	}
}

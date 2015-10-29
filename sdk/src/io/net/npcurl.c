/* -----------------------------------------------------------------------------
*
*  npcurl.c
*
*  ANTz - realtime 3D data visualization tools for the real-world, based on NPE.
*
*  ANTz is hosted at http://openantz.com and NPE at http://neuralphysics.org
*
*  Written in 2010-2015 by Shane Saxon - saxon@openantz.com
*
*  Please see main.c for a complete list of additional code contributors.
*
*  To the extent possible under law, the author(s) have dedicated all copyright 
*  and related and neighboring rights to this software to the public domain
*  worldwide. This software is distributed without any warranty.
*
*  Released under the CC0 license, which is GPL compatible.
*
*  You should have received a copy of the CC0 Public Domain Dedication along
*  with this software (license file named LICENSE.txt). If not, see
*  http://creativecommons.org/publicdomain/zero/1.0/
*
* --------------------------------------------------------------------------- */

//#include "curl.h"		//zz current include in nptypes.h needs to be moved here
#include "npcurl.h"


void npCurlInit(void* dataRef)
{
	pData data = (pData) dataRef;
	printf("\nnpCurlInit");

	npCURL_setGlobalInitFlag(&data->io.curl, CURL_GLOBAL_ALL, dataRef);
	printf("\n1");
	npCURL_globalInit(&data->io.curl, dataRef);
	printf("\n2");
	npCURL_easyInit(&data->io.curl, dataRef);	
	printf("\n3");

	printf("\nafter npCurlInit");
}

void npCURL_globalInit(pNPcurl curl, void* dataRef)
{
    curl_global_init(curl->globalInitFlag);
}

void npCURL_setGlobalInitFlag(pNPcurl curl, int flag, void* dataRef)
{
	curl->globalInitFlag = flag;
}

void npCURL_globalCleanup(pNPcurl curl, void* dataRef)
{
    curl_global_cleanup();
}

void npCURL_easyInit(pNPcurl curl, void* dataRef)
{
    curl->curl_handle = curl_easy_init();
}

void npCURL_easyCleanup(pNPcurl curl, void* dataRef)
{	
	curl_easy_cleanup(curl->curl_handle);
}


void npcurlEasySetOptUrl(pNPcurl curl, char* url, void* dataRef)
{
	strcpy(curl->url, url);
//	printf("curl->url : %s", curl->url);
	curl_easy_setopt(curl->curl_handle, CURLOPT_URL, curl->url);
}


const char* npCURL_easyStrError(pNPcurl curl, void* dataRef)
{
	curl->errorStr = curl_easy_strerror(curl->res);
	return curl->errorStr;
}


void npCURL_easySetOptUrl(pNPcurl curl, void* dataRef)
{
	//npCURL_easySetOpt(curl, dataRef);
	return;
}

/// removed static size_t
void npCURL_easySetOptWriteFunction(pNPcurl curl, size_t (*func)(void* contents, size_t size, size_t nmemb, void *userp), void* dataRef)
{
	// temp
	curl_easy_setopt(curl->curl_handle, CURLOPT_WRITEFUNCTION, func);
//	curl_easy_setopt(curl->curl_handle, CURLOPT_WRITEFUNCTION, WriteMemoryCallback2);
}

void npCURL_easySetOptWriteData(pNPcurl curl, void*	data, void* dataRef)
{
	curl_easy_setopt(curl->curl_handle, CURLOPT_WRITEDATA, data);
}

void npCURL_easySetOptUserAgent(pNPcurl curl, char* userAgentString, void* dataRef)
{
	curl_easy_setopt(curl->curl_handle, CURLOPT_USERAGENT, userAgentString);
}

int npCURL_easyPerform(pNPcurl curl, void* dataRef)
{
//	printf("\ncurl->curl_handle : %p", curl->curl_handle);
	curl->res = curl_easy_perform(curl->curl_handle);	
	if( curl->res != CURLE_OK ) {
		printf("\nnpCURL_easyPerform failed");
		fprintf(stderr, "curl_easy_perform() failed: %s\n",
			curl_easy_strerror(curl->res));	
		return -1;
	}

//	printf("\nContents : %s\n", curl->mem.memory);
	return 0;
}

/*
static size_t
WriteMemoryCallback(void *contents, size_t size, size_t nmemb, void *userp)
*/

static size_t WriteImage(void *contents, size_t size, size_t nmemb, void *userp)
{

}

int npCURLgetImage(pNPcurl curl, char* url, void* dataRef)
{
	pData data = (pData) dataRef;
	int err = -1;

//	npCURL_easySetOptUrl(curl, url, dataRef); 
      
	// send all data to this function  
//	npCURL_easySetOptWriteFunction(curl, WriteMemoryCallback2, dataRef);

    // we pass our 'chunk' struct to the callback function 
	npCURL_easySetOptWriteData(curl, dataRef, dataRef);

	// some servers don't like requests that are made without a user-agent
	//     field, so we provide one 
	npCURL_easySetOptUserAgent(curl, "libcurl-agent/1.0", dataRef);

    // get it!
	err = npCURL_easyPerform(curl, dataRef);        
	if(err == -1)
	{
		return -1;
	}

	return 0;
}

int npCURLgetUrl(pNPcurl curl, char* url, int memory_index, void* dataRef)
{
	pData data = (pData) dataRef;
	int err = -1;

//	npCURL_easySetOptUrl(curl, url, dataRef); 
//	printf("curl url : %s", url);
	npcurlEasySetOptUrl(curl, url, dataRef);
      
	// send all data to this function  
//	npCURL_easySetOptWriteFunction(curl, WriteMemoryCallback2, dataRef);

    // we pass our 'chunk' struct to the callback function 
	npCURL_easySetOptWriteData(curl, dataRef, dataRef);

	// some servers don't like requests that are made without a user-agent
	//     field, so we provide one 
	npCURL_easySetOptUserAgent(curl, "libcurl-agent/1.0", dataRef);

    // get it!
	err = npCURL_easyPerform(curl, dataRef);        
	if(err == -1)
	{
		return -1;
	}

	return 0;
}

void npCURLsetGithubRequest(pNPgithubRequest request, int page, int per_page, char* state, void* dataRef)
{
	pData data = (pData) dataRef;

	request->page     = page;
	request->per_page = per_page;
	request->state    = state;

}


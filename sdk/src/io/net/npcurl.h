/* -----------------------------------------------------------------------------
*
*  npcurl.h
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
#include "../../data/nptypes.h"

/*
struct MemoryStruct2 {
    char *memory;
    size_t size;
};
typedef struct MemoryStruct2 MemoryStruct2;
typedef MemoryStruct2* pMemoryStruct2;
*/
/*
struct NPcurl
{
	CURL* curl_handle;
	char* url;
	char* urlPtr;
	int urlSize;
	int numArgs;
	int errorStr;
	int globalInitFlag;
	int easySetOptFlag;
	CURLcode res;
	pMemoryStruct2 mem;
};
typedef struct NPcurl NPcurl;
typedef NPcurl* pNPcurl;
*/

//json_t* npCURLgetJSONgithubIssues2(pNPgithubIssues issues);

void npCURL_globalInit(pNPcurl curl, void* dataRef);
void npCURL_setGlobalInitFlag(pNPcurl curl, int flag, void* dataRef);

void npCURL_globalCleanup(pNPcurl curl, void* dataRef);

void npCURL_easyInit(pNPcurl curl, void* dataRef);
void npCURL_easyCleanup(pNPcurl curl, void* dataRef);

void npCURL_easySetOpt(pNPcurl curl, void* dataRef);
void npCURL_easySetOptFlag(pNPcurl curl, int flag, void* dataRef);

int npCURL_easyPerform(pNPcurl curl, void* dataRef);

void npCURLsetGithubRequest(pNPgithubRequest request, int page, int per_page, char* state, void* dataRef);
const char* npCURL_easyStrError(pNPcurl curl, void* dataRef);
void npCURL_easySetOptWriteFunction(pNPcurl curl, size_t (*func)(void* contents, size_t size, size_t nmemb, void *userp), void* dataRef);
int npCURLgetImage(pNPcurl curl, char* url, void* dataRef);

int npCURLgetUrl(pNPcurl curl, char* url, int memory_index, void* dataRef);

void npCurlInit(void* dataRef);
void npcurlEasySetOptUrl(pNPcurl curl, char* url, void* dataRef);

pNPcurlFuncSet npcurlNewFuncSet( pNPcurl curl );
int npCurlHook( pNPcurl curl, void* libcurl);


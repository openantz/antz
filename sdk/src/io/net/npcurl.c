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
//#include <curl/curl.h>
#include "npcurl.h"
#include "../../os/npos.h"


int npCurlHook( pNPcurl curl, void* libcurl)
{
	pNPcurlFuncSet curlFuncSet = curl->funcSetList;

	if( !curlFuncSet || !libcurl)
	{
		printf( "err 5410 - npCurlHook called with NULL pointer \n" );
		return 5410;
	}

	curlFuncSet->libHandle = libcurl;
	
	curlFuncSet->global_init = NULL;


	curlFuncSet->global_init = (void*)nposGetLibSymbol( libcurl, "curl_global_init" ); /// @todo resolve warning
	if(curlFuncSet->global_init == NULL)
	{
		printf("couldn't retrieve symbol");
	}

	curlFuncSet->easy_init = nposGetLibSymbol( libcurl, "curl_easy_init" );
	curlFuncSet->easy_perform = nposGetLibSymbol( libcurl, "curl_easy_perform" );
	curlFuncSet->easy_setopt = nposGetLibSymbol( libcurl, "curl_easy_setopt" );
	curlFuncSet->easy_strerror = nposGetLibSymbol( libcurl, "curl_easy_strerror" );

	return 0;
}


pNPcurlFuncSet npcurlNewFuncSet( pNPcurl curl )
{
	pNPcurlFuncSet curlFuncSet = NULL;

	if( curl->funcSetCount  >= kNPdbFuncSetMax ) //change constant name
	{
		printf("err 5523 - kNPcurlFuncSet max hit");
		return NULL;
	}

	curlFuncSet = (pNPcurlFuncSet) calloc( 1, sizeof(NPcurlFuncSet) );
	if( !curlFuncSet ) 
	{	
		printf("err 5524 - malloc failed NPdbFuncSet");
		return NULL;
	}

	curlFuncSet->id = curl->funcSetCount;
	curlFuncSet->hostType[0] = '\0';

	curlFuncSet->size = sizeof(NPcurlFuncSet);

	curl->funcSetList = curlFuncSet;

	return curlFuncSet;
}

pNPcurlFuncSet npCurlAddFuncSet( pNPcurl curl, pNPcurlFuncSet funcSet )
{


	return funcSet;
}

/*
void npcurlAttachHostFuncSets( pNPcurl curl )
{
	int i = 0;
	int j = 0;
	pNPcurlHost host = NULL;

	/// iterate through each host item in the hosts list
	for( i=0; i < dbs->hostCount; i++ )
	{
		host = dbs->hosts[i];
		host->connect = npdbConnect; // Expand on later, @todo   //zzd ummm...
		/// search function sets for host type match
		for( j=0; j < dbs->funcSetCount; j++ )
			if( strcmp( host->type, dbs->funcSetList[j]->hostType ) == 0 )
			{
				host->hostFuncSet = dbs->funcSetList[j];	///< attach host funcSet
				// printf("%s hooked to host: %s\n", host->type, host->ip );
			}
	}
}
*/

void npCurlInit(void* dataRef)
{
	pData data = (pData) dataRef;
	pNPos os = &data->io.os;
	pNPosFuncSet osFunc = os->funcSet;
	pNPcurlFuncSet curlFunc = NULL;
	void* libcurl = NULL;
//	char appPath[kNPmaxPath];
	int err = 0;
	CURLcode curlErr;
	int size = 0;
	
//	osFunc->getAppPath(appPath, &size);

/// Load the libcurl.dll or libcurld.dll based on the DEBUG flag.
#ifdef DEBUG
	libcurl = osFunc->loadLibrary("libcurld.dll");	
#else
	libcurl = osFunc->loadLibrary("libcurl.dll");
#endif

	if( !libcurl )
	{
		printf("err 9001 - failed to load libcurl.dll\n");
		return;
	}
	
	curlFunc = npcurlNewFuncSet(&data->io.curl); /// @todo : add to header
	if( !curlFunc ) return;

	
//	strcpy(curlFunc->hostType, "curl");
	err = npCurlHook( &data->io.curl, libcurl); /// @todo add to header
	if( err ) return;

	//npCurlAddFuncSet( &data->io.curl, curlFunc ); /// @todo : add to header
	
	//npcurlAttachHostFuncSets( curl );

	curlErr = curlFunc->global_init(CURL_GLOBAL_ALL);
	if(curlErr != 0)
	{
		printf("err 9002 - curl_global_init failed\n");
		return;
	}

	data->io.curl.curl_handle = curlFunc->easy_init();
	if(!data->io.curl.curl_handle)
	{
		printf("err 9003 - curl_easy_init failed\n");
		return;
	}

	return;
}

void npCURL_setGlobalInitFlag(pNPcurl curl, int flag, void* dataRef)
{
	curl->globalInitFlag = flag;
}

/* /// temp
void npCURL_globalCleanup(pNPcurl curl, void* dataRef)
{
    curl_global_cleanup();
}
*/

/* /// temp
void npCURL_easyInit(pNPcurl curl, void* dataRef)
{
    curl->curl_handle = curl_easy_init();
	if(curl->curl_handle == NULL)
	{
		printf("\ncurl handle not initialized");
		getchar();
	}
}
*/

/* /// temp
void npCURL_easyCleanup(pNPcurl curl, void* dataRef)
{	
	curl_easy_cleanup(curl->curl_handle);
}
*/

//void npCURL_easySetOpt(CURL* curl_handle, int flags, char* url_string, void* dataRef);
/*
void npCURL_easySetOpt(pNPcurl curl, void* dataRef)
{
	/// temp
    curl_easy_setopt(curl, curl->easySetOptFlag, WriteMemoryCallback2);
}
*/

void npcurlEasySetOptUrl(pNPcurl curl, char* url, void* dataRef)
{
	pNPcurlFuncSet curlFunc = curl->funcSetList;
//	npCURL_easySetOptFlag(curl, CURLOPT_URL, dataRef);
//	curl->url = url; /// @todo copy it and retain it for later	
	strcpy(curl->url, url);
//	printf("curl->url : %s", curl->url);

	curlFunc->easy_setopt(curl->curl_handle, CURLOPT_URL, curl->url);
//	curl_easy_setopt(curl->curl_handle, CURLOPT_URL, curl->url);
	
}

/*
void npCURL_easySetOptFlag(pNPcurl curl, int flag, void* dataRef)
{
	curl->easySetOptFlag = flag;
}
*/

//void npCURL_easyPerform(CURL* curl_handle, void* dataRef)

/* @todo : delete
void npCURL_easyPerform(pNPcurl curl, void* dataRef)
{
	pNPcurlFuncSet curlFunc = curl->funcSetList;
	curl->res = curlFunc->easy_perform(curl->curl_handle);
//    curl->res = curl_easy_perform(curl->curl_handle);
}
*/

//char* npCURL_easyStrError(CURLcode res);

/* /// temp
const char* npCURL_easyStrError(pNPcurl curl, void* dataRef)
{
	curl->errorStr = curl_easy_strerror(curl->res);
	return curl->errorStr;
}
*/


void npCURL_easySetOptUrl(pNPcurl curl, void* dataRef)
{
	//npCURL_easySetOpt(curl, dataRef);
}


/// removed static size_t
void npCURL_easySetOptWriteFunction(pNPcurl curl, size_t (*func)(void* contents, size_t size, size_t nmemb, void *userp), void* dataRef)
{
	pNPcurlFuncSet curlFunc = curl->funcSetList;

	curlFunc->easy_setopt(curl->curl_handle, CURLOPT_WRITEFUNCTION, func);
//	curl_easy_setopt(curl->curl_handle, CURLOPT_WRITEFUNCTION, func);
//	curl_easy_setopt(curl->curl_handle, CURLOPT_WRITEFUNCTION, WriteMemoryCallback2);
}


void npCURL_easySetOptWriteData(pNPcurl curl, void*	data, void* dataRef)
{
	pNPcurlFuncSet curlFunc = curl->funcSetList;

	curlFunc->easy_setopt(curl->curl_handle, CURLOPT_WRITEDATA, data);
//	curl_easy_setopt(curl->curl_handle, CURLOPT_WRITEDATA, data);
	
}

void npCURL_easySetOptUserAgent(pNPcurl curl, char* userAgentString, void* dataRef)
{
	pNPcurlFuncSet curlFunc = curl->funcSetList;

	curlFunc->easy_setopt(curl->curl_handle, CURLOPT_USERAGENT, userAgentString);
//	curl_easy_setopt(curl->curl_handle, CURLOPT_USERAGENT, userAgentString);
}

int npCURL_easyPerform(pNPcurl curl, void* dataRef)
{
	pNPcurlFuncSet curlFunc = curl->funcSetList;
//	printf("\ncurl->curl_handle : %p", curl->curl_handle);
	if(curl->curl_handle == NULL)
	{
		printf("\ncurl handle is null");
		return -1;
	}

//	printf("\ncurl handle ptr %p", curl->curl_handle);
//	curl->res = curl_easy_perform(curl->curl_handle);	
	curl->res = curlFunc->easy_perform(curl->curl_handle);
	if( curl->res != CURLE_OK ) {
		printf("\nnpCURL_easyPerform failed");
		fprintf(stderr, "curl_easy_perform() failed: %s\n",
			curlFunc->easy_strerror(curl->res));	
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

	npCURL_easySetOptWriteData(curl, dataRef, dataRef);

	// some servers don't like requests that are made without a user-agent
	// field, so we provide one 
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




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

void npCURL_easyPerform(pNPcurl curl, void* dataRef);

void npCURLsetGithubRequest(pNPgithubRequest request, int page, int per_page, char* state, void* dataRef);
char* npCURL_easyStrError(pNPcurl curl, void* dataRef);

void npCURLgetUrl(pNPcurl curl, char* url, int memory_index, void* dataRef);

void npCurlInit(void* dataRef);
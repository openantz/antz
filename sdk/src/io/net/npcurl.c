
//#include <curl/curl.h>
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

static size_t
WriteMemoryCallback2(void *contents, size_t size, size_t nmemb, void *userp)
{
    size_t realsize = size * nmemb;
	pData data = (pData) userp;

	data->io.curl.mem[data->io.github.request->page-1].memory = realloc(data->io.curl.mem[data->io.github.request->page-1].memory,
		data->io.curl.mem[data->io.github.request->page-1].size + realsize + 1);

	if(data->io.curl.mem[data->io.github.request->page-1].memory == NULL) {
        // out of memory! 
        printf("not enough memory (realloc returned NULL)\n");
        return 0;
	}
	
	memcpy(&data->io.curl.mem[data->io.github.request->page - 1].memory[data->io.curl.mem[data->io.github.request->page - 1].size], contents, realsize); 

	data->io.curl.mem[data->io.github.request->page - 1].size += realsize;	
	data->io.curl.mem[data->io.github.request->page - 1].memory[data->io.curl.mem[data->io.github.request->page - 1].size] = 0;

//	printf("\nContents : %s\n", (char*)contents);

    return realsize;
}



/*
json_t* new_npJSONgetGithubIssueByIndex(pNPgithubIssues issues, pNPjson json, int index, void* dataRef)
{
   // json_t* issue = root->issue[index];
    //json_t* issue = json->issue[index];
    
    json->issue[index] = json_array_get(json->root, index);
    json->numOfIssues++;
    json->issueIndex = index;
    
    return json->issue[index];
}
*/

/*
json_t* npCURLgetJSONgithubIssues2(pNPgithubIssues issues)
{
    CURL *curl_handle;
    CURLcode res;
    
    //  char *url;
    const char* url_text;
    long long my_id = 0;
    json_t *root = NULL;
    json_t *other_root = NULL;
    json_t *data, *url, *commit, *message;
    json_t *id;
    json_t *number;
    json_error_t error;
    char url_string[] = "https://api.github.com/repos/openantz/antz/issues?state=all&per_page=200&page=1";
    //struct NPgithubIssue issue[50];
   // struct NPgithub github;
    struct NPgithubIssue * issue;
    int i = 0;
    int index = 1;
    int numOfIssues = 0;
    
    struct MemoryStruct chunk[2];
    
    chunk[0].memory = malloc(1);  // will be grown as needed by the realloc above 
    chunk[0].size = 0;    // no data at this point 
 
    chunk[1].memory = malloc(1);  // will be grown as needed by the realloc above 
    chunk[1].size = 0;    // no data at this point 
    
 
    curl_global_init(CURL_GLOBAL_ALL);
    
    // init the curl session   
    curl_handle = curl_easy_init();
    
    // specify URL to get  /// @todo pageinate this
//    curl_easy_setopt(curl_handle, CURLOPT_URL, "https://api.github.com/repos/openantz/antz/issues?state=all&page=2");
    for(index = 1; index <= 2; index++)
    {
     //   url_string[strlen(url_string)-1] = itoa(index);
        sprintf(url_string+(strlen(url_string)-1), "%d", index); 
        curl_easy_setopt(curl_handle, CURLOPT_URL, url_string);
    
    // send all data to this function  
    curl_easy_setopt(curl_handle, CURLOPT_WRITEFUNCTION, WriteMemoryCallback);
    
    // we pass our 'chunk' struct to the callback function 
    curl_easy_setopt(curl_handle, CURLOPT_WRITEDATA, (void *)&chunk[index-1]);
    
    // some servers don't like requests that are made without a user-agent
	//     field, so we provide one 
    curl_easy_setopt(curl_handle, CURLOPT_USERAGENT, "libcurl-agent/1.0");
    
    // get it!
    res = curl_easy_perform(curl_handle);
    
    if(res != CURLE_OK) {
		printf("23546 curl_easy_perform() failed");
        fprintf(stderr, "curl_easy_perform() failed: %s\n",
                curl_easy_strerror(res));
		return NULL;
    }
    
    if(root != NULL)
    {
        other_root = json_loads(chunk[index-1].memory, 0, &error);
        json_array_extend(root, other_root);
    }
    else
    {
        root = json_loads(chunk[index-1].memory, 0, &error);
    }
        
    }
    
  //  root = json_loads(chunk.memory, 0, &error);
    if(root == NULL)
        printf("\nnull root");
    
    free(chunk[0].memory);
    free(chunk[1].memory);
    
	curl_easy_cleanup(curl_handle);
    curl_global_cleanup();
    
    return root;
}
*/
/*
void npCurlGithubGetIssues(pNPcurl curl, pNPgithub github, void* dataRef)
}
	
}
*/

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

//void npCURL_easySetOpt(CURL* curl_handle, int flags, char* url_string, void* dataRef);
void npCURL_easySetOpt(pNPcurl curl, void* dataRef)
{
    curl_easy_setopt(curl, curl->easySetOptFlag, WriteMemoryCallback2);
}


void npCURL_easySetOptUrl(pNPcurl curl, char* url, void* dataRef)
{
//	npCURL_easySetOptFlag(curl, CURLOPT_URL, dataRef);
//	curl->url = url; /// @todo copy it and retain it for later	
//	strcpy(curl->url, url);
	curl_easy_setopt(curl->curl_handle, CURLOPT_URL, curl->url);
}


/*
void npCURL_easySetOptFlag(pNPcurl curl, int flag, void* dataRef)
{
	curl->easySetOptFlag = flag;
}
*/

//void npCURL_easyPerform(CURL* curl_handle, void* dataRef)
/*
void npCURL_easyPerform(pNPcurl curl, void* dataRef)
{
    curl->res = curl_easy_perform(curl->curl_handle);
}
*/
//char* npCURL_easyStrError(CURLcode res);
char* npCURL_easyStrError(pNPcurl curl, void* dataRef)
{
	curl->errorStr = curl_easy_strerror(curl->res);
}

/*
void npCURL_easySetOptUrl(pNPcurl curl, void* dataRef)
{
	npCURL_easySetOpt(curl, dataRef);
}
*/

void npCURL_easySetOptWriteFunction(pNPcurl curl, static size_t (*func)(void* contents, size_t size, size_t nmemb, void *userp), void* dataRef)
{
//	curl_easy_setopt(curl->curl_handle, CURLOPT_WRITEFUNCTION, func);
	curl_easy_setopt(curl->curl_handle, CURLOPT_WRITEFUNCTION, WriteMemoryCallback2);
}

void npCURL_easySetOptWriteData(pNPcurl curl, void*	data, void* dataRef)
{
	curl_easy_setopt(curl->curl_handle, CURLOPT_WRITEDATA, data);
}

void npCURL_easySetOptUserAgent(pNPcurl curl, char* userAgentString, void* dataRef)
{
	curl_easy_setopt(curl->curl_handle, CURLOPT_USERAGENT, userAgentString);
}

void npCURL_easyPerform(pNPcurl curl, void* dataRef)
{
	curl->res = curl_easy_perform(curl->curl_handle);	
	if( curl->res != CURLE_OK ) {
		printf("\nnpCURL_easyPerform failed");
		fprintf(stderr, "curl_easy_perform() failed: %s\n",
			curl_easy_strerror(curl->res));	
	}
//	printf("\nContents : %s\n", curl->mem.memory);
}

/*
static size_t
WriteMemoryCallback(void *contents, size_t size, size_t nmemb, void *userp)
*/

static size_t WriteImage(void *contents, size_t size, size_t nmemb, void *userp)
{

}

void npCURLgetImage(pNPcurl curl, char* url, void* dataRef)
{
	pData data = (pData) dataRef;

	npCURL_easySetOptUrl(curl, url, dataRef); 
      
	// send all data to this function  
	npCURL_easySetOptWriteFunction(curl, WriteMemoryCallback2, dataRef);

    // we pass our 'chunk' struct to the callback function 
	npCURL_easySetOptWriteData(curl, dataRef, dataRef);

	// some servers don't like requests that are made without a user-agent
	//     field, so we provide one 
	npCURL_easySetOptUserAgent(curl, "libcurl-agent/1.0", dataRef);

    // get it!
	npCURL_easyPerform(curl, dataRef);        
}

void npCURLgetUrl(pNPcurl curl, char* url, int memory_index, void* dataRef)
{
	pData data = (pData) dataRef;

	npCURL_easySetOptUrl(curl, url, dataRef); 
      
	// send all data to this function  
	npCURL_easySetOptWriteFunction(curl, WriteMemoryCallback2, dataRef);

    // we pass our 'chunk' struct to the callback function 
	npCURL_easySetOptWriteData(curl, dataRef, dataRef);

	// some servers don't like requests that are made without a user-agent
	//     field, so we provide one 
	npCURL_easySetOptUserAgent(curl, "libcurl-agent/1.0", dataRef);

    // get it!
	npCURL_easyPerform(curl, dataRef);        
}

void npCURLsetGithubRequest(pNPgithubRequest request, int page, int per_page, char* state, void* dataRef)
{
	pData data = (pData) dataRef;

	request->page     = page;
	request->per_page = per_page;
	request->state    = state;

}
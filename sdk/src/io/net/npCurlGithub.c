
#include "npCurlGithub.h"
#include "npGithub.h"


void npCURLsetGithubRepoURL(pNPcurl curl, pNPgithub github, void* dataRef)
{
	sprintf(curl->url, "https://api.github.com/repos/%s/", github->repo_name); 
}

void npCURLsetGithubRepoIssues(pNPcurl curl, pNPgithub github, void* dataRef)
{
	strcat(curl->url, "issues");
}

void npCURLsetGithubUrlPtrToArguments(pNPcurl curl, pNPgithub github, void* dataRef)
{
	curl->urlPtr = strstr(curl->url, "issues");	
	curl->urlPtr += strlen("issues");
	curl->urlPtr++;
	(*curl->urlPtr) = '?';
	curl->urlPtr++;
}

void npCURLsetGithubUrlPtrToEndOfArguments(pNPcurl curl, pNPgithub github, void* dataRef)
{
	curl->urlPtr = curl->url[curl->urlSize];	
}

void npCurlPrepareNextArgument(pNPcurl curl, void* dataRef)
{
	if(curl->numArgs > 0)
	{
		curl->urlSize = sprintf(curl->url[curl->urlSize], "%c", "&");
	}
}

void npCURLsetGithubRepoIssuesState(pNPcurl curl, pNPgithub github, char* state, void* dataRef)
{
	npCURLsetGithubUrlPtrToArguments(curl, github, dataRef);

	npCURLsetGithubUrlPtrToEndOfArguments(curl, github, dataRef);

	npCurlPrepareNextArgument(curl, dataRef);

	curl->urlSize = sprintf(curl->url[curl->urlSize], "state=%s", state);

	curl->numArgs++;
}

void npCURLsetGithubRepoIssuesPerPage(pNPcurl curl, pNPgithub github, int perPage, void* dataRef)
{
	npCURLsetGithubUrlPtrToArguments(curl, github, dataRef);

	npCURLsetGithubUrlPtrToEndOfArguments(curl, github, dataRef);

	npCurlPrepareNextArgument(curl, dataRef);

	curl->urlSize = sprintf(curl->url[curl->urlSize], "per_page=%d", perPage);	
	curl->numArgs++;
}

void npCURLsetGithubRepoName(pNPcurl curl, pNPgithub github, char* repo, void* dataRef)
{
	github->repo_name = malloc(sizeof(char) * 20);
	strcpy(github->repo_name, repo);

	return;
}

void npCURLbuildGithubUrl(pNPcurl curl, pNPgithub github, void* dataRef)
{
	int index = 0;

	curl->urlSize = sprintf(curl->url, "https://api.github.com/repos/%s/issues?state=all&per_page=200&page=%d", github->repo_name ,github->request->page);
	curl->url[curl->urlSize] = '\0';
//	printf("\nURL : %s\n", curl->url);
}

int npCURLsetGithubRepoIssuesPage(pNPcurl curl, pNPgithub github, int page, void* dataRef)
{
	github->request->page = page;	
	return 0;
}


void npCURLsetGithubIssuesRequestPage(pNPgithub github, int page, void* dataRef)
{
	github->request->page = page;
}

void npCURLsetGithubIssuesRequestPerPage(pNPgithub github, int PerPage, void* dataRef)
{
	github->request->per_page = PerPage;
}

void npCURLsetGithubIssuesRequestState(pNPgithub github, char* state, void* dataRef)
{
	github->request->state = state;	
}

void npCURLgetGithubIssuesRequest(pNPgithub github, int page, int PerPage, char* state, void* dataRef)
{
	npCURLsetGithubIssuesRequestPage(github, page, dataRef);
	npCURLsetGithubIssuesRequestPerPage(github, PerPage, dataRef);
	npCURLsetGithubIssuesRequestState(github, state, dataRef);
}

/// Places the next page in curl->mem
int npCURLgetGithubRepoIssuesNextPage(pNPcurl curl, pNPgithub github, void* dataRef)
{
	pData data = (pData) dataRef;

	npCURLsetGithubRepoIssuesPage(curl, github, github->request->page, dataRef);
	npCURLbuildGithubUrl(&data->io.curl, &data->io.github, data->io.github.request->page, dataRef);

	npCURLgetUrl(&data->io.curl, data->io.curl.url, github->request->page, dataRef);

	if( data->io.curl.mem[github->request->page-1].memory[0] == '['
		&&
		data->io.curl.mem[github->request->page-1].memory[3] == ']')
	{
//		printf("\nLast Page %s End Page : %d", data->io.curl.mem[github->request->page-1].memory, github->request->page);
		return 0;
	}

	github->request->page++;

	return 1;
}

size_t write_avatar_image(void* ptr, size_t size, size_t nmemb, FILE *stream) {
    size_t written = fwrite(ptr, size, nmemb, stream);
    return written;
}

char* npCURLgetGithubUserAvatar(pNPcurl curl, pNPgithubUser user, void* dataRef)
{
	pData data = (pData) dataRef;	
	pNPgithubIssues issues = data->io.github.issues;
    CURL *curl_handle = NULL;
    CURLcode res;
    FILE *fp = NULL;
    static int id = 0;
    int my_index = 0;
	char fileName[40] = {'\0'};

	sprintf(fileName, "%s.png", user->login); 
	user->avatar_image_file = strdup(fileName);

    id++;
    fp = fopen(fileName, "wb");
    if(fp == NULL)
	{
		printf("2371 File Open Failed");
		return NULL;
	}

 //   curl_global_init(CURL_GLOBAL_ALL);

    curl_handle = curl_easy_init();
    if(curl_handle == NULL)
	{
		printf("2312 curl easy init failed");
		return NULL;
	}

	if(curl_handle != NULL && fp != NULL)
	{
	  curl_easy_setopt(curl_handle, CURLOPT_URL, user->avatar_url); 
      
	  curl_easy_setopt(curl_handle, CURLOPT_WRITEFUNCTION, write_avatar_image);
      curl_easy_setopt(curl_handle, CURLOPT_WRITEDATA, fp);
 //   curl_easy_setopt(curl_handle, CURLOPT_CONNECTTIMEOUT, 1);
 //   curl_easy_setopt(curl_handle, CURLOPT_HEADER, 0);
      curl_easy_setopt(curl_handle, CURLOPT_USERAGENT, "libcurl-agent/1.0");
    
      res = curl_easy_perform(curl_handle);
      if(res == CURLE_OK)
	  {
		  //printf("All Good");
	  }
	  else
	  {
		printf("\nError : Error with curl_easy_perform : %d", res);
	  }
	

	  if(curl_handle != NULL)
	  {
		//curl_easy_cleanup(curl_handle);
	  }

//	  printf("Closing fp ");
	  if(fp != NULL)
	  {
	    fclose(fp);
	  }
//	  curl_global_cleanup();
//	  curl_easy_cleanup(curl_handle);

	}
	else
	{
		return NULL;
	}

//	printf("DONE 23444");

    return fileName;
	

}


void npCURLgetGithubRepoIssuesAll(pNPcurl curl, pNPgithub github, void* dataRef)
{
	pData data = (pData) dataRef;
	int isLastPage = 1; /// 0 if true, 1 if false
	
	npCURLbuildGithubUrl(curl, github, dataRef);

	data->io.json.root = NULL;
	while(isLastPage == 1)
	{
		isLastPage = npCURLgetGithubRepoIssuesNextPage(curl, github, dataRef);
	}
}


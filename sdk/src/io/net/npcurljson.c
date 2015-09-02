#include "npcurljson.h"
#include "npGithub.h"

char* dumpTill(char* dump_from, char* dump_to, char* till)
{
    int index = 0;
    char* ptr = dump_from;
    while((char)(*ptr) != (char)till)
    {
        dump_to[index] = (*ptr); index++; ptr++;
    }
    
    dump_to[index] = '\0';
    
    return ptr;
}

int npCURLgetJSONgithubRepoIssuesNextPage(pNPcurl curl, pNPgithub github, void* dataRef)
{
	npCURLsetGithubRepoIssuesPage(curl, github, github->request->page, dataRef);
	npCURLgetUrl(curl, curl->url, github->request->page, dataRef);
	github->request->page++;

	return 0;
}

void npCURLgetJSONgithubRepoIssuesAll(pNPcurl curl, pNPgithub github, void* dataRef)
{	
	pData data = (pData) dataRef;
	json_t* other;
	int isLastPage = 1; /// 0 if true, 1 if false

	github->request->page = 0;
	while(isLastPage == 1)
	{
		isLastPage = npCURLgetJSONgithubRepoIssuesNextPage(curl, github, dataRef);
/*
		if(data->io.json.root == NULL)
		{
			npJSON_loads(&data->io.json, curl->mem.memory, dataRef);		
		}
		else
		{
			other = json_loads(curl->mem.memory, 0, NULL);
			npJSON_arrayExtend(&data->io.json.root, other, dataRef);
		}
*/
	}

//	npJSON_loads(&data->io.json, curl->mem.memory, dataRef);

}

void npCURLgetJSONissuesAll(pNPcurl curl, pNPjson json, void* dataRef)
{
//	pData data = (pData) data;
//	pNPgithub github = data->io.github;
	pNPgithub github;

//	npCURLgetGithubRepoIssuesAll
	npCURLgetJSONgithubRepoIssuesAll(curl, github, dataRef);
}



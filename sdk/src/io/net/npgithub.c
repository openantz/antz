/* -----------------------------------------------------------------------------
*
*  npgithub.c
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
#include "npgithub.h"
// #include <assert.h>
#include "../file/npjson.h"
#include "npgitviz.h"
#include "../../npdata.h"



int npGithubInit(pNPgithub github, void* dataRef)
{
    pData data = (pData) dataRef;
	int index = 0;
    //pNPgithubIssues issues = NULL;
    //issues = malloc(sizeof(NPgithubIssues));
    /*
    if(issues == NULL)
    {
        printf("\n2387Malloc Failed");
        return NULL;
    }
    */
    
   // data->io.
/*
	npGithubIssuesInit(&data->io.issues);
    data->io.issues.running = false;
    npGithubCtrlSetCurrentIssueIndex(&data->io.issues, 0, dataRef);
    npGithubCtrlSetCurrentIssue(&data->io.issues, dataRef);
    npGithubCtrlSetRecordTagIndex(&data->io.issues, 0, dataRef);
*/   

	github->issues = malloc(sizeof(NPgithubIssues));
	if(github->issues == NULL)
	{	
		return -1;
	}
	
/*
	for(index = 0; index < 300; index++)
	{
		github->issues->issue[index].user = NULL;
		github->issues->issue[index].num_of_labels = 0;
		github->issues->issue[index].assignee = NULL;
	}
*/

	index = 0;	

	data->io.github.issues->running = false;

	for(index = 0; index < 10; index++)
	{
		github->rR[index].page = -1;
		github->rR[index].per_page = NULL;
		github->rR[index].requestUrl[0] = '\0';
	//	github->rR[index].response = NULL;
		github->rR[index].responseSize = 0;
		github->rR[index].state = NULL;
		github->rR[index].urlSize = -1;
	}
		
	/*
	github->request = malloc(sizeof(NPgithubRequest));
	if(github->request == NULL)
	{
		printf("\nmalloc failed");
		return;
	}
	
	github->request->page = 0;
	github->request->per_page = 200;
	github->request->state = NULL;
	*/
	
	github->num_of_users = 0;
	for(index = 0; index < 200; index++)
	{
		github->user[index].avatar_image_file = NULL;
		github->user[index].avatar_image_file_path = NULL;
		github->user[index].avatar_image_textureID = 0;
		github->user[index].avatar_url = NULL;
		github->user[index].events_url = NULL;
		github->user[index].followers_url = NULL;
		github->user[index].gists_url = NULL;
		github->user[index].gravatar_id = NULL;
		github->user[index].html_url = NULL;
		github->user[index].id = -1;
		github->user[index].login = NULL;
		github->user[index].number = -1;
		github->user[index].organizations_url = NULL;
		github->user[index].received_events_url = NULL;
		github->user[index].repos_url = NULL;
		github->user[index].site_admin = false;
		github->user[index].starred_url = NULL;
		github->user[index].subscriptions_url = NULL;
		github->user[index].type = NULL;
		github->user[index].url = NULL;
	}

	printf("\nnpGithubInit");
	npGithubIssuesInit(github->issues, dataRef); 

	return 0;
}

void npGithubIssuesInit(pNPgithubIssues issues, void* dataRef)
{
	int index = 0;

	printf("\nnext 1");
	issues->count = 0;
	issues->current = NULL;
	issues->index = 0;

	for(index = 0; index < kNPgithubMaxIssues; index++)
	{
		issues->issue[index].user = NULL;
		issues->issue[index].assignee = NULL;
		issues->issue[index].body = NULL;
		issues->issue[index].closed_at = NULL;
		issues->issue[index].closed_by = NULL;
		issues->issue[index].comments_url = NULL;
		issues->issue[index].created_at = NULL;
		issues->issue[index].events_url = NULL;
		issues->issue[index].html_url = NULL;
		issues->issue[index].id = 0;
		issues->issue[index].index = index;
		issues->issue[index].issue_closed_at = NULL;
		issues->issue[index].issue_closed_node = NULL;
		issues->issue[index].issue_creation_node = NULL;
		issues->issue[index].issueGeoType = 0;
		issues->issue[index].issueNodeType = 0;
		issues->issue[index].issueTopoType = 0;
		issues->issue[index].labels_url = NULL;
		issues->issue[index].locked = false;
		issues->issue[index].milestone = NULL;
		issues->issue[index].num_comments = 0;
		issues->issue[index].numOfWordsInIssueTitle = 0;
		issues->issue[index].recordId = 0;
		issues->issue[index].recordTag = NULL;
		issues->issue[index].state = NULL;
		issues->issue[index].this_issue = &issues->issue[index];
		issues->issue[index].title = NULL;
		issues->issue[index].titleTag = NULL;
		issues->issue[index].updated_at = NULL;
		issues->issue[index].url = NULL;
		issues->issue[index].userId = 0;
		issues->issue[index].num_of_labels = 0;
	}
	

	printf("\nnext 2");
}


void npGithubSetRepo(pNPgithub github, char* repo, void* dataRef)
{
	//strcpy(github->repo_name, repo);
	github->repo_name = repo;
}	

void npGithubGetIssues(void* dataRef)
{
	pData data = (pData) dataRef;	

	pNPcurl curl = &data->io.curl; 
	pNPgithub github = &data->io.github;
	
	char* response = NULL;
	int   index    = 0;
	int   err      = -1;


//	pNPjson json = &data->io.json; 
	new2_pNPjson json = &data->io.json2;
//	github->jsonGithub = json;

	/*
	assert(github != NULL);
	assert(curl != NULL);
	assert(json != NULL);
	*/

	npGithubSetRepo(github, "openantz/antz", dataRef);
//	npGithubSetRepo(github, "bagder/curl", dataRef);
//	npGithubSetRepo(github, "reddit/reddit", dataRef);

	do{
		npGithubBuildRequest(github, index, "all", "200", index+1, dataRef);
//		printf("\nurl : %s", github->rR[index].requestUrl); printf("\npage : %d", github->rR[index].page);
		err = npGithubSendRequest(github, index, dataRef);
		if(err == -1)
			return;

		response = npGithubReceiveResponse(github, index, dataRef);	
		index++;
	}while(response != NULL);
	
	github->num_of_RequestResponses = index;
	//printf("github->num_of_RequestResponses : %d", github->num_of_RequestResponses);
//	npCURLgetGithubRepoIssuesAll(curl, github, dataRef);

	for(github->requestResponse_index = 0; github->requestResponse_index < github->num_of_RequestResponses; github->requestResponse_index++)
	{ // include npjson.h
		npjsonSetInput(&data->io.json2, github->rR[github->requestResponse_index].response, github->requestResponse_index, dataRef);	
		npjsonLoadInput(&data->io.json2, dataRef);
	}
	
	if(json->jRoot.jsonRoot == NULL)
	{
		npjsonUnpackRoot(json, dataRef);
	}
	else
	{
		return;
	}


	if(json->jRoot.type == JSON_ARRAY)
	{
		printf("\njson->jRoot.type : %s", "JSON_ARRAY");
//		((pNPjsonArray)json->jRoot.jsonRoot)->
	
		//getchar();
//		npGithubGetIssues(dataRef);
		/*
		printf("0 num labels : %d", data->io.github.issues->issue[0].num_of_labels);
		printf("\n1 num labels : %d", data->io.github.issues->issue[1].num_of_labels);
		printf("\njson ptr : %p", json);
		*/
//		printf("\nnum of issues : %d" ,((pNPjsonArray)json->jRoot.jsonRoot)->numElements);	
		data->io.github.issues->count = ((pNPjsonArray)json->jRoot.jsonRoot)->numElements;	
		printf("\nnum of issues : %d\n" , data->io.github.issues->count );	
		//getchar();
		for(index = 0; index < data->io.github.issues->count; index++)
		{
			//npGithubGetIssue(json, data->io.github.issues, index, dataRef);	
//			printf("INDEX ::: %d :: issues ptr %p", index, data->io.github.issues);
			//printf(".");
			npGithubGetIssue(&data->io.json2, data->io.github.issues, index, dataRef);
		}
	//	printf("\n");	
	}
//	npJSONloadCURLdata(curl, json, dataRef); 
//	printf("\nBreakpoint here");
//	npJSONissuesGetGithubIssues(json, github, dataRef); /// @todo temp	
	return;
}

/*
void npGithubGetIssues(void* dataRef)
{
	pData data = (pData) dataRef;	

	pNPcurl curl = &data->io.curl; 
	pNPgithub github = &data->io.github;
//	github->jsonGithub = &data->io.json;


	pNPjson json = &data->io.json; 
	github->jsonGithub = json;

	npCURLsetGithubRequest(github->request, 1, 200, "all", dataRef);
	npCURLsetGithubRepoName(curl, github, "openantz/antz", dataRef);
//	npCURLsetGithubRepoName(curl, github, "bagder/curl", dataRef);
//	npCURLsetGithubRepoName(curl, github, "reddit/reddit", dataRef);

	npCURLgetGithubRepoIssuesAll(curl, github, dataRef);
	for(curl->memIndex = 0; curl->memIndex < curl->numMem; curl->memIndex++)
	{
		npjsonSetInput(json, curl->mem, curl->memIndex, dataRef);
		npjsonLoadInput(json, dataRef);
	}

	
//	npJSONloadCURLdata(curl, json, dataRef); 

	npJSONissuesGetGithubIssues(json, github, dataRef); /// @todo temp	
	return;
}
*/


size_t write_avatar_image(void* ptr, size_t size, size_t nmemb, FILE *stream) {
    size_t written = fwrite(ptr, size, nmemb, stream);
    return written;
}

char* npGithubGetUserAvatar(pNPgithubUser user, void* dataRef)
{
	pData data = (pData) dataRef;	
	pNPcurl curl = &data->io.curl;
	pNPgithubIssues issues = data->io.github.issues;
    CURL *curl_handle = NULL;
    CURLcode res;
    FILE *fp = NULL;
    static int id = 0;
    int my_index = 0;
	char fileName[40] = {'\0'};

	sprintf(fileName, "%s.png", user->login); 
	user->avatar_image_file = npstrdup( fileName, data );

	//printf("\nuser->avatar_image_file : %s", user->avatar_image_file);
	//getchar();

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

//    return fileName;
	return user->avatar_image_file;
	

}

void npGithubRun(void* dataRef)
{
    pData data = (pData) dataRef;

	pNPgithub github = &data->io.github;
    bool getUsers = false;
	int userIndex = 0;
   
//	printf("\ngithub ptr : %p", github);
	if(github->issues->running != false)
    {
		npGithubGetIssues(dataRef);
//		printf("\ngithub->num_of_users : %d", github->num_of_users);
		while(userIndex < github->num_of_users)
		{
			npGithubGetUserAvatar(&data->io.github.user[userIndex], dataRef);
			userIndex++;
		}

		theNew_npGitViz(&data->io.github, dataRef);
		
		github->issues->running = false;
    }
    
    if( getUsers == true )
    {
		//getGithubProcessUsers(issues, 1, dataRef);
    }
    
}


void npGithubBuildRequest(pNPgithub github, int request_index, char* state, char* per_page, int page, void* dataRef)
{
	pNPgithubRequestResponse rR = &github->rR[request_index];
	github->requestResponse_index = request_index;
//	printf("\nnpGithubBuildRequest");
	
	rR->state = state;
	rR->per_page = per_page;
	rR->page = page;
	
	rR->urlSize = sprintf(rR->requestUrl, "https://api.github.com/repos/%s/issues?state=%s&per_page=%s&page=%d", github->repo_name, rR->state, rR->per_page, rR->page);
	rR->requestUrl[rR->urlSize] = 0;

	return;
}

char* npGithubReceiveResponse(pNPgithub github, int request_index, void* dataRef)
{
	if( github->rR[request_index].response == NULL )
	{
		return NULL;	
	}

	if( github->rR[request_index].response[0] == '[' 
		&&
		github->rR[request_index].response[3] == ']')
	{
		printf("\nNo more to receive");
		return NULL;
	}
	
	if( strstr(github->rR[request_index].response, "API rate limit exceeded") != NULL )
	{
		printf("\nGithub API rate limit exceeded");
		return NULL;
	}


	return github->rR[request_index].response;
}

int npGithubSendRequest(pNPgithub github, int request_index, void* dataRef)
{
	pData data = (pData) dataRef;
	pNPcurl curl = &data->io.curl;
	int err = -1;
	/*
	printf("\nnpGithubSendRequest");
	printf("\nafter github %p", github);
	printf("\nsend request url : %s", github->rR[request_index].requestUrl);

	printf("\nnpCURL_easySetOptWriteFunction");
	*/
	npCURL_easySetOptWriteFunction(curl, npGithubWriteMemoryCallback, dataRef);
//	printf("npCURLgetUrl");
	err = npCURLgetUrl(curl, github->rR[request_index].requestUrl, 0, dataRef);
	if(err == -1)
	{
		return -1;
	}

	return 0;
}

size_t npGithubWriteMemoryCallback(void* contents, size_t size, size_t nmemb, void* userp)
{
	size_t realSize = size * nmemb;
	pData data = (pData) userp;
	pNPgithub github = &data->io.github;
	pNPgithubRequestResponse rR = &github->rR[github->requestResponse_index];	

	/*
	printf("\nnpGithubWriteMemoryCallback : %d", rR->responseSize);
	printf("\nURL : %s", rR->requestUrl); 
	*/

	rR->response = realloc(rR->response, rR->responseSize + realSize + 1);
	if(rR->response == NULL)
		return 0; /// out of memory

//	printf("\ncontents : %s\n", (char*)contents);
	memcpy(&rR->response[rR->responseSize], contents, realSize);
	rR->responseSize += realSize;

	/// null terminate it
	rR->response[rR->responseSize] = '\0';

	return realSize;
}


/// This is easy because all labels are structured in the same way
/*
 - url   :: string
 - name  :: string
 - color :: string
 */
int npGithubGetIssueLabels(pNPjsonObject json_issue, pNPgithubIssue github_issue, void* dataRef)
{
	pNPjsonArray json_issue_labels = NULL;
	pNPjsonObject json_issue_label = NULL;
	int labels_key_index = -1;
	int label_index = 0;
	int url_key_index = -1;
	int name_key_index = -1;
	int color_key_index = -1;

	labels_key_index = npJSONgetObjectKeyIndex(json_issue, "labels", dataRef);	
	json_issue_labels = ((pNPjsonArray)json_issue->jsonValue[labels_key_index].c_value);
	
	for(label_index = 0; label_index < json_issue_labels->numElements; label_index++)
	{
		json_issue_label = ((pNPjsonObject)json_issue_labels->element[label_index]->c_value);		

		url_key_index    = npJSONgetObjectKeyIndex(json_issue_label, "url", dataRef);	
		name_key_index   = npJSONgetObjectKeyIndex(json_issue_label, "name", dataRef);
		color_key_index  = npJSONgetObjectKeyIndex(json_issue_label, "color", dataRef);
//		printf("\n(ids) url, name, color :: %d %d %d", url_key_index, name_key_index, color_key_index); 

		/// now put it in the github issue labels struct
//		printf("\ngithub_issue ptr : %p", github_issue);
//		printf("\nnum_of_labels : %d", github_issue->num_of_labels);
		github_issue->label[github_issue->num_of_labels].url = json_issue_label->jsonValue[url_key_index].c_value;
		github_issue->label[github_issue->num_of_labels].name = json_issue_label->jsonValue[name_key_index].c_value;  
		github_issue->label[github_issue->num_of_labels].color = json_issue_label->jsonValue[color_key_index].c_value;  
		github_issue->num_of_labels++;
	}
	
	return 0;
}

int npGithubGetIssueKeyIndex(pNPjsonObject json_issue, char* key, void* dataRef)
{
	int index = -1;
//	printf("\nKey is %s", key);
	index = npJSONgetObjectKeyIndex(json_issue, key, dataRef);

//	printf("\nkey index : %d", index);
	return index;
}

int npGithubGetUserKeyIndex(pNPjsonObject json_github_user, char* key, void* dataRef)
{
	int index = -1;
	index = npJSONgetObjectKeyIndex(json_github_user, key, dataRef);

	return index;
}

int npGithubSearchForUser(pNPgithub github, char* user, void* dataRef)
{
	int user_index = 0;

	if(user == NULL)
		return -2;

	/// Now search to see if the user's data is already in the system
	for(user_index = 0; user_index < github->num_of_users; user_index++)
	{
		if( github->user[user_index].login != NULL ) 
		{
			//if( strcmp(login, data->io.github.user[user_index].login) == 0 )
			if( strcmp(user, github->user[user_index].login) == 0 )
			{
				/// Found User
//				printf("\nFound User");
//				github_issues->issue[github_issues->count].user = &github->user[user_index];				
				github->issues->issue[user_index].user = &github->user[user_index];
				break;
			}
		}
	}
	
	if(user_index == github->num_of_users)
	{
		return -1;
	}

	return user_index;
}

void npGithubAddUserFromIssue(pNPgithub github, pNPjsonObject json_github_user_object, int issueIndex, void* dataRef)
{
	pData data = (pData) dataRef;
	pNPgithubIssues github_issues = github->issues;
	pNPgithubUser issueUser = NULL;
	int user_login_key_index = -1;
	int user_key_index = -1;
	char* login = NULL;

	user_login_key_index = npJSONgetObjectKeyIndex(json_github_user_object, "login", dataRef);
//	printf("\nuser_login_key_index : %d", user_login_key_index);
	if(user_login_key_index == -1)
	{
//		printf("\nCould not find user login");
		return;
	}


//	printf("\nIssue Index | %d", issueIndex);
//	login = ((pNPjsonObject)issue->jsonValue[key_index].c_value)->jsonValue[user_key_index].c_value;	
//	login = json_github_user_object->jsonValue[user_key_index].c_value;
	login = json_github_user_object->jsonValue[user_login_key_index].c_value;
	if( github_issues->issue[issueIndex].user == NULL)
	{
//		////getchar();
		github_issues->issue[issueIndex].user = &data->io.github.user[data->io.github.num_of_users];
		issueUser = github_issues->issue[issueIndex].user;
/*		
		printf("\nissueUser : %p ||| github_issues->issue[issueIndex].user : %p", issueUser,
			github_issues->issue[issueIndex].user);
*/
//		printf("\nissueUser : %s", github_issues->issue[issueIndex].user->login);
//		printf("\nissueUser : %s", login); 

		data->io.github.num_of_users++;
		issueUser->login = json_github_user_object->jsonValue[user_login_key_index].c_value;
//		printf("\nUser Login : %s", issueUser->login);

		user_key_index = npGithubGetUserKeyIndex(json_github_user_object, "id", dataRef);
		issueUser->id = ((pNPjsonInteger)json_github_user_object->jsonValue[user_key_index].c_value)->j_int;

		user_key_index = npGithubGetUserKeyIndex(json_github_user_object, "avatar_url", dataRef);
		issueUser->avatar_url = json_github_user_object->jsonValue[user_key_index].c_value;

		user_key_index = npGithubGetUserKeyIndex(json_github_user_object, "gravatar_id", dataRef);
		issueUser->gravatar_id = json_github_user_object->jsonValue[user_key_index].c_value;

		user_key_index = npGithubGetUserKeyIndex(json_github_user_object, "url", dataRef);
		issueUser->url = json_github_user_object->jsonValue[user_key_index].c_value;

		user_key_index = npGithubGetUserKeyIndex(json_github_user_object, "html_url", dataRef);
		issueUser->html_url = json_github_user_object->jsonValue[user_key_index].c_value;

		user_key_index = npGithubGetUserKeyIndex(json_github_user_object, "followers_url", dataRef);
		issueUser->followers_url = json_github_user_object->jsonValue[user_key_index].c_value;

/*
		user_key_index = npGithubGetUserKeyIndex(json_github_user_object, "following_url", dataRef);
		github_issues->issue[index].user->following_url = json_github_user_object->jsonValue[user_key_index];
*/
		user_key_index = npGithubGetUserKeyIndex(json_github_user_object, "gists_url", dataRef);
		issueUser->gists_url = json_github_user_object->jsonValue[user_key_index].c_value;

		user_key_index = npGithubGetUserKeyIndex(json_github_user_object, "starred_url", dataRef);
		issueUser->starred_url = json_github_user_object->jsonValue[user_key_index].c_value;

		user_key_index = npGithubGetUserKeyIndex(json_github_user_object, "subscriptions_url", dataRef);
		issueUser->subscriptions_url = json_github_user_object->jsonValue[user_key_index].c_value;

		user_key_index = npGithubGetUserKeyIndex(json_github_user_object, "organizations_url", dataRef);
		issueUser->organizations_url = json_github_user_object->jsonValue[user_key_index].c_value;

		user_key_index = npGithubGetUserKeyIndex(json_github_user_object, "repos_url", dataRef);
		issueUser->repos_url = json_github_user_object->jsonValue[user_key_index].c_value;

		user_key_index = npGithubGetUserKeyIndex(json_github_user_object, "events_url", dataRef);
		issueUser->events_url = json_github_user_object->jsonValue[user_key_index].c_value;
//
		user_key_index = npGithubGetUserKeyIndex(json_github_user_object, "received_events_url", dataRef);
		issueUser->received_events_url = json_github_user_object->jsonValue[user_key_index].c_value;

		user_key_index = npGithubGetUserKeyIndex(json_github_user_object, "type", dataRef);
		issueUser->type = json_github_user_object->jsonValue[user_key_index].c_value;

		user_key_index = npGithubGetUserKeyIndex(json_github_user_object, "site_admin", dataRef);
		issueUser->site_admin = ((pNPjsonBoolean)json_github_user_object->jsonValue[user_key_index].c_value)->j_bool;

//		npGithubPrintUserData(issueUser, dataRef);
//		getchar();
		
	}

}

void npGithubPrintUserData(pNPgithubUser user, void* dataRef)
{
	printf("\nLogin : %s \n ID : %d \n Number : %d", user->login, 
		user->id, user->number);
}


void npGithubGetIssue(new2_pNPjson github_json, pNPgithubIssues github_issues, int index, void* dataRef)
{
	pData data = (pData)dataRef;
	pNPjsonArray issues_array = NULL;
	pNPjsonObject issue = NULL;
	pNPjsonObject json_github_user_object = NULL;
	pNPjsonObject json_github_assignee_object = NULL;
	pNPgithubIssue github_issue = NULL;
	char* login = NULL;
	char* assignee_login = NULL;
	int key_index = -1;
	int labels_key_index = -1;
	int label_index = 0;
	int user_key_index = -1;
	int user_index = 0;
	int user_found = -1;
	int github_user_index = -1;
	int assignee_key_index = -1;

	int assignee_user_index = -1;
	int github_assignee_index = -1;
/*
	if(index > 1000)
	{
		printf("\nindex is over 1000\n"); getchar();
	}
*/
//	assert(index < 1000);

	github_issue = &github_issues->issue[index];
	
	switch( github_json->jRoot.type )
	{
		case JSON_ARRAY:
			issues_array = github_json->jRoot.jsonRoot;
			break;
		case JSON_OBJECT:
			break;
	}
/*
	printf("\nissues_array ptr : %p", issues_array);
	printf("github_issues ptr : %p", github_issues);
//	printf("\nissues_array->element[%d] ptr : %p", index, issues_array->element[index]);
	printf("\nindex : %d", index);
	printf("\nptr : %p", issues_array->element[index]);
	/// is this not an object?
//	printf("\nelement->c_value numOf: %d", ((pNPjsonObject)issues_array->element[index]->c_value)->numNameValuePairs );
	printf("\n0");
*/

	if( index < 0 || index >= issues_array->numElements)
	{
		printf("\nIndex : out of domain");
		getchar();
	}

	issue = (pNPjsonObject)(issues_array->element[index]->c_value);
//	printf("\nnumOf : %d", issue->numNameValuePairs);
//	getchar();

	if(issue->numNameValuePairs < 0 || issue->numNameValuePairs > 30)
	{
		printf("out of domain : %d", issue->numNameValuePairs);
		getchar();
	}

//	printf("\n1 : %d", issue->numNameValuePairs);
	key_index = npGithubGetIssueKeyIndex(issue, "url", dataRef);	
//	printf("\n2");
//	printf("\n2a : %s", (char*)issue->jsonValue[key_index].c_value);
	github_issues->issue[index].url = issue->jsonValue[key_index].c_value;
	//printf("\nURL : %s\n", github_issues->issue[index].url); 
//	printf("\nURL : %s\n", (char*)(issue->jsonValue[key_index].c_value)); 
	
	key_index = npGithubGetIssueKeyIndex(issue, "labels_url", dataRef);
	github_issues->issue[index].labels_url = issue->jsonValue[key_index].c_value;

	key_index = npGithubGetIssueKeyIndex(issue, "comments_url", dataRef);
	github_issues->issue[index].comments_url = issue->jsonValue[key_index].c_value;

	key_index = npGithubGetIssueKeyIndex(issue, "events_url", dataRef);
	github_issues->issue[index].events_url = issue->jsonValue[key_index].c_value;

	key_index = npGithubGetIssueKeyIndex(issue, "html_url", dataRef);
	github_issues->issue[index].html_url = issue->jsonValue[key_index].c_value;

	key_index = npGithubGetIssueKeyIndex(issue, "id", dataRef);
	github_issues->issue[index].id = ((pNPjsonInteger)issue->jsonValue[key_index].c_value)->j_int; /// @todo unpack
	
	key_index = npGithubGetIssueKeyIndex(issue, "number", dataRef);
	github_issues->issue[index].number = ((pNPjsonInteger)issue->jsonValue[key_index].c_value)->j_int; /// @todo unpack

	key_index = npGithubGetIssueKeyIndex(issue, "title", dataRef);
	github_issues->issue[index].title = issue->jsonValue[key_index].c_value; 

	key_index = npGithubGetIssueKeyIndex(issue, "assignee", dataRef);
//	printf("\nassignee_Key_index : %d", key_index);
	assignee_key_index = npJSONgetObjectKeyIndex(issue->jsonValue[key_index].c_value, "login", dataRef);
	
	if(assignee_key_index != -1)
	{
//		printf("\nassignee_login_key_index : %d", assignee_key_index);
		json_github_assignee_object = ((pNPjsonObject)issue->jsonValue[key_index].c_value);
		assignee_login = json_github_assignee_object->jsonValue[assignee_key_index].c_value;
//		printf("\nAssignee_login : %s", assignee_login);

		assignee_user_index = npGithubSearchForUser(&data->io.github, assignee_login, dataRef);
		github_issues->issue[index].assignee = &data->io.github.user[assignee_user_index];
		github_issues->issue[index].assignee->login = assignee_login;

		/*
		github_issues->issue[index].assignee = malloc(sizeof(NPgithubUser)); ///This isn't tied in with the system
		printf("\nAssignee ptr : %p", github_issues->issue[index].assignee);
//		github_issues->issue[index].assignee->login = assignee_login;
//		github_issues->issue[index].assignee->login = strdup(assignee_login);
		github_issues->issue[index].assignee->login = malloc(sizeof(char) * strlen(assignee_login));
		github_issues->issue[index].assignee->login = strcpy(github_issues->issue[index].assignee->login, assignee_login);
		*/
	}
	else
	{
//		printf("\nhas no assignee");
	//	getchar();
	}

	key_index = npGithubGetIssueKeyIndex(issue, "user", dataRef);	
	user_key_index = npJSONgetObjectKeyIndex(issue->jsonValue[key_index].c_value, "login", dataRef);
	json_github_user_object = ((pNPjsonObject)issue->jsonValue[key_index].c_value);
	login = json_github_user_object->jsonValue[user_key_index].c_value;

	github_user_index = npGithubSearchForUser(&data->io.github, login, dataRef);
	if(github_user_index != -1)
	{
		/// User was found
		/*
		printf("\n----------------------------\n");
		printf("\nuser already exists (issue index : %d)\n", index);
		printf("\ngithub user index : %d", github_user_index);
		*/
		github_issues->issue[index].user = &data->io.github.user[github_user_index];		
		
		/*
		printf("\nUser : %s", login);
		printf("\nissue user login : %s", github_issues->issue[index].user->login);
//		getchar();
		printf("\n----------------------------\n");
		*/
	}
	else
	{
		//printf("\n----------------------------\n");
		//printf("\nadd user (issue index : %d)\n", index);
		npGithubAddUserFromIssue(&data->io.github, json_github_user_object, index, dataRef);
		//printf("\n----------------------------\n");
	}

	
	github_assignee_index = npGithubSearchForUser(&data->io.github, assignee_login, dataRef);
	if(github_assignee_index >= 0)
	{
		/// User was found
		//printf("\n----------------------------\n");
		//printf("\nassignee user already exists (issue index : %d)\n", index);
		//github_issues->issue[index].user = &data->io.github.user[github_assignee_index];		
		github_issues->issue[index].assignee = &data->io.github.user[github_assignee_index];		
		/*
		if(github_issues->issue[index].assignee->avatar_image_file == NULL)
		{
			github_issues->issue[index].assignee->avatar_image_file = malloc(sizeof(char) * ( strlen(github_issues->issue[index].assignee->login) + 5 ) ); 
			github_issues->issue[index].assignee->avatar_image_file[0] = '\0';
			sprintf(github_issues->issue[index].assignee->avatar_image_file, "%s.png", github_issues->issue[index].assignee->login);
			printf("\navatar_image_file : %s", github_issues->issue[index].assignee->avatar_image_file);
	//		getchar();
		}
		*/

		//printf("\nassignee User : %s", login);
		//printf("\n----------------------------\n");
	}
	else if(github_assignee_index == -2)
	{
		//printf("\nassignee_login is NULL");
	}
	else if(github_assignee_index == -1)
	{
		//printf("\n----------------------------\n");
		//printf("\nadd assignee user (issue index : %d)\n", index);
		npGithubAddUserFromIssue(&data->io.github, json_github_assignee_object, index, dataRef);
		//printf("\n----------------------------\n");
	}
		

	npGithubGetIssueLabels(issue, github_issue, dataRef); 

	key_index = npGithubGetIssueKeyIndex(issue, "state", dataRef);
	github_issues->issue[index].state = issue->jsonValue[key_index].c_value; 
	
	key_index = npGithubGetIssueKeyIndex(issue, "locked", dataRef);
	github_issues->issue[index].locked = ((pNPjsonBoolean)issue->jsonValue[key_index].c_value)->j_bool; 

	/// @todo milestone	
	/// @todo comments	
	
	key_index = npGithubGetIssueKeyIndex(issue, "created_at", dataRef);
	github_issues->issue[index].created_at = issue->jsonValue[key_index].c_value; 

	key_index = npGithubGetIssueKeyIndex(issue, "updated_at", dataRef);
	github_issues->issue[index].updated_at = issue->jsonValue[key_index].c_value; 

	key_index = npGithubGetIssueKeyIndex(issue, "closed_at", dataRef);
	github_issues->issue[index].closed_at = issue->jsonValue[key_index].c_value; 

	key_index = npGithubGetIssueKeyIndex(issue, "body", dataRef);
	github_issues->issue[index].body = issue->jsonValue[key_index].c_value; 
}
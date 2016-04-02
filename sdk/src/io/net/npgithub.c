/* -----------------------------------------------------------------------------
*
*  npgithub.c
*
*  ANTz - realtime 3D data visualization tools for the real-world, based on NPE.
*
*  ANTz is hosted at http://openantz.com and NPE at http://neuralphysics.org
*
*  Written in 2010-2016 by Shane Saxon - saxon@openantz.com
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

#include "../../npdata.h"
#include "../file/npjson.h"
#include "npgitviz.h"


#define kNPgitvizCSV	"usr/gitviz/csv"		///< GitViz CSV path
#define kNPgitvizImages "usr/gitviz/images"		///< GitViz user avatar images


/// 
int npGithubInit(pNPgithub github, void* dataRef)
{
    pData data = (pData) dataRef;
	int index = 0;
    
	github->issues = malloc(sizeof(NPgithubIssues));
	if(github->issues == NULL)
	{	
		return -1;
	}
	
	index = 0;	

	data->io.github.issues->running = false;

	/// @todo add define : kNPgithubMaxRequests
	for(index = 0; index < 10; index++)
	{
		github->rR[index].page = -1;
		github->rR[index].per_page = NULL;
		github->rR[index].requestUrl[0] = '\0';
		github->rR[index].response = NULL;
		github->rR[index].responseSize = 0;
		github->rR[index].state = NULL;
		github->rR[index].urlSize = -1;
	}
		
	github->num_of_users = 0;
	/// @todo add define : kNPgithubMaxUsers
	for(index = 0; index < kNPgithubMaxUsers; index++)
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

	printf("npGithubInit\n");
	npGithubIssuesInit(github->issues, dataRef); 

	return 0;
}

void npGithubIssuesInit(pNPgithubIssues issues, void* dataRef)
{
	int index = 0;

	printf("next 1\n");
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
	

	printf("next 2\n");
}


void npGithubSetRepo(pNPgithub github, char* repo, void* dataRef)
{
	//strcpy(github->repo_name, repo);
	 github->repo_name = npNewStrcpy( repo, dataRef );
}	

void npGithubGetIssues(void* dataRef)
{
	pData data = (pData) dataRef;	

	pNPcurl curl = &data->io.curl; 
	pNPgithub github = &data->io.github;
	
	char msg[256];

	char* response = NULL;
	int   index    = 0;
	int   err      = -1;

//	pNPjson json = &data->io.json; 
	pNPjson json = &data->io.json; /// @todo change json2 to json
	json->jRoot.jsonRoot = NULL; /// @todo npjsonInit
//	github->jsonGithub = json;

	printf( "%s\n", data->io.gitvizURL );
	npGithubSetRepo( github, data->io.gitvizURL, dataRef );

//	npGithubSetRepo(github, "openantz/antz", dataRef);
//	npGithubSetRepo(github, "bagder/curl", dataRef);
//	npGithubSetRepo(github, "reddit/reddit", dataRef);
//	npGithubSetRepo(github, "torvalds/linux", dataRef);
	
	do{
		npGithubBuildRequest(github, index, "all", "200", index+1, dataRef);
//		printf("url : %s", github->rR[index].requestUrl); printf("\npage : %d\n", github->rR[index].page);
		err = npGithubSendRequest(github, index, dataRef);
		if(err == -1)
			return;

		response = npGithubReceiveResponse(github, index, dataRef);	
		index++;
	}while(response != NULL);
	
	github->num_of_RequestResponses = index;
	//printf("github->num_of_RequestResponses : %d\n", github->num_of_RequestResponses);

	for(github->requestResponse_index = 0; github->requestResponse_index < github->num_of_RequestResponses; github->requestResponse_index++)
	{ // include npjson.h
		npjsonSetInput(&data->io.json, github->rR[github->requestResponse_index].response, github->requestResponse_index, dataRef);	
		npjsonLoadInput(&data->io.json, dataRef);
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
	
//		printf("num of issues : %d\n" ,((pNPjsonArray)json->jRoot.jsonRoot)->numElements);	
		data->io.github.issues->count = ((pNPjsonArray)json->jRoot.jsonRoot)->numElements;	
		
		sprintf( msg, "issues count: %d" , data->io.github.issues->count );	
		npPostMsg( msg, kNPmsgCtrl, data );

		if(data->io.github.issues->count > kNPgithubMaxIssues)
		{
			printf("err 8944 - kNPgithubMaxIssues Exceeded\n");
			getchar();
		}


		for(index = 0; index < data->io.github.issues->count; index++)
		{
			npGithubGetIssue(&data->io.json, data->io.github.issues, index, dataRef); /// @todo This should just take pNPgithub
		}
	}

	return;
}

size_t write_avatar_image(void* ptr, size_t size, size_t nmemb, FILE *stream) {
    size_t written = fwrite(ptr, size, nmemb, stream);
    return written;
}

char* npGithubGetUserAvatar(pNPgithubUser user, void* dataRef)
{
	pData data = (pData) dataRef;	
	pNPcurl curl = &data->io.curl;
	pNPcurlFuncSet curlFunc = curl->funcSetList;
	pNPgithubIssues issues = data->io.github.issues;
    CURL *curl_handle = NULL;
    CURLcode res;
    FILE *fp = NULL;
    static int id = 0;
    int my_index = 0;
	char fileName[kNPurlMax] = {'\0'};

	sprintf(fileName, "%s/%s.png", kNPgitvizImages, user->login); 
	user->avatar_image_file = malloc(sizeof(char) * strlen(fileName));
	strcpy(user->avatar_image_file, fileName);

    id++;
    fp = fopen(fileName, "wb");
    if(fp == NULL)
	{
		printf("err 8495 - gitviz failed to open image file\n");
		return NULL;
	}

 //   curl_global_init(CURL_GLOBAL_ALL);

	curl_handle = curlFunc->easy_init(); 
    if(curl_handle == NULL)
	{
		printf("err 8496 - gitviz failed to easy init curl\n");
		return NULL;
	}

	if(curl_handle != NULL && fp != NULL)
	{
		curlFunc->easy_setopt(curl_handle, CURLOPT_URL, user->avatar_url);
		curlFunc->easy_setopt(curl_handle, CURLOPT_WRITEFUNCTION, write_avatar_image);
		curlFunc->easy_setopt(curl_handle, CURLOPT_WRITEDATA, fp);
		curlFunc->easy_setopt(curl_handle, CURLOPT_USERAGENT, "libcurl-agent/1.0");
    
		res = curlFunc->easy_perform(curl_handle);

      if(res != CURLE_OK)
	  {
		printf("err 8497 - gitviz failed curl_easy_perform : %d\n", res);
	  }

	  fclose(fp);
	}
	else
	{
		return NULL;
	}

	return user->avatar_image_file;
	

}

void npGithubRun(void* dataRef)
{
    pData data = (pData) dataRef;

	pNPgithub github = &data->io.github;
    bool getUsers = false;
	int userIndex = 0;
   
	if(github->issues->running != false)
    {
		npGithubGetIssues(dataRef);
		while(userIndex < github->num_of_users)
		{
		//	printf("userIndex : %d\n", userIndex);
			npGithubGetUserAvatar(&data->io.github.user[userIndex], dataRef); /// temp
			userIndex++;
		}

		theNew_npGitViz(&data->io.github, dataRef);
		
		github->issues->running = false;
    }
}


void npGithubBuildRequest(pNPgithub github, int request_index, char* state, char* per_page, int page, void* dataRef)
{
	pNPgithubRequestResponse rR = &github->rR[request_index];
	github->requestResponse_index = request_index;
	
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
		//printf("No more to receive\n");
		return NULL;
	}
	
	if( strstr(github->rR[request_index].response, "API rate limit exceeded") != NULL )
	{
		printf("warn 8498 - Github API rate limit exceeded\n");
		return NULL;
	}


	return github->rR[request_index].response;
}

int npGithubSendRequest(pNPgithub github, int request_index, void* dataRef)
{
	pData data = (pData) dataRef;
	pNPcurl curl = &data->io.curl;
	pNPcurlFuncSet func = data->io.curl.funcSetList;
	int err = -1;

	npCURL_easySetOptWriteFunction(curl, npGithubWriteMemoryCallback, dataRef);
	err = npCURLgetUrl(curl, github->rR[request_index].requestUrl, 0, dataRef);
	if(err == -1)
		return -1;

	return 0;
}

size_t npGithubWriteMemoryCallback(void* contents, size_t size, size_t nmemb, void* userp)
{
	size_t realSize = size * nmemb;
	pData data = (pData) userp;
	pNPgithub github = &data->io.github;
	pNPgithubRequestResponse rR = &github->rR[github->requestResponse_index];	

	
//	printf("npGithubWriteMemoryCallback : %d\n", rR->responseSize);

	rR->response = realloc(rR->response, rR->responseSize + realSize + 1);
	if(rR->response == NULL)
	{
		printf( "rR->response == NULL\n" );
		return 0; /// out of memory
	}

//	printf("contents : %s\n%d\n", (char*)contents, github->requestResponse_index);
	memcpy(&rR->response[rR->responseSize], contents, realSize);
	rR->responseSize += realSize;

	/// null terminate it
	rR->response[rR->responseSize] = '\0';

	return realSize;
}


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
//		printf("(ids) url, name, color :: %d %d %d\n", url_key_index, name_key_index, color_key_index); 

		/// now put it in the github issue labels struct
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
	index = npJSONgetObjectKeyIndex(json_issue, key, dataRef);

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
			if( strcmp(user, github->user[user_index].login) == 0 )
			{
				/// Found User
				github->issues->issue[user_index].user = &github->user[user_index];
				break;
			}
		}
	}
	
	/// Couldn't find user
	if(user_index == github->num_of_users)
		return -1;

	return user_index;
}

/// @todo rename json_github_user_object TO json_object
void npGithubAddUserFromIssue(pNPgithub github, pNPjsonObject json_github_user_object, int issueIndex, void* dataRef)
{
	pData data = (pData) dataRef;
	pNPgithubIssues github_issues = github->issues;
	pNPgithubUser issueUser = NULL;
	int user_login_key_index = -1;
	int user_key_index = -1;
	char* login = NULL;

	user_login_key_index = npJSONgetObjectKeyIndex(json_github_user_object, "login", dataRef);
	if(user_login_key_index == -1)
	{
		// Could not find user login
		return;
	}

	login = json_github_user_object->jsonValue[user_login_key_index].c_value;
	if( github_issues->issue[issueIndex].user == NULL)
	{
		github_issues->issue[issueIndex].user = &data->io.github.user[data->io.github.num_of_users];
		issueUser = github_issues->issue[issueIndex].user;
		
		data->io.github.num_of_users++;
	//	printf("num_of_users : %d\n", data->io.github.num_of_users);
		if(data->io.github.num_of_users >= kNPgithubMaxUsers)
		{
			printf("kNPgithubMaxUsers Exceeded\n");
			getchar();
		}
		issueUser->login = json_github_user_object->jsonValue[user_login_key_index].c_value;
	//	printf("User Login : %s\n", issueUser->login);

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

		user_key_index = npGithubGetUserKeyIndex(json_github_user_object, "received_events_url", dataRef);
		issueUser->received_events_url = json_github_user_object->jsonValue[user_key_index].c_value;

		user_key_index = npGithubGetUserKeyIndex(json_github_user_object, "type", dataRef);
		issueUser->type = json_github_user_object->jsonValue[user_key_index].c_value;

		user_key_index = npGithubGetUserKeyIndex(json_github_user_object, "site_admin", dataRef);
		issueUser->site_admin = ((pNPjsonBoolean)json_github_user_object->jsonValue[user_key_index].c_value)->j_bool;

//		npGithubPrintUserData(issueUser, dataRef);
	}

}

void npGithubPrintUserData(pNPgithubUser user, void* dataRef)
{
	printf("Login : %s \n ID : %d \n Number : %d\n", user->login, 
		user->id, user->number);
}


void npGithubGetIssue(pNPjson github_json, pNPgithubIssues github_issues, int index, void* dataRef)
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
	int type = 0;
	void* value = NULL;

	github_issue = &github_issues->issue[index];
	
	switch( github_json->jRoot.type )
	{
		case JSON_ARRAY:
			issues_array = github_json->jRoot.jsonRoot;
			break;
		case JSON_OBJECT:
			break;
	}

	if( index < 0 || index >= issues_array->numElements)
	{
		printf("Index : out of domain\n");
		getchar();
	}

	issue = (pNPjsonObject)(issues_array->element[index]->c_value);

	if(issue->numNameValuePairs < 0 || issue->numNameValuePairs > 30)
	{
		printf("out of domain : %d\n", issue->numNameValuePairs);
		getchar();
	}

	key_index = npGithubGetIssueKeyIndex(issue, "url", dataRef);	
	github_issues->issue[index].url = issue->jsonValue[key_index].c_value;

	github_issues->issue[index].labels_url = npJSONgetObjectC(issue, "labels_url", &type, dataRef);

	github_issues->issue[index].comments_url = npJSONgetObjectC(issue, "comments_url", &type, dataRef);
	/// @todo replace rest with npJSONgetObjectC

	github_issues->issue[index].html_url = npJSONgetObjectC(issue, "html_url", &type, dataRef);
	github_issues->issue[index].events_url = npJSONgetObjectC(issue, "events_url", &type, dataRef);

	github_issues->issue[index].id = *(int*)npJSONgetObjectC(issue, "id", &type, dataRef);
	/*
	value = npJSONgetObjectC(issue, "id", &type, dataRef);
	github_issues->issue[index].id = ((pNPjsonInteger)value)->j_int;
	*/

	github_issues->issue[index].number = *(int*)npJSONgetObjectC(issue, "number", &type, dataRef);
	/*
	key_index = npGithubGetIssueKeyIndex(issue, "number", dataRef);
	github_issues->issue[index].number = ((pNPjsonInteger)issue->jsonValue[key_index].c_value)->j_int; /// @todo unpack
	*/

	/// @todo change npJSONgetObjectC to npjsonGetObjectElement
	github_issues->issue[index].title = npJSONgetObjectC(issue, "title", &type, dataRef);
	/*
	key_index = npGithubGetIssueKeyIndex(issue, "title", dataRef);
	github_issues->issue[index].title = issue->jsonValue[key_index].c_value; 
	*/

//	github_issues->issue[index].assignee = npJSONgetObjectC(issue, "assignee", &type, dataRef);
	
	key_index = npGithubGetIssueKeyIndex(issue, "assignee", dataRef);
	assignee_key_index = npJSONgetObjectKeyIndex(issue->jsonValue[key_index].c_value, "login", dataRef);
	
	
	if(assignee_key_index != -1)
	{
		json_github_assignee_object = ((pNPjsonObject)issue->jsonValue[key_index].c_value);
		assignee_login = json_github_assignee_object->jsonValue[assignee_key_index].c_value;

		assignee_user_index = npGithubSearchForUser(&data->io.github, assignee_login, dataRef);
		github_issues->issue[index].assignee = &data->io.github.user[assignee_user_index];
		github_issues->issue[index].assignee->login = assignee_login;
	}

	key_index = npGithubGetIssueKeyIndex(issue, "user", dataRef);	
	user_key_index = npJSONgetObjectKeyIndex(issue->jsonValue[key_index].c_value, "login", dataRef);
	json_github_user_object = ((pNPjsonObject)issue->jsonValue[key_index].c_value);
	login = json_github_user_object->jsonValue[user_key_index].c_value;

	github_user_index = npGithubSearchForUser(&data->io.github, login, dataRef);
	if(github_user_index != -1) { /// User was found
		//printf("User found\n");
		github_issues->issue[index].user = &data->io.github.user[github_user_index];		
	}
	else { /// Not found, adding. 
		//printf("creator user\n");
		npGithubAddUserFromIssue(&data->io.github, json_github_user_object, index, dataRef);
	}
	
	github_assignee_index = npGithubSearchForUser(&data->io.github, assignee_login, dataRef);
	if(github_assignee_index >= 0) { /// Assignee Found
		github_issues->issue[index].assignee = &data->io.github.user[github_assignee_index];		
	}
	else if(github_assignee_index == -2)
	{
		//printf("assignee_login is NULL\n");
	}
	else if(github_assignee_index == -1) {
		//printf("assignee user\n");
		npGithubAddUserFromIssue(&data->io.github, json_github_assignee_object, index, dataRef);
	}
		
	npGithubGetIssueLabels(issue, github_issue, dataRef); 

	key_index = npGithubGetIssueKeyIndex(issue, "state", dataRef);
	github_issues->issue[index].state = issue->jsonValue[key_index].c_value; 
	
	key_index = npGithubGetIssueKeyIndex(issue, "locked", dataRef);
	github_issues->issue[index].locked = ((pNPjsonBoolean)issue->jsonValue[key_index].c_value)->j_bool; 

	/// @todo milestones
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


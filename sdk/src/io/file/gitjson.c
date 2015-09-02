#include "gitjson.h"

json_t* npJSONgetObject(json_t* data, char* key)
{
    json_t *object = NULL;
    object = json_object_get(data, key);
    if(object == NULL)
    {
        printf("\njson_object_get returned null");
    }
    
    return object;
}


void npJSONgetGithubIssueAssignee(pNPgithubIssues issues, pNPjsonGithubIssue jsonIssue, json_t* issue, int issueIndex, void* dataRef)
{	
	pData data = (pData) dataRef;
	json_t* json_login = NULL;
	char* login = NULL;
	int index = 0;

	if(issue == NULL)
	{
		printf("\njson_t* issue is NULL");
		return;
	}

	/// Grab the username of the issue assignee 
	jsonIssue->temp_user = NULL;
	jsonIssue->temp_user = npJSONgetObject(issue, "assignee");
	if(json_is_null(jsonIssue->temp_user) == true)
	{
		//printf("\njsonIssue->temp_user (assignee) is NULL"); 
		return;	
	}
	
	json_login = npJSONgetObject(jsonIssue->temp_user, "login");	
	if(json_is_null(json_login) == true)
	{
		printf("\nassignee user login is NULL");
		return;
	}	
	login = json_string_value(json_login);

	/// Now search to see if the user's data is already in the system
	for(index = 0; index < data->io.github.num_of_users; index++)
	{
		if( data->io.github.user[index].login != NULL)
		{
			if( strcmp(login, data->io.github.user[index].login) == 0 )
			{
				/// Found User
				issues->issue[issueIndex].assignee = &data->io.github.user[index];
				return;
			}
		}
	}

	/// If true, user was not found, adding new user
	if( issues->issue[issueIndex].assignee == NULL )
	{
		issues->issue[issueIndex].assignee->avatar_image_textureID = 0;
		issues->issue[issueIndex].assignee = &data->io.github.user[data->io.github.num_of_users];
		data->io.github.num_of_users++;
		issues->issue[issueIndex].assignee->login = strdup(login);

		jsonIssue->assignee->avatar_url = npJSONgetObject(jsonIssue->temp_user, "avatar_url"); 
		
		if(json_is_null(jsonIssue->assignee->avatar_url) == true )
		{
			printf("\njson : avatar_url is NULL");
			getchar();
			return;
		}	
		else
		{
//			printf("\navatar_url is not NULL");
		}
		
		issues->issue[issueIndex].assignee->avatar_url = json_string_value(jsonIssue->user.avatar_url);

		jsonIssue->assignee->events_url = npJSONgetObject(jsonIssue->temp_user, "events_url");
		if(jsonIssue->assignee->events_url == NULL)
		{
			printf("\njson : events_url is NULL");
			return;
		}

		issues->issue[issueIndex].assignee->events_url = json_string_value(jsonIssue->user.events_url);

		jsonIssue->assignee->followers_url = npJSONgetObject(jsonIssue->temp_user, "followers_url");
		if(jsonIssue->assignee->followers_url == NULL)
		{
			printf("\njson : assignee : followers_url is NULL");
			return;
		}

		issues->issue[issueIndex].assignee->followers_url = json_string_value(jsonIssue->user.followers_url);

		jsonIssue->assignee->following_url = npJSONgetObject(jsonIssue->temp_user, "following_url");
		if(jsonIssue->assignee->following_url == NULL)
		{
			printf("\njson : assignee : following_url is NULL");
			return;
		}

		/// @todo: issues->issue[issueIndex].assignee->follow = json_string_value(jsonIssue->user.followers_url);
	
		jsonIssue->assignee->gists_url = npJSONgetObject(jsonIssue->temp_user, "gists_url");
		if(jsonIssue->assignee->gists_url == NULL)
		{
			printf("\njson : assignee : gists_url is NULL");
			return;
		}

		issues->issue[issueIndex].assignee->gists_url = json_string_value(jsonIssue->user.gists_url);

		jsonIssue->assignee->gravatar_id = npJSONgetObject(jsonIssue->temp_user, "gravatar_id");
		if(jsonIssue->assignee->gravatar_id == NULL)
		{
			printf("\njson : assignee : gravatar_id is NULL");
			return;
		}

		issues->issue[issueIndex].assignee->gravatar_id = json_string_value(jsonIssue->user.gravatar_id);

		jsonIssue->assignee->html_url = npJSONgetObject(jsonIssue->temp_user, "html_url");
		if(jsonIssue->assignee->html_url == NULL)
		{
			printf("\njson : assignee : html_url is NULL");
			return;
		}

		issues->issue[issueIndex].assignee->html_url = json_string_value(jsonIssue->user.html_url);

		jsonIssue->assignee->id = npJSONgetObject(jsonIssue->temp_user, "id");
		if(jsonIssue->assignee->id == NULL)
		{
			printf("\njson : assignee : id is NULL");
			return;
		}

		issues->issue[issueIndex].assignee->id = json_integer_value(jsonIssue->user.id);

		jsonIssue->assignee->organizations_url = npJSONgetObject(jsonIssue->temp_user, "organizations_url");
		if(jsonIssue->assignee->organizations_url == NULL)
		{
			printf("\njson : assignee : organizations_url is NULL");
			return;
		}

		issues->issue[issueIndex].assignee->organizations_url = json_string_value(jsonIssue->user.organizations_url);

		jsonIssue->assignee->received_events_url = npJSONgetObject(jsonIssue->temp_user, "received_events_url");
		if(jsonIssue->assignee->received_events_url == NULL)
		{
			printf("\njson : assignee : received_events_url is NULL");
			return;
		}

		issues->issue[issueIndex].assignee->received_events_url = json_string_value(jsonIssue->user.received_events_url);

		jsonIssue->assignee->repos_url = npJSONgetObject(jsonIssue->temp_user, "repos_url");
		if(jsonIssue->assignee->repos_url == NULL)
		{
			printf("\njson : assignee : repos_url is NULL");
			return;
		}

		issues->issue[issueIndex].assignee->repos_url = json_string_value(jsonIssue->user.repos_url);

		jsonIssue->assignee->site_admin = npJSONgetObject(jsonIssue->temp_user, "site_admin");
		if(jsonIssue->assignee->site_admin == NULL)
		{
			printf("\njson : assignee : site_admin is NULL");
			return;
		}

		issues->issue[issueIndex].assignee->site_admin = json_string_value(jsonIssue->user.site_admin);

		jsonIssue->assignee->starred_url = npJSONgetObject(jsonIssue->temp_user, "starred_url");
		if(jsonIssue->assignee->starred_url == NULL)
		{
			printf("\njson : assignee : starred_url is NULL");
			return;
		}

		issues->issue[issueIndex].assignee->starred_url = json_string_value(jsonIssue->user.starred_url);

		jsonIssue->assignee->subscriptions_url = npJSONgetObject(jsonIssue->temp_user, "subscriptions_url");
		if(jsonIssue->assignee->subscriptions_url == NULL)
		{
			printf("\njson : assignee : subscriptions_url is NULL");
			return;
		}

		issues->issue[issueIndex].assignee->subscriptions_url = json_string_value(jsonIssue->user.subscriptions_url);

		jsonIssue->assignee->type = npJSONgetObject(jsonIssue->temp_user, "type");
		if(jsonIssue->assignee->type == NULL)
		{
			printf("\njson : assignee : type is NULL");
			return;
		}

		issues->issue[issueIndex].assignee->type = json_string_value(jsonIssue->user.type);

		jsonIssue->assignee->site_admin = npJSONgetObject(jsonIssue->temp_user, "site_admin");
		if(jsonIssue->assignee->site_admin == NULL)
		{
			printf("\njson : assignee : site_admin is NULL");
			return;
		}

		issues->issue[issueIndex].assignee->site_admin = json_integer_value(jsonIssue->user.site_admin);

		return; 
	}
	
	issues->issue[issueIndex].assignee = &data->io.github.user[index];
	issues->issue[issueIndex].assignee->avatar_image_file = malloc(20);
	issues->issue[issueIndex].assignee->avatar_image_file[0] = '\0';
	sprintf(issues->issue[issueIndex].assignee->avatar_image_file, "%s.png", issues->issue[issueIndex].assignee->login);

}



void npJSONgetGithubUser(pNPgithubIssues issues, pNPjsonGithubIssue jsonIssue, json_t* issue, int issueIndex, void* dataRef)
{	
	pData data = (pData) dataRef;
	json_t* json_login;
	char* login;
	int index = 0;

	/// Grab the username of the issue creator
	jsonIssue->temp_user = npJSONgetObject(issue, "user");
	if(jsonIssue->temp_user == NULL)
	{
		printf("\njsonIssue->temp_user is NULL"); 
		return;
	}

	json_login = npJSONgetObject(jsonIssue->temp_user, "login");	
	if(json_login == NULL)
	{
		printf("\nuser login is NULL");
		return;
	}	
	login = json_string_value(json_login);

	issues->issue[issueIndex].user = NULL;

	/// Now search to see if the user's data is already in the system
	for(index = 0; index < data->io.github.num_of_users; index++)
	{
		if( data->io.github.user[index].login != NULL)
		{
			if( strcmp(login, data->io.github.user[index].login) == 0 )
			{
				/// Found User
//				printf("\nFound User");
				issues->issue[issueIndex].user = &data->io.github.user[index];
				break;
			}
		}
	}

	/// If true, user was not found, adding new user
	if( issues->issue[issueIndex].user == NULL )
	{
//		getchar();
		issues->issue[issueIndex].user = &data->io.github.user[data->io.github.num_of_users];
		data->io.github.num_of_users++;
		issues->issue[issueIndex].user->login = strdup(login);	
//		getchar();

		jsonIssue->user.avatar_url = npJSONgetObject(jsonIssue->temp_user, "avatar_url"); 
		if(jsonIssue->user.avatar_url == NULL)
		{
			printf("\njson : avatar_url is NULL");
			return;
		}

		issues->issue[issueIndex].user->avatar_url = json_string_value(jsonIssue->user.avatar_url);

		jsonIssue->user.events_url = npJSONgetObject(jsonIssue->temp_user, "events_url");
		if(jsonIssue->user.events_url == NULL)
		{
			printf("\njson : events_url is NULL");
			return;
		}

		issues->issue[issueIndex].user->events_url = json_string_value(jsonIssue->user.events_url);

		jsonIssue->user.followers_url = npJSONgetObject(jsonIssue->temp_user, "followers_url");
		if(jsonIssue->user.followers_url == NULL)
		{
			printf("\njson : user : followers_url is NULL");
			return;
		}

		issues->issue[issueIndex].user->followers_url = json_string_value(jsonIssue->user.followers_url);

		jsonIssue->user.following_url = npJSONgetObject(jsonIssue->temp_user, "following_url");
		if(jsonIssue->user.following_url == NULL)
		{
			printf("\njson : user : following_url is NULL");
			return;
		}

		/// @todo: issues->issue[issueIndex].user->follow = json_string_value(jsonIssue->user.followers_url);
	
		jsonIssue->user.gists_url = npJSONgetObject(jsonIssue->temp_user, "gists_url");
		if(jsonIssue->user.gists_url == NULL)
		{
			printf("\njson : user : gists_url is NULL");
			return;
		}

		issues->issue[issueIndex].user->gists_url = json_string_value(jsonIssue->user.gists_url);

		jsonIssue->user.gravatar_id = npJSONgetObject(jsonIssue->temp_user, "gravatar_id");
		if(jsonIssue->user.gravatar_id == NULL)
		{
			printf("\njson : user : gravatar_id is NULL");
			return;
		}

		issues->issue[issueIndex].user->gravatar_id = json_string_value(jsonIssue->user.gravatar_id);

		jsonIssue->user.html_url = npJSONgetObject(jsonIssue->temp_user, "html_url");
		if(jsonIssue->user.html_url == NULL)
		{
			printf("\njson : user : html_url is NULL");
			return;
		}

		issues->issue[issueIndex].user->html_url = json_string_value(jsonIssue->user.html_url);

		jsonIssue->user.id = npJSONgetObject(jsonIssue->temp_user, "id");
		if(jsonIssue->user.id == NULL)
		{
			printf("\njson : user : id is NULL");
			return;
		}

		issues->issue[issueIndex].user->id = json_integer_value(jsonIssue->user.id);

		jsonIssue->user.organizations_url = npJSONgetObject(jsonIssue->temp_user, "organizations_url");
		if(jsonIssue->user.organizations_url == NULL)
		{
			printf("\njson : user : organizations_url is NULL");
			return;
		}

		issues->issue[issueIndex].user->organizations_url = json_string_value(jsonIssue->user.organizations_url);

		jsonIssue->user.received_events_url = npJSONgetObject(jsonIssue->temp_user, "received_events_url");
		if(jsonIssue->user.received_events_url == NULL)
		{
			printf("\njson : user : received_events_url is NULL");
			return;
		}

		issues->issue[issueIndex].user->received_events_url = json_string_value(jsonIssue->user.received_events_url);

		jsonIssue->user.repos_url = npJSONgetObject(jsonIssue->temp_user, "repos_url");
		if(jsonIssue->user.repos_url == NULL)
		{
			printf("\njson : user : repos_url is NULL");
			return;
		}

		issues->issue[issueIndex].user->repos_url = json_string_value(jsonIssue->user.repos_url);

		jsonIssue->user.site_admin = npJSONgetObject(jsonIssue->temp_user, "site_admin");
		if(jsonIssue->user.site_admin == NULL)
		{
			printf("\njson : user : site_admin is NULL");
			return;
		}

		issues->issue[issueIndex].user->site_admin = json_string_value(jsonIssue->user.site_admin);

		jsonIssue->user.starred_url = npJSONgetObject(jsonIssue->temp_user, "starred_url");
		if(jsonIssue->user.starred_url == NULL)
		{
			printf("\njson : user : starred_url is NULL");
			return;
		}

		issues->issue[issueIndex].user->starred_url = json_string_value(jsonIssue->user.starred_url);

		jsonIssue->user.subscriptions_url = npJSONgetObject(jsonIssue->temp_user, "subscriptions_url");
		if(jsonIssue->user.subscriptions_url == NULL)
		{
			printf("\njson : user : subscriptions_url is NULL");
			return;
		}

		issues->issue[issueIndex].user->subscriptions_url = json_string_value(jsonIssue->user.subscriptions_url);

		jsonIssue->user.type = npJSONgetObject(jsonIssue->temp_user, "type");
		if(jsonIssue->user.type == NULL)
		{
			printf("\njson : user : type is NULL");
			return;
		}

		issues->issue[issueIndex].user->type = json_string_value(jsonIssue->user.type);

		jsonIssue->user.site_admin = npJSONgetObject(jsonIssue->temp_user, "site_admin");
		if(jsonIssue->user.site_admin == NULL)
		{
			printf("\njson : user : site_admin is NULL");
			return;
		}

		issues->issue[issueIndex].user->avatar_image_textureID = 0;

		issues->issue[issueIndex].user->site_admin = json_integer_value(jsonIssue->user.site_admin);
	
	}
	

}

json_t* npJSONgetGithubIssueByIndex(pNPgithubIssues issues, pNPjson json, int index, int* arraySize)
{
	if(json->root == NULL)
	{
		printf("\njson root is NULL");
		return NULL;
	}

	json->data = json_array_get(json->root, index);	
	if(json->data == NULL)
	{
		printf("\njson_array_get returned null");
		return NULL;
	}

	return json->data;
}

void npJSONgetGithubIssue(pNPgithubIssues issues, int issueIndex, pNPjson json, pNPjsonGithubIssue jsonIssue, void* dataRef){
	pData data = (pData) dataRef;
	char* login = NULL;
	json_t* json_login = NULL;
	int index = 0;
	json_t* issue = NULL;
	json_t* userLogin = NULL;

	issue = npJSONgetGithubIssueByIndex(issues, json, issueIndex, &json->array_size);
	if(json->data == NULL && issue != NULL) 
	{
		printf("\nissue is NULL");
		return;
	}

	npJSONgetGithubUser(issues, jsonIssue, issue, issueIndex, dataRef);
	npJSONgetGithubIssueAssignee(issues, jsonIssue, issue, issueIndex, dataRef);

	jsonIssue->json_labels = npJSONgetObject(issue, "labels");
	if(jsonIssue->json_labels == NULL)
	{
		printf("\njson : json_labels is NULL");
		return;
	}
	
	jsonIssue->num_of_labels = json_array_size(jsonIssue->json_labels);
	issues->issue[issueIndex].num_of_labels = jsonIssue->num_of_labels;

	for(index = 0; index < jsonIssue->num_of_labels; index++)
	{
		jsonIssue->temp_label = json_array_get(jsonIssue->json_labels, index);		
		if(jsonIssue->temp_label == NULL)
		{
			printf("\njson : temp_label is NULL");
			return;
		}

		jsonIssue->labels[index].name  = npJSONgetObject(jsonIssue->temp_label, "name" );
		if(jsonIssue->labels[index].name == NULL)
		{
			printf("\njson : lables[%d].name is NULL", index);
			return;
		}

		jsonIssue->labels[index].color = npJSONgetObject(jsonIssue->temp_label, "color");
		if(jsonIssue->labels[index].color == NULL)
		{
			printf("\njson : lables[%d].color is NULL", index);
			return;
		}

		jsonIssue->labels[index].url   = npJSONgetObject(jsonIssue->temp_label, "url"  );
		if(jsonIssue->labels[index].url == NULL)
		{
			printf("\njson : lables[%d].url is NULL", index);
			return;
		}

		issues->issue[issueIndex].label[index].color = json_string_value(jsonIssue->labels[index].color);
		if(jsonIssue->labels[index].color == NULL)
		{
			printf("\njson : lables[%d].color is NULL", index);
			return;
		}

		issues->issue[issueIndex].label[index].name  = json_string_value(jsonIssue->labels[index].name );
		if(jsonIssue->labels[index].name == NULL)
		{
			printf("\njson : lables[%d].name is NULL", index);
			return;
		}

		issues->issue[issueIndex].label[index].url   = json_string_value(jsonIssue->labels[index].url  );
/*
		printf("\nlabel, color : %s, name : %s, url : %s\n", issues->issue[issueIndex].label[index].color,
			issues->issue[issueIndex].label[index].name, issues->issue[issueIndex].label[index].url);
*/
	}

	jsonIssue->body = npJSONgetObject(issue, "body");
	jsonIssue->closed_at = npJSONgetObject(issue, "closed_at");

	jsonIssue->comments_url = npJSONgetObject(issue, "comments_url");
	if(jsonIssue->comments_url == NULL)
	{
		printf("\nNULL value");
	}

	jsonIssue->created_at   = npJSONgetObject(issue, "created_at");
	if(jsonIssue->created_at == NULL)
	{
		printf("\nNULL value");
	}

	jsonIssue->events_url   = npJSONgetObject(issue, "events_url");
	if(jsonIssue->events_url == NULL)
	{
		printf("\nNULL value");
	}

	jsonIssue->html_url     = npJSONgetObject(issue, "html_url");
	if(jsonIssue->html_url == NULL)
	{
		printf("\nNULL value");
	}

	jsonIssue->id		    = npJSONgetObject(issue, "id");	
	if(jsonIssue->id == NULL)
	{	
		printf("\nNULL value");
	}

	jsonIssue->labels_url   = npJSONgetObject(issue, "labels_url");
	if(jsonIssue->labels_url == NULL)
	{
		printf("\nNULL value");
	}

	jsonIssue->locked       = npJSONgetObject(issue, "locked");
	if(jsonIssue->locked == NULL)
	{
		printf("\nNULL value");
	}

	jsonIssue->milestone    = npJSONgetObject(issue, "milestone");
	if(jsonIssue->milestone == NULL)
	{
		printf("\nNULL value");
	}

	jsonIssue->number       = npJSONgetObject(issue, "number");
	if(jsonIssue->number == NULL)
	{	
		printf("\nNULL value");
	}

	jsonIssue->state        = npJSONgetObject(issue, "state");
	if(jsonIssue->state == NULL)
	{
		printf("\nNULL value");
	}

	jsonIssue->title        = npJSONgetObject(issue, "title");
	if(jsonIssue->title == NULL)
	{
		printf("\nNULL value");
	}

	jsonIssue->updated_at   = npJSONgetObject(issue, "updated_at");
	if(jsonIssue->updated_at == NULL)
	{	
		printf("\nNULL value");
	}

	jsonIssue->url			= npJSONgetObject(issue, "url");
	if(jsonIssue->url == NULL)
	{
		printf("\nNULL value");
	}

	issues->issue[issueIndex].body         = json_string_value(jsonIssue->body);
	
	issues->issue[issueIndex].closed_at    = json_string_value(jsonIssue->closed_at);

	issues->issue[issueIndex].comments_url = json_string_value(jsonIssue->comments_url);

	issues->issue[issueIndex].created_at   = json_string_value(jsonIssue->created_at);

	issues->issue[issueIndex].events_url   = json_string_value(jsonIssue->events_url);
	
	issues->issue[issueIndex].html_url     = json_string_value(jsonIssue->html_url);

	issues->issue[issueIndex].id           = json_integer_value(jsonIssue->id);

	issues->issue[issueIndex].labels_url   = json_string_value(jsonIssue->labels_url);

	issues->issue[issueIndex].locked       = json_string_value(jsonIssue->locked);

	issues->issue[issueIndex].milestone    = json_string_value(jsonIssue->milestone);

	issues->issue[issueIndex].number       = json_integer_value(jsonIssue->number);

	issues->issue[issueIndex].state        = json_string_value(jsonIssue->state);

	issues->issue[issueIndex].title        = json_string_value(jsonIssue->title);

	issues->issue[issueIndex].updated_at   = json_string_value(jsonIssue->updated_at);

	issues->issue[issueIndex].url          = json_string_value(jsonIssue->url);
}



void npJSONissuesGetGithubIssues(pNPjson json, pNPgithub github, void* dataRef)
{
	pData data = (pData) dataRef;
	int index = 0;

	NPjsonGithubIssue jsonIssue;

//	printf("\njson->array_size : %d", json->array_size);
	github->issues->count = json->array_size;	
	for(index = 0; index < json->array_size; index++)
	{
		npJSONgetGithubIssue(github->issues, index, json, &jsonIssue, dataRef);
	}

}


json_t* new_npJSONgetGithubIssueByIndex(pNPgithub github, pNPjson json, int index, void* dataRef)
{
		
}


void npJSONgetGithubIssueUrl(pNPjsonGithubIssue jsonIssue)
{
	jsonIssue->url = npJSONgetObject(jsonIssue, "url");
}



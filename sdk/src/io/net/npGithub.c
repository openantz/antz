#include "npGithub.h"

void npCURLbuildGithubURL(pNPcurl curl, pNPgithub github, void* dataRef)
{
	

}


void npGithubGetIssues(void* dataRef)
{
	pData data = (pData) dataRef;	

	pNPgithub github = &data->io.github;

	pNPcurl curl = &data->io.curl; 
	pNPjson json = &data->io.json; 

	npCURLsetGithubRequest(github->request, 1, 200, "all", dataRef);
	npCURLsetGithubRepoName(curl, github, "openantz/antz", dataRef);
//	npCURLsetGithubRepoName(curl, github, "bagder/curl", dataRef);
//	npCURLsetGithubRepoName(curl, github, "reddit/reddit", dataRef);

	npCURLgetGithubRepoIssuesAll(curl, github, dataRef);

	npJSONloadCURLdata(curl, json, dataRef);

	npJSONissuesGetGithubIssues(json, github, dataRef); /// @todo temp	
	return;
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
//		npGitViz(&data->io.github, dataRef);
//		printf("\ngithub->num_of_users : %d", github->num_of_users);
		while(userIndex < github->num_of_users)
		{
			npCURLgetGithubUserAvatar(&data->io.curl, &data->io.github.user[userIndex], dataRef);
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

/*
void npGitVizIssue(pNPgithubIssues issues, int issueIndex, void* dataRef)
{
	pNPnode issue_creation_node;

	pNPgithubIssue current_issue;
	int yearCreatedIndex  = 0;
	int monthCreatedIndex = 0;
	int dayCreatedIndex   = 0;

	current_issue = &issues->issue[issueIndex];

	/// @todo not sure that this segment belongs here

	new_npGithubGetIssueCreatedAt(current_issue);	
	yearCreatedIndex = current_issue->issue_created_at.year - 2014; // - 2014 temporary
	monthCreatedIndex = current_issue->issue_created_at.month - 1;
	dayCreatedIndex = current_issue->issue_created_at.day;

	/// ---------------------------------------------

//	current_issue->issue_creation_node = npNodeNew(kNodePin, NULL, dataRef);

	current_issue->issue_creation_node = (pNPnode) malloc (sizeof(NPnode));
	npInitNodeDefault(current_issue->issue_creation_node); /// new
	current_issue->issue_creation_node->hide = true;
	npGitVizSetIssueCreationNodeState(current_issue, dataRef);

	if( strcmp(current_issue->state, "closed") == 0 )
	{
//		current_issue->issue_closed_node = npNodeNew(kNodePin, NULL, dataRef);
		current_issue->issue_closed_node = (pNPnode) malloc (sizeof(NPnode));
		npInitNodeDefault(current_issue->issue_closed_node);
		current_issue->issue_closed_node->hide = true; 
		npGitVizSetIssueClosedNodeState(current_issue, dataRef);
	}

}
*/

void npGithubCtrlSetCurrentIssue(pNPgithubIssues issues, void* dataRef)
{
    //issue->partOf->current = issue;
    issues->current = &issues->issue[issues->index];
}

void npGithubCtrlSetRecordTagIndex(pNPgithubIssues issues, int index, void* dataRef)
{
  //  pNPgithubIssue current_issue = npGithubCtrlGetCurrentIssue(issue, dataRef);
  //  current_issue->partOf->recordTagIndex = index;
    //issue->partOf->recordTagIndex = index;
    issues->recordTagIndex = index;
}

void new_npGithubGetIssueCreatedAt(pNPgithubIssue issue)
{
    char* ptr = issue->created_at;
    char str[6][5] = {'\0'};
    char* symbol[] = {'-','-','T',':',':', 'Z'};
    int index = 0;
    size_t symbolArraySize = 0;
    symbolArraySize = sizeof(symbol) / sizeof(char*);
    
  //  issue->issue_created_at = malloc(sizeof(NPgithubIssueCreatedAt));
    
    for(index = 0; index <= (int)symbolArraySize-1; index++)
    {
        ptr = dumpTill(ptr, str[index], symbol[index]);
        ptr++;
       // printf("\n%s", str[index]);
    }
    
    issue->issue_created_at.year  = atoi(str[0]);
    issue->issue_created_at.month = atoi(str[1]);
    issue->issue_created_at.day   = atoi(str[2]);
    
    issue->issue_created_at.hour  = atoi(str[3]);
    issue->issue_created_at.minute= atoi(str[4]);
    issue->issue_created_at.second= atoi(str[5]);
    
}


void new_npGithubGetIssueClosedAt(pNPgithubIssue issue)
{
    char* ptr = issue->closed_at;
    char str[6][5] = {'\0'};
//    char* symbol[] = {'-','-','T',':',':', 'Z'};
    char symbol[] = {'-','-','T',':',':', 'Z'};
	int index = 0;
    size_t symbolArraySize = 0;
    symbolArraySize = sizeof(symbol) / sizeof(char*);
//	printf("\nsizeof(symbol) : %d", sizeof(symbol));
//	printf("\nsymbolArraySize : %d", symbolArraySize);

    if(ptr != NULL)
    {
       issue->issue_closed_at = malloc(sizeof(NPgithubIssueCreatedAt));
    }
    else
    {
        return;
    }
        
  //  for(index = 0; index <= (int)symbolArraySize-1; index++)
	for(index = 0; index < sizeof(symbol); index++)
	{
        ptr = dumpTill(ptr, str[index], symbol[index]);
        ptr++;
       // printf("\n%s", str[index]);
    }
    
    issue->issue_closed_at->year  = atoi(str[0]);
    issue->issue_closed_at->month = atoi(str[1]);
    issue->issue_closed_at->day   = atoi(str[2]);
    
    issue->issue_closed_at->hour  = atoi(str[3]);
    issue->issue_closed_at->minute= atoi(str[4]);
    issue->issue_closed_at->second= atoi(str[5]);
    
}

/*
void new_new_gitVizTest(pNPgithub github, void* dataRef)
{
	pNPnode year_node[2];
	pNPnode dayCreated_node;
	pNPnode dayClosed_node;
	pNPnode link;

	pNPgithubIssue current_issue;
	int issueIndex = 0;
	int yearCreatedIndex = 0;
	int monthCreatedIndex = 0;
	int dayCreatedIndex = 0;
	int childCount = 0;

	int yearClosedIndex = 0;
	int monthClosedIndex = 0;
	int dayClosedIndex = 0;

	new_npCreateTimeViz(&year_node, dataRef); 
	for(github->issues->index = 0; github->issues->index < github->issues->count; github->issues->index++)
	{
		npGitVizIssue(github, github->issues->index, dataRef); /// @todo npGitVizAllIssues
		github->issues->current = &github->issues->issue[github->issues->index];	

		new_npGithubGetIssueCreatedAt(github->issues->current);	

		yearCreatedIndex = github->issues->current->issue_created_at.year - 2014; // 2014 temporary

		monthCreatedIndex = github->issues->current->issue_created_at.month - 1;

		dayCreatedIndex = github->issues->current->issue_created_at.day;

//		printf("\nIndexes, Year : %d, Month : %d, Day : %d", yearCreatedIndex, monthCreatedIndex, dayCreatedIndex);
		
		dayCreated_node = year_node[yearCreatedIndex]->child[monthCreatedIndex]->child[dayCreatedIndex];
		dayCreated_node->translate.x = (11.61290 * dayCreatedIndex);
		year_node[yearCreatedIndex]->child[monthCreatedIndex]->hide = false;

		dayCreated_node->hide = false;

		childCount = dayCreated_node->childCount;

		dayCreated_node->child[childCount] = github->issues->current->issue_creation_node;

		npShowIssueCreationNode(github->issues->current, dataRef);
		npSetIssueCreationNodeParent(github->issues->current, dayCreated_node, dataRef);
		npSetIssueCreationNodeGeometry(github->issues->current, kNPgeoTorus, dataRef);
		npSetIssueCreationNodeTopo(github->issues->current, kNPtopoTorus, dataRef);

		dayCreated_node->childCount++;		
		if(github->issues->current->issue_closed_node != NULL)
		{	
			new_npGithubGetIssueClosedAt(github->issues->current);

			yearClosedIndex = github->issues->current->issue_closed_at->year - 2014; // - 2014 temporary
			monthClosedIndex = github->issues->current->issue_closed_at->month - 1;
			dayClosedIndex = github->issues->current->issue_closed_at->day;
			
			dayClosed_node = year_node[yearClosedIndex]->child[monthClosedIndex]->child[dayClosedIndex];
			dayClosed_node->translate.x = (11.61290 * dayClosedIndex);
			year_node[yearClosedIndex]->child[monthClosedIndex]->hide = false;
			dayClosed_node->hide = false;

			childCount = dayClosed_node->childCount;
			dayClosed_node->child[childCount] = github->issues->current->issue_closed_node;
//			dayClosed_node->child[childCount] = current_issue->issue_closed_node;
			
			npShowIssueClosedNode(github->issues->current, dataRef);
			npSetIssueClosedNodeParent(github->issues->current, dayClosed_node, dataRef);
			npSetIssueClosedNodeGeometry(github->issues->current, kNPgeoSphere, dataRef);
			npSetIssueClosedNodeTopo(github->issues->current, kNPtopoSphere, dataRef);

			dayClosed_node->childCount++;
			link = npLinkIssueCreationNodeToIssueClosedNode(github->issues->current->issue_creation_node, github->issues->current->issue_closed_node, dataRef);
			link->geometry = 20;

		}

	}

}
*/

void npGithubCtrlSetCurrentIssueIndex(pNPgithubIssues issues, int index, void* dataRef)
{
    issues->index = index;
}

void npGithubIssuesInit(pNPgithubIssues issues, void* dataRef)
{
	int index = 0;

	printf("\nnext 1");
	issues->count = 0;
	issues->current = NULL;
	issues->index = 0;

	for(index = 0; index < 200; index++)
	{
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
		issues->issue[index].locked = NULL;
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
	}
	

	printf("\nnext 2");
}

void npGithubInit(pNPgithub github, void* dataRef)
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
		return;
	}
	data->io.github.issues->running = false;

	github->request = malloc(sizeof(NPgithubRequest));
	if(github->request == NULL)
	{
		printf("\nmalloc failed");
		return;
	}

	github->request->page = 0;
	github->request->per_page = 200;
	github->request->state = NULL;

	github->num_of_users = 0;
	for(index = 0; index < 20; index++)
	{
		github->user[index].avatar_image_file = NULL;
		github->user[index].avatar_image_file_path = NULL;
		github->user[index].avatar_image_textureID = NULL;
		github->user[index].avatar_url = NULL;
		github->user[index].events_url = NULL;
		github->user[index].followers_url = NULL;
		github->user[index].gists_url = NULL;
		github->user[index].gravatar_id = NULL;
		github->user[index].html_url = NULL;
		github->user[index].id = NULL;
		github->user[index].login = NULL;
		github->user[index].number = -1;
		github->user[index].organizations_url = NULL;
		github->user[index].received_events_url = NULL;
		github->user[index].repos_url = NULL;
		github->user[index].site_admin = NULL;
		github->user[index].starred_url = NULL;
		github->user[index].subscriptions_url = NULL;
		github->user[index].type = NULL;
		github->user[index].url = NULL;
	}

	printf("\nnpGithubInit");
	npGithubIssuesInit(github->issues, dataRef); 
}



void npGithubGetIssueClosedAt(pNPgithubIssue issue)
{
    char* ptr  = issue->closed_at;
    char year_str[5] = {'\0'};
    char month_str[3] = {'\0'};
    char day_str[3] = {'\0'};
    char hour_str[3] = {'\0'};
    char minute_str[3] = {'\0'};
    char second_str[3] = {'\0'};
    
    int index = 0;
    
    issue->issue_closed_at = malloc(sizeof(NPgithubIssueCreatedAt));
    
    if(ptr == NULL)
    {
        printf("\nIssue has not been closed");
        return;
    }
    
    while((*ptr) != '-')
    {
        year_str[index] = (*ptr);
        index++; ptr++;
    }
    
    printf("\nUpdate Year String : %s\n", year_str);
    issue->issue_closed_at->year = atoi(year_str);
    
    ptr++;
    index = 0;
    while((*ptr) != '-')
    {
        month_str[index] = (*ptr);
        index++; ptr++;
    }
    
    printf("\nUpdate Month String : %s\n", month_str);
    issue->issue_closed_at->month = atoi(month_str);
    
    ptr++;
    index = 0;
    while((*ptr) != 'T')
    {
        day_str[index] = (*ptr);
        index++; ptr++;
    }
    
    printf("\nUpdate Day String : %s\n", day_str);
    issue->issue_closed_at->day = atoi(day_str);
    
    ptr++;
    index = 0;
    while((*ptr) != ':')
    {
        hour_str[index] = (*ptr);
        index++; ptr++;
    }
    
    printf("\nUpdate Hour String : %s\n", hour_str);
    issue->issue_closed_at->hour = atoi(hour_str);
    
    ptr++;
    index = 0;
    while((*ptr) != ':')
    {
        minute_str[index] = (*ptr);
        index++; ptr++;
    }
    
    printf("\nUpdate Minute String : %s\n", minute_str);
    issue->issue_closed_at->minute = atoi(minute_str);
    
    ptr++;
    index = 0;
    while((*ptr) != 'Z')
    {
        second_str[index] = (*ptr);
        index++; ptr++;
    }
    
    printf("\nUpdate Second String : %s\n", second_str);
    issue->issue_closed_at->second = atoi(second_str);
    
}


int npGithubNewIssueCreationGlyph(pNPgithubIssue issue, void* dataRef)
{
	/// @todo	
	return 0;
}


int npGithubNewIssueGlyphs(pNPgithubIssue issue, void* dataRef)
{
	static int id = 0;

	if(issue == NULL)	
	{
		printf("\nIssue is NULL");
		return NULL;
	}


  //  npGithubGetIssueCreatedAtYear(issue); /// @todo Rename to npGithubGetIssueCreatedAt
    new_npGithubGetIssueCreatedAt(issue);
    npGithubGetIssueClosedAt(issue);
    issue->issue_node->colorIndex = 2;
    
	issue->issue_creation_node = npNodeNew(kNodePin, NULL, dataRef);
	issue->issue_creation_node->geometry = kNPgeoTetrahedron;
	issue->issue_creation_node->color.r = 255;
	issue->issue_creation_node->color.g = 0;
	issue->issue_creation_node->color.b = 0;
    
    /// -----------------
    if(strcmp(issue->state, "closed") == 0)
    {
		issue->issue_closed_node = npNodeNew(kNodePin, NULL, dataRef); /// Init this @todo
		issue->issue_closed_node->geometry = kNPgeoTorus;
		issue->issue_closed_node->color.r = 0;
		issue->issue_closed_node->color.g = 0;
		issue->issue_closed_node->color.b = 255;
    }
    
    return 1;
}

pNPnode npGithubNewIssueGlyph(pNPgithubIssue issue, void* dataRef)
{
	/*
    pNPrecordTag title_recordTag = NULL;
    pNPnode issue_node = NULL;
    
    static int id = 1;
  
    if(issue == NULL)
    {
        printf("\nIssue is NULL");
        return NULL;
    }
    
    issue_node = npNodeNew(kNodePin, NULL, dataRef);
    
  //  printf("23 parent : %p", issue->partOf->parent);
    if(issue_node == NULL)
    {
        printf("\nnpNodeNew Failed");
        return NULL;
    }
    /// Tag the node
    
     /// now set the properties of the issue node
    //issue->partOf->recordTagIndex = 0;
//    npGithubIssueInitNodeTagIndex(issue->partOf, dataRef);
//    issue->partOf->index = 0;
//    issue->partOf->index = issue->index;
    issue->partOf->index = id++;
   // issue->partOf->recordTagIndex = 0;
    issue->partOf->recordTagVariableFormatIndex = 0;
    issue->partOf->recordTagVariableIndex = 0;
    title_recordTag = npGithubIssueInitNodeTag(issue, dataRef);
    issue_node->recordID = title_recordTag->recordID = id;
    npTagSortAdd(title_recordTag, dataRef);
    npTagNode(issue_node, dataRef);
    

  //  npGithubIssueInitNodeTagTitle(issue, dataRef);
    
     //issue_node->id = issue->number;

//     title_recordTag = npNewTag( dataRef );
//     sprintf(title_recordTag->title, "%s", issue->title);
     //    strcpy( recordTag->title, "TIA" );
//     title_recordTag->titleSize = strlen(issue->title) + 1;
//     issue_node->recordID = title_recordTag->recordID = id;
//     id++;
    
    
    
    
     
     /// @todo npTagGithubIssueNode
    
    issue_node->topo = kNPtopoTorus;
    issue_node->geometry = kNPgeoTorus;
    issue_node->translate.x += 50;
    
  //  npGithubGetIssueCreatedAtYear(issue); /// @todo Rename to npGithubGetIssueCreatedAt
    new_npGithubGetIssueCreatedAt(issue);
    npGithubGetIssueClosedAt(issue);
    issue_node->colorIndex = 2;
    
    issue_state_node = npNodeNew(kNodePin, issue_node, dataRef);
    issue_state_node->geometry = kNPgeoTorus;
    
    created_at_node = npNodeNew(kNodePin, issue_node, dataRef); // year
    created_at_node->translate.y = (issue->issue_created_at->year / 2015) * 180;
    if(issue->issue_created_at->year > 1007)
        created_at_node->translate.y *= -1;
    
    created_at_node->color.b = 0;
    created_at_node->color.g = 255;
    created_at_node->color.r = 0;
    
    
    year_recordTag = npNewTag( dataRef );
    sprintf(year_recordTag->title, "%d", issue->issue_created_at->year);
    //    strcpy( recordTag->title, "TIA" );
    year_recordTag->titleSize = strlen(issue->title) + 1;
    created_at_node->recordID = year_recordTag->recordID = id;
    id++;
    
    npTagSortAdd(year_recordTag, dataRef);
    
    /// @todo npTagGithubIssueNode
    npTagNode(created_at_node, dataRef);
    
    created_at_month = npNodeNew(kNodePin, created_at_node, dataRef);
    created_at_month->translate.y = ( ((issue->issue_created_at)->month / 12) * 180) + 180;
    if(issue->issue_created_at->month > 6)
        created_at_node->translate.y *= -1;
    
    month_recordTag = npNewTag( dataRef );
    sprintf(month_recordTag->title, "%d", issue->issue_created_at->month);
    //    strcpy( recordTag->title, "TIA" );
    month_recordTag->titleSize = strlen(issue->title) + 1;
    created_at_month->recordID = month_recordTag->recordID = id;
    id++;
    
    npTagSortAdd(month_recordTag, dataRef);
    
    /// @todo npTagGithubIssueNode
    npTagNode(created_at_month, dataRef);
    
    
    created_at_day = npNodeNew(kNodePin, created_at_month, dataRef);
    created_at_day->translate.y = ((issue->issue_created_at)->day / 30) * 180;
    if(issue->issue_created_at->day > 15)
        created_at_node->translate.y *= -1;
    
    day_recordTag = npNewTag( dataRef );
    sprintf(day_recordTag->title, "%d", issue->issue_created_at->day);
    //    strcpy( recordTag->title, "TIA" );
    day_recordTag->titleSize = strlen(issue->title) + 1;
    created_at_day->recordID = day_recordTag->recordID = id;
    id++;
    
    npTagSortAdd(day_recordTag, dataRef);
    
    /// @todo  npTagGithubIssueNode
    npTagNode(created_at_day, dataRef);
    
    
    issue_node_time_hour = npNodeNew(kNodePin, created_at_day, dataRef);
    issue_node_time_hour->translate.y = (issue->issue_created_at->hour / 24) * 180;
    if(issue->issue_created_at->hour > 12)
        issue_node_time_hour->translate.y *= -1;
    
    hour_recordTag = npNewTag( dataRef );
    sprintf(hour_recordTag->title, "%d", issue->issue_created_at->hour);
    //    strcpy( recordTag->title, "TIA" );
    hour_recordTag->titleSize = strlen(issue->title) + 1;
    issue_node_time_hour->recordID = hour_recordTag->recordID = id;
    id++;
    
    npTagSortAdd(hour_recordTag, dataRef);
    
    /// @todo npTagGithubIssueNode
    npTagNode(issue_node_time_hour, dataRef);
    
    
    issue_node_time_minute = npNodeNew(kNodePin, issue_node_time_hour, dataRef);
    issue_node_time_minute->translate.y = ((issue->issue_created_at->minute / 60) * 180) + 90;
    if(issue->issue_created_at->minute > 30)
        issue_node_time_minute->translate.y *= -1;
    
    /// -----------------
    minute_recordTag = npNewTag( dataRef );
    sprintf(minute_recordTag->title, "%d", issue->issue_created_at->minute);
    minute_recordTag->titleSize = strlen(issue->title) + 1;
    issue_node_time_minute->recordID = minute_recordTag->recordID = id;
    id++;
    
    npTagSortAdd(minute_recordTag, dataRef);
    
    /// @todo npTagGithubIssueNode
    npTagNode(issue_node_time_minute, dataRef);
    
    /// -----------------
    
    issue_node_time_second = npNodeNew(kNodePin, issue_node_time_minute, dataRef);
    issue_node_time_second->translate.y = ((issue->issue_created_at->second / 60) * 180) + 90;
    if(issue->issue_created_at->second > 30)
        issue_node_time_second->translate.y *= -1;
    
    second_recordTag = npNewTag( dataRef );
    sprintf(second_recordTag->title, "%d", issue->issue_created_at->second);
    //    strcpy( recordTag->title, "TIA" );
    second_recordTag->titleSize = strlen(issue->title) + 1;
    issue_node_time_second->recordID = second_recordTag->recordID = id;
    id++;
    
    npTagSortAdd(second_recordTag, dataRef);
    
    /// @todo npTagGithubIssueNode
    npTagNode(issue_node_time_second, dataRef);
    
    if(strcmp(issue->state, "closed") == 0)
    {
        issue_state_node->geometry = kNPgeoTorus;
        issue_state_node->color.r = 0;
        issue_state_node->color.g = 0;
        issue_state_node->color.b = 255;
        
        /// now set the y translation of this sub node
        
    }
    else
    {
        /// open
        issue_state_node->geometry = kNPgeoTetrahedron;
        issue_state_node->color.r = 255;
        issue_state_node->color.g = 0;
        issue_state_node->color.b = 0;
    }
    
    return issue_node;
	*/
}
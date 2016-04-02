/* -----------------------------------------------------------------------------
*
*  npgitviz.c
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
#include "npgitviz.h"

#include "../../data/npmap.h"
#include "../gl/nptags.h"
//#include "npGithub.h"

#include <time.h>
#include "soil.h"
/// @todo npCreateTimeDayViz2 should be npInitTimeDayViz2

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

/*
pNPnode npCreateTimeDayViz2(int day, pNPnode parent, void* dataRef)
{
	pNPnode day_node;
	pNPrecordTag day_recordTag;
	static int id = 0;

	day_node = npNodeNew(kNodePin, parent, dataRef);
	
	day_recordTag = npNewTag( dataRef );
	//sprintf(year_recordTag->title, "%d", yearIndex + 2014);
	sprintf(day_recordTag->title, "%d", day);
	day_recordTag->titleSize = strlen(day_recordTag->title) + 1;
	day_recordTag->recordID = id+1;

	day_node->recordID = id+1;
	id++;

	npTagSortAdd(day_recordTag, dataRef);
	npTagNode(day_node, dataRef);

	return day_node;
}
*/

/*
pNPnode npCreateTimeMonthViz2(char* month, pNPnode parent, void* dataRef)
{
	pNPnode month_node;
	pNPrecordTag month_recordTag;
	static int id = 0;

	month_node = npNodeNew(kNodePin, parent, dataRef);
	
	month_recordTag = npNewTag( dataRef );
	//sprintf(year_recordTag->title, "%d", yearIndex + 2014);
	sprintf(month_recordTag->title, "%s", month);
	month_recordTag->titleSize = strlen(month_recordTag->title) + 1;
	month_recordTag->recordID = id+1;

	month_node->recordID = id+1;
	id++;

	npTagSortAdd(month_recordTag, dataRef);
	npTagNode(month_node, dataRef);

	return month_node;
}
*/

/*
pNPnode new_npCreateTimeViz2(pNPnode year_node[], void* dataRef)
{
//	pNPnode year_node[2];
	pNPnode month_node;
	pNPnode day_node;

	int yearIndex  = 0;
	int monthIndex = 0;
	int dayIndex   = 0;

	char* monthString[] = {"January", "February", "March", "April", "May", "June", "July", "August", "September", "October", "November", "December"}; 

	printf("\n2354 new_npCreateTimeViz");
	for(yearIndex = 0; yearIndex < 2; yearIndex++)
	{
		year_node[yearIndex] = npCreateTimeYearViz(yearIndex + 2014, dataRef);	
		for(monthIndex = 0; monthIndex < 12; monthIndex++)
		{
			month_node = npCreateTimeMonthViz2(monthString[monthIndex], year_node[yearIndex], dataRef);	
			month_node->translate.x = (-30 * monthIndex);
			month_node->hide = true;
			/// @todo Attach to year node
			for(dayIndex = 0; dayIndex <= 31; dayIndex++)
			{
				day_node = npCreateTimeDayViz2(dayIndex, month_node, dataRef);
				day_node->translate.x = (11.61290 * dayIndex);
				day_node->hide = true;
			}
		}
	}

return year_node;	
}
*/

void npGitVizSetIssueCreationNodeState2(pNPgithubIssue issue, void* dataRef)
{
	pNPnode node = (pNPnode)issue->issue_creation_node;

	node->geometry = kNPgeoTorus;
	node->colorIndex = 1;	// red
	node->color.r  = 255;
	node->color.g  = 0;
	node->color.b  = 0;	
}

void test_npGithubGetIssueCreatedAt(pNPgithubIssue issue, void* dataRef)
{
	char* createdAt_ptr = &issue->created_at[0];
	char buffer[10] = {'\0'};
	int buffer_index = 0;	

//	printf("\nissue->created_at : %s", issue->created_at);
	if( strcmp(issue->created_at, "(null)") == 0 )
	{
		printf("\nInvalid Input");
		getchar();
	}

// ---------------------------------------------------------------------------
	/// Get the year
	while( (*createdAt_ptr) != '-')
	{
		buffer[buffer_index] = (*createdAt_ptr);
	//	printf("\nbuffer[%d] : %c", buffer_index, buffer[buffer_index]);
		buffer_index++; createdAt_ptr++;
	}

	createdAt_ptr++;

	issue->issue_created_at.year = atoi(buffer);
	//printf("\nissue->issue_created_at.year = %d", issue->issue_created_at.year); 
	
	memset(buffer, '\0', 10);
	buffer_index = 0;

// ---------------------------------------------------------------------------
	while( (*createdAt_ptr) != '-')
	{
		buffer[buffer_index] = (*createdAt_ptr);
	//	printf("\nbuffer[%d] : %c", buffer_index, buffer[buffer_index]);
		buffer_index++; createdAt_ptr++;
	}

	createdAt_ptr++;

	issue->issue_created_at.month = atoi(buffer);
	//printf("\nissue->issue_created_at.month = %d", issue->issue_created_at.month); 
	
	memset(buffer, '\0', 10);
	buffer_index = 0;
// ---------------------------------------------------------------------------
	while( (*createdAt_ptr) != 'T')
	{
		buffer[buffer_index] = (*createdAt_ptr);
	//	printf("\nbuffer[%d] : %c", buffer_index, buffer[buffer_index]);
		buffer_index++; createdAt_ptr++;
	}

	createdAt_ptr++;

	issue->issue_created_at.day = atoi(buffer);
	//printf("\nissue->issue_created_at.day = %d", issue->issue_created_at.day); 
	
	memset(buffer, '\0', 10);
	buffer_index = 0;
// ---------------------------------------------------------------------------
	while( (*createdAt_ptr) != ':')
	{
		buffer[buffer_index] = (*createdAt_ptr);
	//	printf("\nbuffer[%d] : %c", buffer_index, buffer[buffer_index]);
		buffer_index++; createdAt_ptr++;
	}

	createdAt_ptr++;

	issue->issue_created_at.hour = atoi(buffer);
	//printf("\nissue->issue_created_at.hour = %d", issue->issue_created_at.hour); 
	
	memset(buffer, '\0', 10);
	buffer_index = 0;
// ---------------------------------------------------------------------------
	while( (*createdAt_ptr) != ':')
	{
		buffer[buffer_index] = (*createdAt_ptr);
	//	printf("\nbuffer[%d] : %c", buffer_index, buffer[buffer_index]);
		buffer_index++; createdAt_ptr++;
	}

	createdAt_ptr++;

	issue->issue_created_at.minute = atoi(buffer);
	//printf("\nissue->issue_created_at.minute = %d", issue->issue_created_at.minute); 
	
	memset(buffer, '\0', 10);
	buffer_index = 0;
// ---------------------------------------------------------------------------
	while( (*createdAt_ptr) != 'Z')
	{
		buffer[buffer_index] = (*createdAt_ptr);
	//	printf("\nbuffer[%d] : %c", buffer_index, buffer[buffer_index]);
		buffer_index++; createdAt_ptr++;
	}

	createdAt_ptr++;

	issue->issue_created_at.second = atoi(buffer);
	//printf("\nissue->issue_created_at.second = %d", issue->issue_created_at.second); 
	
	memset(buffer, '\0', 10);
	buffer_index = 0;
}


void old_npGithubGetIssueCreatedAt2(pNPgithubIssue issue)
{
    char* ptr = issue->created_at;
    char str[6][5] = {'\0'};
    char symbol[] = {'-','-','T',':',':', 'Z'};
    int index = 0;
    size_t symbolArraySize = 0;
    symbolArraySize = sizeof(symbol) / sizeof(char*);
    
//	printf("\nissue->created_at :: %s", ptr);
  //  issue->issue_created_at = malloc(sizeof(NPgithubIssueCreatedAt));
    
    for(index = 0; index <= (int)symbolArraySize-1; index++)
    {
        ptr = dumpTill(ptr, str[index], &symbol[index]);
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
    
//    printf("\nUpdate Year String : %s\n", year_str);
    issue->issue_closed_at->year = atoi(year_str);
    
    ptr++;
    index = 0;
    while((*ptr) != '-')
    {
        month_str[index] = (*ptr);
        index++; ptr++;
    }
    
//    printf("\nUpdate Month String : %s\n", month_str);
    issue->issue_closed_at->month = atoi(month_str);
    
    ptr++;
    index = 0;
    while((*ptr) != 'T')
    {
        day_str[index] = (*ptr);
        index++; ptr++;
    }
    
//    printf("\nUpdate Day String : %s\n", day_str);
    issue->issue_closed_at->day = atoi(day_str);
    
    ptr++;
    index = 0;
    while((*ptr) != ':')
    {
        hour_str[index] = (*ptr);
        index++; ptr++;
    }
    
//    printf("\nUpdate Hour String : %s\n", hour_str);
    issue->issue_closed_at->hour = atoi(hour_str);
    
    ptr++;
    index = 0;
    while((*ptr) != ':')
    {
        minute_str[index] = (*ptr);
        index++; ptr++;
    }
    
//    printf("\nUpdate Minute String : %s\n", minute_str);
    issue->issue_closed_at->minute = atoi(minute_str);
    
    ptr++;
    index = 0;
    while((*ptr) != 'Z')
    {
        second_str[index] = (*ptr);
        index++; ptr++;
    }
    
//    printf("\nUpdate Second String : %s\n", second_str);
    issue->issue_closed_at->second = atoi(second_str);
    
}

/// @todo rewrite this function
void new_npGithubGetIssueClosedAt2(pNPgithubIssue issue)
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
        ptr = dumpTill(ptr, str[index], &symbol[index]);
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
pNPnode npLinkIssueCreationNodeToIssueClosedNode2(pNPnode issue_creation_node, pNPnode issue_closed_node, void* dataRef)
{
	pNPnode link;
		link = npNodeNewLink(issue_creation_node, issue_closed_node, dataRef);
	return link;
}
*/

/* Temporarily commented out, lde /// @todo
void npGitVizIssue2(pNPgithub github, int issueIndex, void* dataRef)
{
	pNPnode issue_creation_node;

	pNPgithubIssue current_issue;
	int yearCreatedIndex  = 0;
	int monthCreatedIndex = 0;
	int dayCreatedIndex   = 0;

	current_issue = github->issue[issueIndex];

	/// @todo not sure that this segment belongs here

//	new_npGithubGetIssueCreatedAt(current_issue);	
//	yearCreatedIndex = current_issue->issue_created_at.year - 2014; // - 2014 temporary
//	monthCreatedIndex = current_issue->issue_created_at.month - 1;
//	dayCreatedIndex = current_issue->issue_created_at.day;

	/// ---------------------------------------------

//	current_issue->issue_creation_node = npNodeNew(kNodePin, NULL, dataRef);

	current_issue->issue_creation_node = (pNPnode) malloc (sizeof(NPnode));
	npInitNodeDefault(current_issue->issue_creation_node); /// new
	current_issue->issue_creation_node->hide = true;
	npGitVizSetIssueCreationNodeState2(current_issue, dataRef);

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

void theNew_npGitVizIssue2(pNPgithub github, int issueIndex, void* dataRef)
{
	pData data = (pData) dataRef;
	pNPgithubIssue current_issue = NULL;
	pNPnode issue_user = NULL;
	time_t now;
//	pNPtag issueTitleTag;
	pNPrecordTag issueTitleTag;
	pNPrecordTag bugTag;
	pNPrecordTag enhancementTag;
	pNPrecordTag durationTag;
	pNPrecordTag issueCreatorTag;
	pNPrecordTag issueAssigneeTag;
	pNPrecordTag firstPriorityTag;
	pNPrecordTag secondPriorityTag;

	pNPnode node = NULL;

	struct tm time_created;
	struct tm time_closed;
	double seconds;
	static int id = 1;
	int index = 0;
	int numWordsInTitle = 0;
	char* titlePtr = NULL;
	int numLettersInTitle = 0;

	time(&now);
	time_created = *localtime(&now);
	time_closed  = *localtime(&now);

//	printf("\nissue index : %d", issueIndex);
	if(issueIndex < 0 || issueIndex >= kNPgithubMaxIssues)
	{
		printf("\nissue index : out of domain");
		getchar();
		return;
	}

	current_issue = &github->issues->issue[issueIndex];
/*	
	printf("\nissue number : %d", current_issue->number);
	getchar();
*/
	// ---------------------------------------------------------------------------
	
	durationTag = npNewTag(dataRef);

	durationTag->recordID = current_issue->number;
	durationTag->tableID = kNPgitvizTableID + 1; //zz 3;

		
	// ---------------------------------------------------------------------------
	current_issue->issue_node = node = npNodeNew(kNodePin, NULL, dataRef);
	node->geometry = kNPgeoCube;

//	old_npGithubGetIssueCreatedAt2(current_issue, dataRef);
//	new_npGithubGetIssueCreatedAt2(current_issue);	
	test_npGithubGetIssueCreatedAt(current_issue, dataRef);
	
	/// 
	time_created.tm_year = current_issue->issue_created_at.year - 1900;
	time_created.tm_mon  = current_issue->issue_created_at.month - 1;
	time_created.tm_mday = current_issue->issue_created_at.day;
	time_created.tm_hour = current_issue->issue_created_at.hour;
	time_created.tm_min  = current_issue->issue_created_at.minute;
	time_created.tm_sec  = current_issue->issue_created_at.second;

	if( strcmp(current_issue->state, "open") == 0 )
	{
		sprintf(durationTag->title, "Creation Date : %s", current_issue->created_at);
		
		node->colorIndex = 1;	// red
		node->color.r = 255;
		node->color.g = 0;
		node->color.b = 0;
		
		seconds = difftime(now, mktime(&time_created));
//		printf("%.f seconds have passed since the issue was created \n", seconds);
	//	seconds += 10000;
//		current_issue->issue_node->scale.z = seconds / 12500000;
		node->scale.z = (float)(seconds / 10000000);
//		printf("\ncurrent_issue->issue_node->scale.z :: %f", current_issue->issue_node->scale.z);  	
	}
	else if( strcmp(current_issue->state, "closed") == 0 )
	{
		sprintf(durationTag->title, "Creation Date : %s || Closed Date : %s", current_issue->created_at, current_issue->closed_at);
		
		node->colorIndex = 3;	// blue
		node->color.r = 0;
		node->color.g = 0;
		node->color.b = 255;
		
	//	new_npGithubGetIssueClosedAt2(current_issue);
		npGithubGetIssueClosedAt(current_issue);

		time_closed.tm_year = current_issue->issue_closed_at->year - 1900;
		time_closed.tm_mon  = current_issue->issue_closed_at->month - 1;
		time_closed.tm_mday = current_issue->issue_closed_at->day;
		time_closed.tm_hour = current_issue->issue_closed_at->hour;
		time_closed.tm_min  = current_issue->issue_closed_at->minute;
		time_closed.tm_sec  = current_issue->issue_closed_at->second;
		seconds = difftime(mktime(&time_closed), mktime(&time_created));
	//	printf("%.f seconds to close the issue\n", seconds);
	//	////getchar();
	//	seconds += 10000;
		//current_issue->issue_node->scale.z = seconds / 12500000;
		node->scale.z = (float)(seconds / 10000000);
	}
	
	durationTag->titleSize = strlen (durationTag->title) + 1;
	npTagSortAdd (durationTag, dataRef);
	node->recordID = durationTag->recordID;
	node->tableID  = kNPgitvizTableID + 1; //zz 3;
	npTagNode (node, dataRef);

	/// Set the Title as text tag	
//	printf("\nTag");
	issueTitleTag = npNewTag(dataRef);
	sprintf(issueTitleTag->title, "%s", current_issue->title);
	issueTitleTag->titleSize = strlen(current_issue->title) + 1;

	// ---------------------------------------------------------------------------
	
	enhancementTag = npNewTag(dataRef);
	sprintf(enhancementTag->title, "%s", "enhancement");
	enhancementTag->titleSize = strlen("enhancement") + 1;

	enhancementTag->recordID = current_issue->number;
	enhancementTag->tableID = kNPgitvizTableID + 6; //zz 6;

	npTagSortAdd(enhancementTag, dataRef);
		
	// ---------------------------------------------------------------------------
	
	bugTag = npNewTag(dataRef);
	sprintf(bugTag->title, "%s", "bug");
	bugTag->titleSize = strlen("bug") + 1;

	bugTag->recordID = current_issue->number;
	bugTag->tableID = kNPgitvizTableID + 5; //zz 5;

	npTagSortAdd(bugTag, dataRef);
	
	// ---------------------------------------------------------------------------
		
//	printf("\ncurrent_issue->user ptr : %p", current_issue->user);
//	printf("\nlogin ptr : %p", current_issue->user->login);
//	printf("\nlogin str : %s", current_issue->user->login);
//	printf("\ncurrent_issue->user->login : %p %s\n", current_issue->user->login, current_issue->user->login);
	issueCreatorTag = npNewTag(dataRef);
	sprintf(issueCreatorTag->title, "Opened By : %s", current_issue->user->login);
	issueCreatorTag->titleSize = strlen(issueCreatorTag->title) + 1;

	issueCreatorTag->recordID = current_issue->number;
	issueCreatorTag->tableID = kNPgitvizTableID + 4; //zz 4;

	npTagSortAdd(issueCreatorTag, dataRef);
	
	// ---------------------------------------------------------------------------

	firstPriorityTag = npNewTag(dataRef);
	sprintf(firstPriorityTag->title, "%s", "1st Priority");
	firstPriorityTag->titleSize = strlen(firstPriorityTag->title) + 1;

	firstPriorityTag->recordID = current_issue->number;
	firstPriorityTag->tableID = kNPgitvizTableID + 8; //zz 8;

	// ---------------------------------------------------------------------------

	secondPriorityTag = npNewTag(dataRef);
	sprintf(secondPriorityTag->title, "%s", "2nd Priority");
	secondPriorityTag->titleSize = strlen(secondPriorityTag->title) + 1;

	secondPriorityTag->recordID = current_issue->number;
	secondPriorityTag->tableID = kNPgitvizTableID + 9; //zz 9;

	npTagSortAdd(secondPriorityTag, dataRef);

	// ---------------------------------------------------------------------------

	npTagSortAdd(issueCreatorTag, dataRef);
	///	Set the recordId to the issue number
	/// @todo create a method to auto-generate the table_id
	node->recordID = (issueTitleTag->recordID = current_issue->number);
	node->tableID =  (issueTitleTag->tableID = kNPgitvizTableID + 2); //zz 2);
	
	/// Tag title text tag to node
	npTagSortAdd(issueTitleTag, dataRef);
//	npTagNode(current_issue->issue_node, dataRef);
//	id++;

	
	/// Add sub-node that indicates user that created the issue
	issue_user = npNodeNew(kNodePin, node, dataRef);
	issue_user->geometry = kNPgeoCube;
	issue_user->colorIndex = 19;	// white
	issue_user->color.r = 255;
	issue_user->color.g = 255;
	issue_user->color.b = 255;
	issue_user->color.a = 220;		// mostly opaque
	issue_user->recordID = issueCreatorTag->recordID;
	issue_user->tableID = issueCreatorTag->tableID;
	npTagNode(issue_user, dataRef);
//	issue_user->translate.x += 50;
//	issue_user->translate.x -= 100;
	issue_user->scale.z = 0.50;

	///Set the recordId to the issue number
//	current_issue->issue_node->child[0]->recordID = (issueTitleTag->recordID = current_issue->number);
//	current_issue->issue_node->child[0]->tableID = (issueTitleTag->tableID = 3);
//	npTagNode(current_issue->issue_node->child[0], dataRef);


	/// Add sub-node that indicates user that is assigned the issue (the assignee)
	npNodeNew(kNodePin, node, dataRef);
	node->child[1]->translate.x -= 100;
	node->child[1]->geometry = kNPgeoSphere;
	node->child[1]->colorIndex = 19;	// white
	node->child[1]->color.r = 255;
	node->child[1]->color.g = 255;
	node->child[1]->color.b = 255;
	node->child[1]->color.a = 220;		// mostly opaque

	/// if no assignee exists, use wireframe geometry 
	if(current_issue->assignee == NULL)
	{
		node->child[1]->geometry = kNPgeoSphereWire;
	}
	else
	{
		issueAssigneeTag = npNewTag(dataRef);
//		printf("\ncurrent_issue ptr : %p", current_issue);
//		printf("\ncurrent_issue->assignee : %p", current_issue->assignee);
		if(current_issue->assignee != NULL)
		{
//			printf("\ncurrent_issue->assignee->login ptr : %p", current_issue->assignee->login);
//			printf("\ncurrent_issue->assignee->login : %s", current_issue->assignee->login);
			
			if(current_issue->assignee->avatar_image_file == NULL)
			{
				current_issue->assignee->avatar_image_file = malloc(sizeof(char) * ( strlen(current_issue->assignee->login) + 5 ) );   
				current_issue->assignee->avatar_image_file[0] = '\0';
				sprintf(current_issue->assignee->avatar_image_file, "%s.png", current_issue->assignee->login);
//				printf("\ncurrent_issue->assignee->avatar_image_file : %s", current_issue->assignee->avatar_image_file);
			}
			
			
//			getchar();

			sprintf(issueAssigneeTag->title, "Assigned To : %s", current_issue->assignee->login);
//			printf("\nafter sprintf");
			issueAssigneeTag->titleSize = strlen(issueAssigneeTag->title) + 1;

			issueAssigneeTag->recordID = current_issue->number;
			issueAssigneeTag->tableID = kNPgitvizTableID + 7; //zz 7;

//			printf("\nbefore npTagSortAdd");
			npTagSortAdd(issueAssigneeTag, dataRef);
//			printf("\nafter npTagSortAdd");

			node->child[1]->recordID = issueAssigneeTag->recordID; 
			node->child[1]->tableID = issueAssigneeTag->tableID;

			npTagNode(node->child[1], dataRef);
//			printf("\nAfter npTagNode");
		}

	}

	/// Add sub-node that for labels
	npNodeNew(kNodePin, node, dataRef);
	node->child[2]->translate.x -= 20;
	node->child[2]->recordID = (issueTitleTag->recordID = current_issue->number);
	node->child[2]->tableID = (issueTitleTag->tableID = kNPgitvizTableID + 2); //zz 2);
		
	npTagNode(node->child[2], dataRef);
	
	///=============================================================
	titlePtr = issueTitleTag->title;
//	numLettersInTitle (spaces count)
	numLettersInTitle = strlen(issueTitleTag->title);
	node->child[2]->geometry = kNPgeoTorus;

	///=============================================================

//	printf("\ncurrent_issue->num_of_labels : %d", current_issue->num_of_labels);
	for(index = 0; index < current_issue->num_of_labels; index++)
	{
		npNodeNew(kNodePin, node->child[2], dataRef);	

/*
		current_issue->issue_node->child[2]->child[index]->recordID = (issueTitleTag->recordID = current_issue->number);
		current_issue->issue_node->child[2]->child[index]->tableID = (issueTitleTag->tableID = 2);
*/

		if( strcmp(current_issue->label[index].name, "enhancement") == 0 )
		{
			node->child[2]->child[index]->colorIndex = 2;	// green
			node->child[2]->child[index]->color.r = 0; 
			node->child[2]->child[index]->color.g = 255;
			node->child[2]->child[index]->color.b = 0;
			node->child[2]->child[index]->color.a = 220;	// mostly opaque
			
			node->child[2]->child[index]->recordID = current_issue->number;
			node->child[2]->child[index]->tableID = kNPgitvizTableID + 6; //zz 6;
		}

		if( strcmp(current_issue->label[index].name, "bug") == 0 )
		{
			node->child[2]->child[index]->color.r = 255; 
			node->child[2]->child[index]->color.g = 0;
			node->child[2]->child[index]->color.b = 0;
			node->child[2]->child[index]->color.a = 220;	// mostly opaque
//			current_issue->issue_node->child[2]->child[index]->color.a = 1;
			node->child[2]->child[index]->recordID = current_issue->number;
			node->child[2]->child[index]->tableID = kNPgitvizTableID + 7; //zz 5;
		}

		if( strcmp(current_issue->label[index].name, "1st priority") == 0 )
		{
			node->child[2]->child[index]->colorIndex = 1;	// red
			node->child[2]->child[index]->color.r = 255; 
			node->child[2]->child[index]->color.g = 0;
			node->child[2]->child[index]->color.b = 0;
			node->child[2]->child[index]->color.a = 220;	// mostly opaque
//			current_issue->issue_node->child[2]->child[index]->color.a = 1;
			node->child[2]->child[index]->recordID = current_issue->number;
			node->child[2]->child[index]->tableID = kNPgitvizTableID + 8; //zz 8;
		}

		if( strcmp(current_issue->label[index].name, "2nd priority") == 0 )
		{
			node->child[2]->child[index]->colorIndex = 17;	// rust orange
			node->child[2]->child[index]->color.r = 255; 
			node->child[2]->child[index]->color.g = 100;
			node->child[2]->child[index]->color.b = 0;
			node->child[2]->child[index]->color.a = 220;	// mostly opaque
//			current_issue->issue_node->child[2]->child[index]->color.a = 1;
			node->child[2]->child[index]->recordID = current_issue->number;
			node->child[2]->child[index]->tableID = kNPgitvizTableID + 9; //zz 9;
		}

		if( strcmp(current_issue->label[index].name, "3rd priority") == 0 )
		{
			node->child[2]->child[index]->colorIndex = 4;	// yellow
			node->child[2]->child[index]->color.r = 255; 
			node->child[2]->child[index]->color.g = 255;
			node->child[2]->child[index]->color.b = 0;
			node->child[2]->child[index]->color.a = 220;	// mostly opaque
//			current_issue->issue_node->child[2]->child[index]->color.a = 1;
			node->child[2]->child[index]->recordID = current_issue->number;
			node->child[2]->child[index]->tableID = kNPgitvizTableID + 9; //zz 9;
		}
		
		if( strcmp(current_issue->label[index].name, "help wanted") == 0 )
		{
			node->child[2]->child[index]->colorIndex = 9;	// medium green
			node->child[2]->child[index]->color.r = 0; 
			node->child[2]->child[index]->color.g = 153;
			node->child[2]->child[index]->color.b = 0;
			node->child[2]->child[index]->color.a = 1;
			node->child[2]->child[index]->recordID = current_issue->number;
			node->child[2]->child[index]->tableID = kNPgitvizTableID + 10; //zz 9;
		}

		if( strcmp(current_issue->label[index].name, "question") == 0 )
		{
			node->child[2]->child[index]->colorIndex = 5;	// magenta
			node->child[2]->child[index]->color.r = 152; 
			node->child[2]->child[index]->color.g = 0;
			node->child[2]->child[index]->color.b = 255;
			node->child[2]->child[index]->color.a = 0;
			node->child[2]->child[index]->recordID = current_issue->number;
			node->child[2]->child[index]->tableID = kNPgitvizTableID + 11; //zz 9;
		}

		if( strcmp(current_issue->label[index].name, "wontfix") == 0 )
		{
			node->child[2]->child[index]->color.r = 18;		// black
			node->child[2]->child[index]->color.g = 51;
			node->child[2]->child[index]->color.b = 51;
			node->child[2]->child[index]->color.a = 100;	// mostly transparent
			node->child[2]->child[index]->recordID = current_issue->number;
			node->child[2]->child[index]->tableID = kNPgitvizTableID + 12; //zz 9;
		}
		
		npTagNode(node->child[2]->child[index], dataRef);
	}

//		printf("\n user ptr : %p", current_issue->user);

		if(current_issue->user == NULL) /// user should never be null
		{
			printf("user is null : %d\n", issueIndex);
			printf("title is %s\n", current_issue->title);
//			////getchar();
			return;
		}

	//	printf("\ncurrent_issue->user->avatar_image_textureID : %d", current_issue->user->avatar_image_textureID);  
		
		if(current_issue->user->avatar_image_textureID == 0)
		{
//			printf("\navatar_image_file : %s", current_issue->user->avatar_image_file);
		//	printf("\nuser->login : %s", current_issue->user->login);

			current_issue->user->avatar_image_textureID = SOIL_load_OGL_texture
	        (
				current_issue->user->avatar_image_file,
				SOIL_LOAD_AUTO,
				SOIL_CREATE_NEW_ID,
				SOIL_FLAG_INVERT_Y |
				SOIL_FLAG_MIPMAPS //|			//disabling breaks RGBA textures
	        // | SOIL_FLAG_NTSC_SAFE_RGB	//we want the entire RGB spectrum
	        // | SOIL_FLAG_COMPRESS_TO_DXT	//no lossy compression, faster too
	        );

			//printf("\ncurrent_issue->user->avatar_image_textureID : %d", current_issue->user->avatar_image_textureID); 
	                
	        if(current_issue->user->avatar_image_textureID == 0)
	        {
				printf("err 2309 - SOIL error: %s\n", SOIL_last_result());         
	        }
	        else
	        {
		        data->io.gl.textureCount = current_issue->user->avatar_image_textureID;
				
				issue_user->textureID = current_issue->user->avatar_image_textureID;
	        }

		}
		else
		{
		//	printf(" Texture already exists %d\n", current_issue->user->avatar_image_textureID);  
		    data->io.gl.textureCount = current_issue->user->avatar_image_textureID;
			issue_user->textureID = current_issue->user->avatar_image_textureID;
		}
		/// -------------------------------------------------------------------------------------------------------------

		if(current_issue->assignee != NULL)
		{
//			printf("\nassignee->avatar_image_file : %s", current_issue->assignee->avatar_image_file);
//			printf("\nassignee->login : %s", current_issue->assignee->login);
//			////getchar();
			if(current_issue->assignee->avatar_image_textureID == 0)
			{
				current_issue->assignee->avatar_image_textureID = SOIL_load_OGL_texture
				(
					current_issue->assignee->avatar_image_file,
					SOIL_LOAD_AUTO,
					SOIL_CREATE_NEW_ID,
					SOIL_FLAG_INVERT_Y |
					SOIL_FLAG_MIPMAPS //|			//disabling breaks RGBA textures
	        // | SOIL_FLAG_NTSC_SAFE_RGB	//we want the entire RGB spectrum
	        // | SOIL_FLAG_COMPRESS_TO_DXT	//no lossy compression, faster too
				);
	        
				if(current_issue->assignee->avatar_image_textureID == 0)
				{
					printf("err 2310 - SOIL error: %s\n", SOIL_last_result());
				}
				else
				{
				    data->io.gl.textureCount = current_issue->assignee->avatar_image_textureID;
		//			current_issue->issue_node->child[1]->textureID = current_issue->assignee->avatar_image_textureID;
		//			issue_assignee->textureID = current_issue->assignee->avatar_image_textureID;
					node->child[1]->textureID = current_issue->assignee->avatar_image_textureID;
		        }
			}
			else
			{
			//	printf(" Texture already exists %d\n", current_issue->assignee->avatar_image_textureID);  
			    data->io.gl.textureCount = current_issue->assignee->avatar_image_textureID;
				node->child[1]->textureID = current_issue->assignee->avatar_image_textureID;
			}
		}
		else
		{
//			printf("\nAssignee is NULL");
//			////getchar();
		}
		

		/// -------------------------------------------------------------------------------------------------------------
	
//	current_issue->issue_node->child[2]->geometry = 

}

/*
void npGitVizIssue2(pNPgithub github, int issueIndex, void* dataRef)
{
	pNPnode issue_creation_node;

	pNPgithubIssue current_issue;
	int yearCreatedIndex  = 0;
	int monthCreatedIndex = 0;
	int dayCreatedIndex   = 0;

//	current_issue = github->issue[issueIndex];
	current_issue = &github->issues->issue[issueIndex];

	
	current_issue->issue_creation_node = npNodeNew(kNodePin, NULL, dataRef);

	current_issue->issue_creation_node = (pNPnode) malloc (sizeof(NPnode));
//	npInitNodeDefault(current_issue->issue_creation_node); /// new
		
//	current_issue->issue_creation_node->hide = true;
	current_issue->issue_creation_node->hide = false;
	npGitVizSetIssueCreationNodeState2(current_issue, dataRef);

	if( strcmp(current_issue->state, "closed") == 0 )
	{
		current_issue->issue_closed_node = npNodeNew(kNodePin, NULL, dataRef);
		current_issue->issue_closed_node = (pNPnode) malloc (sizeof(NPnode));
		npInitNodeDefault(current_issue->issue_closed_node);
//		current_issue->issue_closed_node->hide = true; 
		current_issue->issue_closed_node->hide = false; 
		npGitVizSetIssueClosedNodeState(current_issue, dataRef);
	}

}
*/


void npHideIssueClosedNode2(pNPgithubIssue issue, void* dataRef)
{
	pNPnode node = (pNPnode) issue->issue_closed_node;
	node->hide = true;
}

void npShowIssueClosedNode2(pNPgithubIssue issue, void* dataRef)
{
	pNPnode node = (pNPnode) issue->issue_closed_node;
	node->hide = false;
}

void npSetIssueClosedNodeParent2(pNPgithubIssue issue, pNPnode nodeParent, void* dataRef)
{
	pNPnode node = (pNPnode) issue->issue_closed_node;
	node->parent = nodeParent;	
}

void npSetIssueClosedNodeGeometry2(pNPgithubIssue issue, int geometry, void* dataRef)
{
	pNPnode node = (pNPnode) issue->issue_closed_node;
	node->geometry = geometry;
}

void npSetIssueClosedNodeTopo2(pNPgithubIssue issue, int topo, void* dataRef)
{
	pNPnode node = (pNPnode) issue->issue_closed_node;
	node->topo = topo;
}

void npHideIssueCreationNode2(pNPgithubIssue issue, void* dataRef)
{
	pNPnode node = (pNPnode) issue->issue_creation_node;
	node->hide = true;
}

void npShowIssueCreationNode2(pNPgithubIssue issue, void* dataRef)
{
	pNPnode node = (pNPnode) issue->issue_creation_node;
	node->hide = false;
}

void npSetIssueCreationNodeParent2(pNPgithubIssue issue, pNPnode nodeParent, void* dataRef)
{
	pNPnode node = (pNPnode) issue->issue_creation_node;
	node->parent = nodeParent;
}

void npSetIssueCreationNodeGeometry2(pNPgithubIssue issue, int geometry, void* dataRef)
{
	pNPnode node = (pNPnode) issue->issue_creation_node;
	node->geometry = geometry;
}

void npSetIssueCreationNodeTopo2(pNPgithubIssue issue, int topo, void* dataRef)
{
	pNPnode node = (pNPnode) issue->issue_creation_node;
	node->topo = topo;
}




void theNew_npGitViz(pNPgithub github, void* dataRef)
{
	int issueIndex = 0;

	for(issueIndex = 0; issueIndex < github->issues->count; issueIndex++) 
	{
		theNew_npGitVizIssue2(github, issueIndex, dataRef);
	}

	return;
}

/*
void npGitViz(pNPgithub github, void* dataRef)
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

	new_npCreateTimeViz2(&year_node, dataRef);
//	for(issueIndex = 0; issueIndex < github->count; issueIndex++)
	printf("93929190 %d", github->issues->count);
	//for(issueIndex = 0; issueIndex < github->issues->count; issueIndex++)
	for(issueIndex = 0; issueIndex < 167; issueIndex++)
	{
		npGitVizIssue2(github, issueIndex, dataRef); /// @todo npGitVizAllgithub

//		current_issue = github->issue[issueIndex];
		current_issue = &github->issues->issue[issueIndex];		
		

		new_npGithubGetIssueCreatedAt2(current_issue);	

		yearCreatedIndex = current_issue->issue_created_at.year - 2014; // - 2014 temporary
		monthCreatedIndex = current_issue->issue_created_at.month - 1;
		dayCreatedIndex = current_issue->issue_created_at.day;

//		printf("\nIndexes, Year : %d, Month : %d, Day : %d", yearCreatedIndex, monthCreatedIndex, dayCreatedIndex);
		
		dayCreated_node = year_node[yearCreatedIndex]->child[monthCreatedIndex]->child[dayCreatedIndex];
		dayCreated_node->translate.x = (11.61290 * dayCreatedIndex);
		year_node[yearCreatedIndex]->child[monthCreatedIndex]->hide = false;

		dayCreated_node->hide = false;

		childCount = dayCreated_node->childCount;
		dayCreated_node->child[childCount] = current_issue->issue_creation_node;
		
		npShowIssueCreationNode2(current_issue, dataRef);
		npSetIssueCreationNodeParent2(current_issue, dayCreated_node, dataRef);
		npSetIssueCreationNodeGeometry2(current_issue, kNPgeoTorus, dataRef);
		npSetIssueCreationNodeTopo2(current_issue, kNPtopoTorus, dataRef);	

		dayCreated_node->childCount++;		

		if(current_issue->issue_closed_node != NULL)
		{	
			new_npGithubGetIssueClosedAt2(current_issue);

			yearClosedIndex = current_issue->issue_closed_at->year - 2014; // - 2014 temporary
			monthClosedIndex = current_issue->issue_closed_at->month - 1;
			dayClosedIndex = current_issue->issue_closed_at->day;
			
			dayClosed_node = year_node[yearClosedIndex]->child[monthClosedIndex]->child[dayClosedIndex];
			dayClosed_node->translate.x = (11.61290 * dayClosedIndex);
			year_node[yearClosedIndex]->child[monthClosedIndex]->hide = false;
			dayClosed_node->hide = false;

			childCount = dayClosed_node->childCount;
			dayClosed_node->child[childCount] = current_issue->issue_closed_node;
			
			npShowIssueClosedNode2(current_issue, dataRef);
			npSetIssueClosedNodeParent2(current_issue, dayClosed_node, dataRef);
			npSetIssueClosedNodeGeometry2(current_issue, kNPgeoSphere, dataRef);
			npSetIssueClosedNodeTopo2(current_issue, kNPtopoSphere, dataRef);

			dayClosed_node->childCount++;
			link = npLinkIssueCreationNodeToIssueClosedNode2(current_issue->issue_creation_node, current_issue->issue_closed_node, dataRef);
			link->geometry = 20;
		}

	}

}
*/


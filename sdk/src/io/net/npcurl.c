	//
//  npcurl.c
//  npcurl
//
//  Created by z on 6/20/15.
//  Copyright (c) 2015 z. All rights reserved.
//
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


#include "SOIL.h"
#include "../../os/npos.h"
#include "../gl/nptags.h"

#include "npcurl.h"

enum {
    kNPissueUrl = 0,
    kNPissueId = 1,
    kNPissueTitle = 2,
    kNPissueState = 3,
    kNPissueCreatedAt = 4,
    kNPissueUpdatedAt = 5,
    kNPissueClosedAt = 6
};

enum {
    kNPgithubJSONgetIssueUrl = 0,
    kNPgithubJSONgetIssueId = 1,
    kNPgithubJSONgetIssueTitle = 2,
    kNPgithubJSONgetIssueState = 3,
    kNPgithubJSONgetIssueCreatedAt = 4,
    kNPgithubJSONgetIssueUpdatedAt = 5,
    kNPgithubJSONgetIssueClosedAt = 6,
    kNPgithubJSONgetIssueUserLogin = 7,
    kNPgithubJSONgetIssueUserAvatarUrl = 9,
	kNPgithubJSONgetIssueNumber = 8,
    kNPgithubSetIssueUrl = 10,
    kNPgithubSetIssueId  = 11,
    kNPgithubSetIssueTitle = 12,
    kNPgithubSetIssueState = 13,
    kNPgithubSetIssueCreatedAt = 14,
    kNPgithubSetIssueUpdatedAt = 15,
    kNPgithubSetIssueClosedAt = 16,
    kNPgithubSetIssueUserLogin = 17
};


struct MemoryStruct {
    char *memory;
    size_t size;
};

void npGithubGetIssueCreatedAt(pNPgithubIssue issue)
{
    char* general_string[][15] = {'\0', '\0', '\0', '\0'};
    /// @todo make general
}

/// dumpTill(issue->created_at, year_str, '-')
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


void dump(char* ptr, char* symbol[], size_t symbolArraySize)
{
    
}

/*
void new_npGithubGetDateTime(pNPgithubIssue issue)
{
    char* from_ptr = issue->issue_time; /// issue_time points to char array
    pNPgithubIssueCreatedAt to_ptr = issue->temp_time;
    char str[6][5] = {'\0'};
    char* symbol[] = {'-','-','T',':',':', 'Z'};
    int dateTimeFuncIndex = 0;
    size_t symbolArraySize = 0;
    symbolArraySize = sizeof(symbol) / sizeof(char*);
    
    to_ptr = malloc(sizeof(NPgithubIssueCreatedAt));
    
    from_ptr = dumpTil(from_ptr, str[dateTimeFuncIndex], symbol[symbol_index]);
    
    
   // npLoop();
    
    
}
*/

/// returning 0 means it is done
/*
int npLoopGithubGetDateTime(char* from_ptr, int* fromIndex, int* toIndex, pNPgithubIssueCreatedAt to_ptr )
{
    char* to_ptrs[] = {to_ptr->year, to_ptr->month, to_ptr->day, to_ptr->hour, to_ptr->minute, to_ptr->second};
    char str[6][5] = {'\0'};
    char* symbol[] = {'-','-','T',':',':', 'Z'};
    int index = 0;
    
    to_ptr = malloc(sizeof(NPgithubIssueCreatedAt));
    
    while( (*fromIndex) < (*toIndex) )
    {
        from_ptr = dumpTill(from_ptr, str[index], symbol[index]);
        to_ptrs[index] = atoi(str[index]);
        
      //  to_ptr->year = atoi(str[index]);
      //  to_ptr->month = atoi(str[index]);
      //  to_ptr->day = atoi(str[index]);
      //  to_ptr->hour = atoi(str[index]);
      //  to_ptr->minute = atoi(str[index]);
      //  to_ptr->second = atoi(str[index]);
       
        to_ptr++; (*fromIndex)++; index++;
    }
    
    return 0;
}
*/

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

void new_new_npGithubGetIssueClosedAt(pNPgithubIssues issues)
{
    char* ptr = issues->current->closed_at;
    pNPgithubIssue issue = issues->current;
    char str[6][5] = {'\0'};
    char* symbol[] = {'-','-','T',':',':', 'Z'};
    int index = 0;
    size_t symbolArraySize = 0;
    symbolArraySize = sizeof(symbol) / sizeof(char*);
    
    if(ptr != NULL)
    {
        issue->issue_closed_at = malloc(sizeof(NPgithubIssueCreatedAt));
    }
    else
    {
        return;
    }
    
    for(index = 0; index <= (int)symbolArraySize-1; index++)
    {
        ptr = dumpTill(ptr, str[index], symbol[index]);
        ptr++;
        //printf("\n%s", str[index]);
    }
    
    issue->issue_closed_at->year  = atoi(str[0]);
    issue->issue_closed_at->month = atoi(str[1]);
    issue->issue_closed_at->day   = atoi(str[2]);
    
    issue->issue_closed_at->hour  = atoi(str[3]);
    issue->issue_closed_at->minute= atoi(str[4]);
    issue->issue_closed_at->second= atoi(str[5]);
    
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

void new_new_npGithubGetIssueCreatedAt(pNPgithubIssues issues)
{
    char* ptr = issues->issue[issues->issueVariableIndex]->created_at;
    pNPgithubIssue issue = issues->issue[issues->issueVariableIndex];
    char str[6][5] = {'\0'};
    char* symbol[] = {'-','-','T',':',':', 'Z'};
    int index = 0;
    size_t symbolArraySize = 0;
    symbolArraySize = sizeof(symbol) / sizeof(char*);
    
 //   issue->issue_created_at = malloc(sizeof(NPgithubIssueCreatedAt));
    
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


/*
void new_npGithubGetIssueClosedAt(pNPgithubIssue issue)
{
    char* ptr = issue->closed_at;
    char str[5][5] = {'\0'};
    char* symbol[] = {'-','-','T',':',':', 'Z'};
    int index = 0;
    size_t symbolArraySize = 0;
    symbolArraySize = sizeof(symbol) / sizeof(char*);
    
    issue->issue_created_at = malloc(sizeof(NPgithubIssueCreatedAt));
    
    for(index = 0; index <= (int)symbolArraySize-1; index++)
    {
        ptr = dumpTill(ptr, str[index], symbol[index]);
        ptr++; 
        printf("\n%s", str[index]);
    }
    
   // issue->issue_closed_at->
}
*/

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

char* npGithubIssueNodeTagSet(pNPgithubIssue issue, void* dataRef )
{
    char* recordTagTitle = issue->partOf->recordTag[issue->partOf->recordTagIndex]->title;
    char* recordTagVariable[] = {recordTagTitle}; /// put this in issues
//    char* recordTagVariable[] = {issue->partOf->recordTag[issue->partOf->recordTagIndex]->title};
    //   char** recordTagVariable = issue->partOf->recordTagVariable;
    int recordTagVariableIndex = issue->partOf->recordTagVariableIndex;
    
    void* issueVariable[] = {issue->title, issue->url, issue->created_at};
    int tagTitleSize = 0;
 //   issue->partOf->recordTagIndex = 0;
 //   tagTitleSize = issue->partOf->recordTag[issue->partOf->recordTagIndex]->titleSize;
    
    int issueVariableIndex = issue->partOf->issueVariableIndex;
    int recordTagIndex = issue->partOf->recordTagIndex;
    pNPrecordTag recordTag = issue->partOf->recordTag[recordTagIndex];
    

//    char* format[] = {"%s","%d"};
//    int formatTagIndex = 0;
    char** recordTagVariableFormat = issue->partOf->recordTagVariableFormat;
    int recordTagVariableFormatIndex = issue->partOf->recordTagVariableFormatIndex;

	issue->partOf->recordTagIndex = 0;
    tagTitleSize = issue->partOf->recordTag[issue->partOf->recordTagIndex]->titleSize;
  //  int issueVariableIndex = issue->partOf->issueVariableIndex;
//    sprintf()
    /*
    printf("\nA : %d: %s", recordTagVariableIndex, recordTagVariable[recordTagVariableIndex]);
    printf("\nB : %d: %s", recordTagVariableFormatIndex, recordTagVariableFormat[recordTagVariableFormatIndex]);
    printf("\nC : %d: %s", 2, issueVariable[2]);
    */
    
    tagTitleSize = sprintf(recordTagVariable[recordTagVariableIndex], recordTagVariableFormat[recordTagVariableFormatIndex], issueVariable[issueVariableIndex]);
/// now [1] was [issueVariableIndex]
    
    issue->partOf->recordTag[issue->partOf->recordTagIndex]->titleSize = tagTitleSize;
    
/*
    sprintf(recordTagVariable[recordTagVariableIndex], format[recordTagVariableFormatIndex], issueVariable[issueVariableIndex]);
*/
 /*
    for(int index = 0; index < 1; index++)
    {
        sprintf(recordTagVariable[index], format[index], issueVariable[index]);
//        sprintf(recordTagVariable[index], "%s", issueVariable[index]);
    }
*/
//    sprintf(recordTag->title, "%s", )
    return recordTagVariable[recordTagVariableIndex];
}

pNPrecordTag npGithubCtrlGetRecordTag(pNPgithubIssue issue, void* dataRef)
{
    return issue->partOf->issue[issue->index]->recordTag;
}

void npGithubCtrlSetCurrentIssueIndex(pNPgithubIssues issues, int index, void* dataRef)
{
    issues->index = index;
}

void npGithubCtrlSetCurrentIssue(pNPgithubIssues issues, void* dataRef)
{
    //issue->partOf->current = issue;
    issues->current = issues->issue[issues->index];
}

void npGithubCtrlSetCurrentIssueUserAvatarImageFile(pNPgithubIssues issues, void* dataRef)
{
    int i = 0;
    static int x = 0;
    char index[10] = "\0";
    
    
  //  issues->issue[issues->index]->user.avatar_image_file =
    sprintf(issues->issue[issues->index]->user.avatar_image_file, "%s.png", issues->issue[issues->index]->user.login);
    /*
    for(i = 0; i < 161; i++)
    {
        if(strcmp(issues->issue[issues->index]->user.login, issues->issue[i]->user.login) == 0)
        {
            issues->issue[issues->index]->user.avatar_image_file = malloc(sizeof(char) * 100);
            issues->issue[issues->index]->user.avatar_image_file[0] = '\0';
            strcpy(issues->issue[issues->index]->user.avatar_image_file, issues->issue[i]->user.login);
            
            break;
        }
     
    }
    */
 //   sprintf(index, "%d", x);
    
//    issues->issue[issues->index]->user.avatar_image_file = calloc(sizeof(char), 10);
//    strcpy(issues->issue[issues->index]->user.avatar_image_file, "avatar");
//    strcat(issues->issue[issues->index]->user.avatar_image_file, index);

}

void npGithubCtrlSetCurrentIssueUserAvatarImagePath(pNPgithubIssues issues, void* dataRef)
{
    static char* cwd = NULL;
    static bool firstCall = true;
    static int size = 0;
    int count = 0;
    if(firstCall == true)
    {
        cwd = malloc(sizeof(char) * 40);
        nposGetAppPath(cwd, &size);
        cwd[size] = '\0';
        firstCall = false;
    }
    
    issues->issue[issues->index]->user.avatar_image_file_path = malloc(sizeof(char) * 50);
    issues->issue[issues->index]->user.avatar_image_file_path[0] = '\0';
/*
    count = sprintf(issues->issue[issues->index]->user.avatar_image_file_path, "%s", cwd);
    count += sprintf(issues->issue[issues->index]->user.avatar_image_file_path + count, "%s", issues->issue[issues->index]->user.avatar_image_file);
*/
 //   printf("\n23446 : %s\n", issues->issue[issues->index]->user.avatar_image_file);
    if(issues->issue[issues->index]->user.avatar_image_file[0] == '\0')
    {
        printf("\n23446a : %s\n", issues->issue[issues->index]->user.login);
    }
    
    
    count = sprintf(issues->issue[issues->index]->user.avatar_image_file_path,
                    "%s%s", cwd, issues->issue[issues->index]->user.avatar_image_file);
//    printf("23445, %s", issues->issue[issues->index]->user.avatar_image_file_path);
    /*
    issues->issue[issues->index]->user.avatar_image_file_path = strdup(cwd);
    issues->issue[issues->index]->user.avatar_image_file_path[size] = '\0';
 //  count = sprintf(issues->issue[issues->index]->user.avatar_image_file_path, "%s",
             //      cwd, issues->issue[issues->index]->user.avatar_image_file);
    strcat(issues->issue[issues->index]->user.avatar_image_file_path, issues->issue[issues->index]->user.avatar_image_file);
    //issues->issue[issues->index]->user.avatar_image_file_path[count] = '\0';
    
 //   issues->issue[issues->index]->user.avatar_image_file_path = strcat(issues->issue[issues->index]->user.avatar_image_file_path, ".png");
   // free(cwd);
    */
}

pNPgithubIssue npGithubCtrlGetCurrentIssue(pNPgithubIssue issue, void* dataRef)
{
    return issue->partOf->current;
}

void npGithubCtrlSetRecordTagIndex(pNPgithubIssues issues, int index, void* dataRef)
{
  //  pNPgithubIssue current_issue = npGithubCtrlGetCurrentIssue(issue, dataRef);
  //  current_issue->partOf->recordTagIndex = index;
    //issue->partOf->recordTagIndex = index;
    issues->recordTagIndex = index;
}


void npGithubCtrlInitIssueTitleTextTag(pNPgithubIssues issues, int index, void* dataRef)
{

}

void npGithubCtrlInitIssueTextTag(pNPgithubIssues issues, int index, void* dataRef)
{
	issues->issue[index]->titleTag = npNewTag(dataRef);
	if(issues->issue[index]->titleTag == NULL)
	{
		printf("2387 Null");
	}

}

void npGithubCtrlInitRecordTag(pNPgithubIssues issues, int index, void* dataRef)
{
    npGithubCtrlSetRecordTagIndex(issues, index, dataRef);
    issues->recordTag[index] = npNewTag(dataRef);
    if(issues->recordTag[index] == NULL)
    {
        printf("2387 Null");
    }
    
}

void npGithubCtrlSetRecordTag(pNPgithubIssue issue, int index, void* dataRef)
{
    npGithubCtrlSetRecordTagIndex(issue->partOf, index, dataRef);
    issue->partOf->issue[issue->index]->recordTag = index;
    
}

pNPrecordTag npGithubIssueInitNodeTag(pNPgithubIssue issue ,void* dataRef)
{
    int recordTagVariableIndex = npGithubCtrlGetRecordTagVariableIndex(issue);
    pNPrecordTag recordTag = npGithubCtrlGetRecordTag(issue, dataRef);
//    pNPrecordTag recordTag = issue->partOf->recordTag[issue->partOf->recordTagIndex];
    static int id = 0;
    
  //  if(recordTag == NULL)
    /*
    if(recordTag == NULL)
    {
       // recordTag = npNewTag( dataRef );
        issue->partOf->recordTag[issue->partOf->recordTagIndex] = npNewTag( dataRef);
    }
    */
    
  //  issue->partOf->recordTagVariableIndex = 0;
    npGithubIssueNodeTagSet(issue, dataRef);

    /*
    sprintf(recordTag->title, "%s", issue->title);
    recordTag->titleSize = strlen(issue->title) + 1;
    issue->issue_node->recordID = recordTag->recordID = id;
    id++;
    */
    return issue->partOf->recordTag[issue->partOf->recordTagIndex];
}

pNPrecordTag npGithubIssueInitNodeTagTitle(pNPgithubIssue issue, void* dataRef)
{
    return npGithubIssueInitNodeTag(issue, dataRef);
}

int npGithubIssueInitNodeTagIndex(pNPgithubIssues issues, void* dataRef)
{
    issues->recordTagIndex = 0;
    
    return issues->recordTagIndex;
}

int npGithubCtrlGetRecordTagVariableIndex(pNPgithubIssue issue)
{
    return issue->partOf->recordTagVariableIndex;
}

void npGithubCtrlSetRecordTagVariableFormatIndex(pNPgithubIssue issue, int index)
{
    issue->partOf->recordTagVariableFormatIndex = index;
}

void npGithubCtrlSetRecordTagVariableIndex(pNPgithubIssue issue, int index)
{
    issue->partOf->recordTagVariableIndex = index; /// @todo Pull indexes from index pool
}

int npGithubCtrlGetIssueVariableIndex(pNPgithubIssue issue, int index)
{
    return issue->partOf->issueVariableIndex;
}

void npGithubCtrlSetIssueVariableIndex(pNPgithubIssue issue, int index)
{
    issue->partOf->issueVariableIndex = index;
}

/*
void npGithubCtrlAssignIssueTitleToRecordTagTitle(pNPgithubIssue issue, pNPrecordTag issueTitle_tag, void* dataRef)
{
    npGithubCtrlSetRecordTagVariableIndex(issue, 0);
    //issue->partOf->recordTagVariableIndex = 0;
    
    npGithubCtrlSetRecordTagVariableIndex(issue, 0);
    //issue->partOf->issueVariableIndex = 0;
    
    npGithubCtrlSetRecordTag(issue, 0, dataRef);
    
    issueTitle_tag = npGithubIssueInitNodeTag(issue, dataRef);
 
  //  issueTitle_tag = npGithubIssueInitNodeTag(issue, dataRef);
   // issueTitle_node->recordID = issueTitle_tag->recordID = id;
   // npTagSortAdd(issueTitle_tag, dataRef);
   // npTagNode(issueTitle_node, dataRef);
   // id++;
 
}
*/
char* npGithubCtrlGetImageFileName(pNPgithubIssues issues, void* dataRef)
{
    /// @todo rename avatar_image_path to avatar_image_file_name
    return issues->issue[issues->index]->user.avatar_image_file;
}

/*
void npGithubCtrlSetUserTexture(pNPgithubIssues issues, void* dataRef)
{
    char* image_file = NULL;
    char* cwd = NULL;
    cwd = malloc(sizeof(char) * 45);
    int size = 0;
    
    nposGetAppPath(cwd, &size);
    strcat(cwd, issues->current->user.avatar_image_file);
    //strcpy(issues->current->user.avatar_image_file_path, cwd);
    issues->current->user.avatar_image_file_path = strdup(cwd);
    free(cwd);
 //   image_file = npGithubCtrlGetImageFileName
}
*/

void npGithubCtrlLoadIssueUserAvatarTextureID(pNPgithubIssues issues, void* dataRef)
{
    pData data = (pData) dataRef;
    int index = 0;
    
    /*
    for(index = 0; index < issues->count; index++)
    {
        
        if(strcmp(issues->current->user.avatar_image_file, issues->issue[index]->user.avatar_image_file) == 0)
        {
            
            if(issues->current->user.avatar_image_textureID != 0)
            {
                printf("\n2327 Already Exists %d ", index);
            issues->current->user.avatar_image_textureID = issues->issue[index]->user.avatar_image_textureID;
                return;
            }
            else
            {
                issues->current->user.avatar_image_textureID = SOIL_load_OGL_texture
                (
                 issues->current->user.avatar_image_file_path,
                 SOIL_LOAD_AUTO,
                 SOIL_CREATE_NEW_ID,
                 SOIL_FLAG_INVERT_Y |
                 SOIL_FLAG_MIPMAPS //|			//disabling breaks RGBA textures
                 // | SOIL_FLAG_NTSC_SAFE_RGB	//we want the entire RGB spectrum
                 // | SOIL_FLAG_COMPRESS_TO_DXT	//no lossy compression, faster too
                 );
                
                if(issues->current->user.avatar_image_textureID == 0)
                {
                    printf("\n2309 Soil loading error : %s", SOIL_last_result());
                    
                }
                else
                {
                    data->io.gl.textureCount = issues->current->user.avatar_image_textureID;
                    return;
                }
                
            }
        }
        
        //
   //     if(issues->current->user.avatar_image_textureID == issues->issue[index]->user.avatar_image_textureID)
  //      {
            
  //      }
    //
        
    }
*/
    for(index = 0; index < issues->count; index++)
    {
    
    if(strcmp(issues->current->user.avatar_image_file, issues->issue[index]->user.avatar_image_file) == 0)
    {
        if(issues->current->user.avatar_image_textureID == 0)
            break;
        else
        {
        issues->current->user.avatar_image_textureID = issues->issue[index]->user.avatar_image_textureID;
            return;
        }
            
      //  return;
    }
    
    }
    
    issues->current->user.avatar_image_textureID = SOIL_load_OGL_texture
    (
     issues->current->user.avatar_image_file_path,
     SOIL_LOAD_AUTO,
     SOIL_CREATE_NEW_ID,
     SOIL_FLAG_INVERT_Y |
     SOIL_FLAG_MIPMAPS //|			//disabling breaks RGBA textures
     // | SOIL_FLAG_NTSC_SAFE_RGB	//we want the entire RGB spectrum
     // | SOIL_FLAG_COMPRESS_TO_DXT	//no lossy compression, faster too
     );
    
    if(issues->current->user.avatar_image_textureID == 0)
    {
        printf("\n2309 Soil loading error : %s\n%s\n", SOIL_last_result(),
               issues->current->user.avatar_image_file_path);
        
    }
    else
    {
       // printf("\n2310 Texture Id : %d", issues->current->user.avatar_image_textureID);
    }
    
}

void npGithubCtrlCountWordsInIssueTitle(pNPgithubIssue issue, void* dataRef)
{
    char* issueTitlePtr = &issue->title[0];
    //int numOfWordsInIssueTitle = 0;
    issue->numOfWordsInIssueTitle = 0;
    while((*issueTitlePtr) != '\0')
    {
        while((*issueTitlePtr) != ' ' && (*issueTitlePtr) != '\0')
        {
            issueTitlePtr++;
        }
        issue->numOfWordsInIssueTitle++;
        
        if((*issueTitlePtr) != '\0')
        {
            issueTitlePtr++;
        }
    }
}

void npGithubIncrementIssuesIndex(pNPgithubIssues issues, void* dataRef)
{
	issues->index++;
	issues->current = issues->issue[issues->index];
}

void npGithubInitIssueGlyph(pNPgithubIssues issues, void* dataRef)
{
	pNPnode issue_node = issues->issue[issues->index]->issue_node;

	npInitNodeDefault(issue_node);
}

void npGithubInitAllIssueGlyphs(pNPgithubIssues issues, void* dataRef)
{
	while(issues->index < issues->count)
	{
		npGithubInitIssueGlyph(issues, dataRef);
		npGithubIncrementIssuesIndex(issues, dataRef);
	}
}


void npGithubInitIssue(pNPgithubIssues issues, void* dataRef)
{
	pNPgithubIssue issue = NULL;
	issues->current = issues->issue[issues->index];
	issues->current->assignee = NULL;
	issues->current->body = NULL;
	issues->current->closed_at = NULL;
	issues->current->closed_by = NULL;
	issues->current->comments_url = NULL;
	issues->current->created_at = NULL;
	issues->current->events_url = NULL;
	issues->current->html_url = NULL;
	issues->current->id = 0;
	issues->current->index = issues->index;
	issues->current->issue_node = NULL;
	issues->current->issueGeoType = 0;
	issues->current->issueNodeType = 0;
	issues->current->issueTopoType = 0;
	issues->current->labels_url = NULL;
	issues->current->locked = NULL;
	issues->current->milestone = NULL;
	issues->current->num_comments = 0;
	issues->current->numOfWordsInIssueTitle = 0;
	issues->current->partOf = issues;
	issues->current->recordId = 0;
	issues->current->recordTag = NULL;
	issues->current->state = NULL;
	issues->current->title = NULL;
	issues->current->titleTag = NULL; /// issueTags array
	issues->current->updated_at = NULL;
	issues->current->url = NULL;
	issues->current->userId = 0;

	issues->current->user.avatar_image_file = NULL;
	issues->current->user.avatar_image_file_path = NULL;
	issues->current->user.avatar_image_textureID = 0;
	issues->current->user.avatar_url = NULL;
	issues->current->user.events_url = NULL;
	issues->current->user.followers_url = NULL;
	issues->current->user.gists_url = NULL;
	issues->current->user.gravatar_id = NULL;
	issues->current->user.html_url = NULL;
	issues->current->user.id = NULL;
	issues->current->user.login = NULL;
	issues->current->user.organizations_url = NULL;
	issues->current->user.received_events_url = NULL;
	issues->current->user.repos_url = NULL;
	issues->current->user.site_admin = NULL;
	issues->current->user.starred_url = NULL;
	issues->current->user.subscriptions_url = NULL;
	issues->current->user.type = NULL;
	issues->current->user.url = NULL;
}

void npGithubInitAllIssues(pNPgithubIssues issues, void* dataRef)
{
	while(issues->index < issues->count)
	{
		npGithubInitIssue(issues, dataRef);
		npGithubIncrementIssuesIndex(issues, dataRef);
	}

}

void new_gitVizTest(pNPgithubIssues issues, void* dataRef)
{
	pNPgithubIssue current_issue = NULL;
	pNPnode issue_creation_node = NULL;
	pNPnode issue_closed_node = NULL;
	pNPnode issue_day_node = NULL;
	pNPnode link = NULL;
	static float y_translate = 0;
	static float x_translate = 0;
	static float z_translate = 0;
	static int id = 0;
	int childIndex = 0;
	
	int dayIndex = 0;
	int monthIndex = 0;
	int yearIndex = 0;
	int dayClosedIndex = 0;
	int monthClosedIndex = 0;
	int yearClosedIndex = 0;
	int dayCreatedIndex = 0;
	int monthCreatedIndex = 0;
	int yearCreatedIndex = 0;
	int issuesInYear2014 = 0;
	int issuesInYear2015 = 0;
	int issuesInMonth[12] = {0,0,0,0,0,0,0,0,0,0,0,0}; // [year][month]
	int numOfIssuesInTime[2][12][32] = {0,0,0,0,0,0,0,0,0,0,0,0}; // [year][month][day]
	char* monthString[] = {"January", "February", "March", "April", "May", "June", "July", "August", "September", "October", "November", "December"}; 
	//int id_one = 0;
	//int id_two = 0;
	int idOne = 0;
	int idTwo = 0;
	bool showMonth = true;
	bool showDay = true;

	int issueIndex = 0;
	pNPnode time_node = NULL;
	pNPnode year_2014 = NULL;
	pNPnode year_2015 = NULL;

	pNPrecordTag year_recordTag = NULL;
	pNPrecordTag month_recordTag = NULL;
	pNPrecordTag day_recordTag = NULL;

	pNPnode year_node[2];
	pNPnode month_node;
	pNPnode day_node;

	/*
	year_2014 = npNodeNew(kNodePin, NULL, dataRef);
	year_2014->geometry = kNPgeoTorus;
	year_2014->topo = kNPtopoTorus;

	year_2015 = npNodeNew(kNodePin, NULL, dataRef);
	year_2015->geometry = kNPgeoTorus;
	year_2015->topo = kNPtopoTorus;
	*/

	for(yearIndex = 0; yearIndex < 2; yearIndex++)
	{
		year_node[yearIndex] = npNodeNew(kNodePin, NULL, dataRef);
		year_node[yearIndex]->geometry = kNPgeoTorus;
		year_node[yearIndex]->topo = kNPtopoTorus;

		year_recordTag = npNewTag( dataRef );
		sprintf(year_recordTag->title, "%d", yearIndex + 2014);
		year_recordTag->titleSize = strlen(year_recordTag->title) + 1;
		year_recordTag->recordID = id+1;
		year_node[yearIndex]->recordID = id+1;
		id++;
		npTagSortAdd(year_recordTag, dataRef);
		npTagNode(year_node[yearIndex], dataRef);

		for(monthIndex = 0; monthIndex <= 11; monthIndex++)
		{
			month_node = year_node[yearIndex]->child[monthIndex];
			month_node = npNodeNew(kNodePin, year_node[yearIndex], dataRef);
			month_node->translate.x = (-30 * monthIndex);
//			month_node->geometry = kNPgeoTorus;
//			month_node->topo = kNPtopoTorus;
			month_node->scale.x = 0.5;
			month_node->scale.y = 0.5;
			month_node->scale.z = 0.5;
			month_node->translate.y += 90;

			month_recordTag = npNewTag( dataRef );
			sprintf(month_recordTag->title, "%s", monthString[monthIndex]);
			month_recordTag->titleSize =  3;
			month_recordTag->recordID = id+1;
			month_node->recordID = id+1;
			id++;

			npTagSortAdd(month_recordTag, dataRef);
    
			/// @todo npTagGithubIssueNode
			npTagNode(month_node, dataRef);

			/*
			year_node[yearIndex]->child[monthIndex] = npNodeNew(kNodePin, year_node[yearIndex], dataRef);
			year_node[yearIndex]->child[monthIndex]->translate.x = (30 * monthIndex);
			year_node[yearIndex]->child[monthIndex]->geometry = kNPgeoTorus;
			year_node[yearIndex]->child[monthIndex]->topo = kNPtopoTorus;
			year_node[yearIndex]->child[monthIndex]->scale.x = 0.5;
			year_node[yearIndex]->child[monthIndex]->scale.y = 0.5;
			year_node[yearIndex]->child[monthIndex]->scale.z = 0.5;
			*/
			for(dayIndex = 0; dayIndex <= 31; dayIndex++)
			{
				day_node = year_node[yearIndex]->child[monthIndex]->child[dayIndex];
			//	printf("\n2318 month index : %d", monthIndex);

				day_node = npNodeNew(kNodePin, month_node, dataRef);
//				day_node->translate.x = (11.61290 * dayIndex);
			//	day_node->translate.x = (11.61290 * dayIndex);s
				day_node->translate.x = 0;

		/*
				day_recordTag = npNewTag( dataRef );
				sprintf(day_recordTag->title, "%d", dayIndex+1);
				day_recordTag->titleSize =  strlen(day_recordTag->title) + 1;
				day_recordTag->recordID = 
				//	day_recordTag->recordID = id+1;
			//	day_node->recordID = id+1;
				id++;
		 
				npTagSortAdd(day_recordTag, dataRef);
		
				/// @todo npTagGithubIssueNode
				npTagNode(day_node, dataRef);
		*/

//				day_node->geometry = kNPgeoTorus;
//				day_node->topo = kNPgeoTorus;

				/*
				year_node[yearIndex]->child[monthIndex]->child[dayIndex] = npNodeNew(kNodePin, year_node[yearIndex]->child[monthIndex], dataRef);
				year_node[yearIndex]->child[monthIndex]->child[dayIndex]->translate.x = (11.61290 * dayIndex);
				year_node[yearIndex]->child[monthIndex]->child[dayIndex]->geometry = kNPgeoTorus;
				*/
			}

		}
	}
 
	while(issueIndex < issues->count)
	{
		current_issue = issues->issue[issueIndex];
		//issues->issue[issueIndex]->
		if( strcmp(current_issue->state, "closed") == 0 )
		{
			new_npGithubGetIssueCreatedAt(current_issue);
			yearCreatedIndex = current_issue->issue_created_at.year - 2014;
			monthCreatedIndex = current_issue->issue_created_at.month - 1;
			dayCreatedIndex = current_issue->issue_created_at.day;

			issue_day_node = year_node[yearCreatedIndex]->child[monthCreatedIndex]->child[dayCreatedIndex];
			issue_creation_node = year_node[yearCreatedIndex]->child[monthCreatedIndex]->child[dayCreatedIndex]->child[year_node[yearCreatedIndex]->child[monthCreatedIndex]->child[dayCreatedIndex]->childCount];
			issue_creation_node = npNodeNew(kNodePin, issue_day_node, dataRef);
			issue_creation_node->color.r = 255;
			issue_creation_node->color.g = 0;
			issue_creation_node->color.b = 0;
			issue_creation_node->translate.x = (11.25 * dayCreatedIndex);
//			issue_creation_node->translate.z = 10;

			/// Text tag and record id
			day_recordTag = npNewTag( dataRef );
		//	sprintf(day_recordTag->title, "%d test", dayCreatedIndex);
		//	sprintf(day_recordTag->title, "%d", current_issue->index);
			sprintf(day_recordTag->title, "%s", current_issue->title);
			//	printf("\n2383 day_recordTag->title : %s", day_recordTag->title);	
			day_recordTag->titleSize =  strlen(day_recordTag->title) + 1;
		//	day_recordTag->recordID = current_issue->id;
			day_recordTag->recordID = current_issue->index;
			printf("\n2384 %d", current_issue->index);
			day_recordTag->tableID = 1;
			issue_creation_node->recordID = day_recordTag->recordID;
			issue_creation_node->tableID = day_recordTag->tableID;
			id++;

			npTagSortAdd(day_recordTag, dataRef);
		
			/// @todo npTagGithubIssueNode
		//	npTagNode(issue_day_node, dataRef);
			npTagNode(issue_creation_node, dataRef);

			issuesInMonth[monthCreatedIndex]++;
			numOfIssuesInTime[yearCreatedIndex][monthCreatedIndex][dayCreatedIndex]++;
			
			new_npGithubGetIssueClosedAt(current_issue);
			yearClosedIndex = current_issue->issue_closed_at->year - 2014;
			monthClosedIndex = current_issue->issue_closed_at->month - 1;
			dayClosedIndex = current_issue->issue_closed_at->day;

			issue_day_node = year_node[yearClosedIndex]->child[monthClosedIndex]->child[dayClosedIndex];
			issue_closed_node = year_node[yearClosedIndex]->child[monthClosedIndex]->child[dayClosedIndex]->child[year_node[yearClosedIndex]->child[monthClosedIndex]->child[dayClosedIndex]->childCount];
			//issue_closed_node = npNodeNew(kNodePin, year_node[yearClosedIndex]->child[monthClosedIndex]->child[dayClosedIndex], dataRef);
			issue_closed_node = npNodeNew(kNodePin, issue_day_node, dataRef);
			issue_closed_node->geometry = kNPgeoSphere;
			issue_closed_node->color.r = 0;
			issue_closed_node->color.g = 0;
			issue_closed_node->color.b = 255;
			issue_closed_node->translate.x = (11.25 * dayClosedIndex);

			issue_closed_node->recordID = day_recordTag->recordID;
			issue_closed_node->tableID = day_recordTag->tableID;
			npTagNode(issue_closed_node, dataRef);

			issuesInMonth[monthClosedIndex]++; /// iterate over this later

			numOfIssuesInTime[yearClosedIndex][monthClosedIndex][dayClosedIndex]++;

			link = npNodeNewLink(issue_creation_node, issue_closed_node, dataRef);
			//link->geometry = kNPgeoCylinder;
			//link->geometry = 20;
			link->geometry = 20;
			link->scale.z = 0.5;
			//link->scale.z = 5;
			link->lineWidth = 0.1;
				
			idOne++;
			printf("\nCLOSED : %d", idOne);

		}
		else if( strcmp(current_issue->state, "open") == 0 )
		{
			new_npGithubGetIssueCreatedAt(current_issue);
			yearCreatedIndex = current_issue->issue_created_at.year - 2014;
			monthCreatedIndex = current_issue->issue_created_at.month - 1;
			dayCreatedIndex = current_issue->issue_created_at.day;

			issue_creation_node = year_node[yearCreatedIndex]->child[monthCreatedIndex]->child[dayCreatedIndex];

			printf("\nYear Created Index : %d Year %d", yearCreatedIndex, current_issue->issue_created_at.year);
			printf("\nMonth Created Index : %d Month %d", monthCreatedIndex, current_issue->issue_created_at.month);
			printf("\nOPEN : %d", idTwo);
			
			issue_day_node = year_node[yearCreatedIndex]->child[monthCreatedIndex]->child[dayCreatedIndex];
			issue_creation_node = npNodeNew(kNodePin, issue_day_node, dataRef);
//			issue_creation_node->geometry = kNPgeoTorus;
//			issue_creation_node->topo = kNPtopoTorus;
			issue_creation_node->color.r = 255;
			issue_creation_node->color.g = 0;
			issue_creation_node->color.b = 0;

			day_recordTag = npNewTag( dataRef );
		//	sprintf(day_recordTag->title, "%d test", dayCreatedIndex);
		//	sprintf(day_recordTag->title, "%d test", current_issue->id);
			sprintf(day_recordTag->title, "%s", current_issue->title);
			printf("\n2384 %d", current_issue->index);
			printf("\n2383 day_recordTag->title : %s", day_recordTag->title);	
			day_recordTag->titleSize =  strlen(day_recordTag->title) + 1;
			//day_recordTag->recordID = current_issue->id;
			day_recordTag->recordID = current_issue->index;
			day_recordTag->tableID = 1;
			issue_creation_node->recordID = day_recordTag->recordID;
			issue_creation_node->tableID = day_recordTag->tableID;
			id++;

			npTagSortAdd(day_recordTag, dataRef);
		
			/// @todo npTagGithubIssueNode
		//	npTagNode(issue_day_node, dataRef);
			npTagNode(issue_creation_node, dataRef);


			/*
			year_node[yearCreatedIndex]->child[monthCreatedIndex]->child[issuesInMonth[monthCreatedIndex]]->child[dayCreatedIndex] = npNodeNew(kNodePin, year_node[yearCreatedIndex]->child[monthCreatedIndex]->child[dayCreatedIndex], dataRef);
			year_node[yearCreatedIndex]->child[monthCreatedIndex]->child[issuesInMonth[monthCreatedIndex]]->child[dayCreatedIndex]->color.r = 255;
			year_node[yearCreatedIndex]->child[monthCreatedIndex]->child[issuesInMonth[monthCreatedIndex]]->child[dayCreatedIndex]->color.g = 0;
			year_node[yearCreatedIndex]->child[monthCreatedIndex]->child[issuesInMonth[monthCreatedIndex]]->child[dayCreatedIndex]->color.b = 0;
			*/

			idTwo++;
			issuesInMonth[monthCreatedIndex]++;
			numOfIssuesInTime[yearCreatedIndex][monthCreatedIndex][dayCreatedIndex]++;

			printf("\nid_two after : %d", idTwo);
		}

		issueIndex++;
	}
	
	for(yearIndex = 0; yearIndex < 2; yearIndex++)
	{
		for(monthIndex = 0; monthIndex <= 11; monthIndex++)
		{
			for(dayIndex = 0; dayIndex <= 31; dayIndex++)
			{
				if( numOfIssuesInTime[yearIndex][monthIndex][dayIndex] == 0 )
				{
					/// Nothing here, hide it
					//printf("\nzero");
					showDay = false;
					showMonth = false;
					//year_node[yearIndex]->child[monthIndex]->child[dayIndex]->hide = true;
				}
				else
				{
					//showMonth = true;
					showDay = true;
					showMonth = true;
				}

				if(showDay == false)
				{
					year_node[yearIndex]->child[monthIndex]->child[dayIndex]->hide = true;
					//year_node[yearIndex]->child[monthIndex]->hide = true;
				}
				else
				{
					year_node[yearIndex]->child[monthIndex]->child[dayIndex]->hide = false;
					//year_node[yearIndex]->child[monthIndex]->hide = false;
				}

			}
			

		}
	}


}

void gitVizTest(pNPgithubIssues issues, void* dataRef)
{
	//pNPnode github_node = NULL;
	pNPgithubIssue current_issue = NULL;
	static float y_translate = 0;
	static float x_translate = 0;
	static float z_translate = 0;
	static int id = 0;
	int childIndex = 0;
	
	int monthIndex = 0;
	int yearIndex = 0;
	int monthClosedIndex = 0;
	int yearClosedIndex = 0;
	int monthCreatedIndex = 0;
	int yearCreatedIndex = 0;
	int issuesInYear2014 = 0;
	int issuesInYear2015 = 0;

	
	int issueIndex = 0;
	pNPnode time_node = NULL; /// Not to be confused with time cube
	time_node = npNodeNew(kNodePin, NULL, dataRef);
//	time_node->geometry = kNPgeoPin;
	time_node->geometry = kNPgeoTorus;
	time_node->topo = kNPtopoTorus;
	time_node->scale.z = 4;
	time_node->scale.x = 4;
	time_node->scale.y = 4;
	//time_node->scale.z = 90;
	//time_node->rotate.x = 90;

	/// yearIndex range [0 (2014), 1 (2015)]
	for(yearIndex = 0; yearIndex <= 1; yearIndex++)
	{
		time_node->child[yearIndex] = npNodeNew(kNodePin, time_node, dataRef);
		time_node->child[yearIndex]->geometry = kNPgeoTorus;
		time_node->child[yearIndex]->translate.y += 180;

		if(yearIndex == 0)
			time_node->child[yearIndex]->translate.x += 180;

		/// monthIndex range [0 (January) ,11 (December)]
		for(monthIndex = 0; monthIndex <= 11; monthIndex++)
		{
			time_node->child[yearIndex]->child[monthIndex] = npNodeNew(kNodePin, time_node->child[yearIndex], dataRef);
			time_node->child[yearIndex]->child[monthIndex]->geometry = kNPgeoTorus;
			//time_node->child[yearIndex]->child[monthIndex]->translate.x = (15 * monthIndex);
			time_node->child[yearIndex]->child[monthIndex]->translate.x = (30 * monthIndex);
		}
	}

/// Have a seperate node for creation and closing of an issue, place each on time hierarchy 
/// makes issues tree, link together creation nodes and closing nodes
	/// A month node scales with how many issues it has associated with it

	time_node->scale.y = (issues->count) * 0.10;

	while(issueIndex < issues->count)
	{
		current_issue = issues->issue[issueIndex];
		//issues->issue[issueIndex]->
		if( strcmp(current_issue->state, "closed") == 0 )
		{
			new_npGithubGetIssueClosedAt(current_issue);
			yearClosedIndex =  current_issue->issue_closed_at->year - 2014;
			monthClosedIndex = current_issue->issue_closed_at->month - 1;

			new_npGithubGetIssueCreatedAt(current_issue);
			yearCreatedIndex = current_issue->issue_created_at.year - 2014;
			monthCreatedIndex = current_issue->issue_created_at.month - 1;

			printf("\nYear Created Index : %d Year %d", yearCreatedIndex, current_issue->issue_created_at.year);
			printf("\nMonth Created Index : %d Month %d", monthCreatedIndex, current_issue->issue_created_at.month);
			
			time_node->child[yearCreatedIndex]->child[monthCreatedIndex]->child[id] = npNodeNew(kNodePin, time_node->child[yearCreatedIndex]->child[monthCreatedIndex], dataRef);
			//time_node->child[yearCreatedIndex]->child[monthCreatedIndex]->child[id]->geometry = kNPgeoPin;
		//	time_node->child[yearCreatedIndex]->child[monthCreatedIndex]->child[id]->translate.y += 45;
			time_node->child[yearCreatedIndex]->child[monthCreatedIndex]->translate.y += 0.1;
			id++;
			

		}
		else if( strcmp(current_issue->state, "open") == 0 )
		{
			new_npGithubGetIssueCreatedAt(current_issue);
			yearCreatedIndex = current_issue->issue_created_at.year - 2014;
			monthCreatedIndex = current_issue->issue_created_at.month - 1;

			printf("\nYear Created Index : %d Year %d", yearCreatedIndex, current_issue->issue_created_at.year);
			printf("\nMonth Created Index : %d Month %d", monthCreatedIndex, current_issue->issue_created_at.month);

			time_node->child[yearCreatedIndex]->child[monthCreatedIndex]->child[id] = npNodeNew(kNodePin, time_node->child[yearCreatedIndex]->child[monthCreatedIndex], dataRef);
		//	time_node->child[yearCreatedIndex]->child[monthCreatedIndex]->child[id]->geometry = kNPgeoPin;
		//	time_node->child[yearCreatedIndex]->child[monthCreatedIndex]->translate.y += 0.1;
			id++;

		}

		/*
		if(yearCreatedIndex == 0)
		{
			issuesInYear2014++;
	//		time_node->child[yearCreatedIndex]->scale.x += 0.1;
			time_node->child[yearCreatedIndex]->scale.z += 0.1;
			time_node->child[yearCreatedIndex]->scale.x += 0.1;
			time_node->child[yearCreatedIndex]->scale.y += 0.1;
		}
		else if(yearCreatedIndex == 1)
		{
			issuesInYear2015++;
	//		time_node->child[yearCreatedIndex]->scale.x += 0.1;
			time_node->child[yearCreatedIndex]->scale.z += 0.1;
			time_node->child[yearCreatedIndex]->scale.x += 0.1;
			time_node->child[yearCreatedIndex]->scale.y += 0.1;
		}
		*/

	//	time_node->scale.x += 0.1;
	//	time_node->scale.z += 0.1;
		
		issueIndex++;
	}

/*
	printf("2374 issues->count : %d", issues->count);
	while(childIndex < issues->count)
	{
		time_node->child[childIndex] = npNodeNew(kNodePin, time_node, dataRef);
	//	time_node->child[childIndex]->geometry = kNPgeoCylinder;
		time_node->child[childIndex]->geometry = kNPgeoTorus;
		time_node->child[childIndex]->scale.z = 3;


		if( strcmp(issues->issue[childIndex]->state, "open") == 0 )
		{
			time_node->child[childIndex]->color.r = 255;
			time_node->child[childIndex]->color.g = 0;
			time_node->child[childIndex]->color.b = 0;
		}
		else
		{
			time_node->child[childIndex]->color.r = 0;
			time_node->child[childIndex]->color.g = 0;
			time_node->child[childIndex]->color.b = 255;
		}

		time_node->child[childIndex]->translate.x = x_translate;
		x_translate += 3;

		childIndex++;
	}
*/
}

pNPnode new_new_npGithubNewIssueGlyph(pNPgithubIssue issue, void* dataRef)
{
	time_t issue_creation_time;
	time_t issue_closed_time;
	struct tm created_at_time;
	struct tm closed_at_time;
	struct tm *now_time;
	time_t current_time;
	char buffer[80];
	double duration_of_existence;	
	double time_to_close;
	pNPnode issue_node;
	pNPnode timeOpen_node;
	pNPnode timeClosed_node;
	pNPnode issueTitle_node;
	pNPtag issueTitle_tag;
	pNPnode issueUserLogin_node;
	static int id = 1;
	issueUserLogin_node = NULL;
	issueTitle_node = NULL;
	timeClosed_node = NULL;
	timeOpen_node = NULL;
	timeClosed_node = NULL;
	issue_node = NULL;
	issue_creation_time = NULL;
	duration_of_existence = 0;
	issueTitle_tag = issue->titleTag;


	issue_node = npNodeNew(kNodePin, NULL, dataRef);
	issue_node->geometry = kNPgeoTorus;
	issue_node->topo = kNPtopoTorus;
	//node = npNodeNew(issue->issueNodeType, NULL, dataRef);
	//current_time = time(NULL);
	time(&current_time);
	now_time = localtime(&current_time);
	//printf("Current local time and date: %s", asctime(now_time));

	//strftime(buffer, sizeof(buffer), "%c", &now_time);
	//printf("\n%s", buffer);

	new_npGithubGetIssueCreatedAt(issue);
	created_at_time.tm_year = issue->issue_created_at.year - 1900;
	created_at_time.tm_mon  = issue->issue_created_at.month - 1;
	created_at_time.tm_mday = issue->issue_created_at.day;
	created_at_time.tm_hour = issue->issue_created_at.hour;
	created_at_time.tm_min  = issue->issue_created_at.minute;
	created_at_time.tm_sec  = issue->issue_created_at.second;
	created_at_time.tm_isdst = -1;

	issue_creation_time = mktime(&created_at_time);
	if( issue_creation_time == -1 )
	{
		printf("Error: unable to make time using mktime\n");
	}
	else
	{
		strftime(buffer, sizeof(buffer), "%c", &created_at_time);
	//	printf("\nIssue Opened @ %s", buffer);
	}

	issue_node->translate.z  = (float)((float)created_at_time.tm_year - 114) * 5;
	issue_node->translate.z += (created_at_time.tm_yday  * ( 5/365) );
	issue_node->translate.z += ( created_at_time.tm_mon  * ( 5/11 )  );
	issue_node->translate.z += ( created_at_time.tm_mday * ( 5/31 )  );
	issue_node->translate.z += ( created_at_time.tm_hour * ( 5/23 )  );
	issue_node->translate.z += ( created_at_time.tm_min  * ( 5/59 )  );
	issue_node->translate.z += ( created_at_time.tm_sec  * ( 5/59 )  );

//	issue_node->translate.z += (float)((float)created_at_time.tm_yday / 365) * 5;
//	issue_node->translate.z += (created_at_time.tm_hour / 24) * 5;
	//issue_node->translate.z += (float)((float)created_at_time.tm_hour / 23)  *  ;


//	issue_node->translate.z  += (float)((float)created_at_time.tm_yday / 365) * 10;
//	issue_node->translate.z += (float)((float)created_at_time.tm_mon / 11) * 0.9;
//	issue_node->translate.z += (float)((float)created_at_time.tm_md
//	issue_node->translate.z = (float)((double)issue_creation_time / 100000000) * 15;
	//printf("\n2345 year : %d", (int)created_at_time.tm_year - 114) * 5);
//	printf("\n2346b yearDay : %d"
	//printf("\n2344 month : %f", (float)((float)created_at_time.tm_mon / 11) * 0.9); 
	//printf("\n2346 issue_node->translate.z : %f", issue_node->translate.z);
		
	if(strcmp(issue->state, "closed") == 0)
	{
		new_npGithubGetIssueClosedAt(issue);
		closed_at_time.tm_year = issue->issue_closed_at->year - 1900;
		closed_at_time.tm_mon  = issue->issue_closed_at->month - 1;
		closed_at_time.tm_mday = issue->issue_closed_at->day;
		closed_at_time.tm_hour = issue->issue_closed_at->hour;
		closed_at_time.tm_min  = issue->issue_closed_at->minute;
		closed_at_time.tm_sec  = issue->issue_closed_at->second;
		issue_closed_time = mktime(&closed_at_time);

		time_to_close = difftime(issue_closed_time, issue_creation_time);
	//	printf("\nIssue closed @ %s", asctime(&closed_at_time));
	//	printf("\nTime to close : %f\n", time_to_close);

		issue_node->color.b = 255;
        issue_node->color.r = 0;
        issue_node->color.g = 0;

		timeClosed_node = npNodeNew(kNodePin, issue_node, dataRef);
		timeClosed_node->geometry = kNPgeoTorus;
		timeClosed_node->topo = kNPtopoTorus;
		timeClosed_node->scale.z = (float)(time_to_close / (double)issue_creation_time) * 500;
	//	printf("\nissue_creation_time : %f", (double)issue_creation_time);
	//	printf("\ntimeClosed_node->scale.z : %f", timeClosed_node->scale.z);
	}
	else
	{
		duration_of_existence = difftime(current_time, issue_creation_time);
	//	printf("\nHas existed for %f seconds \n", duration_of_existence);

		/// (time_open / total_seconds_to_now) * 15000
		/*
		issue_node->translate.z = (duration_of_existence / current_time) * 1000;
		printf("\n(duration_of_existence / current_time) : %f", (duration_of_existence / current_time) * 1000);
		*/
		timeOpen_node = npNodeNew(kNodePin, issue_node, dataRef);
		timeOpen_node->geometry = kNPgeoTorus;
		timeOpen_node->topo = kNPtopoTorus;
		timeOpen_node->scale.z = (float)(duration_of_existence / (double)current_time) * 500;
	//	printf("\ntimeOpen_node->scale.z : %f", timeOpen_node->scale.z);

		issue_node->color.b = 0;
        issue_node->color.r = 255;
        issue_node->color.g = 0;
	}

    issueTitle_node = npNodeNew(kNodePin, issue_node, dataRef);
    issueTitle_node->geometry = kNPgeoSphere;
    issueTitle_node->topo = kNPtopoTorus;
    issueTitle_node->scale.x = issue->numOfWordsInIssueTitle / 2;
    issueTitle_node->translate.x += 45;

    issueUserLogin_node = npNodeNew(kNodePin, issue_node, dataRef);
    issueUserLogin_node->geometry = kNPgeoCube;
    issueUserLogin_node->translate.x += (-45);	
    issueUserLogin_node->textureID = issue->partOf->current->user.avatar_image_textureID;

	
	///issueTitle_tag /// lde @todo
	sprintf(issueTitle_tag->title, "%s", issue->title);
	issueTitle_tag->titleSize = strlen(issue->title) + 1;
	issueTitle_node->recordID = (issueTitle_tag->recordID = id);
	npTagSortAdd(issueTitle_tag, dataRef);
    npTagNode(issueTitle_node, dataRef);
	id++;
	/*
     title_recordTag = npNewTag( dataRef );
     sprintf(title_recordTag->title, "%s", issue->title);
     //    strcpy( recordTag->title, "TIA" );
     title_recordTag->titleSize = strlen(issue->title) + 1;
     issue_node->recordID = title_recordTag->recordID = id;
     id++;
    */
    

	/*
	issue->partOf->recordTagVariableIndex = 1;
	issue->partOf->issueVariableIndex = 0;
    issueTitle_tag = npGithubIssueInitNodeTag(issue, dataRef);
    issueTitle_node->recordID = issueTitle_tag->recordID = id;
    npTagSortAdd(issueTitle_tag, dataRef);
    npTagNode(issueTitle_node, dataRef);
    id++;
	*/



	return issue_node;
}

pNPnode new_npGithubNewIssueGlyph(pNPgithubIssue issue, void* dataRef)
{
	/// use time_t
	time_t time = NULL;
	struct tm * now = NULL;
	double secondsSince2014 = 0;
	double secondsIn2014years = 0;
	double totalSecondsToClose = 0;
	double totalSecondsToNow = 0;
	double timeOpen = 0;
	float totalTranslate = 0;
    pNPnode issue_node = NULL;
    pNPrecordTag record_tag = NULL;
    pNPrecordTag issueTitle_tag = NULL;
    static int id = 1;
    double totalSeconds = 0;
    float totalScale = 0;
    pNPnode timeToClose_node = NULL;
    pNPnode timeOpen_node = NULL;
    pNPnode issueTitle_node = NULL;
    pNPnode issueUserLogin_node = NULL;
    
    issue->issueNodeType = kNodePin;
    issue->issueGeoType = kNPgeoTorus;
    issue->issueTopoType = kNPtopoTorus;
    
	now = localtime(&time);

    issue_node = npNodeNew(issue->issueNodeType, NULL, dataRef);
    if(issue_node == NULL)
    {
        printf("\nnpNodeNew Failed");
    //    free(cwd);
        return NULL;
    }
    
	now = localtime( &time );

    issue_node->geometry = issue->issueGeoType;
    issue_node->topo = issue->issueTopoType;
//    printf("\n2306 issue->issueGeoType : %d && kNPpin : %d", issue_node->geometry, kNPpin);
    
    /*
    npGithubCtrlSetRecordTagVariableFormatIndex(issue, 1);
    npGithubCtrlSetRecordTagVariableIndex(issue, 1);
    npGithubCtrlSetIssueVariableIndex(issue, 2);
    npGithubCtrlSetCurrentIssue(issue, dataRef);
    npGithubCtrlSetRecordTag(issue, 0, dataRef);
    npGithubCtrlInitRecordTag(issue, 0, dataRef);
    npGithubCtrlSetRecordTagIndex(issue->partOf, 0, dataRef);
     */
    /* temp
    record_tag = npGithubIssueInitNodeTag(issue, dataRef);
    issue_node->recordID = record_tag->recordID = id;
    npTagSortAdd(record_tag, dataRef);
    npTagNode(issue_node, dataRef);
    id++;
    */
    
    issueTitle_node = npNodeNew(kNodePin, issue_node, dataRef);
    issueTitle_node->geometry = kNPgeoSphere;
    issueTitle_node->topo = kNPtopoTorus;
   // issueTitle_node->scale.x = numOfWordsInIssueTitle / 2;
    issueTitle_node->scale.x = issue->numOfWordsInIssueTitle / 2;
    issueTitle_node->translate.x += 45;
    
 /// @todo   npGithubCtrlAssignIssueTitleToTagTitle(issue, issueTitle_tag);
    /*
    issue->partOf->recordTagVariableIndex = 0;
    issue->partOf->issueVariableIndex = 0;
    issueTitle_tag = npGithubIssueInitNodeTag(issue, dataRef);
    issueTitle_node->recordID = issueTitle_tag->recordID = id;
    npTagSortAdd(issueTitle_tag, dataRef);
    npTagNode(issueTitle_node, dataRef);
    id++;
    */
    
    
    //npGithubGetIssueCreatedAtYear(issue);
    new_npGithubGetIssueCreatedAt(issue);
   

    
    secondsIn2014years = 6.351 * 10000000000;
    totalSeconds = ( (issue->issue_created_at.year * (3.154 * 10000000)) ) + (issue->issue_created_at.month * (2.628 * 1000000))
    + (issue->issue_created_at.day * (86400)) + (issue->issue_created_at.hour * (3600)) + (issue->issue_created_at.minute * (60))
    + (issue->issue_created_at.second * (1));
    
    secondsSince2014 = totalSeconds - secondsIn2014years;
    totalTranslate = (secondsSince2014 / totalSeconds ) * 10000;
    
 //   printf("\n2303 totalTranslate : %2f", totalTranslate);

    ///in Seconds
    issue_node->translate.z = totalTranslate;
    
    
    
    /// scale x & y parameters of closed issue nodes by the amount of time between opening and closing dates
    if(strcmp(issue->state, "closed") == 0)
    {
        new_npGithubGetIssueClosedAt(issue);
        timeToClose_node = npNodeNew(kNodePin, issue_node, dataRef);
        timeToClose_node->geometry = kNPgeoTorus;
        timeToClose_node->topo = kNPtopoTorus;
        issue_node->color.b = 255;
        issue_node->color.r = 0;
        issue_node->color.g = 0;
        
       /// totalScale
        
        totalSecondsToClose = ( (issue->issue_closed_at->year * (3.154 * 10000000)) ) + (issue->issue_closed_at->month * (2.628 * 1000000))
        + (issue->issue_closed_at->day * (86400)) + (issue->issue_closed_at->hour * (3600)) + (issue->issue_closed_at->minute * (60))
        + (issue->issue_closed_at->second * (1));
        totalSecondsToClose -= totalSeconds;
        totalScale = (totalSecondsToClose / totalSeconds) * 15000;
        
  //      printf("\n2304 totalSeconds : %2f", totalSecondsToClose);
   //     printf("\n2305 totalScale : %2f", totalScale);
        
        
        timeToClose_node->scale.z = totalScale;
     //   timeToClose_node->topo = kNPtopoTorus;
        
    }
    else
    { /// State : "open" 
	 
        double totalSecondsToNow = ( (2015 * (3.154 * 10000000)) ) + (7 * (2.628 * 1000000))
        + (17 * (86400)) + (12 * (3600)) + (57 * (60))
        + (11 * (1));
      

		/*
		totalSecondsToNow =  (double)(now->tm_year + 1900) * (3.154 * 10000000);
		totalSecondsToNow += (double)(now->tm_mon  * 2678400);
		totalSecondsToNow += (double)(now->tm_mday * 86400);
		totalSecondsToNow += (double)(now->tm_hour * 3600);
		totalSecondsToNow += (double)(now->tm_min  * 60);
		totalSecondsToNow += (double)(now->tm_sec  * 1);
*/
		/*	
		totalSecondsToNow = (now->tm_year * (3.154 * 10000000));
		totalSecondsToNow += (now->tm_mon * (7 * (2.628 * 1000000)));
		totalSecondsToNow += (now->tm_mday * 86400);
		totalSecondsToNow += (now->tm_hour * 3600);
		totalSecondsToNow += (now->tm_min * 60);
		totalSecondsToNow += (now->tm_sec * 1);
		*/
		/*
		double totalSecondsToNow = (now->tm_year + 1900 * (3.154 * 10000000) ) + ((now->tm_mon+1) * (2.628 * 1000000))
			+ (now->tm_mday * 86400) + (now->tm_hour * 3600) + (now->tm_min * 60))
			+ (now->tm_sec * 1);
		*/
		
        timeOpen = totalSecondsToNow - totalSeconds;
        totalScale = ((float)timeOpen / (float)totalSecondsToNow) * 15000;
     //   printf("\n2306 timeOpen : %2f", timeOpen);
     //   printf("\n2307 totalScale : %2f", totalScale);
        
      //  issue_node->scale.x = totalScale / 2;
      //  issue_node->scale.z = totalScale;
      //  issue_node->scale.y = totalScale;
      //  issue_node->scale.x = totalScale;
      //  issue_node->translate.z -= totalScale;
        //issue_node->translate.z += issue_node->translate.z;
        
        
        timeOpen_node = npNodeNew(kNodePin, issue_node, dataRef);
        timeOpen_node->geometry = kNPgeoTorus;
        timeOpen_node->topo = kNPtopoTorus;
        timeOpen_node->scale.z = totalScale;
        
        issue_node->color.b = 0;
        issue_node->color.r = 255;
        issue_node->color.g = 0;
    }
    
    issueUserLogin_node = npNodeNew(kNodePin, issue_node, dataRef);
    issueUserLogin_node->geometry = kNPgeoCube;
    issueUserLogin_node->translate.x += (-45);
 //   issueUserLogin_node->textureID = ;
    
 //   issue->user.avatar_image_file = strcat(cwd, issue->user.avatar_image_path);
//    free(cwd);
 //   sprintf(issue->user.avatar_image_path, "%s", cwd, issue->user.avatar_image_path);
 //   printf("\n2312 user.avatar_image_path = %s", issue->user.avatar_image_path );
   ///SOIL_CREATE_NEW_ID
    /// @todo npGithubCtrlSetUserTexture(pNPgithubIssues issues, void* dataRef)
 /*
    avatarTextureID = SOIL_load_OGL_texture
    (
     issue->user.avatar_image_file_path,
     SOIL_LOAD_AUTO,
     SOIL_CREATE_NEW_ID,
     SOIL_FLAG_INVERT_Y |
     SOIL_FLAG_MIPMAPS //|			//disabling breaks RGBA textures
     // | SOIL_FLAG_NTSC_SAFE_RGB	//we want the entire RGB spectrum
     // | SOIL_FLAG_COMPRESS_TO_DXT	//no lossy compression, faster too
     );																// texture, debug zz
    
    if(avatarTextureID == 0)
    {
        printf("\n2309 Soil loading error : %s", SOIL_last_result());
        
    }
    else
    {
        data->io.gl.textureCount = avatarTextureID;
    }
*/
    issueUserLogin_node->textureID = issue->partOf->current->user.avatar_image_textureID;
    
    
    /// 23
//    printf("\n2308 avatarTextureID : %d", issueUserLogin_node->textureID);
    //the last texture loaded is the texture count, non-loads return a texture=0
/*    if (textureID)
    {
        data->io.gl.textureCount = textureID;
        printf ("Loaded textureID: %d\n", textureID);
    }
 */
    
    /*
    issue->partOf->recordTagVariableFormatIndex = 1;
    issue->partOf->recordTagVariableIndex = 1;
    issue->partOf->issueVariableIndex = 4;
    record_tag = npGithubIssueInitNodeTag(issue, dataRef);
    issueUserLogin_node->recordID = record_tag->recordID = id;
    npTagSortAdd(record_tag, dataRef);
    npTagNode(issueUserLogin_node, dataRef);
    id++;
    */
/*
    totalSeconds = (double)((double)2678400 * ((double)issue->issue_created_at->month * ((double)2016 - (double)issue->issue_created_at->year)) );
    totalSeconds += (double)((double)issue->issue_created_at->day * (double)86400);
    totalSeconds += (double)((double)issue->issue_created_at->hour * (double)3600);
    totalSeconds += (double)((double)issue->issue_created_at->minute * (double)60);
    totalSeconds += (double)((double)issue->issue_created_at->second);
*/
    
    
 //  totalSeconds *= (issue->issue_created_at->year / 2015);
    
    /*
    issue_node->translate.z = ((issue->issue_created_at->year / 2015) + (issue->issue_created_at->month / 12) +
                                (issue->issue_created_at->day / 31) + (issue->issue_created_at->hour / 24) +
                                (issue->issue_created_at->minute / 60) + (issue->issue_created_at->second / 60)) * 5;
    printf("\ntranslate.z : %2f", issue_node->translate.z);
    */
     
    return issue_node;
}

/// This should take an issue and return pNPgithubIssueNode
pNPnode npGithubNewIssueGlyph(pNPgithubIssue issue, void* dataRef)
{
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
    /*
     title_recordTag = npNewTag( dataRef );
     sprintf(title_recordTag->title, "%s", issue->title);
     //    strcpy( recordTag->title, "TIA" );
     title_recordTag->titleSize = strlen(issue->title) + 1;
     issue_node->recordID = title_recordTag->recordID = id;
     id++;
    */
    
    
    
    
     
     /// @todo npTagGithubIssueNode
    
    /*
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
    */
    
    return issue_node;
}

NPgithubErrValue npGithubSetCurrentIssue(pNPgithubIssues issues, int (*gen_issue_id)(void*), void* data)
{
   // github->current = gen_issue_id(data);
    int issue_index = gen_issue_id(issues);
 //   github->current =
    issues->current = issues->issue[issue_index];
    
    return 0;
}

//NPgithubErrValue npGithubSetIssue

NPgithubErrValue npGithubSetCurrentIssueUrl(pNPgithubIssues issues, char* (*gen_issue_url)(void*), void* data)
{
    issues->current->url = gen_issue_url(data);
    printf("\ngithub->current->url : %s", issues->current->url);
    
    return 0;
}

NPgithubErrValue npGithubSetCurrentIssueId(pNPgithubIssues issues, NPgithubIssueID (*gen_issue_id)(void*),void* data)
{
    issues->current->id = gen_issue_id(data);
    
    return 0;
}


NPgithubErrValue npGithubSetCurrentIssueNumber(pNPgithubIssues issues, NPgithubIssueID (*gen_issue_number)(void*),void* data)
{
    issues->current->index = gen_issue_number(data);
    
    return 0;
}


pNPgithubIssueUrl npGithubGetCurrentIssueUrl(pNPgithubIssues issues)
{
    return issues->current->url;
}


#define Zero 0
#define is ==
/// Encapsulate json_t within NPjson
/*
struct NPgithubIssue* other_npJSONgetGithubIssue(pNPgithubIssues issues, json_t * data)
{
 //   json_t *url, *id, *number;
    pNPgithubIssue issue;
    NPgithubIssueCount count = Zero;

    if( npGithubGetIssueCount(issues) is Zero)
    {
        npGithubIssueCountIncrement(issues);
    }
    else
    {
        count = npGithubGetIssueCount(issues);
        npGithubIssueCountIncrement(issues);
    }

    npGithubSetCurrentIssue(issues, &npGithubGetIssueCount, data);

    npGithubSetCurrentIssueUrl(issues, &npJSONgetGithubIssueUrl, data);
    
    npGithubSetCurrentIssueId(issues, &npJSONgetGithubIssueId, data);
    
    npGithubSetCurrentIssueNumber(issues, &npJSONgetGithubIssueNumber, data);
    
    return issues->current;
}
*/

json_t* new_npJSONgetGithubIssueByIndex(pNPgithubIssues issues, pNPjson json, int index, void* dataRef)
{
   // json_t* issue = root->issue[index];
    //json_t* issue = json->issue[index];
    
    json->issue[index] = json_array_get(json->root, index);
    json->numOfIssues++;
    json->issueIndex = index;
    
    return json->issue[index];
}

json_t* npJSONgetGithubIssueByIndex(pNPgithubIssues issues, json_t * root, int index, int* arraySize)
{
    json_t * issue = NULL;
    if(index >= 0 && ( index < (*arraySize) ) ) /// make sure this isn't off by one
    {
        issue = json_array_get(root, index);
    }
    
    return issue;
}

void npGithubSetIssueUrl(pNPgithubIssues issues, pNPjson json, void* dataRef)
{
   // issues->issue[issues->index]->url = gen_issue_url(issue);
    //npJSONgetGithubIssueUrl(
    issues->issue[issues->index]->url = npJSONgetGithubIssueUrl(json->issue[json->issueIndex]);
}

/*
NPgithubErrValue npGithubSetIssueUrl(pNPgithubIssues issues, int index, char* (*gen_issue_url)(void*), void* issue)
{
    issues->issue[index]->url = gen_issue_url(issue);
 //   printf("\nissues->issue[%d]->url: %s", index, issues->issue[index]->url);
    
    return 0;
}
*/
 
void npGithubSetIssueId(pNPgithubIssues issues, pNPjson json, void* dataRef)
{
    issues->issue[issues->index]->id = npJSONgetGithubIssueId(json->issue[json->issueIndex]);
    
}

/*
NPgithubErrValue npGithubSetIssueId(pNPgithubIssues issues, int index, long long (*gen_issue_id)(void*), void* issue)
{
    issues->issue[index]->id = gen_issue_id(issue);
    
    return 0;
}
 */

char* npGithubSetIssueTitle(pNPgithubIssues issues, pNPjson json, void* dataRef)
{
    return issues->issue[issues->index]->title = npJSONgetGithubIssueTitle(json->issue[json->issueIndex]);
}

/*
char* npGithubSetIssueTitle(pNPgithubIssues issues, int index, char* (*gen_issue_title)(void*), void* issue)
{
    issues->issue[index]->title = gen_issue_title(issue);
    
    return issues->issue[index]->title;
}
*/
 
char* npGithubSetIssueCreatedAt(pNPgithubIssues issues, pNPjson json, void* dataRef)
{
    issues->issue[issues->index]->created_at = npJSONgetGithubIssueCreatedAt(json->issue[json->issueIndex]);
    return issues->issue[issues->index]->created_at;
}

/*
char* npGithubSetIssueCreatedAt(pNPgithubIssues issues, int index, char* (*gen_created_at)(void*), void* issue)
{
    issues->issue[index]->created_at = gen_created_at(issue);
    
    return issues->issue[index]->created_at;
}
*/
char* npGithubSetIssueUpdatedAt(pNPgithubIssues issues, pNPjson json, void* dataRef)
{
    return issues->issue[issues->index]->updated_at = npJSONgetGithubIssueUpdatedAt(json->issue[json->issueIndex]);
}

/*
char* npGithubSetIssueUpdatedAt(pNPgithubIssues issues, int index, char* (*gen_created_at)(void*), void* issue)
{
    issues->issue[index]->updated_at = gen_created_at(issue);
    
    return issues->issue[index]->updated_at;
}
*/

void npGithubSetIssueClosedAt(pNPgithubIssues issues, pNPjson json, void* dataRef)
{
    //issues->issue[issues->index]->closed_at = npJSONgetGithubIssueUpdatedAt(json->root);
    issues->issue[issues->index]->closed_at = npJSONgetGithubIssueClosedAt(json->issue[issues->index]);

}

/*
char* npGithubSetIssueClosedAt(pNPgithubIssues issues, int index, char* (*gen_closed_at)(void*), void* issue)
{
    issues->issue[index]->closed_at = gen_closed_at(issue);
    
    return issues->issue[index]->closed_at;
}
*/

void npGithubSetIssueState(pNPgithubIssues issues, pNPjson json, void* dataRef)
{
    issues->issue[issues->index]->state = npJSONgetGithubIssueState(json->issue[json->issueIndex]);
}

/*
char* npGithubSetIssueState(pNPgithubIssues issues, int index, char* (*gen_issue_state)(void*), void* issue)
{
    issues->issue[index]->state = gen_issue_state(issue);
    
    return issues->issue[index]->state;
}
 */

typedef void* (*gen_issue_func);
typedef gen_issue_func (*npGetIssueFunc);
typedef npGetIssueFunc (*npGetIssueFunction);

/// temporary 903 is kNPlonglong
///void* (*issue_set_function[])
///npGetIssueFunc possible_func[] /// @todo npJSONgetGithubIssueUser


/*
 void* (*issue_set_function[]) = {npGithubSetIssueUrl, npGithubSetIssueId, npGithubSetIssueTitle, npGithubSetIssueState,
 npGithubSetIssueCreatedAt, npGithubSetIssueUpdatedAt, npGithubSetIssueClosedAt, npGithubSetIssueUser};
 */
void* new_npJSONgetGithubIssueFunction(pNPgithubIssues issues, pNPjson json ,int functionIndex, void* dataRef)
{
    void* (*func)(pNPgithubIssues issues, pNPjson json, void* dataRef) = issues->issueFunctions->npGithubIssueFunction[functionIndex];
    issues->issueVariableIndex = functionIndex;
    
    func(issues, json, dataRef);
    


    return func;
}



npGetIssueFunc npJSONgetGithubIssueFunction(char* issue_variable, int* returnType)
{
    void* (*possible_func[]) = {(void*)npJSONgetGithubIssueUrl, (void*)npJSONgetGithubIssueId, (void*)npJSONgetGithubIssueTitle, (void*)npJSONgetGithubIssueNumber, (void*)npJSONgetGithubIssueState,
    (void*)npJSONgetGithubIssueCreatedAt, (void*)npJSONgetGithubIssueUpdatedAt, (void*)npJSONgetGithubIssueClosedAt, (void*)npJSONgetGithubIssueUserLogin};
    char* possible[] = {"url", "id", "title", "state", "created_at", "updated_at", "closed_at", "login"};
    int returnTypes[] = {kNPcharArray, 903, kNPcharArray, kNPcharArray, kNPcharArray, kNPcharArray, kNPcharArray};
    int possible_index = 0;
    int possible_array_size = 9;
    int returnTypes_index = 0;
    int strcmp_result = -1;
 //   void* (*gen_issue_general)(void*) = NULL;
    void* (*gen_issue_general) = NULL;
    
    for(possible_index = 0; possible_index < possible_array_size; possible_index++)
    {
        strcmp_result = strcmp(possible[possible_index], issue_variable);
        
        if(strcmp_result == 0)
        {
            (*returnType) = returnTypes[possible_index];
            gen_issue_general = possible_func[possible_index];
            break;
        }
        
    }
    
    return (void*)gen_issue_general;
}

char* npJSONgetGithubIssueState(json_t * issue)
{
    json_t *state;
    char* issue_state;
    state = npJSONgetObject(issue, "state");
    issue_state = (char*)json_string_value(state);
    return issue_state;
}

char* npJSONgetGithubIssueCreatedAt(json_t * issue)
{
    json_t *created_at;
    char* issue_CreatedAt;
    created_at = npJSONgetObject(issue, "created_at");
    issue_CreatedAt = (char*)json_string_value(created_at);
    return issue_CreatedAt;
}

char* npJSONgetGithubIssueUpdatedAt(json_t * issue)
{
    json_t *updated_at;
    char* issue_UpdatedAt;
    updated_at = npJSONgetObject(issue, "updated_at");
    issue_UpdatedAt = (char*)json_string_value(updated_at);
    return issue_UpdatedAt;
}

char* npJSONgetGithubIssueClosedAt(json_t * issue)
{
    json_t *closed_at;
    char* issue_ClosedAt;
    closed_at = npJSONgetObject(issue, "closed_at");
    issue_ClosedAt = (char*)json_string_value(closed_at);
  //  printf("23 24 closed : %s", issue_ClosedAt);
    return issue_ClosedAt;
}

char* npJSONgetGithubIssueUserLogin(json_t * issue)
{
    json_t *issueUser;
    json_t *data;
    char* issue_user_login = NULL;
    
    issueUser = npJSONgetObject(issue, "user");
    
    data = json_array_get(issueUser, 1);
    issue_user_login = (char*)json_string_value(data);
    return issue_user_login;
}

/*
void npGithubSetIssue(pNPgithubIssues issues, int github_issue_index, void* (*action)(void* issue), char* variable, void* issue)
{
    char* possible[] = {"url", "labels_url", "comments_url"}; /// @todo Make this a parameter
    char* (*gen_issue_url)(void*) = npJSONgetGithubIssueUrl;
  //  char* (*gen_issue_labels_url) = npJSONgetGithubIssueLabel;
    void* (*new_action[])(void* issue) = {gen_issue_url}; /// @todo Make this a parameter
    int possible_index;
    int strcmp_result = 0;
    
    for(possible_index = 0; possible_index < 3; possible_index++)
    {
        strcmp_result = strcmp(possible[possible_index], variable);
        if(strcmp_result == 0)
        {
            /// Same
            
            
        }
        
    }
    
}
*/

///npJSONgetGithubIssueFunction

struct NPjsonReturnValue
{
    int type;
    union returnedValue
    {
        long long longlongReturn;
        char* charArray;
    }value;
    
};
typedef struct NPjsonReturnValue NPjsonReturnValue;
typedef NPjsonReturnValue* pNPjsonReturnValue;

/// was pNPgithubIssueFunctions
pNPgithubIssueFunctions npGithubInitFunctions(void* (*functions[]), int num_functions)
{
    pNPgithubIssueFunctions github_functions = NULL;
 //   pNPgithubIssueFunction github_function = NULL;
//    void* (*new_func)(void*) = NULL;
    void* (*new_func)(void*) = NULL;
    int index = 0;
    
    github_functions = malloc(sizeof(NPgithubIssueFunctions));
    if(github_functions == NULL) return NULL;

    ///Replace with while not null
	num_functions = 10;
    for(index = 0; index < num_functions; index++)
    {
       //   new_func = functions[index];
		printf("\nfunction index : %d", index);
          new_func = (void*)functions[index];
          github_functions->npGithubIssueFunction[index] = new_func;
   //     github_functions->npGithubIssueFunction[index] = functions[index];
    }
    github_functions->currentPositionInLoop[0] = malloc(sizeof(int));
    //github_functions->variable = {"url", "id", "title", "state"};
    github_functions->variable[0] = "url";
    github_functions->variable[1] = "id";
    github_functions->variable[2] = "title";
    github_functions->variable[3] = "state";
    github_functions->variable[4] = "created_at";
    github_functions->variable[5] = "updated_at";
    github_functions->variable[6] = "closed_at";
    github_functions->variable[7] = "login";
    github_functions->variable[8] = "avatar_url";
    
    return github_functions;
}

void new_new2_npJSONsetGithubIssue(pNPgithubIssues issues, pNPgithubIssue issue, pNPjson json, void* dataRef)
{
	int index = 0;
    //new_npJSONgetGithubIssueFunction
    int functionIndex[] = {kNPgithubJSONgetIssueClosedAt,
							kNPgithubJSONgetIssueCreatedAt, 
							kNPgithubJSONgetIssueId, 
							kNPgithubJSONgetIssueNumber,
							kNPgithubJSONgetIssueState,
							kNPgithubJSONgetIssueTitle,
							kNPgithubJSONgetIssueUpdatedAt,
							kNPgithubJSONgetIssueUrl,
							kNPgithubJSONgetIssueUserLogin,
							kNPgithubJSONgetIssueUserAvatarUrl,
							kNPgithubJSONgetIssueClosedAt};
    
    //issues->index = 0;
    for(index = 0; index < 8; index++)
    {
		//printf("\nindex : %d", index);
        new_npJSONgetGithubIssueFunction(issues, json, functionIndex[index], dataRef);
    }
   //2 kNPgithub
    issues->issue[issues->index]->partOf = issues;
    
    
}


pNPgithubIssue new_npJSONsetGithubIssue(pNPgithubIssues issues, int github_issue_index, json_t * issue)
{
 //   char* github_variable[]
 //   void* (*issue_set_function[]) = {npGithubSetIssueUrl, npGithubSetIssueId, npGithubSetIssueTitle, npGithubSetIssueState};
 //   pNPgithubIssueFunctions issueFuncs = npGithubInitFunctions(issue_set_function, 4);
    int issue_set_function_array_index = 0;
    int issue_set_function_array_size = 1;
 //   char* variable[] = {"url", "id", "title", "state"};
    int variable_index = 0;
    int variable_array_size = 8; /// set to 6 when updated_at added
    int returnType = 0;
    
    int longlongReturn = 0;
    int* currentLoopIndex = issues->issueFunctions->currentPositionInLoop[0];
    char* charArray = NULL;
//    void* (*func)(void*) = NULL;
    void* (*func) = NULL;
    NPjsonReturnValue retVal;
 //   retVal.type = 0;
 //   retVal.value.longlongReturn = 0;
//    void* (*set_function)(pNPgithubIssues *issues, int github_issue_index, void* (*func)(), void* issue) = NULL;
    void* (*set_function)(pNPgithubIssues issues, int github_issue_index, void* (*func)(), void* issue) = NULL;

    ///issues->issueFunctions->currentPositionInLoop[0] = 0
    //for(variable_index = 0; variable_index < variable_array_size; variable_index++)
    (*currentLoopIndex) = 0;
    for(; (*currentLoopIndex) < variable_array_size; (*currentLoopIndex)++)
    {
        /// lde new added *
        func = *npJSONgetGithubIssueFunction(issues->issueFunctions->variable[(*currentLoopIndex)], &returnType);
        /* @todo
        switch (returnType)
        {
            case kNPcharArray:c                //charArray = func(issue);
                retVal.type = 0;
                retVal.value.charArray = func(issue);
                break;
            case 903:
                retVal.type = 1;
                retVal.value.longlongReturn = func(issue);
                break;
        }
        */
        //issue_set_function[variable_index](issues, github_issue_index, func, issue);
        //npGithubSetIssueUrl(issues, github_issue_index, func, issue);
    //    set_function = issue_set_function[variable_index];
//       set_function = issues->issueFunctions->npGithubIssueFunction[variable_index];
        set_function = issues->issueFunctions->npGithubIssueFunction[(*currentLoopIndex)];
        set_function(issues, github_issue_index, func, issue);
    }
    
    issues->issue[github_issue_index]->this_issue = issues->issue[github_issue_index];
    issues->issue[github_issue_index]->partOf = issues;
    issues->issue[github_issue_index]->user.avatar_image_textureID = 0;
    return issues->issue[github_issue_index];
}

pNPgithubIssue new_new_npJSONsetGithubIssue(pNPgithubIssues issues, pNPjson json, void* dataRef)
{
    int variable_array_size = 7; /// set to 6 when updated_at added
    int returnType = 0;
    
    int* currentLoopIndex = issues->issueFunctions->currentPositionInLoop[0];
    void* (*func)(void*) = NULL;
    //   retVal.type = 0;
    //   retVal.value.longlongReturn = 0;
//    void* (*set_function)(pNPgithubIssues *issues, int github_issue_index, void* (*func)(), void* issue) = NULL;
    
    void* (*set_function)(pNPgithubIssues issues, int github_issue_index, void* (*func)(), void* issue) = NULL;

    ///issues->issueFunctions->currentPositionInLoop[0] = 0
    //for(variable_index = 0; variable_index < variable_array_size; variable_index++)
    /*
    if((*currentLoopIndex) < variable_array_size)
    {
        func = (void*)npJSONgetGithubIssueFunction(issues->issueFunctions->variable[(*currentLoopIndex)], &returnType);
        set_function = (void*)issues->issueFunctions->npGithubIssueFunction[(*currentLoopIndex)];
        set_function(issues, issues->index, func, issue);
    }
     */
    for((*currentLoopIndex) = 0; (*currentLoopIndex) < variable_array_size; (*currentLoopIndex)++)
    {
        new_npJSONgetGithubIssueFunction(issues, json, (*currentLoopIndex), dataRef);
    }

    issues->issue[issues->index]->this_issue = issues->issue[issues->index];
    issues->issue[issues->index]->partOf = issues;
    return issues->issue[issues->index];
}

int npGithubSystemProcessIssues(int numToProcess, pNPgithubIssues issues)
{ /// @todo Later
    json_t *issuePlaceHolder;
    pNPgithubIssue issue = NULL;
    int numOfIssuesProcessed = 0;
    while(numOfIssuesProcessed < numToProcess)
    {
  //      issue = new_new_npJSONsetGithubIssue(issues, issues->index, issuePlaceHolder);
        if(issue != NULL)
        {
            numOfIssuesProcessed++;
        }
        else
        {
            return numOfIssuesProcessed;
        }
        
    }
    
        
    return numOfIssuesProcessed;
}

void npGithubSystem (pNPgithubIssues issues){
    /// @todo Later
	int numToProcess = 1;
    issues->index = 0;
    npGithubSystemProcessIssues(numToProcess, issues);
    
    
}


/// old NPgithubIssueIndex
/*
pNPgithubIssue npJSONsetGithubIssue(pNPgithubIssues issues, int github_issue_index, json_t * issue)
{
    /// This will overwrite an issue, so make sure that is what you want to do.
    pNPgithubIssue github_issue = NULL;
    
    
    if( github_issue_index >= 0 && github_issue_index <= issues->count )
    {
        github_issue = issues->issue[github_issue_index];
        npGithubSetIssueUrl(issues, github_issue_index, npJSONgetGithubIssueUrl, issue);
        npGithubSetIssueId(issues, github_issue_index, npJSONgetGithubIssueId, issue);
        npGithubSetIssueTitle(issues, github_issue_index, npJSONgetGithubIssueTitle, issue);
        npGithubSetIssueState(issues, github_issue_index, npJSONgetGithubIssueState, issue);
//        npGithubSetIssueCreatedAt(issues, github_issue_index, npJSONgetGithubIssueCreatedAt, issue);
    }
    
    return issues->issue[github_issue_index];
}
 */

/*
void npJSONsetGithubIssuesInRange(pNPgithubIssues issues, int from_index, int to_index, pNPjson json)
{
    int current_index;
    
    while(current_index >= from_index && current_index <= to_index)
    {
        npJSONsetGithubIssue(issues, current_index, json-> )
    }
    
}
*/

/*
pNPjson npJSONinit()
{
*/

pNPgithubIssue npJSONgithubIssueToAntzIssue(pNPjson gitJSON, int json_index, pNPgithubIssues issues, int github_index, void* dataRef)
{
    pNPgithubIssue github_issue = NULL;
    json_t * JSON_issue = NULL;
    
 //   JSON_issue = npJSONgetGithubIssueByIndex(issues, gitJSON->root, json_index, &gitJSON->array_size);
    JSON_issue = new_npJSONgetGithubIssueByIndex(issues, gitJSON, json_index, dataRef);
    if(JSON_issue == NULL)
    {
        /// No such issue exists at that index
        return NULL;
    }
    
 //   github_issue = npJSONsetGithubIssue(issues, github_index, JSON_issue);
//    github_issue = npJSONsetGithubIssue(issues, json_index, JSON_issue);
    
 //   github_issue = new_npJSONsetGithubIssue(issues, json_index, JSON_issue);
 //   github_issue = new_new2_npJSONsetGithubIssue(issues, issues[issues->index], dataRef);
    new_new2_npJSONsetGithubIssue(issues, issues->issue[issues->index], gitJSON, dataRef);
    
    //  github_issue->title = npJSONsetGithubIssueTitle(issues, github_index, JSON_issue);
 //   github_issue->url = npJSONgetGithubIssueUrl(gitJSON->root);
    
    return github_issue;
}

/*
void npJSONgithubIssuesToAntzIssues(pNPjson gitJSON, pNPgithubIssues issues)
{
    pNPgithubIssue github_issue = NULL;
    gitJSON->array_size = npJSONgetGithubIssuesArraySize(gitJSON->root, issues);
    
    /// make sure gitJSON->index is set properly, default is 0 when this function is called
    /// make sure github->index is set properly, default is 0 when this function is called
    issues->count = 0;
    while(gitJSON->jsonCtrl.index < gitJSON->array_size)
    {
        github_issue = npJSONgithubIssueToAntzIssue(gitJSON, gitJSON->index, issues, issues->index);
        //gitJSON->index++;
        issues->index++; issues->count++;
        gitJSON->jsonCtrl.index++;
    }
    
    return;
}
*/

/// Returns the number of githubIssues left to process
int npJSONgithubIssuesToAntzIssues(pNPjson gitJSON, pNPgithubIssues issues, void* dataRef)
{
    int left = 0;
    pNPgithubIssue github_issue = NULL;
    gitJSON->array_size = npJSONgetGithubIssuesArraySize(gitJSON->root, issues);
    
    
    /// make sure gitJSON->index is set properly, default is 0 when this function is called
    /// make sure github->index is set properly, default is 0 when this function is called
 ///   issues->count = 0; lde temp
    if(gitJSON->jsonCtrl.index < gitJSON->array_size)
    {
        github_issue = npJSONgithubIssueToAntzIssue(gitJSON, gitJSON->jsonCtrl.index, issues, issues->index, dataRef);
        
        
    //  gitJSON->index++;
        gitJSON->jsonCtrl.index++;
        
      //  issues->index++; ///issues->count++; // temp
    }
    
    left = issues->count - gitJSON->jsonCtrl.index;
    
    return left;
}



pNPnode npGithubAntzIssueToAntzNode(pNPgithubIssues issues, void* dataRef)
{
    /*
 //   issue->issue_node = npGithubNewIssueGlyph(issue, dataRef);
    npGithubCtrlSetRecordTagVariableFormatIndex(issue, 1);

    npGithubCtrlSetIssueVariableIndex(issue, 2);
    npGithubCtrlSetCurrentIssue(issue, dataRef);
    npGithubCtrlSetRecordTag(issue, 0, dataRef);
 //   npGithubCtrlInitRecordTag(issue, 0, dataRef);
    npGithubCtrlSetRecordTagIndex(issue->partOf, 0, dataRef);
    */
//    issues->current->issue_node = new_npGithubNewIssueGlyph(issues->issue[issues->index], dataRef);
      issues->current->issue_node = new_new_npGithubNewIssueGlyph(issues->issue[issues->index], dataRef);
  
    
    
   // return issue->issue_node;
    return issues->current->issue_node;
}
/*
void npGithubAntzIssuesToAntzNodes(pNPgithubIssues issues, void* dataRef)
{
    pNPgithubIssue issue = NULL;
    pNPgithubNode node = NULL;
    
    printf("\nnpGithubAntzIssuesToAntzNodes");

//    issues->parent = npNodeNew(kNodePin, NULL, dataRef);
    //issues->parent->geometry = 7;

//    issues->parent->topo = kNPtopoTorus;
    issues->parent->geometry = kNPgeoTorus;
    issues->parent->translate.z = 5;
//    issues->parent->scale.x = 8;
//    issues->parent->scale.y = 5;
//    issues->parent->scale.z = 5;

    issues->parent = NULL;
    
    printf("25 parent : %p", issues->parent);
    printf("26 count : %d", issues->count);

    issues->index = 0;
    issues->recordTagIndex = 0;
    while(issues->index < issues->count)
    {
        node = npGithubAntzIssueToAntzNode(issues->issue[issues->index], dataRef);
        issues->index += 1;
        issues->recordTagIndex += 1;
    }
    
    return;
}
*/

int npGithubAntzIssuesToAntzNodes(pNPgithubIssues issues, void* dataRef)
{
    pNPgithubIssue issue = NULL;
    pNPnode node = NULL;
    
 //   printf("\nnpGithubAntzIssuesToAntzNodes");

    issues->parent = NULL;
    issues->current = issues->issue[issues->index];
    issues->current->index = issues->index;
 //   printf("25 parent : %p", issues->parent);
  //  printf("26 count : %d", issues->count);
    
 //   issues->recordTagIndex = 0;

    if(issues->index < issues->count)
    {
     //   node = npGithubAntzIssueToAntzNode(issues, dataRef);
//		gitVizTest(issues, dataRef);      
		issues->recordTagIndex += 1;
    }
    
    return issues->count - issues->index;
}


/// @todo npJSONgetNextGithubIssue
/*
struct NPgithubIssue* npJSONgetGithubIssue(pNPgithubIssues issues, json_t * data)
{
    //   json_t *url, *id, *number;
    pNPgithubIssue issue;
    NPgithubIssueCount count = Zero;
    
    // @todo replace github->count with npGithubGetIssueCount
    if( npGithubGetIssueCount(issues) is Zero)
    {
        npGithubIssueCountIncrement(issues);
    }
    else
    {
        count = npGithubGetIssueCount(issues);
        npGithubIssueCountIncrement(issues);
    }
    
    
    npGithubSetCurrentIssue(issues, npGithubGetIssueCount, data);
    
    npGithubSetCurrentIssueUrl(issues, npJSONgetGithubIssueUrl, data);
 
//    npGithubSetCurrentIssueId(github, &npJSONgetGithubIssueId, data);
    
//    npGithubSetCurrentIssueNumber(github, &npJSONgetGithubIssueNumber, data);
 
     
    return issues->current;
}
*/




static size_t
WriteMemoryCallback(void *contents, size_t size, size_t nmemb, void *userp)
{
    size_t realsize = size * nmemb;
    struct MemoryStruct *mem = (struct MemoryStruct *)userp;
    
    mem->memory = realloc(mem->memory, mem->size + realsize + 1);
    if(mem->memory == NULL) {
        /* out of memory! */
        printf("not enough memory (realloc returned NULL)\n");
        return 0;
    }
    
    memcpy(&(mem->memory[mem->size]), contents, realsize);
    
    mem->size += realsize;
    mem->memory[mem->size] = 0;
    
    return realsize;
}

int npJSON_loads(char* buffer, int flags)
{
    
    return 0;
}

/*
json_t* npJSONgetVariableValue(json_t* json_data, char* variableName,){
    json_t *variable;
    variable = json_object_get(json_data, variableName);
//    issue[i].url = json_string_value(url);
    return variable;
}
*/

json_t* npJSONgetObject(json_t* data, char* key)
{
    json_t *object;
    object = json_object_get(data, key);
    if(object == NULL)
    {
        printf("\njson_object_get returned null");
    }
    
    return object;
}


long long npGithubGetIssueID()
{
    long long issue_id;
    return issue_id;
}

char* npGithubGetIssueURL()
{
    char* issue_url;
    return issue_url;
}

void npAntzSetIssue(int issue_id)
{
    return;
}

void npForRangeOfGithubIssues( struct NPgithubIssues * github, int from_index, int to_index, int * stop_index, int* condition(void*) )
{
    
}

void npGithubIssuesFilter(struct NPgithubIssues * github, int gen_issue_id(void*), void* data)
{
    
}

NPgithubIssueID npGithubGetIssueId(NPgithubIssues * github, NPgithubIssueIndex index)
{
    return github->issue[index]->id;
}

NPgithubIssueIndex npGithubGetIssueIndex(pNPgithubIssues issues)
{
    return issues->index;
}

void npGithubGet(json_t * data, char * variable_name)
{
    json_t *var;
    var = json_object_get(data, variable_name);
 //   issue[i].url = json_string_value(url);
}

void npGithubIssueGetURL()
{
    
}

/// github structure is statically allocated
void npGithubInit(struct NPgithubIssues * github)
{
    int i;
    
    #define kNPissueLimit 100
    for(i = 0; i < kNPissueLimit; i++)
    {
    //    github->issue[i] = NULL;
    }
}

char* npJSONgetGithubIssueUrl(json_t * issue)
{
    json_t *url;
    char* url_text = NULL;
    
    url = npJSONgetObject(issue, "url");
    url_text = (char*)json_string_value(url);
//    printf("\n1");
    if(url_text == NULL)
    {
        printf("json_string_value returned NULL");
        printf("23url");
    }
//    printf("\n2");
    
    return url_text;
}


int npJSONgetGithubIssueId(json_t * issue)
{
    json_t *id;
    int id_val;
    id = npJSONgetObject(issue, "id");
    id_val = json_integer_value(id);
	printf("\n2389 issue id : %d", id_val);
    return id_val;
}

/// @todo typedef json_t* pNPjsonIssue

//// typedef char* pNPgithubIssueTitle
char* npJSONgetGithubIssueTitle(json_t * issue)
{
    json_t *title;
    char* issue_title;
    title = npJSONgetObject(issue, "title");
    issue_title = (char*)json_string_value(title);
    return issue_title;
}

long long npJSONgetGithubIssueNumber(json_t * data)
{
    json_t *number;
    long long number_val;
  //  json_incref(data);
    number = npJSONgetObject(data, "number");
    number_val = json_integer_value(number);
	printf("\n2390 issue number : %d", number_val);
 //   json_decref(data);
    return number_val;
}

void npJSONgetGithubIssueVariable(json_t * issue, char* variable_name)
{
    
    
}

size_t npJSONgetGithubIssuesArraySize(json_t *root, pNPgithubIssues issues)
{
    size_t githubIssuesArraySize = 0;
    
    githubIssuesArraySize = json_array_size(root);
    issues->count = json_array_size(root);
    
    return githubIssuesArraySize;
}
/*
int npJSONgetGithubIssues(json_t *root, pNPgithubIssues github)
{
    int i;
    json_t *data;
    struct NPgithubIssue * issue;
    
    for(i = 0; i < json_array_size(root); i++)
    {
        data = json_array_get(root, i);
        
        if(!json_is_object(data))
        {
            fprintf(stderr, "error: data %d is not an object", i + 1);
            json_decref(root);
            return 1;
        }
        else
        {
            issue = npJSONgetGithubIssue(github, data);
        }
        
    }
    
    json_decref(root);
    return 0;
}
*/

//typedef pNPGithubTextTag pNPrecordTag;
int npzGithubTagSortAdd(pNPrecordTag recordTag, void* dataRef)
{
    npTagSortAdd(recordTag, dataRef);
    return 0;
}


int npzGithubSetIssueTextTag(struct NPgithubIssue *issue, void* dataRef)
{
    pNPrecordTag recordTag = NULL;
    recordTag = npNewTag( dataRef ); /// @todo : npNewIssueTag
    sprintf(recordTag->title, "%d", issue->index);
    recordTag->titleSize = 4;
 //   issue_node->recordID = recordTag->recordID = 1;
    issue->issue_node->recordID = recordTag->recordID = 1;
 //   npTagSortAdd(recordTag, dataRef);
    npzGithubTagSortAdd(recordTag, dataRef);
    
    return 0;
}

NPgithubIssueCount npGithubGetIssueCount(pNPgithubIssues issues)
{
    return issues->count;
}

/*
NPgithubIssueCount npGithubSetIssueCount(pNPgithubIssues issues, int set_count(void*), void* data)
{
    issues->count = set_count(data);
}
*/

/*
NPgithubErrValue npGithubIssueCountIncrement(pNPgithubIssues issues) {
    issues->count++;
}
*/

/*
NPgithubErrValue npGithubIssueCountDecrement(pNPgithubIssues issues) {
    issues->count--;
}
*/

/*
NPgithubErrValue npGithubIssueIndexIncrement(pNPgithubIssues issues) {
    issues->index++;
}
*/

/*
NPgithubErrValue npGithubIssueCount(pNPgithubIssues issues, Nothing action(int))
{
    
}
*/


int npGithubNewIssuesGlyph(pNPgithubIssues issues, void* dataRef)
{
    //issues->index = 0;
    //int index;
    
//    printf("\nnpGithubNewIssuesGlyph");
        issues->parent = npNodeNew(kNodePin, NULL, dataRef);
//    printf("25 parent : %p", issues->parent);
//    printf("26 issue count : %d", issues->count);

    for(issues->index = 0; issues->index < issues->count; issues->index++)
    {
        issues->current = issues->issue[issues->index];
        
        //issue = npGithubGetIssue(github, index);
        if(issues->current == NULL)
        {
            printf("\nISSUE IS NULL");
        }
            
        npGithubNewIssueGlyph(issues->current, dataRef);
    }
    
    return 0;
}

size_t write_avatar_image(void* ptr, size_t size, size_t nmemb, FILE *stream) {
    size_t written = fwrite(ptr, size, nmemb, stream);
    return written;
}

char* npCurlGetGithubAvatar(pNPgithubIssues issues, int index)
{
    CURL *curl_handle = NULL;
    CURLcode res;
    FILE *fp = NULL;
    static int id = 0;
    int my_index = 0;
    
    char* fileName = NULL;
    fileName = malloc(sizeof(char) * 10);
	if(fileName == NULL)
	{
		printf("2365 file name allocation failed, returning NULL");
		return NULL;
	}

    //sprintf(fileName, "%s%d.png", "avatar", id);
    sprintf(fileName, "%s.png", issues->issue[issues->index]->user.login);
    
    for(my_index = 0; my_index < index; my_index++)
    {
        if (strcmp(issues->issue[index]->user.login, issues->issue[my_index]->user.login ) == 0 ) {
           // printf("\navatar already exists, good job");
            //return fileName;
            return issues->issue[my_index]->user.avatar_image_file;
        }
    }
    
  //  printf("\n2311 fileName : %s", fileName);
    id++;
  //  char* avatar_url = "https://avatars.githubusercontent.com/u/1588558?v=4";
   // fp = fopen("avatar0001.png", "wb");
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
      curl_easy_setopt(curl_handle, CURLOPT_URL, issues->issue[index]->user.avatar_url);
      
	  curl_easy_setopt(curl_handle, CURLOPT_WRITEFUNCTION, write_avatar_image);
      curl_easy_setopt(curl_handle, CURLOPT_WRITEDATA, fp);
 //   curl_easy_setopt(curl_handle, CURLOPT_CONNECTTIMEOUT, 1);
 //   curl_easy_setopt(curl_handle, CURLOPT_HEADER, 0);
      curl_easy_setopt(curl_handle, CURLOPT_USERAGENT, "libcurl-agent/1.0");
    
      res = curl_easy_perform(curl_handle);
      if(res == CURLE_OK)
	  {
		  printf("2332 All Good 2332");
	  }
	  else
	  {
		printf("2333 Error : Something failed : %d", res);
	  }
	

	  printf(": cleanup curl handle ");
	  if(curl_handle != NULL)
	  {
		//curl_easy_cleanup(curl_handle);
	  }

	  printf("Closing fp ");
	  if(fp != NULL)
	  {
	    fclose(fp);
	  }
	  printf(": curl global cleanup \n");
//	  curl_global_cleanup();
//	curl_easy_cleanup(curl_handle);

//	fclose(fp);

//	free(fileName);
	}
	else
	{
		return NULL;
	}

    return fileName;
}


json_t* npCURLgetJSONgithubIssues(pNPgithubIssues issues)
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
    
    chunk[0].memory = malloc(1);  /* will be grown as needed by the realloc above */
    chunk[0].size = 0;    /* no data at this point */
 
    chunk[1].memory = malloc(1);  /* will be grown as needed by the realloc above */
    chunk[1].size = 0;    /* no data at this point */
    
 
    curl_global_init(CURL_GLOBAL_ALL);
    
    /* init the curl session */
    curl_handle = curl_easy_init();
    
    /* specify URL to get */ /// @todo pageinate this
//    curl_easy_setopt(curl_handle, CURLOPT_URL, "https://api.github.com/repos/openantz/antz/issues?state=all&page=2");
    for(index = 1; index <= 2; index++)
    {
     //   url_string[strlen(url_string)-1] = itoa(index);
        sprintf(url_string+(strlen(url_string)-1), "%d", index);
        curl_easy_setopt(curl_handle, CURLOPT_URL, url_string);
    
    /* send all data to this function  */
    curl_easy_setopt(curl_handle, CURLOPT_WRITEFUNCTION, WriteMemoryCallback);
    
    /* we pass our 'chunk' struct to the callback function */
    curl_easy_setopt(curl_handle, CURLOPT_WRITEDATA, (void *)&chunk[index-1]);
    
    /* some servers don't like requests that are made without a user-agent
     field, so we provide one */
    curl_easy_setopt(curl_handle, CURLOPT_USERAGENT, "libcurl-agent/1.0");
    
    /* get it! */
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

#define kNPmallocFailed -1
#define maxIssues 300

/*
enum {
    kNPgithubSetIssueUrl = 0,
    kNPgithubSetIssueId = 1,
    kNPgithubSetIssueTitle = 2,
    kNPgithubSetIssueState = 3,
    kNPgithubSetIssueCreatedAt = 4,
    kNPgithubSetIssueUpdatedAt = 5,
    kNPgithubSetIssueClosed = 6
};
*/
 
char* npJSONgetIssueUserLogin(json_t* issue_user)
{
    json_t* login;
    char* login_str;
    login = npJSONgetObject(issue_user, "login");
    login_str = (char*)json_string_value(login);
    printf("\nlogin_str : \n%s", login_str);
    
    return login_str;
}

/*
char* npJSONgetIssueUserAvatarImageFile(json_t* issue_user)
{
    
    
}
*/

char* npJSONgetIssueClosedAt(json_t* issue)
{
    json_t* closed_at;
    char* closed_at_str;
    closed_at = npJSONgetObject(issue, "closed_at");
    closed_at_str = (char*)json_string_value(closed_at);
    return closed_at_str;
}

char* npJSONgetIssueUserAvatarUrl(json_t* issue_user)
{
    json_t* avatar_url;
    char* avatar_url_str;
    avatar_url = npJSONgetObject(issue_user, "avatar_url");
    avatar_url_str = (char*)json_string_value(avatar_url);
    return avatar_url_str;
}

char* npJSONgetIssueUser(json_t *issue_user, void* dataRef)
{
    char* user_avatar_url = NULL;
    char* user_login = NULL;
    char* user_avatar_image_file = NULL;
    user_login = npJSONgetIssueUserLogin(issue_user);
    user_avatar_url = npJSONgetIssueUserAvatarUrl(issue_user);
  //  user_avatar_image_file = npJSONgetIssueUserAvatarImageFile(issue_user);
    
    
    
    return user_avatar_url;
}

void npGithubSetUser(pNPgithubIssues issues, pNPjson json, void* dataRef)
{
    
}

/// @todo Add to header
void npGithubSetIssueNumber(pNPgithubIssues issues, pNPjson json, void* dataRef)
{
	json_t *issue_number = NULL;
	int issue_number_val = 0;

	issue_number = npJSONgetObject(json->issue[json->issueIndex], "number");
	issue_number_val = json_integer_value(issue_number);
	printf("\n2391 issue number : %d", issue_number_val);
	issues->issue[issues->index]->index = issue_number_val;
}

void npGithubSetIssueUser(pNPgithubIssues issues, pNPjson json, void* dataRef)
{
    pNPgithubUser github_user = &issues->issue[issues->index]->user;
   // json_t *user = npJSONgetObject(json->root, "user");
    json_t *user = npJSONgetObject(json->issue[json->issueIndex], "user");
    json_t *avatar_url = npJSONgetObject(user, "avatar_url");
    
    issues->issue[issues->index]->user.login = npJSONgetIssueUserLogin(user);
    issues->issue[issues->index]->user.avatar_url = npJSONgetIssueUserAvatarUrl(user);
 //   issues->issue[issues->index]->user.avatar_image_file = npJSONgetIssueUserAvatarImageFile(user);
    
    issues->loadAvatarTextures = true;
    if( issues->loadAvatarTextures == true)
    {
        issues->issue[issues->index]->user.avatar_image_file = npCurlGetGithubAvatar(issues, issues->index);
    }
}

/*
void npGithubSetIssueUser(pNPgithubIssues issues, int index, void* (*gen_issue_user)(void*), void* issue)
{
    pNPgithubUser github_user = &issues->issue[index]->user;
    json_t *user = npJSONgetObject(issue, "user");
    json_t *avatar_url = npJSONgetObject(user, "avatar_url");
    
    issues->issue[index]->user.login = npJSONgetIssueUserLogin(user);
    issues->issue[index]->user.avatar_url = npJSONgetIssueUserAvatarUrl(user);
    issues->issue[index]->user.avatar_image_file = npCurlGetGithubAvatar(issues, index);
    
    return;
}
*/

/*
int new_npGithubIssuesInit(pNPgithubIssues issues, void* dataRef)
{
	
}
*/



int npGithubIssuesInit(pNPgithubIssues issues)
{
    char* recordTagVariableFormat[] = {"%s", "%s"};
    /// changed void* to void ,lde
    void (*issue_set_function[]) = {npGithubSetIssueUrl, npGithubSetIssueId, npGithubSetIssueTitle, npGithubSetIssueState,
        npGithubSetIssueCreatedAt, npGithubSetIssueUpdatedAt, npGithubSetIssueClosedAt, npGithubSetIssueUser, npGithubSetIssueNumber};
    issues->issueFunctions = npGithubInitFunctions(issue_set_function, 10);
    
    issues->count   = 0;
    issues->current = NULL;
    issues->index   = 0;
    issues->node    = NULL;

    issues->userCount = 0;
    issues->index = 0;
    
    issues->recordTagCount = 0;
    issues->recordTagIndex = 0;
    issues->index = 0;

    issues->recordTagVariableFormat[0] = recordTagVariableFormat[0];
    issues->recordTagVariableFormat[1] = recordTagVariableFormat[1];
    
    /*
    char* recordTagVariable[1] = {"title"};
    issues->recordTagVariable[0] = recordTagVariable[0];
    */
    
    for(issues->recordTagIndex = 0; issues->recordTagIndex < 300; issues->recordTagIndex++ ) {
        issues->recordTag[issues->recordTagIndex] = NULL;
    }
    
    while(issues->index < maxIssues)
    {
        
        issues->issue[issues->index] = malloc(sizeof(NPgithubIssue));
        issues->current = issues->issue[issues->index];
        if(issues->current == NULL)
        {
            printf("Malloc Failed");
            return kNPmallocFailed;
        }
        
        issues->current->assignee = NULL;
        issues->current->body     = NULL;
        issues->current->closed_at= NULL;
        issues->current->closed_by= NULL;
        issues->current->comments_url = NULL;
        issues->current->created_at   = NULL;
        issues->current->events_url   = NULL;
        issues->current->html_url     = NULL;
        issues->current->id           = 0;
        issues->current->index        = 0;
        issues->current->issue_node   = NULL;
        issues->current->labels_url   = NULL;
        issues->current->locked       = NULL;
        issues->current->milestone    = NULL;
        issues->current->num_comments = 0;
        issues->current->state        = NULL;
        issues->current->this_issue   = issues->current;
        issues->current->title        = NULL;
        issues->current->updated_at   = NULL;
        issues->current->url          = NULL;
      //  issues->current->user         = NULL;
        
        issues->index++;
    }
    
    return 0;
}


int new_npGithubIssueInit(pNPgithubIssues issues)
{
    char* recordTagVariableFormat[] = {"%s", "%s"};
    void* (*issue_set_function[]) = {(void*)npGithubSetIssueUrl, (void*)npGithubSetIssueId, (void*)npGithubSetIssueTitle, (void*)npGithubSetIssueState,
        (void*)npGithubSetIssueCreatedAt, (void*)npGithubSetIssueUpdatedAt, (void*)npGithubSetIssueClosedAt};
    issues->issueFunctions = npGithubInitFunctions(issue_set_function, 7);
    issues->count   = 0;
    issues->current = NULL;
    issues->index   = 0;
    issues->node    = NULL;
    issues->recordTagCount = 0;
    issues->recordTagIndex = 0;
    issues->index = 0;

    issues->recordTagVariableFormat[0] = recordTagVariableFormat[0];
    issues->recordTagVariableFormat[1] = recordTagVariableFormat[1];
    
 
 //    char* recordTagVariable[1] = {"title"};
 //    issues->recordTagVariable[0] = recordTagVariable[0];
 
    
    issues->recordTag[issues->recordTagIndex] = NULL;
    
    if(issues->index < maxIssues)
    {
        issues->issue[issues->index] = malloc(sizeof(NPgithubIssue));
        issues->current = issues->issue[issues->index];
        if(issues->current == NULL)
        {
            printf("Malloc Failed");
            return kNPmallocFailed;
        }
        
        issues->current->assignee = NULL;
        issues->current->body     = NULL;
        issues->current->closed_at= NULL;
        issues->current->closed_by= NULL;
        issues->current->comments_url = NULL;
        issues->current->created_at   = NULL;
        issues->current->events_url   = NULL;
        issues->current->html_url     = NULL;
        issues->current->id           = 0;
        issues->current->index        = 0;
        issues->current->issue_node   = NULL;
        issues->current->labels_url   = NULL;
        issues->current->locked       = NULL;
        issues->current->milestone    = NULL;
        issues->current->num_comments = 0;
        issues->current->state        = NULL;
        issues->current->this_issue   = issues->current;
        issues->current->title        = NULL;
        issues->current->updated_at   = NULL;
        issues->current->url          = NULL;
        //  issues->current->user         = NULL;
    }

    return 1;
}


/// Returns number of issues inited
int new_npGithubIssuesInit(pNPgithubIssues issues, int numIssuesToInit)
{
    int numIssuesInited = 0;
    
    while(numIssuesInited < numIssuesToInit)
    {
        new_npGithubIssueInit(issues);
        issues->index++;
        numIssuesInited++;
    }
    
    return numIssuesInited;
}




/// returns json_t* gitJSON->root
json_t* npGitJSONsetRoot(pNPjson gitJSON, void* (*get_json_root)(void* issues), pNPgithubIssues issues )
{
    return gitJSON->root = get_json_root((pNPgithubIssues)issues);
}

/*
pNPjsonIndex npGitJSONsetIndex(pNPjson gitJSON, int (*set_json_index)(void* issues), pNPgithubIssues issues )
{
    return gitJSON->index = set_json_index(issues); /// @todo check indexes before they're returned
}
*/

pNPjsonIndex npGitJSONsetIndex(pNPjson gitJSON, int (*set_json_index)(pNPjson json), pNPgithubIssues issues )
{
    /// gitJSON was issues
    return gitJSON->jsonCtrl.index = set_json_index(gitJSON); /// @todo check indexes before they're returned
}


int npJSONsetZeroIndex(pNPjson json)
{
    return 0;
}

size_t npGitJSONsetArraySize(pNPjson gitJSON, size_t (*npGet_JSON_arraySize)(pNPjson json), pNPgithubIssues issues)
{
    gitJSON->array_size = npGet_JSON_arraySize(gitJSON);
    
    return gitJSON->array_size;
}

void prot(pNPgithubIssues issues, void* dataRef)
{
	
}

void npGitJSONinit(pNPjson gitJSON, pNPgithubIssues issues, void* dataRef)
{
    int githubIssuesLeft = 0;
 //   gitJSON->array_size = npGitJSONsetArraySize(gitJSON, npJSONsetZeroIndex, issues);
    gitJSON->array_size = 0;
//    gitJSON->index = npGitJSONsetIndex(gitJSON, npJSONsetZeroIndex, issues);
    gitJSON->jsonCtrl.index = npGitJSONsetIndex(gitJSON, npJSONsetZeroIndex, issues);
    gitJSON->root  = npGitJSONsetRoot(gitJSON, npCURLgetJSONgithubIssues, issues);
	if(gitJSON->root == NULL)
	{
		printf("\nCurl calls failed");

	}

    //issues->count = 0;
	issues->count = (int)gitJSON->array_size;
	issues->index = 0;
    gitJSON->issueIndex = 0;
    do{/// npJSONgithubIssuesToAntzIssues not in header
        githubIssuesLeft = npJSONgithubIssuesToAntzIssues(gitJSON, issues, dataRef);
        issues->index++;
        gitJSON->issueIndex++;
        issues->count = ((int)gitJSON->array_size - githubIssuesLeft);
         //  printf("\n2301 githubIssues Left : %d", githubIssuesLeft);
    }
    while (githubIssuesLeft > 0);
    issues->index = 0;

}

void githubJSONgetIssueUserAvatarUrl(pNPgithubIssues issues, pNPjson json, void* dataRef)
{
    
    
}

pNPgithubIssues new_npGithubInit(void* dataRef)
{
    pData data = (pData) dataRef;
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
    npGithubIssuesInit(&data->io.issues);
    data->io.issues.running = false;
    npGithubCtrlSetCurrentIssueIndex(&data->io.issues, 0, dataRef);
    npGithubCtrlSetCurrentIssue(&data->io.issues, dataRef);
    npGithubCtrlSetRecordTagIndex(&data->io.issues, 0, dataRef);
    
    
    
    
    return &data->io.issues;
}

pNPjson new_npJSONinit(void* dataRef)
{
    pNPjson json = NULL;
    json = malloc(sizeof(NPjson));
    
    if(json == NULL)
        printf("\n2388Malloc Failed");
        
    return json;
}

void npGithubRun(void* dataRef)
{
    pData data = (pData) dataRef;
    pNPgithubIssues issues = &data->io.issues;
//    pNPgithubUser user = data->io.issues.user[data->io.issues.userIndex];
    bool getUsers = false;
    
    if(issues->running != false)
    {
       // new_getGithubProcessIssues(issues, 20, dataRef);
		new_gitVizTest(issues, dataRef);
		issues->running = false;
    }
    
    if( getUsers == true )
    {
        getGithubProcessUsers(issues, 1, dataRef);
    }
    
}

int getGithubProcessUser(pNPgithubIssues issues, void* dataRef)
{
    
    
    
    return 0;
}


int getGithubProcessUsers(pNPgithubIssues issues, int usersToProcess, void* dataRef)
{
    /*
    static int githubUsersProcessed = 0;
    int usersLeft = 0;
    int index = 0;
    */
     
    getGithubProcessUser(issues, dataRef);
    
    return 0;
}

int new_getGithubProcessIssues(pNPgithubIssues issues, int issuesToProcess, void* dataRef)
{
    static int antzIssuesProcessed = 0;
    int antzIssuesLeft = 0;
    /*
    npGithubCtrlSetCurrentIssueIndex(issues, 0, dataRef);
    npGithubCtrlSetCurrentIssue(issues, dataRef);
    npGithubCtrlSetRecordTagIndex(issues, 0, dataRef);
    */
    
    if(antzIssuesProcessed == issues->count)
    {
        printf("\n239393 All issues processed");
        issues->running = false;
        return 0;
    }
    

    do{
        npGithubCtrlInitRecordTag(issues, issues->index, dataRef);
		npGithubCtrlInitIssueTextTag(issues, issues->index, dataRef);
        //npGithubCtrlSetCurrentIssue(issues->issue[issues->index], dataRef);
        npGithubCtrlSetCurrentIssueIndex(issues, issues->index, dataRef);
        npGithubCtrlSetCurrentIssue(issues, dataRef);
        
        npGithubCtrlSetCurrentIssueUserAvatarImageFile(issues, dataRef);
        npGithubCtrlSetCurrentIssueUserAvatarImagePath(issues, dataRef);
        
        //  npGithubCtrlSetCurrentIssueUserAvatarImagePath(issues, dataRef);
        //npGithubCtrlSetUserTexture(issues, dataRef);
        npGithubCtrlLoadIssueUserAvatarTextureID(issues, dataRef);
        npGithubCtrlCountWordsInIssueTitle(issues->issue[issues->index], dataRef);
        
   //     antzIssuesLeft = npGithubAntzIssuesToAntzNodes(issues, dataRef);
		issues->index++;
        antzIssuesProcessed++;
    }
    while( (antzIssuesProcessed < issuesToProcess) && antzIssuesLeft > 0);
   // while (antzIssuesLeft > 0);
    
    
    
    return 0;
}

int getGithubIssues(void* dataRef)
{
    pData data = (pData) dataRef;
  //  NPgithubIssues issues = data->io.issues;
   // static NPgithubIssues issues;
    pNPgithubIssues issues = malloc(sizeof(NPgithubIssues));
    NPjson gitJSON;
    int issue_index = 0;
    int githubIssuesLeft = 0;
    int antzIssuesLeft = 0;
    
  //  data->io
    
    /// Type initialization functions
    issues->index = 0;

    npGithubIssuesInit(issues);
    npGitJSONinit(&gitJSON, issues, dataRef);

    /// Type conversion functions
    /// @todo lde Optional destructive capability
    issues->count = 0;
   // issues.count = (int)gitJSON.array_size;
    issues->index = 0;
    gitJSON.issueIndex = 0;
    do{
        githubIssuesLeft = npJSONgithubIssuesToAntzIssues(&gitJSON, issues, dataRef);
        issues->index++;
        gitJSON.issueIndex++;
        issues->count = ((int)gitJSON.array_size - githubIssuesLeft);
     //   printf("\n2301 githubIssues Left : %d", githubIssuesLeft);
    }
    while (githubIssuesLeft > 0);
    issues->index = 0;
    
    //issue->issueGeoType = kNPgeoTorus;
   // issues->index = 0;
   /// npGithubCtrlSetRecordTagIndex(issues.issue[0], 0, dataRef);
    //npGithubCtrlSetCurrentIssue(issues->issue[0], dataRef);
    npGithubCtrlSetCurrentIssueIndex(issues, 0, dataRef);
    npGithubCtrlSetCurrentIssue(issues, dataRef);
    npGithubCtrlSetRecordTagIndex(issues, 0, dataRef);
 //   npGithubCtrlSetUserTexture(issues, dataRef); /// temp
    do{
        npGithubCtrlInitRecordTag(issues, issues->index, dataRef);
        //npGithubCtrlSetCurrentIssue(issues->issue[issues->index], dataRef);
        npGithubCtrlSetCurrentIssueIndex(issues, issues->index, dataRef);
        npGithubCtrlSetCurrentIssue(issues, dataRef);
        
        npGithubCtrlSetCurrentIssueUserAvatarImageFile(issues, dataRef);
        npGithubCtrlSetCurrentIssueUserAvatarImagePath(issues, dataRef);
        
      //  npGithubCtrlSetCurrentIssueUserAvatarImagePath(issues, dataRef);
        //npGithubCtrlSetUserTexture(issues, dataRef);
        npGithubCtrlLoadIssueUserAvatarTextureID(issues, dataRef);
        npGithubCtrlCountWordsInIssueTitle(issues->issue[issues->index], dataRef);
        
        //  npGithubCtrlSetRecordTagVariableIndex(issues.issue[issues.index], issues.index);
      //  issues.issue[issues.index]->issueGeoType = kNPgeoTorus;
        antzIssuesLeft = npGithubAntzIssuesToAntzNodes(issues, dataRef);
        issues->index++;
      //  printf("\n2302 antzIssuesLeft Left : %d", antzIssuesLeft);
    }
    while (antzIssuesLeft > 1); // 1 was 0

//    npGithubAntzIssuesToAntzNodes(&issues, dataRef);

        
    json_decref(gitJSON.root);
   
    /// @todo cleanup github structure
    
    return 0;
}


int old(void) /// Can delete
{
    CURL *curl_handle;
    CURLcode res;
    
  //  char *url;
    const char* url_text;
    long long my_id = 0;
    json_t *root;
    json_t *data, *url, *commit, *message;
    json_t *id;
    json_t *number;
    json_error_t error;
    struct NPgithubIssue issue[50];
    int i = 0;
    int numOfIssues = 0;
    
    struct MemoryStruct chunk;
    
    chunk.memory = malloc(1);  /* will be grown as needed by the realloc above */
    chunk.size = 0;    /* no data at this point */
    
    curl_global_init(CURL_GLOBAL_ALL);
    
    /* init the curl session */
    curl_handle = curl_easy_init();
    
    /* specify URL to get */
    curl_easy_setopt(curl_handle, CURLOPT_URL, "https://api.github.com/repos/openantz/antz/issues");
    
    /* send all data to this function  */
    curl_easy_setopt(curl_handle, CURLOPT_WRITEFUNCTION, WriteMemoryCallback);
    
    /* we pass our 'chunk' struct to the callback function */
    curl_easy_setopt(curl_handle, CURLOPT_WRITEDATA, (void *)&chunk);
    
    /* some servers don't like requests that are made without a user-agent
     field, so we provide one */
    curl_easy_setopt(curl_handle, CURLOPT_USERAGENT, "libcurl-agent/1.0");
    
    /* get it! */
    res = curl_easy_perform(curl_handle);
    
    /* check for errors */
    if(res != CURLE_OK) {
        fprintf(stderr, "curl_easy_perform() failed: %s\n",
                curl_easy_strerror(res));
    }
    else {
        /*
         * Now, our chunk.memory points to a memory block that is chunk.size
         * bytes big and contains the remote file.
         *
         * Do something nice with it!
         */
        
     //   printf("\nstr start : %s\n str end", chunk.memory);
        printf("\n%lu bytes retrieved\n", (long)chunk.size);
        root = json_loads(chunk.memory, 0, &error);
        if(!json_is_array(root))
        {
            fprintf(stderr, "error: root is not an array\n");
            json_decref(root);
            return 1;
        }
        else
        {
            printf("\nroot is an array (size of %zu)", json_array_size(root));
            
            for(i = 0; i < json_array_size(root); i++)
            {
                data = json_array_get(root, i);
                
                if(!json_is_object(data))
                {
                    fprintf(stderr, "error: data %d is not an object", i + 1);
                    json_decref(root);
                    return 1;
                }
                else
                {
                    url = npJSONgetObject(data, "url");
                  //  url = json_object_get(data, "url");
          //          issue[i].url = json_string_value(url); /// temp commented
                    
                    id = npJSONgetObject(data, "id");
                    issue[i].id = json_integer_value(id);
                    
                    number = npJSONgetObject(data, "number");
           //         issue[i].number = json_integer_value(number); /// temp commented
                    
                    numOfIssues++;
                    
                }
                
            }
            
        }
        
    }
    
    /* print issues */
    for(i = 0; i < numOfIssues; i++)
    {
     //   printf("\nnumber %d || issue %d || id : %d || url : %s", issue[i].number ,i, issue[i].id, issue[i].url);
    }
    
    /* cleanup curl stuff */
    curl_easy_cleanup(curl_handle);
    
    free(chunk.memory);
    
    /* we're done with libcurl, so clean it up */ 
    curl_global_cleanup();
    printf("\n");
    return 0;
}

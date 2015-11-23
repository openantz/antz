/* -----------------------------------------------------------------------------
*
*  npgithub.h
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
#include "../../data/nptypes.h"
#include "npcurl.h"

void npGithubGetIssue(pNPjson github_json, pNPgithubIssues github_issues, int index, void* dataRef);
int npGithubGetIssueKeyIndex(pNPjsonObject json_issue, char* key, void* dataRef);
int npGithubGetIssueLabels(pNPjsonObject json_issue, pNPgithubIssue github_issue, void* dataRef);
int npGithubSearchForUser(pNPgithub github, char* user, void* dataRef);
void npGithubAddUserFromIssue(pNPgithub github, pNPjsonObject json_github_user_object, int issueIndex, void* dataRef);
void npGithubPrintUserData(pNPgithubUser user, void* dataRef);

void npGithubSetRepo(pNPgithub github, char* repo, void* dataRef);
int npGithubSendRequest(pNPgithub github, int request_index, void* dataRef);
void npGithubBuildRequest(pNPgithub github, int request_index, char* state, char* per_page, int page, void* dataRef);
char* npGithubReceiveResponse(pNPgithub github, int request_index, void* dataRef);

size_t npGithubWriteMemoryCallback(void* contents, size_t size, size_t nmemb, void* userp);
void npGithubIssuesInit(pNPgithubIssues issues, void* dataRef);
int npGithubInit(pNPgithub github, void* dataRef);
void npGithubRun(void* dataRef);
void npGithubGetIssues(void* dataRef);


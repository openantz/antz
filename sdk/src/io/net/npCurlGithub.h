
#include "npcurl.h"

void npCurlGetGithubIssues(pNPcurl curl, void* dataRef);

int npCURLgetGithubRepoIssuesNextPage(pNPcurl curl, pNPgithub github, void* dataRef);

void npCURLgetGithubRepoIssuesAll(pNPcurl curl, pNPgithub github, void* dataRef);

//void npGithubGetIssues(void* dataRef);

int npCURLsetGithubRepoIssuesPage(pNPcurl curl, pNPgithub github, int page, void* dataRef);

void npCURLsetGithubRepoName(pNPcurl curl, pNPgithub github, char* repo, void* dataRef);

void npCURLbuildGithubUrl(pNPcurl curl, pNPgithub github, void* dataRef);
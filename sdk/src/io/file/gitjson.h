#include "../net/npGithub.h"
#include "../../data/nptypes.h"

void npGitJSONinit(pNPjson gitJSON, pNPgithub github, void* dataRef);

void npJSONgetGithubIssueUrl(pNPjsonGithubIssue jsonIssue);

void npJSONgetIssue(pNPjsonGithubIssue jsonIssue);

void npJSONissuesGetGithubIssues(pNPjson json, pNPgithub github, void* dataRef);

void npJSONgetGithubIssueAssignee(pNPgithubIssues issues, pNPjsonGithubIssue jsonIssue, json_t* issue, int issueIndex, void* dataRef);

void npJSONgetGithubUser(pNPgithubIssues issues, pNPjsonGithubIssue jsonIssue, json_t* issue, int issueIndex, void* dataRef);

json_t* npJSONgetObject(json_t* data, char* key);
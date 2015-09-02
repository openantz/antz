#include "npgitviz.h"
#include "npCurlGithub.h"

/*
struct NPgithubRequest {
	char* state;
	int per_page;
	int page;
};
typedef struct NPgithubRequest NPgithubRequest;
typedef NPgithubRequest* pNPgithubRequest;
*/

/*
struct NPgithub {
	char* repo_name;
	pNPgithubIssues issues;
	pNPgithubUser user[20];
	pNPgithubRequest request;
};
typedef struct NPgithub NPgithub;
typedef NPgithub* pNPgithub;
*/

struct NPgithubIssueViz {
	int fnord;	
	pNPrecordTag recordTag;
	pNPgithubIssues partOf;
	pNPgithubIssue this_issue;
	pNPnode issue_creation_node;
	pNPnode issue_closed_node;
};
typedef struct NPgithubIssueViz NPgithubIssueViz;
typedef NPgithubIssueViz* pNPgithubIssueViz;

struct NPgithubIssue3 {
	NPgithubIssueViz githubIssueViz;
};
typedef struct NPgithubIssue3 NPgithubIssue3;
typedef NPgithubIssue3* pNPgithubIssue3;


void npGithubInit(pNPgithub github, void* dataRef);
void npGitVizIssue(pNPgithubIssues issues, int issueIndex, void* dataRef);
void new_npGithubGetIssueCreatedAt(pNPgithubIssue issue);
void npGithubRun(void* dataRef);
void npGithubGetIssues(void* dataRef);
//json_t* npJSONgetObject(json_t* data, char* key);
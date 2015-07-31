//
//  test.h
//  antz_osx_mysql-lde
//
//  Created by z on 7/1/15.
//
//

#ifndef antz_osx_mysql_lde_test_h
#define antz_osx_mysql_lde_test_h

//#include "../../data/nptypes.h"
#include "../../data/nptypes.h"

//#include "../../../libs/curl/include/curl/curl.h"
#include <curl/curl.h>
//#include <jansson.h>
/*
//typedef pNPnode pNPgithubNode;
typedef struct NPgithubIssue NPgithubIssue;
typedef NPgithubIssue* pNPgithubIssue;

//typedef NPgithubIssue* pNPgithubIssue;
typedef int NPgithubIssueIndex;
typedef int NPgithubIssueCount;
//typedef pNPgithubNode pNPgithubCurrentNode;

typedef char NPurl;
typedef NPurl* pNPurl;
//typedef int NPgithubIssueID;
typedef long long NPgithubIssueID;
typedef int NPgithubIssueNumber;
//typedef long long NPgithubIssueNumber;

typedef int NPgithubErrValue;
typedef void Nothing;
typedef pNPurl pNPissueUrl;
typedef pNPissueUrl pNPgithubIssueUrl;
//typedef pNPnode pNPgithubIssueNode;

struct NPgithubUser {
    char* login;
    char* id;
    char* avatar_url;
    char* avatar_image_file;
    char* avatar_image_file_path;
    int avatar_image_textureID;
    char* gravatar_id;
    char* url;
    char* html_url;
    char* followers_url;
    char* gists_url;
    char* starred_url;
    char* subscriptions_url;
    char* organizations_url;
    char* repos_url;
    char* events_url;
    char* received_events_url;
    char* type;
    bool site_admin;
};
typedef struct NPgithubUser NPgithubUser;
typedef struct NPgithubUser* pNPgithubUser;

struct NPgithubIssueTime {
    int hour;
    int minute;
    int second;
};
typedef struct NPgithubIssueTime NPgithubIssueTime;
typedef struct NPgithubIssueTime* pNPgithubIssueTime;

struct NPgithubIssueDate {
    int year;
    int month;
    int day;
};
typedef struct NPgithubIssueDate NPgithubIssueDate;
typedef struct NPgithubIssueDate* pNPgithubIssueDate;

struct NPgithubIssueDates {
    pNPgithubIssueDate date;
    pNPgithubIssueTime time;
};
typedef struct NPgithubIssueDates NPgithubIssueDates;
typedef struct NPgithubIssueDates* pNPgithubIssueDates;

typedef NPgithubIssueDates NPgithubIssueCreation;




typedef int pNPjsonIndex;

/// "raw" json

struct NPgithubIssueFunctions {
    char* variable[10];
    char* IssueFunctions[50][50];
    int*  functionUniqueId[50];
    void* (*npGithubIssueFunction[50])(void*);
    int* executionLock[50]; /// @todo Locks functions not to be called 1 == Locked && 0 == Unlocked
    int* loopStartIndex[50]; /// One for each
    int* loopEndIndex[50];
    int* currentPositionInLoop[50];
};
typedef struct NPgithubIssueFunctions NPgithubIssueFunctions;
typedef struct NPgithubIssueFunctions* pNPgithubIssueFunctions;



struct NPgithubIssues {
    pNPnode parent;
    NPgithubIssueIndex index;
    pNPgithubIssue current;
    pNPnode node;
    pNPgithubIssue issue[300]; /// Hardcoded at 300
    int recordTagVariableIndex;
    char* recordTagVariable[20];
    char* recordTagVariableFormat[20];
    int recordTagVariableFormatIndex;
    char* recordTagFormat[300];
    pNPrecordTag recordTag[300];
    int recordTagIndex;
    int recordTagCount;
    int issueVariableIndex;
    NPgithubIssueCount count;
    pNPgithubIssueFunctions issueFunctions;
    bool loadAvatarTextures;
    bool running;
};
typedef struct NPgithubIssues NPgithubIssues;
typedef struct NPgithubIssues* pNPgithubIssues;

struct NPgithubIssueCreatedAt {
    int year;
    int month;
    int day;
    int hour;
    int minute;
    int second;
};
typedef struct NPgithubIssueCreatedAt NPgithubIssueCreatedAt;
typedef NPgithubIssueCreatedAt* pNPgithubIssueCreatedAt;

struct NPgithubIssue {
    int recordId; /// for text tags
    pNPrecordTag recordTag;
    pNPgithubIssues partOf;
    pNPgithubIssue this_issue;
    pNPnode issue_node;
    pNPgithubIssueUrl url;
    char* labels_url;
    char* comments_url;
    char* events_url;
    char* html_url;
    NPgithubIssueID id;
    NPgithubIssueNumber index; /// This is also number
    char* title;
    int numOfWordsInIssueTitle;
    NPgithubUser user;
    char* state;
    bool locked;
    char* assignee;
    char* milestone; /// Is this char*?
    int num_comments;
    pNPgithubIssueDates dates;
    char* created_at;
    pNPgithubIssueCreatedAt issue_created_at;
    pNPgithubIssueCreatedAt issue_closed_at;
    char* updated_at;
    char* closed_at;
    char* body;
    char* closed_by;
    int issueNodeType; //kNPpin
    int issueGeoType;
    int issueTopoType;
};

struct NPjsonCtrl {
    int index;
};
typedef struct NPjsonCtrl NPjsonCtrl;
typedef struct NPjsonCtrl* pNPjsonCtrl;

struct NPjson {
    struct NPjsonCtrl jsonCtrl;
    json_t *root;
    json_t *issue[300];
    int issueIndex;
    int numOfIssues;
    size_t array_size;
};
typedef struct NPjson NPjson;
typedef struct NPjson* pNPjson;
*/

void gitVizTest(pNPgithubIssues issues, void* dataRef);
int npJSONgithubIssuesToAntzIssues(pNPjson gitJSON, pNPgithubIssues issues, void* dataRef);
int npGithubIssueCountIncrement(pNPgithubIssues issues);
int npGithubIssueCountDecrement(pNPgithubIssues issues);

void npGitJSONinit(pNPjson gitJSON, pNPgithubIssues issues, void* dataRef);
int getGithubIssues(void* dataRef);
json_t* npJSONgetObject(json_t* data, char* key);
char* npJSONgetGithubIssueUrl(json_t * data);
struct NPgithubIssue* npJSONgetGithubIssue(struct NPgithubIssues * github, json_t * data);
int npJSONgetGithubIssueId(json_t * data);
long long npJSONgetGithubIssueNumber(json_t * data);
size_t npJSONgetGithubIssuesArraySize(json_t *root, pNPgithubIssues issues);
char* npJSONgetGithubIssueTitle(json_t * issue);
//char* npGithubSetIssueTitle(pNPgithubIssues issues, int index, char* (*gen_issue_title)(void*), void* issue);
char* npGithubSetIssueTitle(pNPgithubIssues issues, pNPjson json, void* dataRef);
char* npJSONgetGithubIssueState(json_t * issue);
pNPgithubIssue new_npJSONsetGithubIssue(pNPgithubIssues issues, int github_issue_index, json_t * issue);
char* npJSONgetGithubIssueCreatedAt(json_t * issue);
char* npJSONgetGithubIssueUpdatedAt(json_t * issue);
pNPnode npGithubNewIssueGlyph(pNPgithubIssue issue, void* dataRef);
char* npJSONgetGithubIssueClosedAt(json_t * issue);
char* npJSONgetGithubIssueUser(json_t * issue);
char* npJSONgetGithubIssueUserLogin(json_t * issue_user);
//void npGithubSetIssueUser(pNPgithubIssues issues, int index, void* (*gen_issue_user)(void*), void* issue);
void npGithubSetIssueUser(pNPgithubIssues issues, pNPjson json, void* dataRef);
void npGithubCtrlSetRecordTagVariableIndexTo(pNPgithubIssue issue, int index);
void npGithubCtrlAssignIssueTitleToRecordTagTitle(pNPgithubIssue issue, pNPrecordTag issueTitle_tag, void* dataRef);
int npGithubCtrlGetRecordTagVariableIndex(pNPgithubIssue issue);
pNPnode new_npGithubNewIssueGlyph(pNPgithubIssue issue, void* dataRef);
void npGithubCtrlSetRecordTag(pNPgithubIssue issue, int index, void* dataRef);
void new_new2_npJSONsetGithubIssue(pNPgithubIssues issues, pNPgithubIssue issue, pNPjson json, void* dataRef);
void npGithubCtrlSetCurrentIssueUserAvatarImagePath(pNPgithubIssues issues, void* dataRef);
void npGithubCtrlSetCurrentIssueUserAvatarImageFile(pNPgithubIssues issues, void* dataRef);
int new_getGithubProcessIssues(pNPgithubIssues issues, int issuesToProcess, void* dataRef);
int getGithubProcessUsers(pNPgithubIssues issues, int usersToProcess, void* dataRef);
json_t* npGitJSONsetRoot(pNPjson gitJSON, void* (*get_json_root)(void *issues), pNPgithubIssues issues );
void* new_npJSONgetGithubIssueFunction(pNPgithubIssues issues, pNPjson json ,int functionIndex, void* dataRef);
#endif

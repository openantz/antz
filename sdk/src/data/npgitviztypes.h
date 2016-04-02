/* -----------------------------------------------------------------------------
*
*  npgitviztypes.h
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

#ifndef NPGITVIZTYPES_H_
#define NPGITVIZTYPES_H_

#include "stdbool.h"
#include "jansson.h"
#include "curl.h"


typedef struct NPgithubIssue NPgithubIssue;
typedef NPgithubIssue* pNPgithubIssue;

//typedef NPgithubIssue* pNPgithubIssue;
typedef int NPgithubIssueIndex;
typedef int NPgithubIssueCount;
//typedef pNPgithubNode pNPgithubCurrentNode;

typedef char NPurl;
typedef NPurl* pNPurl;

typedef long long NPgithubIssueID;
typedef int NPgithubIssueNumber;

typedef int NPgithubErrValue;
typedef void Nothing;
typedef pNPurl pNPissueUrl;
typedef pNPissueUrl pNPgithubIssueUrl;
//typedef pNPnode pNPgithubIssueNode;

struct NPgithubUser {
    char* login;
    int id;
	int number;
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

struct NPgithubIssueLabel {
	char* url;
	char* name;
	char* color;
};
typedef struct NPgithubIssueLabel NPgithubIssueLabel;
typedef NPgithubIssueLabel* pNPgithubIssueLabel;

struct NPgithubIssue {
	NPgithubIssueLabel label[4];
	int num_of_labels;
    int recordId;
    void* recordTag; //zz pNPrecordTag recordTag;
//    pNPgithubIssues partOf; /// temporarily commented out
    pNPgithubIssue this_issue;
    void* issue_node; //zz pNPnode
	void* issue_creation_node; //zz pNPnode /// @todo new
	void* issue_closed_node;  //zz pNPnode  /// @todo new
    pNPgithubIssueUrl url;
    char* labels_url;
    char* comments_url;
    char* events_url;
    char* html_url;
//    NPgithubIssueID id;
	int id;
    NPgithubIssueNumber index;
    char* title;
	void* titleTag; //pNPtag
    int numOfWordsInIssueTitle;
    pNPgithubUser user;	
	pNPgithubUser assignee;
	int userId;
	int number;
    char* state;
    bool locked;
//    char* assignee;
    char* milestone; 
    int num_comments;
    pNPgithubIssueDates dates;
    char* created_at;
    NPgithubIssueCreatedAt issue_created_at;
    pNPgithubIssueCreatedAt issue_closed_at;
 //   pNPgithubIssueCreatedAt issue_created_at;
 //   pNPgithubIssueCreatedAt issue_closed_at;
    char* updated_at;
    char* closed_at;
    char* body;
    char* closed_by;
    int issueNodeType; //kNodePin
    int issueGeoType;
    int issueTopoType;
};
typedef struct NPgithubIssue NPgithubIssue;
typedef NPgithubIssue* pNPgithubIssue;

#define kNPgithubMaxIssues 2000

struct NPgithubIssues {
    void* parent; //zz pNPnode
    NPgithubIssueIndex index;
    pNPgithubIssue current;
    void* node; //zz pNPnode
    NPgithubIssue issue[kNPgithubMaxIssues];
//    NPgithubUser user[10];    
//	int num_of_users;
//    int userIndex;
//    int userCount;
    int recordTagVariableIndex;
    char* recordTagVariable[20];
    char* recordTagVariableFormat[20];
    int recordTagVariableFormatIndex;
    char* recordTagFormat[300];
    void* recordTag[300]; //zz pNPrecordTag
    int recordTagIndex;
    int recordTagCount;
    int issueVariableIndex;
    NPgithubIssueCount count;
    pNPgithubIssueFunctions issueFunctions;
    bool loadAvatarTextures;
    bool running;
};
typedef struct NPgithubIssues NPgithubIssues;
typedef NPgithubIssues* pNPgithubIssues;


struct NPgithubRequest {
	char url[100];
	int urlSize;
	char* state;
	int per_page;
	int page;
};
typedef struct NPgithubRequest NPgithubRequest;
typedef NPgithubRequest* pNPgithubRequest;


struct NPjsonCtrl {
    int index;
};
typedef struct NPjsonCtrl NPjsonCtrl;
typedef struct NPjsonCtrl* pNPjsonCtrl;



/*
struct NPjsonGithubIssue {
	json_t *url;
	json_t *title;
	json_t *id;
	json_t *number;
	json_t *state;
	json_t *created_at;
	json_t *updated_at;
	json_t *closed_at;
};
typedef struct NPjsonGithubIssue NPjsonGithubIssue;
typedef NPjsonGithubIssue* pNPjsonGithubIssue; 
*/

//zz move this to npjson.c and translate to standard C or np types

/// @todo: implement selectors
struct NPjsonKeyList {
	char key[30]; 
	json_t* value;
	int index;
};
typedef struct NPjsonKeyList NPjsonKeyList;
typedef NPjsonKeyList* pNPjsonKeyList;

struct NPjsonKey {
	char* key;
//	char key[30];
};
typedef struct NPjsonKey NPjsonKey;
typedef NPjsonKey pNPjsonKey;

/*
type (enum)
----
kNPjsonString
kNPjsonNumber
kNPjsonObject
kNPjsonArray
kNPjsonTrue
kNPjsonFalse
kNPjsonNull
*/
struct NPjsonValue {
	int j_type; /// result of json_t* j_value;
	json_t* j_value; 
	int c_type; /// could be: pNPjsonObject, pNPjsonArray
	void* c_value;
};
typedef struct NPjsonValue NPjsonValue;
typedef NPjsonValue* pNPjsonValue;

#define kNPjsonMaxKeyValuePairs 30

/// An object is an unordered set of name/value pairs
struct NPjsonObject {
	void* parent; /// can be pNPjsonArray, pNPjsonObject, if null then no parent
	int parentType;
	int numNameValuePairs;	
	NPjsonKey jsonKey[kNPjsonMaxKeyValuePairs];
	NPjsonValue jsonValue[kNPjsonMaxKeyValuePairs];
};
typedef struct NPjsonObject NPjsonObject;
typedef NPjsonObject* pNPjsonObject;

//#define kNPjsonArrayMaxElements 200
#define kNPjsonArrayMaxElements 1500

struct NPjsonArray {
	void* parent; /// can be pNPjsonArray, pNPjsonObject, if null then no parent
	int numElements;
	pNPjsonValue element[kNPjsonArrayMaxElements];
};
typedef struct NPjsonArray NPjsonArray;
typedef NPjsonArray* pNPjsonArray;

struct NPjsonInteger {
	int j_int;
//	json_int_t j_int;
};
typedef struct NPjsonInteger NPjsonInteger;
typedef NPjsonInteger* pNPjsonInteger;

struct NPjsonReal {
	double j_real;
};
typedef struct NPjsonReal NPjsonReal;
typedef NPjsonReal* pNPjsonReal;

struct NPjsonBoolean {
	bool j_bool; 
};
typedef struct NPjsonBoolean NPjsonBoolean;
typedef NPjsonBoolean* pNPjsonBoolean;

struct NPjsonRoot {
	json_t* root;
	int type; /// can be either kNPjsonArray, or kNPjsonObject
	void* jsonRoot;
};
typedef struct NPjsonRoot NPjsonRoot;
typedef NPjsonRoot* pNPjsonRoot;

struct NPjsonSchema {
	json_t* s_root;
	NPjsonRoot schemaRoot;
};
typedef struct NPjsonSchema NPjsonSchema;
typedef struct NPjsonSchema* pNPjsonSchema;


#define kNPjsonInputMax 30

struct NPjson {
	char* input[kNPjsonInputMax];
	char* input_current; /// Tracks
	int   input_index;
	json_t* root;
	NPjsonRoot jRoot;
	NPjsonValue all_values;
	pNPjsonValue latest;
//	int error;
	json_error_t error; 
};
typedef struct NPjson NPjson;
typedef NPjson* pNPjson;

struct NPgithubJSONuser {
	json_t* login;
	json_t* id;
	json_t* avatar_url;
	json_t* gravatar_id;
	json_t* url;
	json_t* number;
	json_t* html_url;
	json_t* followers_url;
	json_t* following_url;
	json_t* gists_url;
	json_t* starred_url;
	json_t* subscriptions_url;
	json_t* organizations_url;
	json_t* repos_url;
	json_t* events_url;
	json_t* received_events_url;
	json_t* type;
	json_t* site_admin;
};
typedef struct NPgithubJSONuser NPgithubJSONuser;
typedef NPgithubJSONuser* pNPgithubJSONuser;

struct NPjsonGithubIssueLabel {
	json_t* url;
	json_t* name;
	json_t* color;
};
typedef struct NPjsonGithubIssueLabel NPjsonGithubIssueLabel;
typedef NPjsonGithubIssueLabel* pNPjsonGithubIssueLabel;

//struct NPgithubJSONissue {
struct NPjsonGithubIssue {
	json_t* temp_label;
	json_t* json_labels;
	NPjsonGithubIssueLabel labels[5];
	int num_of_labels;
	json_t* url; 
	json_t* labels_url; 
	json_t* comments_url; 
	json_t* events_url; 
	json_t* html_url; 
	json_t* id; 
	json_t* number; 
	json_t* title; 

	json_t* state; 
	json_t* locked; 
	
	json_t* milestone; 
	json_t* comments; 
	json_t* created_at; 
	json_t* updated_at; 
	json_t* closed_at; 
	json_t* body;

	json_t* temp_user;
	NPgithubJSONuser user;

	json_t* temp_assignee;
	pNPgithubJSONuser assignee;
};
typedef struct NPjsonGithubIssue NPjsonGithubIssue;
typedef NPjsonGithubIssue* pNPjsonGithubIssue;


struct NPjsonGithub {
	pNPjsonGithubIssue issues[200];
	int numOfIssues;
};
typedef struct NPjsonGithub NPjsonGithub;
typedef NPjsonGithub* pNPjsonGithub;

struct MemoryStruct2 {
    char *memory;
    size_t size;
};
typedef struct MemoryStruct2 MemoryStruct2;
typedef MemoryStruct2* pMemoryStruct2;


struct NPcurlFuncSet{
	int			id;						///< the function set ID
	
	char		hostType[256];	///< content type, antz or 3rd party
	void*		libHandle;					///< library handle
	
	/// curl uses cdecl as their calling convention
	/// mysql uses stdcall
	CURLcode (__cdecl *global_init)		(long flags);
	void* (__cdecl *easy_init)		();
	int   (__cdecl *easy_perform)		();
	char* (__cdecl *easy_strerror)    ();
	CURLcode (__cdecl *easy_setopt)   ();
	

	///< error and errno use 'db_' prefix to prevent name conflict
	
	int   (*InitConnOptions)		();
	void* (*GetTableFields)			();	///creates the table fields descrisptor
	void* (*getNodeTableFields)  ();
	
	void* (*StatementInsert)		();
	void* (*StatementCreate)		();
	void* (*StatementCreateTable)	();
	void* (*StatementCreateNodeTable) ();
	void* (*StatementUse)			();
	void* (*StatementShow)			();
	void* (*StatementDrop)			();
	void* (*StatementSelect)		();
	void* (*StatementTruncate)		();
	void* (*StatementUpdate)		();
	void* (*StatementDBshow)		();
	void* (*StatementDatabases)     ();
	void* (*getFuncsFromHost)    ();
	
	int   (*showDatabases)          (); // I might pass this a fcn ptr and change it to (*show), lde @todo
	char* (*getTableFields)			();
	
	int size;
};
typedef struct NPcurlFuncSet NPcurlFuncSet;
typedef struct NPcurlFuncSet *pNPcurlFuncSet;


struct NPcurl
{
	CURL* curl_handle;
	char url[100];
	char* urlPtr;
	int urlSize;
	int numArgs;
	int funcSetCount;
	const char* errorStr;
	int globalInitFlag;
	int easySetOptFlag;
	CURLcode res;
	MemoryStruct2 mem[10];
	int numMem;
	int memIndex;
	//pNPcurlFuncSet	funcSetList[kNPdbFuncSetMax]; ///< host type specific
	pNPcurlFuncSet funcSetList; ///< host type specific
};
typedef struct NPcurl NPcurl;
typedef NPcurl* pNPcurl;

struct NPgithubResponse {
	int fnord;	
	void* response;
};
typedef struct NPgithubResponse NPgithubResponse;
typedef NPgithubResponse* pNPgithubResponse;

struct NPgithubRequestResponse {
	char requestUrl[100];
	int urlSize;
	char* state;
	char* per_page;
	int page;
	char* response;
	int responseSize;
};
typedef struct NPgithubRequestResponse NPgithubRequestResponse;
typedef NPgithubRequestResponse* pNPgithubRequestResponse;

#define kNPgithubMaxUsers 1000
struct NPgithub {
	char* repo_name;
	pNPgithubIssues issues;
	NPgithubUser user[kNPgithubMaxUsers];
	int num_of_users;
	int err;
//	pNPgithubRequest request;
	NPgithubRequestResponse rR[50];
	int requestResponse_index;
	int num_of_RequestResponses;
	pNPjson jsonGithub;
};
typedef struct NPgithub NPgithub;
typedef NPgithub* pNPgithub;


#endif


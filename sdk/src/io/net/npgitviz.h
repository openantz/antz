/* -----------------------------------------------------------------------------
*
*  npgitviz.h
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

/* temp commented out @todo
struct NPgithubUser2 {
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
//    bool site_admin;
};
typedef struct NPgithubUser2 NPgithubUser2;
typedef struct NPgithubUser2* pNPgithubUser2;
*/

/* temp commented out @todo
struct NPgithubIssueCreatedAt2 {
    int year;
    int month;
    int day;
    int hour;
    int minute;
    int second;
};
typedef struct NPgithubIssueCreatedAt2 NPgithubIssueCreatedAt2;
typedef NPgithubIssueCreatedAt2* pNPgithubIssueCreatedAt2;
*/

/// temp commenting out pNPgithub partOf
/// temp commenting out pNPgithubIssue this_issue
/// temp commenting out pNPnode issue_node
/*
struct NPgithubIssue2 {
    int recordId; /// for text tags
   // pNPrecordTag recordTag;
//    pNPgithub partOf;
//    pNPgithubIssue this_issue;
//    pNPnode issue_node;
    char* url;
    char* labels_url;
    char* comments_url;
    char* events_url;
    char* html_url;
    int id; /// @todo change id to issueID
    int index; /// github issue number /// @todo change index to issueNumber 
    char* title;
    int numOfWordsInIssueTitle;
    char* state;
  //  bool locked;
    char* assignee;
    char* milestone; /// Is this char*?
    int num_comments;
//    pNPgithubIssueDates dates; /// @todo not necessary, remove
    char* created_at;
    pNPgithubIssueCreatedAt2 issue_created_at;
    pNPgithubIssueCreatedAt2 issue_closed_at;
    char* updated_at;
    char* closed_at;
    char* body;
    char* closed_by;
    int issueNodeType; //kNPpin
    int issueGeoType; /// @todo issueCreationNodeGeoType issueClosedNodeGeoType
    int issueTopoType; /// @todo issueCreationNodeTopoType issueClosedNodeTopoType
};
typedef struct NPgithubIssue2 NPgithubIssue2;
typedef struct NPgithubIssue2* pNPgithubIssue2; 
*/

/// temporarily commenting out all pNPnode
/*
struct NPgithub {
//    pNPnode parent;
    int index;
    pNPgithubIssue current;
//    pNPnode node;
    pNPgithubIssue issue[300]; /// Hardcoded at 300
    NPgithubUser2 user[5]; /// @todo put this in NPgithub

    int recordTagVariableIndex;
    char* recordTagVariable[20];
    char* recordTagVariableFormat[20];
    int recordTagVariableFormatIndex;
    char* recordTagFormat[300];
    pNPrecordTag recordTag[300];
    int recordTagIndex;
    int recordTagCount;

    int issueVariableIndex;
    int count; /// @todo change to issueCount
//    pNPgithubIssueFunctions issueFunctions; /// temp commented out
//    bool loadAvatarTextures; /// temp commented out
//    bool running;			/// temp commented out
};
typedef struct NPgithub NPgithub;
typedef struct NPgithub* pNPgithub;
*/

void theNew_npGitViz(pNPgithub github, void* dataRef);
void theNew_npGitVizIssue2(pNPgithub github, int issueIndex, void* dataRef);
void new_npGithubGetIssueCreatedAt2(pNPgithubIssue issue);
void test_npGithubGetIssueCreatedAt(pNPgithubIssue issue, void* dataRef);


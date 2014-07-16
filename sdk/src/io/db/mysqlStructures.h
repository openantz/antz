

enum
{
	mysqlCREATE = 0,
	mysqlDROP,
	mysqlINSERT,			
	mysqlDELETE,			
	mysqlUPDATE,			
	mysqlSELECT,
	mysqlUSE,
	mysqlALTER,
	mysqlSHOW
};

enum {
 DATABASE = 1,
 TABLE = 2,
 QUERY = 3
};

struct dbResults {
	int numOfDbResults;
	struct dbResult *result;
};

// Type:  MYSQL_RESULT == 0, ORACLE_RESULT == 1
struct dbResult {
	int type;
	void* resultObj;
};

struct mysqlResult {
	int test;
	void* result;
};

struct databaseObject {
	int type;
	void* dbObject;
};

struct dbQueries {
	int numOfQueries;
	struct dbQuery* query;
};

struct dbQuery {
	int type;
	void* queryObject;
};

struct mysqlObject {
	int type;
	void* myobject; // Can be typecasted to either mysqlDatabase or mysqlTable or mysqlQuery
};

/*
struct query {
	int command;
	int object;
	int tableType;
	char* subject;
	char* fields; 
	char** values; 
	struct fieldvalue *fv;
	char** query;
	char **newValues;
	int numOfValues;
};
*/

struct dbConnectMysql {
	char*	host;
	char*	user;
	char*	pass;
	char*	db;
	unsigned int port;
	char*	unix_socket;
	unsigned long client_flag;
	void *conn;
	struct query *q;
};


struct dbNewConnect {
	pNPdatabases dbList; // hack, debug db //zz-db //This really shouldn't be here, debug db
	int dbType;
	struct dbFunctions *dbFuncs;
	void* dbConnection;
};

struct dbConnect {
	char*	host;
	char*	user;
	char*	pass;
	char*	db;
	unsigned int port;
	char*	unix_socket;
	unsigned long client_flag;
	void *conn;
	struct query *q;
};

//It doesn't make sense to have this named csvNode, csvStrContainer would be more appropriate, debug dbChunk
/* I'm using this for more than Nodes (it has a pretty general function....this seems boxy)
	-Other uses
		- channels
		- chmaps
		- text tags

	-New structure:
	struct csvStrObj 
	{
		int size;
		char *csvStr;
	};	

*/
/*
struct csvStrObj
{
	int size;
	char *csvStr;
};
*/

struct csvNode 
{
	int size;
	char *csvStr;
};

/*	 With the renaming of csvNode to csvStrObj, the use of the csvNodes name no longer makes much sense
	  - From now on it will be csvStrObjects

		struct csvStrObjects
		{
			int numOfcsvStrObjects;
			int totalCsvStrObjectsSize;
			struct csvStrObj *csvObj;
		};
*/

/*
struct csvStrObjects
{
	int numOfcsvStrObjects;
	int totalCsvStrObjectsSize;
	struct csvStrObj *csvObj;
};
*/

struct csvNodes
{
	int numOfNodes;
	int totalCsvSize;
	struct csvNode *node;
};

/*
struct chunkObj {
	int numOfStrings;
	char **str;
};
*/

/*
struct newChunkObj {
	int chunkSize;
	struct csvStrObjects *csvObjects;
};
*/

/*
struct chunkObj {
	int chunkSize;
	struct csvNodes *nodes;
};
*/

/*
struct newChunksObj {
	int numOfChunks;
	struct newChunkObj *chunk;
};
*/


struct chunksObj {
	int numOfChunks;
	struct chunkObj *chunk;
};


//newChunksObj
struct newMysqlQuery {
	int command;
	int object;
	int tableType;
	int conditionalType;
	char* subject;
	char* fields; 
	//char** values; 
	struct newChunksObj *values;
	struct fieldvalue *fv;
//	char conditionals[30000];
	char query[3000][80000];
//	int numberOfQueries;
	char **newValues;
	int numOfValues;
	struct mysqlObject* focus;
//	MYSQL_RES *result[300]; // This will be typecasted from a void* to a MYSQL_RES
	void *result[300];
	int numOfResults;
};


// Could make all these mysql Structures more generic, as in arrays of void*'s
struct mysqlQuery {
	int command;
	int object;
	int tableType;
	int conditionalType;
	int numOfResults;
	char* subject;
	char* fields; 
	//char** values; 
	struct chunksObj *values;
	struct fieldvalue *fv;
//	char conditionals[30000];
	char query[3000][80000];
//	int numberOfQueries;
	char **newValues;
	int numOfValues;
	struct mysqlObject* focus;
//	MYSQL_RES *result[300]; // This will be typecasted from a void* to a MYSQL_RES
	void *result[300];
//	int numOfResults;
};
 
struct mysqlTable {
	char* name;
	char* fields;
	//char** values;
	struct chunksObj *values;
	int numOfValues; //???
	int numOfChunks;
	char* formattedValues;
	int tableType;
	int fieldsSize;
	int valuesSize;
	int formattedValuesSize;	
};

struct mysqlDatabase {
	char* name;
};
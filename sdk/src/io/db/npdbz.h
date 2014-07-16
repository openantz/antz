#include "../../npdata.h"

#include <my_global.h>
#include <mysql.h>
//#include "../../npdata.h

struct newChunkObj {
	int chunkSize;
	struct csvStrObjects *csvObjects;
};

struct newChunksObj {
	int numOfChunks;
	struct newChunkObj *chunk;
};

struct csvStrObj
{
	int size;
	char *csvStr;
};

struct csvStrObjects
{
	int numOfcsvStrObjects;
	int totalCsvStrObjectsSize;
	struct csvStrObj *csvObj;
};

struct dbFunctions {
	void* (*connect)(void*);
};

//Init, Connect, and Query are elemental functions
struct dbFunction {
	void* (__stdcall *init)   ();
	void* (__stdcall *connect)();
	void* (__stdcall *query)  ();
	void* (__stdcall *use)    ();
	void* (__stdcall *select) ();
	void* (__stdcall *alter)  ();
	void* (__stdcall *insert) ();
	void* (__stdcall *db_fetch_row) ();
	void* (__stdcall *db_fetch_lengths) ();
	void* (__stdcall *db_num_fields) ();
	void* (__stdcall *db_num_rows) ();
	void* (*createInsertStatement)  ();
	void* (*createStatement)  ();
	void* (*createTableStatement) ();
	void* (*useStatement)     ();
	void* (*showStatement)    ();
	void* (*dropStatement)    ();
	void* (*createSelectStatement) ();
	void* (__stdcall *show)   ();
	void* (__stdcall *storeResult) ();
	void* (__stdcall *close)  ();
};

// Perhaps a sub-structure inside database which deals with database specific stuff
struct database {
	char dbType[kNPurlMax];   // "mysql" "oracle" "postgresql", etc...
	int connid;     // Might be better for this to be void*
	int port;       // Default for MySQL is 3306

	char hostIP[kNPurlMax];   // This could be static
	char user[kNPurlMax];     // root
	char password[kNPurlMax]; // admin 
	char currentlyUsedDatabase[kNPurlMax];

	struct dbFunction *db;
};

struct databases //This should be renamed as struct server ...or not
{
	pNPdatabases dbList;
	int numberOfDatabases;
	struct database *myDatabase;
};

/*
struct query{
	int placeholder;
};
*/

struct tableParameters {
	void* conn;	
	int action; //Create, Insert, Select, Alter, Show, etc...
};


int dbHook(struct dbFunction *db, char* filePath, int dbtype);
//int npConnectToDatabaseServer(struct dbNewConnect *connect, void* dataRef);
void main2(void* dataRef);

int npAddDb(struct databases *dbs, char* dbType, char* hostIP, char* user, char* pass, char* dbname, void* dataRef);
int npUseDatabase2(int connid, struct dbFunction *db, char* dbname);
int npSelect(int connid, struct dbFunction *db, char* tblname); //Add field(s) choice later
void npNewFreeChunks(struct newChunksObj * chunks, void* dataRef);
char* npMysqlInsertStatement(char* tblname, struct newChunkObj *value);
int npdbLoadNodeTbl(int menuItem, void* dataRef);
void npdbSaveAntzStateToDatabase(void* dataRef);
int npOpenDb(struct database *db);
int npAttachDbsToDataRef(struct databases *dbs, void* dataRef);
/* -----------------------------------------------------------------------------
*
*  npdbz.h
*
*  ANTz - realtime 3D data visualization tools for the real-world, based on NPE.
*
*  ANTz is hosted at http://openantz.com and NPE at http://neuralphysics.org
*
*  Written in 2010-2014 by Shane Saxon - saxon@openantz.com
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
*  with this software (license file named COPYING.txt). If not, see
*  http://creativecommons.org/publicdomain/zero/1.0/
*
* --------------------------------------------------------------------------- */

#ifndef NPDBZ_H_
#define NPDBZ_H_

#include "../../npdata.h"

//#include <my_global.h>
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
	void* (*init)   ();
	void* (*connect)();
	void* (*query)  ();
	void* (*use)    ();
	void* (*select) ();
	void* (*alter)  ();
	void* (*insert) ();
	void* (*db_fetch_row) ();
	void* (*db_fetch_lengths) ();
	void* (*db_num_fields) ();
	void* (*db_num_rows) ();
	void* (*db_error) ();
	void* (*db_errno) ();
	void* (*StatementInsert)  ();
	void* (*StatementCreate)  ();
	void* (*StatementCreateTable) ();
	void* (*StatementUse)     ();
	void* (*StatementShow)    ();
	void* (*StatementDrop)    ();
	void* (*StatementSelect) ();
	void* (*show)   ();
	void* (*storeResult) ();
	void* (*freeResult) ();
	void* (*close)  ();
};

struct database {
	char dbType[kNPurlMax];   // "mysql" "oracle" "postgresql", etc...
	//int id;     // Might be better for this to be void*
	void* id;
	int port;       // Default for MySQL is 3306

	char hostIP[kNPurlMax];   // This could be static
	char user[kNPurlMax];     // root
	char password[kNPurlMax]; // admin 
	char currentlyUsedDatabase[kNPurlMax];
	int* idMap;
	//int idMap[kNPnodeMax];

	struct dbFunction *dbFunc;
};

/*
struct databases //This should be renamed as struct server ...or not
{
	pNPdatabases dbList;	///< @todo make this pNPdatabase* dbList
	int numberOfDatabases;
	struct database *activeDB;	///< @todo make this single DB ptr to dbList or dbID index
};
*/

/*
struct query{
	int placeholder;
};
*/

struct tableParameters {
	void* conn;	
	int action; //Create, Insert, Select, Alter, Show, etc...
};

void updateNodeFromMysqlRow (MYSQL_ROW *row, void* dataRef); // Generalize here

int dbHook(struct dbFunction *db, char* filePath, int dbtype);
//int npConnectToDatabaseServer(struct dbNewConnect *connect, void* dataRef);
void main2(void* dataRef);

//int npAddDb(struct databases *dbs, char* dbType, char* hostIP, char* user, char* pass, char* dbName, void* dataRef); //zzd r  // old, lde
int npAddDb(pNPdbs dbs, char* dbType, char* hostIP, char* user, char* pass, char* dbName, void* dataRef);
int npUseDatabase2(void* conn, pNPdbFuncSet func, char* dbName);
int npSelect(void* conn, pNPdbFuncSet func, char* table); //Add field(s) choice later
void npNewFreeChunks(struct newChunksObj * chunks, void* dataRef);

int npOpenDb(struct database *db);
// int npAttachDbsToDataRef(struct databases *dbs, void* dataRef); // old, lde

int npdbAddHost(  char* type, char* ip, int port, char* user, char* pass, void* dataRef);	//zzd
int npdbLoadNodeTbl( pNPdatabase dbItem, void* dataRef );

//void npMysqlHook_old (HINSTANCE dbLib, void* dataRef); // lde

struct csvStrObjects* npRevisedNodeValues(void* dataRef);
struct newChunksObj* npEvenNewerAllChunk(struct csvStrObjects *csvObjects, void* dataRef);
void npInsertAllChunks(struct newChunksObj *chunks, void* dbID, pNPdbFuncSet func, char* table);

#endif


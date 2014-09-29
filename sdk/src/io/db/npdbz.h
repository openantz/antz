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
	void* (*StatementInsert)  ();
	void* (*StatementCreate)  ();
	void* (*StatementCreateTable) ();
	void* (*StatementUse)     ();
	void* (*StatementShow)    ();
	void* (*StatementDrop)    ();
	void* (*StatementSelect) ();
	void* (__stdcall *show)   ();
	void* (__stdcall *storeResult) ();
	void* (__stdcall *close)  ();
};

// Perhaps a sub-structure inside database which deals with database specific stuff
struct database {
	char dbType[kNPurlMax];   // "mysql" "oracle" "postgresql", etc...
	int id;     // Might be better for this to be void*
	int port;       // Default for MySQL is 3306

	char hostIP[kNPurlMax];   // This could be static
	char user[kNPurlMax];     // root
	char password[kNPurlMax]; // admin 
	char currentlyUsedDatabase[kNPurlMax];
	int* idMap;
	//int idMap[kNPnodeMax];

	struct dbFunction *db;
};


struct databases //This should be renamed as struct server ...or not
{
	pNPdatabases dbList;	///< @todo make this pNPdatabase* dbList
	int numberOfDatabases;
	struct database *activeDB;	///< @todo make this single DB ptr to dbList or dbID index
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

int npAddDb(struct databases *dbs, char* dbType, char* hostIP, char* user, char* pass, char* dbName, void* dataRef); //zzd r
int npUseDatabase2(void* conn, pNPdbFuncSet func, char* dbName);
int npSelect(void* conn, pNPdbFuncSet func, char* table); //Add field(s) choice later
void npNewFreeChunks(struct newChunksObj * chunks, void* dataRef);

int npOpenDb(struct database *db);
int npAttachDbsToDataRef(struct databases *dbs, void* dataRef);

int npdbAddHost(  char* type, char* ip, int port, char* user, char* pass, void* dataRef);	//zzd
int npdbLoadNodeTbl( pNPdatabase dbItem, void* dataRef );

void npMysqlHook_old (HINSTANCE dbLib, void* dataRef);

struct csvStrObjects* npRevisedNodeValues(void* dataRef);
struct newChunksObj* npEvenNewerAllChunk(struct csvStrObjects *csvObjects, void* dataRef);
void npInsertAllChunks(struct newChunksObj *chunks, void* dbID, pNPdbFuncSet func, char* table);

#endif


/* -----------------------------------------------------------------------------
*
*  npdb.c
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

#ifndef NPDB_H_
#define NPDB_H_

#include "../../npdata.h"
#include "../../os/npos.h"	

#include "npdbz.h"


// #include "db/npmysql.h"	//zz debug move to npdb.c and re-direct dependencies	//zz dbz

//------------------------------------------------------------------------------
void npInitDB( void* dataRef );
void npCloseDB( void* dataRef );

void npUpdateDB( void* dataRef );

int npdbConnect( pNPdbHost host, void* dataRef );
void npdbFreeTables(pNPdatabase db);
void npdbFreeTable(pNPdbTable tbl);
void npdbFreeFields(pNPdbTable tbl);
void npdbFreeField(pNPdbFields field);

//get list of databases by name
pNPdatabases npdbGetDatabases( void* dataRef );
pNPdbTable npdbFindNodeTbl( pNPdatabase db, int* err, void* dataRef);
char* npdbGetChMapTableFields(void* dataRef);
char** npdbFetchRow_safe(void* result, pNPdbFuncSet func, int* err);
int npdbTableToCSV(pNPdbTable table, char* csvName, void* dataRef);
int npdbTableToCSVthread(void* threadData);


int npdbSetTables( pNPdbHost host, pNPdatabase db );
int npdbSetTable( pNPdbFuncSet func, void* result, pNPdbTable tbl, pNPdbHost host, void* dataRef );
int npdbGetTbls( pNPdbHost host, pNPdatabase db );
void* npdbGetFieldList(pNPdbTable tbl, int* err, void* dataRef);

char* npdbStatementCreateNodeTable(pNPdbFuncSet func, char* (*fields)(void* dataRef), void* dataRef);
void* npdbStoreResult_Safe(pNPdbFuncSet func, pNPdbHost host, int* err);

char* npdbGetNodeTableFields(void* dataRef); // lde @todo
pNPdbFuncSet npdbGetFuncsFromHost(pNPdbHost host, void* dataRef); // lde, @todo add to header file

//format list of DBs as a menu struct
pNPmenu npdbFormatMenu (pNPdatabases dbList, void* dataRef);

//returns pointer to the migrated table
void* npMigrateTable(void* tablePtr, int size, int formatID, void* dataRef);

//prompt user for migration source dataset and to select format mapping prefs	//zz debug
void npMigrate(int tableMap, void* dataRef);

pNPdbTable npdbCreateChMapTable(pNPdatabase dbItem, int* err, void* dataRef);
//-------------------------------------------------------------------------- //zz db
//manual database Load, Save and Update methods
//auto update logic is done at a higher level, shared with DB, file, OSC...

//delete (drop) the database from the server
//int npdbDrop( int serverID, const char* dbName, void* dataRef );

//loads scene from specified database and (any) referenced databases/sources
/// @todo make DB functions use a dbID ref instead of string names
int npdbLoadScene( pNPdatabase dbItem, void* dataRef );
int npdbLoadRange( int minNodeID, int maxNodeID, void* dataRef );
int npdbLoadList( int dbID, pNPnodeList nodes, void* dataRef );

/// updates entire scene from ALL datasets
int npdbLoadUpdate( void* dataRef ); // added parameter name, lde

	//	data->io.db.update = true;
int npdbSaveUpdate( pNPdatabase dbItem, void* dataRef );

void npdbSaveScene(void* dataRef);

pNPdbTable npdbAddTable( pNPdatabase database, pNPdbTable table, int* err );
//save entire scene to specified database
//if DB exists then overwrites all of it

//int npdbSaveScene( int serverID, const char* dbName, dataRef );	//zz debug
int npdbShowTables( pNPdbHost host);

//appends the scene to pre-existing DB, does NOT delete anything
int npdbAppendScene( pNPdatabase dbItem, void* dataRef );

//updates the range of nodes
//to update all, set both min and max to 0
//to update single node, set both min and max to the node ID
int npdbPullNodeRange( pNPdatabase dbID, int minNodeID, int maxNodeID, void* dataRef );

//handles node tables with multiple table_id references
int npdbPullRecRange( pNPdatabase dbID, int tableID, int minRecID, int maxRecID, void* dataRef );

int npdbPullNodeList( pNPnodeList nodes, void* dataRef );

//update only the specified field(s) of the listed nodes
int npdbPullFields( pNPdatabase dbID, void* fields, pNPnodeList nodes, void* dataRef );
int npdbPushFields( pNPdatabase dbID, void* fields, pNPnodeList nodes, void* dataRef );
/*
//save to new DB only the specified field(s) of the listed nodes
int npdbSaveFields(int dbID, int dbpNPfields fields, pNPnodeList nodes, void* dataRef );

//
//selects nodes based on DB query syntax, updates local scene only,
//to (force) update the DB, call npdbUpdate...() afterwards
//may or may not need to access the DB in order to apply the logic
int npdbSelectNodes( int dbID, const char* query, void* dataRef );

//creates a new database on the specified server using the name
//serverID = 0 will use the most recent server connection opened
//dbName = NULL will generate a timestamp name
//returns a dbID reference
int npdbNew( int serverID, const char* dbName, void* dataRef );


int npdbSample( int dbID, int sampleType, int min, int max, int count, void* dataRef);//load sample using interval/random/exp... 
*/

int npdbDrop( const char* dbName, void* dataRef );
//int npdbUse_old( const char* dbName, void* dataRef );
void npdbActiveHost( char* hostName, void* dataRef);
char* npdbActiveDB( void* dataRef );
void npdbSet( char* dbName, char* tblName, char* setStatement, void* dataRef);
void npdbSelect( char* dbName, char* tblName, char* selectWhere, void* dataRef);

int npdbSetField (pNPdbFields field, pNPdbHost host, void* result, void* dataRef);

void npdbAttachHostFuncSets( pNPdbs dbs );

pNPdatabase npdbGetByName( char* dbName, void* dataRef);

void* npdbMalloc(int size, void* dataRef);
void npdbFree( void* ptr, void* dataRef);
int npdbUse( pNPdatabase dbItem, void* dataRef );
int npdbSelectTable( pNPdatabase dbItem, char* table );
//int npdbSelectTable( pNPtable table );
int npdbShowDatabases( pNPdbHost host );
int npdbAddHostDatabases( pNPdbHost host, pNPdbs dbs );
void npdbAddHost( char* type, char* ip, int port, char* user, char* pass, int* err, void* dataRef);
int npdbNumFields_safe(void* result, pNPdbFuncSet func, int rightNum, int* err );

int npdbHostErr( pNPdbHost host );
int npdbItemErr( pNPdatabase dbItem );

void npdbDropDatabase( pNPdatabase database, int* err, void* dataRef );
int npdbUpdateAntzStateFromDatabase( void* dataRef );	//zz db

int npdbTruncate(void* conn, pNPdbFuncSet func, char* table); //zz db // changed a parameter, lde 
int npdbPushScene ( void* dbID, const char* dbName, void* dataRef );
int npDropDatabase(int dbID, pNPdbFuncSet FuncSet, const char* dbName, void* dataRef );
pNPdatabase npdbSaveAs( char* dbName, pNPdbHost host, int* err, void* dataRef );

pNPdbFuncSet npdbNewFuncSet( pNPdbs dbs );
pNPdbTable npdbFindTagTbl( pNPdatabase db, int* err, void* dataRef);
pNPdbTable new_npdbCreateNodeTable( pNPdatabase dbItem, int* err, void* dataRef);
pNPdbTable npdbCreateTable( pNPdatabase dbItem, char* table, char* fields, int* err );
void npdbDropDatabase( pNPdatabase database, int* err, void* dataRef );
void npdbCreateTableQuery( pNPdatabase database, char* name, char* fields, int* err, void* dataRef );
void npdbSaveChMap (pNPdbTable table, int* err, void* dataRef);
void npdbInsertQuery(pNPdbTable table, char* values, int* err, void* dataRef);


int npdbClearDatabaseList( pNPdbs dbs );
int npdbQuery_safe(void* conn, pNPdbFuncSet func, pNPdbHost host ,char* statement); // Organize these new function prototypes, lde @todo
int npdbNumRows_safe(void* result, pNPdbFuncSet func, int* err);
pNPdbTable npdbNewTable( pNPdatabase database ,char* name, char* fields, int* err );
void npdbDropTable( pNPdbTable table, int* err, void* dataRef );

pNPdatabase new_npdbCreateDatabase( char* dbName, pNPdbHost host, pNPdbs dbs, int* err, void* dataRef ); //for now we'll pass pNPdbs dbs, lde @todo
pNPdbFuncSet npdbGetFuncSetFromHost(pNPdbHost host);
pNPdbHost npdbGetHostFromDatabase( pNPdatabase database );
int npdbLoadTagTbl( pNPdatabase dbItem, void* dataRef );
int new_npdbQuery_safe( pNPdatabase dbItem, char* statement );

/// @todo add error handling and cleanp-up the newly added methods
/// @todo add support for all table types: tag, chmap, tracks, globals, etc...

int npdbLoadNodes( pNPdbFuncSet func, void* result, int* err, void* dataRef);
pNPtag npGetTagFromNode(pNPnode node, void* dataRef);

pNPmenu npdbGetMenu ( pNPmenu menu, void* dataRef);
int npdbLoadMenuItem (int item, void* dataRef);
pNPdbHost npdbGetConnectedHost( pNPdbs dbs );

void npdbInitPtrList(pNPdbs dbs, int* err, void* dataRef);

#endif


/* -----------------------------------------------------------------------------
*
*  npdb.c
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

#ifndef NPDB_H_
#define NPDB_H_

#include "../npdata.h"

#include "db/npdbz.h"
// #include "db/npmysql.h"	//zz debug move to npdb.c and re-direct dependencies	//zz dbz

//------------------------------------------------------------------------------
void npInitDB (void* dataRef);
void npConnectDB (void* dataRef);	//zz db2
void npCloseDB (void* dataRef);

void npUpdateDB (void* dataRef);

//get list of databases by name
pNPdatabases npdbGetDatabases (void* dataRef);

//format list of DBs as a menu struct
pNPmenu npdbFormatMenu (pNPdatabases dbList, void* dataRef);

//returns pointer to the migrated table
void* npMigrateTable(void* tablePtr, int size, int formatID, void* dataRef);

//prompt user for migration source dataset and to select format mapping prefs	//zz debug
void npMigrate(int tableMap, void* dataRef);


//-------------------------------------------------------------------------- //zz db
//manual database Load, Save and Update methods
//auto update logic is done at a higher level, shared with DB, file, OSC...

//save scene with given name, set dbName = NULL to use timestamped name
//int npdbSaveAs( int serverID, const char* dbName, void* dataRef );

//delete (drop) the database from the server
//int npdbDrop( int serverID, const char* dbName, void* dataRef );

//loads scene from specified database and (any) referenced databases/sources
int npdbLoadScene( int dbID, void* dataRef );
int npdbLoadRange( int minNodeID, int maxNodeID, void* dataRef );
int npdbLoadList( int dbID, pNPnodeList nodes, void* dataRef );

//updates entire scene using ALL datasets
int npdbPullScene( void* dataRef );
int npdbUpdateScene( void* dataRef );
int npdbLoadUpdate( dataRef );

	//	data->io.db[0].update = true;
int npdbSaveUpdate( void* dataRef );

//save entire scene to specified database
//if DB exists then overwrites all of it

//int npdbSaveScene( int serverID, const char* dbName, dataRef );	//zz debug

//appends the scene to pre-existing DB, does NOT delete anything
int npdbAppendScene( int serverID, const char* dbName, void* dataRef );

//updates the range of nodes
//to update all, set both min and max to 0
//to update single node, set both min and max to the node ID
int npdbPullNodeRange( int dbID, int minNodeID, int maxNodeID, void* dataRef );

//handles node tables with multiple table_id references
int npdbPullRecRange( int dbID, int tableID, int minRecID, int maxRecID, void* dataRef );

int npdbPullNodeList( pNPnodeList nodes, void* dataRef );

//update only the specified field(s) of the listed nodes
int npdbPullFields( int dbID, void* fields, pNPnodeList nodes, void* dataRef );
int npdbPushFields( int dbID, void* fields, pNPnodeList nodes, void* dataRef );
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

//connects to the DB and returns a generated dbID
int npdbConnect( int serverID, const char* dbName, void* dataRef );

int npdbSample( int dbID, int sampleType, int min, int max, int count, void* dataRef);//load sample using interval/random/exp... 
*/

int npdbDrop( const char* dbName, void* dataRef );
int npdbUse( const char* dbName, void* dataRef );
void npdbActiveHost( char* hostName, void* dataRef);
void npdbActiveDB( char* dbName, void* dataRef);
void npdbSet( char* dbName, char* tblName, char* setStatement, void* dataRef);
void npdbSelect( char* dbName, char* tblName, char* selectWhere, void* dataRef);

#endif


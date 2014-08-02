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

#include "npdb.h"

// #include "db/npmysql.h"		//zz dbz
#include "db/npdbz.h"			//zz dbz

//------------------------------------------------------------------------------
void npInitDB (void* dataRef)
{
	pData data = (pData) dataRef;

	//zzsql moved this from npio.c
//	struct dbNewConnect *connect = malloc(sizeof(struct dbNewConnect)); // debug db //zzsql

//zz dbz	
	struct databases *dbs = malloc(sizeof(struct databases));
	data->io.dbs = dbs;
	
//zz dbz	
	dbs->numberOfDatabases = 0;
//zz dbz	
	npAddDb(dbs, "mysql", "localhost", "root", "admin", "", dataRef);

	//((npInitDbConnection(connect, MySQL, "127.0.0.1", "root", "admin", "antz");))
//	npOpenDb()
//zz db2	npOpenDb( &data->io.dbs->myDatabase[0] );
	//npOpenDbConnect(connect, dataRef);
	//npAttachMySQLtoDataRef(connect, dataRef); // debug db // This can be generalized later, debug db
//zz dbz	npAttachDbsToDataRef(dbs, dataRef);
}

//------------------------------------------------------------------------------
void npConnectDB (void* dataRef)
{
	pData data = (pData) dataRef;

	npOpenDb( &data->io.dbs->myDatabase[0] );

//zz dbz	npAttachDbsToDataRef(dbs, dataRef);
}

//------------------------------------------------------------------------------
void npCloseDB (void* dataRef)		//clean-up
{
	return;
}

//------------------------------------------------------------------------------
void npUpdateDB (void* dataRef)							//add to ctrl loop, debug zz
{
	return;
}


//------------------------------------------------------------------------------
int npdbPullScene( void* dataRef )							//add to ctrl loop, debug zz
{
	pData data = (pData) dataRef;

	//set update flag for all databases related to the scene
	data->io.db[0].update = true;

	return 0;
}
int npdbUpdateScene( void* dataRef )	
{
	return npdbPullScene( dataRef );	
}




/*
//------------------------------------------------------------------------------
int npdbSaveAs( int serverID, const char* dbName, void* dataRef )
{
	pData data = (pData) dataRef;

	if (0)
	{
		npPostMsg( "Saved Scene to Database", kNPmsgDB, data );
		return 0;
	}

	npPostMsg( "Failed to Save Scene to Database", kNPmsgDB, data );
	return 5523;
}
*/
/*
//------------------------------------------------------------------------------
int npdbPushScene ( int dbID, void* dataRef )							//add to ctrl loop, debug zz
{
int err = 0;
	char* nodeFields = NULL;
	const char* dbName = NULL;

	struct csvStrObjects *nodes = NULL;
	struct newChunksObj *chunks = NULL;
	struct dbFunction *myDbFuncs = NULL;

	pData data = (pData) dataRef;

	myDbFuncs = data->io.dbs->myDatabase[0].db;
	dbName = ((struct databases*)data->io.dbs)->dbList->list[dbID];

	printf("\ndatabase name : %s\n", dbName);

	//if DB exists then truncate it, deletes all rows to do update
	//else create a new DB
	if (0)
	{
		err = npDropDatabase( dbID, myDbFuncs, dbName );
		return err;
	}
	else if( 1 )
	{			//if DB exists then truncate it to delete all rows for update
		err = npdbTruncate( dbID, myDbFuncs, "node_tbl" );
		if( err ) return err;
	}
	else	//create DB tables with defined fields
	{
		err = npCreateDatabase2( dbID, myDbFuncs, (char*)dbName );
		if( err ) return err; 

		err = npUseDatabase2( dbID, myDbFuncs, (char*)dbName );
		if( err ) return err;

		nodeFields = npNewGenMysqlFields( kNPmapFileBufferMax, kNPnode, data );
		if( nodeFields != NULL)
		{
			err = npCreateTable2( myDbFuncs, dbID, "node_tbl", nodeFields );	
			free( nodeFields );
			if( err ) return err; 
		}
	}

	//insert the current scene nodes into the empty DB
	nodes = npRevisedNodeValues( dataRef ); 
	chunks = npEvenNewerAllChunk( nodes, dataRef ); 
	
	err = 0;
	npInsertAllChunks( chunks, dbID, &data->io.dbs->myDatabase[0], "node_tbl");

	npNewFreeChunks( chunks, dataRef );

	return err;
}
*/
//------------------------------------------------------------------------------
int npdbSaveUpdate ( void* dataRef )							//add to ctrl loop, debug zz
{
	int err = 0;
	int dbID = 0;
	char dbName[256];

	pData data = (pData) dataRef;

	dbID = data->io.dbs->myDatabase[0].id;

	npdbActiveDB( dbName, data );
	err = npdbPushScene( dbID, 
						 data->io.dbs->myDatabase[0].currentlyUsedDatabase, 
						 data );
	
	return err;
}

//------------------------------------------------------------------------------
int npdbUse( const char* dbName, void* dataRef )							//add to ctrl loop, debug zz
{
	int err = 0;
	int dbID = 0;

	struct databases *dbs = NULL;
	struct dbFunction *myDbFuncs = NULL;

	pData data = (pData) dataRef;


	if( 0)//dbs->numberOfDatabases == 0 )
		npAddDb(data->io.dbs, "mysql", "localhost", "root", "rooti", dbName, data);
	else
	{
		dbs = data->io.dbs;
		myDbFuncs = data->io.dbs->myDatabase[0].db;
		dbID = dbs->myDatabase[0].id;
	
		err = npUseDatabase2( dbID, myDbFuncs, (char*)dbName );

		//and set the string
		strcpy( data->io.dbs->myDatabase[0].currentlyUsedDatabase, dbName );
	}

	return err;
}

//------------------------------------------------------------------------------
int npdbDrop( const char* dbName, void* dataRef )							//add to ctrl loop, debug zz
{
	int err = 0;
	int dbID = 0;

	pData data = (pData) dataRef;

	struct dbFunction *myDbFuncs = data->io.dbs->myDatabase[0].db;

	//if DB exists then drop it
	dbID = data->io.dbs->myDatabase[0].id;
	
	err = npDropDatabase( dbID, myDbFuncs, dbName );

	strcpy( data->io.dbs->myDatabase[0].currentlyUsedDatabase, "\0" );

	if( !err )
	{
		npPostMsg( "Dropped Database", kNPmsgView, data );
		return 0;
	}
//	npPostMsg( "Failed to Drop Database", kNPmsgDB, data );
//	return 5522;

	return err;
}

//------------------------------------------------------------------------------
void npdbActiveHost( char* hostName, void* dataRef)
{
	pData data = (pData) dataRef;

	strcpy( hostName, data->io.dbs->myDatabase[0].hostIP );
}

//------------------------------------------------------------------------------
void npdbActiveDB( char* dbName, void* dataRef)
{
	pData data = (pData) dataRef;

	strcpy( dbName, data->io.dbs->myDatabase[0].currentlyUsedDatabase );
//	dbName = npMysqlShowStatement("current DB");
}

//------------------------------------------------------------------------------
void npdbSet( char* dbName, char* tblName, char* setStatement, void* dataRef)
{
	int result = 0;
	char statement[65535];

	pData data = (pData) dataRef;

	struct dbFunction *db = data->io.dbs->myDatabase[0].db;
	int dbID = data->io.dbs->myDatabase[0].id;

	sprintf( statement, "UPDATE %s SET %s", 
							tblName, setStatement );

	result = (int)(*db->query)(dbID, statement);
}


//------------------------------------------------------------------------------
void npdbSelect( char* dbName, char* tblName, char* selectWhere, void* dataRef)
{
	int result = 0;
	char statement[65535];

	pData data = (pData) dataRef;

	struct dbFunction *db = data->io.dbs->myDatabase[0].db;
	int dbID = data->io.dbs->myDatabase[0].id;

	sprintf( statement, "UPDATE %s SET selected=1 WHERE %s", 
							tblName, selectWhere );

	result = (int)(*db->query)(dbID, statement);
}


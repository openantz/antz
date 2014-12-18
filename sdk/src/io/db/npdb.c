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
#include "../file/npcsv.h" // lde
#include "../../data/npmapfile.h" // lde

#include "npmysql.h"			/// @todo move these to npdbz	
#include "npostgresql.h"
#include "npsqlite.h"
#include "nphadoop.h"
#include "npmongo.h"

//@todo, make antz recognize antz databases from their structure, lde
//@todo, exclude databases from menu that are not of antz type, lde

void npdbConnectHosts( pNPdbs dbs, void* dataRef );
void npdbStartConnMonitor( pNPdbs dbs );
int npdbRefreshDatabaseList( pNPdbs dbs );
int npdbCloseHost( pNPdbHost host );
void npdbConnMonitorThread( pNPdbs dbs, void* dataRef );
pNPdbFuncSet npdbGetHostFuncSet( char* hostType, pNPdbs dbs );
void npdbRowToNode( pNPnode node, char** row );
void npdbUpdateNodesFromMysqlResult( MYSQL_RES *result, void* dataRef );
int npdbCreateTable( pNPdatabase dbItem, char* table, char* fields );
pNPdatabase npdbCreateDatabase( char* dbName, pNPdbHost host, pNPdbs dbs );

//------------------------------------------------------------------------------
void npInitDB (void* dataRef)
{
	pData data = (pData) dataRef;
	pNPdbs dbs = &data->io.db;
	
	data->io.refCount = 0;
	/// load DB libraries and hook DB specific functions
	npInitMySQL( data );			///< MySQL client is our default RDBMS
	npInitPostgreSQL( data );		///< PostgreSQL has useful GIS tools
	npInitSQLite( data );			///< SQLite is a local file based RDMS
	npInitMongoDB( data );			///< MongoDB is a realtime NoSQL database

	dbs->connectHosts = npdbConnectHosts; // This should be moved, lde @todo
	dbs->connectHosts( dbs, data );		///< connect to each host in the list
	//npdbConnectHosts( dbs, data );	///< connect to each host in the list
	
	npdbStartConnMonitor( dbs );	///< keep hosts connections alive

	npdbRefreshDatabaseList( dbs );	/// refresh the dbList using hosts list

	if ( dbs->running == true ) {
		printf("true\n");
		dbs->activeDB = dbs->dbList[1]; // temp fix, lde
	}
	else {
		printf("false\n");
	}

	
	dbs->running = true;			/// database system is ready to use // temp, lde

	printf("\n");
}


int npdbConnect( pNPdbHost host, void* dataRef )
{
	int err = 0;

	void* connInit = NULL;	///< void* to store MYSQL* init handler
//	void* conn = NULL;		///< void* to store MYSQL* conn handler 

	pNPdbFuncSet func = NULL;
	pData data = (pData) dataRef;
	
	if( !host )
	{	
		printf( "err 5565 - npdbConnect host is NULL \n" );
		return 5565;
	}
	
	if( (func = host->hostFuncSet) == NULL )
	{	
		printf( "err 5566 - npdbConnect host->funcSet is NULL \n" );
		return 5566;
	}

	/// initialize the database connection structure
	connInit = func->init(NULL);
	if( !connInit )
	{					//you cannot use db error checking if init fails
		printf("err 5567 - host func->init failed, likely out of memory \n");
		return 5567;
	}
	

	/// set connection options, such as RECONNECT flag // esp, lde
	err = (int)func->InitConnOptions( func, connInit );
	if( err ) return err;

	/// connect to the host using default database by passing in "" for DB
	host->conn = func->connect( connInit, host->ip, host->user, host->password,
						  "", host->port, NULL, 0 );
	if( !host->conn )
	{	
		printf( "%s err: %u - %s\n", host->type,
			(unsigned int)func->db_errno(connInit), (char*)func->db_error(connInit) );
		
		host->connected = false; // lde, new
		printf("\nhost not connected");
		return 5568;   //err 5568
	}
	
	host->connected = true; // lde, new
	printf("\nhost connected");
	
	// Should I use client / server instead of host / db, lde
	printf("\ndata->io.db : %p", &data->io.db);
	printf("\ndata->io.db.activeDB : %p", data->io.db.activeDB);
	printf("\ndata->io.db.activeDB->id : %d", data->io.db.activeDB->id);
	printf("\nhost->conn : %d", (int)host->conn);
	data->io.db.activeDB->id = host->conn;
	
	return 0;	//success
}
/// iterate through list of hosts and attempt to connect to each
void npdbConnectHosts( pNPdbs dbs, void* dataRef )
{
	int i = 0;
	int err = 0;

	pNPdbHost host = NULL;
	pData data = (pData) dataRef;
		
	if( !dbs )
		{ printf("err 5577 - npdbConnectHosts has null dbs \n"); return; }
	
	// There needs to be multiple "host" connected variables for connecting to multiple databases, lde
	/// iterate through the hostList, starts at 1 to skip the first null host
	for( i=1; i < dbs->hostCount; i++ )
	{
		host = dbs->hosts[i];
		if( !host )
			printf("err 5578 - null host in db->hosts item: %d \n", i );
		else
		{
			data->io.db.activeDB->host = host;
			//if( err = npdbConnect(host, dataRef) ) // Experimental, lde
			if( err = host->connect(host, dataRef) )
			{
				printf( "err %d - failed to connect %s host: %s\n", 
						err, host->type, host->ip ); 
				host->connected = false;
				printf("\nHost connection lost");
			}
			else
			{
				printf("\nconnected %s host: %s\n", host->type, host->ip );
				host->connected = true;
			}
		}
	}
}

//------------------------------------------------------------------------------
void npCloseDB (void* dataRef)		//clean-up
{
	int i = 0, j = 0;

	pData data = (pData) dataRef;
	pNPdbHost host = NULL;
	pNPdatabase dbItem = NULL;
	pNPdbs dbs = &data->io.db;
	pNPdbTable table = NULL;


	/// @todo add thread lock for DB connection shutdown
	// npdbKillThreads();
	dbs->running = false;	// sudo thread lock

	/// close all host connections and free host structures
	for( i=0; i < dbs->hostCount; i++)
	{
		host = dbs->hosts[i];
		
		if( host->hostFuncSet && host->conn )
			host->hostFuncSet->close( host->conn );	/// close host connections

		free( host );							/// free host structures
	}

	for( i=0; i < dbs->funcSetCount; i++ )
		free( dbs->funcSetList[i] );			/// free host function sets

	for( i=0; i < dbs->dbCount; i++ )
	{
		dbItem = dbs->dbList[i];
		
		for( j=0; j < dbItem->tableCount; j++ )
		{
		//	table = dbItem->tables[i];
			table = dbItem->tableList[i]; // new, lde
			free( table->mapID );				/// free table id maps
			free( table );						/// free tables
		}
	
		free( dbItem );							/// free database items
	}

	return;
}

/*!	Monitors database host connections.
	- Keeps connection alive by pinging hosts list every 10 secs.
	- Attempts to re-estabilish not connected hosts

	@param dbs is the core database structure for this global scene context.
*/
//------------------------------------------------------------------------------
void npdbConnMonitorThread( pNPdbs dbs, void* dataRef )
{
	int i = 0;
	unsigned long temp_conn_id = 0;
	pNPdbHost host = NULL;
	pNPdbFuncSet func = NULL;
	void* conn = NULL;
	
	printf( "DB host pinging thread started \n" );

	
	/// wait for DB system startup
	while( !dbs->running )
	{
	//	printf("\ndbs->running %d", dbs->running);
	}

	/// @todo add thread wrapper with killThread flag
	//printf("\nRUNNING\n");
	while( dbs->running )
	{
		/// ping hosts list every 10 secs
		/// zoneshift.net seems to like 10 secs, shorter causes err 2006
		/// zs times out after about 30 secs w/o ping, not sure why so short
		/// @possible todo : have system test ping limits and settle, lde
		//printf("\nbefore sleeping...\n");
		nposSleep( 10.0 );

		printf( "ping DB hosts\n" );

		for( i=1; i < dbs->hostCount; i++ )
		{
			host = dbs->hosts[i];
			if( !host )
			{
				printf("err 5411 - null host id: %i \n", i);
			}
			else if( !host->hostFuncSet )
			{
				printf("err 5412 - null host function set id: %i \n", i );
			}
			else if( !host->conn )
			{
				/// @todo attempt to establish a connection
				//npdbConnect( host, dataRef ); //@todo, this should be passed a pNPdatabase / pNPserver / pNPdbServer structure, lde
				host->connect( host, dataRef ); // experimental, lde @todo
				
			}
			else
			{
				func = host->hostFuncSet;
				conn = host->conn;

				host->conn_id = (unsigned long)func->conn_thread_id(host->conn);
				temp_conn_id = host->conn_id;
				if( func->ping(conn) ) // If RECONNECT is set then a ping could cause a reconnect, but it isn't guaranteed. One still needs to check, lde
				{
					printf( "err 5528 - ping failed host: %s\n", host->ip );
					printf( "%s err: %u - %s\n", host->type, 
							(unsigned int)func->db_errno(conn), (char*)func->db_error(conn));
					//host->connected = false;
					host->conn_id = (unsigned long)func->conn_thread_id(host->conn);
					if(host->conn_id == temp_conn_id)
					{
						printf("\nConnection Lost, Reconnect failed\n");
						host->connected = false;
					}
					else {
						printf("\nConnection Lost, Reconnect successful\n");
						host->connected = true;
					}

					
					//printf("\nhost not connected");
				}

				
			}
		//	dbs->running = true;
		}
	
	}
}

/** Sync DB with main loop.
	@param dataRef is a global map reference instance.
*/
//------------------------------------------------------------------------------
void npdbStartConnMonitor( pNPdbs dbs )							//add to ctrl loop, debug zz
{
	nposBeginThread( (void*)npdbConnMonitorThread, dbs );
}

/** Sync DB with main loop.
	@param dataRef is a global map reference instance.
*/
//------------------------------------------------------------------------------
void npUpdateDB (void* dataRef)							//add to ctrl loop, debug zz
{
	//int i = 0; // warning, lde
	int result = 0;
	char msg[256];
	//char host[256];
	pData data = (pData) dataRef;
	pNPdbs dbs = NULL;

	//char* dbName = NULL; // warning, lde

	dbs = &data->io.db;
//	dbName = data->io.dbs->activeDB[0].currentlyUsedDatabase;

	/// keep our host connections alive
//	npdbPingHosts( dbs );

	if(0)// db->autoUpdate )
	{	// make this do partial seconds.... and actual time
		if( data->io.cycleCount % (int)dbs->loadUpdateRate == 0 )
			dbs->loadUpdateRate = 5.0f;
	}

	/// if scene is flagged for an update then do it.
	if( dbs->loadUpdate )
	{
		result = npdbUpdateAntzStateFromDatabase( dataRef );
		
		/// @todo pull the msg up a level, is UI task, not a DB task
		if( !result )
		{
		//	sprintf( msg, "Done Updating DB: %s", dbName );
			npPostMsg( msg, kNPmsgDB, data );
		}	
		else if( result == 4242 )
		{
		//	printf("host: %s   db: %s\n", host->id, dbName);
			npPostMsg( "cannot load update - No Active DB", kNPmsgDB, data );
			npPostMsg( "LOAD, SAVE or USE a DB to make Active!", kNPmsgDB, data );
		}

		dbs->loadUpdate = false;
	}

	return;
}


//------------------------------------------------------------------------------
int npdbLoadUpdate( void* dataRef )							//add to ctrl loop, debug zz
{
	char msg[kNPurlMax *2];
	char* dbName;//[kNPurlMax];
	char host[kNPurlMax];

	pData data = (pData) dataRef;

	npdbActiveHost( host, data );
	dbName = npdbActiveDB( data );

	if( !dbName || !host )
	{
		printf("host: %s   db: %s\n", host, dbName);
		npPostMsg( "cannot load update - No Active DB", kNPmsgDB, data );
		npPostMsg( "LOAD, SAVE or USE a DB to make Active!", kNPmsgDB, data );
		return 4244;
	}

	sprintf( msg, "Update Scene from DB: %s  host: %s", dbName, host );
	npPostMsg( msg, kNPmsgDB, data );

	//set update flag for all databases related to the scene
	data->io.db.loadUpdate = true;

	return 0;
}


//------------------------------------------------------------------------------
int npdbSaveUpdate ( pNPdatabase dbItem, void* dataRef )
{
	int err = 0;
	char msg[256];

	pData data = (pData) dataRef;


	if( npdbItemErr(dbItem) ) return 5407;

	printf("Save Update to DB: %s  host: %s", dbItem->name, dbItem->host->ip);

//	err = npdbPushScene( dbID, 
//						 data->io.dbs->activeDB[0].currentlyUsedDatabase, 
//						 data );
	
	/// @todo pull the user msg up a level, is UI task, not a DB task
	if( !err )
	{
		sprintf( msg, "Done Saving DB Update: %s  host: %s", 
			dbItem->name, dbItem->host->ip );
		npPostMsg( msg, kNPmsgDB, data );
	}	
	else
	{
		sprintf( msg, "err 5585 - Save Update failed code: %d DB: %s  host: %s",
				 err, dbItem->name, dbItem->host->ip );
		npPostMsg( msg, kNPmsgDB, data );
	}

	return err;
}


//------------------------------------------------------------------------------
int npdbDrop( const char* dbName, void* dataRef )							//add to ctrl loop, debug zz
{
	int err = 0;
	int dbID = 0;
	char msg[256];

	pData data = (pData) dataRef;

//	struct dbFunction *myDbFuncs = data->io.dbs->activeDB[0].dbFunc; // lde
	pNPdbFuncSet FuncSet = data->io.db.activeDB->host->hostFuncSet;
	
	//if DB exists then drop it
	//dbID = data->io.dbs->activeDB[0].id;
	dbID = data->io.db.activeDB->id;
	
	//err = npDropDatabase( dbID, myDbFuncs, dbName, data );
	printf("\ndbID: %d", dbID);
	err = npDropDatabase(dbID, FuncSet, dbName, data);
	
	//strcpy( data->io.dbs->activeDB[0].currentlyUsedDatabase, "\0" ); // temp lde
	
	if( err )
	{
		sprintf( "Failed to Drop Database: %s", dbName, msg );
		npPostMsg( msg, kNPmsgDB, data );
	}
	else
		npPostMsg( "Dropped Database", kNPmsgErr, data );

	return err;
}

//------------------------------------------------------------------------------
void npdbActiveHost( char* hostName, void* dataRef)
{
	pData data = (pData) dataRef;

//	strcpy( hostName, data->io.dbs->activeDB[0].hostIP );
	strcpy( hostName, data->io.db.activeDB->host->ip);
}

//------------------------------------------------------------------------------
char* npdbActiveDB( void* dataRef )
{
	char* dbName = NULL;
	pData data = (pData) dataRef;

	//dbName = data->io.dbs->activeDB[0].currentlyUsedDatabase;
	dbName = data->io.db.activeDB->name;
	if( dbName )//strcmp( dbName, "") == 0
	{
		if( dbName[0] == '\0')
			return NULL;
	}

	return dbName;
//	dbName = npMysqlStatementShow("current DB");
}

/// @todo update npdbSet
//------------------------------------------------------------------------------
void npdbSet( char* dbName, char* tblName, char* setStatement, void* dataRef)
{
	int result = 0;
	char statement[65535];

	pData data = (pData) dataRef;

	//struct dbFunction *db = data->io.dbs->activeDB[0].dbFunc; // lde
	pNPdbFuncSet FuncSet = data->io.db.activeDB->host->hostFuncSet;
	//int dbID = data->io.dbs->activeDB[0].id;
	int dbID = data->io.db.activeDB->id;
	
	sprintf( statement, "UPDATE %s SET %s", 
							tblName, setStatement );

	//result = (int)(*db->query)(dbID, statement);
	result = (int)(*FuncSet->query)(dbID, statement);
}

//------------------------------------------------------------------------------
void npdbSelect( char* dbName, char* tblName, char* selectWhere, void* dataRef)
{
	int result = 0;
	char statement[65535];

	pData data = (pData) dataRef;

	pNPdbHost host = data->io.db.activeDB->host;
	pNPdbFuncSet func = host->hostFuncSet;
	//pNPdbFuncSet func = data->io.db.activeDB->host->hostFuncSet;
	
	int dbID = data->io.db.activeDB->id;

	sprintf( statement, "UPDATE %s SET selected=1 WHERE %s", 
							tblName, selectWhere );

	result = (int)(*func->query)(dbID, statement);
}


/*!	Get the currently active DB host or first connected host in list.

	@param dbs is the core DB structure for this global scene context.
	@return pointer to first connected host, or NULL if none available.
*/
pNPdbHost npdbGetConnectedHost( pNPdbs dbs )
{
	int i = 0;
	pNPdbHost host = NULL;

	/// if activeDB exists and has a connected host, then return the host
/*
	if( dbs->activeDB && dbs->activeDB->host && dbs->activeDB->host->conn )
		return dbs->activeDB->host;
*/
	if( dbs->activeDB )
	{
		if(dbs->activeDB->host)
		{
			if(dbs->activeDB->host->conn)
			{
				return dbs->activeDB->host;
			}
		}
	}
	
	/// else we search for first connected host
	for( i=1; i < dbs->hostCount; i++ )
	{
		host = dbs->hosts[i];
		if( host && host->conn )
			return host;
	}

	return NULL; //no connected hosts return NULL
}

/*! Save the scene state to the currently active database host.
	@param dataRef is our global scene context pointer.
*/
void npdbSaveScene( void* dataRef )
{
	/*
	int err  = 0;  // Warning, lde
	int dbID = 0; // Warning, lde
	*/
	 
	char dbName[kNPurlMax];	//max db identifier is 64 chars
	char msg[kNPurlMax + kNPurlMax];

	pData data = (pData) dataRef;
	pNPdbs dbs = &data->io.db;
	pNPdbHost host = NULL;

	
	/// @todo add user prompt to choose host and name to Save As
	/*
	// show menu of connected hosts and option to type one in
	strcpy( question, "Save New Database As?");
	strcpy( defaultAnswer, dbName );
	err = npConsoleAskUser( dbName, "Save Database As?", data );
	*/

	/// get the first connected host to save to it
	host = npdbGetConnectedHost( dbs );
	if( !host )
	{	/// @todo add user option to type-in a host or load a globals file
		npPostMsg("err 5507 - no database host connected", kNPmsgErr, data );
		return;
	}

	/// generate time-stamp for DB name
	nposTimeStampName( dbName );

	sprintf(msg,"Saving New Database: %s  host: %s", dbName, host->ip );
	npPostMsg(msg, kNPmsgView, dataRef);

	if( npdbSaveAs(dbName, host, data) == NULL )
	{
		sprintf( msg, "err 5402 - failed to Save DB: %s", dbName);
		npPostMsg( msg, kNPmsgView, data );
	}
	else
	{
		sprintf(msg,"Done Saving Database: %s", dbName );
		npPostMsg(msg, kNPmsgView, dataRef);
	}
}

int npdbLoadTagTable( pNPdatabase dbItem, void* dataRef )
{
	int err = 0;
	
	
	return err;
}


int npdbLoadScene( pNPdatabase dbItem, void* dataRef )
{
	int err = 0;
	char msg[256];
//	char* hostIP = NULL; // Warning, lde

	pData data = (pData) dataRef;
	pNPdatabase activeDB = NULL;

	dbItem->loadNodeTbl = npdbLoadNodeTbl; // move elsewhere, lde @todo // loadNodeTbl should be loadTbl, lde @todo
	if( err = npdbItemErr(dbItem)) return err;	///ascert database connection is valid
	
	
	sprintf( msg, "Loading DB: %s  host: %s", dbItem->name, dbItem->host->ip );
	npPostMsg( msg, kNPmsgView, data );

	/// load the node table into the scene
	//err = npdbLoadNodeTbl( dbItem, data );
	npMapSortInit (data->map.sortID, data);
	dbItem->loadNodeTbl( dbItem, data ); // new, lde
	if( err )
		sprintf( msg, "Failed to Load DB: %s  code: %d", dbItem->name, err );
	else
	{
		/*
		data->io.db.activeDB = dbItem;
		printf("\nnew activeDB : %p", data->io.db.activeDB);
		*/
		//activeDB = dbItem;
		//activeDB->host = dbItem->host;
		//data->io.db.activeDB->host = dbItem->host;
		sprintf( msg,"Done Loading DB: %s", dbItem->name );
//		sprintf( msg,"Done Loading DB: %s", activeDB->name);
	}
	npMapSort(dataRef);
	
	err = npdbLoadTagTbl( dbItem, data );
	if(err) return err;
	
	npPostMsg( msg, kNPmsgDB, dataRef );

	/// @todo add support for all tables, especially tags
	//err += npdbLoadTags( dbItem );
	
	return err;
}

/*!	Clear the list of databases and re-populate from hosts list

	@param dbs is the core database structure for the global context.
	@return 0 if no errors, otherwise return err number or err count.
*/
int npdbRefreshDatabaseList( pNPdbs dbs )
{
	int err = 0;	///< accumulates errs
	int i = 0;
	pNPdbHost host = NULL;
//	pNPdatabase dbItem = NULL; // Warning, lde

	if( !dbs )
	{
		printf( "err 5580 - npdbRefreshDatabaseList given null pNPdbs ptr \n" );
		return 5580; //err
	}

	err = npdbClearDatabaseList( dbs );
	if( err ) return err;

	/// iterate through hosts and add all databases, i=1 to skip null hosts[0]
	for( i=1; i < dbs->hostCount; i++)
	{
		host = dbs->hosts[i];

		if( !host )
			printf( "err 5505 - null host in hosts list \n" );
		else if( !host->conn )
			printf( "ignoring host: %s - not connnected\n", host->ip );
		else 
		{	/// add our host databases to the dbList
		   if( (err = npdbAddHostDatabases(host, dbs)) )
			  printf( "err 5506 - failed to add host databases code: %d",err);
		}

		//printf( "host: %s   conn: %p \n", host->ip, host->conn ); //zzd r
	}

	/// check that all database items and there host references are valid
	for( i=1; i < dbs->dbCount; i++ )
		npdbItemErr( dbs->dbList[i] );

	/*	if ( i <= 3 )	/// test print DB list
			printf("DB: %-24s host: %s\n", dbItem->name, dbItem->host->ip);
		else
			printf( "." );	/// running dots for each additional item
	 */

	printf( "Database List Updated!\n" );
	return err;
}

/*! Iterate through hosts list and attach funcSet to any mysql items.
	param dbs is the core database structure for the global scene context.
*/
void npdbAttachHostFuncSets( pNPdbs dbs )
{
	int i = 0;
	int j = 0;
	pNPdbHost host = NULL;

	/// iterate through each host item in the hosts list
	for( i=0; i < dbs->hostCount; i++ )
	{
		host = dbs->hosts[i];
		host->connect = npdbConnect; // Expand on later, @todo
		/// search function sets for host type match
		for( j=0; j < dbs->funcSetCount; j++ )
			if( strcmp( host->type, dbs->funcSetList[j]->hostType ) == 0 )
			{
				host->hostFuncSet = dbs->funcSetList[j];	///< attach host funcSet
				// printf("%s hooked to host: %s\n", host->type, host->ip );
			}
	}
}
pNPdatabase npdbGetByName( char* dbName, void* dataRef);
pNPdatabase npdbGetByNameAndHost( char* dbName, pNPdbHost host, pNPdbs dbs );
pNPdatabase npdbGetByNameAndHost( char* dbName, pNPdbHost host, pNPdbs dbs )
{
	int i = 0;
	pNPdatabase dbItem = NULL;


	if( !dbName || !host || !dbs )
	{
		printf("err 5406 - npdbGetByNameAndHost has null element \n");
		return NULL;
	}

	/// search dbList for matching dbName for the given host
	for( i=1; i < dbs->dbCount; i++ )
	{
		dbItem = dbs->dbList[i];
		if( npdbItemErr(dbItem) == 0 )
			if( strcmp( host->ip, dbItem->host->ip ) == 0 )
				if( strcmp( dbName, dbItem->name ) == 0 )
					return dbItem;	/// match found return dbItem
	}

	return NULL;					/// item not found return NULL
}

pNPdatabase npdbGetByName( char* dbName, void* dataRef)
{
	int i = 0;

	pData data = (pData) dataRef;
	pNPdbs dbs = &data->io.db;
	pNPdatabase dbItem = NULL;
	pNPdbHost host = NULL;
	

	if( dbs->activeDB )
		host = dbs->activeDB->host;

	/// search activeDB host for dbName match
	if( host )
	{
		/// first search the activeDB host only
		for( i=1; i < dbs->dbCount; i++ )
		{
			dbItem = dbs->dbList[i];
			if( dbItem && dbItem->host )
				if( strcmp( host->ip, dbItem->host->ip ) == 0 )
					if( strcmp( dbName, dbItem->name ) == 0 )
						return dbItem;	/// match found return dbItem
		}
	}

	/// search all databases of all hosts for dbName match
	for( i=1; i < dbs->dbCount; i++ )
	{
		dbItem = dbs->dbList[i];
		if( dbItem )
			if( strcmp( dbName, dbItem->name ) ==  0 )
				return dbItem;			/// match found return dbItem
	}

	return NULL;					/// item not found return NULL
}


/*! Submits USE [database] query statement to the dbItem's host.
	Does not effect the local activeDB pointer.

	@param dbItem is the database reference to USE.
	@return 0 for success otherwise an err number.
*/
int npdbUse( pNPdatabase dbItem )
{
	int err = 0;
	char* statement = NULL;
	void* conn = NULL;
	pNPdbFuncSet func = NULL;

	/// check that the database item and host connection are valid
	if( err = npdbItemErr( dbItem ) ) return err;
	
	func = dbItem->host->hostFuncSet;
	conn = dbItem->host->conn;

	/// create the USE statement
	statement = func->StatementUse( dbItem->name );
	if( !statement ) return 1;

	/// query the USE statement
	printf("query: %s\n", statement);
	err = npdbQuery_safe(conn, func, dbItem->host, statement);
	if( err ) return err;
	
	if(err == 0) // This isn't necessary, lde
	{
		strcpy(dbItem->host->inUseDB, dbItem->name);
		printf("\nDatabase now in use is %s\n", dbItem->host->inUseDB);
	}

	free(statement);
	return err;
}

// I suspect the seg fault originates here, lde @todo // fixed
int new_npdbSelectTable( pNPdbTable table )
{
	pNPdatabase  database	= table->owner;
	pNPdbHost    host		= database->host;
	pNPdbFuncSet func		= host->hostFuncSet;
	void*		 conn		= host->conn;
	char*		 statement  = NULL;
	int			 err		= 0;
	
	printf("\nnew_npdbSelectTable");
	
	
	if( strcmp(database->name, host->inUseDB) != 0 )
	{
	//	npdbUse(database);
		func->use(database);  // should be database->use, lde @todo
	}
	
	statement = func->StatementSelect(table->name);
	if( !statement ) return 2;
	
	printf("query: %s\n",statement);
	
	if( err = npdbItemErr(database))
	{
		printf("\nerr : %d\n", err);
		return err;	/// ascert valid DB and connection
	}
		
	err = npdbQuery_safe(conn, func, host, statement);
	if( err )
	{
		printf("\nnpdbSelectTable query failed");
		return err;
	}

	
	free(statement);
	return err; //Zero for success, nonzero if error occurred
	
}

int npdbSelectQuery( void* conn, pNPdbFuncSet func, pNPdbHost host, char* (*selectStatement)(), char* table ) // new, lde @todo
{
	char* statement = NULL;
	statement = selectStatement(table);
	
	npdbQuery_safe(conn, func, host, statement);
	
	return 0;
}

int npdbSelectTable( pNPdatabase dbItem, char* table ) //Add field(s) choice later, @todo, lde
{
	int err = 0;
	char* statement = NULL;
	void* conn = NULL;
	pNPdbFuncSet func = NULL;
	printf("\nnpdbSelectTable");
	
	if( npdbItemErr(dbItem)) return 1;	/// ascert valid DB and connection

	func = dbItem->host->hostFuncSet;
	conn = dbItem->host->conn;

	statement = func->StatementSelect(table);
	if( !statement ) return 2;

	printf("query: %s\n",statement);
	
	npdbQuery_safe(conn, func, dbItem->host, statement);
	if( err )
	{
		printf("npdbSelectTable query failed \n");
		return err;
	}

	free(statement);
	return err; //Zero for success, nonzero if error occurred
}
//  new_npdbQuery_safe(void* conn, pNPdbFuncSet func, pNPdbHost host, char* (*statement)()) // new, lde @todo
int npdbQuery_safe(void* conn, pNPdbFuncSet func, pNPdbHost host ,char* statement)
{
	int err = 0;
//	printf("\nnpdbQuery_safe");
	
	if( err = (int)func->query( conn, statement ) )
	{
		printf( "%s err: %u - %s\n", host->type,
			   (unsigned int)func->db_errno( conn ), (char*)func->db_error( conn ) );
		err = 5576;   //err 5576 or is it 5583
	}
	
	return err;
}

// New function, lde
int npdbShowTables( pNPdbHost host)
{
	int err = 0;
	char* statement = NULL;
	void* conn = NULL;
	pNPdbFuncSet func = NULL;
	
	
	/// ascert valid host, connection and function set
	if( err = npdbHostErr(host)) return err;
	
	func = host->hostFuncSet; // npdbGetFuncFromHost , lde
	conn = host->conn;
	
	/// create our SHOW TABLES statement and query it
	statement = func->StatementShow( "TABLES" );
	if( !statement ) return 5575;	//err 5576 , lde, check on this
	
	/// post and execute our query
	printf( "host: %.24s  query: %s\n", host->ip, statement );
	
	npdbQuery_safe(conn, func, host, statement); // @todo, make the "safe" functions called by funcPtrs, lde
	if( err ) 
	{
		free(statement);
		return err;
	}
	
	free( statement );
	return err;
	
}


/*! SHOW DATABASES is used to get our DB list for the host.
	After, you must call fetch_result and free the result.

	@param host pointer to item entry in hosts list.
	@return 0 if success or err number if failed.
*/
// The database struct should point back to hosts, lde @todo
int npdbShowDatabases( pNPdbHost host ) // Make this more generic, lde @todo
{
	int err = 0;
	char* statement = NULL;
	void* conn = NULL;
	pNPdbFuncSet func = NULL;

	
	/// ascert valid host, connection and function set
	if( err = npdbHostErr(host)) return err;

	func = host->hostFuncSet;
	conn = host->conn;

	/// create our SHOW DATABASES statement and query it
	statement = func->StatementShow( "DATABASES" );
	if( !statement ) return 5575;	//err 5576

	/// post and execute our query
	printf( "host: %.24s  query: %s\n", host->ip, statement );

	err = npdbQuery_safe(conn, func, host, statement);
	if( err ) return err;

	free( statement );
	return err;
}



/*!
	@param db is a pointer to the core pNPdbs structure.
	@return 0 if success, 1 if failed because pNPdbs structure is null.
*/
int npdbClearDatabaseList( pNPdbs dbs )
{
	int i = 0;

	if( !dbs )
	{
		printf( "err 5409 - npdbClearDatabaseList given null pNPdbs \n" );
		return 1;
	}

	/// set all dbList items to null and free existing structures
	for( i=0; i < kNPdbMax; i++ )
	{
		/// only free the items that are within the count
	//	if( i < dbs->dbCount && dbs->dbList[i] )
			free( dbs->dbList[i] );

		dbs->dbList[i] = NULL;	/// set entire list of pointers to NULL
	}

//	db->dbList[0] = npInitDatabase();	/// add null database item in slot 0
	dbs->dbCount = 1;

	return 0;		//success
}

/// @returns a new initialized database item 
pNPdatabase npdbNewDatabase( char* name, pNPdbHost host, pNPdbs db )
{
	pNPdatabase database = NULL;

	if( db->dbCount >= kNPdbMax )
	{
		printf( "err 5582 - kNPdbMax hit \n" );
		return NULL;
	}

	/// get a newly initialized database structure
	database = npInitDatabase();

	/// set the database id, name and host parameters
	database->id = db->dbCount;
	strncat( database->name, name, kNPdbNameMax - 1 ); //init sets name[0]='\0'
	database->host = host;

	database->format = 0;		/// @todo implement 3rd party format support

	/// add the new database item to the dbList and increment the count
	db->dbList[db->dbCount++] = database;

	return database;
}

/// @returns pointer to the item in the dbList
/// if database is already in the list then update, otherwise add to end.
pNPdatabase npdbAddDatabase( char* dbName, pNPdbHost host, pNPdbs dbs )
{
	int i = 0;
	pNPdatabase dbItem = NULL;

	/// search dbList for existing entry matching host and database name
	for( i=0; i < dbs->dbCount; i++ )
	{
		dbItem = dbs->dbList[i];
		if( dbItem )
		{
			/// if host and database names match then return the item
			if( strcmp( host->ip, dbItem->host->ip ) == 0
				&& strcmp( dbName, dbItem->name ) == 0 )
			{
				return dbItem;	/// exit nothing to do, return existing item
			}
		}
	}

	/// else create a new pNPdatabase item and add to the end of dbList
	dbItem = npdbNewDatabase( dbName, host, dbs );
	
	return dbItem;			/// return the newly added database item
}

//@todo, lde, function to check databases/tables for equality
//@todo, lde, function to copy/move a database/table to another server

int npdbAddHostDatabases( pNPdbHost host, pNPdbs dbs )
{
	int i = 0;
	int err = 0;

	pNPdbFuncSet func = NULL;
	void* conn = NULL;

	void* result = NULL;			/// using void* for MYSQL_RES* type
	char** row = NULL;				/// using char** for MYSQL_ROW type

	unsigned long* fieldLengths;
	int numFields = 0;
	int numRows = 0;
	char dbName[kNPdbNameMax+1];	/// database name max +1 for '\0'


	/// assert our core dbs structure is valid
	if(!dbs){ printf("err 5501 - null core dbs structure \n" ); return 5501; }

	/// assert host, connection and host function set are valid
	if( err = npdbHostErr( host ) ) return err;

	/// assign our host server type specific function set
	func = host->hostFuncSet;
	conn = host->conn;

	/// query host with SHOW DATABASES statement
	//err = npdbShowDatabases( host );
	err = func->showDatabases( host ); // new, lde
	if( err ) return err;

	/// store the result
	result = npdbStoreResult_Safe(func, host, &err);
	if(err)
	{
		printf("npdbGetList failed to store result\n");
		return err; // Should this be a different error than what npdbStoreResult_Safe returns, lde @todo
	}

	/// get the number of fields, should be 1 for list of databases
	numFields = npdbNumFields_safe(result, func, 1, &err);
	if( err ) return err;
	 
	/// get the number of rows in result
	numRows = npdbNumRows_safe(result, func, &err);
	if( err ) return err;

	/// store the database count for this host
	host->dbCount = numRows;
	printf( "host: %.32s  databases: %d\n", host->ip, numRows );
	
	/// iterate through each row and add it to the database list
	/// http://dev.mysql.com/doc/refman/5.0/en/mysql-fetch-row.html
	while( row = func->fetch_row(result) )
	{
		/// get the database name string length, string is not terminated!
		if( (fieldLengths = func->fetch_lengths(result)) == 0 )
		{
			printf("err 5568 - DB function fetch_lengths failed \n" );
			func->free_result( result );
			return 5568;
		}

		/// copy our database cell to our local dbName cstr
		dbName[0] = '\0'; /// starting with '\0' is important for strncat
		
			strncat( dbName, row[0], fieldLengths[0] );  //strncat adds '\0'
			
			/// post first couple rows
			if ( i++ < 3 )
				printf( "add DB: %s\n", dbName );
			else
				printf( "." );		/// running dots for each additional row
			
			/// add or update this database entry in the dbList
			npdbAddDatabase( dbName, host, dbs );
		


	}
	printf("\n");				// terminate our running dots

	func->free_result( result );
	return 0;					// success
}

/*! Gets the function set for the host server type specified as a string.
	
	@param hostType is the host server type string, ie: 'mysql'
	@param dbs is our core DB structure.
	@return pointer to host function set, NULL if host type not present.
*/
pNPdbFuncSet npdbGetHostFuncSet( char* hostType, pNPdbs dbs )
{
	int i = 0;
	pNPdbFuncSet func = NULL;
	printf("\nnpdbGetHostFuncSet : ");
	if( !hostType )
	{
		printf( "err 5529 - npdbGetHostFuncSet called with null hostType \n");
		return NULL;
	}

	printf("\nHMMM");
	printf("\nhostType : %s\n", hostType); // Segfaulting here
	printf("\nAFT");
	/// search function set for matching host type
	for( i=0; i < dbs->funcSetCount; i++ )
	{
		func = dbs->funcSetList[i];
		if( !func )
			printf( "err 5530 - null function set in list \n" );
		else if( strcmp( func->hostType, hostType ) == 0 )
			return func;
	}

	printf( "err 5531 - host type not found in DB function set list \n" );
	return NULL;   /// host type function set not present, return null
}

/*! Adds the host connection to the hosts list.
	
	@param type is the kind of database server: 'mysql', 'postgresql', etc.
	@param ip is the IP address or URL of the host.
	@param port is the Port number of the host.
	@param user is the login username.
	@param pass is the login password.
	@param dataRef
	@return 0 if success, else an error number if there is a input format problem.
*/
int npdbAddHost( char* type, char* ip, int port, char* user, char* pass, void* dataRef)	//zzd
{
	int i = 0;
	pData data = (pData) dataRef;

	pNPdbs dbs = &data->io.db;
	pNPdbHost host = NULL;

	/// @todo add err and default handling for all function input parameters
	if( !dbs->hosts )
	{
		printf( "err 5577 - db->hosts is NULL \n" );
		return 0;
	}

//	printf("\nhostCount : %d", dbs->hostCount);
	if(dbs->hostCount == 1) // First host is place holder
	{
		//begin npInitDB() // Bring under npInitDb Func
	/*	
		dbs->activeDB                    = malloc(sizeof(NPdatabase) * 1);
		dbs->activeDB->host              = malloc(sizeof(NPdbHost));
		dbs->activeDB->host->hostFuncSet = malloc(sizeof(NPdbFuncSet));
	*/	
		dbs->activeDB					 = npMalloc(0, sizeof(NPdatabase), dataRef);
		dbs->activeDB->host				 = npMalloc(0, sizeof(NPdbHost), dataRef);
		dbs->activeDB->host->hostFuncSet = npMalloc(0, sizeof(NPdbFuncSet), dataRef);
		/*
		dbs->activeDB = npdbMalloc(dbs->activeDB, dbs, dataRef);
		dbs->activeDB->host = npdbMalloc(dbs->activeDB->host, dbs, dataRef);
		dbs->activeDB->host->hostFuncSet = npdbMalloc(dbs->activeDB->host->hostFuncSet, dbs, dataRef);
		npdbPRC(dbs);
		*/
		/// @todo upgrade idMap to map from DB node_id to scene node ptr, fasted updates
		/// @todo support changes to scene graph structure when updating DB
		/// @todo add kNPnodeList type for npMalloc
		
		//dbs->activeDB[0].idMap = malloc( sizeof(int) * kNPnodeMax ); // old, lde
		dbs->activeDB->idMap = malloc( sizeof(int) * kNPnodeMax );
		//if( !dbs->activeDB[0].idMap ) return 1010; // old, lde
		if( !dbs->activeDB->idMap ) return 1010;
		
		for(i = 0; i < kNPnodeMax; i++)
			dbs->activeDB->idMap[i] = -1;
		
		/// @todo copy the structures rather then just point to existing mem
		
		strcpy(dbs->activeDB->host->ip, ip);
		strcpy(dbs->activeDB->host->user, user);
		strcpy(dbs->activeDB->host->password, pass);
		strcpy(dbs->activeDB->host->inUseDB, "none");
		dbs->activeDB->host->conn_id = 0;
		dbs->activeDB->host->connected = false;
		printf("\nhost not connected");
		dbs->dbCount++; // dbCount should really be server count, lde
		
	}
	else
	{
		//! @todo add support for multiple databases
		// dbs->activeDB = realloc
		printf("\nerr 9494 - MySQL currently only supports 1 DB\n");
	}
	
	/// if host already in list then set local host pointer to existing item
	for( i=0; i < dbs->hostCount; i++ )
	{
		if( dbs->hosts[i] != NULL )
			if( strcmp( ip, dbs->hosts[i]->ip ) == 0 )
			{
				host = dbs->hosts[i];
				break;
			}
	}
	/// else create a new host entry, unless kNPdbHostMax hit
	if( !host )
	{
		if( dbs->hostCount >= kNPdbHostMax )
		{
			printf( "err 5548 - can't add host, kNPdbHostMax hit \n" );
			return 0;
		}
		if( dbs->hosts[dbs->hostCount] != NULL )
		{
			printf( "err 5549 - corrupted db->hosts list \n" );
			return 0;
		}
		host = dbs->hosts[dbs->hostCount] = npInitHostDB();
		host->id = dbs->hostCount;
		host->hostFuncSet = npdbGetHostFuncSet( type, dbs );
	}

	printf("4");
	host->id = dbs->hostCount;
	strcpy( host->type, type );
	strcpy( host->ip, ip );
	host->port = port;
	strcpy( host->user, user );
	strcpy( host->password, pass );

	/// add our new host to the hosts list and increment hostCount
	dbs->hosts[dbs->hostCount++] = host;

	printf("5");
	return host->id;
	/// @todo upgrade idMap to map from DB node_id to scene node ptr, fasted updates
	/// @todo support changes to scene graph structure when updating DB
	/// @todo add kNPnodeList type for npMalloc
}


pNPdbFuncSet npdbNewFuncSet( pNPdbs db )
{
	pNPdbFuncSet funcSet = NULL;

	if( db->funcSetCount >= kNPdbFuncSetMax )
	{
		printf("err 5523 - kNPdbFuncSet max hit");
		return NULL;
	}

	/// note that we use calloc so that our function pointers are null
	funcSet = (pNPdbFuncSet)calloc( 1, sizeof(NPdbFuncSet) );
	if( !funcSet )
	{
		printf("err 5524 - malloc failed NPdbFuncSet");
		return NULL;
	}

	funcSet->id = db->funcSetCount;		///< set our id based on array index
	funcSet->hostType[0] = '\0';		///< null terminate the hostType str

	funcSet->size = sizeof(NPdbFuncSet);

	return funcSet;
}

/*! Check if host, connection and function set are valid.

	@param host pointer to an element in the hosts list to validate.
	@return 0 for no err: host, connection and host functions are valid.
			Otherwise post err and return err number.
*/
int npdbHostErr( pNPdbHost host )
{
	int err = 0;	///< return 0 if host, hostFuncSet and conn are valid

	if( !host )
		{ printf("err 5502 - null host \n" ); return 5502; }

	if( !host->hostFuncSet )
		{ printf("err 5503 - null host function set \n" ); return 5504; }

	if( !host->conn )
		{ printf("err 5504 - null host connection \n" ); return 5503; }

	return err;
}

/*! Ascerts that the database item is not null and host connection is valid.

	@param dbItem is a pointer to the database item to err check.
	@param return 0 if valid database and connection, else an err number.
*/
// lde, @todo
int npdbItemErr( pNPdatabase dbItem )
{
//	printf("\nnpdbItemErr");
	
	if( !dbItem )
	{
		printf( "err 5581 - null dbItem \n" );
		return 5581;
	}

	if( !dbItem->host )
	{
		printf( "err 5582 - database: %s has null host \n", dbItem->name );
		return 5582;
	}

	if( !dbItem->host->conn )
	{
		printf( "err 5583 - not connected to host: %s\n", dbItem->host->ip );
		return 5583;
	}

	if( !dbItem->host->hostFuncSet )
	{
		printf( "err 5584 - missing funcSet host: %s\n", dbItem->host->ip );
		return 5584;
	}

	return 0;
}


//zzd
/*
		/// create a new map list an reference
		void npFreeMapID( pNPmapIDtoPtr mapID );
		void* npNewMapID( int mapSize, int mapType, void* dataRef );
		void npInitMapID( pNPmapIDtoPtr mapID );

		/// Create new mapID list if null, otherwise re-allocate and initialize.
		if( !mapID )	/// if no mapID then create a newly initialized one
			pNPmapNodeID mapID = npNewMapID( mapSize, kNPmapIDtoPtr, dataRef );
		else
		{
			if( mapSize == mapID->size )
				npInitMapID( mapID )	///< just re-init mapID if same size
			else
			{	/// free mapID resources and references
				npFreeMapID( mapID );

				/// get a new mapID structure, npNewMapID() calls npInitMapID()
				pNPmapID mapID = npNewMapID( mapSize, kNPmapIDtoPtr, dataRef );
		}	}
*/			

		/// call before updating from DB to sync scene node changes
		/// such as new, deleted or moved (branch) nodes.
		/// npUpdateMapList( mapListRef, dataRef);

/*		for(i = 0; i < kNPnodeMax; i++)		///< @todo replace kNPnodeMax with node count
		{
			mapID[i]->id = 0;
			mapID[i]->node = NULL;
		}
*/

 // lde @todo
/*
double npGetTime(pNPos os, void* (*function)(), ...)
{
	double diff  = 0;
	double start = 0;
	double end   = 0;
	pNPosFuncSet func = os->funcSet;
	
	start = (double)func->getTime();
//  function();
	end   = (double)func->getTime();
	
	diff = end - start;
	
	return diff;
}
 */


double npGetTime(double start, void* function, double end)
{
	double diff = 0;
	diff = end - start;
	return diff;
}

int new_npdbLoadTags ( pNPdbFuncSet func, void* result, void* dataRef )
{
	pData data = (pData) dataRef;
	pNPtags tags = &data->io.gl.hud.tags;
	pNPrecordSet recordSet = NULL;			// list of records as native C data
	char** row = NULL;						///< char** to store MYSQL_ROW handle;
	int totalCount = 0;
	int rowCount = 0;
	int count = 0;
	int err = 0;
	
	int format = 0;
	int type = 0;
	int rowNum = 0;
	
	char* buffer = NULL;
	
	rowCount = npdbNumRows_safe(result, func, &err);
	if(err) return err;
	
	buffer = malloc( sizeof(char) * (rowCount * 150) ); // kNPmaxFile 4096 // arbritary, lde @todo // make this dynamic, lde @todo
	if(buffer == NULL) return 1237; // add error code, lde @todo
	
	char* read = buffer;
	
	buffer[0]  = '\0';
	
	recordSet = npMalloc(0, sizeof(NPrecordSet), data);
	if(!recordSet) return 2345; // set err , lde @todo
	
	count = sprintf(read+count, "%s\n", "np_tag_id,record_id,table_id,title,description");
	read += count;
//	buffer = read;
	totalCount += count;
	while( row = func->fetch_row(result) ) // enfp, lde @todo // make into npdbReadTag, lde @todo
	{
		rowNum++;
		if( row == NULL && rowNum < rowCount )
		{
			printf("\nrow is NULL\n");
			return 8372; // add error code, lde @todo
		}
		 
		count = sprintf(read, "%s,%s,%s,\"%s\",\"%s\"\r\n", row[0], row[1], row[2], row[3], row[4]);
//		printf("\nread : %s\n", read);
		read += count;
		totalCount += count;
		//	npAddTa
	}
	
	buffer = npGetType(&format, &type, buffer, totalCount, data);
	recordSet->type = type;
	
	// Iterate to next line, lde @todo
	
	npPreLoadInit(recordSet, data);
//	buffer = npNextLine(buffer);
//	printf("\nBuffer : %s\n", buffer);
	
	totalCount = strlen(buffer);
	npCSVtoC(recordSet, buffer, totalCount, data);
	free( recordSet );
	
	return 0;
}

int npdbLoadTags ( pNPdbFuncSet func, void* result, void* dataRef )
{
	time_t start = (int)nposGetTime();
	//	double start = nposGetTime();
		pData data = (pData) dataRef;
	int nodeCount = 0;
	char** row = NULL;		///< char** to store MYSQL_ROW handle;
	int x = 0;
	//	int numFields = 0; // Warning, lde
	//	int rowCount = 0; // Warning, lde
	//	int count = 0; // Warning, lde
	pNPtags tags = &data->io.gl.hud.tags;
	int recordCount = 0;
	
	int totalCount = 0;
	int id = 0;
	int record_id = 0;
	int table_id = 0;
	char* desc = NULL;
	char* title = NULL;
	
	int count = 0;
	int type = 0;
	int branchLevel = 0;
	int parentID = 0;
	double end  = 0;
	double diff = 0;
	int err = 0;
	char* buffer = malloc( sizeof(char) * 4096); // kNPmaxFile 4096
	char* read = buffer;
	
	buffer[0] = '\0';
	//	pData data = (pData) dataRef; // Warning, lde
	
	pNPnode node = NULL;
	//	pNPtable table = NULL;
	//	pNPmapID map = NULL; // Warning, lde
	
	
	//numFields = mysql_num_fields(result);
	//	table = npdbGetTable( "node_tbl", dbItem );
	
	//count = sprintf(buffer+count, "%s\n", "np_tag_id,record_id,table_id,title,description");
	while( row = func->fetch_row(result) ) // enfp, lde @todo
	{
		x++;
		/*
		id        = npatoi(row[0]);
		record_id = npatoi(row[1]);
		table_id  = npatoi(row[2]);
		title     = row[3];
		desc      = row[4];
		*/
		printf("\n%s", row[0]);
		printf("\n%s", row[1]);
		printf("\n%s", row[2]);
		printf("\n%s", row[3]);
		printf("\n%s", row[4]);
		
		count = sprintf(read, "%s,%s,%s,\"%s\",\"%s\"\r\n", row[0], row[1], row[2], row[3], row[4]);
		printf("\nbuffer : %s\n", read);
		read += count;
		totalCount += count;
	//	npAddTa
	}
	//buffer[count-1] = '\0';
	printf("\nbuffer : %s\n", buffer);
	
//	recordCount = npLoadTags(buffer, count, dataRef);
	recordCount = npLoadTags(buffer, totalCount, dataRef);
	tags->recordCount = recordCount;
	
	free(buffer);
//	npSyncChMap (data);
//	npSyncTags(dataRef);

	
	return err;
}

int npdbTableRowsToNodes() // lde @todo
{

	
}

int npdbLoadNodes( pNPdbFuncSet func, void* result, void* dataRef ) // npdbLoadNodesFromTable , lde @todo
{
	time_t start = (int)nposGetTime();
//	double start = nposGetTime();
	
	int nodeCount = 0;
	char** row = NULL;		///< char** to store MYSQL_ROW handle;
	int x = 0;
//	int numFields = 0; // Warning, lde
//	int rowCount = 0; // Warning, lde
//	int count = 0; // Warning, lde

	int id = 0;
	int type = 0;
	int branchLevel = 0;
	int parentID = 0;
	double end  = 0;
	double diff = 0;

//	pData data = (pData) dataRef; // Warning, lde
	
	pNPnode node = NULL;
//	pNPtable table = NULL;
//	pNPmapID map = NULL; // Warning, lde
	

	//numFields = mysql_num_fields(result);
	//	table = npdbGetTable( "node_tbl", dbItem );

	while( row = func->fetch_row(result) )
	{
		x++;
		id = npatoi(row[0]);
		type = npatoi(row[1]);
		
		/*
		 //workaround for keeping track of link end B, processed by orphan list
		 if (node->type == kNodeLink)
		 node->childIndex = childID;
		 */	
		
		branchLevel = npatoi(row[5]);
		parentID = npatoi(row[4]);
		//	node = npAddNode( id, parentID, data );
			//	mapID[x]->tableID = id;
			//	mapID[x]->nodeID = node->id; //or mapID[x][1]

		if( 0 )// update )
		{
			// node = npGetNodeByMapID( id, map, data );
			/// two kinds of updates with and without structural changes
			// if( !node )
			//	if( updateCreate )
			//		npMapNodeAdd();//else ignore row, do not assign properties
		}
		else
			node = npMapNodeAdd (id, type, branchLevel, parentID, kNPmapNodeCSV, dataRef); // kNPmapNodeCSVvTwo is format		
	
		if( !node )
			printf("err 5538 - npMapNodeAdd returned null node id: %d \n", id);
		else
		{		
			//data->io.dbs->activeDB[0].idMap[id] = node->id;			
			npdbRowToNode( node, row );
			//npAddNodeMapID( map, id, node, data );
			nodeCount++;

			if( (nodeCount % 10000) == 0 )	/// print '+' every 10k nodes
				printf("+");
		}
	}
	printf("\n");
	//Process Orphans...

	
//	printf("parse time: %.0f secs\n", difftime((int)nposGetTime(), start)); // This was causing a single node not be loaded back in, lde
	
	//The timing for this function should be done from outside, lde @ todo
	end = (double)nposGetTime();
	diff = end - start;
//	printf("parse time: %d secs\n", diff);
	//	//zzdb printf("\nNumber of nodes added : %d", x);
	
	
	return nodeCount;
}

/* lde, @todo
 npdbGetFields retrieves the char* fields element of the pNPdbTable struct, lde
 */
int npdbGetFields ( pNPdbTable tbl, void* dataRef)
{

	return 0;
}


/*
 struct NPdbFields{
 char*		name;		///< field name
 int*		type;		///< field type constant
 char*		typeStr;	///< field type as DB string format
 char*		params;		///< any additional field params, limits, etc.
 int		count;		///< number of field columns
 };
 */

int npdbSetField (pNPdbFields field, pNPdbHost host, void* result, void* dataRef)
{
	char** row    = NULL;
	int    err   = 0;
/*
	char* name    = NULL; // Warning, lde
	char* typeStr = NULL; // Warning, lde
	int i = 0; // Warning, lde
*/	
	unsigned long* fieldLengths = NULL;
	
	
//	void* conn = host->conn; // Warning, lde
	pNPdbFuncSet func = host->hostFuncSet;
	
	//tbl->fields[index] = malloc(sizeof(NPdbFields));
	//field = malloc(sizeof(NPdbFields));
	
	row = func->fetch_row(result);
	
	if( (fieldLengths = func->fetch_lengths(result)) == 0 )
	{
		printf("err 5568 - DB function fetch_lengths failed \n" );
		func->free_result( result );
		return 5568;
	}
	
	//tbl->fieldCount++;
//	tbl->fields[index] = malloc(sizeof(NPdbFields));
	
//	name    = field->name;
//	typeStr = field->typeStr;

	field->name    = malloc(sizeof(char) * ( fieldLengths[0] + 1 ) );
	field->typeStr = malloc(sizeof(char) * ( fieldLengths[1] + 1 ) );
	field->params  = malloc(sizeof(char) * 1);
	field->type    = malloc(sizeof(int)  * 1);
	field->count   = 2;
	
	if(field->name == NULL || field->typeStr == NULL || field->params == NULL || field->type == NULL)
	{
		printf("\nMALLOC FAILED\n");
		err = 1283;
		return err;
	}
		
	field->name    = strncpy(field->name,    row[0], ( fieldLengths[0] + 1 ) );
	field->typeStr = strncpy(field->typeStr, row[1], ( fieldLengths[1] + 1 ));
	
//	field->name[fieldLengths[0]] = '\0';
//	field->typeStr[fieldLengths[1]] = '\0';
	
	printf("\n %p Name : %s ::: %p typeStr : %s", field->name, field->name, field->typeStr, field->typeStr);
	
	return err;
}


int npdbSetFields ( pNPdbTable tbl, pNPdbHost host, void* dataRef) // fieldLength validation should be variable, lde
{
	pNPdbFuncSet func = host->hostFuncSet;
	
//	void* conn = host->conn; // Warning, lde
	void* result = NULL;
	
	int numFields = 0, numRows = 0, i = 0;
	int err = 0;
//	unsigned long* fieldLengths; // Warning, lde
	
//	char** row = NULL;		///< char** to store MYSQL_ROW handle; // Warning, lde
	
	/// store the result
	result = npdbStoreResult_Safe(func, host, &err);
	if( err ) return err;
	 
	/// get the number of fields, should be 6 for list of fields , lde
	numFields = npdbNumFields_safe(result, func, 6, &err);
	if( err ) return err;
	
	/// get the number of rows in result
	numRows = npdbNumRows_safe(result, func, &err);
	if( err ) return err;
	
	//numRows = (int)func->num_rows(result);
	tbl->fieldCount = numRows;
	
	//tbl->fields = malloc(sizeof(pNPdbFields) * tbl->fieldCount);
	
	for( i = 0; i < tbl->fieldCount; i++)
	{
		tbl->fields[i] = malloc(sizeof(NPdbFields));
		if(tbl->fields[i] == NULL)
		{
			printf("failed to allocate tbl->fields[%d]", i);
			func->free_result( result );
			return 8730; // add error code, lde @todo
		}
	}
	
	/// store the database count for this host
	//host->dbCount = numRows;
	printf( "host: %.32s  fields: %d\n", host->ip, numRows );
	
	for(i = 0; i < numRows; i++)
	{
		err = npdbSetField(tbl->fields[i], host, result, dataRef);
		if( err ) return err; 
	}
	 
	printf("\n");
	
	func->free_result( result );
	return 0;					// success
}

/* lde, @todo
 npdbShowFields queries the database table for a table's fields
 */
int npdbShowFields ( pNPdbHost host, pNPdbTable tbl, void* dataRef)
{
	int err = 0;
	char* statement = NULL;
	void* conn = NULL;
	pNPdbFuncSet func = NULL;
	
	
	/// ascert valid host, connection and function set
	if( err = npdbHostErr(host)) return err;
	
	func = host->hostFuncSet;
	conn = host->conn;
	
	/// create our SHOW FIELDS from [table] statement and query it
	statement = func->StatementShow( "FIELDS" );
	sprintf(statement, "%s FROM %s", statement, tbl->name); 
	if( !statement ) return 5575;	//err 5576
	
	/// post and execute our query
	printf( "host: %.24s  query: %s\n", host->ip, statement );
	err = npdbQuery_safe(conn, func, host, statement);
	if(err)
	{
		printf("\nShow Fields query failed");
		return err;
	}
	
	free( statement );
	return err;
}

void* npdbGetFieldList(pNPdbTable tbl, int* err, void* dataRef)
{
	char* fields = NULL;
	int count = 0;
	int i     = 0;
	fields = malloc(sizeof(char) * kNPmaxFields);
	err = (int*)0;
//	printf("\nerr : %d", (int*)err);
	
	if(fields == NULL)
	{
		err = (int*)1234; // find proper error code, lde @todo
		return fields;
	}
	
	for(i = 0;i < tbl->fieldCount; i++)
	{
		count += sprintf(fields+count, "%s %s,", tbl->fields[i]->name, tbl->fields[i]->typeStr);
	}
	
	fields[count-1] = '\0';
	//printf("\nfields : %s\n", fields);

	return fields;
}


pNPdbTable npdbFindTagTbl( pNPdatabase db, int* err, void* dataRef)
{
	char* tag_tbl_fields = NULL;
	char* test_tbl_fields = NULL;
	int i = 0;
	err = (int*)0;
		
	printf("\nnpdbFindTagTbl\n");
	//node_tbl_fields = npMysqlGetTableFields(kNPnode, dataRef); // Make this npdbGetTableFields
	tag_tbl_fields = db->host->hostFuncSet->getTableFields(kNPtag, dataRef); // shorten, lde @todo
	printf("\ntag_tbl_fields : %s\n", tag_tbl_fields);
	
	if(tag_tbl_fields == NULL)
	{
		//return (void*)8371; // add error code, lde @todo // npdbFindNodeTbl should have int* err as a parameter, lde @todo
		err = (int*)8371;
		return NULL;
	}
	
	
	for(i = 0; i < db->tableCount; i++)
	{
		test_tbl_fields = npdbGetFieldList(db->tableList[i], err ,dataRef);
		if(err)
		{
			printf("\nnpdbGetFieldList failed");
			return NULL;
		}
		
	//	 Clean up and encompass This into a "toggled" function pointer, lde
/*
		printf("\n--------------------------------------------------------\n");
		 printf("\ntest_tbl_fields : %s\n", test_tbl_fields);
		 printf("\ntag_tbl_fields : %s\n", tag_tbl_fields);
		 printf("\n--------------------------------------------------------\n");
		 printf("\nstrlen of node_tbl_fields : %d\n", strlen(tag_tbl_fields));
		 printf("\nRESULT : %d\n", strcasecmp(tag_tbl_fields, test_tbl_fields));
		 printf("\nstrlen of test_tbl_fields : %d\n", strlen(tag_tbl_fields));
*/		 
		
		if(strcasecmp(tag_tbl_fields, test_tbl_fields) == 0)
		{
			printf("\nFound It\n");
			return db->tableList[i];
		}
	}
	
	// npdbGetFieldList(
	
	//printf("\nFields are : %s\n", fields);
	err = (int*)8372; // add error code, lde @todo
	return 0;
}

pNPdbTable npdbFindNodeTbl( pNPdatabase db, int* err, void* dataRef)
{
	char* node_tbl_fields = NULL;
	char* test_tbl_fields = NULL;
	int i = 0;
	err = (int*)0;
	
	printf("\nnpdbFindNodeTbl\n");
	//node_tbl_fields = npMysqlGetTableFields(kNPnode, dataRef); // Make this npdbGetTableFields
	node_tbl_fields = db->host->hostFuncSet->getTableFields(kNPnode, dataRef); // shorten, lde @todo
	printf("\nnode_tbl_fields : %s\n", node_tbl_fields);
	
	if(node_tbl_fields == NULL)
	{
		//return (void*)8371; // add error code, lde @todo // npdbFindNodeTbl should have int* err as a parameter, lde @todo
		err = (int*)8371;
		return NULL;
	}
	
	 
	for(i = 0; i < db->tableCount; i++)
	{
		test_tbl_fields = npdbGetFieldList(db->tableList[i], err ,dataRef);
		if(err)
		{
			printf("\nnpdbGetFieldList failed");
			return NULL;
		}
		
		/* Clean up and encompass This into a "toggled" function pointer, lde
		printf("\n--------------------------------------------------------\n");
		printf("\ntest_tbl_fields : %s\n", test_tbl_fields);
		printf("\nnode_tbl_fields : %s\n", node_tbl_fields);
		printf("\n--------------------------------------------------------\n");
		printf("\nstrlen of node_tbl_fields : %d\n", strlen(node_tbl_fields));
		printf("\nRESULT : %d\n", strcasecmp(node_tbl_fields, test_tbl_fields));
		printf("\nstrlen of test_tbl_fields : %d\n", strlen(test_tbl_fields));
		*/
		
		if(strcmp(node_tbl_fields, test_tbl_fields) == 0)
		{
			printf("\nFound It\n");
			return db->tableList[i];
		}
	}
	
	// npdbGetFieldList(
			
	//printf("\nFields are : %s\n", fields);
	return 0;
}

int npdbAddField(pNPdbTable tbl, char* name, char* typeStr)
{
	tbl->fields[tbl->fieldCount] = malloc(sizeof(NPdbFields));
	printf("\n%s :: %s", name, typeStr);
	strcpy(tbl->fields[tbl->fieldCount]->name, name);
	strcpy(tbl->fields[tbl->fieldCount]->typeStr, typeStr);
	//tbl->fieldCount++;
	return 0;
}

int npdbFetchRow( pNPdbFuncSet func, pNPdbTable tbl )
{
	void* result;
	char** row = NULL;
/*
	char* name    = NULL; // Warning, lde
	char* typeStr = NULL; // Warning, lde
*/
	unsigned long *fieldLengths = 0;
	
	row = func->fetch_row(result);
	
	/// get the table name string length, string is not terminated!
	if( (fieldLengths = func->fetch_lengths(result)) == 0 )
	{
		printf("err 5568 - DB function fetch_lengths failed \n" );
		func->free_result( result );
		return 5568;
	}
	
	// Start, lde, all this could be put into a function @todo npdbAddField
	// @todo when the table structure gets created initialize fieldCount to 0 , lde
	//name = row[0];
	printf("\n");
	
	return 0;
}

void* npdbFetchTableName( pNPdbFuncSet func, void* result, pNPdbTable tbl)
{
	char** row = NULL;
	char* name = NULL;
	int    err = 0;
/*
	char* typeStr = NULL; // Warning, lde
	int   err     = 0;    // Warning, lde
*/
//	unsigned long *fieldLengths = 0;
	
	row = npdbFetchRow_safe(result, func, &err);
//	row = func->fetch_row(result);
	
	// @todo when the table structure gets created initialize fieldCount to 0 , lde
	name = row[0];
	printf("\nnpdbFetchTableName : Table Name : %s", name);
	printf("\n");
	
	return name;
}

int npdbSetTable( pNPdbFuncSet func, void* result, pNPdbTable tbl, pNPdbHost host, void* dataRef )
{
	char* name = NULL;
	int   err  = 0;
//	tbl = malloc(sizeof(NPdbTable));
	
	if(tbl == NULL)
		return 1234; 
	
	name = npdbFetchTableName(func, result, tbl);
	if((int)name == 5568) // temporary, lde @todo
	{
		return 5568;
	}
	
	tbl->name = malloc(sizeof(char) * strlen(name));
	if(tbl->name == NULL)
		return 5432; // add err code, lde
	
	strcpy(tbl->name, name);
	
	// Next query fields
	err = npdbShowFields(host, tbl, dataRef);
	if(err != 0)
		return err;
	
	err = npdbSetFields(tbl, host, dataRef);
	if(err != 0)
	{
		return err;
	}
		
	printf("\ntable addresss : %p\n", tbl);
	
	return err;
}

void* npdbStoreResult_Safe(pNPdbFuncSet func, pNPdbHost host, int* err) 
{
	void* result = NULL;
	void* conn = host->conn;
	err = (int*)0;
	
	result = func->store_result(conn);

	if(result == NULL)
	{
		printf("err 5565 - npdbSetFields failed to store result \n" ); // Make this failed to store result, lde
		printf( "%s err: %u - %s\n", host->type,
			   (unsigned int)func->db_errno( conn ), (char*)func->db_error( conn ) );
		func->free_result( result ); // If store_result fails does result really need to be freed, lde
		err = (int*)5565;
	}
	
	return result;
}

int npdbNumFields_safe(void* result, pNPdbFuncSet func, int rightNum, int* err )
{
	int numFields = 0;
	numFields = (int)func->num_fields(result);
	err = (int*)0;
	
	if( numFields != rightNum )
	{
		printf( "err 5566 - numFields != %d \n", rightNum );
		func->free_result( result );
		err = (int*)5566;
	}

	return numFields;
}

int npdbNumRows_safe(void* result, pNPdbFuncSet func, int* err)
{
	int numRows = (int)func->num_rows(result);
	err = (int*)0;
	
	if( !numRows )
	{
		printf( "err 5567 - npdbGetList numRows < 1 \n" );
		func->free_result( result );
		err = (int*)5567;
	}
	
	return numRows;
}

void npdbFreeField(pNPdbFields field) // This should return void, not int, lde @todo // DONE
{
	if(field->name != NULL)
	{
		//printf(" 1 : %s ", field->name);
		free(field->name); 
	}
	
	if(field->typeStr != NULL)
	{
		//printf(" 2 : %s", field->typeStr);
		free(field->typeStr);
	}
	
	if(field->params != NULL);
	{
		//printf(" 3 : %s ", field->params);
		free(field->params);
	}
		
	if(field->type != NULL)
	{
		//printf(" 4 : %d ", field->type);
		free(field->type);
	}
	
	//printf(" 5 ");
	free(field);
	//printf(" 6 ");
	return;
}

void npdbFreeFields(pNPdbTable tbl)
{
	int i = 0;
	for(i = 0; i < tbl->fieldCount; i++)
	{
		//printf("\nField %d", i);
		npdbFreeField(tbl->fields[i]);
	}
		
	return;
}

void npdbFreeTable(pNPdbTable tbl)
{
	npdbFreeFields(tbl);
	
	if(tbl->name != NULL)
		free(tbl->name);
	
	free(tbl);
	
	return;
}

void npdbFreeTables(pNPdatabase db)
{
	//printf("\nnpdbFreeTables : %d ", db->tableCount);
	int i = 0;
	for( i = 0; i < db->tableCount; i++ )
	{
		//printf("\nTable %d", i);
		npdbFreeTable(db->tableList[i]);
	}
		
	return;
}

int npdbSetTables(pNPdbHost host, pNPdatabase db)
{
	pNPdbFuncSet func = host->hostFuncSet;
//	pNPdbTable *tbl = NULL;
//	void* conn = host->conn; // Warning, lde
	void* result = NULL;
	int numFields = 0, numRows = 0, numTables = 0 ,i = 0;
	int err = 0;
	/*
	char** row = NULL;
	char* name = NULL;
	char* typeStr = NULL;
	unsigned long* fieldLengths = 0;
	*/
	
	/* the void* result should be within the pNPdbHost host struct, lde @todo */
	/* unless this causes problems with thread safety */
	
	printf("\nResult ptr : %p\n", &result);
	/// store the result
	err = 0;
	result = npdbStoreResult_Safe(func, host, &err);
	if( err ) return err;
	
	/// get the number of fields, should be 1 for list of tables , lde
	numFields = npdbNumFields_safe(result, func, 1, &err);
	if( err ) return err; // npdbErr , ?? lde @todo
	
	/// get the number of rows in result
	numRows = npdbNumRows_safe(result, func, &err);
	if( err ) return err;

	numTables = numRows;
	db->tableCount = numTables;
	
	for( i = 0; i < numRows; i++)
	{
		printf("\ndb->tableList[i] ptr : %p", db->tableList[i]);
		db->tableList[i] = malloc(sizeof(NPdbTable)); // check to make sure this is successful , lde @todo
		
		if(db->tableList[i] == NULL)
		{
			printf("\nMalloc Failed");
			// set new table count and return, lde @todo
			return 2893; // add err code
		}
		printf("\ndb->tableList[i] ptr : %p", db->tableList[i]);
	}
	
	/// store the table count for this host
	printf( "host: %.32s  tables: %d\n", host->ip, numRows );
	
	for( i=0; i < numTables; i++ )
	{
		err = npdbSetTable(func, result, db->tableList[i], host, NULL);
		if(err != 0)
		{
			printf("Error Occurred, freeing result");
			func->free_result( result );
			return 9036; // add error code, lde @todo
		}
		
		db->tableList[i]->owner = db;
	}
	printf("\n");
	
	func->free_result( result );
	return 0;					// success
	
}

int npdbGetTbls(pNPdbHost host, pNPdatabase db)
{	
	int err = 0;
	err = npdbShowTables(host);
	if(err) return err;
	
	err = npdbSetTables(host, db);
	if(err)
	{
		printf("\nnpdbSetTables failed");
		return err;
	}
	
//	printf("\nnpdbGetTbls\ntable address : %p\n", db->tableList[0]);
	
	return err;
}

// This might have better performance with a lookup table, lde
// Finish, @todo lde
int npdbErrMysqlToAntz(int err) // @todo lde, add to header file when finished
{
	switch (err) {
		case 2000: // Unknown error
			return 1111; 
			break;
		case 2001: // Can't create unix socket
			return 2222;
			break;
		case 2002: // Can't connect to local MySQL server through socket '%s' (%d) 
			return 3333;
			break;
		case 2003: // Can't connect to MySQL server on '%s' (%d) 
			return 4444;
			break;
		case 2004: // Can't create TCP/IP socket (%d) 
			return 5555;
			break;
		case 2005: // Unknown MySQL server host '%s' (%d) 
			return 6666;
			break;
		case 2006: // MySQL server has gone away 
			return 7777;
			break;
		case 2007: // Protocol mismatch; server version = %d, client version = %d 
			return 8888;
			break; 
		/* @todo, lde 
		   More error codes exist, add more
		 */
		case 2013: // Server lost
			return 5432; // make error code for all these, todo lde
			break;

		default:
			printf("Error code not added yet");
			return 0101;
			break;
	}
	
	return 0;
}

// @todo, lde, finish writing this function
char** npdbFetchRow_safe(void* result, pNPdbFuncSet func, int* err)
{
	char** row = NULL;
	err = 0;
	row = func->fetch_row(result);
	if( row == NULL)
	{
		err = (int*)func->db_errno(result); // Now translate what is returned into an antz error code
		err = (int*)9999;
	}
	
	return row;
}

// Function malfunctioning, lde @todo
int npdbGetCSVrowFromDB(pNPdbFuncSet func, void* result, int numFields, char* csvRow) // new, lde
{
	char** row;
	int i      = 0;
	int count  = 0;
	int err    = 0;
	
	
	row = npdbFetchRow_safe(result, func, &err);
	if(row == NULL) // Either there are no more rows or an error occurred
	{
		err = (int)func->db_errno(result); 
		//err = -1;
		return err;
	}
		
	for( i = 0; i < numFields; i++ )
	{
		count += sprintf(csvRow+count, "%s,", row[i]);
	}
	
	csvRow[count-1] = '\0';
	//printf("\ncsvRow : %s\n", csvRow);
	
	count = 0;
	
	return err;
}


// Do this if the csv file is small, lde @todo later
int npdbGetCSVrowsFromDB(pNPdbFuncSet func, void* result, int numFields, char* csvRow) // add to header, lde
{
	while( npdbGetCSVrowFromDB(func, result, numFields, csvRow) != -1 )
	{
		// Write csvRow out to file <--- This doesn't belong here, lde, at least not within THIS function
		  // npWriteToFile // or something to that effect
		// Now zero it out
		csvRow[0] = '\0';
	}
	
	return 0;
}

void npdbFreeFieldList(char* fieldList) // lde, @todo add to header file
{
	free(fieldList);
}

void* npdbMalloc(void* memory, pNPdatabase db, void* dataRef)
{
	memory = npMalloc(0, sizeof(memory), dataRef);
	if( memory != NULL )
		db->refCount++;	
	
	return memory;
}

void npdbFree(void* memory, pNPdatabase db, void* dataRef)
{
	npFree(memory, dataRef);
	db->refCount--;
}

void npdbPRC(pNPdatabase db)
{
	printf("\nReference Count : %d", db->refCount);
}

/*
int npdbTableToCSVthread(void* threadData)
{
	pNPdbCSVwrite theThreadData = threadData;
	char* csvName    = theThreadData->csvName;
	printf("\ncsvName : %s", csvName);
	pNPdbTable table = theThreadData->table;
	printf("\ntable : %p", table);
	void* dataRef    = theThreadData->dataRef; 
	printf("\ndataRef : %p", dataRef);
	int err = 0;
	npdbTableToCSV(table, csvName, dataRef);
	theThreadData->running = 0;
//	npdbFreeTables(table->owner);
	free(threadData);
	
	return err;
}
*/

// Experimental, lde @todo
pNPdbReservedWord npdbAddReservedWord(char* word, int type)
{
	pNPdbReservedWord reservedWord = NULL;
	reservedWord = malloc(sizeof(NPdbReservedWord));
	if( reservedWord == NULL)
		return NULL;
	
	strcpy(reservedWord->word, word);
	reservedWord->type = type;
	
	return reservedWord;
}

// Experimental, lde @todo
void npdbDeleteKeyword(pNPdbReservedWord reservedWord)
{
	free(reservedWord);
	return;
}

char** npdbBuildQuery(char* item, int* num)
{
	// Search keywords for item (item might be "Select")
	
	// When found, add keyword->word to query string
	
	// Then return an array of keywords that can be applied afterward
//	item = tolower(item);
	char** newPtr;
//	int i = 0;
	
	if( strcmp(item, "create") == 0 )
	{
		/*
		newPtr    = malloc( sizeof(char**) * 2 );
		newPtr[0] = strdup("database");
		newPtr[1] = strdup("table");
		*/
		// return the npdbBuildCreateQuery function and the "potential" arguments
		// For example, npdbBuildCreateQuery is returned and a char** with "database" and "table" is returned
		
		return newPtr;
	}
	
	if( strcmp(item, "database") == 0)
	{
		// Grab from db list and return
	}
	
	if( strcmp(item, "table") == 0 )
	{
		
	}
	
	return newPtr;
}

void npdbBuildCreateQuery(char* type) // type being database or table or etc..., lde @todo
{
	
	
	return;
}

int npdbTableToCSV(pNPdbTable table, char* csvName, void* dataRef) // Put in header file, lde
{
	char*          fieldList    = NULL;
	char*		   headerBuffer	= NULL;
	char			   fileName[150] = {'\0'}; // 150 is arbritary use kNPmaxFileSize , lde @todo
	char			   csvRow[421]  = {'\0'}; // Revise, lde // changed to 800 from 421, only temp @todo
	void*          result       = NULL;
	int            err	        = 0;
	int			   rowCount     = 0;
	int			   rowIndex     = 0;
	int			   count		= 0;
	int			   numFields    = 0; // lde, @todo
	int			   size			= 0;
	pNPdatabase    database     = table->owner;
	pNPdbHost      host         = database->host; 
	pNPdbFuncSet   func         = host->hostFuncSet;
//	void*	       conn         = host->conn; // Warning, lde
	double		   start	    = (int)nposGetTime();
	FILE*		   filePtr		= NULL;
	
	/*
	printf("\nbefore new_npdbShowDatabases");
	new_npdbShowDatabases(func, host, dataRef);
	printf("\nafter new_npdbShowDatabases");
	*/
	err = new_npdbSelectTable(table);
	if( err )
	{
		printf("\nnew_npdbSelectTable failed");
		return err;
	}
	
	/// get the result records (all rows)
	result = npdbStoreResult_Safe(func, host, &err);
	if( err )
	{
		printf("\nStoring fieldList failed");
		return err;
	}
	
	/// get the row count
	rowCount = npdbNumRows_safe(result, func, &err);
	if( err )
	{
		printf("\ntable is empty, no rows");
		return err;
	}
	
	printf( "node_tbl row count: %d \n", rowCount );
	printf( "store result time: %.0f sec \n", (nposGetTime() - start) );
	
	/// get the number of fields
	numFields = npdbNumFields_safe(result, func, 94, &err);
	if( err )
	{
		printf("\nFailed to retrieve number of fields");
		return err;
	}
	
	//numFields = (int)func->num_fields(result);
	
	//strcpy(fileName, table->name);
	nposGetAppPath(fileName, &size); 
	//strcpy(fileName, csvName);
	strcat(fileName, csvName); // @todo, rename fileName to filePath, lde
	strcat(fileName, ".csv");
	printf("\nFile Path : %s\n", fileName);

	filePtr = npOpenCSV(fileName, "w+", dataRef);

	if( filePtr == NULL )
		return 2345; // add error code, lde @todo
	
	headerBuffer = (char*) npMalloc(0, 1100, dataRef);
	printf("\nheaderBuffer : %d", kNPfileBlockSize);
	
	if( headerBuffer == NULL)
	{
		printf("Allocation of headerBuffer failed");
		return 8327; // Add error code, @todo lde
	}
	
	npMapTypeName( kNPmapNode, dataRef );
	npWriteMapHeader(headerBuffer, kNPmapFileBufferMax, kNPnode, dataRef);
	
//	printf("\nCount = %d, headerBuffer : %s\n", count, headerBuffer);;
	count = npWriteCSV(headerBuffer, 1, strlen(headerBuffer), filePtr, dataRef);
	npFree(headerBuffer, dataRef);
	
	fieldList = npdbGetFieldList(table, &err, dataRef);
	
	if( fieldList == NULL )
		return 1234; // lde, add proper error code
	
	for( rowIndex = 0; rowIndex < rowCount; rowIndex++ )
	{
	//	if(rowIndex % 500 == 0) printf("%d ", rowIndex);
		if( err = npdbItemErr(database))
		{
			func->free_result(result);
			npdbFreeFieldList(fieldList);
			npCloseCSV(filePtr, dataRef);
			
			return 1;	/// ascert valid DB and connection
		}
		err = npdbGetCSVrowFromDB(func, result, numFields, csvRow);
		if(err != 0)
			break;
		
		count = npWriteCSV(csvRow, 1, strlen(csvRow), filePtr, dataRef);
		count = npWriteCSV("\n", 1, strlen("\n"), filePtr, dataRef);
		// Now zero it out
		csvRow[0] = '\0';
	}
	
	if( rowIndex != rowCount )
	{
		printf("\nAn Error Occurred : %d", err);
		printf("\nrowIndex : %d", rowIndex);
		printf("\nrowCount : %d", rowCount);
	}
	
	npCloseCSV(filePtr, dataRef);
	
	func->free_result(result);
	npdbFreeFieldList(fieldList);
	
	return err;
}

// This is on the backburner, this belongs in a utility , lde (unfinished)
int npdbCSVtoTable( pNPdbTable tbl, FILE* filePtr, void* dataRef) // add to header, lde @todo
{
	int		 count  = 0;
	int		 format = 0;
	int		 type   = 0;
	
	char*   buffer = NULL;
//	char*   field  = NULL; // Warning, lde

	printf("\nnpdbCSVtoTable");
	buffer = (char*)malloc(kNPmapFileBufferMax);
	
	if (buffer == NULL)
	{
		printf ("err 4285 - malloc failed, cannot write file");
		return 4285;
	}
	
	count = npFileRead (buffer, 1, kNPfileBlockSize, filePtr, dataRef);
	printf("\nCount : %d\n", count);
	//printf("\nnpdbCSVtoTable buffer file header : \n%s\n", buffer);
	// Parse the `buffer`
	//field = strtok(buffer, ",");
	
	// Tomorrow, Nov 17th, 2014 work on this @todo , lde
	npGetType(&format, &type, buffer, count, dataRef);
	printf("\ntype : %d\n", type);
	
	if(type == kNPmapNode)
		printf("\nkNPmapNode\n");
	
//	printf("\nField : %s", field);
	
//	tbl_fields = npMysqlGetTableFields(tableType, dataRef); // Make this npdbGetTableFields // tableType is kNPnode @todo
	
	free(buffer);
	
	return 0;
}

// create npdbLoadTable(int type, pNPdatabase dbItem, void* dataRef) lde @todo

//new, lde @todo
int npdbLoadTagTbl( pNPdatabase dbItem, void* dataRef )
{
	int err = 0;
	int rowCount = 0;
	pData data = (pData) dataRef;
	pNPdbHost host = NULL;
	pNPdbFuncSet func = NULL;
	pNPdbTable tag_table = NULL;
	void* conn;
	void* result = NULL;
	
	if( err = npdbItemErr(dbItem)) return err;
	
	host = dbItem->host;
	func = host->hostFuncSet;
	conn = host->conn;
	
	/// select our database to USE
	err = npdbUse( dbItem );
	if( err ) return err;
	
	npdbGetTbls(host, dbItem);
	
	tag_table = npdbFindTagTbl(dbItem, &err, dataRef);
	if(err || tag_table == NULL)
	{
		printf("\nnpdbFindTagTbl failed");
		return err;
	}
	
	printf("\ntag_table owner ptr : %p", tag_table->owner);
	printf("\ntag_table owner name : %s", ((pNPdatabase)tag_table->owner)->name);
//	printf("\ntag_table owner : %p %s", tag_table->owner, ((pNPdatabase)tag_table->owner)->name);
	
	if(err != 0)
		printf("Err : %d", err);
	
	/// SELECT all records (rows) from specified table
	//err = npdbSelectTable( dbItem, "node_tbl"); //dbItem->table[kNPnode]
	
	
	err = new_npdbSelectTable(tag_table); // new, lde
	if( err ) return err;
	
	/// get the result records (all rows) // @todo, replace with safe function, lde
	result = npdbStoreResult_Safe(func, host, &err);
	if( err )
	{
		printf("\nfailed to store result");
		return 5593; ///err 5593 - failed to store result
	}
	
	rowCount = npdbNumRows_safe(result, func, &err);
	if( err )
	{
		printf("err 5594 - node_tbl is empty, no rows \n" );
		//return 5594;
		return err;
	}
	
	printf("\nrowCount : %d\n", rowCount);
	if(!rowCount) return 5595;
	
	new_npdbLoadTags(func, result, dataRef);
	
	//	printf( "total time: %.0f sec \n", (nposGetTime() - start) ); // lde @todo
	
	/// free our result structure, important to maintain connection
	func->free_result( result ); // I need to make sure result isn't null before freeing this, lde @todo
	npSyncTags(dataRef);
	
	return err;
}

/*	Load node table into scene from specified database.
	
	@param dbItem is the database item to load node_tble from.
	@param dataRef is our global scene context.

	@return 0 for success, otherwise an err number.
*/
// new, lde @todo
int npdbLoadNodeTbl( pNPdatabase dbItem, void* dataRef )
{
	pData data = (pData) dataRef;
	pNPdbHost host = NULL;
	pNPdbFuncSet func = NULL;

	void* conn = NULL;	
//	pNPtables tableList   = NULL;				// Warning, lde
	
//	pNPdbTable node_data  = NULL;				// Warning, lde
	double start          = (int)nposGetTime(); // Warning, lde
	char msg[256];								// Warning, lde
	
	int rowCount          = 0;					// Warning, lde
	int nodeCount         = 0;					// Warning, lde
//	int i                 = 0;					// Warning, lde
	int diff = 0;
	
	void* result = NULL;
	pNPdbTable node_table = NULL;
	int err = 0;

//	FILE* filePtr = NULL; // doesn't belong here , lde @todo
	
	//Temp, lde
/*  Remove, lde @todo
	filePtr = npOpenCSV("node_tbl.csv", "r", dataRef);
	printf("\nfilePtr : %p\n", filePtr);
*/	

//	void* result = NULL;	///< using void* to store MYSQL_RES* result handle // Warning, lde
	printf("\nkNodeLink : %d", kNodeLink);
	
	///ascert valid database item, function set and connection
	if( err = npdbItemErr(dbItem)) return err;

	host = dbItem->host;
	func = host->hostFuncSet;
	conn = host->conn;

	/// select our database to USE
	err = npdbUse( dbItem );
	if( err ) return err;

	printf("\ndbItem host : %p", dbItem->host);
	// @todo lde , npdbGetTbls, populates the pNPdbTables structure
	npdbGetTbls(host, dbItem); // new, lde
	
	printf("\ndbItem host : %p", dbItem->host);
	// Finds the "node table" based on fields comparison, lde
	node_table = npdbFindNodeTbl(dbItem, &err, dataRef); // new, lde
	if(err)
	{
		printf("\nnpdbFindNodeTbl failed");
		return err;
	}
		
	/*
	if((int)node_table == 8371)
		return -1; // temp, lde
	*/
	 
	printf("\ndbItem host : %p", dbItem->host);
	
	// Remove file ptr parameter from this function , lde
//	err = npdbTableToCSV(node_table, NULL, dataRef); // make node_table into node_data, lde @todo 
	
	// @todo CSV to table , lde
//	err = npdbCSVtoTable(node_data, filePtr, dataRef);
	
	
	if(err != 0)
		printf("Err : %d", err);
	
	/// SELECT all records (rows) from specified table
	//err = npdbSelectTable( dbItem, "node_tbl"); //dbItem->table[kNPnode]

	
	err = new_npdbSelectTable(node_table); // new, lde
	
	if( err ) return err;

	/// get the result records (all rows) // @todo, replace with safe function, lde
	result = npdbStoreResult_Safe(func, host, &err);
	if( err )
	{
		printf("\nfailed to store result");
		return 5593; ///err 5593 - failed to store result
	}
/*
	if( (result = func->store_result( conn )) == NULL )
	{
		printf( "%s err: %u - %s\n", host->type,
				func->db_errno(conn),func->db_error(conn));
		return 5593;	///err 5593 - failed to store result
	}
*/	
	/// get the row count
	rowCount = npdbNumRows_safe(result, func, &err);
	if( err )
	{
		printf("err 5594 - node_tbl is empty, no rows \n" );
		//return 5594;
		return err;
	}

/*
	if( (rowCount = (int)func->num_rows( result )) == 0 )
	{
		printf("err 5594 - node_tbl is empty, no rows \n" );
		return 5594;
	}
*/
	printf( "node_tbl row count: %d \n", rowCount );
	printf( "store result time: %.0f sec \n", (nposGetTime() - start) );

	/// parse the result and load the items into the scene

//	diff = npGetTime(nposGetTime(), (npdbLoadNodes( func, result, data ) ), nposGetTime());
	
//	printf("\nNew Parse Time : %d\n", diff);
	nodeCount = npdbLoadNodes( func, result, data ); // This should be a function pointer, lde
	printf( "total time: %.0f sec \n", (nposGetTime() - start) ); // lde @todo
	
	
	/// free our result structure, important to maintain connection
	func->free_result( result );
	
	/// @todo replace nodeCount with actual DB load node count
	sprintf( msg, "DB loaded: %d of %d rows", nodeCount, rowCount );
	npPostMsg( msg, kNPmsgDB, dataRef );

	/// After database loaded then select the first pin root node.
	/// @todo store the currently selected cam, grid and node using globals
	npSelectNode(data->map.node[kNPnodeRootPin], data);
	 
	
	return 0;	//success
}

int npdbUpdateAntzStateFromDatabase(void* dataRef)
{
	/*
	int dbNodeID = 0; // Warning, lde
	int i		 = 0; // Warning, lde
	*/
	char* dbName = NULL;
	char msg[kNPurlMax];
	
	MYSQL_RES *myResult;

	pData data = (pData) dataRef;

	// dbName = data->io.dbs->activeDB[0].currentlyUsedDatabase; // temp lde 

	/// return err 4242 if active DB is NULL or blank string '\0'
	if( !dbName[0] )
		return 4242;
	if( dbName[0] == '\0' )
		return 4242;


//	npSelect((void*)data->io.dbs->activeDB[0].id, data->io.dbs->activeDB[0].db, "node_tbl");
	
	// @todo: sort out void* or int id mixup, lde
	// (myResult = (*data->io.dbs->activeDB[0].dbFunc->storeResult)(data->io.dbs->activeDB[0].id)) // old, lde
	if( (myResult = (*data->io.db.activeDB->host->hostFuncSet->store_result)(data->io.db.activeDB->host->id)) == NULL )
	{
		printf("Error storing DB result\n");
		sprintf( msg, "err 5589 - failed to load update from DB: %s", dbName );
		npPostMsg( msg, kNPmsgErr, data );
		return 1;
	}

	npdbUpdateNodesFromMysqlResult( myResult, dataRef );

	return 0;
}

/*!	Processses menu item number and loads corresponding dataset.

	@param item number is the menu item chosen by user. 
	@param dataRef pointer to our global scene context.

	@return 0 for success, or err number if fails to procees item.
*/
int npdbLoadMenuItem (int item, void* dataRef)
{
	int err = 0;

	pData data = (pData) dataRef;
	pNPdbs dbs = &data->io.db;
	pNPdatabase dbItem = NULL;
	pNPdatabase activeDB = data->io.db.activeDB;

	/// check bounds, note that dbCount is +1 for null array slot[0], hence >=
	if( item >= dbs->dbCount )
	{
		printf( "err 5586 - item: %d >= dbCount: %d", item, dbs->dbCount);
		return 5586;
	}

	/// select database to load by menu item index 
	dbItem = dbs->dbList[item];
	if( !dbItem )
	{
		printf( "err 5587 - database is null for item: %d", item );
		return 5587;
	}

	/// load the database
	printf("\ndbItem: %p", dbItem);
	//printf("\ndbItem name : %s", dbItem->name);
//	data->io.db.activeDB = dbItem;
	activeDB = dbItem;
	printf("\nactiveDB->name : %s", activeDB->name);
	//	strcpy(data->io.db.activeDB->name, dbItem->name); // new, lde @todo
//	data->io.db.activeDB->host = dbItem->host; // new, lde @todo
	
	if( err = npdbLoadScene( dbItem, data ) )
		printf( "err 5588 - menu item function fail code: %d", err);

	return err;	//success
}


/// @todo move npdbGetMenu to npdb.h
//------------------------------------------------------------------------------
pNPmenu npdbGetMenu ( pNPmenu menu, void* dataRef)
{
	int i = 0;
	int err = 0; // new, lde
	int itemCount = 0;

	pData data = (pData) dataRef;
	pNPdbs dbs = &data->io.db;
	pNPdatabase dbItem = NULL;


	if( !dbs )
	{
		npPostMsg("err 5555 - npdbGetMenu 'dbs' is null", kNPmsgErr, dataRef);
		return NULL;
	}

	/// clear the dbList and re-populate by querying hosts
	err = npdbRefreshDatabaseList( dbs ); // new, lde
	if(err) return err;

	itemCount = dbs->dbCount -1;	// -1 to skip the null dbItem in array[0]

	//set the menubar fields
	menu->name = npNewStrcpy("Database", data);
	menu->details = npNewStrcpy("Location               |   Nodes", data);
	if( !menu->name || !menu->details )
	{
		printf( "err 5558 - npdbGetMenu failed to allocate npNewStrcpy \n" );
		return NULL;
	}
	//copy the db list to the menu format, start at 1 to coordinate with UI
	for (i=1; i <= itemCount; i++)
	{
		menu->list[i] = malloc( 80 + kNPurlMax );	//host can be a URL
		if (!menu)
		{
			npPostMsg("err 5558 - malloc failed npdbGetMenu", kNPmsgErr, dataRef);
			return NULL;
		}

		/// select the database item
		dbItem = dbs->dbList[i];
		if( !dbItem || !dbItem->name || !dbItem->host || !dbItem->host->ip )
		{
			npPostMsg("err 5559 - NULL element in dbList", kNPmsgErr, dataRef);
			break;		//exit loop on first NULL item encountered
		}		

		/// move menu logic to console.h this should just make a menu content 
		/// reference pointer array then call npConsoleSetMenu( char** items...)
		/// copy the database item to the menu list
		sprintf( menu->list[i], "|%4.0d.| %-33s| %-23s|%10d |",
				 i, dbItem->name, dbItem->host->ip, dbItem->nodeCount ); //zzsql
		//test printf("%s\n", menu->list[i]);
	}

	//set the list size
	menu->count = itemCount;

	npPostMsg( "Updated DB Menu List!", kNPmsgDB, dataRef );

	return menu;
}


// This is for a single node
void npdbRowToNode( pNPnode node, char** row )
{
	pNPnode nodeParent;
	if(node->type == 1)
	{
		return;
	}

	node->selected = npatoi(row[3]);		/// @todo convert from atio to npatoi
	
	//workaround for keeping track of link end B, processed by orphan list , lde
	if(node->type == kNodeLink)
	{
		printf("\nnode type is kNodeLink : %d\n", kNodeLink);
		node->childIndex = npatoi(row[6]);
	}
	else {
		node->childIndex = npatoi(row[7]);
	}

//	node->childIndex = npatoi(row[7]);
	
	node->chInputID		= npatoi(row[9]);
	node->chOutputID	= npatoi(row[10]);
	node->chLastUpdated	= npatoi(row[11]);
		
	node->average		= npatoi(row[12]);
	node->interval		= npatoi(row[13]); // Samples???
		
	node->auxA.x		= npatoi(row[14]);
	node->auxA.y		= npatoi(row[15]);
	node->auxA.z		= npatoi(row[16]);
	node->auxB.x		= npatoi(row[17]);
	node->auxB.y		= npatoi(row[18]);
	node->auxB.z		= npatoi(row[19]);
		
	node->colorShift	= npatof(row[20]);
		
	node->rotateVec.x		= npatof(row[21]);		//was rotate
	node->rotateVec.y		= npatof(row[22]);
	node->rotateVec.z		= npatof(row[23]);
	node->rotateVec.angle	= npatof(row[24]);	
		
	node->scale.x		= npatof(row[25]);
	node->scale.y		= npatof(row[26]);
	node->scale.z		= npatof(row[27]);
		
	node->translate.x	= npatof(row[28]);
	node->translate.y	= npatof(row[29]);
	node->translate.z	= npatof(row[30]);
		
	node->tagOffset.x	= npatof(row[31]);
	node->tagOffset.y	= npatof(row[32]);
	node->tagOffset.z	= npatof(row[33]);
		
	node->rotateRate.x	= npatof(row[34]);
	node->rotateRate.y	= npatof(row[35]);
	node->rotateRate.z	= npatof(row[36]);
		
	node->rotate.x		= npatof(row[37]);					//was rotateRad
	node->rotate.y		= npatof(row[38]);
	node->rotate.z		= npatof(row[39]);
		
	node->scaleRate.x	= npatof(row[40]);
	node->scaleRate.y	= npatof(row[41]);
	node->scaleRate.z	= npatof(row[42]);
		
	node->translateRate.x = npatof(row[43]);
	node->translateRate.y = npatof(row[44]);
	node->translateRate.z = npatof(row[45]);
		
	node->translateVec.x = npatof(row[46]);
	node->translateVec.y = npatof(row[47]);
	node->translateVec.z = npatof(row[48]);
		
	node->shader		= atoi(row[49]);
	node->geometry		= atoi(row[50]);
		
	node->lineWidth		= npatof(row[51]);
	node->pointSize		= npatof(row[52]);
	node->ratio			= npatof(row[53]);
		
	node->colorIndex	= npatoi(row[54]);
		
	node->color.r		= npatoi(row[55]);
	node->color.g		= npatoi(row[56]);
	node->color.b		= npatoi(row[57]);
	node->color.a		= npatoi(row[58]);
		
	node->colorFade		= npatoi(row[59]);
	node->textureID		= npatoi(row[60]);
		
	node->hide			= npatoi(row[61]);
	node->freeze		= npatoi(row[62]);
		
	//	node->center		= center;		//removed	
		
	node->topo			= npatoi(row[63]);			//moved topo
	node->facet			= npatoi(row[64]);		//added topo facet number
		
	node->autoZoom.x	= npatoi(row[65]);	//moved down a slot
	node->autoZoom.y	= npatoi(row[66]);
	node->autoZoom.z	= npatoi(row[67]);
		
	//	node->scroll		= scroll;		//removed made space for facet, zz debug
		
	node->triggerHi.x	= npatoi(row[68]);
	node->triggerHi.y	= npatoi(row[69]);
	node->triggerHi.z	= npatoi(row[70]);
		
	node->triggerLo.x	= npatoi(row[71]);
	node->triggerLo.y	= npatoi(row[72]);
	node->triggerLo.z	= npatoi(row[73]);
		
	node->setHi.x		= npatof(row[74]);
	node->setHi.y		= npatof(row[75]);
	node->setHi.z		= npatof(row[76]);
		
	node->setLo.x		= npatof(row[77]);
	node->setLo.y		= npatof(row[78]);
	node->setLo.z		= npatof(row[79]);
		
	node->proximity.x	= npatof(row[80]);
	node->proximity.y	= npatof(row[81]);
	node->proximity.z	= npatof(row[82]);
		
	node->proximityMode.x = atoi(row[83]);			//int cast supports 1st ver CSV
	node->proximityMode.y = atoi(row[84]);
	node->proximityMode.z = atoi(row[85]);
		
	node->segments.x	= npatoi(row[86]);		//grid segments were stored in node->data,
	node->segments.y	= npatoi(row[87]);		//was node->data->segments.x
	node->segments.z	= npatoi(row[88]);		//now node->segments.x
		
	node->tagMode		= npatoi(row[89]);
	node->formatID		= npatoi(row[90]);
	node->tableID		= npatoi(row[91]);
	node->recordID		= npatoi(row[92]);

	if (node->topo == 0 && node->type == kNodePin)
	{
		////zzdb printf ("topo = 0   id: %d\n", node->id);
		if (node->branchLevel == 0)
			node->topo = kNPtopoPin;	//set root topo to a pin
		else if (node->parent != NULL)  //orhpan child methods in npMapSort
		{
			nodeParent = node->parent;
			if ( nodeParent->topo == kNPtopoPin || nodeParent->topo == 0
				|| nodeParent->topo == kNPtopoTorus )
				node->topo = kNPtopoTorus;
			else
				node->topo = kNPtopoPin;
		}	
	}
}

void npdbUpdateNodeFromRow( char** row, void* dataRef ) // Generalize here
{
//	MYSQL_ROW row;
	pData data = (pData) dataRef;
	pNPnode node = NULL;
	/*
	pNPnode nodeParent = NULL; // Warning, lde
	int type		   = 0;	   // Warning, lde
	int branchLevel	   = 0;    // Warning, lde
	int parentID	   = 0;    // Warning, lde
	int x			   = 0;    // Warning, lde
	int count		   = 0;    // Warning, lde
	*/
	int id = 0;

	
	id = atoi( (const char*)row[0] );

	/// @todo create node id map for scene to DB that supports merged scenes.
	//node = npGetNodeByID(data->io.dbs->activeDB[0].idMap[id], dataRef); // old, lde
	node = npGetNodeByID(data->io.db.activeDB->idMap[id], dataRef);
	
	printf( "db node_id: %4d   scene node id: %4d \n", id, node->id );

	npdbRowToNode( node, row );
}

void npdbUpdateNodesFromMysqlResult(MYSQL_RES *result, void* dataRef)
{
	char** row;	//MYSQL_ROW
	pData data = (pData) dataRef;
	//struct database *db = &data->io.dbs->myDatabase[0];
	//struct database *db = &data->io.dbs->activeDB[0]; // old, lde
	pNPdatabase db = data->io.db.activeDB;
	
	// row = (*db->dbFunc->db_fetch_row)(result) // old, lde
	while( row = (*db->host->hostFuncSet->fetch_row)(result) )
	{
		updateNodeFromMysqlRow( (MYSQL_ROW*)row, dataRef );
	}
}


int npdbUpdateMapID(pNPmapID map, void* dataRef);
void npdbStartMapID( pNPmapID map, void* dataRef );
pNPnode npdbGetNodeByMapID( int id, pNPmapID map, void* dataRef );
int npdbInitMapID( pNPmapID map, int count, void* dataRef );
void npdbStopMapID( pNPmapID map, void* dataRef );

/// synchronize map to current scene graph structure
int npdbUpdateMapID(pNPmapID map, void* dataRef)
{
	/// @todo search map for any deleted nodes, if so set id to 0
	return 0;
}

void npdbStartMapID( pNPmapID map, void* dataRef )
{
	/// @todo add scene lock for structural node changes like new, del, etc.
	//npLockSceneGraph(true);
	npdbUpdateMapID( map, dataRef );
	map->nextID = 0;
}

int npdbInitMapID( pNPmapID map, int count, void* dataRef )
{
	map->nextID = 0;

	map->nodeID = (int*)calloc( count, sizeof(int) );
	if( !map->nodeID )
	{
		printf("malloc failed to allocate nodeID list count: %d\n ", count );
		return 1;
	}
	map->recID = (int*)calloc( count, sizeof(int) );
	if( !map->nodeID )
	{
		printf("malloc failed to allocate map list count: %d\n ", count );
		return 1;
	}

	map->count = count;

	return 0;
}

void npdbStopMapID( pNPmapID map, void* dataRef )
{
	/// @todo add post processing after loading a dataset
	//npLockSceneGraph(false);
	return;
}

/*! Matches an external record 'id' to a local scene item pointer.
	Search speed is optimized for ordered list of input id's.
	Should first call npStartMapID() before map processing.

	@param id of the external table record to map to a scene node.
	@param map list of id pairs to search for match. If does not exist
		in the list then add to the create list for post processing.
	@return a pointer to the scene node, a deleted node returns null.
*/
pNPnode npdbGetNodeByMapID( int id, pNPmapID map, void* dataRef )
{
	int recID = map->recID[map->nextID];

	if( id != recID )
	{
		/// @todo search list for id match, if none add node to create list
		if( id > recID )
			while( id > map->recID[map->nextID] && map->count > map->nextID++ );
		else
		{
			/// @todo use hash table for when list is detected as un-ordered
			/// if our id is behind nextID then our list must be un-ordered!
			/// a while loop search is on order n^2... not good for a million
			while( id < map->recID[map->nextID] && 0 < map->nextID-- );	
		}

		/// @todo add method to create new items during update
		if( map->nextID <= 0 || map->nextID >= map->count )
			return NULL;	
	}

	return npGetNodeByID( map->nodeID[map->nextID++], dataRef );
}

// old int npdbTruncate(void* conn, struct dbFunction *db, char* table)
int npdbTruncate(void* conn, pNPdbFuncSet func, char* table)
{
	char statement[128];

	sprintf( statement, "%s%s","TRUNCATE ", table);
	if( (*func->query)(conn, statement) ) // Replace with safe function, lde @todo
	{
		printf("err 5586 - failed to TRUNCATE database\n");
		return 1;
	}

	return 0;
}

/*! Creates a new database using specified name and host.

	@param dbName is pointer to the name of the DB to create.
	@param host is a pointer to the host to create the DB on.
	@return 0 for success, otherwise an err number.
*/
pNPdatabase npdbCreateDatabase( char* dbName, pNPdbHost host, pNPdbs dbs )
{
	pNPdatabase dbItem = NULL;
	pNPdbFuncSet func  = NULL;
	char* statement    = NULL;
	void* conn		   = NULL;
	int   err		   = 0;
	

	if( npdbHostErr(host) ) return NULL;

	func = host->hostFuncSet;
	conn = host->conn;

	statement = func->StatementCreate( dbName );
	if( !statement ) return NULL;

	/// create our new database and add to the dbList
	//printf( "query: %0.60s\n", statement ); //?? , lde
	printf("\nquery : %s", statement);
	
	err = npdbQuery_safe(conn, func, host, statement);
	if(err)
	{
		printf("\nFailed to create database : %s", dbName);
	}
	else {
		dbItem = npdbAddDatabase( dbName, host, dbs );
	}

	/*
	if( (int)func->query( conn, statement) )
		printf( "%s err: %u - %s\n", host->type,
				func->db_errno(conn), func->db_error(conn) );
	else
		dbItem = npdbAddDatabase( dbName, host, dbs );
	*/
	

	free(statement);
	return dbItem;
}

// lde @todo
//int npdbCreateTableQuery(void* conn, pNPdbFuncSet func, pNPdbHost host, 

/// @todo make npCreateTable2 part of the function set and move to mysql
int npdbCreateTable( pNPdatabase dbItem, char* table, char* fields )
{
	int err = 0;
	char* statement = NULL;
	pNPdbFuncSet func = NULL;
	pNPdbHost host = NULL;
	void* conn = NULL;

	if( err = npdbItemErr(dbItem) ) return err;

	host = dbItem->host;
	if( err = npdbHostErr(host) ) return err;

	func = host->hostFuncSet;
	conn = host->conn;

	// Make func->StatementCreate("table") return func->StatementCreateTable , lde @todo
	// Make func->StatementCreateTable(char* name, char* (*fields)() ) // lde @todo lets not calculate fields until we absolutely need it.
	statement = func->StatementCreateTable( table, fields );
	if( !statement ) return 5504;   //err 5506

	//printf( "query: %0.64s...\n", statement ); // ??, lde
	err = npdbQuery_safe(conn, func, host, statement);
	if( err )
	{
		printf("\nFailed to create table");
		return err;
	}
	
	free(statement);
	return err;
}


char* npdbGetFieldsDesc( int type, void* dataRef );
#define kNPdbNodeBlock 4096		///< number of nodes per block
/*! Saves a database using the specified name and host.

	@todo If the database already exists, then updates it.
	@param dbName is pointer to a cstr that contains the name to Save As.
	@param host is a pointer to the host item to save to.
	@param is our global scene context.
	@return 0 for success, otherwise an err number.
*/

char* npdbGetNodeTableFields(void* dataRef) // lde @todo
{
	char* fields = NULL;
	pNPdbFuncSet func = NULL;
	pNPdbHost    host = NULL; // fix this, lde, @todo
	func = host->hostFuncSet;
	
	fields = func->GetTableFields( kNPnode, dataRef );
	
	return fields;
}

/*
char* npdbGetTagTableFields(void* dataRef) // lde @todo
{
	char* fields = NULL;
	pNPdbFuncSet func = NULL;
	pNPdbHost    host = NULL; // fix this, lde, @todo
	func = host->hostFuncSet;
	
	fields = func->GetTableFields( kNPtag, dataRef );
	
	return fields;
}
*/ 
 
// Include function pointer arguments lde @todo
char* new_npdbStatementCreateTable(char* table, char* (*GetTableFields)(void* dataRef), void* dataRef)
{
	char* createStatement = NULL;
	char* fields = NULL;
	
	fields = GetTableFields(dataRef);
	createStatement = malloc( sizeof(char) * (16 + strlen(table) + strlen(fields) ) );
	sprintf(createStatement, "CREATE TABLE %s(%s)", table, fields);
	
	return createStatement;
}

// Should I pass this the table name too, lde @todo
//char* new_npdbStatementCreateNodeTable(char* (*StatementCreateTable)(), char* (*GetNodeTableFields)(), void* dataRef)
char* new_npdbStatementCreateNodeTable(pNPdbFuncSet func, void* dataRef)
{
	char* statement = NULL;
	char* nodeTableFields = NULL;
	//statement = new_npdbStatementCreateTable("node_tbl", GetNodeTableFields, dataRef);
	nodeTableFields = func->getNodeTableFields();
	statement = func->StatementCreateTable("node_tbl", nodeTableFields);
	
	
	return statement;
}

char* npdbGetTagTableFields(pNPdbFuncSet func, void* dataRef)
{
	char* tagTableFields = NULL;
	tagTableFields = func->getTableFields(kNPtag, dataRef);
	return tagTableFields;
}

char* npdbStatementCreateTagTable(pNPdbFuncSet func, void* dataRef)
{
	char* statement	    = NULL;
	char* tagTableFields = NULL;
	
//	tagTableFields = func->getTagTableFields(); // create getTagTableFields lde @todo
	tagTableFields = npdbGetTagTableFields(func, dataRef);
	
	statement      = func->StatementCreateTable("tag_tbl", tagTableFields);
	free(tagTableFields);
	
	return statement; 
}



char* newest_npdbSaveAs()
{
	
	
}

// This should be passed StatementCreateTable AND getFields function pointers, lde @todo
char* npdbStatementCreateNodeTable(pNPdbFuncSet func, char* (*fields)(void* dataRef), void* dataRef)
{
	char* statement = NULL;
	char* theFields = NULL;
	theFields = fields(dataRef);
	statement = func->StatementCreateTable( "node_tbl", fields);
	free(theFields);
	
	return statement;
}

pNPdbFuncSet npdbGetFuncsFromHost(pNPdbHost host, void* dataRef) // lde, @todo add to header file
{
	return host->hostFuncSet;
}

pNPdbHost npdbGetHostFromDB(pNPdatabase dbItem) // lde, @todo add to header file
{
	return dbItem->host;
}

void* npdbGetConnFromHost(pNPdbHost host) // lde, @todo add to header file
{
	return host->conn;
}

char* new_npdbQuery_safe(void* (*getConn)(pNPdbHost host, void* dataRef), void* (*getFuncs)(pNPdbHost host, void* dataRef), void* (*getHost)(pNPdatabase dbItem), pNPdatabase dbItem, char* statement, void* dataRef)
{
//	char* statement = NULL;
	pNPdbHost host = NULL;
	
	host = getHost(dbItem);
	
	
	return statement;
}

void npdbCreateNodeTable( pNPdatabase dbItem, pNPdbFuncSet func , int* err, void* dataRef )
{
//	char* fields = NULL;
//	fields = func->getNodeTableFields(dataRef);
	
	//	fields = npdbGetNodeTableFields(dataRef); // make into function pointer, lde @todo
//	npdbCreateTable(dbItem, "node_tbl", fields); // make fields parameter into a function pointer, lde @todo
	
//	func->StatementCreateNodeTable(func,

	char* statement = NULL;
//	pNPdbFuncSet func = NULL;
	pNPdbHost host = NULL;
	void* conn = NULL;

	if( err = npdbItemErr(dbItem) ) return err;
	
	host = npdbGetHostFromDB(dbItem);
	if( err = npdbHostErr(host) ) return err;
	
	func = npdbGetFuncsFromHost(host, dataRef);
	conn = npdbGetConnFromHost(host);
	
	statement = func->StatementCreateNodeTable(func, func->getNodeTableFields, dataRef); // This could be reduced to just func and dataRef, lde @todo
	if( !statement ) return 5504; // err 5506
	
	err = npdbQuery_safe(conn, func, host, statement);
	if( err )
	{
		printf("\nFailed to create node table");
		return err;
	}
	
	free(statement);
	return ;
}

int npdbCreateTagTable( pNPdatabase dbItem, void* dataRef)
{
	int err = 0;
	char* statement = NULL;
	pNPdbFuncSet func = NULL;
	pNPdbHost host = NULL;
	void* conn = NULL;
	
	if( err = npdbItemErr(dbItem) ) return err;
	
	host = dbItem->host;
	if( err = npdbHostErr(host) ) return err;
	
	func = host->hostFuncSet;
	conn = host->conn;
	
	statement = npdbStatementCreateTagTable(func, dataRef);
	if( !statement ) return 5504;   //err 5506
	
	err = npdbQuery_safe(conn, func, host, statement);
	
//	char* fields;
//	fields = npdbGetTagTableFields(dbItem->host->hostFuncSet,dataRef);
//	npdbCreateTable(dbItem, "tag_tbl", fields);
	free(statement);
	return err;
}



pNPdatabase new_npdbSaveAs( char* dbName, pNPdbHost host, void* dataRef )
{
	int err = 0;
	//	int i   = 0; // Warning, lde
	
	char* fields = NULL;					///< fields descriptor
	struct csvStrObjects *nodes = NULL;
	struct newChunksObj *chunks = NULL;
	
	pData data = (pData) dataRef;
	pNPdbs dbs = &data->io.db;
	pNPdatabase dbItem = NULL;
	pNPdbFuncSet func = NULL;
	
	
	if( npdbHostErr(host) ) return NULL;
	
	func = host->hostFuncSet;
	
	/// @todo add restrictions on dbName, such as length, character types...
	if( !dbName )
	{
		printf( "err 5532 - npdbSaveAs called with null dbName \n" );
		return NULL;
	}
	
	/// if DB already exists then update it
	if( dbItem = npdbGetByNameAndHost(dbName, host, dbs) )
	{
		if( npdbSaveUpdate( dbItem, data ) )
			return NULL;  //err
		else
			return dbItem;
	}
	
	/// create new DB on specified host
	dbItem = npdbCreateDatabase( dbName, host, dbs );
	if( !dbItem ) return NULL; 
	
	/// now use the database so we may operate on it
	err = npdbUse( dbItem );
	if( err ) return NULL;
	
	npdbCreateNodeTable(dbItem, func, &err, dataRef);
	
	
	/*
	/// add tables to the database, first get the fields descriptor
	fields = func->GetTableFields( kNPnode, data ); // create npdbNodeTableFields, lde @todo
	//	fields = npMysqlGetTableFields( kNPnode, data );
	if( !fields ) return NULL;
	
	/// create the new table using fieldDesc
	if( err = npdbCreateTable( dbItem, "node_tbl", fields ) ) // Let's make this very lazy, lde @todo
	{
		free( fields );
		return NULL; 
	}
	*/
	
	/// @todo implement new DB save procedure for writing nodes in 4k blocks
	nodes = npRevisedNodeValues( dataRef ); 
	chunks = npEvenNewerAllChunk( nodes, dataRef ); 
	npInsertAllChunks( chunks, host->conn, func, "node_tbl");
	/*
	 /// save scene state to the newly created (empty) tables in blocks
	 for( i=1; i <= data->map.nodeCount; i += kNPdbNodeBlock )
	 {
	 err = npdbNodesToDB( insertBlock, i, kNPdbNodeBlock, dbItem, data );
	 if( err ) break;
	 
	 err = npdbInsert( dbItem, "node_tbl", insertBlock );
	 if( err ) break;
	 }
	 */
	npNewFreeChunks( chunks, dataRef );
	free( fields );
	
	if( err )
		return NULL;
	
	return dbItem;	
}

pNPtag npGetTagFromNode(pNPnode node, void* dataRef)
{	
	if(node->tag == NULL) // This isn't even needed, lde @todo
		return NULL;
	
	return node->tag;
}

void npGetTags(void* dataRef)
{
	pData data = (pData) dataRef;
	pNPnode node = NULL;
	pNPtag  tag  = NULL;
	int i = 0;
	
	for(i = 0; i < data->map.nodeRootCount; i++)
	{
		node = data->map.node[i];
		if( ( tag = npGetTagFromNode(node, dataRef) ) != NULL )
		{
			printf("\ntitle : %s\n", tag->title);
			printf("desc  : %s\n", tag->desc );
		}
		
	}
	
}


void npGetCSVtagFromNode(char** buffer, int *index ,pNPnode node, void* dataRef)
{	 
	pNPtag tag = NULL;
	tag = npGetTagFromNode(node, dataRef); 

	if( tag )
	{
		if( strncmp(tag->title, "id: ", 4) )
		{
			buffer[*index] = malloc(sizeof(char) * 4096); // 4096 is arbritary, lde @todo
			if(buffer[*index] == NULL)
			{
				return;
			}
			
			sprintf(buffer[*index], "%i,%i,%i,\"%s\",\"%s\"", (*index)+1, node->recordID, tag->tableID, tag->title, tag->desc );
			//printf("\nbuffer[%d] : %s\n", (*index), buffer[*index] );
			(*index)++;
		}
		else if ( (*index) != 0 )
		{
			(*index)--;
		}
	}
	
	return;
}

void npGetCSVtagsFromNodeTree(char** buffer, int* index, pNPnode node, void* dataRef)
{
	int  i;
	
	npGetCSVtagFromNode(buffer, index, node, dataRef);
	for( i = 0; i < node->childCount; i++ )
	{
		npGetCSVtagFromNode(buffer, index, node->child[i], dataRef);
		
		//printf("\nchildCount : %d\n", node->child[i]->childCount);
		if( node->child[i]->childCount )
		{
			
			npGetCSVtagsFromNodeTree( buffer, index, node->child[i], dataRef );
		}
		
	}
	
	return;
}

void npGetCSVtagsFromAllNodes(char** buffer, int* index, void* dataRef)
{
	pData data = (pData) dataRef;
	pNPnode node = NULL;
	int  x;
	for( x = kNPnodeRootPin; x < data->map.nodeRootCount; x++ )
	{
		npGetCSVtagsFromNodeTree(buffer, index, data->map.node[x], dataRef);
	}
}

void new_npGetTags(char** buffer, int *index ,pNPnode node, void* dataRef)
{
	pNPtag tag = node->tag;
	int i = 0;
	
	printf("\nIndex is %d", *index);
	if(tag != NULL)
	{
		if(strncmp(tag->title, "id: ", 4) != 0)
		{
			buffer[*index] = malloc(sizeof(char) * 4096); // 4096 is arbritary, lde @todo
			sprintf(buffer[*index], "%i,%i,%i,\"%s\",\"%s\"", (*index)+1, node->recordID, tag->tableID, tag->title, tag->desc );
			printf("\nbuffer[%d] : %s\n", (*index), buffer[*index] );
			(*index)++;
		}
		else
		{
			if( (*index) != 0 )
			{
				(*index)--;
			}
		}
	}
	
	printf("\nchildCount : %d", node->childCount);
	
	// data->map.nodeRootCount // lde, @todo
	/*
	 int x = 0;
	 for( x = kNPnodeRootPin; x < data->map.nodeRootCount; x++ )
	 {
		node = data->map.node[x];
		for(i = 0; i < node->childCount; i++)
		{
			new_npGetTags(buffer, index, node->child[i], dataRef);
		}
	 
	 }
	 
	 */
	
	for( i = 0; i < node->childCount; i++ )
	{
	//	(*index)++;
		new_npGetTags(buffer, index, node->child[i], dataRef);
	}
	
	return;
}



int npdbSaveTags(pNPdatabase dbItem, pNPdbFuncSet func, char* table, void* dataRef) // lde @todo
{ 
	pData data = (pData) dataRef;
	pNPtags tags = &data->io.gl.hud.tags;
	pNPrecordTag recordTag = NULL;
	int i = 0;
	char line[421] = {'\0'}; // 421 is arbritrary, lde @todo
	char* statement = NULL;
	char** buffer = malloc(sizeof(char*) * data->map.nodeCount);
	int index = 0;
	
	// put in this in npdbSaveTag , lde @todo
	printf("\ntags->recordCount : %d\n", tags->recordCount);
	printf("\ntags->count : %d\n", tags->count);
	printf("\ntags->sortCount : %d\n", tags->sortCount);
	printf("\ntags->size : %d\n", tags->size);
	
	//npGetTags(dataRef);
	
//	new_npGetTags(buffer, &index, data->map.node[kNPnodeRootPin], dataRef); // Pass this a buffer, lde @todo
	npGetCSVtagsFromAllNodes(buffer, &index, dataRef); // If this fails and returns it needs to set an err var, lde @todo
	for(i = 0; i < index; i++)
	{
		statement = func->StatementInsert("tag_tbl", buffer[i]);
		//printf("\nstatement : %s\n", statement);
		npdbQuery_safe(dbItem->host->conn, func, dbItem->host, statement);
		free(statement);
		free(buffer[i]);
	}
	
	/*
	for( i = 0; i < kNPtagMax; i++ )
	{
		recordTag = tags->recordList[i];
		if( recordTag != NULL )
		{
			sprintf(line, "%i,%i,%i,\"%s\",\"%s\"", recordTag->id, recordTag->recordID, recordTag->tableID, recordTag->title, recordTag->desc );
			statement = func->StatementInsert("tag_tbl", line);
			printf("\nstatement : %s\n", statement);
			npdbQuery_safe(dbItem->host->conn, func, dbItem->host, statement);
			free(statement);
			line[0] = '\0';			
		}
		else {
			//printf("\nNULL");
		}
	 
		
	}
	*/
/*
	for(i = 0; i < tags->recordCount; i++)
	{
		recordTag = tags->recordList[i];
		sprintf(line, "%i,%i,%i,\"%s\",\"%s\"", recordTag->id, recordTag->recordID, recordTag->tableID, recordTag->title, recordTag->desc );
		statement = func->StatementInsert("tag_tbl", line);
		npdbQuery_safe(dbItem->host->conn, func, dbItem->host, statement);
		free(statement);
		line[0] = '\0';
	}
*/		
	return 0;
}

int npTagToCSV(pNPtag tag, void* buffer) // lde @todo
{

	return 0;
}

/*
int npdbStatementInsertQuery(char* row, char* table)
{
	
	return 0;
}
*/

pNPdatabase npdbSaveAs( char* dbName, pNPdbHost host, void* dataRef )
{
	int err = 0;
//	int i   = 0; // Warning, lde

	char* fields = NULL;					///< fields descriptor
	struct csvStrObjects *nodes = NULL;
	struct newChunksObj *chunks = NULL;

	pData data = (pData) dataRef;
	pNPtags tags = &data->io.gl.hud.tags;
	pNPdbs dbs = &data->io.db;
	pNPdatabase dbItem = NULL;
	pNPdbFuncSet func = NULL;
	
	
	if( npdbHostErr(host) ) return NULL;
	
	func = host->hostFuncSet;

	/// @todo add restrictions on dbName, such as length, character types...
	if( !dbName )
	{
		printf( "err 5532 - npdbSaveAs called with null dbName \n" );
		return NULL;
	}

	/// if DB already exists then update it
	if( dbItem = npdbGetByNameAndHost(dbName, host, dbs) )
	{
		if( npdbSaveUpdate( dbItem, data ) )
			return NULL;  //err
		else
			return dbItem;
	}

	/// create new DB on specified host
	dbItem = npdbCreateDatabase( dbName, host, dbs );
	if( !dbItem ) return NULL; 

	/// now use the database so we may operate on it
	err = npdbUse( dbItem );
	if( err ) return NULL;

	/// add tables to the database, first get the fields descriptor
	fields = func->GetTableFields( kNPnode, data ); // create npdbNodeTableFields, lde @todo
//	fields = npMysqlGetTableFields( kNPnode, data );
	if( !fields ) return NULL;

	/// create the new table using fieldDesc
	if( err = npdbCreateTable( dbItem, "node_tbl", fields ) ) // Let's make this very lazy, lde @todo
	{
		free( fields );
		return NULL; 
	}
	
	npdbCreateTagTable(dbItem, dataRef);
	// Check to see if tags exist
//	npSyncTags(dataRef);
	npdbSaveTags(dbItem, func, "tag_tbl", dataRef);

	if(tags->recordCount > 0)
	{
//		npdbSaveTags(dbItem, func, "node_tbl", dataRef);
	}
	else {
		
	}

	//

/// @todo implement new DB save procedure for writing nodes in 4k blocks
	nodes = npRevisedNodeValues( dataRef ); 
	chunks = npEvenNewerAllChunk( nodes, dataRef ); 
	npInsertAllChunks( chunks, host->conn, func, "node_tbl");
/*
	/// save scene state to the newly created (empty) tables in blocks
	for( i=1; i <= data->map.nodeCount; i += kNPdbNodeBlock )
	{
		err = npdbNodesToDB( insertBlock, i, kNPdbNodeBlock, dbItem, data );
		if( err ) break;

		err = npdbInsert( dbItem, "node_tbl", insertBlock );
		if( err ) break;
	}
*/
	npNewFreeChunks( chunks, dataRef );
	free( fields );

	if( err )
		return NULL;

	return dbItem;
}

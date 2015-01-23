/* -----------------------------------------------------------------------------
*
*  npmysql.c
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

#include "npmysql.h"

#include "npdbz.h"
#include "npdb.h"
#include "../../os/npos.h"

#define kNPdbFieldsDescMax 4096

void* nposLoadLibrary( char* filePath );	/// @todo move load library to npos.h

int npMysqlHook( pNPdbFuncSet func, void* dbLib);
pNPdbFuncSet npMysqlAddFuncSet( pNPdbs db, pNPdbFuncSet funcSet );
int npMysqlInitConnOptions( pNPdbFuncSet func , void* connInit );
char* new_npMysqlStatementInsertFromChunk(char* table, struct newChunkObj *theChunk);

/*! Loads the MySQL Connector client library.
	- Creates a new function set and hooks the MySQL specific methods.
	- Adds the new funcSet to the funcSetList.
	- Attaches this funcSet to any existing database hosts of this type.

	@param dataRef global scene context.
*/
void npInitMySQL (void* dataRef)
{
	int err = 0;

	pData data = (pData) dataRef;
	pNPdbs dbs = &data->io.db;
	pNPos  os  = &data->os;
	pNPosFuncSet osFunc = os->funcSet;
	pNPdbFuncSet func = NULL;
	void* dbLib = NULL;   /// dbLib stores the 'HINSTANCE' handle as a void*
	char appPath[kNPmaxPath];
	int size = 0;
	
//	nposGetAppPath(appPath, &size);
	osFunc->getAppPath(appPath, &size);
	
	printf("\nnpInitMySQL : %s\n", appPath);
	/// post the mysql connector client library version
	printf("data->io.db : %p", dbs);
	printf( "\nMySQL Client ver: %d\n", MYSQL_VERSION_ID );

	/// load the mysql client library
	printf("\nAttempting to load windows dll\n");
	//dbLib = nposLoadLibrary( "libmysql.dll" );
	dbLib = osFunc->loadLibrary( "libmysql.dll" );
	if( !dbLib )
	{
		printf("\nFailed loading dll, trying to load mac dylib (32-bit)\n");
		
		//dbLib = nposLoadLibrary( "libmysqlclient.18.dylib" );
		dbLib = osFunc->loadLibrary( "libmysqlclient.18.dylib" );
		if( !dbLib )
		{
			printf("\nFailed loading dll, trying to load mac dylib (64-bit)\n");
			
			//dbLib = nposLoadLibrary( "libmysqlclient.18.64.dylib" );
			dbLib = osFunc->loadLibrary( "libmysqlclient.18.64.dylib" );
			
			if( !dbLib )
			{
				printf("\nFailed loading dll\n");
			}
			
		}
		
	}
	
	if( !dbLib )
	{
		printf("\nAll dynamic linking failed");
		return;
	}
		

	/// create a new initialized funcSet structure
	func = npdbNewFuncSet( dbs );
	if( !func ) return;
	
	/// define our funcSet host type
	strcpy( func->hostType, "mysql" );		

	/// hook our database specific library methods and local utilities
	err = npMysqlHook( func, dbLib );
	if( err ) return;

	/// add our funcSet to the list, replace if same host type already in list
	npMysqlAddFuncSet( dbs, func );
	if( err ) return;

	/// attach function sets to host items by matching the host types
	npdbAttachHostFuncSets( dbs );

	return;
}

/// @todo close mysql connections, unload library and remove function set
void npCloseMySQL (void* dataRef)
{
	/// search hosts list for mysql host->type
		/// close connection and set hostFuncSet to null

	/// remove mysql function set

	/// unload mysql library

	return;
}

void npUpdateMySQL (void* dataRef)
{
	return;
}

char* npdbShowStatement()
{
	char* buffer = NULL;
	buffer = malloc(sizeof(char) * 20);
	if(buffer == NULL)
		return NULL;
	
	strcpy(buffer, "show");
	return buffer;
}

char* npdbDatabasesStatement()
{
	char* buffer = NULL;
	buffer = malloc(sizeof(char) * 20);
	if(buffer == NULL)
		return NULL;
	
	strcpy(buffer, "databases");
	return buffer;
}

char* npConCat( char* (*first)(), char* (*second)(), int* err )
{
	char* buffer1 = NULL;
	char* buffer2 = NULL;
	char* buffer  = NULL;
	
	buffer1 = first();
	if( buffer1 == NULL )
		return NULL;
	
	buffer2 = second();
	if( buffer2 == NULL )
		return NULL;
	
	buffer = malloc(sizeof(char) * ( strlen(buffer1) + strlen(buffer2) + 5 ) );
	if(buffer == NULL)
	{
		err = (int*)9182;
		return NULL;
	}
	
	sprintf(buffer, "%s %s", buffer1, buffer2);
	printf("\n---------%s---------\n", buffer);
	
	free(buffer1);
	free(buffer2);
	
	return buffer;
}

char* npdbShowDatabasesStatement( char* (*showStatement)(), char* (*dbStatement)(), int* err )
{
	char* buffer  = NULL;
	
	buffer = npConCat(showStatement, dbStatement, err);

	return buffer;
}


/* // delete, lde @todo
void new_npdbShowDatabases(pNPdbFuncSet func, pNPdbHost host, void* dataRef)
{
	char* statement = NULL;
	int   err    = 0;
	void* conn   = NULL;
	
	func = host->hostFuncSet;
	conn = host->conn;
	
	statement = npdbShowDatabasesStatement(func->StatementDBshow, func->StatementDatabases, &err);
	
	err = npdbQuery_safe(conn, func, host, statement);
	
	free(statement);
}
*/

/*! Hook MySQL specific external library methods and local utility functions.
	@param func is an initialized funcSet structure to be hooked up.
	@param dbLib is the connection handle.
	@param dataRef global scene context.
	@return 0 if success or 1 if error occured. 
*/
int npMysqlHook( pNPdbFuncSet func, void* dbLib)
{
	// This scopes the functions so they can't be called outside of the func struct, lde
	// @todo for more functions, lde
	char* npMysqlStatementUse(char* dbName); 

	if( !func || !dbLib )
	{
		printf( "err 5410 - npMysqlHook called with NULL pointer \n" );
		return 5410;
	}
	
	/// store our library handle
	func->dbLib = dbLib;

	/// hook our external database library functions
	func->init			= (void*)nposGetLibSymbol( dbLib, "mysql_init"		    ); 
	func->connect		= (void*)nposGetLibSymbol( dbLib, "mysql_real_connect"  ); // Make nposGetLibSymbol into function ptr, lde
	func->options		= (void*)nposGetLibSymbol( dbLib, "mysql_options"	    );
	func->ping			= (void*)nposGetLibSymbol( dbLib, "mysql_ping"		    );
	func->close			= (void*)nposGetLibSymbol( dbLib, "mysql_close"		    );

	func->query			= (void*)nposGetLibSymbol( dbLib, "mysql_query"		    );
	func->store_result	= (void*)nposGetLibSymbol( dbLib, "mysql_store_result"  );
	func->free_result	= (void*)nposGetLibSymbol( dbLib, "mysql_free_result"	);
	func->fetch_row		= (void*)nposGetLibSymbol( dbLib, "mysql_fetch_row"	    );
	func->fetch_lengths	= (void*)nposGetLibSymbol( dbLib, "mysql_fetch_lengths" );
	func->num_fields	= (void*)nposGetLibSymbol( dbLib, "mysql_num_fields"	);
	func->num_rows		= (void*)nposGetLibSymbol( dbLib, "mysql_num_rows"	    );
	func->db_error		= (void*)nposGetLibSymbol( dbLib, "mysql_error"		    );
	func->db_errno		= (void*)nposGetLibSymbol( dbLib, "mysql_errno"	        );
	func->conn_thread_id= (void*)nposGetLibSymbol( dbLib, "mysql_thread_id"		);	
	
	func->InitConnOptions		= npMysqlInitConnOptions;
	func->GetTableFields		= (void*)npMysqlGetTableFields;

	/// hook our local DB utility functions, specific to 'mysql' host type
	func->StatementCreate		= (void*)npMysqlStatementCreate;
	func->StatementInsert		= (void*)npMysqlStatementInsert;	
	func->StatementSelect		= (void*)npMysqlStatementSelect; 
	func->StatementCreateTable	= (void*)npMysqlStatementCreateTable;
	func->StatementCreateNodeTable = (void*)npdbStatementCreateNodeTable;
	func->getNodeTableFields    =  (void*)npdbGetNodeTableFields;
	func->getFuncsFromHost		= (void*)npdbGetFuncsFromHost;
	
	func->StatementUse			= (void*)npMysqlStatementUse;
	func->StatementShow			= (void*)npMysqlStatementShow;
	func->StatementDrop			= (void*)npMysqlStatementDrop;
	func->StatementTruncate		= (void*)npMysqlStatementTruncate;
	func->StatementUpdate		= (void*)npMysqlStatementUpdate;
	func->StatementDBshow		= npdbShowStatement;
	
	func->showDatabases         = npdbShowDatabases; // new, lde
	func->StatementDatabases    = npdbDatabasesStatement;
	func->use					= npdbUse;
	func->getTableFields		= npMysqlGetTableFields;
//	func->show					= npdbShow;
//	func->select				= new_npdbSelectTable;
	
	return 0;
}

pNPdbFuncSet npMysqlAddFuncSet( pNPdbs db, pNPdbFuncSet funcSet )
{
//	pNPdbFuncSet hostTypeMatch = NULL; // Warning, lde

	/// if host type already exist then update it otherwise add it to list
/*	for(i=0; i< db->funcSetCount; i++)
		if( strcasecmp( db->funcSetList[i]->hostType, func->hostType) == 0 )
			hostTypeMatch =  db->funcSetList[i];

	if( hostTypeMatch )
	{
		free( hostTypeMatch );
		hostTypeMatch = func;	///< update existing host funcSet
	}
	else /// add function set to list and increment the funcSetCount
*/	
	db->funcSetList[db->funcSetCount++] = funcSet;	///< add new funcSet

//	}

	return funcSet;
}

/*! Load OS specific library.
	- .dll on Windows
	- .so on Linux and Unix
	- .dylib on OSX

	@param filePath of the library to load, can be local to app.
	@return the library handle pointer if success, else return NULL.
*/ // Moved nposLoadLibrary from npmysql.c to nposx.c
/*
void* nposLoadLibrary( char* filePath )
{
	void* library = LoadLibrary( filePath );
	if( !library )
	{
		printf( "err 9933 - failed to load library: %s\n", filePath);
		return NULL;
	}

	return library;
}
 */

// Make generic, @todo lde
int npMysqlInitConnOptions( pNPdbFuncSet func, void* connInit )
{
	int err = 0;
	bool enable = true;		///< enable MYSQL_OPT_RECONNECT
	
	printf("\nconnInit : %p\n", connInit);
	if( !func || !connInit )
	{
		printf("err 5590 - npMysqlInitConnOptions called with null element\n");
		return 5590;
	}	

	/// set the recconect flag to prevent mysql err 2006 - connection lost
	err = func->options( connInit, MYSQL_OPT_RECONNECT, &enable ); // changed &enable to 0
//	err = mysql_options( connInit, MYSQL_OPT_RECONNECT, &enable );
	printf("\nconnInit : %p\nerr : %d\n", connInit, err);
	if( err )
	{
		printf( "%s err: %u - %s\n", func->hostType,
				(unsigned int)func->db_errno(connInit), (char*)func->db_error(connInit) );
		return 5591;  //err 5591
	}
	printf("\noptions");

	return 0;  //success
}

char* npMysqlStatementDrop(char* dropType, char* dropName, void* dataRef)
{
	//char* statement = malloc(sizeof(char) * (7 + strlen(dropType) + strlen(dropName)));
	pData data = (pData) dataRef;
	char* statement = npdbMalloc( ( sizeof(char) * (7 + strlen(dropType) + strlen(dropName)) ) , dataRef);
	if( statement )
		sprintf(statement, "DROP %s %s", dropType, dropName);

	return statement;
}

char* npMysqlStatementShow(char* showWhat)
{
	char* statement = malloc(sizeof(char) * ( 35 + strlen(showWhat) ) );
	sprintf(statement, "SHOW %s", showWhat);
	
	return statement;
}


char* npMysqlStatementInsert(char* table, char* values) // perhaps I can pass this a function which will extract all the values from the chunks structure, lde
{
	int count = 0;
	char* statement = malloc(sizeof(char) * (20 + strlen(table) + 3 + strlen(values))); //Account for all parens, debug db
	count = sprintf(statement, "INSERT INTO %s VALUES(%s) ", table, values);
	
	return statement;
}

// @todo, lde , change npdbGetList to npdbGetDBlist
// lde, in progress @todo
//@lde, make a tblList

/* npdbGetTblList: This is essentially the same function as npdbGetList
	Pass it a function pointer with either npdbShowTables or npdbShowDatabases, @todo, lde
 */

//@todo, make error verbosity variable, lde, low priority
void* npdbGetTblList(pNPdbHost host ,pNPdatabase db, pNPdbFuncSet func, void* dataRef) // Put in header file, lde
{
	/* be vigilant against errors */ // lde
	/* dbList becomes tblList */
	
	void* myResult = NULL;
	int numFields = 0;
	int numRows = 0;
//	char* row = NULL;
	//MYSQL_ROW row;					// Abstract this away, lde
	char** row = NULL;
	//int fieldLengths = 0;				// old, lde
	unsigned long* fieldLengths;
	int i = 0, j = 0;
	pNPtables tableList = NULL;
	
//	npdbUse(db);
	npdbShowTables(host);				// Put in header file, lde
	
	/* */
//	myResult = (func->store_result)(db->id); // Reduce lde
	myResult = (func->store_result)(host->conn); // Reduce lde
	if( !myResult )
	{
		npPostMsg("err 5565 - npdbGetTblList failed to storeResult", kNPmsgErr, dataRef );
		goto abort;
	}
	
	numFields = (int)(*db->host->hostFuncSet->num_fields)(myResult);
	if( numFields != 1 )
	{
		npPostMsg( "err 5566 - npdbGetTblList numFields != 1", kNPmsgErr, dataRef );
		goto abort;
	}
	
	numRows = (int)(*db->host->hostFuncSet->num_rows)(myResult);
	if( !numRows )
	{
		npPostMsg( "err 5567 - npdbGetTblList numRows < 1", kNPmsgErr, dataRef );
		goto abort;
	}
	printf( "Databases: %d\n", numRows );
	
	tableList = malloc( sizeof(NPtables));
	if( !tableList )
	{
		npPostMsg("err 5568 - malloc failed to create tableList", kNPmsgErr, dataRef);
		goto abort;
	}
	
	//zz update this to allocate a dbItemList and swap(double buffer) with existing global ptr then free the old
	tableList->list = malloc( sizeof(char*) * numRows);
	if( !tableList->list )
	{
		npPostMsg("err 5569 - malloc failed to create tableList->list", kNPmsgErr, dataRef);
		goto abort;
	}
	
	
	db->tableCount = numRows;
	
	printf( "Tables: %d\n", numRows );
	
	db->tableCount = -1;
	//zz add proper MySQL error checking for all commands
	// http://dev.mysql.com/doc/refman/5.0/en/mysql-fetch-row.html
	while( (row = (*db->host->hostFuncSet->fetch_row)(myResult)) )
	{
		//add error checking for malloc, fetch_row and fetch_lengths //zz debug
		fieldLengths = (*db->host->hostFuncSet->fetch_lengths)(myResult);
		if( !fieldLengths )
		{
			npPostMsg("err 5571 - malloc failed to create tableList->list[x]", kNPmsgErr, dataRef);
			goto abort;
		}
		
		db->tableList[db->tableCount++] = malloc(sizeof(NPdbTable)); // ???, lde
		
		//zz for loop is not needed because numFields = 1
		//would not work for multiple fields either, as 
		for(j = 0; j < numFields; j++)
		{
			//dbList->list[i] = malloc(sizeof(char) * (fieldLengths[j]));
			tableList->list[i] = malloc(sizeof(char) * (fieldLengths[j]));
			row[j][fieldLengths[j]] = '\0';
			//row should be copied to new memory then mysql_free...
			tableList->list[i] = row[j];
			printf("\nTable Added : %s\n", row[j]);
			tableList->size = i;
		}
		i++;
	}
	
	(*db->host->hostFuncSet->free_result)(myResult);
	
	return (void*) tableList;
	
abort:
	//zz add check DB for errors
	(*db->host->hostFuncSet->free_result)(myResult); //important to do this to maintain connection
	return NULL;
}

char* new_npMysqlStatementInsertFromChunk(char* table, struct newChunkObj *theChunk)
{
	int count = 0;
	int i = 0;
	int statementLength = sizeof(char) * (20 + strlen(table) + 3 + theChunk->chunkSize + 500); // added +500, hotfix, debug lde
	char* statement = malloc(statementLength);
	
//	printf("\nstatement length : %d", statementLength);
//	printf("\nchunkSize : %d", theChunk->chunkSize);
//	printf("\ntotalCsvStrObjectsSize : %d", theChunk->csvObjects->totalCsvStrObjectsSize); // debug, lde

	
	count = sprintf(statement, "INSERT INTO %s VALUES", table);
	for(i = 0; i <= theChunk->csvObjects->numOfcsvStrObjects; i++ ) 
		count += sprintf(statement+count, "(%s),", theChunk->csvObjects->csvObj[i].csvStr);
	
	statement[count-1] = '\0';
	
	//printf("\nStatement : \n%s\n", statement);
	
	return statement;
}

/*
 char* npMysqlStatementInsert(char* table, struct newChunkObj *value)
 {
 char* statement = NULL;
 int count = 0;
 int index = 0;
 
 statement = malloc(sizeof(char) * (21 + strlen(table) + value->chunkSize + (value->csvObjects->numOfcsvStrObjects * 6) ) ); //*6 is too much, reduce, debug db
 //zz	statement = malloc(sizeof(char) * (64 + strlen(table) + value->chunkSize + (value->csvObjects->numOfcsvStrObjects * 6) ) ); //*6 is too much, reduce, debug db
 // count = sprintf(statement, "TRUNCATE %s", table);
 count = sprintf(statement, "INSERT INTO %s VALUES ", table);
 //	count = sprintf(statement, "UPDATE SET %s VALUES ", table);
 
 for(index = 0; index <=	value->csvObjects->numOfcsvStrObjects; index++) 
 {
 count += sprintf(statement+count, "(%s),", value->csvObjects->csvObj[index].csvStr);
 free(value->csvObjects->csvObj[index].csvStr);
 }
 
 statement[count-1] = '\0';
 
 return statement;
 }
 */



char* npMysqlStatementCreate(char* dbName)
{
	char* statement = malloc(sizeof(char) * ( 17 + strlen(dbName) ) ); //16 for "CREATE DATABASE " + 1 for null terminator
	sprintf(statement, "CREATE DATABASE %s", dbName); // This should be dynamic

	return statement;
}


char* npMysqlStatementUse( char* dbName )
{
	int count = 0;
	char* statement = NULL;

	if( !dbName)
	{
		printf( "err 5621 - npMysqlStatementUse has null dbName \n");
		return NULL;
	}

	count = strlen(dbName) + 5;   /// +5 for 'USE ' and null terminator '\0'

	statement = (char*)malloc( sizeof(char) * count );
	if ( !statement )
	{
//		printf("err 5622 - malloc failed dbName: %0.24s - %d\n", dbName, count);
		printf("err 5622 - malloc failed dbName: %s - %d\n", dbName, count);
		return NULL;
	}

	sprintf( statement, "USE %s", dbName );

	return statement;
}


int npMysqlServerCtrl(void (*ctrlFunction)(void*), void* parameters)
{
	return 0;
}


char* npMysqlStatementCreateTable(char* table, char* fields)
{
	char* statement = malloc(sizeof(char) * (16 + strlen(table) + strlen(fields)));
	sprintf(statement, "CREATE TABLE %s(%s)", table, fields);
	return statement;
}


char* npMysqlStatementSelect( char* table )
{
	int count = 0;
	char* statement = NULL;

	if( !table)
	{
		printf( "err 5623 - npMysqlStatementSelect has null table \n");
		return NULL;
	}

	count = strlen(table) + 15;   /// +5 for 'USE ' and null terminator '\0'

	statement = (char*)malloc( sizeof(char) * count );
	if ( !statement )
	{
	//	printf("err 5624 - malloc failed table: %0.24s - %d\n", table, count);
		printf("err 5624 - malloc failed table: %s - %d\n", table, count);
		return NULL;
	}

	sprintf(statement, "SELECT * FROM %s", table);

	return statement;
}


char* npMysqlStatementTruncate( int dbID, char* tableName )
{
	int count = 0;
	char* statement = malloc(sizeof(char) * ( 10 + strlen(tableName) ) ); //4 for "USE " + 1 for null terminator
	count = sprintf(statement, "TRUNCATE %s", tableName); // This should be dynamic
	statement[count] = '\0';

	return statement;
}
char* npMysqlStatementUpdate( int dbID, char* tableName )
{
	int count = 0;
	char* statement = malloc(sizeof(char) * ( 8 + strlen(tableName) ) ); //4 for "USE " + 1 for null terminator
	count = sprintf(statement, "UPDATE %s", tableName); // This should be dynamic
	statement[count] = '\0';

	return statement;
}

/* lde @todo, later
char* npdbGetTableFields ( int type, void* dataRef )
{
	
}
*/

/*! Get the table fields descriptor for the given table type.

	@param type is a constant that defines the table type.
	@param dataRef is our global scene context.
	@return a string formatted as the DB specific table fields descriptor.
*/
char* npMysqlGetTableFields( int type, void* dataRef ) // @todo, lde, make a npdbGetTableFields function
{
	int i = 0;
	int count = 0;			/// buffer char count
	int itemCount = 0;

	pData data = (pData) dataRef;
	pNPmapType map = NULL;
	char* fields = NULL;
	
	fields = (char*)malloc(sizeof(char) * kNPdbFieldsDescMax);
	if( !fields )
	{
		printf("err 5408 - malloc failed npdbGetFieldsDesc \n" );
		return NULL;
	}
	
//	printf("\nFields allocated\n");

	map = data->map.typeMapNode;		//debug, zz
	
	
	//set map and item count based on type, could be passed in, debug zz
	switch (type)
	{
		case kNPnode: 
			map = data->map.typeMapNode;
			itemCount = kNPnodeItemCount;
			break;
		case kNPcamera :
			map = data->map.typeMapCamera;
			itemCount = kNPcameraItemCount;
			break;
		case kNPgrid :
			map = data->map.typeMapGrid;
			itemCount = kNPgridItemCount;
			break;
		case kNPpin :
			map = data->map.typeMapPin;
			itemCount = kNPpinItemCount;
			break;
		case kNPtag :
			map = data->map.typeMapTag;
			itemCount = kNPtagItemCount;
			break;
		case kNPchMap :
			map = data->map.typeMapChMap;
			itemCount = kNPChMapItemCount;
			break;
		default : 
			//zzdb printf ("err 4381 - unknown type, cant write header field names");
			return 0;	
			break;
	}
	
	//	// This seems a little kludgy, debug ll
	//	((struct mysqlTable*)object->myobject)->fields = malloc(itemCount * 70); // sizeof char* or char ???, debug ll
	//	//zzdb printf("\nbuffer pntr :: %p\n", ((struct mysqlTable*)object->myobject)->fields);
	

	for (i=0; i < itemCount; i++)
	{
		// insert a comma for CSV and JSON files
		//first field name to lead without a comma one without comma
		if (i > 0)
			count += sprintf ((fields + count), ",");
		
		// Changed float to float , debug lde
		//Change int(10) to INT(11), debug db
		switch (map[i].type)
		{
			case kNPvoidPtr :
				count += sprintf ((fields + count), "%s int(10)", map[i].name );
				break;

			case kNPfloat :
				count += sprintf ((fields + count), "%s float", map[i].name );		//zz debug, added this to fix ration bug#83
				break;

			case kNPint :			// same as default
				count += sprintf ((fields + count), "%s int(10)", map[i].name );
				break;
			
			case kNPcharArray :
				count += sprintf ((fields + count), "%s VARCHAR(250)", map[i].name ); // might want to have some sort of setup so that size is variable, debug ll
				break;
			case kNPfloatXYZ : 
				count += sprintf ((fields + count), "%s_x float,%s_y float,%s_z float",
								  map[i].name, map[i].name, map[i].name );
				break;
			case kNPintXYZ : 
				count += sprintf ((fields + count), "%s_x int(10),%s_y int(10),%s_z int(10)",
								  map[i].name, map[i].name, map[i].name );
				break;
			case kNPboolXYZ : 
				count += sprintf ((fields + count), "%s_x int(10),%s_y int(10),%s_z int(10)",
								  map[i].name, map[i].name, map[i].name );	// Don't know what to do with BOOLS in MySQL, just calling it int(10) for now, debug ll....should be TINYINT(1)
				break;
			case kNPfloatXYZA : 
				count += sprintf ((fields + count), "%s_x float,%s_y float,%s_z float,%s_a float",
								  map[i].name, map[i].name, map[i].name, map[i].name );
				break;
			case kNPfloatXYZS : 
				count += sprintf ((fields + count), "%s_x float,%s_y float,%s_z float,%s_s float",
								  map[i].name, map[i].name, map[i].name, map[i].name );
				break;
			case kNPboolXYZS : 
				count += sprintf ((fields + count), "%s_x int(10),%s_y int(10),%s_z int(10),%s_s int(10)",
								  map[i].name, map[i].name, map[i].name, map[i].name );
				break;
			case kNPubyteRGBA : 
				count += sprintf ((fields + count), "%s_r int(10),%s_g int(10),%s_b int(10),%s_a int(10)",
								  map[i].name, map[i].name, map[i].name, map[i].name );
				break;
			case kNPubyteRGB : 
				count += sprintf ((fields + count), "%s_r int(10),%s_g int(10),%s_b int(10)",
								  map[i].name, map[i].name, map[i].name );
				break;
			case kNPfloatRGBA : 
				count += sprintf ((fields + count), "%s_r int(10),%s_g int(10),%s_b int(10),%s_a int(10)",
								  map[i].name, map[i].name, map[i].name, map[i].name );
				break;
			case kNPfloatRGB : 
				count += sprintf ((fields + count), "%s_r int(10),%s_g int(10),%s_b int(10)", 
								  map[i].name, map[i].name, map[i].name );
				break;

			case kNPbyte :														//zz dbz
			case kNPbool :
			case kNPuint :
			case kNPubyte :
				count += sprintf ((fields + count), "%s int(10)", map[i].name );
				break;

			default :				// same as case 0
				/// @todo sync DB type list to new map types and move to npmap.h
				/// @param fieldMap specifies the field syntax, ex: ' int(10),'
				/// npMapToStr(nodeList, fieldMap, data) 
				printf( "warn 7272 - unknown kNPtype: %d\n", map[i].type );	//zz dbz
			//	npPostMsg( msg, kNPmsgDB, data );
				count += sprintf ((fields + count), "%s int(10)", map[i].name );
				break;
		}
	}

//		printf("\ncount : %d", count); //getch();;
	//	count += sprintf ((((struct mysqlTable*)object->myobject)->fields + count), "\n");
	
	//return count;
	return fields;
}


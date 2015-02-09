/* -----------------------------------------------------------------------------
*
*  npauxdb.h
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

#ifndef NPAUXDB_H_
#define NPAUXDB_H_

//#include <my_global.h> // Not needed
#include "mysql.h"

struct fieldvalue
{
	int fieldType;
	int fieldSize;
	char* fieldName;
	char* value;
	struct fieldvalue *next;
};

/*
 tableType:
 1 == AntzChMap
 2 == AntzNodeState
 3 == AntzTags
 */

enum {
	AntzChMap = 1,
	AntzNodeState,
	AntzTags
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
 
/* functions dealing with mysqlDatabase struct
 npInitMysqlDb
 npCloseMysqlDb
 npMysqlDbAddTable
 npMysqlDbRemoveTable
 */
/*
struct mysqlDatabase {
	char* name;
	int dbEngine;
	struct tables *tableList;
};
*/

/*functions dealing with tables struct
  void npInitTables(struct tables *tableList, void* dataRef)
  npCloseTables
 */
/*
struct tables {
	struct mysqlTable *table;
	struct tables *next;
};
*/

/* functions dealing with mysqlTable
 npInitMysqlTable
 npCloseMysqlTable
 npSetMysqlTableName
 npSetMysqlTableFields
 npSetMysqlTableValues
 npSetMysqlTableFormattedValues
 */

//Later build out database engines, debug ll
/*
struct mysqlDatabase {
	char* name;
};
*/
/*
struct mysqlDatabase {
	char* name;
};
*/

// It doesn't make sense for the table to have pointers to the values, queries should have that
/*
struct mysqlTable {
	char* name;
	char* fields;
	char** values;
	int numOfValues; //???
	int numOfChunks;
	char* formattedValues;
	int tableType;
	int fieldsSize;
	int valuesSize;
	int formattedValuesSize;	
};
*/
/*
struct mysqlQuery {
	int command;
	int object;
	int tableType;
	char* subject;
	char* fields; 
	char** values; 
	struct fieldvalue *fv;
	char conditionals[50];
	int conditionalType;
	char query[300][90000];
	int numberOfQueries;
	char **newValues;
	int numOfValues;
	struct mysqlObject* focus;
	MYSQL_RES *result[300];
	int numOfResults;
};
 */
/*
enum {
 DATABASE = 1,
 TABLE = 2,
 QUERY = 3
};
*/

/*
struct mysqlObject {
	int type;
	void* myobject; // Can be typecasted to either mysqlDatabase or mysqlTable or mysqlQuery
};
*/
 
// FIFO Linked List
// void initQueue(struct queue queryQueue, void* dataRef)
// void enqueueNode(struct queue queryQueue, struct query *q, void* dataRef)
// void dequeueNode(struct queue queryQueue, struct query *, void* dataRef)
// void deleteQueue(struct queue queryQueue, void* dataRef)

struct queue {
	struct queue_node *first;
	struct queue_node *last;
	struct queue_node *current;
};

struct queue_node {
	struct queue_node *next;
	struct query *q;
};
 

//-----------------------------------------

// db debug, ll
struct NPdatabase {
	char*	host;
	char*	user;
	char*	pass;
	char*	db;
	unsigned int port;
	char*	unix_socket;
	unsigned long client_flag;
	MYSQL *conn;
	struct query *q;
};
//struct query *q;
//typedef struct NPdatabase NPdatabase;
//typedef struct NPdatabase * pNPdatabase;

// debug, ll


/*
enum 
{
	DATABASE = 0,
	TABLE
};
*/
enum
{
	DEF = 0,
	kNPmysql
};
/* debug db
enum
{
	TINYINT = 0,
	SMALLINT,
	MEDIUMINT,
	INT,
	BIGINT,
	DECIMAL,
	FLOAT,
	DOUBLE,
	BIT,
	CHAR,
	VARCHAR = 10,
	BINARY,
	VARBINARY,
	TINYBLOB,
	BLOB,
	MEDIUMBLOB,
	LONGBLOB,
	TINYTEXT,
	TEXT,
	MEDIUMTEXT,
	LONGTEXT,
	ENUM,
	SET,
	DATE,
	TIME,
	DATETIME,
	TIMESTAMP,
	YEAR,
	GEOMETRY,
	POINT,
	LINESTRING,
	POLYGON,
	GEOMETRYCOLLECTION,
	MULTILINESTRING,
	MULTIPOINT,
	MULTIPOLYGON
};
*/ 


//------------------------------------------------------------------------------
void npInitAuxDB (void* dataRef);
void npCloseAuxDB (void* dataRef);

void npUpdateAuxDB (void* dataRef);


#endif


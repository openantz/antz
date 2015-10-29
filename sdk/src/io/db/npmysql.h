/* -----------------------------------------------------------------------------
*
*  npmysql.h
*
*  ANTz - realtime 3D data visualization tools for the real-world, based on NPE.
*
*  ANTz is hosted at http://openantz.com and NPE at http://neuralphysics.org
*
*  Written in 2010-2015 by Shane Saxon - saxon@openantz.com
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

#ifndef NPMYSQL_H_
#define NPMYSQL_H_  


/// @todo move all MySQL function calls here, from npdbz.c and elsewhere

void npInitMySQL (void* dataRef);
void npCloseMySQL (void* dataRef);
void npUpdateMySQL (void* dataRef);

/// @todo remove any dependency to dbz

int npMysqlServerCtrl(void (*ctrlFunction)(void*), void* parameters);

//char* npMysqlStatementUse(char* dbName);
char* npMysqlStatementShow(char* showWhat);
char* npMysqlStatementCreate(char* dbName);
char* npMysqlStatementCreateTable(char* table, char* fields);
//char* npMysqlStatementInsert(char* table, struct newChunkObj *value);
char* npMysqlStatementInsert(char* table, char* values);
char* npMysqlStatementSelect(char* table);
char* npMysqlStatementTruncate( int dbID, char* tableName );
char* npMysqlStatementUpdate( int dbID, char* tableName );
char* npMysqlStatementDrop(char* dropType, char* dropName, void* dataRef);

//char* new_npMysqlStatementInsertFromChunk(char* table, struct newChunkObj *theChunk);

char* npMysqlGetTableFields( int type, void* dataRef );

char* new_npMysqlStatementInsertFromChunk(char* table, struct newChunkObj *theChunk);

#endif


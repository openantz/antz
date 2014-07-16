/* -----------------------------------------------------------------------------
*
*  npdb.h
*
*  ANTz - realtime 3D data visualization tools for the real-world, based on NPE.
*
*  ANTz is hosted at http://openantz.com and NPE at http://neuralphysics.org
*
*  Written in 2010-2013 by Shane Saxon - contact@saxondigital.net
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


#endif


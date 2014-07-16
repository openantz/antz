/* -----------------------------------------------------------------------------
*
*  npdb.c
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


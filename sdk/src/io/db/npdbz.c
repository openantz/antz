/* -----------------------------------------------------------------------------
*
*  npdbz.c
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

#include "npdbz.h"

#include "npdb.h"

//#include "../../npdata.h"							//defines which OS to compile for

//#include <errno.h>

#include "npmysql.h"

#include "../../os/npos.h"
#include "../../data/npmapfile.h"


int npCreateTable2(struct dbFunction *db, int dbID, char* table, char* fields);
void* npShowDatabases(int dbID, struct dbFunction *db, void* dataRef );
//void* npdbGetList( struct database *db, void* dataRef ); // not needed, been replaced
// int npConnectDB( void* dataRef ); // old, lde
int npdbUse_old( const char* dbName, void* dataRef );

// This is for a single node
void assignNodePropertiesFromArray(char** row, pNPnode node)
{
	pNPnode nodeParent;
	//printf("\nbefore if");
	if(node == NULL) // temp, lde @todo
	{
		return;
	}
	
	if(node->type == 1)
	{
		printf("\nnode->type is 1, returning");
		return;
	}
	//printf("\nafter if");
	//printf("\n");
	//printf("1");
	node->selected = npatoi(row[3]);		/// @todo convert from atio to npatoi
	node->childIndex = npatoi(row[7]);
	
	node->chInputID		= npatoi(row[9]);
	node->chOutputID	= npatoi(row[10]);
	node->chLastUpdated	= npatoi(row[11]);
		
	node->average		= npatoi(row[12]);
	node->interval		= npatoi(row[13]); // Samples???
		
	node->auxA.x		= npatof(row[14]);		//zz
	node->auxA.y		= npatof(row[15]);
	node->auxA.z		= npatof(row[16]);
	node->auxB.x		= npatof(row[17]);
	node->auxB.y		= npatof(row[18]);
	node->auxB.z		= npatof(row[19]);
	//printf("2");
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
	//printf("3");
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
	//printf("4");
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
	//printf("5");
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
	//printf("6");
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
	//printf("7");
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
	//printf("8");
	//printf("\n");

	if( node->type == kNodeGrid )
	{
		if( node->auxA.x == 0.0f)
			node->auxA.x = kNPgridSpacing;
		if( node->auxA.y == 0.0f)
			node->auxA.y = kNPgridSpacing;
		if( node->auxA.z == 0.0f)
			node->auxA.z = kNPgridSpacing;
	}
}

void updateNodeFromMysqlRow (MYSQL_ROW *row, void* dataRef) // Generalize here
{
//	MYSQL_ROW row;
	pData data = (pData) dataRef;
	pNPnode node = NULL;
//	pNPnode nodeParent = NULL; // Warning, lde
	int id = 0;

	id = atoi( (const char*)row[0] );

	/// @todo create node id map for scene to DB that supports merged scenes.
	//node = npGetNodeByID(data->io.dbs->activeDB[0].idMap[id], dataRef); // old, lde
	//printf( "\nBefore npGetNodeByID" );
//	printf( "\ndata->io.db.activeDB->idMap[%d] = %d", id, data->io.db.activeDB->idMap[id] );
//	printf( "\ndata->io.db.activeDB->idMap[%d]", id);
//	printf( " = %d", data->io.db.activeDB->idMap[id] ); // If one saves off a database then does an update, it doesn't create an idMap, lde @todo
	//printf( "\nactiveDB :: %p", data->io.db.activeDB);
	node = npGetNodeByID(data->io.db.activeDB->idMap[id], dataRef);
	//printf("\nAfter npGetNodeByID : node ptr %p", node); 

//	printf( "db node_id: %4d   scene node id: %4d \n", id, node->id ); // temp, lde , fix
	
	
	if(node == NULL) // @todo
	{
		printf("\nNode is NULL ~~~111~~~");
		//node = npNodeNew(5, NULL, dataRef); // hack, lde @todo
		//node = npMapNodeAdd(id, 5, 0, 0, 0, dataRef);
		//printf("\nnew node id is %d", node->id);
	}
		
	//printf("\nBefore assignNodePropertiesFromArray");
	assignNodePropertiesFromArray( (char**)row, node );
	//printf("\nAfter assignNodePropertiesFromArray");

}

void updateNodesFromMysqlResult(MYSQL_RES *result, void* dataRef)
{
	MYSQL_ROW row;
	pData data = (pData) dataRef;
	
//	struct database *db = &data->io.dbs->activeDB[0]; // old, lde
	pNPdatabase db = data->io.db.activeDB;
	
	// row = (*db->dbFunc->db_fetch_row)( result ) // old, lde
	while( row = (*db->host->hostFuncSet->fetch_row)( result ) )
	{
		updateNodeFromMysqlRow( (MYSQL_ROW*)row, dataRef );
	}
}


struct newChunksObj* npInitAllChunkObjects(struct newChunksObj *chunks, void* dataRef)
{
	int chunkIndex = 0;

	for(chunkIndex = 0; chunkIndex < chunks->numOfChunks; chunkIndex++)
	{
		if( (chunks->chunk[chunkIndex].csvObjects = malloc(sizeof(struct csvStrObjects))) == NULL )
			printf( "newChunksObj getch() commented out\n" ); //getch();
	}

	return chunks;
}

void npInitcsvStrObjects(struct csvStrObjects *csvObjects, int numOfObjects, void* dataRef)
{
	int i = 0;
//	printf("\nABC");
	csvObjects->csvObj = NULL;
//	printf("\nBefore Malloc");
	csvObjects->csvObj = malloc(sizeof(struct csvStrObj) * numOfObjects);
//	printf("\nnumOfObjects %d\n", numOfObjects);
//	getch();
	//_ASSERTE( _CrtCheckMemory( ) );
	if(csvObjects->csvObj == NULL)
	{
		printf("\nMalloc Failed");
	}

	//_ASSERTE( _CrtCheckMemory( ) );
//	printf("\nAssign num");
	csvObjects->numOfcsvStrObjects = numOfObjects;

	for(i = 0; i < csvObjects->numOfcsvStrObjects; i++)
	{
//		printf("\ncsvObjects->csvObj[%d] : %p",i, &csvObjects->csvObj[i]);
	}


	//_ASSERTE( _CrtCheckMemory( ) );
}

struct newChunksObj* npGeneralAddObjectsToChunks(struct newChunksObj* chunks, struct csvStrObjects* strObjects, void* dataRef)
{
	struct csvStrObj * chunkStartPtr;
	struct csvStrObj * chunkEndPtr;
	int chunkSize =  0;
	int chunkLock =  0; // 1 == Locked, 0 == Unlocked
	int chunkNum  =  0;
	int objNum    =  0;
//	int i         =  0; // Warning, lde

	int objIndex = 0;
	int numberOfObjectsInChunk = 0;

	for(objIndex = 0; objIndex < strObjects->numOfcsvStrObjects; objIndex++)
	{
		if(chunkNum >= chunks->numOfChunks)
		{
			printf("\nExceeded allocated chunks");
		}

		if(chunkSize == 0)
		{
			chunkStartPtr = &(strObjects->csvObj[objIndex]);
		}		

		chunkSize += strObjects->csvObj[objIndex].size;
		numberOfObjectsInChunk++;
		
		//If only one chunk exists then this while loop wont run and chunkLock will never be set to 1
		while(chunkSize > 65535) 
		{
			chunkSize -= strObjects->csvObj[objIndex].size;
			objIndex--;
			numberOfObjectsInChunk--;
			chunkLock = 1;
		}

		if(chunkLock == 1)
		{
			chunkEndPtr = &(strObjects->csvObj[(objIndex)]); 
			npInitcsvStrObjects(chunks->chunk[chunkNum].csvObjects, numberOfObjectsInChunk, dataRef);
			chunks->chunk[chunkNum].chunkSize = chunkSize;

			objNum = 0;
			while(chunkStartPtr != chunkEndPtr) 
			{
				chunks->chunk[chunkNum].csvObjects->numOfcsvStrObjects = objNum;
				chunks->chunk[chunkNum].csvObjects->csvObj[objNum] = *chunkStartPtr;
				*chunkStartPtr++;
				objNum++;
			}

			
			chunks->chunk[chunkNum].csvObjects->numOfcsvStrObjects = objNum;
			chunks->chunk[chunkNum].csvObjects->csvObj[objNum] = *chunkStartPtr;

			chunkNum++;	
			chunkSize = 0;
			numberOfObjectsInChunk = 0;
			chunkLock = 0;
		}

	}
	// Get Remainder
	
	chunkEndPtr = &(strObjects->csvObj[strObjects->numOfcsvStrObjects]);
	npInitcsvStrObjects(chunks->chunk[chunkNum].csvObjects, numberOfObjectsInChunk, dataRef);
	chunks->chunk[chunkNum].chunkSize = chunkSize;
	objNum = 0;
	
	//reset totalsizeofcsvStrobjects
	chunks->chunk[chunkNum].csvObjects->totalCsvStrObjectsSize = 0;
	while(chunkStartPtr != (chunkEndPtr))
	{
		chunks->chunk[chunkNum].csvObjects->numOfcsvStrObjects = objNum;
		chunks->chunk[chunkNum].csvObjects->csvObj[objNum] = *chunkStartPtr;
		chunks->chunk[chunkNum].csvObjects->totalCsvStrObjectsSize += chunks->chunk[chunkNum].csvObjects->csvObj[objNum].size; //new
		*chunkStartPtr++;
		objNum++;
	}
	

	chunks->numOfChunks = chunkNum;
	return chunks;

}


struct newChunksObj* npTestNewInitChunksObj(struct newChunksObj *chunks, int chunkSize, void* dataRef)
{
	int i = 0;
//	chunks = NULL;
	//printf("\nnpInitChunksObj malloc");
	//chunks = malloc(sizeof(struct newChunksObj)); //Changed chunksObj* to chunksObj
	chunks = malloc(sizeof(struct newChunksObj));
//	printf("\nchunks ptr : %p", chunks);
	//getch();	
	if(chunks == NULL)
	{
		printf("\nCannot allocate initial chunk structure");
		//getch();									//zzsql	
	}

	//zzdb printf("\n----Approx : %d----", (totalObjsSize % chunkSize)); //Change these types to floats, debug db

	chunks->numOfChunks = 5;
	
	printf("\nnpInitChunksObj malloc 2 : %d", chunks->numOfChunks);
	printf("\nWhat is sizeof(struct newChunkObj)?\n%lu", sizeof(struct newChunkObj));
	//getch();
//	chunks->chunk = malloc(sizeof(struct newChunkObj) * chunks->numOfChunks);
	chunks->chunk = malloc(sizeof(struct newChunkObj) * chunks->numOfChunks);
//	printf("\nErrno : %s\n", strerror(errno));
	printf("\nchunks->chunk ptr : %p", chunks->chunk);
	//getch();
	//	printf("\nafter %p", chunks->chunk);

	for(i = 0; i < chunks->numOfChunks; i++)
	{		
	//chunks->chunk[i] = malloc(sizeof(struct chunkObj));
//	printf("\n%d %p ", i, chunks->chunk[i]);
	//getch();
	}


	return chunks;
	
}

struct newChunksObj* npNewInitChunksObj(struct newChunksObj *chunks, int totalObjsSize, int chunkSize, void* dataRef)
{
	int i = 0;
	
	if(chunks == NULL)
	{
		printf("\nCannot allocate initial chunk structure");
	//	getch();									//zzsql	
	}

	//zzdb printf("\n----Approx : %d----", (totalObjsSize % chunkSize)); //Change these types to floats, debug db

	chunks->numOfChunks = (totalObjsSize / chunkSize);
	chunks->numOfChunks += 40; //Was 14
	chunks->numOfChunks++;

	chunks->chunk = malloc(sizeof(struct newChunkObj) * chunks->numOfChunks);

	for(i = 0; i < chunks->numOfChunks; i++)
	{		
		chunks->chunk[i].chunkSize = 0;
		chunks->chunk[i].csvObjects = NULL;
	}

	return chunks;
}


struct newChunksObj* npTestEvenNewerAllChunk(void* dataRef)
{
	struct newChunksObj *chunks = NULL;
	
	printf("\nnpNewInitChunksObj");
//temp	chunks = npNewInitChunksObj(chunks, csvObjects->totalCsvStrObjectsSize, 65535, dataRef);
	chunks = npTestNewInitChunksObj(chunks, 65535, dataRef);
	printf("\nnpInitAllChunkObjects");
//	chunks = npInitAllChunkObjects(chunks, dataRef); // Really this should be npInitAllChunkObjects
	printf("\nnpGeneralAddObjectsToChunks");
//	chunks = npGeneralAddObjectsToChunks(chunks, csvObjects, dataRef);

//	printf("\nReturn");
	return chunks;

}

struct newChunksObj* npEvenNewerAllChunk(struct csvStrObjects *csvObjects, void* dataRef)
{
	struct newChunksObj *chunks;
	chunks = malloc(sizeof(struct newChunksObj)); // This really should be in the npNewInitChunksObj function

	//Really all these functions should be function pointers within the chunks structure
//	printf("\n-------------------------------------------------------------------------\n");
//	printf("\nnpEvenNewerAllChunk total csv string objects size : %d\n", csvObjects->totalCsvStrObjectsSize);
//	printf("\n-------------------------------------------------------------------------\n");
	chunks = npNewInitChunksObj(chunks, csvObjects->totalCsvStrObjectsSize ,65535, dataRef);
	chunks = npInitAllChunkObjects(chunks, dataRef); 
	chunks = npGeneralAddObjectsToChunks(chunks, csvObjects, dataRef);

	return chunks;

}

void npNewMapTraverseTreeLL(struct csvStrObjects *nodes, int* index, pNPnode node, int format, void* dataRef)
{
//	pData data = (pData) dataRef; // Warning, lde
	int i = 0;
//	int count = 0;
	
//	//zzdb printf("\nIndex : %d", *index);
	//	//zzdb printf("\nIndex is %d :: node->branchLevel : %d :: node->childCount : %d", *index, node->branchLevel, node->childCount);
	printf("\nnn1 : node->branchLevel : %d", node->branchLevel);
	if (node->branchLevel > 0)
	{
//		nodes->node[*index].size = npWriteNode(nodes->node[*index].csvStr, node, format, dataRef);
		printf("\nnn2");
		nodes->csvObj[*index].size = npWriteNode(nodes->csvObj[*index].csvStr, node, format, dataRef);
		printf("\nstr : %s\n", nodes->csvObj[*index].csvStr);
		//nodes->totalCsvSize += nodes->node[*index].size;
		printf("\nnn3");
		nodes->totalCsvStrObjectsSize += nodes->csvObj[*index].size;
		printf("\nnn4");
	//	nodes->node[*index].csvStr[nodes->node[*index].size] = '\0';
		printf("\nnn5");
		nodes->csvObj[*index].csvStr[nodes->csvObj[*index].size] = '\0';
	//	//zzdb printf("\nCSV NODE %d : %s\nTotal Size : %d",*index, nodes->node[*index].csvStr, nodes->totalCsvSize);
			
		printf("\nnn6");
		(*index)++;
		//data->io.write = nodes->node[*index].csvStr; tmp
	}
	
	//recursively calls this function for all children
	printf("\nnode->childCount : %d", node->childCount);
	for (i=0; i < node->childCount; i++)//node->childCount; j++)
	{
	//	//zzdb printf("\nChild : %d", i);
		if (node->child[i] != NULL
			&& node->child[i]->child[0] != node)	//don't write twice, this is link B so skip
		{
			//count += npNewMapTraverseTreeLL(buffer, index, totalCount, arbritary, node->child[i], format, dataRef);
			//(*totalCount) = npNewMapTraverseTreeLL(buffer, index, totalCount, arbritary, node->child[i], format, dataRef);
			npNewMapTraverseTreeLL(nodes, index, node->child[i], kNPmapNodeCSV, dataRef);
			//data->io.write = nodes->node[*index].csvStr; tmp
			//nodes->node[x].size = npMapTraverseTreeLL(nodes, &x, node, kNPmapNodeCSV, dataRef);
			//nodes->totalCsvSize += nodes->node[*index].size;
		}
	}
	//	//zzdb printf("npMapTraverseTreeLL totalCount :: %d", count);
	
	
	//return count;
	//return nodes->totalCsvSize;
	
}

void npNewerMapTraverseTreeLL(struct csvStrObjects *nodes, int* index, pNPnode node, int format, void* dataRef)
{
//	pData data = (pData) dataRef; // Warning, lde
	int i = 0;

//	printf("\nn1 : node->branchLevel : %d", node->branchLevel);
	if (node->branchLevel > 0)
	{
		//nodes->node[*index].size = npWriteNode(nodes->node[*index].csvStr, node, format, dataRef);
//		printf("\nn2");
//		printf("\nSecondary : %d", *index);
		nodes->csvObj[*index].size = npWriteNode(nodes->csvObj[*index].csvStr, node, format, dataRef);
//		printf("\n%s", nodes->csvObj[*index].csvStr);

			if( *index == 66 )
			{
		//		printf("\nTHIS HERE ----> 66 : %s\n", nodes->csvObj[*index].csvStr);
				//getch(); getch(); getch();
			}
		//		printf("\nn3");
		//nodes->totalCsvSize += nodes->node[*index].size;
		nodes->totalCsvStrObjectsSize += nodes->csvObj[*index].size;
//		printf("\nn4");
		//nodes->node[*index].csvStr[nodes->node[*index].size] = '\0';
		nodes->csvObj[*index].csvStr[nodes->csvObj[*index].size] = '\0';
//		printf("\nn5");
		(*index)++;
	}
	
	//recursively calls this function for all children
//	printf("\nnode->childCount : %d", node->childCount);
	for (i=0; i < node->childCount; i++)//node->childCount; j++)
	{
		if (node->child[i] != NULL
			&& node->child[i]->child[0] != node)	//don't write twice, this is link B so skip
		{
//			npNewMapTraverseTreeLL(nodes, index, node->child[i], kNPmapNodeCSV, dataRef);
			npNewerMapTraverseTreeLL(nodes, index, node->child[i], kNPmapNodeCSV, dataRef);
		}
	}
}

struct csvStrObjects* npRevisedNodeValues(void* dataRef)
{
	pData data = (pData) dataRef;
	struct csvStrObjects *nodes = malloc(sizeof(struct csvStrObjects));
	pNPnode node = NULL;
	int nodeIndex = 0;
	int totalNodeCount = 0;

//	printf("\nnpRevisedNodeValues");
	nodes->numOfcsvStrObjects = data->map.nodeCount;
	nodes->totalCsvStrObjectsSize = 0;
	nodes->csvObj = NULL;

	if( (nodes->csvObj = malloc((nodes->numOfcsvStrObjects) * sizeof(struct csvStrObj))) == NULL )
		printf("\nNULL");

	for(nodeIndex = 0; nodeIndex < nodes->numOfcsvStrObjects; nodeIndex++)
	{
		if((nodes->csvObj[nodeIndex].csvStr = malloc(1500 * sizeof(char))) == NULL)
			printf("NULL");
	}

	totalNodeCount = 0;
	for(nodeIndex = 0; nodeIndex < (data->map.nodeRootCount); nodeIndex++)
	{
		node = data->map.node[nodeIndex];
	if (node->type != kNodeHUD)
	{
		nodes->csvObj[totalNodeCount].size = npWriteNode (nodes->csvObj[totalNodeCount].csvStr, node, kNPmapNodeCSV, dataRef);
		nodes->totalCsvStrObjectsSize += nodes->csvObj[totalNodeCount].size;
		nodes->csvObj[totalNodeCount].csvStr[(nodes->csvObj[totalNodeCount].size)] = '\0';
		totalNodeCount++;

		if (node->childCount)
			npNewerMapTraverseTreeLL(nodes, &totalNodeCount, node, kNPmapNodeCSV, dataRef);

	}
	}

	nodes->numOfcsvStrObjects = totalNodeCount;

	return nodes;
}

struct csvStrObjects* npNodeValues(void *dataRef)
{
	struct csvStrObjects *nodes = malloc(sizeof(struct csvStrObjects));
	pData data = (pData) dataRef;
	pNPnode node = NULL;
//	int totalCount = 0; // Warning, lde
	int nodeIndex = 0;
	int x = 0;
	int y = 0;
	
//	nodes = NULL;
/*
	if( (nodes = malloc(sizeof(struct csvStrObjects))) == NULL )
	{
		printf("\nnodes null");
	}
*/

	//_ASSERTE( _CrtCheckMemory( ) );
//	nodes = malloc(sizeof(nodes));
	//_ASSERTE( _CrtCheckMemory( ) );
	printf("\n%p", nodes);

	printf("node att1 : %p", nodes);
	printf("\nglobalCount : %d", data->map.globalsCount);
	nodes->numOfcsvStrObjects = data->map.nodeCount;
	printf("\nnode att2 : %d", nodes->numOfcsvStrObjects);
	//getch();
	nodes->totalCsvStrObjectsSize = 0; //Temp
	nodes->csvObj = NULL;	
	//_ASSERTE( _CrtCheckMemory( ) );
	printf("\ncalloc2 : %p", nodes->csvObj);

	////_ASSERTE( _CrtCheckMemory( ) );
	if( (nodes->csvObj = malloc((nodes->numOfcsvStrObjects*2) * sizeof(struct csvStrObj))) == NULL )
	{
		printf("\nNULL");
	}
	printf("\nsizeof(int) : %lu", sizeof(int));
//	printf("\nsizeof(struct csvNode *node) : %d", sizeof(struct csvNode*));

	printf("\nfor 1 : %p", nodes->csvObj);
	printf("\nSizeof(nodes) : %lu", sizeof(nodes));
//	printf("\nSizeof(struct csvNodes) : %d", sizeof(struct csvNodes));
	nodes->totalCsvStrObjectsSize = 0;
	////_ASSERTE( _CrtCheckMemory( ) );
	for(nodeIndex = 0; nodeIndex <= (nodes->numOfcsvStrObjects); nodeIndex++) //Removed <=
	{
		printf("\nnodeIndex : %d : %p", nodeIndex, &nodes->csvObj[nodeIndex]);
		
		if((nodes->csvObj[nodeIndex].csvStr = malloc(1000 * sizeof(char))) == NULL)
		{
			printf("\nNULL");
			//getch();
		}
		else
		{
			/*
			if((nodeIndex == 66) || (nodeIndex == 65))
			{			
				printf("\nnodes->csvObj[%d].csvStr ptr : %p", nodeIndex, nodes->csvObj[nodeIndex].csvStr);
				printf("\n%s\n", nodes->csvObj[nodeIndex].csvStr);
				getch();
			}
			*/
		}
	}
		
	//_ASSERTE( _CrtCheckMemory( ) );
	x = 0;
	printf("\nfor 2 : %d", data->map.nodeRootCount);
	//getch();
	for(nodeIndex = 0; nodeIndex < (data->map.nodeRootCount); nodeIndex++)
	{
		printf("\nMain Index %d", nodeIndex);
		node = data->map.node[nodeIndex];
		//_ASSERTE( _CrtCheckMemory( ) );
		
	//	if (node->type != kNodeHUD)

		if(1 == 1)
		{
		//	printf("\na");
			nodes->csvObj[x].size = npWriteNode (nodes->csvObj[x].csvStr, node, kNPmapNodeCSV, dataRef);
			if( x == 66 )
			{
			//	printf("\nTHIS HERE ----> 66 : %s\n", nodes->csvObj[x].csvStr);
				//getch(); getch(); getch();
			}

	//_ASSERTE( _CrtCheckMemory( ) );
		//	printf("\nnodes->totalCsvStrObjectsSize : %d", nodes->totalCsvStrObjectsSize);
			nodes->totalCsvStrObjectsSize += nodes->csvObj[x].size;
	//_ASSERTE( _CrtCheckMemory( ) );
		//	printf("\nc");
			nodes->csvObj[x].csvStr[(nodes->csvObj[x].size)] = '\0'; //Adding -1
	//_ASSERTE( _CrtCheckMemory( ) );
		//	printf("\nd");

			x++;
		//	printf("\nnode->childCount : %d", node->childCount);
			if (node->childCount)
			{
				npNewerMapTraverseTreeLL(nodes, &x, node, kNPmapNodeCSV, dataRef);
	//_ASSERTE( _CrtCheckMemory( ) );
			}
		}
		else {
			y++;
		}


	}

	printf("\nFIN");
	printf("\nX : %d", x);
	printf("\nY : %d", y);
	//getch();
	nodes->numOfcsvStrObjects = x;
	printf("\n-----NUMBER OF CSV STRING OBJECTS : %d-----\n", nodes->numOfcsvStrObjects);
	//getch(); getch();
	//_ASSERTE( _CrtCheckMemory( ) );

	return nodes;
}



char* npNewGenMysqlFields(int count, int type, void* dataRef)
{
	int i = 0;
	int itemCount = 0;

	pData data = (pData) dataRef;
	pNPmapType map = NULL;
	char* fields = malloc(2000);
//	char msg[128];
	
	map = data->map.typeMapNode;		//debug, zz
	

		if (fields == NULL || count < 5)
			return 0;
		
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
		
		count = 0; 
		for (i=0; i < itemCount; i++)
		{
			// insert a comma for CSV and JSON files
			//first field name to lead without a comma one without comma
			if (i > 0)
				count += sprintf ((fields + count), ",");
			
			//Change INT(10) to INT(11), debug db
			switch (map[i].type)
			{
				case kNPvoidPtr :
				count += sprintf ((fields + count), "%s INT(10)", map[i].name );
					break;

				case kNPfloat :
					count += sprintf ((fields + count), "%s FLOAT(10)", map[i].name );		//zz debug, added this to fix ration bug#83
					break;

				case kNPint :			// same as default
					count += sprintf ((fields + count), "%s INT(10)", map[i].name );
					break;
				
				case kNPcharArray :
					count += sprintf ((fields + count), "%s VARCHAR(250)", map[i].name ); // might want to have some sort of setup so that size is variable, debug ll
					break;
				case kNPfloatXYZ : 
					count += sprintf ((fields + count), "%s_x FLOAT(10),%s_y FLOAT(10),%s_z FLOAT(10)",
									  map[i].name, map[i].name, map[i].name );
					break;
				case kNPintXYZ : 
					count += sprintf ((fields + count), "%s_x INT(10),%s_y INT(10),%s_z INT(10)",
									  map[i].name, map[i].name, map[i].name );
					break;
				case kNPboolXYZ : 
					count += sprintf ((fields + count), "%s_x INT(10),%s_y INT(10),%s_z INT(10)",
									  map[i].name, map[i].name, map[i].name );	// Don't know what to do with BOOLS in MySQL, just calling it INT(10) for now, debug ll....should be TINYINT(1)
					break;
				case kNPfloatXYZA : 
					count += sprintf ((fields + count), "%s_x FLOAT(10),%s_y FLOAT(10),%s_z FLOAT(10),%s_a FLOAT(10)",
									  map[i].name, map[i].name, map[i].name, map[i].name );
					break;
				case kNPfloatXYZS : 
					count += sprintf ((fields + count), "%s_x FLOAT(10),%s_y FLOAT(10),%s_z FLOAT(10),%s_s FLOAT(10)",
									  map[i].name, map[i].name, map[i].name, map[i].name );
					break;
				case kNPboolXYZS : 
					count += sprintf ((fields + count), "%s_x INT(10),%s_y INT(10),%s_z INT(10),%s_s INT(10)",
									  map[i].name, map[i].name, map[i].name, map[i].name );
					break;
				case kNPubyteRGBA : 
					count += sprintf ((fields + count), "%s_r INT(10),%s_g INT(10),%s_b INT(10),%s_a INT(10)",
									  map[i].name, map[i].name, map[i].name, map[i].name );
					break;
				case kNPubyteRGB : 
					count += sprintf ((fields + count), "%s_r INT(10),%s_g INT(10),%s_b INT(10)",
									  map[i].name, map[i].name, map[i].name );
					break;
				case kNPfloatRGBA : 
					count += sprintf ((fields + count), "%s_r INT(10),%s_g INT(10),%s_b INT(10),%s_a INT(10)",
									  map[i].name, map[i].name, map[i].name, map[i].name );
					break;
				case kNPfloatRGB : 
					count += sprintf ((fields + count), "%s_r INT(10),%s_g INT(10),%s_b INT(10)", 
									  map[i].name, map[i].name, map[i].name );
					break;

	//			case kNPvoidPtr :		//zz debug need special handling for foreign key IDs
				case kNPbyte :														//zz dbz
				case kNPbool :
				case kNPuint :
				case kNPubyte :
					count += sprintf ((fields + count), "%s INT(10)", map[i].name );
					break;

				default :				// same as case 0
					/// @todo sync DB type list to new map types and move to npmap.h
					/// @param fieldMap specifies the field syntax, ex: ' INT(10),'
					/// npMapToStr(nodeList, fieldMap, data) 
					printf( "warn 7272 - unknown kNPtype: %d\n", map[i].type );	//zz dbz
				//	npPostMsg( msg, kNPmsgDB, data );
					count += sprintf ((fields + count), "%s INT(10)", map[i].name );
					break;
			}
		}

//		printf("\ncount : %d", count); //getch();;
	//	count += sprintf ((((struct mysqlTable*)object->myobject)->fields + count), "\n");
	
	//return count;
	return fields;
}


void npInsert(void* dbID, pNPdbFuncSet func, char* table, struct newChunkObj *value)
{
	int queryReturnValue = 0;
	char* statement = NULL; 
	statement = func->StatementInsert(table, value->csvObjects->csvObj->csvStr);
	queryReturnValue = (int)func->query(dbID, statement);
	free(statement);
}

//zzd r // This function might be obsolete, lde @todo
int npSelect(void* conn, pNPdbFuncSet func, char* table) //Add field(s) choice later
{
	int err = 0;
	char* statement = func->StatementSelect(table);
	 
	statement = func->StatementSelect(table);
	if( !statement )
	{
		printf( "err 5528 - failed SELECT statement table: %s\n", table );
		return 5528;
	}

	err = (int)func->query(conn, statement); //Create a function to process mysql error codes
	if( err )
		printf( "MYSQL error: %u (%s)\n",
				(unsigned int)func->db_errno(conn), (char*)func->db_error(conn) );

	//printf("\nreturned value : %d\n", queryReturnValue);
	//printf("\nnpSelect error : %d", error);
	free(statement);
	return err; //Zero for success, nonzero if error occurred
}

//zzd r
void* npShowDatabases(int dbID, struct dbFunction *db, void* dataRef )
{
	char* statement = NULL;
	void* error = NULL;

	statement = (*db->StatementShow)("DATABASES");
	if( statement )
	{
		printf("query: %s\n", statement);
		
		error = (*db->query)(dbID, statement);
		if( error )
			printf( "err 5572 - failed DB query statement", kNPmsgErr, dataRef );

		free(statement);
	}
	else
		npPostMsg("err 5573 - failed DB StatementShow", kNPmsgErr, dataRef );

	return error;
}

// This function might be obsolete, lde @todo
/*
int npUseDatabase2(void* conn, pNPdbFuncSet func, char* dbName)
{
//	printf("ERRRRRRRR npUseDatabse2 called!!!\n"); return 1;
		int err = 0;
	char* statement = NULL;
	
	statement = func->StatementUse( dbName );
	printf("query: %s\n", statement);
	err = (int)func->query( conn, statement );
	if( err )
	{
		printf("err 5583 - failed to USE database \n");
		printf("MYSQL err: %u - %s\n", func->db_errno(conn), func->db_error(conn));
	}
	free(statement);
	return err;
}
*/

//Should queries be renamed to statements
int npCreateDatabase2(int dbID, struct dbFunction *db, char* dbName)
{
	int err = 0;
	char* statement = NULL;
	
	statement = (*db->StatementCreate)(dbName);
	err = (int)(*db->query)(dbID, statement);
	if( err )
	 printf("err 5584 - failed to CREATE database\n");

	free(statement);
	return err;
}
/*
// This function might be obsolete, lde @todo
int npDropDatabase(int dbID, pNPdbFuncSet FuncSet, const char* dbName, void* dataRef )
{
	//char* statement = (*db->StatementDrop)("DATABASE", dbName); // old, lde
	char* statement = (*FuncSet->StatementDrop)("DATABASE", dbName);
	int success = 0;
	
	//success = (int)(*db->query)(dbID, statement); // old, lde
	success = (int)(*FuncSet->query)(dbID, statement);

	if( !success )
		npPostMsg( "warn 5585 - failed to DROP database", kNPmsgDB, dataRef);
	else
		free(statement);

	return 0;
}
*/

int npDropTable(int dbID, struct dbFunction *db, char* table)
{
	char* statement = (*db->StatementDrop)("TABLE", table);
	(*db->query)(dbID, statement);
	free(statement);

	return 0;
}

/*
int npConnectToDatabaseServer(struct dbNewConnect *connect, void* dataRef)
{
	
	return 0;
}
*/

/* DB system test app

void main(void* dataRef)
{
	char* nodeFields;
	int sqlinit = 0;
	int dbID = 0;
	int x = 1;
	int index = 0;
	int success = 0;
	struct dbFunction *dbFunctions = NULL;
	struct csvStrObjects *nodes = NULL;
	struct newChunksObj *chunks = NULL;
	MYSQL_RES *myResult;

	dbFunctions = malloc(sizeof(struct dbFunction));
	x = dbHook(dbFunctions, "libmysql.dll", 0);
	
	sqlinit = (*dbFunctions->init)(NULL);
	dbID = (*dbFunctions->connect)(sqlinit, "localhost", "root", "admin", "", 3306, NULL, 0);
//	printf("\nhostID %p\n", dbID);
	
	//Code for saving
	
	npCreateDatabase2(dbID, dbFunctions, "antz1");
	
	success = npUseDatabase2(dbID, dbFunctions, "antz1");
	
	nodeFields = npNewGenMysqlFields(kNPmapFileBufferMax, kNPnode, dataRef);

	
	success = npCreateTable2(dbFunctions, dbID, "node_tbl", nodeFields);
	free(nodeFields);
	

	printf("\nnodevalues");
//	nodes = npNodeValues(dataRef);


	nodes = npRevisedNodeValues(dataRef);
	printf("\nAfter npRevisedNodeValues %p\n", nodes->csvObj[66].csvStr);
//	getch();
//	printf("\nchunk");
	chunks = npEvenNewerAllChunk(nodes, dataRef);
	
	success = 0;;
	printf("\nchunks->numOfChunks : %d", chunks->numOfChunks); //getch();
	printf("\nStart Time : %f\n", nposGetTime());
	for(index = 0; index <= chunks->numOfChunks; index++)
	{
		printf("\nchunkIndex : %d", index);
		npInsert(dbID,dbFunctions,"node_tbl", &chunks->chunk[index]);
	}
	printf("\nStop Time : %f\n", nposGetTime());
	


	// Code for loading 
	 
	success = npUseDatabase2(dbID, dbFunctions, "antz1");
	success = 0;
	success = npSelect(dbID, dbFunctions, "node_tbl");

	myResult = (*dbFunctions->storeResult)(dbID);	
	printf("\nBefore");
	npdbLoadNodes(myResult, dataRef);
	printf("\nAfter");
	
	//	npMysqlStatementInsert("node_tbl", &chunks->chunk[0]);
//	npCreateDatabase(dbID, dbFunc, "chMapTable");
//	npUseDatabase(dbID, dbFunc, "chMapTable");
//	npDropDatabase(dbID, dbFunc, "node_tbl");
//	npDropDatabase(dbID, dbFunc, "chMapTable");

	printf("\nclose");
	(*dbFunctions->close)(dbID);

	free(dbFunctions);
//	free(nodeFields);
//	printf("\nReturning");

}
*/

void npRevisedFreeNodeValues(struct csvStrObjects *nodes, void* dataRef)
{
	// npFreeChunks frees the csvStr that nodes point to, make sure to check whether csvStr is already freed (I.E. set to NULL)
	int nodeIndex = 0;
	for(; nodeIndex < nodes->numOfcsvStrObjects; nodeIndex++)
	{
		
	}
}

struct newChunkObj* npGetChunk(struct newChunksObj *chunks, int index)
{
	return &chunks->chunk[index];
}

// lde, @todo drop a range of databases

void npInsertAllChunks(struct newChunksObj *chunks, void* dbID, pNPdbFuncSet func, char* table)
{
	int index = 0;
	char* statement = NULL;
	struct newChunkObj *chunkPtr = NULL;
	int queryReturnValue = 0;
	
	//printf("\nnpInsertAllChunks");
	for(; index <= chunks->numOfChunks; index++)
	{
		//npInsert(dbID, func, table, &chunks->chunk[index]);
		chunkPtr = npGetChunk(chunks, index);
		statement = new_npMysqlStatementInsertFromChunk("node_tbl", chunkPtr);
		queryReturnValue = (int)func->query(dbID, statement);
		
		//printf("node value%s\n", chunks->chunk[index]);
	}
}


/// @todo make npCreateTable2 part of the function set and move to mysql
int npCreateTable2(struct dbFunction *db, int dbID, char* table, char* fields)
{
	char* statement = npMysqlStatementCreateTable(table, fields);
	(*db->query)(dbID, statement);
	free(statement);

	return 0;
}

void npNewFreeChunks(struct newChunksObj * chunks, void* dataRef)
{
	int chunkIndex = 0;
	/*
	int nodeIndex  = 0; // Warning, lde
	int totalNodes = 0; // Warning, lde
	*/
	 
	for(chunkIndex = 0; chunkIndex <= chunks->numOfChunks; chunkIndex++)
	{
		free(chunks->chunk[chunkIndex].csvObjects->csvObj); 
		free(chunks->chunk[chunkIndex].csvObjects); 
	}

	free(chunks->chunk); 
	free(chunks); 

	return;
}

/* // old, lde
int npAttachDbsToDataRef(struct databases *dbs, void* dataRef)
{
	pData data = (pData) dataRef;
	data->io.dbs = dbs;
	return 0;
}
*/


// This function might be obsolete, lde @todo
int npdbOpen( pNPdatabase database, void* dataRef ) 
{
	// pData data = (pData) dataRef; // Warning, lde

//	dbID = npMysqlConnect( host, data );

	return 0;
}

/// iterate through list of hosts and attempt to connect to each

//zzd r
int npOpenDb(struct database *db)
{
	int err = 0; // This has use in this function, keep lde
	MYSQL *connInit = NULL;	/// pointer to connection handler
	MYSQL *conn = NULL;

	db->port = 3306; //zzd r
	db->currentlyUsedDatabase[0] = '\0';	//zzd r

	/// initialize the database connection structure
	connInit = (MYSQL*)(*db->dbFunc->init)(NULL);
	if( !connInit )
	{
		printf( "err 5566 - mysql_init() failed, likely out of memory \n" );
		return 0;
	}

	/// connect to the host using default database by passing in "" for DB
	conn = (MYSQL*)(*db->dbFunc->connect)( connInit, db->hostIP, 
									  db->user, db->password,
									  "", db->port, NULL, 0 );

	if( !conn )
	{
		//	printf( "MYSQL error: %u (%s)\n",func->errno(conn), func->error(conn));
		printf( "MYSQL err: %u - %s\n", (unsigned int)(*db->dbFunc->db_errno)(connInit), (char*)(*db->dbFunc->db_error)(connInit));
		return 0;
	}
	
	/// success, store our connection handle for this host connection
	db->id = (int)conn;
	
//	printf("db->dbID == %d  ", db->id); printf("dbFunc %p\n", db->db);

	return (int)conn;
}


//zzd r
// int npAddDb(struct databases *dbs, char* dbType, char* hostIP, char* user, char* pass, char* dbName, void* dataRef)  // old, lde
int npAddDb(pNPdbs dbs, char* dbType, char* hostIP, char* user, char* pass, char* dbName, void* dataRef)
{
	int i = 0;

	/// initialize structure if this is first database (Server?) to be added.
	//if( dbs->numberOfDatabases == 0 ) // old, lde
	printf("\nhostCount is %d", dbs->hostCount);
	if(dbs->hostCount == 0)
	{
	//begin npInitDB() // Bring under npInitDb Func
		//dbs->activeDB = malloc(sizeof(struct database) * 1); // old, lde
		dbs->activeDB = malloc(sizeof(NPdatabase) * 1);
		//dbs->activeDB[0].dbFunc = malloc(sizeof(struct dbFunction)); // old, lde
		dbs->activeDB->host = malloc(sizeof(NPdbHost));
		dbs->activeDB->host->hostFuncSet = malloc(sizeof(NPdbFuncSet));
		dbs->inUseDB2[0] = '\0';

		/// @todo upgrade idMap to map from DB node_id to scene node ptr, fasted updates
		/// @todo support changes to scene graph structure when updating DB
		/// @todo add kNPnodeList type for npMalloc
		
		//dbs->activeDB[0].idMap = malloc( sizeof(int) * kNPnodeMax ); // old, lde
		//printf("1722 allocating idMap");
		dbs->activeDB->idMap = malloc( sizeof(int) * kNPnodeMax );
		//if( !dbs->activeDB[0].idMap ) return 1010; // old, lde
		if( !dbs->activeDB->idMap ) return 1010;

		/* // old, lde
		for(i = 0; i < kNPnodeMax; i++)
			dbs->activeDB[0].idMap[i] = -1;	/// @todo make idMap = 0 not -1
		*/ 
		
		for(i = 0; i < kNPnodeMax; i++)
			dbs->activeDB->idMap[i] = -1;
		 
		/// @todo copy the structures rather then just point to existing mem
		
		/* // old, lde
		strcpy( dbs->activeDB[0].hostIP, hostIP );
		strcpy( dbs->activeDB[0].user, user );
		strcpy( dbs->activeDB[0].password, pass );
		strcpy( dbs->activeDB[0].currentlyUsedDatabase, dbName ); // currentlyUsedDatabase now named inUseDB
		strcpy( dbs->activeDB[0].dbType, dbType );
		dbs->numberOfDatabases++;
		*/
		
		strcpy(dbs->activeDB->host->ip, hostIP);
		strcpy(dbs->activeDB->host->user, user);
		strcpy(dbs->activeDB->host->password, pass);
		strcpy(dbs->activeDB->host->inUseDB, dbName);
		dbs->dbCount++;
		
	}
	else
	{
		//! @todo add support for multiple databases
		// dbs->activeDB = realloc
		printf("\n2 err 9494 - MySQL currently only supports 1 DB\n");
	}

	return 0;
}
//zz db2 - end


//zz
//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
int npdbPushScene ( void* dbID, const char* dbName, void* dataRef )							//add to ctrl loop, debug zz
{
	int err = 0;

	// pData data = (pData) dataRef;

	struct csvStrObjects *nodes = NULL;
	struct newChunksObj *chunks = NULL;


	pData data = (pData) dataRef;

	//struct database *myDb = &data->io.dbs->activeDB[0]; // old, lde
	pNPdatabase myDb = data->io.db.activeDB;
//	struct dbFunction *myDbFuncs = data->io.dbs->activeDB[0].dbFunc;
	pNPdbFuncSet myDbFuncs = data->io.db.activeDB->host->hostFuncSet;
	
	
	//pNPdatabases dbList = ((struct databases*)data->io.dbs)->dbList; // old, lde
	pNPdatabase *dbList = data->io.db.dbList; // hmm
	
	
	// printf( "Save DB Update: %s  host: %s", dbName, hostName );

//	err = npUseDatabase2(dbID, myDbFuncs, (char*)dbName);
	if (err) return err;

	//strcpy(myDb->currentlyUsedDatabase, dbName); // Not necessary at present, structure has changed, lde // old, lde
	strcpy(myDb->name, dbName);
	
	// npSelect(dbID, myDbFuncs, "node_tbl");

	//if DB exists then truncate it to delete all rows for update
//	err = npdbTruncate( dbID, myDbFuncs, "node_tbl" );
	err = npdbTruncate( myDb->host->conn, myDbFuncs, "node_tbl");

	if( err ) return err;

	printf("npdbPushScene DB: TRUNCATE node_tbl FROM %s\n", myDb->name);


	//insert the current scene nodes into the empty DB
	nodes = npRevisedNodeValues( dataRef ); // cleanup these function names, lde @todo
	chunks = npEvenNewerAllChunk( nodes, dataRef ); 
	
//	npInsertAllChunks( chunks, dbID, &data->io.dbs->activeDB[0], "node_tbl");
	npInsertAllChunks( chunks, myDb->host->conn, myDbFuncs, "node_tbl" );

	// printf( "Done Saving DB Update: %s  host: %s", dbName, hostName );

	npNewFreeChunks( chunks, dataRef );

	return err;
}

/*
int npdbLoadNodeTbl(int menuItem, void* dataRef)
{
/*	char msg[4096];
	pData data = (pData) dataRef;
	struct database *myDb = &data->io.dbs->activeDB[0];
	struct dbFunction *myDbFuncs = data->io.dbs->activeDB[0].db;

	int myConnid = data->io.dbs->activeDB[0].id;
	int success = 0;

	pNPdatabases dbList = ((struct databases*)data->io.dbs)->dbList;

	char* selectedItem = dbList->list[menuItem];
	MYSQL_RES *myResult;

	printf("\n-----YOU SELECTED %s-----\n", selectedItem);
	
	success = npUseDatabase2(myConnid, myDbFuncs, selectedItem);
	strcpy(myDb->currentlyUsedDatabase, selectedItem);

	npSelect(myConnid, myDbFuncs, "node_tbl");
	*/
/*
	if( (myResult = (*data->io.dbs->activeDB[0].db->storeResult)(myConnid)) == NULL )
		printf("\nError storing result");
	
	//dbTagsResult = npNewdbCtrl(connect, TABLE, TAGS_TABLE, "tagsTable", NULL, NULL, NULL, NULL, Select, dataRef);
	
	npdbLoadNodes(myResult, dataRef);

	npSelectNode(data->map.node[kNPnodeRootPin], data);
	npPostMsg("Done Loading Database", kNPmsgDB, data);

	sprintf(msg, "%d nodes loaded from database", (data->map.nodeCount)-31); //This isn't accurate super accurate, please disregard it...I'll fix it later.
	npPostMsg(msg, kNPmsgDB, dataRef);


	return 0;
}
*/

//zzd r
// old void* npdbGetList(struct database *db, void* dataRef ) , lde
/*
void* npdbGetList(pNPdatabase db, void* dataRef ) 
{
	int i = 0, j= 0;

	void* myResult = NULL;
	int numFields = 0;
	int numRows = 0;

	unsigned long* fieldLengths;
	MYSQL_ROW row;
	pNPdatabases dbList = NULL;				//zzsql


	npShowDatabases(db->id, db->dbFunc, dataRef);

	myResult = (*db->dbFunc->storeResult)(db->id);
	if( !myResult )
	{
		npPostMsg("err 5565 - npdbGetList failed to storeResult", kNPmsgErr, dataRef );
		goto abort;
	}
	
	numFields = (int)(*db->dbFunc->db_num_fields)(myResult);
	if( numFields != 1 )
	{
		npPostMsg( "err 5566 - npdbGetList numFields != 1", kNPmsgErr, dataRef );
		goto abort;
	}//	numRows = (*db->db_num_rows)(myResult);

	numRows = (int)(*db->dbFunc->db_num_rows)(myResult);
	if( !numRows )
	{
		npPostMsg( "err 5567 - npdbGetList numRows < 1", kNPmsgErr, dataRef );
		goto abort;
	}
	printf( "Databases: %d\n", numRows );

	dbList = malloc( sizeof(NPdatabases)); //zz was 'struct NPdatabases' ???
	if( !dbList )
	{
		npPostMsg("err 5568 - malloc failed to create dbList", kNPmsgErr, dataRef);
		goto abort;
	}

	//zz update this to allocate a dbItemList and swap(double buffer) with existing global ptr then free the old
	dbList->list = malloc( sizeof(char*) * numRows);
	if( !dbList->list )
	{
		npPostMsg("err 5569 - malloc failed to create dbList->list", kNPmsgErr, dataRef);
		goto abort;
	}

//	row = (*db->db->db_fetch_row)(myResult)) )
//	{
//		if( !row )
//		{
//			npPostMsg("err 5570 - no", kNPmsgErr, dataRef);
//			goto abort;
//		}
//	//while 'i' loop with inner for 'j' loop


	//zz add proper MySQL error checking for all commands
	// http://dev.mysql.com/doc/refman/5.0/en/mysql-fetch-row.html
	while( (row = (*db->dbFunc->db_fetch_row)(myResult)) )
	{
		//add error checking for malloc, fetch_row and fetch_lengths //zz debug
		fieldLengths = (*db->dbFunc->db_fetch_lengths)(myResult);
		if( !fieldLengths )
		{
			npPostMsg("err 5571 - malloc failed to create dbList->list[x]", kNPmsgErr, dataRef);
			goto abort;
		}

		//zz for loop is not needed because numFields = 1
		//would not work for multiple fields either, as 
		for(j = 0; j < numFields; j++)
		{
			dbList->list[i] = malloc(sizeof(char) * (fieldLengths[j]));
			if( !dbList->list )
			{
				printf("fieldLengths[i]: %d\n", fieldLengths[i]);
				npPostMsg("err 5572 - malloc failed to create dbList->list[x]", kNPmsgErr, dataRef);
				goto abort;
			}
			row[j][fieldLengths[j]] = '\0';
			//row should be copied to new memory then mysql_free...
			dbList->list[i] = row[j];
			dbList->size = i;
		}
		i++;
	}

	(*db->dbFunc->freeResult)(myResult);
	return (void*)dbList;

abort:
	//zz add check DB for errors
	(*db->dbFunc->freeResult)(myResult); //important to do this to maintain connection
	return NULL;
}
*/

//------------------------------------------------------------------------------
int npdbUse_old( const char* dbName, void* dataRef )							//add to ctrl loop, debug zz
{
	int err = 0;
	int dbID = 0;

	//struct databases *dbs = NULL; // old, lde
	pNPdatabase db = NULL;
	//struct dbFunction *myDbFuncs = NULL; // old, lde
	pNPdbFuncSet hostFuncSet = NULL;

	pData data = (pData) dataRef;

	/// @todo support checking DB connection and re-connect on command


	//dbs = data->io.dbs; // old, lde
	db = data->io.db.activeDB;
	//myDbFuncs = data->io.dbs->activeDB[0].dbFunc; // old, lde
	hostFuncSet = data->io.db.activeDB->host->hostFuncSet;
	//dbID = dbs->activeDB[0].id; // old, lde
	dbID = db->id;
	
//	err = npUseDatabase2( (void*)dbID, &myDbFuncs, (char*)dbName );

	/// copy the DB name string to our currently used database reference
//	strcpy( data->io.dbs->activeDB[0].currentlyUsedDatabase, dbName ); // @todo, change to reflect new structure, lde
	strcpy( data->io.db.activeDB->name, dbName);
	
	return err;
}

 //zzd r
//------------------------------------------------------------------------------
/* //lde
void npMysqlHook_old (HINSTANCE dbLib, void* dataRef)
{
	pData data = (pData) dataRef;

	struct dbFunction* db = data->io.dbs->activeDB[0].db;

	db->init = (void*)GetProcAddress(dbLib, "mysql_init");
	db->connect = (void*)GetProcAddress(dbLib, "mysql_real_connect");
	db->query = (void*)GetProcAddress(dbLib, "mysql_query");
	db->storeResult = (void*)GetProcAddress(dbLib, "mysql_store_result");
	db->close = (void*)GetProcAddress(dbLib, "mysql_close");
	db->db_fetch_row = (void*)GetProcAddress(dbLib, "mysql_fetch_row");
	db->db_fetch_lengths = (void*)GetProcAddress(dbLib, "mysql_fetch_lengths");
	db->db_num_fields = (void*)GetProcAddress(dbLib, "mysql_num_fields");
	db->db_num_rows = (void*)GetProcAddress(dbLib, "mysql_num_rows");

	db->StatementCreate = npMysqlStatementCreate;
	db->StatementInsert = npMysqlStatementInsert;	
	db->StatementSelect = npMysqlStatementSelect; 
	db->StatementCreateTable = npMysqlStatementCreateTable;
	db->StatementUse = npMysqlStatementUse;
	db->StatementShow = npMysqlStatementShow;
	db->StatementDrop = npMysqlStatementDrop;

	return;
}
*/
 
//------------------------------------------------------------------------------
/* // old, lde
int npConnectDB (void* dataRef)
{
	pData data = (pData) dataRef;

	return npOpenDb( &data->io.dbs->activeDB[0] );

//zz dbz	npAttachDbsToDataRef(dbs, dataRef);
}
*/


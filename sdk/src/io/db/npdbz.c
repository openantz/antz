/* -----------------------------------------------------------------------------
*
*  npdbz.c
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

//#include "../../npdata.h"							//defines which OS to compile for
#include "npdbz.h"
#include <windows.h>		//DLL loader...	//zz dbz debug move DLL win code to /npos/...
//#include <errno.h>

#include "../../os/npos.h"
#include "../../data/npmapfile.h"

char* npMysqlCreateStatement(char* dbName);
char* npMysqlUseStatement(char* dbName);

// This is for a single node
// void (*nodeAction)(int id, void* dataRef), 

void assignNodePropertiesFromArray(char** row, pNPnode node)
{
	pNPnode nodeParent;
	if(node->type == 1)
	{
		return;
	}

	node->selected = atoi(row[3]);
	node->childIndex = atoi(row[7]);
	
	node->chInputID		= atoi(row[9]);
	node->chOutputID	= atoi(row[10]);
	node->chLastUpdated	= atoi(row[11]);
		
	node->average		= atoi(row[12]);
	node->interval		= atoi(row[13]); // Samples???
		
	node->auxA.x		= atoi(row[14]);
	node->auxA.y		= atoi(row[15]);
	node->auxA.z		= atoi(row[16]);
	node->auxB.x		= atoi(row[17]);
	node->auxB.y		= atoi(row[18]);
	node->auxB.z		= atoi(row[19]);
		
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
		
	node->colorIndex	= atoi(row[54]);
		
	node->color.r		= atoi(row[55]);
	node->color.g		= atoi(row[56]);
	node->color.b		= atoi(row[57]);
	node->color.a		= atoi(row[58]);
		
	node->colorFade		= atoi(row[59]);
	node->textureID		= atoi(row[60]);
		
	node->hide			= atoi(row[61]);
	node->freeze		= atoi(row[62]);
		
	//	node->center		= center;		//removed	
		
	node->topo			= atoi(row[63]);			//moved topo
	node->facet			= atoi(row[64]);		//added topo facet number
		
	node->autoZoom.x	= atoi(row[65]);	//moved down a slot
	node->autoZoom.y	= atoi(row[66]);
	node->autoZoom.z	= atoi(row[67]);
		
	//	node->scroll		= scroll;		//removed made space for facet, zz debug
		
	node->triggerHi.x	= atoi(row[68]);
	node->triggerHi.y	= atoi(row[69]);
	node->triggerHi.z	= atoi(row[70]);
		
	node->triggerLo.x	= atoi(row[71]);
	node->triggerLo.y	= atoi(row[72]);
	node->triggerLo.z	= atoi(row[73]);
		
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
		
	node->segments.x	= atoi(row[86]);		//grid segments were stored in node->data,
	node->segments.y	= atoi(row[87]);		//was node->data->segments.x
	node->segments.z	= atoi(row[88]);		//now node->segments.x
		
	node->tagMode		= atoi(row[89]);
	node->formatID		= atoi(row[90]);
	node->tableID		= atoi(row[91]);
	node->recordID		= atoi(row[92]);

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

void updateNodeFromMysqlRow (MYSQL_ROW *row, void* dataRef) // Generalize here
{
//	MYSQL_ROW row;
	pData data = (pData) dataRef;
	pNPnode node = NULL;
	pNPnode nodeParent = NULL;
	int id = 0, type = 0, branchLevel = 0, parentID = 0, x = 0, count = 0;

	id = atoi( (const char*)row[0] );
	node = npGetNodeByID(data->io.dbs->myDatabase[0].idMap[id], dataRef);
	assignNodePropertiesFromArray( (char**)row, node );
}

void updateNodesFromMysqlResult(MYSQL_RES *result, void* dataRef)
{
	MYSQL_ROW row;
	while( row = mysql_fetch_row( result ))
	{
		updateNodeFromMysqlRow( (MYSQL_ROW*)row, dataRef );
	}
}



void npLoadNodeStateResultIntoAntz(MYSQL_RES *result, void* dataRef)
{
	MYSQL_ROW row;
	//	int numFields, i = 0;
	int x = 0;
	int count = 0;
	//	buffer = malloc(100000);
	time_t start;
	time_t end;
	pNPnode node = NULL;
	pNPnode nodeParent = NULL;
	int id = 0, type = 0, branchLevel = 0, parentID = 0;
	// Node Data Start
	pData data = (pData) dataRef;
	
	
	// Node Data End
	
	//	memset(buffer, '\0', 1500);
	//numFields = mysql_num_fields(result);
	
	// This could be optimized, strcat needs to traverse to the end of buffer everytime it runs,
	// With each successive loop it takes longer. A better solution would be to simply insert row[i] and ',' from the end each time,
	// thus incurring no performance penalty.
	//	//zzdb printf("\nFIELDS: %s\n", result->fields); pause();
	start = time(NULL);
	while( row = mysql_fetch_row(result) )
	{
		////zzdb printf("\nNew Node", x);
		x++;
		id = atoi(row[0]);
		type = atoi(row[1]);
		branchLevel = atoi(row[5]);
		parentID = atoi(row[4]);
		node = npMapNodeAdd (id, type, branchLevel, parentID, kNPmapNodeCSV, dataRef); // kNPmapNodeCSVvTwo is format		
		data->io.dbs->myDatabase[0].idMap[id] = node->id;
		assignNodePropertiesFromArray(row, node);
		/*
		node->selected = atoi(row[3]);
		node->childIndex = atoi(row[7]);
		
		node->chInputID		= atoi(row[9]);
		node->chOutputID	= atoi(row[10]);
		node->chLastUpdated	= atoi(row[11]);
		
		node->average		= atoi(row[12]);
		node->interval		= atoi(row[13]); // Samples???
		
		node->auxA.x		= atoi(row[14]);
		node->auxA.y		= atoi(row[15]);
		node->auxA.z		= atoi(row[16]);
		node->auxB.x		= atoi(row[17]);
		node->auxB.y		= atoi(row[18]);
		node->auxB.z		= atoi(row[19]);
		
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
		
		node->colorIndex	= atoi(row[54]);
		
		node->color.r		= atoi(row[55]);
		node->color.g		= atoi(row[56]);
		node->color.b		= atoi(row[57]);
		node->color.a		= atoi(row[58]);
		
		node->colorFade		= atoi(row[59]);
		node->textureID		= atoi(row[60]);
		
		node->hide			= atoi(row[61]);
		node->freeze		= atoi(row[62]);
		
		//	node->center		= center;		//removed	
		
		node->topo			= atoi(row[63]);			//moved topo
		node->facet			= atoi(row[64]);		//added topo facet number
		
		node->autoZoom.x	= atoi(row[65]);	//moved down a slot
		node->autoZoom.y	= atoi(row[66]);
		node->autoZoom.z	= atoi(row[67]);
		
		//	node->scroll		= scroll;		//removed made space for facet, zz debug
		
		node->triggerHi.x	= atoi(row[68]);
		node->triggerHi.y	= atoi(row[69]);
		node->triggerHi.z	= atoi(row[70]);
		
		node->triggerLo.x	= atoi(row[71]);
		node->triggerLo.y	= atoi(row[72]);
		node->triggerLo.z	= atoi(row[73]);
		
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
		
		node->segments.x	= atoi(row[86]);		//grid segments were stored in node->data,
		node->segments.y	= atoi(row[87]);		//was node->data->segments.x
		node->segments.z	= atoi(row[88]);		//now node->segments.x
		
		node->tagMode		= atoi(row[89]);
		node->formatID		= atoi(row[90]);
		node->tableID		= atoi(row[91]);
		node->recordID		= atoi(row[92]);
		//node->size		= lineSize;				// handled during node creation
		
		//support for first version CSV
		//	if (format == kNPmapNodeCSVvOne)
		//	npMapCSVvOne (node);
		
		//file compatability prior to 2012-04-22
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
		*/
		
	}
	//buffer[count] = '\0';
	end = time(NULL);
	//zzdb printf("\nDifference TIME : %f\n", difftime(end, start));
	//	//zzdb printf("\nNumber of nodes added : %d", x);
	//	//zzdb printf("\nDONE PARSING : %s\n", buffer);// pause();
	////zzdb printf("\nnpParseResultMySQL :: \n%s\n", buffer);
	
	
	if(result != NULL)
	{
		mysql_free_result(result);
	}
	//	return buffer;
}

struct newChunksObj* npInitAllChunkObjects(struct newChunksObj *chunks, void* dataRef)
{
	int chunkIndex = 0;

	for(chunkIndex = 0; chunkIndex < chunks->numOfChunks; chunkIndex++)
	{
		if( (chunks->chunk[chunkIndex].csvObjects = malloc(sizeof(struct csvStrObjects))) == NULL )
			getch();				//zz db debug???
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
	_ASSERTE( _CrtCheckMemory( ) );
	if(csvObjects->csvObj == NULL)
	{
		printf("\nMalloc Failed");
	}

	_ASSERTE( _CrtCheckMemory( ) );
//	printf("\nAssign num");
	csvObjects->numOfcsvStrObjects = numOfObjects;

	for(i = 0; i < csvObjects->numOfcsvStrObjects; i++)
	{
//		printf("\ncsvObjects->csvObj[%d] : %p",i, &csvObjects->csvObj[i]);
	}


	_ASSERTE( _CrtCheckMemory( ) );
}

struct newChunksObj* npGeneralAddObjectsToChunks(struct newChunksObj* chunks, struct csvStrObjects* strObjects, void* dataRef)
{
	struct csvStrObj * chunkStartPtr;
	struct csvStrObj * chunkEndPtr;
	int chunkSize =  0;
	int chunkLock =  0; // 1 == Locked, 0 == Unlocked
	int chunkNum  =  0;
	int objNum    =  0;
	int i         =  0;

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
	printf("\nWhat is sizeof(struct newChunkObj)?\n%d", sizeof(struct newChunkObj));
	//getch();
//	chunks->chunk = malloc(sizeof(struct newChunkObj) * chunks->numOfChunks);
	chunks->chunk = malloc(sizeof(struct newChunkObj) * chunks->numOfChunks);
	printf("\nErrno : %s\n", strerror(errno));
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
		getch();									//zzsql	
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
	struct newChunksObj *chunks;
	
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
	chunks = npNewInitChunksObj(chunks, csvObjects->totalCsvStrObjectsSize ,65535, dataRef);
	chunks = npInitAllChunkObjects(chunks, dataRef); 
	chunks = npGeneralAddObjectsToChunks(chunks, csvObjects, dataRef);

	return chunks;

}

void npNewMapTraverseTreeLL(struct csvStrObjects *nodes, int* index, pNPnode node, int format, void* dataRef)
{
	pData data = (pData) dataRef;
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
	pData data = (pData) dataRef;
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
	int totalCount = 0;
	int nodeIndex = 0;
	int x = 0;
	int y = 0;
	
//	nodes = NULL;
	printf("\ncalloc : %p");
/*
	if( (nodes = malloc(sizeof(struct csvStrObjects))) == NULL )
	{
		printf("\nnodes null");
	}
*/

	_ASSERTE( _CrtCheckMemory( ) );
//	nodes = malloc(sizeof(nodes));
	_ASSERTE( _CrtCheckMemory( ) );
	printf("\n%p", nodes);

	printf("node att1 : %p", nodes);
	printf("\nglobalCount : %d", data->map.globalsCount);
	nodes->numOfcsvStrObjects = data->map.nodeCount;
	printf("\nnode att2 : %d", nodes->numOfcsvStrObjects);
	//getch();
	nodes->totalCsvStrObjectsSize = 0; //Temp
	nodes->csvObj = NULL;	
	_ASSERTE( _CrtCheckMemory( ) );
	printf("\ncalloc2 : %p", nodes->csvObj);

	_ASSERTE( _CrtCheckMemory( ) );
	if( (nodes->csvObj = malloc((nodes->numOfcsvStrObjects*2) * sizeof(struct csvStrObj))) == NULL )
	{
		printf("\nNULL");
	}
	printf("\nsizeof(int) : %d", sizeof(int));
//	printf("\nsizeof(struct csvNode *node) : %d", sizeof(struct csvNode*));

	printf("\nfor 1 : %p", nodes->csvObj);
	printf("\nSizeof(nodes) : %d", sizeof(nodes));
//	printf("\nSizeof(struct csvNodes) : %d", sizeof(struct csvNodes));
	(struct csvStrObjects*)nodes->totalCsvStrObjectsSize = 0;
	_ASSERTE( _CrtCheckMemory( ) );
	for(nodeIndex = 0; nodeIndex <= (nodes->numOfcsvStrObjects); nodeIndex++) //Removed <=
	{
		printf("\nnodeIndex : %d : %p", nodeIndex, nodes->csvObj[nodeIndex]);
		
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
		
	_ASSERTE( _CrtCheckMemory( ) );
	x = 0;
	printf("\nfor 2 : %d", data->map.nodeRootCount);
	//getch();
	for(nodeIndex = 0; nodeIndex < (data->map.nodeRootCount); nodeIndex++)
	{
		printf("\nMain Index %d", nodeIndex);
		node = data->map.node[nodeIndex];
		_ASSERTE( _CrtCheckMemory( ) );
		
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

	_ASSERTE( _CrtCheckMemory( ) );
		//	printf("\nnodes->totalCsvStrObjectsSize : %d", nodes->totalCsvStrObjectsSize);
			nodes->totalCsvStrObjectsSize += nodes->csvObj[x].size;
	_ASSERTE( _CrtCheckMemory( ) );
		//	printf("\nc");
			nodes->csvObj[x].csvStr[(nodes->csvObj[x].size)] = '\0'; //Adding -1
	_ASSERTE( _CrtCheckMemory( ) );
		//	printf("\nd");

			x++;
		//	printf("\nnode->childCount : %d", node->childCount);
			if (node->childCount)
			{
				npNewerMapTraverseTreeLL(nodes, &x, node, kNPmapNodeCSV, dataRef);
	_ASSERTE( _CrtCheckMemory( ) );
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
	_ASSERTE( _CrtCheckMemory( ) );

	return nodes;
}



char* npNewGenMysqlFields(int count, int type, void* dataRef)
{
	int i = 0;
	int itemCount = 0;

	pData data = (pData) dataRef;
	pNPmapType map = NULL;
	char* fields = malloc(2000);
	char msg[128];
	
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
					sprintf( msg, "warn 7272 - unknown kNPtype: %d", map[i].type );	//zz dbz
					npPostMsg( msg, kNPmsgDB, data );
					count += sprintf ((fields + count), "%s INT(10)", map[i].name );
					break;
			}
		}

//		printf("\ncount : %d", count); //getch();;
	//	count += sprintf ((((struct mysqlTable*)object->myobject)->fields + count), "\n");
	
	//return count;
	return fields;
}


char* npMysqlCreateTableStatement(char* table, char* fields)
{
	char* statement = malloc(sizeof(char) * (16 + strlen(table) + strlen(fields)));
	sprintf(statement, "CREATE TABLE %s(%s)", table, fields);
	return statement;
}

int npCreateTable2(struct dbFunction *db, int dbID, char* table, char* fields)
{
	char* statement = npMysqlCreateTableStatement(table, fields);
	(*db->query)(dbID, statement);
	free(statement);

	return 0;
}

char* npMysqlSelectStatement(char* table)
{
	char* statement = malloc(sizeof(char) * (15 + strlen(table)));
	int count = 0;

	count = sprintf(statement, "SELECT * FROM %s", table);
	return statement;
}

char* npMysqlInsertStatement(char* table, struct newChunkObj *value)
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

void npInsert(int dbID, struct dbFunction *db, char* table, struct newChunkObj *value)
{
	int queryReturnValue = 0;
	char* statement = NULL;
	statement = (*db->createInsertStatement)(table, value);
	queryReturnValue = (int)(*db->query)(dbID, statement);
	free(statement);
}

int npSelect(int dbID, struct dbFunction *db, char* table) //Add field(s) choice later
{
	int error = 0;
	char* statement = (*db->createSelectStatement)(table);
	error = (int)(*db->query)(dbID, statement); //Create a function to process mysql error codes
	//printf("\nreturned value : %d\n", queryReturnValue);
	//printf("\nnpSelect error : %d", error);
	free(statement);
	return error; //Zero for success, nonzero if error occurred
}

char* npMysqlDropStatement(char* dropType, char* dropName)
{
	char* statement = malloc(sizeof(char) * (7 + strlen(dropType) + strlen(dropName)));
	sprintf(statement, "DROP %s %s", dropType, dropName);

	return statement;
}

char* npMysqlShowStatement(char* showWhat)
{
	char* statement = malloc(sizeof(char) * ( 6 + strlen(showWhat) ) );
	sprintf(statement, "SHOW %s", showWhat);

	return statement;
}

/*
char* npMysqlInsertStatement(char* table)
{
	int count = 0;
	char* statement = malloc(sizeof(char) * (20 + strlen(table))); //Account for all parens, debug db
	count = sprintf(statement, "INSERT INTO %s VALUES ", table);
	
	return statement;
}
*/

char* npMysqlCreateStatement(char* dbName)
{
	char* statement = malloc(sizeof(char) * ( 17 + strlen(dbName) ) ); //16 for "CREATE DATABASE " + 1 for null terminator
	sprintf(statement, "CREATE DATABASE %s", dbName); // This should be dynamic

	return statement;
}

char* npMysqlUseStatement(char* dbName)
{
	int count = 0;
	char* statement = malloc(sizeof(char) * ( 5 + strlen(dbName) ) ); //4 for "USE " + 1 for null terminator
	count = sprintf(statement, "USE %s", dbName); // This should be dynamic
	statement[count] = '\0';

	return statement;
}

int npMysqlServerCtrl(void (*ctrlFunction)(void*), void* parameters);

int npMysqlServerCtrl(void (*ctrlFunction)(void*), void* parameters)
{

}


int dbHook(struct dbFunction *db, char* filePath, int dbtype)
{
	HINSTANCE dbLibrary;
//	FARPROC dbInit;
//	FARPROC dbConnect;

	dbLibrary = LoadLibrary(filePath); 
	printf("\ndbLibrary %p", dbLibrary);

	db->init = GetProcAddress(dbLibrary, "mysql_init");
	db->connect = GetProcAddress(dbLibrary, "mysql_real_connect");
	db->query = GetProcAddress(dbLibrary, "mysql_query");
	db->storeResult = GetProcAddress(dbLibrary, "mysql_store_result");
	db->close = GetProcAddress(dbLibrary, "mysql_close");
	db->db_fetch_row = GetProcAddress(dbLibrary, "mysql_fetch_row");
	db->db_fetch_lengths = GetProcAddress(dbLibrary, "mysql_fetch_lengths");
	db->db_num_fields = GetProcAddress(dbLibrary, "mysql_num_fields");
	db->db_num_rows = GetProcAddress(dbLibrary, "mysql_num_rows");

	db->createStatement = npMysqlCreateStatement;
	db->createInsertStatement = npMysqlInsertStatement;	
	db->createSelectStatement = npMysqlSelectStatement; 
	db->createTableStatement = npMysqlCreateTableStatement;
	db->useStatement = npMysqlUseStatement;
	db->showStatement = npMysqlShowStatement;
	db->dropStatement = npMysqlDropStatement;

	return 0;
}

//void interpretError(

void* npShowDatabases(int dbID, struct dbFunction *db)
{
	char* statement;
	void* error = 0;
	printf("\nA"); 
	statement = (*db->showStatement)("Databases");
	printf("\nB : %s", statement);
	error = (*db->query)(dbID, statement);
//	error = mysql_query(dbID, statement);
	printf("\nC");
	free(statement);
	printf("\nD");

	return error;
}

//void* npdbGetList(int dbID, struct dbFunction *db, void* dataRef)
void* npdbGetList(struct database *db)
{
	void* myResult;
	int numFields = 0;
	int numRows = 0;
	int x = 0;
	int i = 0;
	unsigned long* fieldLengths;
	MYSQL_ROW row;
	pNPdatabases dbList = NULL;				//zzsql

	printf("\nBefore npShowDatabases");
	npShowDatabases(db->id, db->db);
	printf("\nAfter npShowDatabases");
//	myResult = (*db->storeResult)(db->dbID);
	printf("\nStore Result");
	myResult = (*db->db->storeResult)(db->id);

	printf("\nNum Fields");;
	numFields = (int)(*db->db->db_num_fields)(myResult);
//	numRows = (*db->db_num_rows)(myResult);
	printf("\nNum Rows");
	numRows = (int)(*db->db->db_num_rows)(myResult);

	printf("\nmalloc dbList");
	dbList = malloc(sizeof(struct NPdatabases));		//zzsql
	printf("\nmalloc dbList->list");
	dbList->list = malloc(sizeof(char*) * numRows);

	printf("\nBefore While");
	while ((row = (*db->db->db_fetch_row)(myResult)))
	{
		fieldLengths = (*db->db->db_fetch_lengths)(myResult);

		for(i = 0; i < numFields; i++)
		{
			dbList->list[x] = malloc(sizeof(char) * (fieldLengths[i]));
			row[i][fieldLengths[i]] = '\0';
			dbList->list[x] = row[i];
			dbList->size = x;
		}
		x++;
	}
	printf("\nAfter While");
	mysql_free_result(myResult); //abstract away

	return (void*)dbList;
}

int npUseDatabase2(int dbID, struct dbFunction *db, char* dbName)
{
	int error;
	char* statement = (*db->useStatement)(dbName);
	error = (int)(*db->query)(dbID, statement);

	return 0;
}


//Should queries be renamed to statements
int npCreateDatabase2(int dbID, struct dbFunction *db, char* dbName)
{
	char* statement = (*db->createStatement)(dbName);
	(*db->query)(dbID, statement);
	free(statement);

	return 0;
}

int npDropDatabase(int dbID, struct dbFunction *db, const char* dbName)
{
	char* statement = (*db->dropStatement)("DATABASE", dbName);
	(*db->query)(dbID, statement);
	free(statement);

	return 0;
}

int npDropTable(int dbID, struct dbFunction *db, char* table)
{
	char* statement = (*db->dropStatement)("TABLE", table);
	(*db->query)(dbID, statement);
	free(statement);

	return 0;
}

int npdbTruncate(int dbID, struct dbFunction *db, char* table)
{
	char statement[128];

	sprintf( statement, "%s%s","TRUNCATE ", table);
	(*db->query)(dbID, statement);

	return 0;
}

int npConnectToDatabaseServer(struct dbNewConnect *connect, void* dataRef)
{
	
	return 0;
}

/*
void main2(void* dataRef)
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
	npLoadNodeStateResultIntoAntz(myResult, dataRef);
	printf("\nAfter");
	
	//	npMysqlInsertStatement("node_tbl", &chunks->chunk[0]);
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

void npInsertAllChunks(struct newChunksObj *chunks, int dbID, struct database *db, char* table)
{
	int index = 0;
	for(; index <= chunks->numOfChunks; index++)
		npInsert(dbID, db->db, table, &chunks->chunk[index]);
}

int npdbLoadNodeTbl(int menuItem, void* dataRef)
{
	char msg[4096];
	pData data = (pData) dataRef;
	struct database *myDb = &data->io.dbs->myDatabase[0];
	struct dbFunction *myDbFuncs = data->io.dbs->myDatabase[0].db;

	int myConnid = data->io.dbs->myDatabase[0].id;
	int success = 0;

	pNPdatabases dbList = ((struct databases*)data->io.dbs)->dbList;

	char* selectedItem = dbList->list[menuItem];
	MYSQL_RES *myResult;

	printf("\n-----YOU SELECTED %s-----\n", selectedItem);
	
	success = npUseDatabase2(myConnid, myDbFuncs, selectedItem);
	strcpy(myDb->currentlyUsedDatabase, selectedItem);

	npSelect(myConnid, myDbFuncs, "node_tbl");

	if( (myResult = (*data->io.dbs->myDatabase[0].db->storeResult)(myConnid)) == NULL )
		printf("\nError storing result");
	
	//dbTagsResult = npNewdbCtrl(connect, TABLE, TAGS_TABLE, "tagsTable", NULL, NULL, NULL, NULL, Select, dataRef);
	
	npLoadNodeStateResultIntoAntz(myResult, dataRef);

	npSelectNode(data->map.node[kNPnodeRootPin], data);
	npPostMsg("Done Loading Database", kNPmsgDB, data);

	sprintf(msg, "%d nodes loaded from database", (data->map.nodeCount)-31); //This isn't accurate super accurate, please disregard it...I'll fix it later.
	npPostMsg(msg, kNPmsgDB, dataRef);


	return 0;
}

void npdbUpdateAntzStateFromDatabase(void* dataRef)
{
	pData data = (pData) dataRef;
	MYSQL_RES *myResult;
//	MYSQL_ROW row;				//zz db
	pNPnode node = NULL;
	pNPnode nodeParent = NULL;
	int dbNodeID = 0;
	int i = 0;
//	int **idMap = data->io.dbs->myDatabase[0].idMap;

	printf("\nnpdbUpdateAntzStateFromDatabaseTTTTTTTTTTT\n\n");
//	getch();
	/*
	for(i = 0; i < kNPnodeMax; i++) // First index is database id, second index is antz id
	{
		printf("\n%d : %d", data->io.dbs->myDatabase[0].idMap[i][0], data->io.dbs->myDatabase[0].idMap[i][1]);
		if(data->io.dbs->myDatabase[0].idMap[i][0] == -1 && data->io.dbs->myDatabase[0].idMap[i][1] == -1)
		{
			break;
		}

	}
	*/
	// Index is database id, box contains antz id
	// Do a select, iterate against idMap, perform updates CATMANDO
//	printf("\nBefore Select : %s", data->io.dbs->myDatabase[0].currentlyUsedDatabase);
	if(data->io.dbs->myDatabase[0].currentlyUsedDatabase[0] != '\0')
	{
		npSelect(data->io.dbs->myDatabase[0].id, data->io.dbs->myDatabase[0].db, "node_tbl");
	
//	printf("\nAfter Select");

	if( (myResult = (*data->io.dbs->myDatabase[0].db->storeResult)(data->io.dbs->myDatabase[0].id)) == NULL )
		printf("\nError storing result");

	updateNodesFromMysqlResult(myResult, dataRef);
	}
	/*
	while( row = mysql_fetch_row(myResult) )
	{
		dbNodeID = atoi(row[0]);
		node = npGetNodeByID(data->io.dbs->myDatabase[0].idMap[dbNodeID], dataRef);
		printf("\nThe dbNodeID is %d", dbNodeID);
		printf("\nThe internal antz node id is %d", node->id);
		
		node->type = atoi(row[1]);
		node->branchLevel = atoi(row[5]);
		//parentID = atoi(row[4]);

		node->selected = atoi(row[3]);
		node->childIndex = atoi(row[7]);
		
		node->chInputID		= atoi(row[9]);
		node->chOutputID	= atoi(row[10]);
		node->chLastUpdated	= atoi(row[11]);
		
		node->average		= atoi(row[12]);
		node->interval		= atoi(row[13]); // Samples???
		
		node->auxA.x		= atoi(row[14]);
		node->auxA.y		= atoi(row[15]);
		node->auxA.z		= atoi(row[16]);
		node->auxB.x		= atoi(row[17]);
		node->auxB.y		= atoi(row[18]);
		node->auxB.z		= atoi(row[19]);
		
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
		
		node->colorIndex	= atoi(row[54]);
		
		node->color.r		= atoi(row[55]);
		node->color.g		= atoi(row[56]);
		node->color.b		= atoi(row[57]);
		node->color.a		= atoi(row[58]);
		
		node->colorFade		= atoi(row[59]);
		node->textureID		= atoi(row[60]);
		
		node->hide			= atoi(row[61]);
		node->freeze		= atoi(row[62]);
		
		//	node->center		= center;		//removed	
		
		node->topo			= atoi(row[63]);			//moved topo
		node->facet			= atoi(row[64]);		//added topo facet number
		
		node->autoZoom.x	= atoi(row[65]);	//moved down a slot
		node->autoZoom.y	= atoi(row[66]);
		node->autoZoom.z	= atoi(row[67]);
		
		//	node->scroll		= scroll;		//removed made space for facet, zz debug
		
		node->triggerHi.x	= atoi(row[68]);
		node->triggerHi.y	= atoi(row[69]);
		node->triggerHi.z	= atoi(row[70]);
		
		node->triggerLo.x	= atoi(row[71]);
		node->triggerLo.y	= atoi(row[72]);
		node->triggerLo.z	= atoi(row[73]);
		
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
		
		node->segments.x	= atoi(row[86]);		//grid segments were stored in node->data,
		node->segments.y	= atoi(row[87]);		//was node->data->segments.x
		node->segments.z	= atoi(row[88]);		//now node->segments.x
		
		node->tagMode		= atoi(row[89]);
		node->formatID		= atoi(row[90]);
		node->tableID		= atoi(row[91]);
		node->recordID		= atoi(row[92]);
		//node->size		= lineSize;				// handled during node creation
		
		//support for first version CSV
		//	if (format == kNPmapNodeCSVvOne)
		//	npMapCSVvOne (node);
		
		//file compatability prior to 2012-04-22
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
	*/
}


//---------------------------------------------------------------------------
int npdbSaveAs( int dbID, const char* dbName, void* dataRef )
{
	int err = 0;
	char* nodeFields = NULL;

	struct csvStrObjects *nodes = NULL;
	struct newChunksObj *chunks = NULL;
	struct dbFunction *myDbFuncs = NULL;

	pData data = (pData) dataRef;

	myDbFuncs = data->io.dbs->myDatabase[0].db;
	
	printf("\ndatabase name : %s\n", dbName);

	//if DB exists then truncate it, deletes all rows to do update
	//else create a new DB
	if (0)
	{
		err = npDropDatabase( dbID, myDbFuncs, dbName );
		return err;
	}
	else if( 0 )
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


//---------------------------------------------------------------------------
void npdbSaveScene( void* dataRef )
{
	int dbID = 0;
	char *dbName = malloc(65);	//max db identifier is 64 chars
	char defaultAnswer[256];
	char question[256];

	pData data = (pData) dataRef;

	dbID = data->io.dbs->myDatabase[0].id;

	nposTimeStampName( dbName );
/*
	strcpy( question, "Save New Database As?");
	strcpy( defaultAnswer, dbName );
	dbName = npConsoleAskUser( question, defaultAnswer, data );
*/
	npdbSaveAs( dbID, dbName, data );
}

void npNewFreeChunks(struct newChunksObj * chunks, void* dataRef)
{
	int chunkIndex = 0;
	int nodeIndex = 0;
	int totalNodes = 0;

	for(chunkIndex = 0; chunkIndex <= chunks->numOfChunks; chunkIndex++)
	{
		free(chunks->chunk[chunkIndex].csvObjects->csvObj); 
		free(chunks->chunk[chunkIndex].csvObjects); 
	}

	free(chunks->chunk); 
	free(chunks); 

	return;
}


int npAttachDbsToDataRef(struct databases *dbs, void* dataRef)
{
	pData data = (pData) dataRef;
	data->io.dbs = dbs;
	return 0;
}

int npOpenDb(struct database *db)
{
	int success = 0;
	int sqlinit = 0;
	int dbID  = 0;

	if( strcasecmp("mysql", db->dbType) <= 0 ) //Make switch statement
	{
		success = dbHook(db->db, "libmysql.dll", 0);
		db->port = 3306; //Move this
	}

	sqlinit = (int)(*db->db->init)(NULL); //db->db could be confusing....change to db->dbFunc
	db->id = (int)(*db->db->connect)(sqlinit, db->hostIP, db->user, db->password, "", db->port, NULL, 0);

	db->currentlyUsedDatabase[0] = '\0';
	printf("\ndb->dbID == %d", db->id);
	printf("dbFunc %p", db->db);
//	getch();

	return 0;
}

//zz db2
int npAddDb(struct databases *dbs, char* dbType, char* hostIP, char* user, char* pass, char* dbName, void* dataRef)
{
	int i = 0;
	if( dbs->numberOfDatabases == 0 )
	{
		dbs->myDatabase = malloc(sizeof(struct database) * 1);
		dbs->myDatabase[0].db = malloc(sizeof(struct dbFunction));
		
		for(i = 0; i < kNPnodeMax; i++)
		{
			dbs->myDatabase[0].idMap[i] = -1;
		}
	
		strcpy( dbs->myDatabase[0].hostIP, hostIP );
		strcpy( dbs->myDatabase[0].user, user );
		strcpy( dbs->myDatabase[0].password, pass );
		strcpy( dbs->myDatabase[0].currentlyUsedDatabase, dbName );
		strcpy( dbs->myDatabase[0].dbType, dbType );
		dbs->numberOfDatabases++;
	}
	else
	{
		// Todo, debug db
		// dbs->myDatabase = realloc
		printf("\nerr 9494 - MySQL currently only supports 1 DB\n");
	}

	return 0;
}
//zz db2 - end


//zz
//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
int npdbPushScene ( int dbID, const char* dbName, void* dataRef )							//add to ctrl loop, debug zz
{
int err = 0;
	char* nodeFields = NULL;
//	char* dbName = NULL;

	struct csvStrObjects *nodes = NULL;
	struct newChunksObj *chunks = NULL;
/*	struct dbFunction *myDbFuncs = NULL;

	pData data = (pData) dataRef;

//	if(data->io.dbs->myDatabase[0].currentlyUsedDatabase[0] != '\0')
//		npSelect(data->io.dbs->myDatabase[0].id, data->io.dbs->myDatabase[0].db, "node_tbl");
	printf("\ndatabase id: %d\n", dbID);
	myDbFuncs = data->io.dbs->myDatabase[0].db;
	dbName = selectedItem = dbList->list[menuItem];//((struct databases*)data->io.dbs)->dbList->list[dbID];
printf("\ndatabase id: %d\n", dbID);
	printf("database name : %s\n", dbName);
*/
	
	int success = 0;
	char msg[4096];
	char hostName[kNPurlMax] = "localhost";

	pData data = (pData) dataRef;

	struct database *myDb = &data->io.dbs->myDatabase[0];
	struct dbFunction *myDbFuncs = data->io.dbs->myDatabase[0].db;

	pNPdatabases dbList = ((struct databases*)data->io.dbs)->dbList;

	MYSQL_RES *myResult;

//	dbID = data->io.dbs->myDatabase[0].id;
//	dbName = dbList->list[dbID];

//	dbID = 38;
	printf( "push db_id: %d db_name: %s\n", dbID, dbName );

	success = npUseDatabase2(dbID, myDbFuncs, (char*)dbName);
	strcpy(myDb->currentlyUsedDatabase, dbName);

//	npSelect(dbID, myDbFuncs, "node_tbl");

	//if DB exists then truncate it to delete all rows for update
	err = npdbTruncate( dbID, myDbFuncs, "node_tbl" );
	if( err ) return err;

	printf("TRUNCATE node_tbl\n");

//		err = npUseDatabase2( dbID, myDbFuncs, (char*)dbName );
//		if( err ) return err;

	//insert the current scene nodes into the empty DB
	nodes = npRevisedNodeValues( dataRef ); 
	chunks = npEvenNewerAllChunk( nodes, dataRef ); 
	
	npInsertAllChunks( chunks, dbID, &data->io.dbs->myDatabase[0], "node_tbl");

	sprintf( msg, "Updated DB: %s host: %s", dbName, hostName );
	npPostMsg( msg, kNPmsgDB, data );

	npNewFreeChunks( chunks, dataRef );

	return err;
}

/*
int npdbLoadNodeTbl(int menuItem, void* dataRef)
{
/*	char msg[4096];
	pData data = (pData) dataRef;
	struct database *myDb = &data->io.dbs->myDatabase[0];
	struct dbFunction *myDbFuncs = data->io.dbs->myDatabase[0].db;

	int myConnid = data->io.dbs->myDatabase[0].id;
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
	if( (myResult = (*data->io.dbs->myDatabase[0].db->storeResult)(myConnid)) == NULL )
		printf("\nError storing result");
	
	//dbTagsResult = npNewdbCtrl(connect, TABLE, TAGS_TABLE, "tagsTable", NULL, NULL, NULL, NULL, Select, dataRef);
	
	npLoadNodeStateResultIntoAntz(myResult, dataRef);

	npSelectNode(data->map.node[kNPnodeRootPin], data);
	npPostMsg("Done Loading Database", kNPmsgDB, data);

	sprintf(msg, "%d nodes loaded from database", (data->map.nodeCount)-31); //This isn't accurate super accurate, please disregard it...I'll fix it later.
	npPostMsg(msg, kNPmsgDB, dataRef);


	return 0;
}
*/


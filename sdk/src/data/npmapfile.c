/* -----------------------------------------------------------------------------
*
*  npmapfile.c
*
*  ANTz - realtime 3D data visualization tools for the real-world, based on NPE.
*
*  ANTz is hosted at http://openantz.com and NPE at http://neuralphysics.org
*
*  Written in 2010-2016 by Shane Saxon - saxon@openantz.com
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

#include "npmapfile.h"


#include "../npdata.h"
#include "../data/npnode.h"
#include "../os/npos.h"
#include "../io/npfile.h"
#include "../io/npch.h"

//#include "../io/gl/nptags.h"	//move the necessary functions to npdata, debug zz
#include "../io/npgl.h"			//zz debug, added for npPostTool()

void npLoadChannelFile (char* buffer, int wordSize, int size, void* dataRef);


//-----------------------------------------------------------------------------
void npInitMapFile (void* dataRef)
{
	return;
}

//-----------------------------------------------------------------------------
void npCloseMapFile (void* dataRef)
{
	return;
}

//-----------------------------------------------------------------------------
// mode usage: 3 parameters "r/w/a" followed by optional "b" and optional "+"
// example mode = "rb+"
// r open file for reading
// w open file for overwriting. Create new file if doesn't already exist
// a open file for appending data. 
// b open file as a binary file
// + open file for read and writing
//-----------------------------------------------------------------------------
FILE* npOpenMapFile (const char* fileName, const char* mode, void* dataRef)
{
	FILE* file = NULL;

	file = fopen(fileName, mode); 

	if ( file == NULL )
	{
		printf("err 4244 - failed to open file\n");
		return NULL;
	}

	// add code to add the file ptr to the data->map							debug, zz

	return file;
}

//-----------------------------------------------------------------------------
FILE* npNewMapFile (const char* fileName, const char* mode, void* dataRef)
{
	return NULL;
}


//-----------------------------------------------------------------------------
int npReadMapFile (void* readBuffer, int elementSize, int elementCount, 
				 FILE* file, void* dataRef)
{
	int bytes = 0;

	bytes = fread (readBuffer, elementSize, elementCount, file);
	
	if ( bytes == 0 )
		printf("Failed Reading\n");
	else
		printf("Success Reading\n");

	return bytes;
}


//-----------------------------------------------------------------------------
FILE* npDialogMapFile (const char* fileName, int dialogType, void* dataRef)
{	
	
	switch (dialogType)
	{
		case kNPfileDialogNew : ; break;

		case kNPfileDialogOpen : ; break;
		case kNPfileDialogClose : ; break;

		case kNPfileDialogSave : ; break;
		case kNPfileDialogSaveAs : ; break;

		case kNPfileDialogImport : ; break;
		case kNPfileDialogExport : ; break;

		default : break;
	}

	return NULL;
}


//-----------------------------------------------------------------------------
int npWriteMapFile (const void* str, int wordSize, int size, FILE* file, void* dataRef)
{
	return fwrite( str, wordSize, strlen(str), file );
}


void npSaveMapFile (FILE* file, void* dataRef)
{
	return;
}


//-----------------------------------------------------------------------------
int npImportMapFile (FILE* file, int type, void* dataRef)
{
	int size = 0;
	return size;
}


//-----------------------------------------------------------------------------
// write the header structure
int npWriteMapHeader (char* bufferIndex, int count, int type, void* dataRef)
{
	int i = 0;
	int itemCount = 0;

	pNPmapType map = NULL;

	pData data = (pData) dataRef;
	
	map = data->map.typeMapNode;		//debug, zz

	if (bufferIndex == NULL || count < 5)
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
		default : 
			printf ("err 4381 - unknown type, cant write header field names");
			return 0;	
			break;
	}

	count = 0;
	for (i=0; i < itemCount; i++)
	{
		// insert a comma for CSV and JSON files
		//first field name to lead without a comma one without comma
		if (i > 0)
			count += sprintf ((bufferIndex + count), ",");

		switch (map[i].type)
		{
			case 0 :				// same as default
				count += sprintf ((bufferIndex + count), "%s", map[i].name );
				break;
			case kNPfloatXYZ : 
				count += sprintf ((bufferIndex + count), "%s_x,%s_y,%s_z",
									map[i].name, map[i].name, map[i].name );
				break;
			case kNPintXYZ : 
				count += sprintf ((bufferIndex + count), "%s_x,%s_y,%s_z",
									map[i].name, map[i].name, map[i].name );
				break;
			case kNPboolXYZ : 
				count += sprintf ((bufferIndex + count), "%s_x,%s_y,%s_z",
									map[i].name, map[i].name, map[i].name );	
				break;
			case kNPfloatXYZA : 
				count += sprintf ((bufferIndex + count), "%s_x,%s_y,%s_z,%s_a",
						map[i].name, map[i].name, map[i].name, map[i].name );
				break;
			case kNPfloatXYZS : 
				count += sprintf ((bufferIndex + count), "%s_x,%s_y,%s_z,%s_s",
						map[i].name, map[i].name, map[i].name, map[i].name );
				break;
			case kNPboolXYZS : 
				count += sprintf ((bufferIndex + count), "%s_x,%s_y,%s_z,%s_s",
						map[i].name, map[i].name, map[i].name, map[i].name );
				break;
			case kNPubyteRGBA : 
				count += sprintf ((bufferIndex + count), "%s_r,%s_g,%s_b,%s_a",
						map[i].name, map[i].name, map[i].name, map[i].name );
				break;
			case kNPubyteRGB : 
				count += sprintf ((bufferIndex + count), "%s_r,%s_g,%s_b",
									map[i].name, map[i].name, map[i].name );
				break;
			case kNPfloatRGBA : 
				count += sprintf ((bufferIndex + count), "%s_r,%s_g,%s_b,%s_a",
						map[i].name, map[i].name, map[i].name, map[i].name );
				break;
			case kNPfloatRGB : 
				count += sprintf ((bufferIndex + count), "%s_r,%s_g,%s_b", 
									map[i].name, map[i].name, map[i].name );
				break;

			default :				// same as case 0
				count += sprintf ((bufferIndex + count), "%s", map[i].name );
				break;
		}
	}

	count += sprintf ((bufferIndex + count), "\n");

	return count;
}


//-----------------------------------------------------------------------------
int npMapNodesToCSV (char* buffer, int size, int* rootIndex, void* dataRef)
{
	int i = 0, rootCount = 0, count = 0;

	pNPnode node = NULL;
	pData data = (pData) dataRef;

//	char* nodePtr = data->io.write;					//zz debug, pull this out from npWriteNode()

	data->io.write = buffer;

	//write the header only if it is the start of the root node array
	if (*rootIndex == 0)
	{
		count += npWriteMapHeader (buffer, size, kNPnode, dataRef);
		data->io.write += count;
	}

	// iterate through data map and write out each node
	//for (i=0; i < data->map.nodeRootCount; i++)
	rootCount = data->map.nodeRootCount;

	for (i = *rootIndex; i < rootCount; i++)
	{
		node = data->map.node[i];

		if (node->type != kNodeHUD )				//zz debug 
		{	
			//zzf
			if ( data->io.file.saveSelect )
			{
				if ( node->selected )
				{
					count += npWriteNode ((buffer + count), node, kNPmapNodeCSV, data);

					if (node->childCount)
						count += npMapTraverseTree ((buffer + count), node, kNPmapNodeCSV, data);
				}
			}
			else
			{
				count += npWriteNode ((buffer + count), node, kNPmapNodeCSV, data);

				if (node->childCount)
					count += npMapTraverseTree ((buffer + count), node, kNPmapNodeCSV, data);
			}
			//zzf end
		}

		if (count > kNPfileBlockSize)
			break;
	}

	*rootIndex = i + 1;	//i + 1	for next pass
	
//increment the buffer
//	data->io.write += count;	//zz debug

	return count;
}

// returns file version based on buffer contents
//-----------------------------------------------------------------------------
int npFileFormat(char* buffer, int wordSize, int size)
{
	int fileVer = 0;
	

	if (buffer == NULL)
		return 0;

	//original CSV version started with a newline, check for di
	if ( buffer[0] == '\n' || buffer[0] == '\r' )
		fileVer = kNPmapNodeCSVvOne;		//original CSV version 1
	else
		fileVer = kNPmapNodeCSV;		//otherwise assume ver 2 or later

	//update to make more robust, future proof, detailed comparison, debug zz

	return fileVer;
}

//-----------------------------------------------------------------------------
void npMapCSVvOne(pNPnode node)
{
	NPgridPtr gridData = NULL;

	node->recordID = (int)node->proximity.x;	//re-mapped

	node->lineWidth = 1.0f;				//original line width

	if (node->type == kNodePin)
	{
		//what was rotateRad.y is now rotate.z
		node->rotate.z = -node->rotate.y * kRADtoDEG;
		node->rotateRate.z = -node->rotateRate.y * kRADtoDEG;
		node->rotate.y = 0.0f;
		node->rotateRate.y = 0.0f;

		//for branchLevel 2 and above, translate.z offset is now translate.x
		//rotate.x is converted to translate.x
		if (node->branchLevel >= 2)	//child node rotation different from root
		{
			node->translate.z = node->translate.x;		// z = x

			if (node->branchLevel == 2)	//map orientation, 0 deg is N y-axis
				node->translate.x = node->rotate.x * kRADtoDEG - 90.0f;
			else
				node->translate.x = node->rotate.x * kRADtoDEG;// - 90.0f;

			node->translateRate.x = node->rotateRate.x * kRADtoDEG;
			node->rotate.x = 0.0f;
			node->rotateRate.x = 0.0f;
		}
	}
	else if (node->type == kNodeCamera)
	{
		node->rotate.x = -node->rotate.x * kRADtoDEG + 90.0f;
		node->rotate.y = node->rotate.y * kRADtoDEG;

		node->rotateRate.x = -node->rotateRate.x * kRADtoDEG;
		node->rotateRate.y = node->rotateRate.y * kRADtoDEG;

		node->rotate.z = 0.0f;
		node->rotateRate.z = 0.0f;
	}
	else if (node->type == kNodeGrid)
	{
		gridData = node->data;

		node->textureID = 1;
				
		node->segments.x = 16;
		node->segments.y = 16;
		node->segments.z = 1;

		node->scale.x = 1.0f;			//changes 30 unit spacing to 10
		node->scale.y = 1.0f;
		node->scale.z = 1.0f;			//original grid had z = 1.5

		node->auxA.x	= 10.0f;		//zz grid
		node->auxA.y	= 10.0f;
		node->auxA.z	= 15.0f;		//zz grid end	//original had z = 1.5
	}
	
	node->rotateVec.x = 0.0f;			//cam default is assigned to all
	node->rotateVec.y = 0.0f;
	node->rotateVec.z = 1.0f;
	node->rotateVec.angle = 0.0f;

}


// iterate through nodes update translation rotation and scale
//-----------------------------------------------------------------------------
int npExportMapFile (FILE* file, int type, void* dataRef)
{
	int size = 0;

	return size;
}


//zzs debug
//upgrade to use a pointer map similar to chmap and process in a simple loop
/*for(i=0;i<fieldCount;i++)
	{	
		switch(nodeAttribType[i])
		case kNPtypeInt : nodeAttribMap[i]=npstrtoi(&cursor); break;
		case kNPtypeFloat : nodeAttribMap[i]=npstrtof(&cursor); break;
		case kNPtypeString : nodeAttribMap[i]=npCSVstrncpy(&cursor); break;
		default :
			npPostMsg("err 9182 - unknown node data type",
						kNPmsgErr, dataRef); 
			break;
	}
*/
//build ptr map based on field names in the first line csv header
//-----------------------------------------------------------------------------
//npStrToNode
//converts a single CSV node record string and returns a pointer to the node
//void* npStrToNode (const char* buffer, char** endPtr, int size, void* dataRef)
void* npReadMapNodeCSV (const char* buffer, int wordSize, int size, 
						int format, int* scanNumRet, char** endPtr, 
						void* dataRef)
{
	char* cursor = (char*)buffer;
	int nodeCount = 0;
	int numExpected = 0;		//Expected number for scanNumRet returned from sscanf

	//Group #1
	int id, type, dataID, selected,	parentID, branchLevel, 
		childID, childIndex, childCount, nodeSize;


	//node tree pointers
	pNPnode node = NULL;
	pNPnode parent = NULL;
	pData data = (pData) dataRef;

	//replaces entire hard coded procedure
//	for (i=0; i < numOfElements; i++)
	{
		//where node[i] is this nodes current sub element
//		if(nodeStruct->type[i] == int)
//			(int)node[i] = npstrtoi(&cursor);
//		else
//			(float)node[i] = npstrtof(&cursor);
	}
	//then post process node for data tree attachment
		//add support for 			//zz debug


	//start processing buffer, these values updated during node creation
	id			= npstrtoi(&cursor);
	type		= npstrtoi(&cursor);
	dataID		= npstrtoi(&cursor);
	selected	= npstrtoi(&cursor);
	parentID	= npstrtoi(&cursor);
	branchLevel = npstrtoi(&cursor);
	childID		= npstrtoi(&cursor);
	childIndex  = npstrtoi(&cursor);	
	childCount	= npstrtoi(&cursor);

	//support for CSV ver 1
	if (format == kNPmapNodeCSVvOne && type == 8)	//workaround for 7-11 to 7-21 CSV files, debug zz
			type = kNodePin;

//zzhp debug, ONLY add the node to the map once finished reading in
	//add the node to the data map and map sort list, returns node ptr
	node = npMapNodeAdd (id, type, branchLevel, parentID, format, dataRef);
	if (node == NULL)
		return NULL;

	//update values already converted
	node->selected		= selected;
	node->childIndex	= childIndex;

	//convert the rest of string to the node values
	node->chInputID			= npstrtoi(&cursor);
	node->chOutputID		= npstrtoi(&cursor);
	node->chLastUpdated		= npstrtoi(&cursor);
	
	node->average			= npstrtoi(&cursor);
	node->interval			= npstrtoi(&cursor);
	
	node->auxA.x			= npstrtof(&cursor);		//zz grid
	node->auxA.y			= npstrtof(&cursor);
	node->auxA.z			= npstrtof(&cursor);
	node->auxB.x			= npstrtof(&cursor);
	node->auxB.y			= npstrtof(&cursor);
	node->auxB.z			= npstrtof(&cursor);		//zz grid end

	node->colorShift		= npstrtof(&cursor);

	node->rotateVec.x		= npstrtof(&cursor);
	node->rotateVec.y		= npstrtof(&cursor);
	node->rotateVec.z		= npstrtof(&cursor);
	node->rotateVec.angle	= npstrtof(&cursor);	
	
	node->scale.x			= npstrtof(&cursor);
	node->scale.y			= npstrtof(&cursor);
	node->scale.z			= npstrtof(&cursor);

	node->translate.x		= npstrtof(&cursor);
	node->translate.y		= npstrtof(&cursor);
	node->translate.z		= npstrtof(&cursor);

	node->tagOffset.x		= npstrtof(&cursor);
	node->tagOffset.y		= npstrtof(&cursor);
	node->tagOffset.z		= npstrtof(&cursor);
	
	node->rotateRate.x		= npstrtof(&cursor);
	node->rotateRate.y		= npstrtof(&cursor);
	node->rotateRate.z		= npstrtof(&cursor);

	node->rotate.x			= npstrtof(&cursor);
	node->rotate.y			= npstrtof(&cursor);
	node->rotate.z			= npstrtof(&cursor);
	
	node->scaleRate.x		= npstrtof(&cursor);
	node->scaleRate.y		= npstrtof(&cursor);
	node->scaleRate.z		= npstrtof(&cursor);

	node->translateRate.x	= npstrtof(&cursor);
	node->translateRate.y	= npstrtof(&cursor);
	node->translateRate.z	= npstrtof(&cursor);

	node->translateVec.x	= npstrtof(&cursor);
	node->translateVec.y	= npstrtof(&cursor);
	node->translateVec.z	= npstrtof(&cursor);
	
	node->shader			= npstrtoi(&cursor);
	node->geometry			= npstrtoi(&cursor);

	node->lineWidth			= npstrtof(&cursor);
	node->pointSize			= npstrtof(&cursor);
	node->ratio				= npstrtof(&cursor);

	node->colorIndex		= npstrtoi(&cursor);
	
	node->color.r			= npstrtoi(&cursor);
	node->color.g			= npstrtoi(&cursor);
	node->color.b			= npstrtoi(&cursor);
	node->color.a			= npstrtoi(&cursor);

	node->colorFade			= npstrtoi(&cursor);
	node->textureID			= npstrtoi(&cursor);

	node->hide				= npstrtoi(&cursor);
	node->freeze			= npstrtoi(&cursor);	

	node->topo				= npstrtoi(&cursor);
	node->facet				= npstrtoi(&cursor);

	node->autoZoom.x		= npstrtoi(&cursor);
	node->autoZoom.y		= npstrtoi(&cursor);
	node->autoZoom.z		= npstrtoi(&cursor);

	node->triggerHi.x		= npstrtoi(&cursor);
	node->triggerHi.y		= npstrtoi(&cursor);
	node->triggerHi.z		= npstrtoi(&cursor);

	node->triggerLo.x		= npstrtoi(&cursor);
	node->triggerLo.y		= npstrtoi(&cursor);
	node->triggerLo.z		= npstrtoi(&cursor);
	
	node->setHi.x			= npstrtof(&cursor);
	node->setHi.y			= npstrtof(&cursor);
	node->setHi.z			= npstrtof(&cursor);

	node->setLo.x			= npstrtof(&cursor);
	node->setLo.y			= npstrtof(&cursor);
	node->setLo.z			= npstrtof(&cursor);

	node->proximity.x		= npstrtof(&cursor);
	node->proximity.y		= npstrtof(&cursor);
	node->proximity.z		= npstrtof(&cursor);

	node->proximityMode.x	= (int)npstrtof(&cursor);	//int cast supports CSV ver 1
	node->proximityMode.y	= (int)npstrtof(&cursor);
	node->proximityMode.z	= (int)npstrtof(&cursor);

	node->segments.x		= (int)npstrtof(&cursor);	//grid segments were stored in node->data,
	node->segments.y		= (int)npstrtof(&cursor);	//was node->data->segments.x
	node->segments.z		= (int)npstrtof(&cursor);	//now node->segments.x

	node->tagMode			= npstrtoi(&cursor);
	node->formatID			= npstrtoi(&cursor);
	node->tableID			= npstrtoi(&cursor);
	node->recordID			= npstrtoi(&cursor);

	//node->size			handled during node creation
	nodeSize				= npstrtoi(&cursor);	//perhaps don't increment, debug zzs
	
	//return the endpoint
	*endPtr = cursor;

	if( nodeSize != 420 )						//possibly add sizes, debug zz
	{
		printf( "err 4257 - unexpected end line size: %d\n", size );

		*scanNumRet = -1;	//zzs handles end of file... will abort on one bad node
							//add ability to skip over bade nodes in wrapping function
		return node;
	}

	//support for first version CSV
	if (format == kNPmapNodeCSVvOne)
		npMapCSVvOne (node);

	//support for ver 2 CSV, compatability prior to 2012-04-22
	//we now require that pin nodes have a topo type
	if (node->topo == 0 && node->type == kNodePin)
	{
		//printf ("topo = 0   id: %d\n", node->id);
		if (node->branchLevel == 0)
			node->topo = kNPtopoPin;	//set root topo to a pin
		else if (node->parent != NULL)  //orhpan child methods in npMapSort
		{
			parent = node->parent;
			if ( parent->topo == kNPtopoPin || parent->topo == 0
				|| parent->topo == kNPtopoTorus )
				node->topo = kNPtopoTorus;
			else
				node->topo = kNPtopoPin;
		}
	}

	//workaround for keeping track of link end B, processed by orphan list
	if (node->type == kNodeLink)
		node->childIndex = childID;

	//for sorting orphan nodes
	nodeCount = ++data->map.sortCount;

	if( node->type == kNodeGrid )				//zz grid
	{
		if( node->auxA.x == 0.0f)
			node->auxA.x = kNPgridSpacing;
		if( node->auxA.y == 0.0f)
			node->auxA.y = kNPgridSpacing;
		if( node->auxA.z == 0.0f)
			node->auxA.z = kNPgridSpacing;
	}											//zz grid end

	return node;
}

//-----------------------------------------------------------------------------
void npLoadMapFile (char* buffer, int wordSize, int size, void* dataRef)
{
	char* endPtr = NULL;
	int i = 0, count = 0, curs = 0;				//Cursor position in Buffer parsing source
	int scanNumRet = 0;			//sscanf return value. Number of successfuly scanned elements
	int nodeCount = 0;
	int format = 0;
	
	pNPnode node = NULL;
	pData data = (pData) dataRef;


	// CSV version 1, starts with a '\n' so line 1 is blank, line 2 is field names
	// and data starts on 3rd line
	// CSV ver 2 starts with field names and data on 2nd line
	format = npFileFormat (buffer, wordSize, size);
	if ( format == kNPmapNodeCSVvOne )							
		count += curs = npNextLine( buffer ); buffer = &buffer[curs];	//skip blank line

	// print first two lines
	printf ("File Version %d Contents:\n", format);	
	for( i = 0; i < 80; i++ )			//first 80 chars of line one
		printf( "%c", buffer[i]);

	curs = npNextLine( buffer );	//find next line, but dont advance buffer
	for( i = 0; i < 80; i++ )			//second line
		printf( "%c", buffer[curs+i]);

	//for sorting parent and child nodes that are out of order in the file
	npMapSortInit (data->map.sortID, data);

	printf ("Processing Nodes...\n");

	//scan in all lines and create nodes
	while( scanNumRet != -1 )
	{
		//advance buffer to next line
		count += curs = npNextLine (buffer); buffer = &buffer[curs];

		// if end of buffer then exit loop
		if (count >= size)
		{
			scanNumRet = -1;
			continue;								// end of file, exit loop
		}

		//maps a single line CSV formatted to a node
		node = npReadMapNodeCSV (buffer, wordSize, size, 
								format, &scanNumRet, &endPtr, dataRef);
	//	buffer = endPtr;

		if (node != NULL)
		{
			nodeCount++;
			
			//print part of the first few lines of data
			if ( nodeCount <= 3)
			{
				printf("node_id: %d  type: %d  level: %d  color_id: %d  record_id: %d\n", 
					node->id, node->type, node->branchLevel,  node->colorIndex, node->recordID);
	
				if ( nodeCount == 3)
					printf("... ");
			}
			else if ( nodeCount == (nodeCount / 1000) * 1000 )	//print id from file every 100 nodes
				printf("%d ", nodeCount);
		}
		else
			printf("err 2440 - npLoadMapFile node is null\n");

	}	// end loop

	// print last node info
	if (node != NULL )
	{
		printf("\nnode_id: %d  type: %d  level: %d  color_id: %d  record_id: %d\n", 
				node->id, node->type, node->branchLevel,  node->colorIndex, node->recordID);
	}
	else
		npPostMsg("err 2350 - last node loaded from file is NULL", kNPmsgErr, data);

	//attach orphan child nodes to parent nodes
	npMapSort (data);

	printf ("\nNode Count: %d\n", nodeCount);

	//update the selected and active node to be root of last node created
	npSelectNode (data->map.node[data->map.nodeRootCount - 1], data);

	//if old original CSV then set the active cam to the proper child
	if ( format == kNPmapNodeCSVvOne )
	{
		node = data->map.node[kNPnodeRootCamera];
		data->map.currentCam = node->child[node->childIndex];
		npSelectNode (data->map.node[kNPnodeRootGrid], data);
	}

//	printf ("Synchronizing Text Tags\n");
//	npSyncTags (data);						//make this smarter, debug zz

	printf ("Synchronizing Channel Map\n");
	npSyncChMap (data);							//zz-JJ
}
//MB-End//



//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------



//-----------------------------------------------------------------------------
int npFileOpenMap (char* filePath, int wordSize, int size, void* dataRef)
{
	FILE* file = NULL;
	size_t count = 0;
	int i = 0;
	int byteCount = 0;
	char* bufferIndex = 0;
	char* buffer;

	pData data = (pData) dataRef;


	printf("\nOpen File: %s\n", filePath);
	file = npFileOpen (filePath, "r", dataRef);	// open file in read mode "r"

	if( file != NULL )   // check to see if file exists
	{	
		buffer = (char*) malloc(kNPmapFileBufferMax);
		if (buffer == NULL)
		{
			printf ("err 4285 - malloc failed, cannot read file");
			return 0;
		}

		printf("Reading File... ");
		count = npFileRead (buffer, 1, kNPmapFileBufferMax, file, dataRef);
		printf("Bytes Read: %d ", count);

		npFileClose (file, dataRef);	//done reading, close file
		
		if (count > 0)
		{
			printf("converting\n");
			npLoadMapFile (buffer, wordSize, count, dataRef);	//process data
			printf ("Done\n");
		}
		else
			printf("warn 5287 - zero bytes read\n");

		free (buffer);
	}
	else
		printf("warn 5288 - cannot open file\n");


	return 1;
}


//-----------------------------------------------------------------------------
int npFileSaveMap (char* filePath, int wordSize, int size, void* dataRef)
{
	int i = 0;
	int err = 0;
	int total = 0;
	int rootIndex = 0;
	size_t count = 0;
	FILE* file = NULL;

	pData data = (pData) dataRef;
	pNPnode node = NULL;

	char* buffer = (char*) malloc(kNPmapFileBufferMax);
	if (buffer == NULL)
	{
		printf ("4214 error - malloc failed, cannot write file\n");
		free (buffer);
		return 0;
	}

	//zzf
	//if 'Alt' key held then only save current selection
	if (data->io.key.modAlt)
		data->io.file.saveSelect = true;
	else
		data->io.file.saveSelect = false;
	//zzf end

	// open the file, "w+" overwrites existing files
	printf ("\nSave File: %s\n", filePath);
	file = npFileOpen (filePath, "w+", dataRef);

	if (file == NULL)
	{
		printf("err 4215 - file open failed, cannot write file\n");							//debug, add err reporting, zz
		free (buffer);
		return 0;
	}
	
	// copies current scene state to the write buffer, formats as CSV
	printf("Writing...");
	count = npMapNodesToCSV (buffer, kNPmapFileBufferMax, &rootIndex, dataRef);

	while (count > 0)
	{
		printf(".");
		total += count = npFileWrite (buffer, 1, count, file, dataRef);

		if (count >= kNPfileBlockSize)
		{
			printf("-");
			count = npMapNodesToCSV (buffer, kNPmapFileBufferMax, &rootIndex, dataRef);
		}
		else
			count = 0;
	}
	printf("\n");

	// print first line of file contents
	if (total > 0)
	{
		printf("Bytes Written: %d\n", count);
		npFileRewind(file);
		size = npFileRead (buffer, 1, 79, file, dataRef);
		printf("File Contents:\n");
		for( i = 0; i < size; i++ )
			printf("%c", buffer[i]);
		printf("\n");
	}
	else
		printf("err 4216 - file write failure, zero bytes written\n");

	err = npFileClose(file, dataRef);
	if (err)
		printf("err 4217 - file close failed, return: %d\n", i);

	printf("Done\n\n");

	// separate into CSV files and equivalent db tables, debug, zz
	// file tables include pin data, child lists, camera data

	free (buffer);

	data->io.file.saveSelect = false;			//zzf end

	return 1;
}


//-----------------------------------------------------------------------------
// if not saved then opens SaveAs dialog before reseting app
void npCloseFileMapFile (int size, void* dataRef)
{
	return;
}

/*
//-----------------------------------------------------------------------------
int npGetMapFileAttrib (int attrib, void* dataRef)
{
	switch (attrib)
	{
	case kNPversion: ; break;
	case kNPmapCount: ; break;	// number of struct equivalent data tables 
	case kNPrecordCount: ; break;	// total number of records
	case kNPnodeRootCount: ; break;
	case kNPsize: ; break;			// RAM used by map file

	default : break;
	}

	return 0;
}

//-----------------------------------------------------------------------------
void npFileOpenMapAttrib (int attrib, void* dataRef)
{
	return;
}

//------------------------------------------------------------------------------
void npFloatToCSV(char** buffer, const int* value)
{
	itoa(*value, *buffer, 0);
}

*/

//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------


//zz debug perhaps add handling for utf8 to ASCII and auto line returns for page format
//allows quotes in the CSV string denoted by a pair of double quotes
//copies CSV field string to a cstr processing quotes
//returns size of cstr in bytes or -1 if reached end size without finding a csv str
//processess strings with and without quotes, strings w/o quotes have a limited
//ASCII subset which excludes commas, white space... ie; ,rotate_x,rotate_y,...
int npCSVstrncpy(char* cstrout, char** csvstr, int size)
{
	int i = 0;
	int curs = 0;
	char* csvPtr = *csvstr;

//	int size = 0;	//number of characters in the source CSV string cstr excludes '\0' at end

//	if (*csvstr != ',') csvstr++;

	//deliminators are ',' for fields and '\"' for a string field
	//within quotes is allowed any ASCII value, including tabs, line returns, etc.
	//a quote can contained as a pair of double quotes "" which is copied to the
	//C string as a single '\"' character

	//first chars must be either:  
	//acceptc format with either	"","abc"  or  ,no_quotes_id,name,title,...
	
	//first char can be:  'a' or ',' or '\"' and not a newline

	//if no quotes then copy csv string until ',' or endline reached

	if (curs >= size)
	{
		cstrout[0] = '\0';	//set the output string to null
		return 0;		//reached end of the line without finding a CSV string
	}

	//next character should be the beginning double quote of the title field
	//zz csv string does not require quotes
	if (0)//csvPtr[curs] != '\"')		//upgrade to allow for a number or no title, debug zz
	{
		//if (csvstr[curs++] != ',')
			//skip any white space
		printf ("err 4912 - npCSVstrncpy begin quote missing: %.13s\n", csvPtr);

		//process string up to the next ',' or line return
		//we support line returns only in fields that are encapsulated in quotes
	}
	else
	{		//advance past white space
		//while (csvstr[curs++] != '\"' && curs < (size - 1){}

		//iterate through char and copy to cstr
		//if '\"' followed by ',' then exit
		//if '\"' followed by '\"' then convert to single quote
	
		if ( csvPtr[curs] == '\"')		//upgrade to allow for a number or no title, debug zz
		{
			curs += 1;//depth = 1;	//	csvstr++;	

			while ( i < size )
			{
				if ( csvPtr[curs] == '\"' && csvPtr[curs+1] == ',' )
					break;

				if ( csvPtr[curs] == '\n' )
				{
					curs++;
					cstrout[i++] = '\r';
				//	printf( "\nTEST newline TEST\n\n" );
				}
				else if ( csvPtr[curs] == '\r' )
				{
					curs++;
					cstrout[i++] = '\r';
				}
				else
					cstrout[i++] = csvPtr[curs++];
			}
		}
		else
		{
			while ( i < size )
			{
				if ( csvPtr[curs+1] == ',' )
						break;

		//		if ( csvPtr[curs] == '\n' )
		//			cstrout[i++] = '\n';
		//		else
		/*		if ( csvPtr[curs] == '\n' )
					cstrout[i++] = '\n';
				else if ( csvPtr[curs] == '\r' )
					cstrout[i++] = '\n';
				else
		*/			cstrout[i++] = csvPtr[curs++];
			}
		}
		//terminate the cstr
		cstrout[i] = '\0';

		if (curs >= size)
		{
			//printf("err 3238 - npCSVstrncpy string length exceeds max size");
			*csvstr = &csvPtr[curs];
		}
		else
		{
			if ((size - curs) > 0)
				curs++;
			if (csvPtr[curs] == ',')
				curs++;
			
			*csvstr = &csvPtr[curs];
		}

	}
//	cstrout[i] = '\0';

	//recordTag->title[count] = '\0';

					//zz debug, does not handle line returns...
//	if (*csvstr[0] == '"') *csvstr = &csvPtr[curs + 1];
//	if (*csvstr[0] == ',') *csvstr = &csvPtr[curs + 1];

	return i;
}

//tags->list[i] = npCSVtoTag (csvRowPtr, maxLineSize, kNPmapTag, data);
//buffer points to the start of a CSV formatted row
//size limits the max number of characters to scan to avoid memory violation
//type specifies the table type which corresponds to the type of C struct
//returns a ptr to a new tag struct and increments buffer ptr to line end
// pNPrecordTag npCSVtoTag (char** buffer, int size, int type, void* dataRef);
//-----------------------------------------------------------------------------
pNPrecordTag npCSVtoTag (char** read, int size, int* scanNumRet, void* dataRef)
{
	int strLength = 0;
	char* curs = *read;

	pNPrecordTag tag = NULL;
	pData data = (pData) dataRef;


	//get a new initialized tag
	tag = npNewTag(data);
	
	//add to the tag sort list, to be processed after all tags read in
	npTagSortAdd (tag, data);

	//parse first 3 integer fields
	tag->id			= npstrtoi(&curs);
	tag->recordID	= npstrtoi(&curs);
	tag->tableID	= npstrtoi(&curs);
	
	//copy title to tag as cstr with '\0', concatenated if >= kNPtagTitleMax
	strLength = size - (curs - *read);
	if (strLength >= kNPtagTitleMax)
		strLength = kNPtagTitleMax;

	tag->titleSize = npCSVstrncpy(tag->title, &curs, strLength);


	//copy desc
/*	strLength = size - (curs - *read);
	if (strLength >= kNPtagDescMax)
		strLength = kNPtagDescMax;

	tag->descSize = npCSVstrncpy(tag->desc, &curs, strLength);
*/
	//update our passed by ref params
	*scanNumRet = curs - *read;
	*read = curs;

	return tag;
}

struct NPbuffer			//circular buffer
{
	char** buffer;		//pointer to the beginning of the buffer
	int writeIndex;		//most current data entry, 
	int readIndex;		//write lock from index to the lock, holds until processed
	int size;			//end of the buffer, starts out at zero, stops at max
};
typedef struct NPbuffer NPbuffer;
typedef struct NPbuffer *pNPbuffer;


#define kNPtagFieldCount 5

int npLoadNodesCSV (const char* buffer, int size, int type, void* dataRef);
//-----------------------------------------------------------------------------
//zz debug, use a func ptr to npLoadCSV() from generic parent file IO process	
int npLoadNodesCSV (const char* buffer, int size, int type, void* dataRef)
{
//	printf("npLoadNodesCSV Test!\n");
//	npLoadMapFile ((char*)buffer, 1, size, dataRef);

	char* endPtr = NULL;
	int i = 0, count = 0, curs = 0;				//Cursor position in Buffer parsing source
	int scanNumRet = 0;			//sscanf return value. Number of successfuly scanned elements
	int nodeCount = 0;
	int format = type;
	
	pNPnode node = NULL;
	pData data = (pData) dataRef;


	// CSV version 1, starts with a '\n' so line 1 is blank, line 2 is field names
	// and data starts on 3rd line
	// CSV ver 2 starts with field names and data on 2nd line
//	format = kNPmapNodeCSV;

	//if exists, skip over leading line return
	if (*buffer == '\r' || *buffer == '\n')
	{
		count += curs = npNextLineLimit(buffer, size);
		buffer = &buffer[curs];
	}

	// print first two lines
	printf ("File Version %d Contents: ", format);	
	printf( "%.40s\n", buffer);

	//for sorting parent and child nodes that are out of order in the file
//	npMapSortInit (data->map.sortID, data);

	printf ("Processing Nodes...\n");

	//scan in all lines and create nodes
	while( scanNumRet != -1 )
	{
		//maps a single line CSV formatted to a node
		node = npReadMapNodeCSV (buffer, 1, size, 
								format, &scanNumRet, &endPtr, dataRef);

		//advance buffer to next line
		count += curs = npNextLineLimit (buffer, (size - count));
		buffer = &buffer[curs];

		//print out the first few records
		if (node != NULL)
		{
			nodeCount++;
			
			//print part of the first few lines of data
			if ( nodeCount <= 3)
			{
//				printf("count: %d   size: %d\n",count,size);				//zzhp
				printf("node_id: %d  type: %d  level: %d  color_id: %d  record_id: %d\n", 
					node->id, node->type, node->branchLevel,  node->colorIndex, node->recordID);
	
				if ( nodeCount == 3)
					printf("... ");
			}
			else if ( nodeCount == (nodeCount / 1000) * 1000 )	//print id from file every 100 nodes
				printf("%d ", nodeCount);
		}
		else
			printf("err 2440 - npLoadMapFile node is null\n");

		// if end of buffer then exit loop
		if (count >= size)
			scanNumRet = -1;
	}	// end loop

	// print last node info
	if (node != NULL)
	{
		if(nodeCount > 3)
			printf("\nnode_id: %d  type: %d  level: %d  color_id: %d  record_id: %d\n", 
					node->id, node->type, node->branchLevel,  node->colorIndex, node->recordID);
	}
	else
		npPostMsg("err 2350 - last node loaded from file is NULL", kNPmsgErr, data);

	return nodeCount;
}

//-----------------------------------------------------------------------------
//zz debug, use a func ptr to npLoadCSV() from generic parent file IO process	
int npCSVtoC (pNPrecordSet recSet, const char* read, int size, void* dataRef)
{
	pData data = (pData) dataRef;
	int recordCount = 0;

	switch(recSet->type)
	{
		case kNPmapTag :
			recordCount = npLoadTags (read, size, dataRef);
			data->map.tagCount = recordCount; /// @todo temp, lde
			break;
		case kNPmapNode :
			recordCount = npLoadNodesCSV (read, size, recSet->type, dataRef);
			break;
		default : break;
	}

	recSet->count = recordCount;

	return recordCount;
}

//-----------------------------------------------------------------------------
//zz debug, use a func ptr to npLoadCSV() from generic parent file IO process	
int npLoadTags (const char* buffer, int size, void* dataRef)
{
	int ver = 0;				//zz debug, replace with table specific func ptr
	int count = 0; 
	int curs = 0;				//Cursor position in Buffer parsing source

	int scanNumRet = 0;			//sscanf return value. Number of successfuly scanned elements
	int recordCount = 0;

	pNPrecordTag tag = NULL;
	pData data = (pData) dataRef;


	char* read = (char*)buffer;

	if (!size)
		return 0;

	if (*read == '\r' || *read == '\n')
	{
		count += curs = npNextLineLimit(read, size);
		read = &read[curs];
	}

	//scan in all lines and create nodes, exits at end of file
	//exit when we reach the end of the last row
	while( count < size )
	{		
		//processes a single Tag record, one line in the CSV file
		tag = npCSVtoTag (&read, size - count, &scanNumRet, dataRef);
		if (!tag)
			printf("err 2340 - record tag is null\n");

		//update count and set read ptr to beginning of next line
		count += scanNumRet;	
		count += curs = npNextLineLimit(read, size - count);
		read = &read[curs];

		//print part of the first few lines of data
		recordCount++;
		if ( recordCount <= 3)
		{
			printf("id: %d  record_id: %d table_id: %d tag: %.12s \n", //desc: %.8s\n",
				tag->id, tag->recordID, tag->tableID, tag->title );//tag->desc);
			//printf("countDown: %d  curs: %d  scanNumRet: %d  recordCount: %d  size: %d\n", countDown, curs, scanNumRet, recordCount, size);
			if ( recordCount == 3) printf("... ");
		}
		else if ( recordCount == (recordCount / 1000) * 1000 )	//print id from file every 100 nodes
			printf("%d ", tag->id);

	} //end loop

	// print last record if more then 11 records
	if (tag != NULL && recordCount > 3)
		printf("id: %d  record_id: %d table_id: %d tag: %.12s desc: %.8s\n",
				tag->id, tag->recordID, tag->tableID, tag->title, tag->desc);

	printf ("Tag Count: %d\n", recordCount);
	return recordCount;
}


//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------


/// Get file type based on filename extension
//------------------------------------------------------------------------------
int npGetFileTypeCat( int* fileCategory, const char* filePath, void* dataRef )
{
	int i = 0, j = 0;
	int filePathSize = 0;
	int fileCatNull = 0;			/// dummy location to point to if NULL
	char ext[kNPmaxName] = {'\0'};

	filePathSize = strnlen( filePath, kNPmaxPath );

	if( filePathSize >= kNPmaxPath )
	{
		npPostMsg( "err 2341 - kNPmaxPath exceeded", kNPmsgErr, dataRef );
		return 0;
	}

	if( fileCategory == NULL )
		fileCategory = &fileCatNull; // need to point to something

	// find the last period in the path
	i = filePathSize;
	while( filePath[i] != '.' && i >= 0 )
		i--;

	// if no file extension then return 0
	if( i <= 0 && filePath[0] != '.' )
		return 0;

	i++;	// increment past the period

	// force the file extension to be all lower case
	while( filePath[i] != '\0' )
	{
		ext[j] = filePath[i];
		if (ext[j] >= 'A' && ext[j] <= 'Z')
			ext[j] += 32;			//ASCII 'a' - 'A' = 32
		i++; j++;
	}

	/// @todo upgrade file type identification with a hash table
	// most (not all) filename extensions have 4 or less chars = could use int
	// set the fileCategory and return the extension type
	*fileCategory = kNPfileCatImage;
	if (!strcmp(ext, "dds")) return kNPfileDDS; // these types here for speed
	if (!strcmp(ext, "bmp")) return kNPfileBMP;
	if (!strcmp(ext, "gif")) return kNPfileGIF;
	if (!strcmp(ext, "jpg")) return kNPfileJPG;
	if (!strcmp(ext, "jpeg")) return kNPfileJPG;
	if (!strcmp(ext, "png")) return kNPfilePNG;

	*fileCategory = kNPfileCatTable;
	if (!strcmp(ext, "csv")) return kNPfileCSV;

	*fileCategory = kNPfileCatCode;
	if( !strcmp(ext, "h")) return kNPfileH;
	if( !strcmp(ext, "c")) return kNPfileC;
	if( !strcmp(ext, "cpp")) return kNPfileCPP;
	if( !strcmp(ext, "py")) return kNPfilePY;
	if( !strcmp(ext, "pyc")) return kNPfilePYC;
	if( !strcmp(ext, "pyd")) return kNPfilePYC;
	if( !strcmp(ext, "pyo")) return kNPfilePYC;
	
	*fileCategory = kNPfileCatText;
	if (!strcmp(ext, "txt")) return kNPfileTXT;

	*fileCategory = kNPfileCatWeb;
	if (!strcmp(ext, "xml")) return kNPfileXML;
	if (!strcmp(ext, "json")) return kNPfileJSON;
	
	*fileCategory = kNPfileCatBin;				// Executable binaries
	if (!strcmp(ext, "exe")) return kNPfileEXE;
	if (!strcmp(ext, "dll")) return kNPfileDLL;

	*fileCategory = kNPfileCatImage;			// texture map images
//	if (!strcmp(ext, "bmp")) return kNPfileBMP;
//	if (!strcmp(ext, "dds")) return kNPfileDDS;
//	if (!strcmp(ext, "gif")) return kNPfileGIF;
	if (!strcmp(ext, "jp2")) return kNPfileJ2K;
//	if (!strcmp(ext, "jpg")) return kNPfileJPG;
//	if (!strcmp(ext, "jpeg")) return kNPfileJPG;
//	if (!strcmp(ext, "png")) return kNPfilePNG;
	if (!strcmp(ext, "psd")) return kNPfilePSD;
	if (!strcmp(ext, "raw")) return kNPfileRAW;
	if (!strcmp(ext, "tga")) return kNPfileTGA;
	if (!strcmp(ext, "tif")) return kNPfileTIFF;
	if (!strcmp(ext, "tiff")) return kNPfileTIFF;
	if (!strcmp(ext, "cut")) return kNPfileCUT;
	if (!strcmp(ext, "exr")) return kNPfileEXR;
	if (!strcmp(ext, "rfg3")) return kNPfileRFG3;
	if (!strcmp(ext, "hdr")) return kNPfileHDR;
	if (!strcmp(ext, "ico")) return kNPfileICO;
	if (!strcmp(ext, "iff")) return kNPfileIFF;
	if (!strcmp(ext, "jbig")) return kNPfileJBIG;
	if (!strcmp(ext, "jng")) return kNPfileJNG;
	if (!strcmp(ext, "ms-photo")) return kNPfileJPGXR;
	if (!strcmp(ext, "koa")) return kNPfileKOA;
	if (!strcmp(ext, "ipe")) return kNPfileIPE;
	if (!strcmp(ext, "mng")) return kNPfileMNG;
	if (!strcmp(ext, "pcx")) return kNPfilePCX;
	if (!strcmp(ext, "pbm")) return kNPfilePBM;
	if (!strcmp(ext, "pgm")) return kNPfilePBM;
	if (!strcmp(ext, "ppm")) return kNPfilePBM;
	if (!strcmp(ext, "pict")) return kNPfilePICT;
	if (!strcmp(ext, "ras")) return kNPfileRAS;
	if (!strcmp(ext, "sgi")) return kNPfileSGI;
	if (!strcmp(ext, "wbmp")) return kNPfileWBMP;
	if (!strcmp(ext, "webp")) return kNPfileWEBP;
	if (!strcmp(ext, "xbm")) return kNPfileXBM;
	if (!strcmp(ext, "xpm")) return kNPfileXPM;

	*fileCategory = kNPfileCatModels;			// 3D models
	if (!strcmp(ext, "kml")) return kNPfileKML;
	if (!strcmp(ext, "dae")) return kNPfileDAE;
	if (!strcmp(ext, "obj")) return kNPfileOBJ;
	if (!strcmp(ext, "stl")) return kNPfileSTL;
	if (!strcmp(ext, "ply")) return kNPfilePLY;
	if (!strcmp(ext, "fbx")) return kNPfileFBX;
	if (!strcmp(ext, "gltf")) return kNPfileGLTF;
	if (!strcmp(ext, "glb")) return kNPfileGLTF;
	if (!strcmp(ext, "blend")) return kNPfileBLEND;
	if (!strcmp(ext, "3ds")) return kNPfile3DS;	
	if (!strcmp(ext, "ase")) return kNPfileASE;
	if (!strcmp(ext, "ifc")) return kNPfileIFC;
	if (!strcmp(ext, "xgl")) return kNPfileXGL;
	if (!strcmp(ext, "zgl")) return kNPfileXGL;
	if (!strcmp(ext, "dxf")) return kNPfileDXF;
	if (!strcmp(ext, "lwo")) return kNPfileLWO;
	if (!strcmp(ext, "lws")) return kNPfileLWS;
	if (!strcmp(ext, "lxo")) return kNPfileLXO;
	if (!strcmp(ext, "x")) return kNPfileX;
	if (!strcmp(ext, "ac")) return kNPfileAC;
	if (!strcmp(ext, "ms3d")) return kNPfileMS3D;
	if (!strcmp(ext, "cob")) return kNPfileCOB;
	if (!strcmp(ext, "bvh")) return kNPfileBVH;
	if (!strcmp(ext, "csm")) return kNPfileCSM;
//	if (!strcmp(ext, "xml")) return kNPfileOXML;
	if (!strcmp(ext, "irrmesh")) return kNPfileIRRMESH;
	if (!strcmp(ext, "irr")) return kNPfileIRR;
	if (!strcmp(ext, "mdl")) return kNPfileMDL;
	if (!strcmp(ext, "md2")) return kNPfileMD2;
	if (!strcmp(ext, "md3")) return kNPfileMD3;
	if (!strcmp(ext, "pk3")) return kNPfilePK3;
	if (!strcmp(ext, "mdc")) return kNPfileMDC;
	if (!strcmp(ext, "md5")) return kNPfileMD5;
	if (!strcmp(ext, "smd")) return kNPfileVTA;
	if (!strcmp(ext, "vta")) return kNPfileVTA;
	if (!strcmp(ext, "ogex")) return kNPfileOGEX;
	if (!strcmp(ext, "3d")) return kNPfile3D;
	if (!strcmp(ext, "b3d")) return kNPfileB3D;
	if (!strcmp(ext, "q3d")) return kNPfileQ3D;
	if (!strcmp(ext, "q3s")) return kNPfileQ3D;
	if (!strcmp(ext, "nff")) return kNPfileNFF;
//	if (!strcmp(ext, "nff")) return kNPfileS8NFF;
	if (!strcmp(ext, "off")) return kNPfileOFF;
	if (!strcmp(ext, "praw")) return kNPfilePRAW;
	if (!strcmp(ext, "ter")) return kNPfileTER;
	if (!strcmp(ext, "3mdl")) return kNPfile3MDL;
	if (!strcmp(ext, "hmp")) return kNPfileHMP;
	if (!strcmp(ext, "nod")) return kNPfileNDO;

	*fileCategory = kNPfileCatAudio;				// Audio
	if (!strcmp(ext, "aif")) return kNPfileAIFF;
	if (!strcmp(ext, "aiff")) return kNPfileAIFF;

	*fileCategory = kNPfileCatAV;					// AV (audio/video)
	if (!strcmp(ext, "mxf")) return kNPfileMXF;

	// printf( "undefined file extension: %s\n", ext );
	*fileCategory = kNPfileCatNull;

	return kNPfileNull;	///< unknown file extension type
}

/// determines file format based on buffer contents
int npGetFileFormat(const char* buffer, void* dataRef)
{
	int format = kNPformatNull;
	int fileType = 0;

	//get file type
//	fileType = npGetFileType (filePath, dataRef);

	//check header contents
	//nodes, tags, chmap, chtracks, globals...
	switch (fileType)
	{
		case kNPfileCSV : format = kNPformatCSV; break;
		default : break;
	}

	return format;
}

/*
//------------------------------------------------------------------------------
void npThreadImportCSV(const char* buffer, int size, void* dataRef)
{
	int count = 0;
	int keepOpen = true;	//keep file open
	int format = kNPformatNull;

	npPostMsg("Import CSV...", kNPmsgCtrl, dataRef);

//	format = npGetFileFormat(buffer, size, dataRef);
format = npGetFileFormat(buffer, dataRef);
		if (count > 0)
		{
			printf("Loading File\n");
			npLoadTags (buffer, size, dataRef);	//process data
			printf ("Done\n");
		}
		else
			printf("err 4987 - zero bytes read\n");


//	free (buffer);
}
*/

//-----------------------------------------------------------------------------
int npFileReadBlocks (const char* filePath, char* buffer, void* dataRef)
{
	FILE* file = NULL;

	int lock = false; //data->io.file.read.lock;

	int count = 0;		//number of bytes read
	int total = 0;
	int reading = true;

	char msg[256];

	// open file in read mode "r"
	sprintf(msg,"File Path: %s",filePath);
	npPostMsg(msg, kNPmsgCtrl, dataRef);
	file = npFileOpen(filePath, "r", dataRef);

	if( file == NULL )   // if NULL then failed to open the file 
	{
		npPostMsg("err 4988 - cannot open file", kNPmsgErr, dataRef);
		return 0;
	}

	//fill buffer with file contents, wait for thread lock to overwrite
	npPostMsg("Reading File...", kNPmsgCtrl, dataRef);
	while (reading)
	{
		count = npFileRead (buffer, 1, kNPfileBlockSize, file, dataRef);

		if (!count)
			reading = false;	//done, exit loop
		else
		{
			buffer += count;
			total += count;
			while (lock) nposSleep(0.001);	//sleep while buffer locked
		}
	}

	//if processing stops handshaking then we close the file and exit thread
	//	npPostMsg("err 7384 - file read halted", kNPmsgErr, dataRef);

	npFileClose (file, dataRef);
	npPostMsg("Done Reading, File Closed", kNPmsgCtrl, dataRef);

	return total;
}


void npFileOpenThread (void* dataRef);
const char* gThreadFilePath;
bool gThreadTagFileLock;
//-----------------------------------------------------------------------------
void npThreadFileOpenMap (void* dataRef)
{
	int result = 0;

//	char msg[256];

	pData data = (pData) dataRef;

	char filePath[4096]; // = data->io.file.currentOpenPath;

//	pNPbuffer read = malloc(sizeof(NPbuffer));

	strcpy (filePath, data->io.file.currentOpenPath);

	result = npFileOpenMap (filePath, 1, strlen(filePath), dataRef);

	if (!result)
		npPostMsg ("err 4981 - npThreadFileOpenMap failed", kNPmsgErr, data);
//	else
//		npPostMsg ("Done Loading Nodes", kNPmsgErr, data);

	if (data->map.syncTagsReady)
	{
		printf ("Sync Tags...\n");
		npSyncTags (dataRef);
		npPostMsg ("Sync Done!", kNPmsgCtrl, dataRef);
	}
	else
		data->map.syncNodes =  true;		//sync when both are ready
/*	
	strcpy (filePath, data->io.file.csvPath);
				strcat (filePath, "antztag0001.csv");
				sprintf(msg, "Loading: %s", filePath);
				npPostMsg (msg, kNPmsgCtrl, dataRef);
				gThreadFilePath = msg;
				//result = npFileOpenTag (filePath, 1, strlen(filePath), dataRef);
				npFileOpenTagThread (dataRef);
*/
	nposEndThread();
//	return;
}


//update to recognize all our file types and binary data structures			debug zz
//also recognize the apps own code and foreign code...
char* npGetType(int* type, int* format, const char* str, int size, void* dataRef);
//-----------------------------------------------------------------------------
char* npGetType(int* type, int* format, const char* str, int size, void* dataRef)
{
	int curs = 0;

	if (size < 40) 
	{
		npPostMsg("err 3459 - unknown file type", kNPmsgErr, dataRef);
		*type = 0;
		*format = 0;
		return (char*)str;
	}

	//skip white space
	str = npSkipWhiteSpace( str, size );

	//first field should be 'id' the following fields vary
	if( strncmp(str, "id,record_id,table_id,title,description", 39) == 0 
		  || strncmp(str, "np_tag_id,record_id,table_id,title,description", 46) == 0)
	{
		printf ("CSV Tag File Ver: 1\n");
		printf( "%.39s\n", str);
		*type = kNPfileCSV;
		*format = kNPmapTag;
	}
	else if (0)//strncmp(str, "id", 2) != 0)/// @todo ??? shouldn't this be == 0
	{
		//add checking for CSV ver 1
	//	curs = npNextLineLimit(str, size);
		if( strncmp(&str[curs], "id,type,data,selected,parent", 28) == 0)
		{
			printf ("CSV Node File Ver: 2\n");	//add file version, debug zz
			printf( "%.39s\n", str);
			*type = kNPfileCSV;
			*format = kNPmapNode;
		}
		else
		{
			npPostMsg("err 3460 - unknown file type, expected a CSV file", kNPmsgErr, dataRef);
			return (char*)str;
		}
	}
	else if( strncmp( str, "id,type,data,selected,parent", 28) == 0 )  //detects ver 1 or 2
	//	if (strncmp(str, "id,type,data,selected,parent_id,branch_level", 44) == 0) //ver 2
	{
		printf ("CSV node table\n");// Ver: 2\n");
		printf( "%.39s\n", str);
		*type = kNPfileCSV;
		*format = kNPmapNode;
	}
	else if (strncmp(str, "id,channel_id,track_id,attribute,track_table_id", 47) == 0)
	{
		printf ("CSV Channel Map File Ver: 1\n");
		printf( "%.39s\n", str);
		*type = kNPfileCSV;
		*format = kNPmapChannel;
	}
	else if (strncmp(str, "track_id,", 9) == 0)
	{
		printf ("CSV Tracks File Ver: 1\n");
		printf( "%.39s\n", str);
		*type = kNPfileCSV;
		*format = kNPmapTrack;
	}
	else if (strncmp(str,"np_globals_id,", 14) == 0)
	{
		printf ("CSV Globals File Ver: 1\n");
		printf( "%.39s\n", str);
		*type = kNPfileCSV;
		*format = kNPmapGlobals;
	}
	else
		return (char*)str;


	curs = npNextLineLimit(str, size);		
	return (char*)&str[curs];
}

void npSyncRecords (pNPrecordSet recordSet, void* dataRef);
//-----------------------------------------------------------------------------
void npSyncRecords (pNPrecordSet recordSet, void* dataRef)
{
	pData data = (pData) dataRef;

	npTagSort(dataRef);	//sort prior to sync

	//flag ctrl that tags are ready for sync	//zz debug, move this logic to the main ctrl loop to sync datamap
	if (data->map.syncNodes)
	{
		printf ("Sync Tags...\n");
		npSyncTags (dataRef);								//upgrade this, debug zz
		npPostMsg ("Sync Done!", kNPmsgCtrl, dataRef);

		npFree (recordSet, dataRef);
	}
	else
	{
		data->map.syncTagsReady = true;
		//store(recordSet);
	}
}


//-----------------------------------------------------------------------------
void npPreLoadInit(pNPrecordSet recSet, void* dataRef)
{
	pData data = (pData) dataRef;

//for sorting parent and child nodes that are out of order in the file	//zz debug
	if (recSet->type == kNPmapNode)
		npMapSortInit (data->map.sortID, data);
}

void npPostMap (pNPrecordSet recSet, void* dataRef);
//-----------------------------------------------------------------------------
void npPostMap (pNPrecordSet recSet, void* dataRef)
{
	pData data = (pData) dataRef;
		
	//npMapSync(data);

	//post process for node attachment and selection
	if (recSet->type == kNPmapNode)
	{
		//update the selected and active node to be root of last node created
		npSelectNode (data->map.node[data->map.nodeRootCount - 1], data);

			//if old original CSV then set the active cam to the proper child
		//if ( recordSet->type == kNPmapNodeCSVvOne )
		{
		//	node = data->map.node[kNPnodeRootCamera];
		//	data->map.currentCam = node->child[node->childIndex];
		//	npSelectNode (data->map.node[kNPnodeRootGrid], data);
		}

		printf ("Map Sort...\n");	//attach orphan nodes
		npMapSort (data);
	
		data->io.file.loading = false;											//zzhp
		npSelectNode(data->map.node[kNPnodeRootPin], data);			//zzhp

		data->map.syncNodes = true;	//flag that nodes are ready for tag sync

		printf ("Sync Channel Map...\n");
		npSyncChMap (data);
		
		if( data->map.syncTagsReady )//recSet->count)
		{
			printf ("Tags are READY before nodes...\n");
			npSyncRecords(recSet, data);//zz-JJ
		}
	}
	else if (recSet->type == kNPmapTag)
	{
		data->map.syncTagsReady = true;
		//sync the nodes and tags
		if(data->map.syncNodes == true)
			npSyncRecords( recSet, data );
		else
			data->map.syncTagsReady = true;
	}
//	if (recSet->count)
//			npSyncRecords(recSet, data);

}

//zz debug, read loop with circular buffer, separate str parsing threads
/// @todo use multiple threads to parse and a single one to read/write
// note that it is fastest to read one file per thread
//thread the process
//-----------------------------------------------------------------------------
void npFileOpenThread (void* threadData)
{
	pNPthreadFile threadFile = (pNPthreadFile) threadData;
	pData data = (pData) threadFile->dataRef;	 //get our context dataRef

	char* filePath = threadFile->filePath; //filePath with filename
	FILE* file = threadFile->file;		//file handle used to read file

	char* read = NULL;				//current read buffer block
	char* splitBlock = NULL;		//to process data split across read blcoks

	pNPrecordSet recordSet = NULL;	//list of records as native C data

	int count = 0;					//read block byte count
	int total = 0;					//total bytes read
	int records = 0;				//total number of complete records or rows

	int firstRow = 0;				//start of first complete data item or row
	int lastEOL = 0;				//endline of last complete item in the block
	int tailSize = 0;				//leftover bytes after lastEOL, splitBlock
	int cropSize = 0;				//length of complete row item in splitBuffer
	int EOLsize = 0;

	int format = 0;					//data format - CSV, JSON, XML, MySQL
	int type = 0;					//data type - globals, nodes, tags, channels

	char msg[256];

	double startTime = nposGetTime();


	//recordSet stores a list of the new records created
	//used for processing the file blocks, attaching records, orphans...
	recordSet = npMalloc(0, sizeof(NPrecordSet), data);
	if (!recordSet) goto postProcess;

	//allocate read buffer			 add support for a set of buffers //zz debug
	read = npMalloc(kNPfileBlock, kNPfileBlockSize, data);
	if (!read) goto postProcess;

	//splitPacket buffer for handling partial packets split across read blocks
	splitBlock = npMalloc(kNPfileBlock, kNPfileBlockSize, data);
	if (!splitBlock) goto postProcess;

	// if no file ptr then open file in read only mode "r" using the path
	if (!file)
	{
		sprintf(msg, "File Open: %s", filePath);
		npPostMsg(msg, kNPmsgCtrl, data);
		file = npFileOpen(filePath, "r", data);
		if (!file)
		{
			npPostMsg("err 4988 - cannot open file", kNPmsgErr, data);
			goto endPoint;
		}
	}
	//allow option to either close file between reads, or keep open	//zz debug
	//implement file block region locking for SAN and/or NUMA topology

	//printf("read first file chunk \n");
	count = npFileRead (read, 1, kNPfileBlockSize, file, data);//kNPmapFileBufferMax
	if (count <= 0) 
		goto postProcess;
	total += count;

	//identify content type (CSV, XML, MySQL...) and table format (tag, node...)
	npGetType( &format, &type, read, count, data );
	recordSet->type = type;
	
	if( !type )
	{
		npPostMsg( "err 4977 - unknown data type", kNPmsgErr, data );
		goto postProcess;
	}

	/// @todo redo globals processing to be compatible with this function
	if( type == kNPmapGlobals )
	{
		npFileClose( file, data);	//workaround
		npOpenMapCSV( filePath, kNPmapGlobals, data);

		goto endPoint;
	}

	// setup for reading node table
	npPreLoadInit( recordSet, data );

	if (type == kNPmapNode)												//zzhp
		data->io.file.loading = true;

//start csv format specific methods, agnostic to table data type
	//search from end of buffer to find end of the last complete row
	lastEOL = npLastEOL(read, count);
	tailSize = count - lastEOL;

	//zz debug, currently copying the line returns to the top of splitbuffer, fix this
	//strncpy(splitBlock, &read[lastEOL + EOLsize], tailSize - EOLsize);
	
	//copy first half of split row to splitBlock buffer
	strncpy(splitBlock, &read[lastEOL], tailSize);

	//skip over the header row, possibly remove limit ver if tail handled
	firstRow = npNextLineLimit(read, count);

	//load records for current read block, passes in start of first row
	cropSize = lastEOL - firstRow;
	if (cropSize > 0)
		records += npCSVtoC (recordSet, &read[firstRow], cropSize, data);

	//read next block, handle leftover from previous block
	while (count >= kNPfileBlockSize)
	{
		//read the next block
		count = npFileRead (read, 1, kNPfileBlockSize, file, data);
		if (count <= 0) 
			goto postProcess;
		total += count;

		//if splitBlock then find start of first complete record row
		if (tailSize > 0)
			firstRow = npNextLineLimit(read, count);
		else
			firstRow = 0;	//else set to 0

		//process data that is split across blocks
		//tailSize and lastEOL still currently have previous block read values
		if ((tailSize + firstRow) >= kNPfileBlockSize)
			printf("err 4986 - row size %d + %d >= buffer\n", tailSize, firstRow);
		else
		{
			//append 2nd half of data packet to 1st half in splitBlock buffer
			strncpy(&splitBlock[tailSize], read, firstRow);
			cropSize = kNPfileBlockSize - lastEOL + firstRow;

			//process complete packet
			records += npCSVtoC (recordSet, splitBlock, cropSize, data);
		}

		//search buffer starting from the end and find last complete row EOL
		lastEOL = npLastEOL(read, count);

		//update tailSize and copy the nex 1st half of packet splitBuffer row
		tailSize = count - lastEOL;
		if ((tailSize + firstRow) >= kNPfileBlockSize)
			printf("err 4986 - row size %d + %d >= buffer\n", tailSize, firstRow);
		else //copy first half of split row to splitBlock buffer
			strncpy(splitBlock, &read[lastEOL], tailSize);
	
		//loads the tags that are fully contained within the current read block
		cropSize = lastEOL - firstRow;
		if (cropSize > 0)
			records += npCSVtoC (recordSet, &read[firstRow], cropSize, data);

		//insert threaded read block parsing here							//zz debug	
		//while (parseIndex == blockIndex) nposSleep(0.001);
	}

postProcess:
	npFileClose (file, data);	//done reading, close file

	printf("\nTotal Bytes: %d\n", total);
	switch (type)
	{
		case kNPmapNode : printf("Total Nodes: %d\n", records); break;		
		case kNPmapTag : printf("Total Tags: %d\n", records); break;
		default : printf("Total Rows: %d\n", records); break;
	}
	sprintf(msg, "load time: %f secs", (nposGetTime() - startTime) );
	npPostMsg(msg,kNPmsgCtrl,data);

	//sort n sync nodes, tags, channels... and attach nodes to data map tree
	startTime = nposGetTime();		//set start time to measure sync time
	npPostMap (recordSet, data);

	sprintf(msg, "sync time: %f secs", (nposGetTime() - startTime) );
	npPostMsg(msg,kNPmsgCtrl,data);

	//zz debug workaround for tag sync causing tool 'id: 19' bug
	npMenuRefresh( NULL, data );

endPoint:
	//flag other processess that loading is done, resume z-sort draw
	if (type == kNPmapNode)												//zzhp
		data->io.file.loading = false;

	//free read buffers and threadFile structure
	npFree (splitBlock, data);
	npFree (read, data);
	npFree (threadFile->filePath, data);	//free the filePath str copy
	npFree (threadFile, data);				//free the thread container

	//move this out to a thread worker wrapper								//zz debug
	nposEndThread();
}


/*
	struct {
		void* dataRef;
		char** buffer;
		int writeIndex;
		int readIndex;			//current point read up to
		int blockSize;			//file IO is 4KB to 16MB
		int	threadID;			//threads divide up the block based on count
		int workerID;			//id and count determine buffer slot
		int workerCount;		//number of threads to divide up task 
	}
*/

//------------------------------------------------------------------------------
int npTableAlreadyLoaded(const char* filePath, void* dataRef)
{
	pData data = (pData) dataRef;

	//update the table_map for load, merge and/or update
	if( 0 )//npAlreadyLoaded( filePath, data ) )
		return 1;
	else
		return 0;
}

//------------------------------------------------------------------------------
int npGetTableID(const char* filePath, void* dataRef)
{
	int tableID = 0;
	pData data = (pData) dataRef;

	if( npTableAlreadyLoaded( filePath, data ) )
	{
		return 1;
	}
	else
	{
		//generate a table id or return 0?	//zz debug
	}

	return tableID;
}


//------------------------------------------------------------------------------
int npTableMapUpdate (const char* filePath, FILE* file, void* dataRef)
{
	int tableID = 1;
	pData data = (pData) dataRef;

	//update the table_map for load, merge and/or update
	tableID = npGetTableID( filePath, data );
	if( !tableID )
	{
	//	tableID = npTableNew( filePath, file, data );
		printf( "table_map insert: %s\n", filePath );
	}
	else
		printf( "table_map update: %s\n", filePath );

	return tableID;
}


/// @todo add methods for 3rd part tables
//------------------------------------------------------------------------------
int npFileOpenAuto (const char* filePath, FILE* file, void* dataRef)
{
	int fileType = 0, fileCat = 0;
	int id = 0;

	pData data = (pData) dataRef;
	pNPmodels models;

	//pData data = (pData) dataRef;
	pNPthreadFile threadFile = NULL;

	threadFile = npMalloc (0, sizeof(NPthreadFile), dataRef);
	if( !threadFile ) return 1;

	threadFile->filePath = npNewStrcpy(filePath, dataRef);
	threadFile->file = file;
	threadFile->dataRef = dataRef;

	fileType = npGetFileTypeCat( &fileCat, filePath, data );

	//update the table_map for load, merge and/or update
	npTableMapUpdate( filePath, file, data );

	switch( fileCat )
	{
		case kNPfileCatImage :
			if( id = npLoadTexture( filePath, fileType, data) )
				npSetSelectedNodes( kNPtextureID, &id, data );
			break;
		case kNPfileCatTable :
			nposBeginThread( npFileOpenThread, threadFile );
			break;
		case kNPfileCatModels :
			if( models = npLoadModels( filePath, data ) )
			{
				npSetSelectedNodes( kNPgeometry, &models->geometryID, data );
				npSetSelectedNodes( kNPtextureID, &models->textureID, data );
			}
			break;
		default :
			npPostMsg("err 4989 - File type not supported", kNPmsgErr, data);
			break;
	}
	return 0;
}


//-----------------------------------------------------------------------------
int npFileSaveTag (const char* filePath, int wordSize, int size, void* dataRef)
{
	printf ("err 4339 - failed to save CSV Tag file\n");
	return 0;
}

//recursively traverses tree for all child branches
//------------------------------------------------------------------------------
int npMapTraverseTree (char* buffer, pNPnode node, int format, void* dataRef)
{
	int i = 0;
	int count = 0;
	pNPnode nodeChild = NULL;

	//zzf
	pData data = (pData) dataRef;
	if ( data->io.file.saveSelect && !node->selected )
		return count;

	// if not root then write the data, root has already been written
	if (node->branchLevel > 0)
		count += npWriteNode (buffer, node, format, dataRef);

	//recursively calls this function for all children
	for (i=0; i < node->childCount; i++)//node->childCount; j++)
	{
		nodeChild = node->child[i];
		//zzf
		if ( data->io.file.saveSelect && !nodeChild->selected )
			continue;
	//	else
		{
			if (node->child[i] != NULL				//zzsql debug, this is bad logic method
				&& node->child[i]->child[0] != node)	//don't write twice, this is link B so skip
				count += npMapTraverseTree (buffer+count, node->child[i], format, data);	
		}
	}

	return count;
}
//zzsql node write traversal
int npMapTraverseTreeLL(char* buffer, int* index, pNPnode node, int format, void* dataRef)
{
	pData data = (pData) dataRef;
	int i = 0;
	int count = 0;
	
	//	printf("\nIndex is %d :: node->branchLevel : %d :: node->childCount : %d", *index, node->branchLevel, node->childCount);
	if (node->branchLevel > 0)
	{
		count = npWriteNode(buffer, node, format, dataRef);
		buffer[count] = '\0';
		//	printf("\nIndex : %d", *index);
		//	printf("\nchild : %s", buffer[*index]);
		//getch();
		(*index)++;
		data->io.write = buffer;			//zzsql does not seem to make sense...
	}
	
	//recursively calls this function for all children
	for (i=0; i < node->childCount; i++)//node->childCount; j++)
	{
		if (node->child[i] != NULL					//zzsql debug, this is bad logic method
			&& node->child[i]->child[0] != node)	//don't write twice, this is link B so skip
		{
			count += npMapTraverseTreeLL(buffer, index, node->child[i], format, dataRef);
		}
	}
	//	printf("npMapTraverseTreeLL totalCount :: %d", count);
	
	
	//return count;
	return count;
}
//zzsql

//zzsql
//-----------------------------------------------------------------------------
// keep track of a bufferIndex for each list
// write list of children
// write node or node list
// write node specific data to a separate list, ie pin, camera, etc
// write list of channels,						channels NOT YET IMPLEMENTED, debug, zz
// returns a pointer to the last char written to buffer
//-----------------------------------------------------------------------------
//int npWriteNode (const char* buffer, pNPnode node, int format, char** endPtr, void* dataRef)
// debug db
int npWriteNode (const char* buffer, pNPnode node, int format, void* dataRef)
{
	int n			= 0;
	int parentID	= 0;
	int childID		= 0;

	pNPnode parent = NULL;

	pData data = (pData) dataRef;

	// pointers to the node data, update to be a generic list, debug, zz
	// break into separate buffers, debug zz
	char* nodePtr = (char*) buffer;

/*
	switch (format)
	{	
		case kNPnodeCSV :	npNodeToCSV(buffer, nodeRef, format, dataRef); break;
		case kNPnodeOSC :	npNodeToOSC (buffer, nodeRef, format, dataRef); break;
		case kNPnodeMySQL : npNodeToSQL (buffer, nodeRef, format, dataRef); break;
		case kNPnodeXML :	npNodeToKML (buffer, nodeRef, format, dataRef); break;
		case kNPnodeJSON :	npNodeToJSON (buffer, nodeRef, format, dataRef); break;
		case kNPnodeKML :	npNodeToKML (buffer, nodeRef, format, dataRef); break;
		case default : break;
	}
*/
	//if parent exists then set the parentID, otherwise defaults to 0
	if (node->parent != NULL)
		parentID = node->parent->id;

	//if kNodeLink then childID is set to the link-B node id, default is 0
	if (node->type == kNodeLink)
		childID = node->child[0]->id;	//link-B node ptr stored at child[0]

	//format as CSV and fill the buffer, broken into groups for readability
	n += sprintf ((nodePtr + n), "%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%f,%f,%f,%f,%f,%f",		//zz grid
		node->id,					//id is unique to this node
		node->type,		
		node->id,					// node->data->id uses this node id
		
		node->selected,
		parentID,					// parent id replaces pointer to parent
		node->branchLevel,													//zz debug level
		childID,					// either same as the node or id of link end B
		node->childIndex,
		node->childCount,
		
		node->chInputID,
		node->chOutputID,	
		node->chLastUpdated,

		node->average,
		node->interval,

		node->auxA.x,		//zz grid
		node->auxA.y,
		node->auxA.z,

		node->auxB.x,
		node->auxB.y,
		node->auxB.z		//zz grid end
	);

	//  next group
	n += sprintf ((nodePtr + n), ",%f,%f,%f,%f,%f,%f,%f,%f,%f,%f,%f,%f,%f,%f,%f,%f,%f,%f,%f,%f",
		node->colorShift,

		node->rotateVec.angle, 
		node->rotateVec.x,
		node->rotateVec.y,
		node->rotateVec.z,
		
		node->scale.x,
		node->scale.y,
		node->scale.z,
		
		node->translate.x,
		node->translate.y,
		node->translate.z,

		node->tagOffset.x,
		node->tagOffset.y,
		node->tagOffset.z,
		
		node->rotateRate.x,
		node->rotateRate.y,
		node->rotateRate.z,

		node->rotate.x,
		node->rotate.y,
		node->rotate.z
	);

	n += sprintf ((nodePtr + n), ",%f,%f,%f,%f,%f,%f,%f,%f,%f", 
		node->scaleRate.x,
		node->scaleRate.y,
		node->scaleRate.z,

		node->translateRate.x,
		node->translateRate.y,
		node->translateRate.z,

		node->translateVec.x,
		node->translateVec.y,
		node->translateVec.z
	);

	n += sprintf ((nodePtr + n), ",%d,%d,%f,%f,%f,%d,%d,%d,%d,%d,%d,%d",
		node->shader,
		node->geometry,

		node->lineWidth,
		node->pointSize,
		node->ratio,

		node->colorIndex,
		
		node->color.r,
		node->color.g,
		node->color.b,
		node->color.a,

		node->colorFade,
		node->textureID
	);

	n += sprintf ((nodePtr + n), ",%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d",
		node->hide,
		node->freeze,
		node->topo,
		node->facet,

		node->autoZoom.x,
		node->autoZoom.y,
		node->autoZoom.z,
		
		node->triggerHi.x,
		node->triggerHi.y,
		node->triggerHi.z,

		node->triggerLo.x,
		node->triggerLo.y,
		node->triggerLo.z 
	);

	n += sprintf ((nodePtr + n), ",%f,%f,%f,%f,%f,%f,%f,%f,%f,%d,%d,%d",
		node->setHi.x,
		node->setHi.y,
		node->setHi.z,

		node->setLo.x,
		node->setLo.y,
		node->setLo.z,

		node->proximity.x,
		node->proximity.y,
		node->proximity.z,

		node->proximityMode.x,
		node->proximityMode.y,
		node->proximityMode.z
	);

	//last group
	n += sprintf ((nodePtr + n), ",%d,%d,%d,%d,%d,%d,%d,%d",
		node->segments.x,
		node->segments.y,
		node->segments.z,

		node->tagMode,

		node->formatID,
		node->tableID,
		node->recordID,
		
		node->size 
	);

/*
//------------------------------------------------------------------------------
void npitoa(char** buffer, const int* value)
{
	itoa(*value, *buffer, 0);
}

//------------------------------------------------------------------------------
void npftoa(char** buffer, const int* value)
{
	ftoa(*value, *buffer, 0);
}

//------------------------------------------------------------------------------
void npIntToCSV(char** buffer, const int* value)
{
	itoa(*value, *buffer, 0);
	buffer++;
	buffer* = ',';
	buffer++;
}

//------------------------------------------------------------------------------
void npFloatToCSV(char** buffer, const int* value)
{
	itoa(*value, *buffer, 0);
}

	npIntToCSV(&buffer, &node->id);
	//...
	npFloatToCSV(&buffer, &node->proximity.z);

	npIntToCSV(&buffer, &node->proximityMode.x);
	npIntToCSV(&buffer, &node->proximityMode.y);
	npIntToCSV(&buffer, &node->proximityMode.z);
	//...
	npIntToCSV&buffer, &node->recordID);

	npitoa(&buffer, &node->size);		//no comma for last field

//	buffer++;

	//after last group, end the node row using a newline
	*buffer = '\n';
*/


	//after last group, end the node row using a newline
	n += sprintf ((nodePtr + n), "\n");						//zz debug pull this out to higher level... SQL compatible

	//increment the buffer
	data->io.write += n;

	return n;
}


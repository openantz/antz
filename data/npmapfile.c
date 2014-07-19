/* -----------------------------------------------------------------------------
*
*  npmapfile.c
*
*  ANTz - realtime 3D data visualization tools for the real-world, based on NPE.
*
*  ANTz is hosted at http://openantz.com and NPE at http://neuralphysics.org
*
*  Written in 2010-2012 by Shane Saxon - contact@saxondigital.net
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

#include "npmapfile.h"


#include "../npdata.h"
#include "../data/npnode.h"
#include "../os/npos.h"
#include "../io/npfile.h"
#include "../io/npch.h"

#include "../io/gl/nptags.h"	//move the necessary functions to npdata, debug zz

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

//recursively traverses tree for all child branches
//------------------------------------------------------------------------------
int npMapTraverseTree (char* buffer, NPnodePtr node, int format, void* dataRef)
{
	int i = 0;
	int count = 0;

	// if not root then write the data, root has already been written
	if (node->branchLevel > 0)
			count += npWriteNode (buffer, node, format, dataRef);

	//recursively calls this function for all children
	for (i=0; i < node->childCount; i++)//node->childCount; j++)
	{
		if (node->child[i] != NULL)
			count += npMapTraverseTree (buffer+count, node->child[i], format, dataRef);	
	}

	return count;
}

//-----------------------------------------------------------------------------
// write the header structure
int npWriteMapHeader (char* bufferIndex, int count, int type, void* dataRef)
{
	int i = 0;
	int itemCount = 0;

	NPmapTypePtr map = NULL;

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
int npGetMapFile (char* buffer, int wordSize, int size, void* dataRef)
{
	int i = 0;
	int type = 0;
	int count = 0;

	NPnodePtr node = NULL;
	pData data = (pData) dataRef;

/*	data->io.write[kNPcamera] = bufferA [kNPmapFileBufferMax]);
	data->io.write[kNPgrid] = bufferB [kNPmapFileBufferMax]);
	data->io.write[kNPpin] = bufferC [kNPmapFileBufferMax];
	data->io.write[kNPchild] = bufferD [kNPmapFileBufferMax];
	data->io.write[kNPchannel] = bufferE [kNPmapFileBufferMax];
*/
/*
	count += npWriteMapHeader (data->io.write[kNPcamera], size, kNPcamera, dataRef);
	data->io.write[kNPcamera] += count;

	count += npWriteMapHeader (data->io.write[kNPgrid], size, kNPgrid, dataRef);
	data->io.write[kNPgrid] += count;

	count += npWriteMapHeader (data->io.write[kNPpin], size, kNPpin, dataRef);
	data->io.write[kNPpin] += count;

	count += npWriteMapHeader (data->io.write[kNPchild], size, kNPchild, dataRef);
	data->io.write[kNPchild] += count;

	count += npWriteMapHeader (data->io.write[kNPchannel], size, kNPchannel, dataRef);
	data->io.write[kNPchannel] += count;
*/

	data->io.write[kNPnode] = buffer;
												//kNPmapNodeCSV, debug zz
	count += npWriteMapHeader ((buffer + count), size, kNPnode, dataRef);
	data->io.write[kNPnode] += count;

	// iterate through data map and write out each node
	for (i=0; i < data->map.nodeRootCount; i++)
	{
		node = data->map.node[i];

		count += npWriteNode ((buffer + count), node, kNPmapNodeCSV, dataRef);

		if (node->childCount)
			count += npMapTraverseTree ((buffer + count), node, kNPmapNodeCSV, dataRef);
	}

	return count;
}

// iterate through nodes update translation rotation and scale
//-----------------------------------------------------------------------------
int npExportMapFile (FILE* file, int type, void* dataRef)
{
	int size = 0;

	return size;
}

//-----------------------------------------------------------------------------
void npMapSortInit (void** mapRef, void* dataRef)
{
	int i = 0;

	pData data = (pData) dataRef;
/*
	if (data->map.sortID != NULL)
		free (data->map.sortID);

	data->map.sortID = (void*)malloc(kNPnodeMax * sizeof(void*));

	if (data->map.sortID == NULL)
	{
		printf ("err 4833 - malloc failed to allocate sortID array\n");
		return;
	}
*/	
	//fill with node pointers to kNPnodeRootNull
	for (i=0; i < kNPnodeMax; i++)
	{
		data->map.sortID[i] = NULL;
		data->map.parentID[i] = 0;
		data->map.orphanList[i] = 0;
	}

	data->map.orphanCount = 0;
	data->map.sortCount = 0;
}

//if id mapped then returns the node, else it returns the kNPnodeRootNull node
//-----------------------------------------------------------------------------
void npMapSortAdd (int id, int parentID, void* nodeRef, void* dataRef)
{
	int count = 0;

	NPnodePtr node = (NPnodePtr) nodeRef;
	pData data = (pData) dataRef;

	if (id < 0 || id >= kNPnodeMax)
	{
		printf ("err 4833 - id: %d out of range, npMapSortAdd()\n", id);
		return;
	}
	
	// check if id already used, if so replaces entry in orphanList
	// perhaps change from overwrite and assign a new id update actual node, debug zz
	if (data->map.sortID[id] != NULL)
		printf ("warning 4834 - id: %d previously used in table, overwriting\n", id);

	// add the node to the sort parent lookup
	data->map.sortID[id] = nodeRef;

	// if orphan then add to list
	if (node->parent == data->map.node[kNPnodeRootNull] )
	{	
		// make sure in bounds of list max
		count = data->map.orphanCount;

		if ( count >= kNPnodeMax - 1 )
		{
			printf ("err 4835 - kNPorphanMax node limit exceeded\n");
			return;
		}

		data->map.orphanList[count] = id;
		data->map.parentID[count] = parentID;
		
		data->map.orphanCount++;

		printf ("orphanID: %d   parentID: %d\n", id, parentID);
	}
}

//-----------------------------------------------------------------------------
NPnodePtr npMapSortID (int id, void* dataRef)
{
	pData data = (pData) dataRef;

	if (id < 0 || id >= kNPnodeMax)
	{	
		printf ("err 4834 - id: %d out of range, npMapSortAdd()\n", id);
		return data->map.node[kNPnodeRootNull]; //NULL debug zz
	}

	if (data->map.sortID[id] == NULL)
		return data->map.node[kNPnodeRootNull];	// do we need this, debug zz

	//make a special node with no limit on child nodes,						 debug, zz

	return data->map.sortID[id];
}

//update node branchLevel based on parent level, traverses all sub-child nodes
//-----------------------------------------------------------------------------
void npNodeUpdateBranchLevel (NPnodePtr node)
{
	int i = 0;

	NPnodePtr parent = node->parent;
	
	if (parent == NULL)
		node->branchLevel = 0;
	else
		node->branchLevel = parent->branchLevel + 1;

	//recursively traverse all child nodes and update them
	for (i=0; i < node->childCount; i++)
	{
		npNodeUpdateBranchLevel (node->child[i]);	// recursive call
	}

//	printf ("id: %d  updated branchLevel: %d\n", node->id, node->branchLevel);
}


//-----------------------------------------------------------------------------
void npNodeMoveBranch (NPnodePtr node, NPnodePtr newParent, void* dataRef)
{
	int i = 0;
	int childIndex = 0;
	int parentIndex = 0;

	pData data = (pData) dataRef;

	if (node == NULL || newParent == NULL)
	{
		printf ("err 4835 - node or nodeParent is NULL\n");
		return;
	}
		
	if (newParent->childCount >= kNPnodeChildMax)
	{
		printf ("err 4836 - kNPnodeChildMax, cannot add to parent\n");
		return;
	}

	//remove node from parent old parent
	npNodeRemove(0, node, data);

	//attach to new parent
	node->parent = newParent;
	
	newParent->child[node->childCount] = node;
	newParent->childCount++;

	//updates branchLevel for child and any sub child nodes in the tree
	npNodeUpdateBranchLevel (node);

//	printf ("node id: %d  branchLevel: %d  parent id: %d\n", 
//			node->id, node->branchLevel, newParent->id );
}

//-----------------------------------------------------------------------------
void npMapSort(void* dataRef)
{
	int i = 0;
	int parentIndex = 0;

	NPnodePtr node = NULL;
	NPnodePtr nodeParent = NULL;
	pData data = (pData) dataRef;

	//iterate through the orphanList
	for (i=0; i < data->map.orphanCount; i++)
	{
		node = data->map.sortID[data->map.orphanList[i]];
		nodeParent = data->map.sortID[data->map.parentID[i]];

		if (nodeParent != data->map.node[kNPnodeRootNull])
		{
			npNodeMoveBranch (node, nodeParent, data);

			printf ("orphan id: %d attached to parent id: %d\n", 
				data->map.orphanList[i], data->map.parentID[i] );
		}
		else
		{
			npNodeDelete(node, dataRef);
			data->map.sortID[i] = NULL;

			printf ("err 4838 - orphan id: %d   missing parent id: %d\n",
				data->map.orphanList[i], data->map.parentID[i] );
		}
	}
}

//MB//
// seeks to next line, accepts cr and or nl to be compatible with Mac, Unix, MS
//-----------------------------------------------------------------------------
int npSeekToNextLine( char* buffer )	//Todo: Check Max
{
	char ch = 0;
	int curs = 0;

	//either Carraige Return or New Line will end loop
	while( ch != '\r' && ch != '\n' )
	{
		ch = buffer[curs];
		curs++;
	}

	// handles carriage return followed by line feed by incrementing forward
	if ( ch == '\r' && buffer[curs] == '\n' )
		curs++;

	return curs;
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


//logic for building the data map from an unsorted list of nodes
//creates a new node or updates some nodes like exisitng cameras and grids
//need to call npMapSort() after all nodes added to attach orphan nodes
//-----------------------------------------------------------------------------
void* npMapNodeAdd (int id, int type, int branchLevel, int parentID, 
					 int format, void* dataRef)
{
	int i = 0, count = 0;
	int curs = 0;				//Cursor position in Buffer parsing source
//	int scanNumRet = 0;			//sscanf return value. Number of successfuly scanned elements
	int numExpected = 0;		//Expected number for scanNumRet returned from sscanf
	int nodeCount = 0;
	int parentBranchLevel = 0;
	int childIndex = 0;

	//node tree pointers
	NPnodePtr nodeParent = NULL;
	NPnodePtr node = NULL;
	NPnodePtr nodeChild = NULL;
	
	pData data = (pData) dataRef;

	
	//process node logic based on type and branchLevel
	//either updates existing node or create a new one to add to scene
	//update based on user setting or node type and ID
	//if new one, either a child or parent, determined by branchLevel
	//default is to update->camera, update->grid
	//new node for all others

	//root node
	if( branchLevel <= 0 ) // or perhaps parentID == 0, debug zz
	{
		switch (type)
		{
			case kNodeCamera : node = data->map.node[kNPnodeRootCamera]; break; //CSV v1 requires this
			case kNodeGrid : node = data->map.node[kNPnodeRootGrid]; break;	//CSV v1 requires this
			case kNodePin :
				node = npNodeNew (type, NULL, dataRef);
				if (format == kNPmapNodeCSVvOne)
					nodeChild = npNodeNew (type, node, dataRef);	//create primary torus
				break;			//create Pin or other
			case kNodeDefault : 
				if (id == 1 && type == kNodeDefault)
					node = data->map.node[kNPnodeRootNull];
				else
					node = npNodeNew (type, NULL, dataRef);
				break;
			default : printf ("err 4840 - id: %d type: %d unknown\n", id, type); break;
		}
	}

	//first child branchLevel
	if( branchLevel == 1 )
	{
		// if parent is NULL then returns the nodeRootNull
		nodeParent = npMapSortID(parentID, dataRef);

		switch (type)
		{
			case kNodeCamera :
				if (1)//format == kNPmapNodeCSVvOne)
				{
					nodeParent = data->map.node[kNPnodeRootCamera];

					if (id >= 3 && id <= 6)
					{
						node = nodeParent->child[id - 3];
						if (node == NULL)
							node = npNodeNew (type, nodeParent, dataRef);	//else create new node
					}
					else
						node = npNodeNew (type, nodeParent, dataRef);	//else create new node
				}
				break;
			case kNodeGrid :
				if (format == kNPmapNodeCSVvOne)
					nodeParent = data->map.node[kNPnodeRootGrid];
				node = npNodeNew (type, nodeParent, dataRef);	//create new node
				break;
			case kNodePin :
				if (format == kNPmapNodeCSVvOne && nodeParent->child[0] != NULL)
					node = nodeParent->child[0];	//already created primary torus
				else
					node = npNodeNew (type, nodeParent, dataRef);	//create the primary toroid
				break;			//create Pin or other
			default : node = npNodeNew (type, nodeParent, dataRef); break;
		}
	}

	//New Sub Child Toroid
	if( branchLevel >= 2 )		//Second Level (or greater) Toroid Sub Child  
	{
		nodeParent = npMapSortID (parentID, dataRef);	//if DNE return default node
		node = npNodeNew (type, nodeParent, dataRef);
	}

	// add to parent lookup table for sorting orphan child nodes at end
	npMapSortAdd (id, parentID, node, dataRef);

	return node;
}

//-----------------------------------------------------------------------------
void npMapCSVvOne(NPnodePtr node)
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
			node->translate.z = node->translate.x;

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

		gridData->spacing.x	= 10.0f;
		gridData->spacing.y	= 10.0f;
		gridData->spacing.z	= 15.0f;	//original grid had z = 1.5
	}
	
	node->rotateVec.x = 0.0f;			//cam default is assigned to all
	node->rotateVec.y = 0.0f;
	node->rotateVec.z = 1.0f;
	node->rotateVec.angle = 0.0f;

}

//currently is just an unsorted list, later will use a HASH method,		   debug zz
//-----------------------------------------------------------------------------
void npAddRecordTag (pNPrecordTag recordTag, void* dataRef)
{
	int i = 0;
	pNPrecordTag searchTag = NULL;
	pData data = (pData) dataRef;

	//if identical recordID && tableID then replace existing
	for (i=0; i < kNPrecordTagListMax; i++)
	{
		searchTag = data->io.gl.hud.tags.recordList[i];

		//reached end of list add record
		if (searchTag == NULL)
		{	
			if (data->io.gl.hud.tags.recordCount >= kNPrecordTagListMax - 1)
			{
				printf ("error 4834 - kNPrecordTagListMax exceeded\n");
				return;
			}
			//add the tag to the end of the list
			data->io.gl.hud.tags.recordList[i] = recordTag;
			data->io.gl.hud.tags.recordCount++;

			printf("record_id: %d  tag imported\n", recordTag->recordID);
			i = kNPrecordTagListMax; //exit loop

		}		//if identical record then update it
		else if ( searchTag->recordID == recordTag->recordID
				&& searchTag->tableID == recordTag->tableID )
		{
			data->io.gl.hud.tags.recordList[i] = recordTag;

			free(searchTag);	//debug make sure this okay, debug zz

			printf("record_id: %d  tag updated\n", recordTag->recordID);
			i = kNPrecordTagListMax; //exit loop
		}
	}
}

//-----------------------------------------------------------------------------
int npSeekToNextField( char* buffer )	//Todo: Check Max
{
	char ch = 0;
	int curs = 0;

	//either Carraige Return or New Line will end loop
	while( ch != ',')
	{
		ch = buffer[curs];
		curs++;
	}

	return curs;
}

//-----------------------------------------------------------------------------
pNPrecordTag npReadTagFromCSV (char* buffer, int wordSize, int size, 
						int format, int* scanNumRet, void* dataRef)
{
	int curs = 0;
	int count = 0;
	int numExpected = 3;		//Expected number for scanNumRet returned from sscanf
//	int scanNumRet = 0;			//sscanf return value. Number of successfuly scanned elements
	
	int id = 0, record_id = 0, table_id = 0;

	pData data = (pData) dataRef;
	pNPrecordTag recordTag = NULL;

	//Scan in the first 3 field columns which should contain only integers
	*scanNumRet = sscanf( buffer, "%d,%d,%d", &id, &record_id, &table_id);

	//if -1 then exit loop, otherwise err check item count and lineSize
	if( *scanNumRet != numExpected )
	{
		if( *scanNumRet == -1 )
			return NULL;							// end of file, exit loop

		if (*scanNumRet != 0)
			printf( "err 4956 - scanNumRet: %d  id: %d\n", scanNumRet, id);	
		
		return NULL;								// skip this line
	}

	//now separate out the title, then desc
	//first skip over the first 3 columns
	//advance buffer to next line
	count += curs = npSeekToNextField (buffer); buffer = &buffer[curs];
	count += curs = npSeekToNextField (buffer); buffer = &buffer[curs];
	count += curs = npSeekToNextField (buffer); buffer = &buffer[curs];
//	printf ("count: %d\n",count);
	count = 0;

	//next character should be the beginning double quote of the title field
	if (buffer[0] != '\"')		//upgrade to allow for a number or no title, debug zz
	{
		printf ("err 4912 - double quote missing for title string\n");
		return NULL;
	}
	
	buffer++;

	//also add handling for utf8 to ASCII and auto line returns for long sentences
	//upgrade to allow quotes in the CSV string denoted by two double quotes
	//will need to convert to "" to '\"'								//debug zz
	while (buffer[count] != '\"' || buffer[count+1] == '\"' )
	{
		if (buffer[count] == '\"' && buffer[count+1] == '\"')
			count++;

		count++;
	}

	recordTag = (pNPrecordTag) malloc(sizeof(NPrecordTag));
	if (recordTag == NULL)
	{
		printf ("err 4279 - malloc failed making an NPrecordTag\n");
		return NULL;
	}
	npInitDataRecordTag (recordTag, data);

	//copy the id fields
	recordTag->id = id;
	recordTag->recordID = record_id;
	recordTag->tableID = table_id;

	if (count > kNPtitleSizeMax)
	{
		printf("err 4959 - record title exceeds kNPtitleSizeMax\n");
		//need to copy then advance buffer
		strncpy(recordTag->title, buffer, kNPtitleSizeMax);
		recordTag->title[kNPtitleSizeMax] = '\0';
		buffer = &buffer[count];
		count = kNPtitleSizeMax;
	}
	else
	{//copy the string and add a null character
		strncpy(recordTag->title, buffer, count);
		recordTag->title[count] = '\0';
		buffer = &buffer[count];
	}
	recordTag->titleSize = count;
	printf ("titleSize: %d  ", count);

	buffer++;
	//repeat process for desc
	//next character should be the beginning double quote of the title field
	if (buffer[0] != ',' && buffer[1] != '\"')		//upgrade to allow for a number or no title, debug zz
	{
		printf ("\n");	
		printf ("err 4912 - desc string missing double quote\n");

		//permits recordTag with title but no desc
		npAddRecordTag (recordTag, dataRef);
		return recordTag;
	}
	
	buffer++;
	buffer++;

	count = 0;
	//will need to convert to "" to '\"'								//debug zz
	while (buffer[count] != '\"' || buffer[count+1] == '\"' )
	{
		if (buffer[count] == '\"' && buffer[count+1] == '\"')
			count++;

		count++;
	}

	if (count > kNPdescSizeMax)
	{
		printf("err 4960 - record description exceeds kNPdescSizeMax\n");
		//need to copy then advance buffer
		strncpy(recordTag->desc, buffer, kNPdescSizeMax);
		recordTag->desc[kNPdescSizeMax] = '\0';
		buffer = &buffer[count];
		count = kNPdescSizeMax;
	}
	else
	{//copy the string and add a null character
		strncpy(recordTag->desc, buffer, count);
		recordTag->desc[count] = '\0';
		buffer = &buffer[count];
	}
	recordTag->descSize = count;
	printf ("descSize: %d\n", count);

	//add the recordTag to the recordTag list
	npAddRecordTag (recordTag, dataRef);

	return recordTag;
}

//-----------------------------------------------------------------------------
void* npReadMapNodeCSV (char* buffer, int wordSize, int size, 
						int format, int* scanNumRet, void* dataRef)
{
	int nodeCount = 0;
	int numExpected = 0;		//Expected number for scanNumRet returned from sscanf
//	int scanNumRet = 0;			//sscanf return value. Number of successfuly scanned elements
	
	//Group #1
	int id, type, dataID, selected,	parentID, branchLevel, childID, childIndex, childCount, 
		chInputID, chOutputID, chLastUpdated, average, interval,
		auxA_x, auxA_y, auxA_z, auxB_x, auxB_y, auxB_z;
	//Group #2
	float colorShift,
		rotateVec_x, rotateVec_y, rotateVec_z, rotateVec_s, scale_x, scale_y, scale_z,
		translate_x, translate_y, translate_z, tagOffset_x, tagOffset_y, tagOffset_z, 
		rotateRate_x, rotateRate_y, rotateRate_z, rotate_x, rotate_y, rotate_z;
	//Group #3
	float scaleRate_x, scaleRate_y, scaleRate_z, 
		translateRate_x, translateRate_y, translateRate_z, 
		translateVec_x, translateVec_y, translateVec_z;
	//Group #4
	int shader, geometry;									
	float lineWidth, pointSize, ratio;								
	int colorIndex, color_r, color_g, color_b, color_a, colorFade, textureID;
	//Group #5
	int hide, freeze, topo, facet, autoZoom_x, autoZoom_y, autoZoom_z,
		triggerHi_x, triggerHi_y, triggerHi_z, 
		triggerLow_x, triggerLow_y, triggerLow_z;
	//Group #6
	float triggerLevelHi_x, triggerLevelHi_y, triggerLevelHi_z, 
		triggerLevelLow_x, triggerLevelLow_y, triggerLevelLow_z,
		proximityRadius_x, proximityRadius_y, proximityRadius_z,
		proximity_x, proximity_y, proximity_z;
	//Group #7 has changed, recordID moved
	float segments_x, segments_y, segments_z;
	int tagMode, formatID, tableID, recordID, lineSize;

	//node tree pointers
	NPnodePtr node = NULL;
	pData data = (pData) dataRef;

	//Scan Whole Line in 
	numExpected = 94;				//21d + 19f + 9f + 11f + 14d + 15f + 5d
	*scanNumRet = sscanf( buffer, 
		"%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%f,%f,%f,%f,%f,%f,%f,%f,%f,%f,%f,%f,%f,%f,%f,%f,%f,%f,%f,%f,%f,%f,%f,%f,%f,%f,%f,%f,%f,%d,%d,%f,%f,%f,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%f,%f,%f,%f,%f,%f,%f,%f,%f,%f,%f,%f,%f,%f,%f,%d,%d,%d,%d,%d", 
		&id, &type, &dataID, &selected, &parentID, &branchLevel, &childID, &childIndex, &childCount, 
		&chInputID, &chOutputID, &chLastUpdated, &average, &interval,
		&auxA_x, &auxA_y, &auxA_z, &auxB_x, &auxB_y, &auxB_z,
		&colorShift,
		&rotateVec_x, &rotateVec_y, &rotateVec_z, &rotateVec_s, &scale_x, &scale_y, &scale_z,
		&translate_x, &translate_y, &translate_z, &tagOffset_x, &tagOffset_y, &tagOffset_z,
		&rotateRate_x, &rotateRate_y, &rotateRate_z, &rotate_x, &rotate_y, &rotate_z,
		&scaleRate_x, &scaleRate_y, &scaleRate_z,
		&translateRate_x, &translateRate_y, &translateRate_z,
		&translateVec_x, &translateVec_y, &translateVec_z,
		&shader, &geometry,
		&lineWidth, &pointSize, &ratio,
		&colorIndex, &color_r, &color_g, &color_b, &color_a, &colorFade, &textureID,
		&hide, &freeze, &topo, &facet, &autoZoom_x, &autoZoom_y, &autoZoom_z, 
		&triggerHi_x, &triggerHi_y, &triggerHi_z,
		&triggerLow_x, &triggerLow_y, &triggerLow_z,
		&triggerLevelHi_x, &triggerLevelHi_y, &triggerLevelHi_z,
		&triggerLevelLow_x, &triggerLevelLow_y, &triggerLevelLow_z,
		&proximityRadius_x, &proximityRadius_y, &proximityRadius_z,
		&proximity_x, &proximity_y, &proximity_z,
		&segments_x, &segments_y, &segments_z,
		&tagMode, &formatID, &tableID, &recordID, &lineSize
	);

	//if -1 then exit loop, otherwise err check item count and lineSize
	if( *scanNumRet != numExpected )
	{
		if( *scanNumRet == -1 )
			return node;							// end of file, exit loop
	
		if (*scanNumRet != 0)
			printf( "err 4256 - scanNumRet: %d  id: %d\n", scanNumRet, id);	
		
		return node;								// skip this line
	} 

	if( lineSize != 420 )						//possibly add sizes, debug zz
		printf( "err 4257 - unexpected end line size: %d\n", lineSize );

	//support for CSV ver 1 
	if (format == kNPmapNodeCSVvOne && type == 8)	//workaround for 7-11 to 7-21 CSV files, debug zz
			type = kNodePin;

	//add the node to the data map and map sort list, returns node ptr
	node = npMapNodeAdd (id, type, branchLevel, parentID, format, dataRef);

	//copy the values to the newly created node
	//node->type		= type;					// handled during node creation
	//node->data		= NULL;					// handled during node creation

	node->selected		= selected;

	//node->parent		= NULL;					// handled during node creation
	//node->branchLevel	= 0;					// handled during node creation

	node->childIndex	= childIndex;
	//node->childCount	= childCount;			// handled during node creation

	node->chInputID		= chInputID;
	node->chOutputID	= chOutputID;
	node->chLastUpdated	= chLastUpdated;
	
	node->average		= average;
	node->interval		= interval;
	
	node->auxA.x		= auxA_x;
	node->auxA.y		= auxA_y;
	node->auxA.z		= auxA_z;
	node->auxB.x		= auxB_x;
	node->auxB.y		= auxB_y;
	node->auxB.z		= auxB_z;

	node->colorShift	= colorShift;

	node->rotateVec.x		= rotateVec_x;		//was rotate
	node->rotateVec.y		= rotateVec_y;
	node->rotateVec.z		= rotateVec_z;
	node->rotateVec.angle	= rotateVec_s;	
	
	node->scale.x		= scale_x;
	node->scale.y		= scale_y;
	node->scale.z		= scale_z;
	
	node->translate.x	= translate_x;
	node->translate.y	= translate_y;
	node->translate.z	= translate_z;

	node->tagOffset.x	= tagOffset_x;
	node->tagOffset.y	= tagOffset_y;
	node->tagOffset.z	= tagOffset_z;
	
	node->rotateRate.x	= rotateRate_x;
	node->rotateRate.y	= rotateRate_y;
	node->rotateRate.z	= rotateRate_z;

	node->rotate.x		= rotate_x;					//was rotateRad
	node->rotate.y		= rotate_y;
	node->rotate.z		= rotate_z;
	
	node->scaleRate.x	= scaleRate_x;
	node->scaleRate.y	= scaleRate_y;
	node->scaleRate.z	= scaleRate_z;

	node->translateRate.x = translateRate_x;
	node->translateRate.y = translateRate_y;
	node->translateRate.z = translateRate_z;

	node->translateVec.x = translateVec_x;
	node->translateVec.y = translateVec_y;
	node->translateVec.z = translateVec_z;
	
	node->shader		= shader;
	node->geometry		= geometry;

	node->lineWidth		= lineWidth;
	node->pointSize		= pointSize;
	node->ratio			= ratio;

	node->colorIndex	= colorIndex;
	
	node->color.r		= color_r;
	node->color.g		= color_g;
	node->color.b		= color_b;
	node->color.a		= color_a;

	node->colorFade		= colorFade;
	node->textureID		= textureID;

	node->hide			= hide;
	node->freeze		= freeze;
//	node->center		= center;		//removed	

	node->topo			= topo;			//moved topo
	node->facet			= facet;		//added topo facet number

	node->autoZoom.x	= autoZoom_x;	//moved down a slot
	node->autoZoom.y	= autoZoom_y;
	node->autoZoom.z	= autoZoom_z;

//	node->scroll		= scroll;		//removed made space for facet, zz debug
	
	node->triggerHi.x	= triggerHi_x;
	node->triggerHi.y	= triggerHi_y;
	node->triggerHi.z	= triggerHi_z;

	node->triggerLo.x	= triggerLow_x;
	node->triggerLo.y	= triggerLow_y;
	node->triggerLo.z	= triggerLow_z;
	
	node->setHi.x		= triggerLevelHi_x;
	node->setHi.y		= triggerLevelHi_y;
	node->setHi.z		= triggerLevelHi_z;

	node->setLo.x		= triggerLevelLow_x;
	node->setLo.y		= triggerLevelLow_y;
	node->setLo.z		= triggerLevelLow_z;

	node->proximity.x	= proximityRadius_x;
	node->proximity.y	= proximityRadius_y;
	node->proximity.z	= proximityRadius_z;

	node->proximityMode.x = (int)proximity_x;			//int cast supports 1st ver CSV
	node->proximityMode.y = (int)proximity_y;
	node->proximityMode.z = (int)proximity_z;

	node->segments.x	= (int)segments_x;		//grid segments were stored in node->data,
	node->segments.y	= (int)segments_y;		//was node->data->segments.x
	node->segments.z	= (int)segments_z;		//now node->segments.x

	node->tagMode		= tagMode;
	node->formatID		= formatID;
	node->tableID		= tableID;
	node->recordID		= recordID;
	//node->size		= lineSize;				// handled during node creation

	//support for first version CSV
	if (format == kNPmapNodeCSVvOne)
		npMapCSVvOne (node);

	nodeCount = ++data->map.sortCount;

	//print part of the first few lines of data
	if ( nodeCount < 12)
		printf("file id: %d   type: %d   branchLevel: %d   colorIndex: %d   recordID: %d\n", 
			id, node->type, node->branchLevel, node->colorIndex, node->recordID);
	if ( nodeCount == 12)
		printf("... ");
	if ( nodeCount == (nodeCount / 100) * 100 )	//print id from file every 100 nodes
		printf("%d ", id);

	return node;
}

//-----------------------------------------------------------------------------
void npLoadMapFile (char* buffer, int wordSize, int size, void* dataRef)
{
	int i = 0, count = 0, curs = 0;				//Cursor position in Buffer parsing source
	int scanNumRet = 0;			//sscanf return value. Number of successfuly scanned elements
	int nodeCount = 0;
	int format = 0;
	
	NPnodePtr node = NULL;
	pData data = (pData) dataRef;


	// CSV version 1, starts with a '\n' so line 1 is blank, line 2 is field names
	// and data starts on 3rd line
	// CSV ver 2 starts with field names and data on 2nd line
	format = npFileFormat (buffer, wordSize, size);
	if ( format == kNPmapNodeCSVvOne )							
		count += curs = npSeekToNextLine( buffer ); buffer = &buffer[curs];	//skip blank line

	// print first two lines
	printf ("File Version %d Contents:\n", format);	
	for( i = 0; i < 80; i++ )			//first 80 chars of line one
		printf( "%c", buffer[i]);

	curs = npSeekToNextLine( buffer );	//find next line, but dont advance buffer
	for( i = 0; i < 80; i++ )			//second line
		printf( "%c", buffer[curs+i]);

	//for sorting parent and child nodes that are out of order in the file
	npMapSortInit (data->map.sortID, data);

	printf ("Processing Nodes...\n");

	//scan in all lines and create nodes
	while( scanNumRet != -1 )
	{
		//advance buffer to next line
		count += curs = npSeekToNextLine (buffer); buffer = &buffer[curs];

		// if end of buffer then exit loop
		if (count >= size)
		{
			scanNumRet = -1;
			continue;								// end of file, exit loop
		}

		//maps a single line CSV formatted to a node
		node = npReadMapNodeCSV (buffer, wordSize, size, 
								format, &scanNumRet, dataRef);

		if (node != NULL)
			nodeCount++;
	}	// end loop

	// print last node info
	if (node != NULL )
	{
		printf("\nid: %d   type: %d   branchLevel: %d   colorIndex: %d   recordID: %d\n", 
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
		data->map.activeCam = node->child[node->childIndex];
		npSelectNode (data->map.node[kNPnodeRootGrid], data);
	}

	printf ("Synchronizing Text Tags\n");
	npSyncTags (data);						//make this smarter, debug zz

	printf ("Synchronizing Channel Map\n");
	npSyncChMap (data);							//zz-JJ
}
//MB-End//


//-----------------------------------------------------------------------------
void npLoadTagFile (char* buffer, int wordSize, int size, void* dataRef)
{
	int i = 0, count = 0, curs = 0;				//Cursor position in Buffer parsing source
	int scanNumRet = 0;			//sscanf return value. Number of successfuly scanned elements
	int recordCount = 0;
	int version = 0;
	
	NPnodePtr node = NULL;
	pData data = (pData) dataRef;

	pNPrecordTag recordTag = NULL;


	//update npFileFormat to recognize a Tag file,							debug zz
	//	version = npFileFormat (buffer, wordSize, size);

				//add handling of very small files, less then 40 chars, debug zz
	// print first two lines
	printf ("CSV Tag File Ver: 1\n");	//add file version, debug zz
	for( i = 0; i < 40; i++ )			//first 80 chars of line one
		printf( "%c", buffer[i]);
	printf("\n");

	curs = npSeekToNextLine( buffer );	//find next line, but dont advance buffer
	for( i = 0; i < 80; i++ )			//second line
		printf( "%c", buffer[curs+i]);

	printf ("\nProcessing Tags...\n");

	//scan in all lines and create nodes, exits at end of file
	while( scanNumRet != -1 )
	{
		//advance buffer to next line
		count += curs = npSeekToNextLine (buffer); buffer = &buffer[curs];

		// if end of buffer then exit loop
		if (count >= size)
		{
			scanNumRet = -1;
			continue;								// end of file, exit loop
		}

		//processes a single Tag record, one line in the CSV file
		recordTag = npReadTagFromCSV (buffer, wordSize, size, 
								version, &scanNumRet, dataRef);

		if (recordTag != NULL)
		{
			recordCount++;
			
			//print part of the first few lines of data
			if (1)// recordCount < 12)
				printf("record_id: %d  table_id: %d  title: %0.12s  desc: %0.20s\n",
					recordTag->recordID, recordTag->tableID,
					recordTag->title, recordTag->desc);
			else if ( recordCount == 12)
				printf("... ");
			else if ( recordCount == (recordCount / 100) * 100 )	//print id from file every 100 nodes
				printf("%d ", recordTag->id);
		}
	} // end loop

	// print last record if more then 11 records
	if (recordTag != NULL && recordCount >= 12)
	{
		printf("\nrecord_id: %d  table_id: %d  title: %0.12s  desc: %0.20s\n",
					recordTag->recordID, recordTag->tableID,
					recordTag->title, recordTag->desc);
	}

	printf ("\nRecord Count: %d\n", recordCount);

	printf ("Synchronizing Text Tags...\n");
	npSyncTags(dataRef);		//make this smarter, debug zz
}


//-----------------------------------------------------------------------------
int npFileOpenTag (char* filePath, int wordSize, int size, void* dataRef)
{
	FILE* file = NULL;
	size_t count = 0;
	int i = 0;
	int byteCount = 0;
	char* bufferIndex = 0;
	
	pData data = (pData) dataRef;

	char* buffer = (char*)malloc(kNPmapFileBufferMax);
	if (buffer == NULL)
	{
		printf ("err 4985 - malloc failed, cannot write file");
		return 0;
	}


	printf("\nOpen File: %s\n", filePath);
	file = npFileOpen(filePath, "r", dataRef);	// open file in read mode "r"

	if( file != NULL )   // check to see if file exists
	{
		printf("Reading File...\n");
		count = npFileRead (buffer, 1, kNPmapFileBufferMax, file, dataRef);
		printf("Bytes Read: %d\n", count);

		npFileClose (file, dataRef);	//done reading, close file
		
		if (count > 0)
		{
			printf("Loading File\n");
			npLoadTagFile (buffer, wordSize, count, dataRef);	//process data
			printf ("Done\n\n");
		}
		else
			printf("err 4987 - zero bytes read\n");
	}
	else
		printf("err 4988 - cannot open file \n");


	free (buffer);
	return 1;
}
//-----------------------------------------------------------------------------
int npFileSaveTag (char* filePath, int wordSize, int size, void* dataRef)
{
	printf ("err 4339 - failed to save CSV Tag file\n");
	return 0;
}

//-----------------------------------------------------------------------------
int npFileOpenMap (char* filePath, int wordSize, int size, void* dataRef)
{
	FILE* file = NULL;
	size_t count = 0;
	int i = 0;
	int byteCount = 0;
	char* bufferIndex = 0;
	
	pData data = (pData) dataRef;

	char* buffer = (char*) malloc(kNPmapFileBufferMax);
	if (buffer == NULL)
	{
		printf ("err 4285 - malloc failed, cannot write file");
		return 0;
	}

	printf("\nOpen File: %s\n", filePath);
	file = npFileOpen (filePath, "r", dataRef);	// open file in read mode "r"

	if( file != NULL )   // check to see if file exists
	{
		printf("Reading File...\n");
		count = npFileRead (buffer, 1, kNPmapFileBufferMax, file, dataRef);
		printf("Bytes Read: %d\n", count);

		npFileClose (file, dataRef);	//done reading, close file
		
		if (count > 0)
		{
			printf("Loading File\n");
			npLoadMapFile (buffer, wordSize, count, dataRef);	//process data
			printf ("Done\n\n");
		}
		else
			printf("err 4287 - zero bytes read\n");
	}
	else
		printf("err 4288 - cannot open file \n");

	free (buffer);

	return 1;
}


//-----------------------------------------------------------------------------
int npFileSaveMap (char* filePath, int wordSize, int size, void* dataRef)
{
	int i = 0;
	size_t count = 0;
	FILE* file = NULL;

	pData data = (pData) dataRef;
	NPnodePtr node = NULL;

	char* buffer = (char*) malloc(kNPmapFileBufferMax);
	if (buffer == NULL)
	{
		printf ("4214 error - malloc failed, cannot write file\n");
		free (buffer);
		return 0;
	}

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
	printf("Processing...\n");
	count = npGetMapFile (buffer, 1, kNPmapFileBufferMax, dataRef);

	// write the nodes file
	printf("Writing...\n");
	count = npFileWrite (buffer, 1, count, file, dataRef);	

	// print first line of file contents
	if (count > 0)
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

	i = npFileClose(file, dataRef);
	if (i)
		printf("err 4217 - file close failed, return: %d\n", i);

	printf("Done\n\n");


	// separate into CSV files and equivalent db tables, debug, zz
	// file tables include pin data, child lists, camera data

	free (buffer);

	return 1;
}


//-----------------------------------------------------------------------------
// if not saved then opens SaveAs dialog before reseting app
void npCloseFileMapFile (int size, void* dataRef)
{
	return;
}


//-----------------------------------------------------------------------------
int npGetMapFileAttrib (int attrib, void* dataRef)
{
	switch (attrib)
	{
	case kNPversion: ; break;
	case kNPtableCount: ; break;	// number of struct equivalent data tables 
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




//-----------------------------------------------------------------------------
// keep track of a bufferIndex for each list
// write list of children
// write node or node list
// write node specific data to a separate list, ie pin, camera, etc
// write list of channels,						channels NOT YET IMPLEMENTED, debug, zz
//-----------------------------------------------------------------------------
int npWriteNode (char* buffer, void* nodeRef, int format, void* dataRef)
{
	int n			= 0;
	int parentID	= 0;

	NPnodePtr parent = NULL;
	NPnodePtr node = (NPnodePtr) nodeRef;

	pData data = (pData) dataRef;

					// pointers to the node data, update to be a generic list, debug, zz
					// break into separate buffers, debug zz
	char* nodePtr		= data->io.write[kNPnode];


	// set the parentID, if NULL it will be zero
	if (node->parent != NULL)
	{
		parent = node->parent;
		parentID = parent->id;

		// printf ("parentID %d\n", parentID);
	}
/*
	switch (format)
	{	
		case kNPmapNodeOldCSV :	//writes node to buffer, single line of CSV
			npWriteMapNodeCSV(buffer, nodeRef, format, dataRef);
			break;
		case kNPmapNodeCSV :		//writes node to buffer, single line of CSV
			npWriteMapNodeCSV(buffer, nodeRef, format, dataRef);
			break;
		case kNPmapNodeKML :		//writes node to buffer, single line of CSV
			npWriteMapNodeKML (buffer, nodeRef, format, dataRef);
			break;
		case kNPmapNodeSQL :		//writes node to buffer, single line of CSV
			npWriteMapNodeKML (buffer, nodeRef, format, dataRef);
			break;
		case default : break;
	}
*/
	//format as CSV and fill the buffer, broken into groups for readability
	n += sprintf ((nodePtr + n), "%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d",
		node->id,					//id is unique to this node
		node->type,		
		node->id,					// node->data->id uses this node id
		
		node->selected,
		parentID,					// parent id replaces pointer to parent
		node->branchLevel,
		node->id,					// node->id is same as childList->id
		node->childIndex,
		node->childCount,
		
		node->chInputID,
		node->chOutputID,	
		node->chLastUpdated,

		node->average,
		node->interval,

		node->auxA.x,
		node->auxA.y,
		node->auxA.z,

		node->auxB.x,
		node->auxB.y,
		node->auxB.z
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
//		node->center,
		node->topo,
		node->facet,

		node->autoZoom.x,
		node->autoZoom.y,
		node->autoZoom.z,

//		node->scroll,
		
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

	//after last group, end the node row using a newline
	n += sprintf ((nodePtr + n), "\n");	

	//increment the buffer
	data->io.write[kNPnode] += n;

	return n;
}


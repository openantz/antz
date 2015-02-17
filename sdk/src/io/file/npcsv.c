/* -----------------------------------------------------------------------------
*
*  npcsv.c
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

#include "npcsv.h"


#include "../../os/npos.h"
#include "../npfile.h"

#include "../db/npdb.h"			//zz db2 added to access member elements

#include "../gl/nptags.h"
#include "../npgl.h"
#include "../../npctrl.h"

//-----------------------------------------------------------------------------
void npInitCSV (void* dataRef)
{
	return;
}

//-----------------------------------------------------------------------------
void npCloseCSV (FILE* filePtr, void* dataRef)
{
	fclose(filePtr);
	
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
FILE* npOpenCSV (const char* fileName, const char* mode, void* dataRef)
{
	FILE* file = NULL;

	file = fopen(fileName, mode); 

	if ( file == NULL )
	{
		perror("Failed to open/create file");
		return NULL;
	}

	// add code to add the file ptr to the data->map							debug, zz

	return file;
}


//-----------------------------------------------------------------------------
int npReadCSV (void* readBuffer, int elementSize, int elementCount, 
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
void npTestCSV (void* fileName, int wordSize, int size, void* dataRef)
{
	FILE *file;
	size_t count;
	char *str = (char*)fileName;
	int i = 0;


	char buffer[256];
/*
	// get the Current Working Directory
	npGetCWD (buffer, &size);

	printf("CWD path : ");
	for(i = 0; i < size; i++)
		printf ("%c", buffer[i]);

	printf("\n");
*/
	// open/create and write to the file, "w" overwrites existing files
	file = npOpenCSV (fileName, "r+", dataRef);

	// write the file path in the file as text
	count = npWriteCSV (buffer, 1, strlen(buffer), file, dataRef);

	printf("wrote %d bytes.\n", count); 

	// read the file
//	npFileRewind(file);

	size = npReadCSV (buffer, 1, 256, file, dataRef);


	printf("file contents : \n");
	for(i = 0; i < size; i++)
		printf ("%c", buffer[i]);

//mkdir(strB, S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH);

}

//-----------------------------------------------------------------------------
FILE* npCSVDialog (const char* fileName, int dialogType, void* dataRef)
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
int npWriteCSV (const void* str, int wordSize, int size, FILE* file, void* dataRef)
{
	return fwrite( str, 1, strlen(str), file );
}



void npSaveCSV (FILE* file, void* dataRef)
{
	return;
}


//-----------------------------------------------------------------------------
void npImportCSV (FILE* file, int type, void* dataRef)
{
	return;
}

//-----------------------------------------------------------------------------
void npExportCSV (FILE* file, void* dataRef)
{
	return;
}

//-----------------------------------------------------------------------------

// if not saved then opens SaveAs dialog before reseting app
void npCloseFileCSV (int size, void* dataRef)
{
	return;
}

//-----------------------------------------------------------------------------
void npCSVGetAttrib (int size, void* dataRef)
{
	return;
}

//-----------------------------------------------------------------------------
void npCSVSetAttrib (int size, void* dataRef)
{
	return;
}

//-----------------------------------------------------------------------------
//passed an open State File and reads it in based on type
void npCSVtoMap (FILE* file, int type, void*dataRef)
{
	size_t count = 0;
	int i = 0;
	int byteCount = 0;
	char* bufferIndex = 0;
	
	pData data = (pData) dataRef;
	pNPnode node = NULL;


	char* buffer = (char*)malloc(kNPmapFileBufferMax);
	if (buffer == NULL)
	{
		printf ("err 4285 - malloc failed, cannot write file");
		return;
	}

	if( file != NULL )   // check to see if file exists
	{
		printf("Reading File...\n");
		count = npFileRead (buffer, 1, kNPmapFileBufferMax, file, dataRef);
		printf("Bytes Read: %d\n", count);

		npFileClose (file, dataRef);	//done reading, close file
		
		if (count > 0)
		{
			printf("Loading File\n");			//add table type handling, debug zz
			npLoadMapFile (buffer, 1, count, dataRef);	//process data
			printf ("Done\n\n");
		}
		else
			printf("err 4297 - zero bytes read\n");
	}
	else
		printf("err 4298 - cannot open file \n");


	free (buffer);

	return;
}

//zz debug, need a table similar to npmap.c generates for node header with ptrs like chmap
//need arrays for sub elements to, color = black, white...

//-----------------------------------------------------------------------------
char* npGetValueStr(char* dest, pNPmapLink item, void* dataRef) //*(int*)mapGlobals[id].mapPtr,
{
	//*(int*)mapGlobals[id].mapPtr,

	
	pNPubyteRGBA	ubyteRGBA = NULL;
	pNPfloatRGBA	floatRGBA = NULL;
	pNPfloatXYZ		floatXYZ = NULL;
	pNPfloatXY		floatXY = NULL;
	pNPintXYZ		intXYZ = NULL;
	pNPintXY		intXY = NULL;

	//zz debug, should this type be here???
	pNPgl			gl = NULL;

	//	_itoa(*(int*)item->mapPtr, dest, 0);	//zz debug optimize

	// sort order is map_path, np_osc, tag match, element match
	//type tags match
	//elements match
	//then elementB
//	switch (item->typeTagA)
//	{
//		case 
	switch (item->typeA)
	{
		//the constant is the functionID, plugins can use number or 'name'
		case kNPcmdFuncPtr : sprintf (dest, "%d", kNPcmdFuncPtr); break;

		case kNPfloat : sprintf (dest, "%f", *(float*)item->mapPtr); break;
		case kNPint : sprintf (dest, "%d", *(int*)item->mapPtr); break;
		case kNPcstrPtr : sprintf (dest, "%s", (char*)item->mapPtr); break;
		

		case kNPubyteRGBA :
			ubyteRGBA = item->mapPtr;
			sprintf(dest, "%d,%d,%d,%d", 
					ubyteRGBA->r,
					ubyteRGBA->g,
					ubyteRGBA->b,
					ubyteRGBA->a
					);
			break;

		case kNPfloatRGBA :
			floatRGBA = item->mapPtr;
			sprintf(dest, "%f,%f,%f,%f",
					floatRGBA->r,
					floatRGBA->g,
					floatRGBA->b,
					floatRGBA->a
					);
			break;

		case kNPfloatXYZ :
			floatXYZ = item->mapPtr;
			sprintf (dest, "%f,%f,%f", floatXYZ->x, floatXYZ->y, floatXYZ->z);
			break;

		case kNPintXYZ :
			intXYZ = item->mapPtr;
			sprintf (dest, "%d,%d,%d", intXYZ->x, intXYZ->y, intXYZ->z);
			break;

		case kNPfloatXY :
			floatXY = item->mapPtr;
			sprintf (dest, "%f,%f", floatXY->x, floatXY->y);
			break;

		case kNPintXY :
			intXY = item->mapPtr;
			sprintf (dest, "%d,%d", intXY->x, intXY->y);
			break;

	//zz debug probably should not be here this is the element not the type_id...
		case kNPpositionXY :
			gl = item->mapPtr;
			sprintf (dest, "%d,%d", gl->position.x, gl->position.y);
			break;

		case kNPwindowSizeXY :
			gl = item->mapPtr;
			sprintf (dest, "%d,%d", gl->windowSize.x, gl->windowSize.y);
			break;

		default : //report err
				strcpy (dest, "22");//"err 9999 - unknown pNPmapType item");
			break;
	}

	
	return  dest;
}


//-----------------------------------------------------------------------------

	// ISSUES 
	// best not to limit osc str length
	// need to get global ptr when populating list, or servicing it....
	// advantage to both runtime const to ptr conversion and pre-mapped
	// may change data pointer for struct member mapping pointer offsets
	// nodeMemberPtrOffset = (&node->translate - node)
	// (pNPfloatXYZ)node+nodeMemberPtrOffset

	// for example /np/node/42 iiiifff...i 42 1 1 7... 420
	// needs to get node 42 then populate, more of an offset method


//zz debug, this would seem to be a map_path_id not typeA?
//this is currently not called by anything
//npMapIDtoPtr
void* npMapElementIDtoPtr (int elementID, void* dataRef);
//-----------------------------------------------------------------------------
void* npMapElementIDtoPtr (int elementID, void* dataRef)
{
	int i = 0;
	int itemCount = 0;

	pData data = (pData) dataRef;

	pNPmapLink mapType = NULL;


	//get the map and itemCount
	//first check globals
	mapType = data->map.typeMapGlobals;
	itemCount = data->map.globalsCount;

	//scan map using the ID and if found return a pointer to the data
	for (i=0; i <= itemCount; i++)
	{
		if (mapType[i].elementID == elementID)
			return mapType[i].mapPtr;
	}

	//then check osc
	mapType = data->map.typeMapOSC;
	itemCount = data->map.oscCount;

	//scan map using the ID and if found return a pointer to the data
	for (i=0; i <= itemCount; i++)
	{
		if (mapType[i].elementID == elementID)
			return mapType[i].mapPtr;
	}

	return NULL;
}


char* npGetMapTypeName (int elementID, void* dataRef);
//-----------------------------------------------------------------------------
char* npGetMapTypeName (int elementID, void* dataRef)
{
	int i = 0;
	int itemCount = 0;

	pData data = (pData) dataRef;

	pNPmapLink mapType = NULL;


	//get the map and itemCount
	//first check globals
	mapType = data->map.typeMapGlobals;
	itemCount = data->map.globalsCount;

	//scan map using the ID and if found return a pointer to the data
	for (i=0; i <= itemCount; i++)
	{
		if (mapType[i].elementID == elementID)
			return mapType[i].elementA;
	}

	//then check osc
	mapType = data->map.typeMapOSC;
	itemCount = data->map.oscCount;

	//scan map using the ID and if found return a pointer to the data
	for (i=0; i <= itemCount; i++)
	{
		if (mapType[i].elementID == elementID)
			return mapType[i].elementA;
	}

	return NULL;
}


int npMapStrToID (const char* str, void* dataRef);
//-----------------------------------------------------------------------------
int npMapStrToID (const char* str, void* dataRef)
{
	int i = 0;
	int result = 0;
	int itemCount = 0;

	pData data = (pData) dataRef;

	pNPmapLink mapType = NULL;
	pNPmapLink mapItem = NULL;

	//first check globals
	mapType = data->map.typeMapGlobals;
	itemCount = data->map.globalsCount;

	//scan elementA lists and return the corresponding constant kNP... = 42
	for (i=0; i <= itemCount; i++)
	{
		mapItem = &mapType[i];

		if (strncmp(str, (const char*)mapItem->elementA, kNPnameMax) == 0)
		{
			result = mapItem->elementID;
			break;
		}
	}
		
	//then check OSC map
	mapType = data->map.typeMapOSC;
	itemCount = data->map.oscCount;

	//scan elementA lists and return the corresponding constant kNP... = 42
	for (i=0; i <= itemCount; i++)
	{
		mapItem = &mapType[i];

		if (strncmp(str, (const char*)mapItem->elementA, kNPnameMax) == 0)
		{
			result = mapItem->elementID;
			break;
		}
	}

	return result;
}

void* npMapStrToPtr (const char* str, void* dataRef);
//-----------------------------------------------------------------------------
void* npMapStrToPtr (const char* str, void* dataRef)
{
	//quick implementation... not efficient, make direct like others..
//	return nMapIDtoPtr ( npMapStrToID (str, dataRef), dataRef );

	int i = 0;
	int itemCount = 0;

	pData data = (pData) dataRef;

	pNPmapLink mapType = NULL;

	//get the map and itemCount
	mapType = data->map.typeMapGlobals;
	itemCount = data->map.globalsCount;

	//scan map using the ID and if found return a pointer to the data
	for (i=0; i <= itemCount; i++)
	{
		if (strncmp(str, (const char*)mapType[i].elementA, kNPnameMax) == 0)
			return mapType[i].mapPtr;
	}

	//get the map and itemCount
	mapType = data->map.typeMapOSC;
	itemCount = data->map.oscCount;

	//scan map using the ID and if found return a pointer to the data
	for (i=0; i <= itemCount; i++)
	{
		if (strncmp(str, (const char*)mapType[i].elementA, kNPnameMax) == 0)
			return mapType[i].mapPtr;
	}

	return NULL;
}


//upgrade to HASH method or similar
//-----------------------------------------------------------------------------
void* npMapElementToPtr (const char* str, void* dataRef)
{
	//quick implementation... not efficient, make direct like others..
//	return nMapIDtoPtr ( npMapStrToID (str, dataRef), dataRef );

	int i = 0;
	int itemCount = 0;

	pData data = (pData) dataRef;

	pNPmapLink mapType = NULL;

	//get the map and itemCount
	mapType = data->map.typeMapGlobals;
	itemCount = data->map.globalsCount;

	//scan map using the ID and if found return a pointer to the data
//	printf("\nitemCount : %d", itemCount); // temp, lde
//	printf("\nstr : %s\n", str); // temp, lde
	
	for( i=0; i < itemCount; i++) // Fixed, lde
	{
		//printf("%d ",i);
		//printf("\n(const char*)mapType[%d].elementA : %s\n", i, (const char*)mapType[i].elementA);
		if (strncmp(str, (const char*)mapType[i].elementA, kNPnameMax) == 0) 
		{
			//printf("\nFound It 5472\n");
			return mapType[i].mapPtr;
		}
	}
	
/*
	for( i=1; i <= itemCount; i++ )		//zz debug, why i=1 and <= works
	{									//instead of i=0 and <
		if (strncmp(str, (const char*)mapType[i].elementA, kNPnameMax) == 0) // Fails here, lde @todo
			return mapType[i].mapPtr;
	}	
*/
	
	//get the map and itemCount
	mapType = data->map.typeMapOSC;
	itemCount = data->map.oscCount;

	//scan map using the ID and if found return a pointer to the data
	for (i=1; i <= itemCount; i++)
	{
		if (strncmp(str, (const char*)mapType[i].elementA, kNPnameMax) == 0)
			return mapType[i].mapPtr;
	}

	return NULL;
}

//-----------------------------------------------------------------------------
char* npMapTypeName (int mapType, void* dataRef)
{
	//quick implementation... not efficient, make direct like others..
//	return nMapIDtoPtr ( npMapStrToID (str, dataRef), dataRef );

	int i = 0;
	int itemCount = 0;

	pData data = (pData) dataRef;

	pNPmapType mapTypeList = NULL;

	//get the map and itemCount
	mapTypeList = data->map.mapTypeList;
	itemCount = data->map.mapTypeCount;

	//scan map using the ID and if found return a pointer to the data
	for( i=1; i <= itemCount; i++ )		//zz debug, why i=1 and <= works
	{									//instead of i=0 and <
		if( mapTypeList[i].type == mapType )
			return mapTypeList[i].name;
	}

	//if not found
	npPostMsg( " err 4427 - unknown mapType", kNPmsgErr, data );

	return NULL;
}

//-----------------------------------------------------------------------------
void* npMapAddressToPtr (const char* str, void* dataRef)
{
	//quick implementation... not efficient, make direct like others..
//	return nMapIDtoPtr ( npMapStrToID (str, dataRef), dataRef );

	int i = 0;
	int itemCount = 0;

	pData data = (pData) dataRef;

	pNPmapLink mapType = NULL;

	//get the map and itemCount
	mapType = data->map.typeMapGlobals;
	itemCount = data->map.globalsCount;

	//scan map using the ID and if found return a pointer to the data
	for (i=0; i <= itemCount; i++)
	{
		if (strncmp(str, (const char*)mapType[i].elementB, kNPnameMax) == 0)
			return mapType[i].mapPtr;
	}

	//get the map and itemCount
	mapType = data->map.typeMapOSC;
	itemCount = data->map.oscCount;

	//scan map using the ID and if found return a pointer to the data
	for (i=0; i <= itemCount; i++)
	{
		if (strncmp(str, (const char*)mapType[i].elementB, kNPnameMax) == 0)
			return mapType[i].mapPtr;
	}

	return NULL;
}


//-----------------------------------------------------------------------------
pNPmapLink npMapAddressToMapTypeItem (const char* str, void* dataRef)
{
	//quick implementation... not efficient, make direct like others..
//	return nMapIDtoPtr ( npMapStrToID (str, dataRef), dataRef );

	int i = 0;
	int itemCount = 0;

	pData data = (pData) dataRef;

	pNPmapLink mapType = NULL;

	//get the map and itemCount
	mapType = data->map.typeMapGlobals;
	itemCount = data->map.globalsCount;

	//scan map using the ID and if found return a pointer to the data
	for (i=0; i <= itemCount; i++)
	{
		if (strncmp(str, (const char*)mapType[i].elementB, kNPnameMax) == 0)
			return &mapType[i];
	}

	//get the map and itemCount
	mapType = data->map.typeMapOSC;
	itemCount = data->map.oscCount;

	//scan map using the ID and if found return a pointer to the data
	for (i=0; i <= itemCount; i++)
	{
		if (strncmp(str, (const char*)mapType[i].elementB, kNPnameMax) == 0)
			return &mapType[i];
	}

	printf( "OSC Address: %s\n", str ); //zz osc debug

	return NULL;
}


//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
void npMapTypeInit (void* dataRef)
{
	int i = 0;
	int size = 0;

	pData data = (pData) dataRef;

	pNPnode rootGrid = data->map.node[kNPnodeRootGrid];

	// DB Table Formats
	// CSV - our 'native' external format... Lowest-Common-Denominator (LCD)
	// directly translates to our internal C-structs and optimized for GL draw
	// ...
	// MySQL tables are directly (loss-less) inter-changable with our CSV files
	// JSON allows efficiently describing data tree structure and its members
	// XML is less efficient then JSON but similar in many ways

	// OSC - our 'native' Network PROTOCAL
	// OSC is an oddball... its a 'keep it simple stupid' approach (to a fault)
	// typically OSC is sent over UDP/IP, but can be sent over any network comm
	// does NOT provide quality-of-service(QOS) but works with AVB to do so
	// OSC-MIDI bridge (3rd party software modules and standalone hardware)
	
	// REALTIME Video Network Hardware Layers (QOS)
	// Serial such as RS232, RS422, serial over USB and SPI for chip-to-chip
	// DVB - Digital Video Broadcasting transmits over satellite, fiber, coax
	// AVB - Audio Video Broadcasting provides QOS for AVB Ethernet devices
	// SDI - Serial Digital Interface for video and audio or coax, has metadata

	// HIGH Performance Networking
	// FC - Fibre Channel is the HIGHEST performance, 8Gbps 250K IOPS
	// Infiniband - Bandwidth similar to FC, IOPS are about 25K IOPS
	// 10GigE - Bandwidth performance varies 6-10Gbps (typical) 10K IOPS

	// ATM - A-sync Transfer M...

	// In general, FC is best for HPC environments such as VR Caves
	//----

	//----
		// itemID = can be a specific item number '42', a range '[42:100]', a mixed list '{42, 43, 44, 45, [55:62]}'	//zz debug, change mapType->item_id to string
		// or the '*' (wildcard) can be used to assign (default init) values to all items
		// id = 'active' applies to all currently active (selected) item(s)
		// item_id = 0 is a valid index
		// negative id values behaviour is undefined and will generate a warning... for example '[3-10]' is an invalid format
		// single instance items (globals) without an id use item_id = 0, for 'nil' 
		// item_id meaning is based on the map_path format...
		// OSC uses the ID field to represent the connection ID where as 'np_...' uses it as an array index or record_id
		
		// bias = 0 for full duplex, output = TX = +1.0 for A to B, input = RX = -1.0 for B to A
		// bias  0.5 = 100% A to B and 50% B to A ... ie mapping XY to RGBA color
		//
		// key value (record_id) is a combo of the map_path (file path, DB name and table_id...) and id

		//build descriptor table for globals with pointer to specific elements
		//note that mapItemID = element = address (CSV field name for native types)
	
	//--------------------------------------------------------------------------
	// A-B map link describes both link ends usinng a 3 part address + Type Tag
	// 1st part maps the path to the location of the file, DB table, net, RAM
	// 2nd part is the row id(s), can be nil = "" or a set {1,2,3,42,42,[50:99]}
	// 3rd part is the column field element name
	// plus a type tag to desribe parameter value types
	//--------------------------------------------------------------------------
	NPmapLink mapGlobals[] = {								//zz swap id <--> element
		//	data struct ptr			elementTypeID	element		direction	mapPath		index	elementA,  id	element				type_tag
		{ (void*)NULL,				kNPint,			kNPgNull,			0,	"np",			0,	"null",				"i",	"null" },

//		{ &data->io,				kNPstrList,		kNPmainArgcArgv,	0,	"globals",		0,	"main_argc_argv",	"is",	},

		{ &data->io.gl.alphaMode,	kNPint,			kNPgAlphaMode,		0, 	"np_gl",		1,	"alpha_mode",		"i",	"Transparency Mode" },	
		{ &data->io.clear,			kNPfloatRGBA,	kNPgBackground,		0,	"np_gl",		1,	"background_rgba",	"ffff",	"Background Color" },
								
		{ &data->io.gl.fullscreen,	kNPint,			kNPgFullscreen,		0,	"np_gl",		1,	"fullscreen",		"i",	},
	//	{ &data->io.gl.width,		kNPint,			kNPwidth,			0,	"np_gl",		1,	"width",			"i",	},
	//	{ &data->io.gl.height,		kNPint,			kNPheight,			0,	"np_gl",		1,	"height",			"i",	},
		{ &data->io.gl.windowSize,	kNPintXY,		kNPwindowSize,		0,	"np_gl",		1,	"window_size_xy",	"ii",	},
		{ &data->io.gl.position,	kNPintXY,		kNPposition,		0,	"np_gl",		1,	"position_xy",		"ii",	},
	
		{ &data->io.gl.hud.console.level, kNPint,	kNPhudLevel,		0,	"np_gl",		1,	"hud_level",		"i",	"HUD mode level" },
		{ &data->io.gl.subsample,	kNPint,			kNPsubsample,		0,	"np_gl",		1,	"subsample",		"i",	"Subsample nodes" },
		
		{ &data->io.mouse.tool,		kNPint,			kNPmouseTool,		0,	"np_mouse",		1,	"tool",				"i",	"Tool selected" },
		{ &data->io.mouse.camMode,	kNPint,			kNPmouseCamMode,	0,	"np_mouse",		1,	"cam_mode",			"i",	"Mouse Camera Mode" },
		{ &data->io.mouse.pickMode,	kNPint,			kNPmousePickMode,	0,	"np_mouse",		1,	"pick_mode",		"i",	"Mouse Pick Mode" },

		//zzd // uncommented, lde
		/*
		{ data->io.db.activeDB->host->ip,		kNPcstrPtr, kNPloginHostIP,	0,	"np_db",	1,	"host_ip",			"s",	"Host IP address" },
		{ data->io.db.activeDB->host->user,			kNPcstrPtr, kNPloginUser,	0,	"np_db",	1,	"user",				"s",	"Username" },
		{ data->io.db.activeDB->host->password,		kNPcstrPtr, kNPloginPassword, 0, "np_db",	1,	"password",			"s",	"Password" },
		{ data->io.db.activeDB->host->type,		kNPcstrPtr, kNPloginHostType, 0, "np_db",	1,	"db_type",			"s",	"Database Type" },
		 */
		{ data->io.db.hosts[1]->ip,			kNPcstrPtr, kNPloginHostIP,		0, "np_db",	1,	"host_ip",			"s",	"Host IP address" },
		{ data->io.db.hosts[1]->user,		kNPcstrPtr, kNPloginUser,		0, "np_db",	1,	"user",				"s",	"Username" },
		{ data->io.db.hosts[1]->password,	kNPcstrPtr, kNPloginPassword,	0, "np_db",	1,	"password",			"s",	"Password" },
		{ data->io.db.hosts[1]->type,		kNPcstrPtr, kNPloginHostType,	0, "np_db",	1,	"db_type",			"s",	"Database Type" },
		
	//	{ &data->io.mouse.PickMode,	kNPint,			kNPmouseCamMode,	0,	"np_mouse",		1,	"cam_mode",			"i",	"Mouse Camera Mode" },
	
		// tables require the element list to be unique, but only within the table scope

		// Play/Pause tracks
		// OSC IP and port TXRX pairs

		//{ data->io.osc.list[].protocal,	kNPcstrPtr,		kNPprotocal,0,	"np_connect",	1,	"protocal",				"s",	"Protocal ie: mysql osc vrpn snmp" },
		{ &data->io.osc.list[0].txIP,	kNPcstrPtr,		kNPtxIP,	0,	"np_osc",	1,	"tx_ip",				"s",	"Transmit IP" },
		{ &data->io.osc.list[0].rxIP,	kNPcstrPtr,		kNPrxIP,	0,	"np_osc",	1,	"rx_ip",				"s",	"Receive IP" },
		{ &data->io.osc.list[0].txPort,	kNPint,			kNPtxPort,	0,	"np_osc",	1,	"tx_port",				"i",	"TX Port" },
		{ &data->io.osc.list[0].rxPort,	kNPint,			kNPrxPort,	0,	"np_osc",	1,	"rx_port",				"i",	"RX Port" },

		// URL for external Browser record retrieval
		{ data->io.url,				kNPcstrPtr,		kNPgBrowserURL,		0, "np_browser",	1,	"url",				"s",	"Browser URL" },

		{ &data->map.globalsCount,	kNPint,			kNPitemCount,		1, "np_globals",	1,	"item_count",		"i",	"Item count for this table"}	//end
	};

	// antz native csv table field 'elementB' name matches the 'elementA' name
	// other 3rd party CSV table fields can be mapped and would NOT match
	// OSC often uses a float normalized 0.0 to 1.0 for sliders, buttons...


//JJ-zz-osc

	// go through all nptypes.h lists and make one for elements and one for paths
	
	// may add connection specific OSC maps for greater 3rd party support //zz debug
	
	//zz-JJ planning to add base_type(globals,node...) and record_id columns

	//make sure all the mapPath constants are in a single enumerated list

	// list processed from beginning to end, new items added to end 
	// this means they are processed first and override earlier entries... similar to c++ class inheritance
	// shortcut addresses are used, perhaps expand this to auto-complete partial address, return close matches, suggest...
	// some elements trigger function calls such as fullscreen, screen_size, etc... 
	NPmapLink mapOSC[] = {
	//	data struct ptr			elementTypeID	mapPathB	direction  element		id	type_tag		mapPathA,		id	element					type_tag
	{ (void*)NULL,					kNPint,		kNPgNull,			0, "osc",		0,	"null",					"",		0, "osc",		0,	"/np/null",				"i" },

	//alternate layout comparing set A with set B
	{ &data->io.gl.alphaMode,	kNPint,			kNPgAlphaMode,							//C struct
							0, 	"np_gl",	1,	"alpha_mode",				"i",			//set A C-struct converted to String Version
								"osc",		0,	"/np/gl/1/alpha_mode",		"f"			},	//set B external mapping type such as OSC or CSV														
											//  kNPcolorRGBA	..						//clear_color													//back_color_rgba 
	{ &data->io.clear,			kNPfloatRGBA,	kNPgBackground,		0, "np_gl",		1,	"color_rgba",	"ffff",	"osc",  0,	"/np/gl/1/color_rgba", "ffff" , "Background Color" },

	{ &data->io.clear.r,		kNPfloat,		kNPgBackgroundR,	0, "np_gl",		1,	"color_r",		"f",	"osc",  0,	"/np/gl/1/color_r",	"f" , "Background Color R" },
	{ &data->io.clear.g,		kNPfloat,		kNPgBackgroundG,	0, "np_gl",		1,	"color_g",		"f",	"osc",  0,	"/np/gl/1/color_g",	"f" , "Background Color G" },
	{ &data->io.clear.b,		kNPfloat,		kNPgBackgroundB,	0, "np_gl",		1,	"color_b",		"f",	"osc",  0,	"/np/gl/1/color_b",	"f" , "Background Color B" },
	{ &data->io.clear.a,		kNPfloat,		kNPgBackgroundA,	0, "np_gl",		1,	"color_a",		"f",	"osc",  0,	"/np/gl/1/color_a",	"f" , "Background Color A" },

	
	{ &data->map.node,			kNPint,			kNPrecordID,	    1, "np_node",		1,	"record_id",	"i",	"osc",  0,	"/np/node/42/record_id",	"i" },	//should be a double insted of 'i'
																					//0 denotes wildcard, ie: * = "selected" for selected node(s) kNPcmdTag makes a tag for currently selected items
																					// or "active" for current active target, or "set*" for all selection sets
//	{ &data->map.node,			kNPtagPtr,		kNPtag,	    1, "np_node",	0,	"tag",				"iss",	"osc",  0,	"/np/node/*/tag",		"iss" },	//get or set tag, create tag if none

	// setting and instance of a node or tag that does not exist will create it on the fly
	{ &data->map.node,			kNPint,			kNPtagID,			1, "np_node",	0,	"tag_id",			"i",	"osc",  0,	"/np/node/*/tag_id",	"i" },		//get or set tagID
	{ &data->map.node,			kNPcstrPtr,		kNPtagName,			1, "np_node",	1,	"tag_name",			"s",	"osc",  0,	"/np/node/*/tag_name",	"s", "redirect: /np/tag/[../node/*/tag_id]/name" },
	{ &data->map.node,			kNPcstrPtr,		kNPtagDesc,			1, "np_node",	1,	"tag_desc",			"s",	"osc",  0,	"/np/node/*/tag_desc",	"s" },

	{ data->io.url,				kNPcstrPtr,		kNPgBrowserURL,		0, "np_browser", 1,	"/np/browser/1/url", "s",	"osc",	 0,	"/mrmr/textinput/7/z-zs-iPhone", "s" },


	// data->io.gl.fullscreen +1 direction status flag is OUT (only)
	// +1 direction status flag denotes A to B and that its an output (read only)
	// data->io.g[id] context 'id' = 1 is the main GL window, 0 = system console
//	{ &data->io.gl,				kNPint,			kNPfullscreen,	    1, "np_gl",		1,	"fullscreen",		"i",	"osc",  0,	"/np/cmd/fullscreen",	"f" },
//	{ &data->ctrl.cmdFunc,		kNPcmdFuncPtr,	kNPcmdTrigger,		0, "np_cmd",	kNPcmdFullscreen, "fullscreen",		"i",	"osc",  0,	"/2/push7",	"f" },

	// kNPcmdFullscreen will toggle the fullscreen mode of the active user window
	// and is the same as pressing 'ctrl_cmd' + F or 'esc' key
	// alternatively, kNPcmdFullscreenID will go fullscreen on a specific gl_context_id //zz implement

	// active user window context can be set separately from the active command context
	{ &data->ctrl.cmdFunc,		kNPcmdFuncPtr,	kNPcmdContext,		0, "np_cmd",	kNPcmdContext,	"context_next",		"i",	"osc",  0,	"/np/cmd/context_next",	"f" },
	{ &data->ctrl.cmdFunc,		kNPcmdFuncPtr,	kNPcmdContextID,	0, "np_cmd",	kNPcmdContextID, "context_id",		"i",	"osc",  0,	"",	"f" },
	{ &data->ctrl.cmdFunc,		kNPcmdFuncPtr,	kNPcmdUserFocus,	0, "np_cmd",	kNPcmdUserFocus, "user_focus",		"i",	"osc",  0,	"",	"f" },

//	{ &data->io.clear.a,		kNPfloat,		kNPposition,		0, "np_gl",		1,	"position_xy",		"f",	"osc",  0,	"/np/gl/1/background_a",	"f" },
//	{ &data->io.clear.a,		kNPfloat,		kNPscreen,			0, "np_gl",		1,	"screen_xy",		"fff",	"osc",  0,	"/np/gl/1/background_a",	"f" },

//	{ &data->io.clear.a,		kNPfloat,		kNPdimensions,		0, "np_gl",		1,	"dimensions",		"fff",	"osc",  0,	"/np/gl/1/background_a",	"f" },
//	{ &data->io.clear.a,		kNPfloat,		kNPdimensions,		0, "np_gl",		1,	"dimensions_height","f",	"osc",  0,	"/np/gl/1/dimensions_height",	"f" },
//	{ &data->io.clear.a,		kNPfloat,		kNPdimensions,		0, "np_gl",		1,	"dimensions_depth",	"f",	"osc",  0,	"/np/gl/1/dimensions_depth",	"f" },

	
	{ &rootGrid->scale.z,		kNPfloat,		kNPnodeRootGrid,	0, "np_node",	 0, "root_grid_scale_z", "f", "osc", 0, "/1/fader4", "f", "", "applies to entire scene" },

//	{ &data->io.gl.subsample,	kNPint,			kNPcmdSubsample,	0, "np_gl",		1, "subsample", "i", "osc", 0, "/1/fader5", "f", "", "applies to entire scene" },

//	{ &data->ctrl->cmdFuncList,	kNPint,			kNPcmdTrigger,		0, "np_cmd",	0, "select_all_on", "f", "osc", 0, "/1/toggle1", "f", "", "applies to entire scene" },
	{ &data->ctrl.cmdFunc,		kNPcmdFuncPtr,	kNPcmdTrigger,		0, "np_cmd",	kNPcmdSelectAll, "select_all", "i", "osc", 0, "/1/toggle4", "f", "", "applies to entire scene" },
	{ &data->ctrl.cmdFunc,		kNPcmdFuncPtr,	kNPcmdTrigger,		0, "np_cmd",	kNPcmdSelectAll, "select_all", "i", "osc", 0, "/2/toggle4", "f", "", "applies to entire scene" },
	{ &data->ctrl.cmdFunc,		kNPcmdFuncPtr,	kNPcmdTrigger,		0, "np_cmd",	kNPcmdSelectAll, "select_all", "i", "osc", 0, "/3/toggle4", "f", "", "applies to entire scene" },
	{ &data->ctrl.cmdFunc,		kNPcmdFuncPtr,	kNPcmdTrigger,		0, "np_cmd",	kNPcmdSelectAll, "select_all", "i", "osc", 0, "/4/toggle4", "f", "", "applies to entire scene" },
/*
	{ &data->ctrl.cmdFunc,		kNPcmdFuncPtr,	kNPcmdTrigger,		0, "np_cmd",	kNPcmdXincrease, "increase_x", "i", "osc", 0, "/2/push1", "f", "", "", "" },
	{ &data->ctrl.cmdFunc,		kNPcmdFuncPtr,	kNPcmdTrigger,		0, "np_cmd",	kNPcmdXdecrease, "decrease_x", "i", "osc", 0, "/2/push2", "f", "", "", "" },
	{ &data->ctrl.cmdFunc,		kNPcmdFuncPtr,	kNPcmdTrigger,		0, "np_cmd",	kNPcmdYincrease, "increase_y", "i", "osc", 0, "/2/push3", "f", "", "", "" },
	{ &data->ctrl.cmdFunc,		kNPcmdFuncPtr,	kNPcmdTrigger,		0, "np_cmd",	kNPcmdYdecrease, "decrease_y", "i", "osc", 0, "/2/push4", "f", "", "", "" },
	{ &data->ctrl.cmdFunc,		kNPcmdFuncPtr,	kNPcmdTrigger,		0, "np_cmd",	kNPcmdZincrease, "increase_z", "i", "osc", 0, "/2/push5", "f", "", "", "" },
	{ &data->ctrl.cmdFunc,		kNPcmdFuncPtr,	kNPcmdTrigger,		0, "np_cmd",	kNPcmdZdecrease, "decrease_z", "i", "osc", 0, "/2/push6", "f", "", "", "" },

	{ &data->ctrl.cmdFunc,		kNPcmdFuncPtr,	kNPcmdTrigger,		0, "np_cmd",	kNPcmdRotateLeft, "rotate_y_left",	"i", "osc", 0, "/2/push7", "f", "", "", "" },
	{ &data->ctrl.cmdFunc,		kNPcmdFuncPtr,	kNPcmdTrigger,		0, "np_cmd",	kNPcmdRotateRight,"rotate_y_right",	"i", "osc", 0, "/2/push8", "f", "", "", "" },
	{ &data->ctrl.cmdFunc,		kNPcmdFuncPtr,	kNPcmdTrigger,		0, "np_cmd",	kNPcmdRotateUp,	  "rotate_x_up",	"i", "osc", 0, "/2/push9", "f", "", "", "" },
	{ &data->ctrl.cmdFunc,		kNPcmdFuncPtr,	kNPcmdTrigger,		0, "np_cmd",	kNPcmdRotateDown, "rotate_x_down",	"i", "osc", 0, "/2/push10", "f", "", "", "" },
	{ &data->ctrl.cmdFunc,		kNPcmdFuncPtr,	kNPcmdTrigger,		0, "np_cmd",	kNPcmdRotateCCW,  "rotate_z_ccw",	"i", "osc", 0, "/2/push11", "f", "", "", "" },
	{ &data->ctrl.cmdFunc,		kNPcmdFuncPtr,	kNPcmdTrigger,		0, "np_cmd",	kNPcmdRotateCW,   "rotate_z_cw",	"i", "osc", 0, "/2/push12", "f", "", "", "" },
*/
	{ &data->ctrl.cmdFunc,		kNPcmdFuncPtr,	kNPcmdTrigger,		0, "np_cmd",	kNPcmdCamera,		"increase_x",	"i", "osc", 0, "/2/push1", "f", "", "", "" },
	{ &data->ctrl.cmdFunc,		kNPcmdFuncPtr,	kNPcmdTrigger,		0, "np_cmd",	kNPcmdSelectToggle, "decrease_x",	"i", "osc", 0, "/2/push2", "f", "", "", "" },
	{ &data->ctrl.cmdFunc,		kNPcmdFuncPtr,	kNPcmdTrigger,		0, "np_cmd",	kNPcmdGrid,			"increase_y",	"i", "osc", 0, "/2/push3", "f", "", "", "" },
	{ &data->ctrl.cmdFunc,		kNPcmdFuncPtr,	kNPcmdTrigger,		0, "np_cmd",	kNPcmdPin,			"decrease_y",	"i", "osc", 0, "/2/push4", "f", "", "", "" },

	{ &data->ctrl.cmdFunc,		kNPcmdFuncPtr,	kNPcmdTrigger,		0, "np_cmd",	kNPcmdColorDown,	"increase_z",	"i", "osc", 0, "/2/push5", "f", "", "", "" },
	{ &data->ctrl.cmdFunc,		kNPcmdFuncPtr,	kNPcmdTrigger,		0, "np_cmd",	kNPcmdColorUp,		"decrease_z",	"i", "osc", 0, "/2/push6", "f", "", "", "" },
//	{ &data->ctrl.cmdFunc,		kNPcmdFuncPtr,	kNPcmdTrigger,		0, "np_cmd",	kNPcmdAltColor,		"select_axes",	"i", "osc", 0, "/2/push7", "f", "", "", "" },
	{ &data->ctrl.cmdFunc,		kNPcmdFuncPtr,	kNPcmdTrigger,		0, "np_cmd",	kNPcmdFullscreen,	"fullscreen",	"i","osc",  0, "/2/push7", "f", "", "", "" },
	{ &data->ctrl.cmdFunc,		kNPcmdFuncPtr,	kNPcmdTrigger,		0, "np_cmd",	kNPcmdTexture,		"rotate_y_right","i","osc", 0, "/2/push8", "f", "", "", "" },
	
	{ &data->ctrl.cmdFunc,		kNPcmdFuncPtr,	kNPcmdTrigger,		0, "np_cmd",	kNPcmdAlphaDown,	"rotate_x_up",	"i", "osc", 0, "/2/push9", "f", "", "", "" },
	{ &data->ctrl.cmdFunc,		kNPcmdFuncPtr,	kNPcmdTrigger,		0, "np_cmd",	kNPcmdAlphaMode,	"rotate_x_down","i", "osc", 0, "/2/push10", "f", "", "", "" },
	{ &data->ctrl.cmdFunc,		kNPcmdFuncPtr,	kNPcmdTrigger,		0, "np_cmd",	kNPcmdHide,			"rotate_z_ccw",	"i", "osc", 0, "/2/push11", "f", "", "", "" },
	{ &data->ctrl.cmdFunc,		kNPcmdFuncPtr,	kNPcmdTrigger,		0, "np_cmd",	kNPcmdAlphaUp,		"rotate_z_cw",	"i", "osc", 0, "/2/push12", "f", "", "", "" },

	{ &data->ctrl.cmdFunc,		kNPcmdFuncPtr,	kNPcmdTrigger,		0, "np_cmd",	kNPcmdNew,			"new",			"i", "osc", 0, "/2/push13", "f", "", "", "" },
	{ &data->ctrl.cmdFunc,		kNPcmdFuncPtr,	kNPcmdTrigger,		0, "np_cmd",	kNPcmdTopo,			"topo",			"i", "osc", 0, "/2/push14", "f", "", "", "" },
	{ &data->ctrl.cmdFunc,		kNPcmdFuncPtr,	kNPcmdTrigger,		0, "np_cmd",	kNPcmdGeometry,		"geometry",		"i", "osc", 0, "/2/push15", "f", "", "", "" },
	{ &data->ctrl.cmdFunc,		kNPcmdFuncPtr,	kNPcmdTrigger,		0, "np_cmd",	kNPcmdDelete,		"delete",		"i", "osc", 0, "/2/push16", "f", "", "", "" },
//	{ &data->ctrl.cmdFunc,		kNPcmdFuncPtr,	kNPcmdTrigger,		0, "np_cmd",	kNPcmdRotateCW,		"rotate_z_cw",	"i", "osc", 0, "/2/push16", "f", "", "", "" },

	{ &data->ctrl.cmdFunc,		kNPcmdFuncPtr,	kNPcmdTrigger,		0, "np_cmd",	kNPcmdZoomOn,		"zoom_in",		"i", "osc", 0, "/2/push16", "f", "", "" },
//	{ &data->ctrl.cmdFunc,		kNPint,			kNPcmdTrigger,		0, "np_cmd",	kNPcmdZoomOut,		"rotate_z_cw",	"i", "osc", 0, "/2/push12", "f", "", "" },


	//kNPmouseDown "ii"
	//kNPmouseUp
	//kNPmouseMove

	//color, texture, transparency -,+
	//topo, geo, 

	// example of mapping osc directly to np and then mapping 3rd party 'osc' to native osc 'osc_np'
	// osc - osc maps can be dynamically created and routed without passing through np...
	// if an 'osc_np' address entry is used then it can be dynamically mapped directly to an internal c-struct parameter...
	// if both osc - osc mapping pair addresses are unknown then the data can be routed RX->TX and auto-referenced by the internal global c-structure

	{ data->io.url,				kNPcstrPtr,		kNPgBrowserURL,		0, "np_browser", 1,	"url",				 "s",	"osc_np",0,	"/np/browser/1/url", "s" },
	// above is native translation of Globals...
	// below is 3rd party OSC schema
	{ data->io.url,				kNPcstrPtr,		kNPgBrowserURL,		0, "np_browser", 1,	"/np/browser/1/url", "s",	"osc",	 0,	"/mrmr/textinput/7/z-zs-iPhone", "s" },
	{ data->io.url,				kNPcstrPtr,		kNPgBrowserURL,		0, "osc_np",	 1,	"/np/browser/1/url", "s",	"osc",	 0,	"/mrmr/textinput/7",			"s" },

	//	data struct ptr			elementTypeID		type/tag		a_map_path	id	element					mapPathA,  id	element					type_tag
//	{ &data->io.gl.position,	kNPposition,		kNPfloatXY,		0, "np_gl",		1,	"position_xy",		"ii",	"osc",  1,	"/np/gl/1/position_xy",		"ii" },
//	{ &data->io.gl,screen,		kNPscreen,			kNPfloatXY,		0, "np_gl",		1,	"screen_size_xy",	"ii",	"csv",	1,	"screen_size_xy",			"ii" },

	{ &data->io.url,			kNPgBrowserURL,		kNPcstrPtr,		0, "np_globals", 1,	"browser_url",	 "s",	"osc",  1,	"/np/io/browser_url",		"s" },
	// above is a copy of the mapGlobals... 
	//zz - update to auto-generate (learn) new OSC addresses spaces, dynamic node parsing...

	
	{ &data->io.clear,			kNPfloatRGBA,	kNPgBackground,		0, "np_gl",	1,	"back_color_rgba",	 "ffff",	"osc",  0,	"/3/xy",					"ff" },
	{ &data->io.clear.r,		kNPfloat,		kNPgBackgroundR,	0, "np_gl",	1,	"back_color_r",			"f",	"osc",  0,	"/1/fader1",				"f" },
	{ &data->io.clear.g,		kNPfloat,		kNPgBackgroundG,	0, "np_gl",	1,	"back_color_g",			"f",	"osc",  0,	"/1/fader2",				"f" },
	{ &data->io.clear.b,		kNPfloat,		kNPgBackgroundB,	0, "np_gl",	1,	"back_color_b",			"f",	"osc",  0,	"/1/fader3",				"f" },
//	{ &data->io.clear.a,		kNPfloat,		kNPgBackgroundA,	0, "np_gl",	1,	"back_color_a",			"f",	"osc",  0,	"/1/fader4",				"f" },

/*
	{ &data->,		kNPfloat,		kNP,	0, "np_",	1,	"",			"f",	"osc",  0,	"/COG/NEUTRAL",				"f" },
	{ &data->,		kNPfloat,		kNP,	0, "np_",	1,	"",			"f",	"osc",  0,	"/COG/PUSH",				"f" },
	{ &data->,		kNPfloat,		kNP,	0, "np_",	1,	"",			"f",	"osc",  0,	"/COG/PULL",				"f" },
	{ &data->,		kNPfloat,		kNP,	0, "np_",	1,	"",			"f",	"osc",  0,	"/COG/LIFT",				"f" },
	{ &data->,		kNPfloat,		kNP,	0, "np_",	1,	"",			"f",	"osc",  0,	"/COG/DROP",				"f" },
	{ &data->,		kNPfloat,		kNP,	0, "np_",	1,	"",			"f",	"osc",  0,	"/COG/LEFT",				"f" },
	{ &data->,		kNPfloat,		kNP,	0, "np_",	1,	"",			"f",	"osc",  0,	"/COG/RIGHT",				"f" },
	{ &data->,		kNPfloat,		kNP,	0, "np_",	1,	"",			"f",	"osc",  0,	"/COG/ROTATE_LEFT",			"f" },
	{ &data->,		kNPfloat,		kNP,	0, "np_",	1,	"",			"f",	"osc",  0,	"/COG/ROTATE_RIGHT",		"f" },
	{ &data->,		kNPfloat,		kNP,	0, "np_",	1,	"",			"f",	"osc",  0,	"/COG/ROTATE_CLOCKWISE",	"f" },
	{ &data->,		kNPfloat,		kNP,	0, "np_",	1,	"",			"f",	"osc",  0,	"/COG/ROTATE_COUNTER_CLOCKWISE", "f" },
	{ &data->,		kNPfloat,		kNP,	0, "np_",	1,	"",			"f",	"osc",  0,	"/COG/ROTATE_FORWARDS",		"f" },
	{ &data->,		kNPfloat,		kNP,	0, "np_",	1,	"",			"f",	"osc",  0,	"/COG/ROTATE_REVERSE",		"f" },
	{ &data->,		kNPfloat,		kNP,	0, "np_",	1,	"",			"f",	"osc",  0,	"/COG/DISAPPEAR",			"f" },
*/
	{ &data->io.clear.b,		kNPfloat,		kNPgBackgroundB,	0, "np_gl",	1,	"back_color_b",			"f",	"osc",  0,	"/AFF/Frustration",			"f" },
	{ &data->io.clear.b,		kNPfloat,		kNPgBackgroundB,	0, "np_gl",	1,	"back_color_b",			"f",	"osc",  0,	"/AFF/Engaged/Bored",		"f" },
	{ &data->io.clear.g,		kNPfloat,		kNPgBackgroundG,	0, "np_gl",	1,	"back_color_g",			"f",	"osc",  0,	"/AFF/Excitement",			"f" },
	{ &data->io.clear.b,		kNPfloat,		kNPgBackgroundB,	0, "np_gl",	1,	"back_color_b",			"f",	"osc",  0,	"/AFF/Excitement Long Term","f" },
	{ &data->io.clear.r,		kNPfloat,		kNPgBackgroundR,	0, "np_gl",	1,	"back_color_r",			"f",	"osc",  0,	"/AFF/Meditation",			"f" },

	{ &data->io.clear.r,		kNPfloat,		kNPgBackgroundB,	0, "np_gl",	1,	"back_color_r",			"f",	"osc",  0,	"/EXP/WINK_LEFT",			"f" },
	{ &data->io.clear.g,		kNPfloat,		kNPgBackgroundB,	0, "np_gl",	1,	"back_color_g",			"f",	"osc",  0,	"/EXP/WINK_RIGHT",			"f" },
	{ &data->io.clear.b,		kNPfloat,		kNPgBackgroundB,	0, "np_gl",	1,	"back_color_b",			"f",	"osc",  0,	"/EXP/BLINK",				"f" },
	{ &data->io.clear.r,		kNPfloat,		kNPgBackgroundB,	0, "np_gl",	1,	"back_color_r",			"f",	"osc",  0,	"/EXP/LEFT_LID",			"f" },
	{ &data->io.clear.g,		kNPfloat,		kNPgBackgroundB,	0, "np_gl",	1,	"back_color_g",			"f",	"osc",  0,	"/EXP/RIGHT_LID",			"f" },
	{ &data->io.clear.r,		kNPfloat,		kNPgBackgroundR,	0, "np_gl",	1,	"back_color_r",			"f",	"osc",  0,	"/EXP/HORIEYE",				"f" },
	{ &data->io.clear.g,		kNPfloat,		kNPgBackgroundG,	0, "np_gl",	1,	"back_color_g",			"f",	"osc",  0,	"/EXP/VERTEYE",				"f" },
	{ &data->io.clear.b,		kNPfloat,		kNPgBackgroundB,	0, "np_gl",	1,	"back_color_b",			"f",	"osc",  0,	"/EXP/SMILE",				"f" },
	{ &data->io.clear.r,		kNPfloat,		kNPgBackgroundB,	0, "np_gl",	1,	"back_color_r",			"f",	"osc",  0,	"/EXP/CLENCH",				"f" },
	{ &data->io.clear.g,		kNPfloat,		kNPgBackgroundB,	0, "np_gl",	1,	"back_color_g",			"f",	"osc",  0,	"/EXP/LAUGH",				"f" },
	{ &data->io.clear.b,		kNPfloat,		kNPgBackgroundB,	0, "np_gl",	1,	"back_color_b",			"f",	"osc",  0,	"/EXP/SMIRK_LEFT",			"f" },
	{ &data->io.clear.r,		kNPfloat,		kNPgBackgroundB,	0, "np_gl",	1,	"back_color_r",			"f",	"osc",  0,	"/EXP/SMIRK_RIGHT",			"f" },
	{ &data->io.clear.g,		kNPfloat,		kNPgBackgroundB,	0, "np_gl",	1,	"back_color_g",			"f",	"osc",  0,	"/EXP/FURROW",				"f" },
	{ &data->io.clear.b,		kNPfloat,		kNPgBackgroundB,	0, "np_gl",	1,	"back_color_b",			"f",	"osc",  0,	"/EXP/EYEBROW",				"f" },

	{ &data->map.oscCount,		kNPint,			kNPitemCount,	    1, "osc",	0,	"item_count",			"i",	"osc",  0,	"/np/map/osc/item_count",	"i", "Map item count", "designates end of list" }
	
	}; //end of list

	//add pointer element to the map descriptor
	NPmapType mapTypeList[] = {
	{ 0,	kNPmapNull,		"null",			"" },
	{ 0,	kNPmapGlobals,	"globals",		"Accesses global map elements" },
	{ 0,	kNPmapNode,		"node",			"Scene Graph tree Node element" },
	{ 0,	kNPmapTag,		"tag",			"Text Tag" },
	{ 0,	kNPmapChannel,	"channel",		"Channel maps tracks to nodes, globals & commands" },
	{ 0,	kNPmapTrack,	"track",		"Tracks animate the scene using files or live IO" },
	{ 0,	kNPmapGL,		"gl",			"OpenGL context specific parameters" },
	{ 0,	kNPmapCSV,		"csv",			"CSV table descriptor" },
	{ 0,	kNPmapJSON,		"json",			"JSON data descriptor" },
	{ 0,	kNPmapCount,	"count",		"Item count for this list" }
	};

	//scan the list looking for kNPitemCount to dynamically set the list size
	i = 0; while (mapTypeList[i++].type != kNPmapCount) data->map.mapTypeCount++;

	// copy the structure to newly allocated RAM	
	size = sizeof(mapTypeList);
	data->map.mapTypeList = npMalloc(0, size, data);
	memcpy(data->map.mapTypeList, (const void*)mapTypeList, size);

	//scan the list looking for kNPitemCount to dynamically set the list size	//zz debug probably a simpler way to do this?
	i = 0; while (mapGlobals[i++].elementID != kNPitemCount) data->map.globalsCount++;

	// copy the structure to newly allocated RAM								 //zz debug , perhaps a way to avoid this too....
	size = sizeof(mapGlobals);
	data->map.typeMapGlobals = npMalloc(0, size, data);
	memcpy(data->map.typeMapGlobals, (const void*)mapGlobals, size);

	// REPEAT for mapOSC
	//scan the list looking for kNPcount to dynamically get the size
	i = 0; while (mapOSC[i++].elementID != kNPitemCount) data->map.oscCount++;

	// copy the structure to newly allocated RAM, proably a better way, zz
	size = sizeof(mapOSC);
	data->map.typeMapOSC = npMalloc(0, size, data);
	memcpy(data->map.typeMapOSC, (const void*)mapOSC, size);

	//-------------
	// TEST MAP		//zz debug
//	printf("\n TEST GLOBALS alphaMode: %d\n\n", *(int*)mapGlobals[1].mapPtr);
	printf("\nGlobals Map itemCount: %d\n", data->map.globalsCount);
//	printf(" Globals Map size: %d\n", sizeof(mapGlobals));
//	printf(" TEST MAP id: %d\n", npMapStrToID("full_screen\0",data));
//	printf(" TEST MAP value: %d\n\n", *(int*)npMapStrToPtr ("full_screen\0", data));	

//	printf("\n TEST GLOBALS alphaMode: %d\n\n", *(int*)mapGlobals[1].mapPtr);
	printf("OSC Map itemCount: %d\n", data->map.oscCount);
//	printf(" TEST MAP size: %d\n", sizeof(mapOSC));
//	printf(" TEST MAP id: %d\n", npMapAddressToPtr("/1/fader4\0",data));
//	printf(" TEST MAP value: %f\n\n", *(float*)npMapAddressToPtr ("/1/fader4\0", data));

	//this should change the background color 'b' blue component
//	*(float*)npMapAddressToPtr ("/1/fader1\0", data) = 0.6f;
//	*(float*)npMapAddressToPtr ("/1/fader2\0", data) = 0.4f;
//	*(float*)npMapAddressToPtr ("/1/fader3\0", data) = 0.6f;

//	*(float*)npMapAddressToMapTypeItem ("/1/fader1\0", data)->mapPtr = 0.6f;
//	*(float*)npMapAddressToMapTypeItem ("/1/fader2\0", data)->mapPtr = 0.4f;
//	*(float*)npMapAddressToMapTypeItem ("/1/fader3\0", data)->mapPtr = 0.6f;
}

int npMapToCSV (char* csvStr, int mapType, int size, int* index, void* dataRef);
//-----------------------------------------------------------------------------
int npMapToCSV (char* csvStr, int mapType, int size, int* index, void* dataRef)
{
	int id = 1;				//first id should start at 1
	int itemCount = 0;

	int n = 0;				//curs byte count offset
	char* curs = csvStr;

	char str[4096];			//temp memory for value string, optimize this out, //zz debug			

	int i = 0, j = 0;		//node tag sort stuff
	int count = 0;
	void** nodes;
					
	pNPmapLink mapList = NULL;
	pNPmapLink mapItem = NULL;

	pNPnode node = NULL;
	pNPtag tag = NULL;

	pData data = (pData) dataRef;

//zz-JJ		itemCount = data->map.globalsCount;

	if ( mapType == kNPmapOSC )
	{
		// write the first row header, includes CRLF line ending
		// ---
		// 'item_id' = record_id for nodes or window_id for GL window context... etc.
		//
		// 'map_id' = maps the data location... file, db table, list ptr
		// map_path connects MySQL, URL, OSC, files...
		// table names such as 'globals', 'node', 'gl', 'track', 'cmd'
		// custom table names... node_tbl_5... or MyCustomTableName...
		//example: 1,node,42,translate_xyz,/np/node/42/translate_xyz
		// ---
		mapList = data->map.typeMapOSC;
		itemCount = data->map.oscCount;

		// write first row header first field as table_name_id based on mapType
	//	includes CRLF line ending
		n += sprintf (curs, "np_osc_id");	//npGetMapName(mapType, dataRef); //zz debug
	}
	else if ( mapType == kNPmapGlobals )
	{
		mapList = data->map.typeMapGlobals;
		itemCount = data->map.globalsCount;
	
		n += sprintf (curs, "np_globals_id");
	}
	else if ( mapType == kNPmapTag )
	{
		//mapList = data->map.typeMapTag;
		itemCount = kNPnodeRootPin;	//data->map.tagsCount;					//zz debug
		
		//allocate a pair of destination buffers for the nodes and tags
		nodes = npMalloc(0, sizeof(pNPnode) * kNPnodeMax, data);			//data->map.nodeCount
		if (!nodes) return 0;

		//populate the sort lists, skip over null nodes
		for (i = kNPnodeRootPin ; i < kNPnodeMax; i++)	//data->map.nodeCount; i++)
			if (data->map.nodeID[i] != NULL)
				nodes[j++] = data->map.nodeID[i];

#define kNPtagWriteLimit 110000			//zz debug temp workaround, need file block procedure
		if (j > kNPtagWriteLimit )
			j = kNPtagWriteLimit;		//zz debug kNPtagWriteLimit

		//sort nodes and tags grouped first by table_id and second by record_id
	//	npSort(nodes, j, sizeof(void*), npCompareNodes, data);

		//zz debug change to np_tag_id,map_path,record_id,title,description
		n += sprintf (curs, "np_tag_id,record_id,table_id,title,description\n");
		//for (id = 1; id <= itemCount; id++)
		for (i=0; i < j; i++)
		{
			node = nodes[i];
		//	node = data->map.node[id];
			if ( node )
			{
				tag = node->tag;
				if( tag && node->recordID ) //zz debug, test w/o '&& node->recordID' to save blank tags
				{	
					n += sprintf ((curs + n), "%d,%d,%d,\"%s\",\"%s\"\n",	//zz optimize with npitoa...
					id++,
					node->recordID,
					node->tableID,
					tag->title,
					tag->desc
					);
				}
			}
		}

		// cleanup					//zz debug, add handling for &index with file blocks 
		npFree( nodes, data );
		return n;
	}
	else
	{
		npPostMsg("err 4545 - unknown mapType", kNPmsgErr, data);
		return 0;
	}

	//append rest of the header row
	n += sprintf ((curs + n), ",map_path,item_id,element,type,permisions" );

	n += sprintf ((curs + n), ",name,desc,value\n" );

	for (id = 1; id <= itemCount; id++)
	{
		mapItem = &mapList[id];

		n += sprintf ((curs + n), "%d,\"%s\",%d,\"%s\",\"%s\",%d,\"%s\",\"%s\",\"%s\"\n",	//zz optimize with npitoa...
					id,

					mapItem->mapPathA,		//"node"	//mapItem->baseType	//zz-JJ this will be added to array. mapItem->baseType
					mapItem->itemA,			//list array index or 0 for single elementA
					mapItem->elementA,
					mapItem->typeTagA,
					mapItem->permissions,	//permissions +0 for full duplex, +1 = A to B, -1 = B to A
	
					mapItem->name,
					mapItem->desc,

					npGetValueStr(str, mapItem, data)	//formats parameter values as csv 
				 );
	}
/*	}
	else if ( mapType == kNPmapGlobals )
	{
	//		npMapTypeInit(data);
		mapTypePtr = data->map.typeMapGlobals;
		itemCount = data->map.globalsCount;

		//write the first row header, includes CRLF line ending
		n += sprintf (curs, "np_globals_id,a_map_path,a_item_id,a_element,a_type_id,b_map_path,b_item_id,b_element,b_type_tag,init_value,title,desc\n");
		
		//example: 1,gl,1,clear_rgba,"1.0,0.5,0.5,1.0","background color RGBA"
	
		//loop and write out all globals in the mapType[] list
		//populates the rows using the globals map descriptor array 
		for (id = 1; id <= itemCount; id++)
		{
	//zz-JJ		mapItem = &data->map.typeMapGlobals[id];
			mapItem = &mapTypePtr[id];

			n += sprintf ((curs + n), "%d,%s,%d,\"%s\",%s,%s,%d,\"%s\",%s,\"%s\",\"%s\",\"%s\"\n",	//zz optimize with npitoa...
						id,
						mapItem->mapPathA,	//"node"	//mapItem->baseType	//zz-JJ this will be added to array. mapItem->baseType
						mapItem->itemA,		//list index or 0 for single elementA
						mapItem->elementA,
						mapItem->typeTagA,		//this is a type by 'ID' rather then type tag by 'NAME'

						mapItem->mapPathB,
						mapItem->itemB,
						mapItem->elementB,
						mapItem->typeTagB,	//this one is by name

						npGetValueStr(str, mapItem, data),	//format value str

						"",					//title
						""					//desc
					 );
		}
	}
*/
	return n;
}

int npGetFormat( const char* file, const char* curs, int size, void* dataRef );
//-----------------------------------------------------------------------------
int npGetFormat( const char* file, const char* curs, int size, void* dataRef )
{
	int mapType = 0;
	//if *.csv then check for comma, line returns, etc... make sure it really is a csv...

	return kNPformatCSV;

	if (strncmp(curs, "map_id", 6) == 0)		// native CSV format
		mapType = kNPmapCSV;

	else if (strncmp(curs, "osc_id", 6) == 0)	//3rd party OSC handler
		mapType = kNPformatOSC;
	else if (strncmp(curs, "json_id", 6) == 0)
		mapType = kNPformatJSON;
	else if (strncmp(curs, "snmp_id", 6) == 0)
		mapType = kNPformatSNMP;
	else if (strncmp(curs, "xml_id", 6) == 0)
		mapType = kNPformatXML;
	else if (strncmp(curs, "kml_id", 6) == 0)
		mapType = kNPformatKML;

	//	default : break; //run heuristics on content samples...
	// try and figure out the data type, compare against a list of known types
	// using a folder/path(s) to a bunch of sample data file/types

	return mapType;
}

//non-native (alien) universal format translator
int npMapAuto( int format, const char* curs, int size, void* dataRef );
//------------------------------------------------------------------------------
int npMapAuto( int format, const char* curs, int size, void* dataRef )
{
	int result = 0;

	return result;		//zz debug, add 3rd party support CSV, JSON, DB...
}


//identifies map format based on first field text, native types are np_....._id
int npGetMapType( int format, const char* curs, int size, void* dataRef );
//------------------------------------------------------------------------------
int npGetMapType( int format, const char* curs, int size, void* dataRef )
{
	int mapType = 0;
	char msg[128];

	if( strncmp( curs, "np_", 3 ) != 0 )
		return npMapAuto(format, curs, size, dataRef );		//dispatch non-native format

	//advance cursor to compare next str component
	curs += 3;

	if (strncmp(curs, "_id", 3) == 0)
		mapType = kNPmapNP;				// "np_id" default handler, agnostic
	else if (strncmp(curs, "globals_id", 10) == 0)		//globals map
		mapType = kNPmapGlobals;
	else if (strncmp(curs, "node_id", 7) == 0)			//node map
		mapType = kNPmapNode;						
	else if (strncmp(curs, "tag_id", 6) == 0)			//tag map
		mapType = kNPmapTag;
	else if (strncmp(curs, "map_id", 6) == 0)		//describes other maps... native CSV format
		mapType = kNPmapCSV;
	else if (strncmp(curs, "osc_id", 6) == 0)		//3rd party OSC handler
		mapType = kNPmapOSC;
	else if (strncmp(curs, "json_id", 7) == 0)
		mapType = kNPmapJSON;
	else if (strncmp(curs, "snmp_id", 7) == 0)
		mapType = kNPmapSNMP;
	else if (strncmp(curs, "kml_id", 6) == 0)
		mapType = kNPmapKML;
	else if (strncmp(curs, "xml_id", 6) == 0)
		mapType = kNPmapXML;
	else
	{
		//zz, could attempt other detection methods and/or generic automapper
		sprintf(msg,"err 7822 - unknown native mapType: %.20s...",curs);
		npPostMsg(msg, kNPmsgErr, dataRef);
	}

	return mapType;
}

void* npMapItemPtr (char* mapPath, char* item, char* element, char* typeTag, void* dataRef);
void* npMapItemPtr (char* mapPath, char* item, char* element, char* typeTag, void* dataRef)
{
	return npMapElementToPtr (element, dataRef);
}

void npUpdateGlobals( void* dataRef );
// update global states at startup and after loading datasets with globals
//-----------------------------------------------------------------------------
void npUpdateGlobals( void* dataRef )
{
	pData data = (pData) dataRef;

	pNPgl gl = &data->io.gl;

	//don't sync if this is the startup load, gl context does NOT yet exist!
	if ( data->ctrl.startup )
		return;

	printf( "full: %d  pos_x: %d  pos_y: %d  size_x: %d  size_y: %d\n",
			gl->fullscreen,
			gl->position.x,
			gl->position.y,
			gl->windowSize.x,
			gl->windowSize.y );

	//queries current window mode //Removed GLUT_FULL_SCREEN replaced with 0x01FF
	if ( glutGet( 0x01FF ) != gl->fullscreen )
	{
	//	gl->fullscreen = 1 - gl->fullscreen;
		npCtrlCommand( kNPcmdFullscreen, data ); // temp, lde
	}
	
	if ( glutGet( GLUT_WINDOW_X ) != gl->position.x 
		|| glutGet( GLUT_WINDOW_Y ) != gl->position.y
		|| glutGet( GLUT_WINDOW_WIDTH ) != gl->windowSize.x
		|| glutGet( GLUT_WINDOW_HEIGHT ) != gl->windowSize.y )
	{

		gl->width = gl->windowSize.x;
		gl->height = gl->windowSize.y;

		npGLResizeScene(gl->windowSize.x, gl->windowSize.y);
			
		gl->width = gl->windowSize.x;
		gl->height = gl->windowSize.y;
		glutReshapeWindow (gl->windowSize.x, gl->windowSize.y);
		glutPositionWindow (gl->position.x, gl->position.y);
	}


/*				
	if ( glutGet( GLUT_WINDOW_WIDTH ) != data->io.gl.width
		 || glutGet( GLUT_WINDOW_HEIGHT ) != data->io.gl.height )
		npGLResizeScene( data->io.gl.width, data->io.gl.height );
*/

/*
	// if needed set fullscreen mode, size and position can be ignored
	if ( data->io.gl.fullscreen != npGetFullscreenMode(data) )
		npCtrlCommand( kNPcmdFullscreen, data );
	else
	{
		// set window size and position, resizing also sets position
		if ( data->io.gl.windowSize != npGetWindowSize(data) )
			npResizeWindow(data);
		else if ( data->io.gl.position != npGetWindowPosition(data) )
			npMoveWindow(data);
	}
*/
}

/// @todo add parallel (threaded) file and database parsing routines
// file parsing tends to be CPU intensive, be mindful of performance
// the parsing routines are optimized for native antz structures
// max bandwidth is achieved through large block read/write such as 4MB or 8MB
// currently some systems perform well up to 12MB block size, others slow down
// high IOPS can only be achieved with good flow control and efficient parsing
//-----------------------------------------------------------------------------
int npOpenMapCSV (char* filePath, int mapType, void* dataRef)
{
	int mapFormat = kNPformatNull;		//kNPformatCSV
//	int mapType = kNPmapNull;					//zz debug, reconcile different definitions of mapType

	int result = 0;
	int eof = 0;
	int endPoint = 0;

	FILE* file = NULL;
	size_t count = 0;
	size_t size = 0;
	int i = 0;
	int byteCount = 0;
	char* bufferIndex = 0;
	char* buffer = NULL;		//file read buffer
	char* curs = NULL;

	char*	id			= NULL;	//record ID
	char*	permissions	= NULL;

	char*	mapPathA	= NULL;
	char*	itemA		= NULL;		//typically an integer
	char*	typeTagA	= NULL;
	char*	elementA	= NULL;

	char*	mapPathB	= NULL;
	char*	itemB		= NULL;
	char*	typeTagB	= NULL;
	char*	elementB	= NULL;

	char*	name		= NULL;
	char*	desc		= NULL;
	char*	valueStr	= NULL;

	void*	elementRef	= NULL;
	int		subElement = 0;

	char msg[512] = {'\0'};

	pData data = (pData) dataRef;


	// allocate read buffer
	buffer = npMalloc( 0, kNPfileBlockSize, data); if( !buffer )return 0;

	// open file in read only mode "r"
	file = npFileOpen( filePath, "r", dataRef ); if( !file ) goto finish;

	// read file then immediately close file to minimize file-open-lock time
	size = npFileRead( buffer, 1, kNPfileBlockSize, file, dataRef );
	
	// close file only if all contents have been read
	if (size < kNPfileBlockSize)
		npFileClose( file, dataRef );
			
	// exit if file is empty, otherwise process it in chunks
	if( !size ) goto finish;
		
//	printf( "Bytes Read: %d", size );

	//find the first field by skipping over any white space at beginning of file
	curs = npSkipWhiteSpace (buffer, size);
	if (!curs)
	{
		npPostMsg("err 8311 - file buffer is blank", kNPmsgErr, data);
		goto finish;
	}

	//id the table format and data type, native tables start with np_...._id
	mapFormat = npGetFormat( filePath, curs, size, data );
	mapType = npGetMapType( mapFormat, curs, size, data );
		
// npLoadMapFile (buffer, wordSize, count, dataRef);
//------------------------------------------------------------------------------
//zz tokenize fields into ptr structure and convert quotes " to cstring '/0'
/*
//	mapItemFuncPtr = npGetMapItemFuncPtr( mapFormat, mapType, data );

	void* mapItemFuncPtr = NULL;

	switch( mapFormat )
	{	case kNPformatCSV : 
			switch( mapType )
			{	case kNPmapGlobals : mapItemFuncPtr = npMapItemFromCSV(); break;
				case kNPmapNode : mapItemFuncPtr = npMapNodeFromCSV(); break;
				case kNPmapTag : mapItemFuncPtr = npMapTagFromCSV(); break;
				case kNPmapOSC : mapItemFuncPtr = npMapOscFromCSV(); break;
				default : mapItemFuncPtr = npMapItemFromCSV(); break;
			}
		case kNPformatDB : break;		//MySQL formatted
		case kNPformatOSC : break;	//this is an actual OSC binary stream...
		case kNPformatJSON : break;
		case kNPformatSNMP : break;
		case kNPformatXML : break;
		case kNPformatKML : break;
	}
*/
	//now ready to parse data
	//start processing items (CSV/table rows)

	//find the chunk endpoint of the last record	//zz debug, add handling for record size > chunk size
	endPoint = npLastEOL( buffer, size );
	if (!endPoint){ printf(" err 8312 - no EOL in file chunk"); goto finish;}

/* //zz
		// set read ptr to beginning of first data row
	
		//zz debug, replace below with comma and slash aware getMapItemPtr() 
		// directly STREAM parse csv/json/osc... different separators ','',''',""
		//
		// tokenizing converts the comma to a NULL-terminated cstr
		// stream parsing has to detect various separators such as ""
		// both methods avoid the need to copy the string
		i=0; while( curs[i] != ',' ) i++;			//find the next comma

		if (i>0) curs[i-1] = '\0'; curs++;			// i=0 could cause bad ptr 
													// i-1 to remove ending quote ...",

		//then search the globals mapType and get the element data ptr
		intPtr = (int*)npMapItemPtr (NULL, NULL, curs, NULL, data);
*/	
	//goto start of first record, typically starts with its id
	count += i = npNextLineLimit(curs, endPoint - count); curs = &curs[i];

	while( endPoint > (int)count )
	{
		id = curs;															//id

		count += i = npNextField(curs); mapPathA	= curs = &curs[i];	//array or element ptr
		count += i = npNextField(curs); itemA		= curs = &curs[i];	//array index int, 0 if element such as np_globals

		count += i = npNextField(curs); elementA	= curs = &curs[i];	//element ptr
		count += i = npNextField(curs); typeTagA	= curs = &curs[i];	//type_tag_a descriptor
		curs -= 2; *curs = '\0'; curs+=2; 	//zz debug, i-2 to convert ending quote ...", to cstr for element ptr

		count += i = npNextField(curs); permissions = curs = &curs[i];	//direction

		count += i = npNextField(curs); name		= curs = &curs[i];	//UI name
		count += i = npNextField(curs); desc		= curs = &curs[i];	//UI description

		count += i = npNextField(curs); valueStr	= curs = &curs[i];	//value

		//seek next record
		count += i = npNextLineLimit( curs, endPoint - count ); curs = &curs[i];

		//set the element value ptr, (at this time) type tags must match
		//zz add custom type transcoding and a generic defualt handler
		if(0)//strcmp( typeTagA, typeTagB ) )
		{
			printf(" warn 8323 - type tags do not match, id: %d\n", id);
			// npAutoTranslate();
		}
		else
		{
		//switch (typeTagA)
		//     case 'ii'
		//			switch (typeTagB)
			elementA++; typeTagA++; valueStr++;
				
			//printf("valueStr : %.32s", elementA, valueStr);
	
			elementRef = npMapItemPtr (NULL, NULL, elementA, NULL, data);
			if( !elementRef )
			{
					printf(" - unknown element - warn 9458");	 //zz debug add a default handler
					// return 1;	//0;	`````````````````````````````````````````//zz debug
			}
			else if( strncmp( typeTagA, "i", 1 ) == 0 ) 
			{
				while( *typeTagA++ == 'i' ) 
				{	
					*(int*)elementRef = npatoi(valueStr);
					//	printf ("subElement value: %d\n", *(int*)elementRef );
					printf (" %d", *(int*)elementRef );

					if (*typeTagA == 'i')	//goto next field if more then one element
					{
						(int*)elementRef += 1;
						i = npNextField(valueStr); valueStr = &valueStr[i];
					}
				}
			}
			else if( strncmp( typeTagA, "f", 1 ) == 0 ) 
			{
				while( *typeTagA++ == 'f' ) //loop processes each type tag
				{	
					*(float*)elementRef = npatof(valueStr);
					printf (" %f", *(float*)elementRef );
					
					if (*typeTagA == 'f')	//goto next field if more then one element
					{
						(float*)elementRef += 1;
						i = npNextField( valueStr ); valueStr = &valueStr[i];	//zz debug, what if no more fields... mem violation?
					}
				}
			}
			else if( strncmp( typeTagA, "s", 1 ) == 0 ) 
			{
				while( *typeTagA++ == 's' ) //loop processes each type tag
				{	
					//zz debug, how do we know if a char* or allocated char[array] ??? currently  arrays ONLY!!!!
					//need to (re)allocate when assigning to ptr
					
					//npCSVtoCstr (valueStr);
					i=0; while ( *(valueStr+i) != '\"' ) i++; *(valueStr+i) = '\0';
					strcpy( (char*)elementRef, valueStr );
				//	(char*)elementRef = npNewStrcpy( valueStr, data );		//zz debug, need to handle free() and NULL etc...
					
					printf (" %.20s", (char*)elementRef );
					
					if (*typeTagA == 's')	//goto next field if more then one element
					{
						(char*)elementRef += 1;
						i = npNextField( valueStr ); valueStr = &valueStr[i];	//zz debug, what if no more fields... mem violation?
					}
				}
//				printf( " strings not yet supports: %3s...\n", *(char*)elementRef );
			}
			else
				printf( " err 8333 - unknown type tag: %.20s\n", typeTagA );
		}
		printf("\n");

		result = 1;

	} //end loop

	if( mapType == kNPmapGlobals )
	{	
		//printf("\n7382 kNPmapGlobals");
		npUpdateGlobals(dataRef);
		//printf("\nafter 7382");
	}

//		return result;


	//loop and write out all globals in the mapType[] list
	//populates the rows using the globals map descriptor array 
/*	for (id = 1; id <= itemCount; id++)
	{
		mapItem = &data->map.typeMapGlobals[id];

		n += sprintf ((curs + n), "%d,%s,\"%s\",\"%s\"\n",	//zz optimize with npitoa...
					id,
	//				type?
					mapItem->name,
					mapItem->typeTagB,
					npGetValueStr(str, mapItem, data)	//format value str			
				 );
	}
*/

	// 
	//similar to npCSVtoTag()
	//npLoadMapFile()

	//get the first row
	while( !eof )
	{
		//goto start of next row

		//id = npstrtoi(&cursor);
		//typeA = kNPgFullscreen
		//name =
		//typeTagB =	
		//valueStr =

		eof = true;
	}
//------------------------------------------------------------------------------

finish:

	npFree( buffer, data );

	if( msg )
	{
		npPostMsg( msg, kNPmsgFile, data );
	}

	return result;
}

/*
//-----------------------------------------------------------------------------
int npSaveGlobalsCSV (char* fileName, void* dataRef)
{
	char* path = NULL;
	FILE* fileRef = NULL;

	pData data = (pData) dataRef;

	csvStr = npMalloc (kNPfileBlock, kNPfileBlockSize, data);

	//if no fileName then assign default time-stamped name
	if(!fileName)
	{
		path = npMalloc(0, kNPmaxPath, data);
		strcpy (path, "antzglobals.csv");			//add time stamp and CWD	//zz debug
	}
	else
		path = fileName;

	//open the file for writing
	fileRef = npFileOpen ((const char*)path, "rw+", data);


	//get the first row header, includes CRLF line ending
	size = npGetGlobalsCSV (csvStr, data);

	npFileWrite ((const void*)csvStr, 1, size, fileRef, data);
	
	npFree(path);
	npFree(csvStr);

	return size;
}
*/


//-----------------------------------------------------------------------------
int npOpenGlobalsCSV (char* filePath, int wordSize, int size, void* dataRef)
{
	return npOpenMapCSV(filePath, kNPmapGlobals, dataRef);
}


// appends map table name to filePath and adds '.csv' extension
//-----------------------------------------------------------------------------
int npSaveMapToCSV( char* datasetName, int mapTypeID, void* dataRef )
{
	pData data = (pData) dataRef;

	int i = 0;
	int err = 0;
	int size = 0;
	int total = 0;
	int index = 0;		//same as rootIndex...
	int rootIndex = 0;
	size_t count = 0;
	FILE* file = NULL;

	char* path = data->io.file.csvPath;
	char* name = NULL;
	char* mapTypeName = NULL;
		
	char* buffer = NULL;
	char msg[kNPurlMax];
	char filePath[kNPurlMax];
	pNPnode node = NULL;


	//convert lookup table
	mapTypeName = npMapTypeName( mapTypeID, data );

	//construct the file path name for this map type using passed in dataset name
	sprintf( filePath, "%s%s%s%s", path, datasetName, mapTypeName, ".csv" );
	sprintf( msg, "Saving: %s",  filePath);
	npPostMsg (msg, kNPmsgCtrl, data );

	if( mapTypeID == kNPmapNode )
		return npFileSaveMap( filePath, 1, strlen( filePath ), data );

	buffer = (char*) npMalloc(0, kNPfileBlockSize, data);
	if( !buffer ) return 0;

	// open the file, "w+" overwrites existing files
	printf ("\nSave File: %s\n", filePath);
	file = npFileOpen (filePath, "w+", dataRef);

	if (file == NULL)
	{
		printf("err 4215 - file open failed, cannot write file\n");							//debug, add err reporting, zz
		free (buffer);
		return 0;
	}
	
	// copies current state to the write buffer, formats as CSV
	// file buffer needs to be larger then kNPfileBlockSize
	printf("Writing CSV...\n");
	count = npMapToCSV( buffer, mapTypeID, kNPmapFileBufferMax, &index, data );

	while (count > 0)
	{
		printf(".");
		total += count = npFileWrite( buffer, 1, count, file, data );
	
		if (count >= kNPfileBlockSize) //0)//zz debug file block methods //
		{
			printf("-");
			count = npMapToCSV( buffer, mapTypeID, kNPmapFileBufferMax, &index, data );
		}
		else
			count = 0;
	}
	printf(":)\n");

	// print partial file contents
	if (total > 0)
	{
		printf("Bytes Written: %d\n", total);
		npFileRewind(file);
		size = npFileRead (buffer, 1, 79, file, dataRef);
		printf("File Contents:\n");
		for( i = 0; i < size; i++ )
			printf("%c", buffer[i]);
		printf("\n");
	}
	else
		printf("err 4316 - file write failure, zero bytes written\n");

	err = npFileClose(file, dataRef);

	free (buffer);

	npPostMsg("Done writing CSV", kNPmsgCtrl, data);	//zz debug, ...writing Globals to...

	return total;
} 


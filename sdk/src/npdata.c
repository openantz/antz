/* -----------------------------------------------------------------------------
*
*  npdata.c
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

#include "npdata.h"

#include "npctrl.h"
#include "data/npmap.h"
#include "io/npkey.h"
#include "io/npfile.h"
#include "io/npmouse.h"
#include "io/npgl.h"
#include "os/npos.h"

#include "io/db/npdb.h"		//zz db

void* gData;

void npInitDataMap (void* dataRef);
void npInitDataIO (int argc, char** argv, void* dataRef);
void npInitDataCtrl (void* dataRef);
void npInitMessage (void* dataRef);


//------------------------------------------------------------------------------
void* npInitData (int argc, char** argv)
{
	pData data = NULL;

	// if console help requested then show command usage help and exit
	npSystemConsoleHelp( argc, argv );

	printf ("Starting...\n");

	// allocate memory for the global 'data' structure
	data = (pData) malloc (sizeof(Data));
	if (data == NULL)
	{
		printf ("err 4213 - malloc failed cannot run app \n");
		return NULL;
	}
	gData = data;						//store a copy for retrieval

	// initialize data
	printf("\nnpInitDataMap");
	npInitDataMap (data);
	printf("\nnpInitDataIO");
	npInitDataIO (argc, argv, data);	//stores the command line arguments
	printf("\nnpInitDataCtrl");
	npInitDataCtrl (data);

	return gData;
	// create a fixed array of global data pointers that is thread safe, zz
	// need to support multiple instances within a single app
}

//------------------------------------------------------------------------------
void npInitDataMap(void* dataRef)
{
	int i = 0;
	pData data = (pData) dataRef;

	pNPnode node = NULL;
	pNPnode nodeChild = NULL;

	npInitMessage (data);					//zz debug, need to move struct from IO
	npInitMap (dataRef);
	npInitNode (dataRef);

	// global root reserved at index = 0
	npNodeNew (kNodeDefault, NULL, dataRef);

	// root camera, reserved at index = 1
	node = npNodeNew (kNodeCamera, NULL, dataRef);
	npDataCameraPreset (0, node, dataRef);

	// child cameras attached to root camera, skips root cam 0
	for (i=1; i < kNPcameraCount; i++)
	{
		nodeChild = npNodeNew (kNodeCamera, node, dataRef);
		npDataCameraPreset (i, nodeChild, dataRef);
	}

	// root grid at index = 2
	node = npNodeNew (kNodeGrid, NULL, dataRef);
	node->textureID = 1;		// first texture is defualt for the root grid

	// select the camera
	npSelectNode (data->map.node[kNPnodeRootCamera], dataRef);		//zz debug, no ctrl commands until after ctrl inits
}


//------------------------------------------------------------------------------
void npInitMessage (void* dataRef)
{
	int i = 0, j = 0;
	pData data = (pData) dataRef;

	data->io.message.queIndex = 0;

	for (i=0; i < kNPmessageQueMax; i++)
		for (j=0; j <= kNPmessageLengthMax; j++)  //write 257 chars, 256 +1 null
			data->io.message.que[i][j] = '\0';

	data->io.message.size = sizeof(NPmessage);
}


//------------------------------------------------------------------------------
void npInitDataRecordTag (pNPrecordTag recordTag, void* dataRef)
{
	int i = 0;

	recordTag->id = 0;

	recordTag->recordID = 0;
	recordTag->tableID = 0;
	
	recordTag->titleSize = 0;
	recordTag->descSize = 0;

	for (i = 0; i <= kNPtagTitleMax; i++)
		recordTag->title[i] = '\0';

	for (i = 0; i <= kNPtagDescMax; i++)
		recordTag->desc[i] = '\0';
	
	recordTag->size = sizeof(NPrecordTag);
}

//------------------------------------------------------------------------------
void npInitTextTag (pNPtag tag, void* dataRef)
{
	int i = 0;

	tag->tableID = 0;
	tag->recordID = 0;
	
	tag->titleSize = 0;
	tag->descSize = 0;

	tag->font = kNP_GLUT_BITMAP_9_BY_15;
	
	tag->mode = kNPtagModeBoxOutlineHUD;
	tag->lineWidth = 1.0f;

	tag->boxSize.x = 0.0f;
	tag->boxSize.y = 0.0f;

	tag->color.r = 255;			//white text
	tag->color.g = 255;
	tag->color.b = 255;
	tag->color.a = 180;			//70% opacity

	tag->boxColor.r = 0;		//black background box
	tag->boxColor.g = 0;
	tag->boxColor.b = 0;
	tag->boxColor.a = 85;		//33% opacity

	tag->lineColor.r = 255;		//white outline
	tag->lineColor.g = 255;
	tag->lineColor.b = 255;
	tag->lineColor.a = 64;		//70% opacity

	//note <= since the buffer is (kNPtagTitleMax + 1) for the null terminator
	for (i = 0; i <= kNPtagTitleMax; i++)
		tag->title[i] = '\0';

	for (i = 0; i <= kNPtagDescMax; i++)
		tag->desc[i] = '\0';
}


//may convert to a node type, zz
//------------------------------------------------------------------------------
void npInitNodeConsole (void* consoleRef, void* dataRef)
{
	int i = 0, j = 0;
	pData data = (pData) dataRef;

	pNPconsole console = (pNPconsole) consoleRef;

	console->selected = false;					//true if console is active item

	console->translate.x = 5.0f;				//location relative to origin
	console->translate.y = 5.0f;				//corner is 9,5 not 0,0		 debug, zz
	console->translate.z = 0.0f;
	
	console->lineWidth = 0.0f;					//box outline width, 0 for none

	console->colorIndex = 19;					//white font and outline color
	console->color.r = 255;
	console->color.g = 255;
	console->color.b = 255;
	console->color.a = 180;

	console->boxColorIndex = 18;				//black background box
	console->boxColor.r = 0;
	console->boxColor.g = 0;
	console->boxColor.b = 0;
	console->boxColor.a = 64;					//background box transparency

	console->colorFade = 0;						//color alpha fade with time
	console->textureID = 0;						//box background texture

	console->hide = false;						//hide console, still updated
	console->freeze = false;					//freezes lineIndex position

	console->screen.x = 0.0f;					//relative to position clamp
	console->screen.y = 0.0f;
	console->screen.z = 0.0f;

	console->position.x = kNPpositionLeft;
	console->position.y	= kNPpositionBottom;

	console->mode = 0;							//kNPconsoleMessage = default mode
	
	console->level = kNPconsoleLevelThree;		//3 lines of text
	console->box.x = 734.0f;					//80 chars * 9 + 14 (for border)
	console->box.y = 53.0f;						//3 lines * 15 + 8 (for border)

	console->font = kNP_GLUT_BITMAP_9_BY_15;
	console->justify = kNPjustifyLeft;

	console->lineMax = kNPconsoleLineMax;			//default is 40 lines max
	console->charPerLine = kNPconsoleCharPerLine;	//default is 80 characters

	console->lineIndex = 0;						//for scrolling through history
	console->lineCount = 0;
	console->historyMax = 0;					//max number of lines stored

	console->page = 0;							//zero to scroll current line
	console->selectIndex = 1;
	console->selectText = 0;					//zero for not selected
	console->insertText = 0;

	console->cursorLine = 0;
	console->cursorColumn = 0;

	console->cursorType = 0;					//default cursor is underbar
	console->cursorShow = false;

	console->inputIndex = 0;

	for (i=0; i <= kNPconsoleInputMax; i++)
		console->inputStr[i] = '\0';

	for (i=0; i < kNPconsoleLineMax; i++)
		for (j=0; j <= kNPconsoleCharPerLine; j++)	//write 81 chars, 80 +1 null
			console->line[i][j] = '\0';

	console->size = sizeof(NPconsole);	
}

//------------------------------------------------------------------------------
void npInitDataTags (pNPtags tags, void* dataRef)
{
	static bool firstRun = true;
	int i = 0;
	pData data = (pData) dataRef;
	
	//ascerts we allocate memory only on the first call
	//subsequent calls clear existing memory, upgrade to allow multiple instances //zz debug
	if (firstRun)
	{
		tags->list = NULL;												//zz debug, move this
		tags->recordList = NULL;
		tags->sort = NULL;
	}

	tags->count = 0;
	tags->recordCount = 0;
	tags->sortCount = 0;


	//check to see if mem already allocated, allows calling anytime
	if (tags->list == NULL)
	{
		tags->list = (void*) malloc (kNPtagDrawMax * sizeof(void*));
		if (tags->list == NULL)
		{
			printf ("err 4278 - malloc failed to allocate tags node list\n");
			exit(EXIT_FAILURE);
		}
	}
	for (i=0; i < kNPtagDrawMax; i++)
			tags->list[i] = NULL;

	if (tags->recordList == NULL)
	{
		tags->recordList = (void*) malloc (kNPtagMax * sizeof(void*));
		if (tags->recordList == NULL)
		{
			printf ("err 4279 - malloc failed to allocate record tag list\n");
			exit(EXIT_FAILURE);
		}
	}
	for (i=0; i < kNPtagMax; i++)
		tags->recordList[i] = NULL;

	if (tags->sort == NULL)
	{
		tags->sort = (void*) malloc (kNPtagMax * sizeof(void*));	//zz debug, should be dynamic		
		if (tags->sort == NULL)
		{
			printf ("err 4280 - malloc failed to allocate sort tag list\n");
			exit(EXIT_FAILURE);
		}
	}
	for (i=0; i < kNPtagMax; i++)
		tags->sort[i] = NULL;


	tags->size = sizeof(NPtags);	
}

//------------------------------------------------------------------------------
void npInitTools (pNPnode root, void* dataRef)
{
	pData data = (pData) dataRef;
	pNPhud hud = (pNPhud) &data->io.gl.hud;
	pNPnode node = NULL;

	//create the root HUD node
	root = npNodeNew (kNodeHUD, 0, data);

	//create the primary HUD branches
	node = npNodeNew (kNodeHUD, root, data);	//reserved for future use
		node->hudType = kNPhudNull;
	node = npNodeNew (kNodeHUD, root, data);
		node->hudType = kNPhudTags;
	node = npNodeNew (kNodeHUD, root, data);
		node->hudType = kNPhudSelection;
	node = npNodeNew (kNodeHUD, root, data);
		node->hudType = kNPhudConsole;
	node = npNodeNew (kNodeHUD, root, data);
		node->hudType = kNPhudFPS;
	node = npNodeNew (kNodeHUD, root, data);
		node->hudType = kNPhudCompass;
		//make tilt and coordXYZ sub members of compass
		node = npNodeNew (kNodeHUD, root, data);
			node->hudType = kNPhudAngle;
		node = npNodeNew (kNodeHUD, root, data);
			node->hudType = kNPhudCoordX;
		node = npNodeNew (kNodeHUD, root, data);
			node->hudType = kNPhudCoordY;
		node = npNodeNew (kNodeHUD, root, data);
			node->hudType = kNPhudCoordZ;

	node = npNodeNew (kNodeHUD, root, data);
		node->hudType = kNPhudMode;
		data->io.mouse.pickMode = kNPmodePin;		//needs to match npInitTools, zz debug, change init order?
		npPostMode (node, data);					//had to include npgl.h, zz debug

	node = npNodeNew (kNodeHUD, root, data);
		node->hudType = kNPhudTool;
		data->io.mouse.tool = kNPtoolCombo;			//needs to match npInitTools, zz debug
		npPostTool (node, data);					//had to include npgl.h, zz debug

	node = npNodeNew (kNodeHUD, root, data);
		node->hudType = kNPhudSave;

	node = npNodeNew (kNodeHUD, root, data);		//zzf
		node->hudType = kNPhudSelect;				//zz select

	node = npNodeNew (kNodeHUD, root, data);
		node->hudType = kNPhudCreate;
	node = npNodeNew (kNodeHUD, root, data);
		node->hudType = kNPhudLink;
	node = npNodeNew (kNodeHUD, root, data);
		node->hudType = kNPhudCombo;
	node = npNodeNew (kNodeHUD, root, data);
		node->hudType = kNPhudMove;
	node = npNodeNew (kNodeHUD, root, data);
		node->hudType = kNPhudRotate;
	node = npNodeNew (kNodeHUD, root, data);
		node->hudType = kNPhudSize;
	node = npNodeNew (kNodeHUD, root, data);
		node->hudType = kNPhudTopo;
	node = npNodeNew (kNodeHUD, root, data);
		node->hudType = kNPhudGeo;
	node = npNodeNew (kNodeHUD, root, data);
		node->hudType = kNPhudColor;
	node = npNodeNew (kNodeHUD, root, data);
		node->hudType = kNPhudTexture;
	node = npNodeNew (kNodeHUD, root, data);
		node->hudType = kNPhudHide;
	node = npNodeNew (kNodeHUD, root, data);
		node->hudType = kNPhudTag;					//zz select-end


	node = npNodeNew (kNodeHUD, root, data);		//zzf //zz debug ??
//		node->hudType = kNPhudCursor;
}

//------------------------------------------------------------------------------
void npInitDataHUD (void* dataRef)
{
	pData data = (pData) dataRef;
	pNPhud hud = (pNPhud) &data->io.gl.hud;

	hud->root			= NULL;	
	hud->drawTags		= true;
	hud->drawCompass	= true;
	hud->drawCoord		= true;
	hud->drawFPS		= true;
	hud->drawConsole	= true;
	hud->drawAxes		= true;
	hud->drawMouseMode	= true;
	hud->drawSelectMode	= true;
	
	npInitDataTags (&hud->tags, dataRef);
	npInitNodeConsole (&hud->console, dataRef);
	npInitTextTag (&hud->fps, dataRef);
	npInitTools (hud->root, dataRef);

	hud->size = sizeof(NPhud);
}

//------------------------------------------------------------------------------
void npInitDataGL(void* dataRef)
{
	pData data = (pData) dataRef;
	pNPgl gl = &data->io.gl;

	int i = 0;

	gl->id = 0;				//the node ID used to store this GL item 

	gl->windowID = 0;		//multiple GL contexts can share a windowID
	gl->glContext = 0;		//multiple windows can share a GL context

	strcpy (gl->name, "ANTz");		//window title //zzf

	gl->fullscreen = true;
	gl->stereo3D = false;

	gl->width = 0;
	gl->height = 0;

	gl->windowSize.x = kNPwindowWidth;
	gl->windowSize.y = kNPwindowHeight;

	gl->position.x = kNPwindowPositionX;
	gl->position.y = kNPwindowPositionY;

	gl->pixelDepth = 0;
	gl->refreshRate = 0.0f;

	gl->normal = kNPglNormalize;
	gl->shade = kNPglShadeSmooth; //kNPglShadeFlat; //works if starts Flat, debug zz
	
	gl->alphaMode = kNPalphaModeSubtractive;

	gl->textureCount = 0;

	gl->pickPass = false;
	gl->pickID = 0;

	gl->screenGrab = false;
	gl->datasetName[0] = '\0';

	gl->subsample = 1;			//zzhp

	gl->linkQueCount = 0;

	for (i=0; i < kNPlinkQueMax; i++)
		gl->linkQue[i] = NULL;

	npInitDataHUD(data);
}

//------------------------------------------------------------------------------
//zz-JJ
void npInitPropertyTracks (pNPnodePropertyTracks propertyTracks)
{
	int i = 0;

	for( i = 0; i < kNPmaxFloatProperties; i++) 
	{
		propertyTracks->nodePropertyMemoryLocations[i] = NULL;
		propertyTracks->propertyTracks[i] = 0;
	}
	propertyTracks->nodePropertyMemoryNextLocation = &propertyTracks->nodePropertyMemoryLocations[0];
	propertyTracks->propertyTrackIndex = 0;	
}

//------------------------------------------------------------------------------
//
void npInitDataChannel (void* dataRef)
{
	int i = 0, j = 0;

	pData data = (pData) dataRef;
	pNPch ch = &data->io.ch;


	memset (&ch->xref, 0, sizeof(NPxref) * kNPmaxUcharProperties);
	ch->xrefIndex = 0;
	
	// data source for the tracks
//JJ-zz	ch->fileDataSource	= NULL;
//JJ-zz	ch->fullFilePath	= NULL;

	// properties apply to all tracks on the channel
	ch->updateData			= false;
	ch->channelReadIndex	= 0;
	ch->channelWriteIndex	= 0;
	ch->channelDataSize		= 0;
	ch->channelSampleRate	= 0;
	ch->channelNextDataChangeTime = 0;
	
	// track specific data, data loaded from the track file
	ch->trackNames	= NULL;
	ch->trackFloatData	= NULL;
	ch->trackUcharData	= NULL;
	ch->trackIntData = NULL;
	ch->numberTracks	= 0;

	// for mapping track-to-node-attrib - set up when a Node subscribes to a ch
	npInitPropertyTracks (&ch->floatTracks);
	npInitPropertyTracks (&ch->ucharTracks);
	npInitPropertyTracks (&ch->intTracks);

	ch->pause = false;

	ch->size = sizeof(NPch);
}

//------------------------------------------------------------------------------
void npInitDataFile (pNPfile file, void* dataRef)
{
	int i = 0, length = 0;

	pData data = (pData) dataRef;
	
	file->loading = false;
	file->saveSelect = false;

	memset (&file->appPath, '\0', kNPmaxPath);
	memset (&file->csvPath, '\0', kNPmaxPath);
	memset (&file->mapPath, '\0', kNPmaxPath);
	memset (&file->userSelectedPath, '\0', kNPmaxPath);
	memset (&file->currentOpenPath, '\0', kNPmaxPath);

	/// @todo move nposGetAppPath to npInitIO branch
	nposGetAppPath (file->appPath, &length);

	strcat (file->csvPath, file->appPath);
	strcat (file->mapPath, file->appPath);

#ifdef NP_MSW_
	strcat (file->csvPath, "usr\\csv\\");			//default user data folders //zz debug npos.h
	strcat (file->mapPath, "usr\\images\\");
#else
	strcat (file->csvPath, "usr/csv/");
	strcat (file->mapPath, "usr/images/");
#endif

//	printf ("app path: %s\n", file->appPath);	//zz debug
//	printf ("app path: %s\n", file->csvPath);
//	printf ("app path: %s\n", file->mapPath);

	file->size = sizeof(NPfile);
}

//------------------------------------------------------------------------------
void npInitDataOSC (pNPosc osc, void* dataRef)
{
	int size = 0;

	pData data = (pData) dataRef;
	pNPoscPackListener oscItem = NULL;

	osc->max = kNPoscListMax;	//zz, fix list size at this time
	
	size = osc->max * sizeof(NPoscItem);

	//osc->list = npCalloc( 0, size, data );	//zz, fix list size at this time
	//	memset( osc->list, NULL, kNPoscListMax );

	osc->id = 0;
	osc->logMode = 0;	//0 = none, 1 = system, 2 = err, 3 = warn, 4 = debug
	osc->msgMode = kNPmsgOSC;	//0 = quiet, 1 = system, 2 = GUI

	strcpy( osc->list[0].txIP, "127.0.0.1" );
	strcpy( osc->list[0].rxIP, "127.0.0.1" );
	osc->list[0].txPort = 8000;
	osc->list[0].rxPort = 9000;

	
//	npNewConnect( txIP, txPort, rxIP, rxPort, data );

	//for( i=0; i < data->io.connectCount; i++ )
	{
	data->io.connect[0] = npMalloc( 0, sizeof(NPconnect),data );

	oscItem = &data->io.connect[0]->oscListener;
	oscItem->rxPort = 8000;
	oscItem->txPort = 9000;
	data->io.connectCount++;

	data->io.connect[1] = npMalloc( 0, sizeof(NPconnect),data );

	oscItem = &data->io.connect[1]->oscListener;
	oscItem->rxPort = 8001;
	oscItem->txPort = 9001;
	data->io.connectCount++;

	data->io.connect[2] = npMalloc( 0, sizeof(NPconnect),data );

	oscItem = &data->io.connect[2]->oscListener;
	oscItem->rxPort = 7400;
	oscItem->txPort = 7401;
	data->io.connectCount++;
	}
data->io.connectCount = 0;
	osc->size = sizeof(NPosc) + size;
}

/*
pNPdatabase npdbAddTable( void );
pNPdatabase npdbAddTable( void )
{
	pNPdatabase database = NULL;
	
	database = malloc( sizeof(NPdbTable) );
	if( !database )
	{
		printf( "err 5512 - malloc failed db->dbList[i] \n" );
		return NULL;
	}


	return database;
}
*/
 
pNPdatabase npInitDatabase( void )
{
	pNPdatabase database = NULL;
	int i = 0;
	
	database = malloc( sizeof(NPdatabase) );
	if( !database )
	{
		printf( "err 5512 - malloc failed db->dbList[i] \n" );
		return NULL;
	}

	/// @todo add thread mutex locks for generating id's
	database->id = 0;
	database->format = 0;

	database->name[0] = '\0';

	database->host = NULL;

//	database->tables     = NULL; // old, lde
	
	// new, lde
	for(i = 0; i < kNPtblMax; i++)
		database->tableList[i] = NULL;
	
	database->tableInUse = NULL;
	database->tableCount = 0;

	database->nodeCount = 0;

	database->loadUpdateRate = 0.0f;
	database->saveUpdateRate = 0.0f;

	return database;
}

pNPdbHost npInitHostDB( void )
{
	pNPdbHost host = NULL;

	host = malloc( sizeof(NPdbHost) );
	if( !host )
	{
		printf( "err 5513 - malloc failed to allocate NPdbHost \n" );
		return NULL;
	}

	host->id = 0;
	host->conn_id = 0;
	host->hostFuncSet = NULL;
	host->conn = NULL;
	host->type[0] = '\0';
	host->ip[0] = '\0';
	host->port = 0;
	host->user[0] = '\0';
	host->password[0] = '\0';
	host->inUseDB[0] = '\0';
	host->dbCount = 0;

	return host;
}

//zzd
/// allocate and initialize the core 'db' structure
//------------------------------------------------------------------------------
//void npInitDataDB (struct databases* dbs, void* dataRef) // old, lde
void npInitDataDB (void* dataRef)
{
	int i = 0;
	int err = 0;
	
	pData data = (pData) dataRef;
	pNPdbs db = &data->io.db;

	npdbInitPtrList(&data->io.db, &err, dataRef);
	db->coreNode = NULL;	///< core nodes are created after all data map inits.
							///< npAddCoreNode( kNPcoreDB, db, dataRef );
							///< init the core node, creates one if does not exist
	
	/// init single instance (global) variables
	db->running = false;		///< false during data init, set true by IO init
	db->loadUpdateRate = 0.0f;	///< startup with auto update OFF = 0
	db->saveUpdateRate = 0.0f;	///< startup with auto update OFF = 0
	db->activeDB = NULL;

	/// clear the lists for hosts, databases and function sets
	for( i=0; i < kNPdbHostMax; i++)
		db->hosts[i] = NULL;
	for( i=0; i < kNPdbFuncSetMax; i++)
		db->funcSetList[i] = NULL;
	for( i=0; i < kNPdbMax; i++)
		db->dbList[i] = NULL;

	/// create a blank host and database in slot 0 of each array, for safety
	db->hosts[0] = npInitHostDB();
	db->dbList[0] = npInitDatabase();

	db->hostCount = 1;		///< host count = 1 for the null item in slot 0
	db->funcSetCount = 0;	///< function sets are initialized by npInitDB
	db->dbCount = 1;		///< DB count = 1 for the null item in slot 0

	/// Add our default host now, this is needed for antzglobals.csv file.
	///	'localhost' is same as 127.0.0.1 and mysql default port is 3306.
	
	printf("\nnpdbAddHost");
//	npdbAddHost( "mysql", "localhost", 3306, "root", "admin", data );
	npdbAddHost("mysql", "127.0.0.1", 3306, "root", "admin", &err, data);
//	npdbAddHost("mysql", "192.168.1.130", 3306, "root", "admin", data);

	
	/// add access to the antz public database 
	// npdbAddHost( "mysql", "openantz.com", 3306, "guest", "guest", dataRef);	//zzd
	
	db->size = 0;			///< @todo add methods to track memory usage

//------------- //zzd r
	/* // Not necessary anymore, old, lde
	data->io.dbs = malloc(sizeof(struct databases));
	data->io.dbs->numberOfDatabases = 0;
	data->io.dbs->dbList = NULL;
	*/
	//data->io.db = malloc(sizeof(NPdbs));
//	npAddDb( db, "mysql", "localhost", "root", "admin", "", dataRef);	//zzd r   // lde, grab from npdbAddHost parameters
	
//	data->io.dbs->activeDB[0].currentlyUsedDatabase[0] = '\0';	//zz hmm

}


//------------------------------------------------------------------------------
void npInitDataQue (pNPque que, void* dataRef)
{
	int size = 0;

	pData data = (pData) dataRef;

	que->max = kNPqueMax;	//zz, fix list size at this time
	
	size = que->max * sizeof(NPqueItem);

	
	que->list = npCalloc( kNPqueItem, size, data );

	que->id = 0;
	que->type = 0;

	que->size = sizeof(NPque) + size;
}

//------------------------------------------------------------------------------
void npInitDataIO(int argc, char** argv, void* dataRef)
{
	int i = 0;

	pData data = (pData) dataRef;
	pNPio io = &data->io;

	// store reference to the command line arguements
	io->argc = argc;
	io->argv = argv;

	io->time = 0.0;
	io->timeStart = 0.0;
	io->cycleRateHz = 60.0;		//29.97 NTSC, 59.94 1080p, 119.88 Stereo HD
	io->cyclePeriod = 1.0 / io->cycleRateHz;
	io->cycleDelta = io->cyclePeriod;	//init with x Hz
	io->cycleCount = 0;

	io->blinkState = false;

	io->clearFrameBuffer = true;
	io->clearIndex = 0;

	io->clear.r = 0.0f;
	io->clear.g = 0.0f;
	io->clear.b = 0.0f;
	io->clear.a = 1.0f;

	io->drawMenu = false;

	io->axes.x = true;
	io->axes.y = true;
	io->axes.z = true;

	io->write = NULL;

	// setup the default URL
	memset (&io->url, '\0', kNPurlMax);
	sprintf (io->url, "http://openantz.com/code/docs/id.html");

	npInitKey (data);					///< setup key command assignments	
	npInitDataGL (data);
	npInitDataChannel (data);
	npInitDataOSC (&io->osc, data);
//	npInitDataDB (io->db, data);		///< create core db structure // old, lde
	printf("\nnpInitDataDB");
	npInitDataDB (data);				///< create core db structure
	printf("\nnpInitDataFile");
	npInitDataFile (&io->file, data);	//zz file io to npglobals.csv needs to be last

	io->size = sizeof(NPio);	//memory size of this struct, runtime dynamic
	
	//get app start time
	nposUpdateTime (data);
}

//------------------------------------------------------------------------------
void npInitDataCPU (pNPcpu cpu)
{
	//this matches a single socket 3930k cpu, 6 cores, 12MB L3, 256K L2 per core

	//system wide CPU resources
	cpu->multiPartition = 1;	//support large multi-partition NUMA HPC systems
	cpu->systemNodes	= 1;	//number of compute blades or server nodes
	cpu->sockets		= 1;	//total number of sockets in the system
	cpu->cores			= 6;	//total cores in entire system
	cpu->threads		= 12;	//total threads in entire partition

	//local compute node CPU resources
	cpu->socketsPerNode	= 1;	//number of CPU sockets in compute node
	cpu->coresPerCPU	= 6;	//can be even or number of cores
	cpu->threadsPerCPU	= 12;	//cores * 2(hyper-threads) = threadsPerCPU
		
	//system wide memory
	cpu->multiRAM	= 17179869184;	//same as cpu->RAM for single board servers
	cpu->systemRAM	= 17179869184;	//same as...

	//local compute node memory resources
	cpu->RAM		= 17179869184;	//local RAM on the compute board
	cpu->appRAM		=  4294967296;	//4GB limit for 32bit app

	cpu->cacheL3	=    12582912;	//total L3 cache per CPU
	cpu->cacheL2	=      262144;	//L2 per core, shared by hyper-threads
	cpu->cacheL1	=       65536;	//32K instruction and 32K of data

	cpu->size = sizeof(NPcpu);
}

//------------------------------------------------------------------------------
void npInitDataCtrl(void* dataRef)
{
	int i = 0;
	pData data = (pData) dataRef;

	data->ctrl.slow = 0.05f;	//normalize speed to 1.0 and 10.0, debug zz
	data->ctrl.fast = 0.5f;		//do this in npengine

	npInitDataCPU(&data->ctrl.cpu);

	data->ctrl.startup = true;

	data->ctrl.cmdFunc = NULL;

	for(i=0; i < kNPuserMax; i++)				//zz select
		data->ctrl.user[i] = NULL;

	//attach the node list to HUD (or CTRL) tree
//	npInitDataCtrlNode(data->ctrl.user[0]);
	data->ctrl.user[0] = (pNPuser)npMalloc(0, sizeof(NPuser), data);
	data->ctrl.user[0]->tool = (pNPnode)npMalloc(kNPnode, sizeof(NPnode), data);
	npInitNodeDefault(data->ctrl.user[0]->tool);
	data->ctrl.user[0]->tool->hide = true;
												//zz select - end
	data->ctrl.size = sizeof(NPctrl);
}


//------------------------------------------------------------------------------
void npCloseData( void* dataRef )
{
	pData data = (pData) gData;

	if ( data != dataRef )
		printf( "err 8888 - gData: %p != dataRef: %p\n", data, dataRef );
	else
	{		//recursively free all data elements
			//npMemoryClose(dataRef);				//zz debug
		free (data);
	}
}


//------------------------------------------------------------------------------
void* npGetDataRef()
{
	return gData;
}


//------------------------------------------------------------------------------
void npUpdateData (void* dataRef)
{
	pData data = (pData) dataRef;

	//update the data->map

	return;
}

//------------------------------------------------------------------------------
char* npGetDataTypeName (int type)
{
	if (type > 0 )//&& type < kTypeCount)
		return NULL;//(data->map.typeList[type].name);
	else
		return NULL;
}

//------------------------------------------------------------------------------
char* npGetDataTypeDesc (int type)
{
	if (type > 0 )//&& type < kTypeCount)
		return NULL;//(data->map.typeList[type].description);
	else
		return NULL;
}


// loads preset scene using currently selected object as the origin
// if alt is pressed (ie ALT+5) it loads preset at the global origin
//------------------------------------------------------------------------------
void npDataPreset (int preset, void* dataRef)
{
	int i = 0, j = 0, k = 0;		//zzhp

	NPfloatXYZ	translateOffset;
	NPfloatXYZ	rotateOffset;
	NPfloatXYZ	scaleOffset;

	pData data = (pData) dataRef;
	pNPnode node = data->map.node[data->map.nodeRootIndex];

	pNPnode nodeChild = NULL;

	NPpinPtr pinData = NULL;

	// don't offset if alt key held or camera selected
	if (data->io.key.modAlt || data->map.nodeRootIndex == kNPnodeRootPin)
	{
		translateOffset.x = 0.0f;
		translateOffset.y = 0.0f;
		translateOffset.z = 0.0f;

		rotateOffset.x = 0.0f;
		rotateOffset.y = 0.0f;
		rotateOffset.z = 0.0f;

		scaleOffset.x = 0.0f;
		scaleOffset.y = 0.0f;
		scaleOffset.z = 0.0f;
	}
	else
	{		// add code to calc translate, rotate position based on
			// global coordinates think of GL order of operation,	debug zz
		translateOffset.x = node->translate.x;
		translateOffset.y = node->translate.y;
		translateOffset.z = node->translate.z;

		rotateOffset.x = node->rotate.x;
		rotateOffset.y = node->rotate.y;
		rotateOffset.z = node->rotate.z;

		scaleOffset.x = node->scale.x;
		scaleOffset.y = node->scale.y;
		scaleOffset.z = node->scale.z;
	}

	switch (preset)
	{
		case kNPcmdPresetOne :

			node = npNodeNew (kNodePin, NULL, dataRef);
				node->rotate.z = 1.7f;
				node->scale.x = 1.5f;
				node->scale.y = 1.5f;
				node->scale.z = 1.5f;

				node->translate.x -= 3.0f;
				node->translate.y -= 22.0f;

				node->colorIndex = 0;

			//create primary torus
			node = nodeChild = npNodeNew (kNodePin, node, dataRef);
				nodeChild->colorIndex = 14;

			nodeChild = npNodeNew (kNodePin, node, dataRef);
				nodeChild->colorIndex = 0;
			nodeChild = npNodeNew (kNodePin, node, dataRef);
				nodeChild->colorIndex = 0;

			node = npNodeNew (kNodePin, node, dataRef);
				node->colorIndex = 1;
			nodeChild = npNodeNew (kNodePin, node, dataRef);
				nodeChild->colorIndex = 14;
			node = npNodeNew (kNodePin, node, dataRef);
				node->colorIndex = 4;
			node = npNodeNew (kNodePin, node, dataRef);
				node->colorIndex = 3;
								node->rotate.x = -0.7f;
			node = npNodeNew (kNodePin, node, dataRef);
				node->colorIndex = 3;
								node->rotate.x = 0.7f;
			node = npNodeNew (kNodePin, node, dataRef);
				node->colorIndex = 4;
								node->rotate.x = 0.7f;
			node = npNodeNew (kNodePin, node, dataRef);
				node->colorIndex = 2;
								node->rotate.x = 0.7f;
			node = npNodeNew (kNodePin, node, dataRef);
				node->colorIndex = 1;
								node->rotate.x = 1.5f;

			npCtrlCommand (kNPcmdSelectAll, data);
			npCtrlCommand (kNPcmdColorUp, data);
			npCtrlCommand (kNPcmdColorDown, data);
			npCtrlCommand (kNPcmdSelectAll, data);

			npSelectNode (data->map.node[data->map.nodeRootCount - 1], data);

			break;
		case kNPcmdPresetTwo :
			// draw 6x6 grid of pins
			for(i = -16; i < 16; i++)		//zzhp
				for(j=-16; j < 16; j++)		//zzhp
					for(k=0; k < 24; k++)		//zzhp
				{
					// create the root pin
					node = npNodeNew (kNodePin, NULL, dataRef);
					node->translate.x = i * 5.0f;
					node->translate.y = j * 5.0f;
					node->translate.z = k * 10.0f;		//zzhp

					node->translate.x += translateOffset.x;
					node->translate.y += translateOffset.y;
					node->translate.z += translateOffset.z;

						node->colorIndex = 0;
					node = nodeChild = npNodeNew (kNodePin, node, dataRef);
						nodeChild->colorIndex = 14;

					nodeChild = npNodeNew (kNodePin, node, dataRef);
						nodeChild->colorIndex = 1;
					nodeChild = npNodeNew (kNodePin, node, dataRef);
						nodeChild->colorIndex = 14;
					nodeChild = npNodeNew (kNodePin, node, dataRef);
						nodeChild->colorIndex = 1;
					node = npNodeNew (kNodePin, node, dataRef);
						node->colorIndex = 2;
					nodeChild = npNodeNew (kNodePin, node, dataRef);
						nodeChild->colorIndex = 1;
					node = npNodeNew (kNodePin, node, dataRef);
						node->colorIndex = 3;
					nodeChild = npNodeNew (kNodePin, node, dataRef);
						nodeChild->colorIndex = 4;				
				}
				npSelectNode (data->map.node[data->map.nodeRootCount - 1], data);	

			break;
		case kNPcmdPresetThree :
			//first pin root
			node = npNodeNew (kNodePin, NULL, dataRef);
									node->rotateRate.z = 0.001f;
				node->colorIndex = 0;
			node = nodeChild = npNodeNew (kNodePin, node, dataRef);
				nodeChild->colorIndex = 14;
			//first pin children
			nodeChild = npNodeNew (kNodePin, node, dataRef);
				nodeChild->colorIndex = 1;
			nodeChild = npNodeNew (kNodePin, node, dataRef);
				nodeChild->colorIndex = 14;
			nodeChild = npNodeNew (kNodePin, node, dataRef);
				nodeChild->colorIndex = 1;
			node = npNodeNew (kNodePin, node, dataRef);
				node->colorIndex = 2;
			nodeChild = npNodeNew (kNodePin, node, dataRef);
				nodeChild->colorIndex = 1;
			node = npNodeNew (kNodePin, node, dataRef);
				node->colorIndex = 3;
			nodeChild = npNodeNew (kNodePin, node, dataRef);
				nodeChild->colorIndex = 4;

			//next pin
			node = npNodeNew (kNodePin, NULL, dataRef);
									node->rotateRate.z = -0.001f;
				node->colorIndex = 0;
			node = nodeChild = npNodeNew (kNodePin, node, dataRef);
				nodeChild->colorIndex = 14;

			nodeChild = npNodeNew (kNodePin, node, dataRef);
				nodeChild->colorIndex = 1;
			nodeChild = npNodeNew (kNodePin, node, dataRef);
				nodeChild->colorIndex = 14;
			nodeChild = npNodeNew (kNodePin, node, dataRef);
				nodeChild->colorIndex = 1;
			node = npNodeNew (kNodePin, node, dataRef);
				node->colorIndex = 2;
			nodeChild = npNodeNew (kNodePin, node, dataRef);
				nodeChild->colorIndex = 1;
			node = npNodeNew (kNodePin, node, dataRef);
				node->colorIndex = 3;
			nodeChild = npNodeNew (kNodePin, node, dataRef);
				nodeChild->colorIndex = 4;

			//next pin
			node = npNodeNew (kNodePin, NULL, dataRef);
				node->colorIndex = 0;
			node = nodeChild = npNodeNew (kNodePin, node, dataRef);
				nodeChild->colorIndex = 0;

			nodeChild = npNodeNew (kNodePin, node, dataRef);
				nodeChild->colorIndex = 1;
			nodeChild = npNodeNew (kNodePin, node, dataRef);
				nodeChild->colorIndex = 14;
			nodeChild = npNodeNew (kNodePin, node, dataRef);
				nodeChild->colorIndex = 1;
			node = npNodeNew (kNodePin, node, dataRef);
								node->rotateRate.x = 0.001f;
				node->colorIndex = 2;
			nodeChild = npNodeNew (kNodePin, node, dataRef);
				nodeChild->colorIndex = 1;
			node = npNodeNew (kNodePin, node, dataRef);
									node->rotateRate.x = 0.005f;
				node->colorIndex = 3;
			nodeChild = npNodeNew (kNodePin, node, dataRef);
				nodeChild->colorIndex = 4;
			
			//next pin
			node = npNodeNew (kNodePin, NULL, dataRef);
									node->rotateRate.z = 0.005f;
				node->colorIndex = 0;
			node = nodeChild = npNodeNew (kNodePin, node, dataRef);
				nodeChild->colorIndex = 14;

			nodeChild = npNodeNew (kNodePin, node, dataRef);
				nodeChild->colorIndex = 1;
			nodeChild = npNodeNew (kNodePin, node, dataRef);
				nodeChild->colorIndex = 14;
			nodeChild = npNodeNew (kNodePin, node, dataRef);
				nodeChild->colorIndex = 1;
			node = npNodeNew (kNodePin, node, dataRef);
				node->colorIndex = 2;
			nodeChild = npNodeNew (kNodePin, node, dataRef);
				nodeChild->colorIndex = 2;
			node = npNodeNew (kNodePin, node, dataRef);
				node->colorIndex = 3;
			nodeChild = npNodeNew (kNodePin, node, dataRef);
				nodeChild->colorIndex = 4;

			//next pin
			node = npNodeNew (kNodePin, NULL, dataRef);
									node->rotateRate.z = 0.001f;
				node->colorIndex = 0;
			node = nodeChild = npNodeNew (kNodePin, node, dataRef);
				nodeChild->colorIndex = 14;

			nodeChild = npNodeNew (kNodePin, node, dataRef);
				nodeChild->colorIndex = 1;
			nodeChild = npNodeNew (kNodePin, node, dataRef);
				nodeChild->colorIndex = 14;
			nodeChild = npNodeNew (kNodePin, node, dataRef);
				nodeChild->colorIndex = 1;
			node = npNodeNew (kNodePin, node, dataRef);
				node->colorIndex = 2;
			nodeChild = npNodeNew (kNodePin, node, dataRef);
				nodeChild->colorIndex = 1;
			node = npNodeNew (kNodePin, node, dataRef);
				node->colorIndex = 3;
			nodeChild = npNodeNew (kNodePin, node, dataRef);
				nodeChild->colorIndex = 4;

			npCtrlCommand (kNPcmdSelectAll, data);
			npCtrlCommand (kNPcmdColorUp, data);
			npCtrlCommand (kNPcmdColorDown, data);
			npCtrlCommand (kNPcmdSelectAll, data);

			npSelectNode (data->map.node[data->map.nodeRootCount - 1], data);

			break;
		case kNPcmdPresetFour : 
			
			node = npNodeNew (kNodePin, NULL, dataRef);
				node->colorIndex = 0;

			// primary torus, branchLevel == 1 
			node = npNodeNew (kNodePin, node, dataRef);//node->child[0];
				node->colorIndex = 14;
			// child branchLevel == 2
			nodeChild = npNodeNew (kNodePin, node, dataRef);
				nodeChild->colorIndex = 1;
			nodeChild = npNodeNew (kNodePin, node, dataRef);
				nodeChild->colorIndex = 14;
			nodeChild = npNodeNew (kNodePin, node, dataRef);
				nodeChild->colorIndex = 1;

			// select child, branchLevel == 3
			node = npNodeNew (kNodePin, node, dataRef);
				node->colorIndex = 2;
			nodeChild = npNodeNew (kNodePin, node, dataRef);
				nodeChild->colorIndex = 1;

			// next branchLevel
			node = npNodeNew (kNodePin, node, dataRef);
				node->colorIndex = 3;
			nodeChild = npNodeNew (kNodePin, node, dataRef);
				nodeChild->colorIndex = 4;

			npCtrlCommand (kNPcmdSelectAll, data);
			npCtrlCommand (kNPcmdColorUp, data);
			npCtrlCommand (kNPcmdColorDown, data);
			npCtrlCommand (kNPcmdSelectAll, data);

			npSelectNode (data->map.node[data->map.nodeRootCount - 1], data);

			break;
		case kNPcmdPresetFive : break;
		case kNPcmdPresetSix : break;
		case kNPcmdPresetSeven : break;
		case kNPcmdPresetEight : break;

		default : break;
	}

	npCtrlCommand (kNPcmdCamera, data);			// select the camera
}


//resets the camera node passed in, specific to the preset number
//------------------------------------------------------------------------------
void npDataCameraPreset (int preset, pNPnode node, void* dataRef)
{
	pNPnode nodeParent = NULL;
	
	// reset to defaults
	// retains parameters node->id, node->branchLevel, node->parent...
	npInitNodeCamera (node);

	//set preset values
	switch (preset)
	{
		case 0 :	//diagnol
			node->translate.x = -27.0f;
			node->translate.y = -25.0f;
			node->translate.z = 9.0f;

			node->rotate.x = 75.0f;			// x is tilt, axis is inverted
			node->rotate.y = 30.0f;			// y is heading		
			node->rotate.z = 0.0f;			// z is roll
			break;
		case 1 :	//top
			node->translate.x = -0.5f;		// fits origin grid screen vertical
			node->translate.y = 0.0f;
			node->translate.z = 571.75f;

			node->rotate.x = 0.0f;			// zero tilt is straight down -z axis
			node->rotate.y = 0.0f;			// zero heading is North
			node->rotate.z = 0.0f;
			break;
		case 2 :	//front
			node->translate.x = 0.0f;
			node->translate.y = -90.0f;
			node->translate.z = 7.0f;

			node->rotate.x = 90.0f;			// 90 degrees looks at horizon
			node->rotate.y = 0.0f;			// zero heading is North
			node->rotate.z = 0.0f;			// zero roll for horizontal horizon
			break;
		case 3 :	//right side
			node->translate.x = 85.0f;
			node->translate.y = 0.0f;
			node->translate.z = 7.0f;

			node->rotate.x = 90.0f;
			node->rotate.y = -90.0f;
			node->rotate.z = 0.0f;
			break;
		default :	//diagnol
			node->translate.x = -27.0f;
			node->translate.y = -25.0f;
			node->translate.z = 9.0f;

			node->rotate.x = 75.0f;
			node->rotate.y = 15.0f;
			node->rotate.z = 0.0f;
			break;
	}
}

//zz debug add support for startup messages to be sent to system console	//zz debug
//adds message to the que to be processed by npDispatchMessages
//------------------------------------------------------------------------------
void npPostMsg (char* message, int type, void* dataRef)
{
	pData data = (pData) dataRef;
	//add handling for type filtering, perhaps create a msg struct with type, debug zz

#ifdef NDEBUG																//zz debug
	if (type == kNPmsgDebug)		//discard debug messages if NOT Debug build
		return;
#endif

	if( data->ctrl.startup )
	{
		printf( "%s\n", message );
		return;					//zz debug, update to store messages elsewhere
	}
	
	if ( data->io.gl.hud.console.mode == kNPconsoleMenu && type != kNPmsgView )
		return;

	data->io.message.queIndex++;
	if (data->io.message.queIndex >= kNPmessageQueMax)
	{
		printf("warning 8888 - message que max exceeded, messages lost\n");
		data->io.message.queIndex = 1;		//queIndex = 0 for no messages
	}

	strncpy (&data->io.message.que[data->io.message.queIndex][0], 
				message, kNPmessageLengthMax);

	//add err reporting of messages that are too long, debug zz
}


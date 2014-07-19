/* -----------------------------------------------------------------------------
*
*  npdata.c
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

#include "npdata.h"
#include "npctrl.h"
#include "data/npmap.h"
#include "io/npkey.h"
#include "io/npfile.h"
#include "io/npmouse.h"
#include "os/npos.h"

void* gData;

void npInitDataMap (void* dataRef);
void npInitDataIO (int argc, char** argv, void* dataRef);
void npInitDataCtrl (void* dataRef);
void npInitMessage (void* dataRef);


//------------------------------------------------------------------------------
void* npInitData (int argc, char** argv)
{
	// allocate memory for the global 'data' structure
	pData data = (pData) malloc (sizeof(Data));
	if (data == NULL)
	{
		printf ("err 4213 - malloc failed cannot run app \n");
		return NULL;
	}

	printf ("Starting...\n");

	// initialize data
	npInitDataMap (data);
	npInitDataIO (argc, argv, data);	//stores the command line arguements
	npInitDataCtrl (data);


	gData = data;						//store a copy for retrieval

	return gData;
	// create a fixed array of global data pointers that is thread safe, zz
	// need to support multiple instances within a single app
}


//------------------------------------------------------------------------------
void npInitDataMap(void* dataRef)
{
	int i = 0;
	pData data = (pData) dataRef;

	NPnodePtr node = NULL;
	NPnodePtr nodeChild = NULL;

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
	// select the first child camera
//	npCtrlCommand (kNPcmdCamera, dataRef);		//zz debug, no ctrl commands until after ctrl inits

	// root grid at index = 2
	node = npNodeNew (kNodeGrid, NULL, dataRef);
	node->textureID = 1;		// first texture is defualt for the root grid

	// setup initial scene using preset 1
	// npDataPreset (1, dataRef);

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

	for (i = 0; i <= kNPtitleSizeMax; i++)
		recordTag->title[i] = '\0';

	for (i = 0; i <= kNPdescSizeMax; i++)
		recordTag->desc[i] = '\0';
}

//------------------------------------------------------------------------------
void npInitTextTag (pNPtextTag tag, void* dataRef)
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

	for (i = 0; i <= kNPtitleSizeMax; i++)
		tag->title[i] = '\0';

	for (i = 0; i <= kNPdescSizeMax; i++)
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

	console->mode = kNPconsoleModeThree;		//3 lines of text
	console->box.x = 734.0f;					//80 chars * 9 + 14 (for border)
	console->box.y = 53.0f;						//3 lines * 15 + 8 (for border)

	console->font = kNP_GLUT_BITMAP_9_BY_15;
	console->justify = kNPjustifyLeft;

	console->lineMax = kNPconsoleLineMax;			//default is 40 lines max
	console->charPerLine = kNPconsoleCharPerLine;	//default is 80 characters

	console->lineIndex = 0;						//for scrolling through history
	console->lineCount = 0;
	console->historyMax = 0;					//max number of lines stored

	for (i=0; i < kNPconsoleLineMax; i++)
		for (j=0; j <= kNPconsoleCharPerLine; j++)	//write 81 chars, 80 +1 null
			console->line[i][j] = '\0';

	console->size = sizeof(NPconsole);	
}

//------------------------------------------------------------------------------
void npInitDataTags (pNPtags tags, void* dataRef)
{
	int i = 0;
	pData data = (pData) dataRef;

	tags->count = 0;
	tags->recordCount = 0;


	tags->list = (void*) malloc (kNPtagListMax * sizeof(void*));
	if (tags->list == NULL)
	{
		printf ("err 4278 - malloc failed to allocate tags node list\n");
		exit(EXIT_FAILURE);
	}

	for (i=0; i < kNPtagListMax; i++)
			tags->list[i] = NULL;

	tags->recordList = (void*) malloc (kNPrecordTagListMax * sizeof(void*));
	if (tags->recordList == NULL)
	{
		printf ("err 4279 - malloc failed to allocate record tag list\n");
		exit(EXIT_FAILURE);
	}

	for (i=0; i < kNPrecordTagListMax; i++)
		tags->recordList[i] = NULL;

	tags->size = sizeof(NPtags);	
}

//------------------------------------------------------------------------------
void npInitHUD(void* dataRef)
{
	pData data = (pData) dataRef;
	pNPhud hud = (pNPhud) &data->io.gl.hud;

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

	hud->size = sizeof(NPhud);
}

//------------------------------------------------------------------------------
void npInitDataGL(void* dataRef)
{
	pData data = (pData) dataRef;
	
	data->io.gl.normal = kNPglNormalize;
	data->io.gl.shade = kNPglShadeSmooth; //kNPglShadeFlat; //works if starts Flat, debug zz
	
	data->io.gl.alphaMode = kNPalphaModeSubtractive;

	data->io.gl.textureCount = 0;

	data->io.gl.pickPass = false;
	data->io.gl.pickID = 0;

	data->io.gl.width = 0;
	data->io.gl.height = 0;

	data->io.gl.pixelDepth = 0;
	data->io.gl.refreshRate = 0.0f;

	data->io.gl.fullScreen = false;
	data->io.gl.stereo = false;

	data->io.gl.windowID = 0;

	npInitHUD(data);
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
	ch->fileDataSource	= NULL;
	ch->fullFilePath	= NULL;

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
	
	memset (&file->appPath, '\0', kNPmaxPath);
	memset (&file->csvPath, '\0', kNPmaxPath);
	memset (&file->mapPath, '\0', kNPmaxPath);
	memset (&file->userSelectedPath, '\0', kNPmaxPath);

	nposGetAppPath (file->appPath, &length);

	strcat (file->csvPath, file->appPath);
	strcat (file->mapPath, file->appPath);

#ifdef NP_MSW_
	strcat (file->csvPath, "antzcsv\\");
	strcat (file->mapPath, "antzmap\\");
#else
	strcat (file->csvPath, "antzcsv/");
	strcat (file->mapPath, "antzmap/");
#endif

//	printf ("app path: %s\n", file->appPath);	//zz debug
//	printf ("app path: %s\n", file->csvPath);
//	printf ("app path: %s\n", file->mapPath);

	file->size = sizeof(NPfile);
}


//------------------------------------------------------------------------------
void npInitDataIO(int argc, char** argv, void* dataRef)
{
	int i = 0;

	pData data = (pData) dataRef;
	pNPio io = &data->io;

	// store reference to the command line arguements
	data->io.argc = argc;
	data->io.argv = argv;

	data->io.time = 0.0;
	data->io.timeStart = 0.0;
	data->io.cycleRateHz = 60.0;		//29.97 NTSC, 59.94 1080p, 119.88
	data->io.cyclePeriod = 1.0 / data->io.cycleRateHz;
	data->io.cycleDelta = data->io.cyclePeriod;	//init with x Hz
	data->io.cycleCount = 0;

	data->io.clearFrameBuffer = true;
	data->io.clearIndex = 0;

	data->io.clear.r = 0.0f;
	data->io.clear.g = 0.0f;
	data->io.clear.b = 0.0f;
	data->io.clear.a = 0.0f;

	data->io.drawMenu = false;

	data->io.axes.x = true;
	data->io.axes.y = true;
	data->io.axes.z = true;

	// clear arrays
	for (i=0; i < kNPdataTypeMax; i++) data->io.write[i] = NULL;

	// setup the default URL
	memset (&data->io.url, '\0', kNPurlMax);
	sprintf (data->io.url, "http://openantz.com/code/docs/id.html");

	// setup initial key command assignments
	npInitKey (data);
	npInitDataGL (data);
	npInitDataChannel (data);
	npInitDataFile (&io->file, data);

	nposUpdateTime (data);


	data->io.size = sizeof(NPio);
}


//------------------------------------------------------------------------------
void npInitDataCtrl(void* dataRef)
{
	pData data = (pData) dataRef;

	data->ctrl.slow = 0.05f;	//normalize speed to 1.0 and 10.0, debug zz
	data->ctrl.fast = 0.5f;		//do this in npengine

	data->ctrl.size = sizeof(NPctrl);
}


//------------------------------------------------------------------------------
void npCloseData()
{
	pData data = (pData) gData;
	
	free (data);
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
	int i = 0;
	int j = 0;

	NPfloatXYZ	translateOffset;
	NPfloatXYZ	rotateOffset;
	NPfloatXYZ	scaleOffset;

	pData data = (pData) dataRef;
	NPnodePtr node = data->map.node[data->map.nodeRootIndex];

	NPnodePtr nodeChild = NULL;

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
	{
					// add code to calc translate, rotate position based on
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
			for(i = -3; i < 3; i++)
				for(j=-3; j < 3; j++)
				{
					// create the root pin
					node = npNodeNew (kNodePin, NULL, dataRef);
					node->translate.x = i * 15.0f;
					node->translate.y = j * 15.0f;

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
void npDataCameraPreset (int preset, NPnodePtr node, void* dataRef)
{
	NPnodePtr nodeParent = NULL;
	
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


//adds message to the que to be processed by npDispatchMessages
//------------------------------------------------------------------------------
void npPostMsg (char* message, int type, void* dataRef)
{
	pData data = (pData) dataRef;
	//add handling for type filtering, perhaps create a msg struct with type, debug zz

#ifdef NDEBUG																//zz debug
//	if (type == kNPmsgDebug)		//discard debug messages if NOT Debug build
//		return;
#endif

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


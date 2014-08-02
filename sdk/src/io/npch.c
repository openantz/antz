/* -----------------------------------------------------------------------------
*
*  npch.c
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

#include "npch.h"

#include "../npio.h"
#include "../npctrl.h"
#include "npfile.h"
#include "../os/npos.h"

#include "net/nposcpack.h"									//JJ-zz //zz osc


#include <time.h>


int GetTrackOffset (char** trackScanner, char* propertyName);
void MapTrackOffset (int trackOffset, pNPnode node, char* propertyName, void* dataRef);
void CreateTracks (int numberTracks, void *dataRef);
void SaveHeader (int offset, char* header, void *dataRef);
int SaveHeaders (char** headers, void *dataRef);
bool npChSaveTrackData (int numberTracks, int offset, float** trackFloatData, unsigned char** trackUcharData, int** trackIntData, char** buffer);
void DumpChannel (char* str, pNPch ch);

bool loopAtEndOfTrack = true;	//zz,  move to data->io.ch


//------------------------------------------------------------------------------
int GetTrackOffset(char** trackScanner, char* propertyName)
{
	int offset = 0;

	while (*trackScanner) 
	{
		if (strstr(*trackScanner, propertyName) != NULL)
			return offset;

		offset++;
		trackScanner++;
	}

	return -1;
}

//------------------------------------------------------------------------------
void MapTrackToIntAttribute (int trackOffset, int* attributeValuePtr, void* dataRef)
{
	pData data = (pData) dataRef;

	if (data->io.ch.intTracks.propertyTrackIndex < kNPmaxPropertiesMapped) 
	{
		printf("mapping int trackOffset %d to memory address %x\n", trackOffset, attributeValuePtr);
		*data->io.ch.intTracks.nodePropertyMemoryNextLocation++ = (void*)attributeValuePtr;
		data->io.ch.intTracks.propertyTracks[data->io.ch.intTracks.propertyTrackIndex++] = trackOffset;
	}
}

//------------------------------------------------------------------------------
void MapTrackToFloatAttribute (int trackOffset, float* attributeValuePtr, void* dataRef)
{
	pData data = (pData) dataRef;

	if (data->io.ch.floatTracks.propertyTrackIndex < kNPmaxPropertiesMapped) 
	{
		printf("mapping float trackOffset %d to memory address %x\n", trackOffset, attributeValuePtr);
		*data->io.ch.floatTracks.nodePropertyMemoryNextLocation++ = (void*)attributeValuePtr;
		data->io.ch.floatTracks.propertyTracks[data->io.ch.floatTracks.propertyTrackIndex++] = trackOffset;
	}
}

//------------------------------------------------------------------------------
void MapTrackToUcharAttribute (int trackOffset, unsigned char* attributeValuePtr, void* dataRef)
{
	pData data = (pData) dataRef;

	if (data->io.ch.ucharTracks.propertyTrackIndex < kNPmaxPropertiesMapped) 
	{
		printf("mapping uchar trackOffset %d to memory address %x\n", trackOffset, attributeValuePtr);
		*data->io.ch.ucharTracks.nodePropertyMemoryNextLocation++ = (void*)attributeValuePtr;
		data->io.ch.ucharTracks.propertyTracks[data->io.ch.ucharTracks.propertyTrackIndex++] = trackOffset;
	}
}

//------------------------------------------------------------------------------
void MapTrackOffset(int trackOffset, pNPnode node, char* propertyName, void* dataRef)
{
	pData data = (pData) dataRef;

	printf("Map trackOffset %d to property %s\n", trackOffset, propertyName);
	//items have been ordered in approximate order of likelihood of use
	if (strcmp(propertyName, "translate_x") == 0) MapTrackToFloatAttribute (trackOffset, &node->translate.x, data);
	else if (strcmp(propertyName, "translate_y") == 0) MapTrackToFloatAttribute (trackOffset, &node->translate.y, data);
	else if (strcmp(propertyName, "translate_z") == 0) MapTrackToFloatAttribute (trackOffset, &node->translate.z, data);
	else if (strcmp(propertyName, "rotate_x") == 0) MapTrackToFloatAttribute (trackOffset, &node->rotate.x, data);
	else if (strcmp(propertyName, "rotate_y") == 0) MapTrackToFloatAttribute (trackOffset, &node->rotate.y, data);
	else if (strcmp(propertyName, "rotate_z") == 0) MapTrackToFloatAttribute (trackOffset, &node->rotate.z, data);
	else if (strcmp(propertyName, "scale_x") == 0) MapTrackToFloatAttribute (trackOffset, &node->scale.x, data);
	else if (strcmp(propertyName, "scale_y") == 0) MapTrackToFloatAttribute (trackOffset, &node->scale.y, data);
	else if (strcmp(propertyName, "scale_z") == 0) MapTrackToFloatAttribute (trackOffset, &node->scale.z, data);

	else if (strcmp(propertyName, "geometry") == 0) MapTrackToIntAttribute (trackOffset, &node->geometry, data);
	else if (strcmp(propertyName, "ratio") == 0) MapTrackToFloatAttribute (trackOffset, &node->ratio, data);
	else if (strcmp(propertyName, "topo") == 0) MapTrackToIntAttribute (trackOffset, &node->topo, data);
	else if (strcmp(propertyName, "facet") == 0) MapTrackToIntAttribute (trackOffset, &node->facet, data);

	else if (strcmp(propertyName, "color_index") == 0) MapTrackToIntAttribute (trackOffset, &node->colorIndex, data);
	else if (strcmp(propertyName, "color_r") == 0) MapTrackToUcharAttribute (trackOffset, &node->color.r, data);
	else if (strcmp(propertyName, "color_g") == 0) MapTrackToUcharAttribute (trackOffset, &node->color.g, data);
	else if (strcmp(propertyName, "color_b") == 0) MapTrackToUcharAttribute (trackOffset, &node->color.b, data);
	else if (strcmp(propertyName, "color_a") == 0) MapTrackToUcharAttribute (trackOffset, &node->color.a, data);
	else if (strcmp(propertyName, "color_shift") == 0) MapTrackToFloatAttribute (trackOffset, &node->colorShift, data);

	else if (strcmp(propertyName, "texture_id") == 0) MapTrackToIntAttribute (trackOffset, &node->textureID, data);
	else if (strcmp(propertyName, "hide") == 0) MapTrackToUcharAttribute (trackOffset, &node->hide, data);

	else if (strcmp(propertyName, "tag_mode") == 0) MapTrackToIntAttribute (trackOffset, &node->tagMode, data);
	else if (strcmp(propertyName, "tag_offset_x") == 0) MapTrackToFloatAttribute (trackOffset, &node->tagOffset.x, data);
	else if (strcmp(propertyName, "tag_offset_y") == 0) MapTrackToFloatAttribute (trackOffset, &node->tagOffset.y, data);
	else if (strcmp(propertyName, "tag_offset_z") == 0) MapTrackToFloatAttribute (trackOffset, &node->tagOffset.z, data);
	
	else if (strcmp(propertyName, "translate_rate_x") == 0) MapTrackToFloatAttribute (trackOffset, &node->translateRate.x, data);
	else if (strcmp(propertyName, "translate_rate_y") == 0) MapTrackToFloatAttribute (trackOffset, &node->translateRate.y, data);
	else if (strcmp(propertyName, "translate_rate_z") == 0) MapTrackToFloatAttribute (trackOffset, &node->translateRate.z, data);
	else if (strcmp(propertyName, "rotate_rate_x") == 0) MapTrackToFloatAttribute (trackOffset, &node->rotateRate.x, data);
	else if (strcmp(propertyName, "rotate_rate_y") == 0) MapTrackToFloatAttribute (trackOffset, &node->rotateRate.y, data);
	else if (strcmp(propertyName, "rotate_rate_z") == 0) MapTrackToFloatAttribute (trackOffset, &node->rotateRate.z, data);
	else if (strcmp(propertyName, "scale_rate_x") == 0) MapTrackToFloatAttribute (trackOffset, &node->scaleRate.x, data);
	else if (strcmp(propertyName, "scale_rate_y") == 0) MapTrackToFloatAttribute (trackOffset, &node->scaleRate.y, data);
	else if (strcmp(propertyName, "scale_rate_z") == 0) MapTrackToFloatAttribute (trackOffset, &node->scaleRate.z, data);

	//below are items that are not likely to be used, but maybe
	else if (strcmp(propertyName, "selected") == 0) MapTrackToIntAttribute (trackOffset, &node->selected, data);
	else if (strcmp(propertyName, "shader") == 0) MapTrackToIntAttribute (trackOffset, &node->shader, data);
	else if (strcmp(propertyName, "average") == 0) MapTrackToIntAttribute (trackOffset, &node->average, data);
	else if (strcmp(propertyName, "interval") == 0) MapTrackToIntAttribute (trackOffset, &node->interval, data);
	
	else if (strcmp(propertyName, "lineWidth") == 0) MapTrackToFloatAttribute (trackOffset, &node->lineWidth, data);
	else if (strcmp(propertyName, "pointSize") == 0) MapTrackToFloatAttribute (trackOffset, &node->pointSize, data);
	
	else printf("unknown property '%s'\n", propertyName);
	// may add the below node attributes at some point, but don't see the need
	//	NPintXYZ	autoZoom;
	//	NPintXYZ	triggerHi;
	//	NPintXYZ	triggerLo;
	//	NPfloatXYZ	setHi;
	//	NPfloatXYZ	setLo;
	//	NPfloatXYZ	proximity;
	//	NPintXYZ	proximityMode;
	//	NPintXYZ	segments;
}

//------------------------------------------------------------------------------
void CreateTracks (int numberTracks, void *dataRef)
{
	pData data = (pData) dataRef;
	pNPch ch = &data->io.ch;
	int i = 0;
	float** trackFloatDataScanner = NULL;
	unsigned char** trackUcharDataScanner = NULL;
	int** trackIntDataScanner = NULL;
	char* trackBuffer = NULL;

	ch->trackFloatData = (float**) calloc (numberTracks + 1, sizeof(float*));
	if (ch->trackFloatData == NULL)
	{
		npPostMsg ("err 2944 - malloc failed, CreateTracks", kNPmsgErr, data);
		return;
	}
	ch->trackUcharData = (unsigned char**) calloc (numberTracks + 1, sizeof(unsigned char*));
	if (ch->trackUcharData == NULL)
	{
		npPostMsg ("err 2944 - malloc failed, CreateTracks", kNPmsgErr, data);
		return;
	}
	ch->trackIntData = (int**) calloc (numberTracks + 1, sizeof(int*));
	if (ch->trackIntData == NULL)
	{
		npPostMsg ("err 2944 - malloc failed, CreateTracks", kNPmsgErr, data);
		return;
	}
	trackFloatDataScanner = ch->trackFloatData;
	trackUcharDataScanner = ch->trackUcharData;
	trackIntDataScanner = ch->trackIntData;

	for (i = 0; i < numberTracks; i++) 
	{
		// allocate and initialize the "float" track buffers
		trackBuffer = (char*) calloc (kNPtrackBufferSize, sizeof(float*));	
		if (trackBuffer == NULL)
		{
			npPostMsg ("err 2945 - malloc failed, CreateTracks", kNPmsgErr, data);
			return;
		}
		*(trackFloatDataScanner++) = (float*)trackBuffer;
		*(trackFloatDataScanner) = NULL;

		// allocate and initialize the "unsigned char" track buffers
		trackBuffer = (char*) calloc (kNPtrackBufferSize, sizeof(unsigned char*)); 
		if (trackBuffer == NULL)
		{
			npPostMsg ("err 2945 - malloc failed, CreateTracks", kNPmsgErr, data);
			return;
		}
		*(trackUcharDataScanner++) = (unsigned char*)trackBuffer;
		*(trackUcharDataScanner) = NULL;

		// allocate and initialize the "int" track buffers
		trackBuffer = (char*) calloc (kNPtrackBufferSize, sizeof(int*)); 
		if (trackBuffer == NULL)
		{
			npPostMsg ("err 2945 - malloc failed, CreateTracks", kNPmsgErr, data);
			return;
		}
		*(trackIntDataScanner++) = (int*)trackBuffer;
		*(trackIntDataScanner) = NULL;
	}
}

//------------------------------------------------------------------------------
void SaveHeader(int offset, char* header, void *dataRef)
{
	pData data = (pData) dataRef;
	char* headerCopy = NULL;
	
	headerCopy = (char*) malloc (strlen(header) + 1);
	if (headerCopy == NULL)
	{
		npPostMsg ("err 2946 - malloc failed, SaveHeader", kNPmsgErr, data);
		return;
	}

	strcpy (headerCopy, header);
	*(data->io.ch.trackNames + offset) = headerCopy;
	offset++;
	*(data->io.ch.trackNames + offset) = NULL;

	//printf("Just saved header %s, now have %d headers\n", header, offset);
	// If we've reached limit, need to re-allocate both lists
}

//------------------------------------------------------------------------------
int SaveHeaders(char** headers, void *dataRef)
{
	// NULL terminated list of pointers
	int offset = 0;

	while (*headers != NULL)
		SaveHeader (offset++, *headers++, dataRef);

	return offset;
}

// saves data for each track into the different track data buffers, saves as all data types
// so same track can be mapped to any data type
//------------------------------------------------------------------------------
bool npChSaveTrackData(int numberTracks, int offset, float** trackData, unsigned char** trackUcharData, int** trackIntData, char** buffer)
{
	int i = 0;
	float* singleFloatTrack = NULL;
	unsigned char* singleUcharTrack = NULL;
	int* singleIntTrack = NULL;
	int bufferAsInt;

	if (buffer == NULL) return false;

	for (i = 0; i < numberTracks; i++, buffer++) 
	{
		bufferAsInt = atoi (*buffer);

		// store as a "float" track
		singleFloatTrack = *(trackData + i);
		*(singleFloatTrack + offset) = (float)atof(*buffer);

		// store as an "unsigned char" track
		singleUcharTrack = *(trackUcharData + i);
		*(singleUcharTrack + offset) = (unsigned char)bufferAsInt;

		// store as an "int" track
		singleIntTrack = *(trackIntData + i);
		*(singleIntTrack + offset) = bufferAsInt;
	}

	return true;
}
/////////////////////////////////
// end of support functions
/////////////////////////////////


//init variables, allocate RAM
//------------------------------------------------------------------------------
void npInitCh (void* dataRef)
{
	pData data = (pData) dataRef;
	pNPch ch = &data->io.ch;
	int i = 0;

	ch->xrefIndex = 0;
	ch->setNumberToLoad = 1;  // default is 1, example data for this set is part of project

	for (i = 0; i < kNPmaxTrackToAttributeMappings; i++)
	{
		ch->xref[i].attributeName = NULL;
		ch->xref[i].trackOffset = 0;
		ch->xref[i].channelNumber = 0;
	}

	ch->updateData = true;
	ch->channelReadIndex = kNPtrackBufferSize - 1;
	ch->channelWriteIndex = 0;
	ch->channelSampleRate = 60;	//zz-JJ needs to be a float
	ch->channelDataSize = 1;
				// wait a year before spontaneously starting, 1000*60*60*24*365;
	ch->channelNextDataChangeTime = clock() + 31536000000;
	ch->numberTracks = 0;
							// +1 for NULL pointer indicating end of list
	ch->trackNames = (char**) malloc(sizeof(char*) * (kNPmaxTracks+1));
	if (ch->trackNames == NULL)
		npPostMsg ("err 7657 - malloc failed, npInitCh", kNPmsgErr, data);

	npInitPropertyTracks (&ch->floatTracks);
	npInitPropertyTracks (&ch->ucharTracks);
	npInitPropertyTracks (&ch->intTracks);

	ch->trackFloatData = NULL;
	ch->trackUcharData = NULL;
	ch->trackIntData = NULL;

	ch->pause = false;
}

//------------------------------------------------------------------------------
void npCloseCh (void* dataRef)		//clean-up
{
	return;
}

//------------------------------------------------------------------------------
void DumpChannel(char* str, pNPch ch)
{
	printf("%s\n", str);
	printf(" channelNextDataChangeTime: %d\n", ch->channelNextDataChangeTime);
	printf(" channelReadIndex: %d\n", ch->channelReadIndex);
	printf(" channelSampleRate: %d\n", ch->channelSampleRate);
	printf(" channelWriteIndex: %d\n", ch->channelWriteIndex);
	//printf(" : %d\n", channel->nodePropertyMemoryLocations);
	//printf(" : %d\n", channel->nodePropertyMemoryNextLocation);
	printf(" numberTracks: %d\n", ch->numberTracks);
	printf(" propertyFloatTrackIndex: %d\n", ch->floatTracks.propertyTrackIndex);
	//printf(" : %d\n", channel->propertyTracks);
	//printf(" : %d\n", channel->trackData);
	//printf(" : %d\n", channel->trackNames);
}

//
//  Check if there is a valid data source for reading track data
//
bool npChHasValidDataSource( pNPtrackDataSource trackDataSource )
{
	if (trackDataSource->dataSourceType == kDataFromFile) {
		return trackDataSource->trackFP != NULL;
	}
	else if (trackDataSource->dataSourceType == kDataFromUDP) {
		return true;  // we always listen on a UDP port
	}
	else {
		return false;
	}
}

//
//  Close connection track data source has to outside world
//
void npCloseDataSource( pNPtrackDataSource trackDataSource, void* dataRef )
{
	if (trackDataSource->dataSourceType == kDataFromFile)
	{
		npFileClose( trackDataSource->trackFP, dataRef );
		trackDataSource->trackFP = NULL;
	}
}

//
//  Reopen a data source
//
void npReopenDataSource( pNPch ch, void* dataRef )
{
	pNPtrackDataSource trackDataSource = &ch->trackDataSource;
	if (trackDataSource->dataSourceType == kDataFromFile )
	{
		trackDataSource->trackFP = npFileOpen( trackDataSource->fullFilePath, "r", dataRef );

		// read and ignore the initial header line
		npChTrackTokens( ch, dataRef );
	}
}


// 
//  Must run in another thread, reads data into circular buffer
//
// Currently have two options for behavior when we reach the end:
//  - pause
//  - loop
//
// If threads are supported this function will be called in its own thread.  
// In that case, it will implement circular buffer as part of loading data -- 
// waiting for data to be read before overwriting it.
// With looping with files larger than the circular buffer size, the large files
// are re-opened and re-read. This allows looping to occur with any size file.
//
// If threads are not supported, the buffer loading stops as soon as the buffer
// is full. The remaining data is never loaded.
// Looping occurs only on the data loaded into the buffer.
//------------------------------------------------------------------------------
bool threadRunning = false;
void npCircularBufferLoadLoop (void* dataRef)
{
	pData data							= (pData)dataRef;
	pNPch ch							= &data->io.ch;
	pNPtrackDataSource trackDataSource	= &ch->trackDataSource;
	char** tokens						= NULL;

	ch->numberTracks = SaveHeaders ( npChReadTrackTokens(ch, data), data);
	CreateTracks (ch->numberTracks, data);

	// reset read/write index on reload
	ch->channelReadIndex = kNPtrackBufferSize - 1;
	ch->channelWriteIndex = 0;
	while ( npChHasValidDataSource( trackDataSource ))
	{
		while ( true )
		{
			tokens = npChReadTrackTokens(ch, data);
			if (tokens == NULL) break;

			npChSaveTrackData( ch->numberTracks, ch->channelWriteIndex++, 
				ch->trackFloatData, ch->trackUcharData, ch->trackIntData, tokens );
			if (ch->channelWriteIndex > ch->channelDataSize) 
				ch->channelDataSize = ch->channelWriteIndex;

			// circular buffer requires thread, otherwise, we are totally done
			if (nposSupportsAntzThreads()) 
			{
				if (ch->channelWriteIndex == kNPtrackBufferSize)
					ch->channelWriteIndex = 0;
				//printf("channelReadIndex %d, channelWriteIndex %d, channelDataSize now %d\n", data->io.ch.channelReadIndex, data->io.ch.channelWriteIndex, data->io.ch.channelDataSize);
				
				 // wait for data to be read before overwriting
				while (ch->channelWriteIndex == ch->channelReadIndex) 
				{
					//printf ("Waiting for data to be read..\n");
					nposSleep(0.1);
				}
			}		// without threads, break out
			else if (ch->channelWriteIndex == kNPtrackBufferSize)
			{
				break;
			}
		}
		npCloseDataSource (trackDataSource, dataRef);
		//printf("ALL DONE!\n");
		// if we are looping, and the file was too large to fit entirely into track memory,
		// re-read the file
		if ( loopAtEndOfTrack 
			&& ch->channelDataSize == kNPtrackBufferSize
//JJ-zz			&& nposSupportsAntzThreads()
			&& (ch->trackDataSource.dataSourceType == kDataFromFile)) 
		{
			printf("LOOPING AT END OF TRACK\n");
			data->io.ch.channelReadIndex = kNPtrackBufferSize - 1;
			data->io.ch.channelWriteIndex = 0;
			npReopenDataSource( ch, dataRef );
		}
	}
	threadRunning = false;
	npPostMsg ("..npCircularBufferLoadLoop exit", kNPmsgCtrl, data);
	nposEndThread();
}

//------------------------------------------------------------------------------
// 
//  Convert a list of text tokens into a mapping from a channel track to a node attribute
//
//------------------------------------------------------------------------------
void npMapTokens( char** tokens, pData data )
{
	char** tokenScanner = tokens;
	while (*tokenScanner) printf("token %s\n", *tokenScanner++);
	data->io.ch.xref[data->io.ch.xrefIndex].attributeName = npNewStrcpy(*(tokens + kNPattributeNameOffset), data);
	data->io.ch.xref[data->io.ch.xrefIndex].trackOffset = atoi(*(tokens + kNPtrackOffset));
	data->io.ch.xref[data->io.ch.xrefIndex].channelNumber = atoi(*(tokens + kNPchannelOffset));

	printf("Mapped track offset %d to attribute %s\n", 
		data->io.ch.xref[data->io.ch.xrefIndex].trackOffset,	data->io.ch.xref[data->io.ch.xrefIndex].attributeName);

	// once we hit the limit of mappings, we just keep overwriting the final mapping entry
	// JJ - need to handle this case better
	if (data->io.ch.xrefIndex < kNPmaxTrackToAttributeMappings)
		data->io.ch.xrefIndex++;
}

//open the file and read contents into a buffer
//determine the file type, and call either npChMapImport() or npChDataImport()
//------------------------------------------------------------------------------
//
// The channel is the data pipe between the tracks and the nodes
//
// Expected format is 7 header columns:
//
//    id,track_set_id,track_column,attribute_name,id1,id2,id3
//
// Currently only using track_column, attribute_name
//
//    track_column is the column offset in the data loaded with npLoadChTracks
//    attribute_name is the name of the node property
//
// The data loaded here goes into xref[] array of NPxref.  
// Each entry in this array mays a track offset to a Node attribute name
//------------------------------------------------------------------------------
void npLoadChMap (const char* filePath, void* dataRef)
{
	pData data = (pData) dataRef;
	pNPch ch = &data->io.ch;
	FILE* file;
	char** tokens;
	char** tokenScanner;
	int tokenCount = 0;
	int i = 0;

	printf("npLoadChMap %s\n", filePath);
	file = npFileOpen (filePath, "r", data);
	ch->trackDataSource.channelFP = file;
	if (file != NULL) 
	{
		// read the header line, which is only used to verify that the expected 7 header entries are present
		tokens = tokenScanner = npChMapTokens (ch, dataRef);
		while (*tokenScanner++) tokenCount++;		//count tokens
		if (tokenCount != 7) 	// expecting 7 columns, if not then err, abort
		{ 
			npPostMsg ("err 4588 - Invalid ANTzChMap file", kNPmsgErr, data);
			npFileClose(file, dataRef);
			return;
		}

		// loop to read all the data lines, each line maps to an entry in the "io.ch.xref" array
		while ((tokens = npChMapTokens(ch, dataRef)) != NULL) 
		{
			npMapTokens( tokens, data );
		}
		//printf("Closing file\n");
		npFileClose(file, dataRef);
	}
}


//------------------------------------------------------------------------------
//
// The channel is the data pipe between the tracks and the nodes
//
// Format is:
//
// id,track_set_id,track_column,attribute_name,id1,id2,id3
//
// For now, only using track_column, attribute_name
//
// track_column is the column offset in the data loaded with npLoadChTracks
//  
// data loaded here goes into xref[] array of NPxref that map a track offset to
// an attribute name
//------------------------------------------------------------------------------
void npSaveChMap (const char* filePath, void* dataRef)
{
	pData data = (pData) dataRef;
	FILE* file;
	int i = 0;

	file = npFileOpen (filePath, "w", data);

	if (file != NULL) 
	{
		fprintf(file, "%s\n", kNPchannelMapHeader);
		for (i = 0; i < data->io.ch.xrefIndex; i++)
		{
			fprintf(file, "%d,%d,%d,%s,%d,%d,%d\n", i+1, data->io.ch.xref[i].channelNumber, 
				data->io.ch.xref[i].trackOffset, data->io.ch.xref[i].attributeName, 0, 0, i+1);
		}
		npFileClose(file, dataRef);
	}
}

void npActivateDataLoadThread( void* dataRef )
{
	pData data = (pData)dataRef;
	if (!threadRunning) {
		threadRunning = true;
		npPostMsg ("..calling nposBeginThread.. to actually load the data", kNPmsgDebug, data);
		nposBeginThread (npCircularBufferLoadLoop, data);
		npPostMsg ("..track loading thread has started", kNPmsgCtrl, data);
	}
}

// The track file is a text form of multiple tracks
// The first CSV line is column headers, free form text, not used by program,
// can be used to describe track
// Each column of data is a single track
//------------------------------------------------------------------------------
void npLoadChTracks (const char* filePath, void* dataRef)
{
	pData data = (pData) dataRef;
	pNPch ch = &data->io.ch;
	pNPtrackDataSource trackDataSource = &ch->trackDataSource;
	FILE* file = NULL;
	float** trackData = NULL;

	//char msg[kNPmaxPath];
	//sprintf(msg, "npLoadChTracks %s\n", filePath);
	//npPostMsg(msg, kNPmsgDebug, data);

	// convention following to start is to always have a Track data file that lists all the incoming columns
	// locally.  A command track then redirects the read to another data source.
	file = npFileOpen (filePath, "r", data);

	if (file != NULL)
	{
		ch->trackDataSource.fullFilePath = npNewStrcpy (filePath, data);				//zz-JJ
		ch->trackDataSource.trackFP = file;
		ch->trackDataSource.dataSourceType = kDataFromFile;
	}
	else
	{
		ch->trackDataSource.dataSourceType = kDataFromUDP;  // for now, if file not found, expect data from UDP
		npPostMsg ("..going to be loading data from udp", kNPmsgDebug, data);
	}

	// data loaded in another thread to allow circular buffer, new thread is
	// for filling the buffer from the file this thread reads from circular
	// buffer, the new thread we are creating here writes to the circular
	// buffer both reading and writing threads follow boundary convention 
	// to implement circular buffer
	// 
	// circular buffer conventions:
	// writeIndex != readIndex means data can be written at the current index
	// writeIndex == readIndex means the circular buffer is full, nothing more
	//    can be written into it
	//  ((readIndex + 1)%circularBufferSize) == writeIndex means there is no
	//    data available to read
	//
	npActivateDataLoadThread( data );
}


void* npChReadLineFromFile(char* bufferStart, int maxLength, pNPtrackDataSource trackDataSource, bool channelRead)
{
	int cval = 0;
	int i = 0;
	char *result = bufferStart;
	FILE* fp = channelRead ? trackDataSource->channelFP : trackDataSource->trackFP;

	// ignore blank lines, allow any combination of \n \r at end of line
	*result = 0;
	for (i = 0; i < maxLength; i++) 
	{
		cval = fgetc(fp);
		if (cval == -1) 
		{
			break;
		} 
		else 
		{
			if ((cval == '\n') || (cval == '\r')) 
			{
				// if we have a newline character, and have read some other characters into buffer, we are done
				if (result != bufferStart) break;
				// otherwise we just ignore it
			} 
			else if ((cval != '"') && (cval != '\''))
			{
				*result++ = (char)cval;
				*result = 0;
			}
		}
	}
	if (cval == -1) return NULL;
	return bufferStart;
}


// 
// Read a line of text from the track data source
//
void* npChReadLine( char* bufferStart, int maxLength, pNPtrackDataSource trackDataSource, bool channelRead, void* dataRef )
{
	pData data = (pData) dataRef;

	// for now, just trying to get track data only from UDP, need to re-factor away "channelRead" 
	int dataSourceType = channelRead ? kDataFromFile : trackDataSource->dataSourceType;
	*bufferStart = 0;
	switch( dataSourceType )
	{
	case kDataFromFile:
		return npChReadLineFromFile( bufferStart, maxLength, trackDataSource, channelRead );
	case kDataFromUDP:
		if (channelRead) {
			npPostMsg("channelRead from UDP!!!", kNPmsgDebug, dataRef);
		}
		else {
			//npPostMsg("trackRead from UDP!!!", kNPmsgDebug, dataRef);
		}
		return npOscReadLine( bufferStart, maxLength, data->io.osc.list[0].id); //JJ-zz data->io.oscListener.oscPackIdx );
	default:
		return NULL;
	}
}

// returns list of null terminated tokens from file line containing comma separated values
// ---------------------------------------------------------------------------------------
char** npChReadTokens (pNPch ch, bool isChannelRead, void* dataRef)
{
	int i = 0;
	char* bufferScan = &ch->channelFileTokenBuffer[0];

	//read a chunk of the file
	if (npChReadLine(&ch->channelFileTokenBuffer[0], kNPmaxLineLength, &ch->trackDataSource, isChannelRead, dataRef) == NULL)
		return NULL;

	ch->channelFileTokenBuffer[strlen(ch->channelFileTokenBuffer) - 1] = 0;
	ch->channelFileTokens[i++] = bufferScan;
	ch->channelFileTokens[i] = NULL;

	while (*bufferScan) 
	{
		if (*bufferScan == ',')
		{
			*bufferScan = 0;
			ch->channelFileTokens[i++] = bufferScan + 1;
			ch->channelFileTokens[i] = NULL;
		} 
		bufferScan++;
	}

	return &ch->channelFileTokens[0];
}

char** npChMapTokens (pNPch ch, void* dataRef)
{
	return npChReadTokens( ch, true, dataRef );
}

char** npChTrackTokens (pNPch ch, void* dataRef)
{
	return npChReadTokens( ch, false, dataRef );
}

char** npChReadTrackTokens (pNPch ch, void* dataRef)
{
	pData data = (pData) dataRef;

	int i = 0;
	char* bufferScan = &ch->trackFileTokenBuffer[0];

	//read a chunk of the file
	if (npChReadLine(&ch->trackFileTokenBuffer[0], kNPmaxLineLength, &ch->trackDataSource, false, dataRef) == NULL) {
		return NULL;
	}
	ch->trackFileTokenBuffer[strlen(ch->trackFileTokenBuffer) - 1] = 0;
	ch->trackFileTokens[i++] = bufferScan;
	ch->trackFileTokens[i] = NULL;

	while (*bufferScan) 
	{
		if (*bufferScan == ',') 
		{
			*bufferScan = 0;
			ch->trackFileTokens[i++] = bufferScan+1;
			ch->trackFileTokens[i] = NULL;
		} 
		bufferScan++;
	}

	return &ch->trackFileTokens[0];
}

// load track metadata such as name, sample rate, data type, time-stamp, etc...
//------------------------------------------------------------------------------
void npLoadChMeta (const char* filePath, void* dataRef)
{
	return;
}

//passed in a buffer containing the entire contents of the antzchmap000x.csv
//parses the CSV formatted contents and builds the channel map list
//merging by using multiple imports allowed
//------------------------------------------------------------------------------
void npChMapImport (char* buffer, int size, void* dataRef)
{
	return;
}

//------------------------------------------------------------------------------
void npChDataImport (char* buffer, int size, void* dataRef)	//antzch0001.csv
{
	return;
}

//exports entire channel map to the buffer in CSV format
//the function that calls this is responsible for freeing the buffer when done
//typically would be written to a CSV file or DB then the buffer is freed
//------------------------------------------------------------------------------
void npChMapExport (char* buffer, int *size, void* dataRef)
{
	return;
}

//------------------------------------------------------------------------------
void npChDataExport (char* buffer, int *size, void* dataRef)
{
	return;
}

//all functions can be used to read/write to or from a CSV file or DB
//separate out the buffer to make this possible...
//add or remove ch attribute track pairs
//------------------------------------------------------------------------------
void npChMapAttributeAdd (int channel, int attribute, int track, void* dataRef)
{
	return;
}

//------------------------------------------------------------------------------
void npChMapAttributeDelete (int channel, int attribute, void* dataRef)
{
	return;
}

//------------------------------------------------------------------------------
void npNewChannel (int channel, void* dataRef)
{
	return;
}

//------------------------------------------------------------------------------
void npDeleteChannel (int channel, void* dataRef)
{
	return;
}

// node->chInputID specifies the channel to use for attribute mapping
// of the track-attrib pairs added to the ptr-ptr list
// node->chInputID = 0 is the null channel, results in no channel mapping
//------------------------------------------------------------------------------
void npChSubscribeNode (pNPnode node, void* dataRef)
{
	pData data = (pData) dataRef;
	pNPch ch = &data->io.ch;
	int i = 0;
	static char msg[100];
	sprintf(msg, "..calling npChSubscribeNode, id %d", node->id);
	npPostMsg (msg, kNPmsgDebug, dataRef);
	if (node->chInputID == 0)	{//zz
		npPostMsg ("..returning, chInputID is 0", kNPmsgDebug, dataRef);
		return;
	}

	ch->channelNextDataChangeTime = clock();
	// ch->channelReadIndex = 0; no need to reset this, keep reading at current position

	for (i = 0; i < ch->xrefIndex; i++) {
		if (node->chInputID == ch->xref[i].channelNumber) {
			sprintf(msg, "node %d uses channel %d, mapping attribute %s, using trackOffset %d", node->id, node->chInputID, ch->xref[i].attributeName, ch->xref[i].trackOffset);
			npPostMsg(msg, kNPmsgDebug, dataRef);
			MapTrackOffset(ch->xref[i].trackOffset, node, ch->xref[i].attributeName, data);
		}
	}

	printf("npChSubscribeNode, list of node property memory address goes from 0x%x to 0%x\n",
		&ch->ucharTracks.nodePropertyMemoryLocations[0],ch->ucharTracks.nodePropertyMemoryNextLocation);
}

//---------------------------------------------------------------
// get the maximum and minimum channel numbers
int npChGetMaxChannel (pNPch ch)
{
	int result = 0;
	int i = 0;
	for (i = 0; i < ch->xrefIndex; i++) 
	{
		if (ch->xref[i].channelNumber > result) 
		{
			result = ch->xref[i].channelNumber;
		}
	}
	return result;
}

int npChGetMinChannel (pNPch ch)
{
	int result = 0;
	int i = 0;
	for (i = 0; i < ch->xrefIndex; i++) 
	{
		if ((ch->xref[i].channelNumber < result) || (result == 0)) 
		{
			result = ch->xref[i].channelNumber;
		}
	}
	return result;
}

//-----------------------------------------------------
// cycle backwards through the channel numbers present in the most recently
// loaded ChMap file.  The current channel number is a parameter, returns
// the next lower channel number (if there is one), or the greatest number channel
// number (if the starting channelNumber is zero), or zero (if channelNumber we
// start with already is the lowest channelNumber)
//
int npChGetChannelBefore(pNPch ch, int channelNumber) 
{
	int result = 0;
	int currentMaxChannelNumber = channelNumber;
	int i;
	int channelNumberCandidate;

	if (channelNumber == 0) 
	{
		return npChGetMaxChannel(ch);
	} 
	else 
	{
		for (i = 0; i < ch->xrefIndex; i++) 
		{
			channelNumberCandidate = ch->xref[i].channelNumber;
			if (channelNumberCandidate < currentMaxChannelNumber) 
			{
				if (result == 0) // this is first candidate, save it
				{
					result = channelNumberCandidate;
				}
				// if the channelNumberCandidate is closer to starting channelNumber, it becomes the new result
				else if ((channelNumber - result) > (channelNumber - channelNumberCandidate))
				{	
					result = channelNumberCandidate;
				}
			}
		}
		return result;
	}
}

//-----------------------------------------------------
// cycle forward through the channel numbers present in the most recently
// loaded ChMap file.  The current channel number is a parameter, returns
// the next higher channel number (if there is one), or zero (if there is 
// no channel number higher than the current one)
//
int npChGetChannelAfter(pNPch ch, int channelNumber) 
{
	int result = -1;  // -1 means "not found"
	int i = 0;
	int channelNumberCandidate;

	for (i = 0; i < ch->xrefIndex; i++)
	{
		// if we have a "before" candidate
		channelNumberCandidate = ch->xref[i].channelNumber;
		if (channelNumberCandidate > channelNumber)
		{
			if (result == -1) 			// this is first candidate, save it
				result = channelNumberCandidate;
			else 
			{
				// check if this candidate is closer to the channelNumber than the previous candidate
				// replace if result if this is the case
				if ((result - channelNumber) > (channelNumberCandidate - channelNumber))
					result = channelNumberCandidate;
			}
		}
	}
	return (result == -1) ? 0 : result;
}


void npChRemoveMemoryReferences (pNPnodePropertyTracks propertyTracks, void* boundaryStart, void* boundaryEnd)
{
	int scanOffset = 0;
	void** memoryScanner = &propertyTracks->nodePropertyMemoryLocations[0];
	while (memoryScanner != propertyTracks->nodePropertyMemoryNextLocation) 
	{
		if (( *memoryScanner >= boundaryStart ) && ( *memoryScanner < boundaryEnd )) 
		{
			// we are losing a mapping, so we shift "next write" locations one to the left
			// the entry that is being lost is overwritten with the current end, then
			// we clear out values in current "next write" location

			// shift left
			propertyTracks->nodePropertyMemoryNextLocation--;
			propertyTracks->propertyTrackIndex--;

			// overwrite current entry
			*memoryScanner = *(propertyTracks->nodePropertyMemoryNextLocation);
			propertyTracks->propertyTracks[scanOffset] = propertyTracks->propertyTracks[propertyTracks->propertyTrackIndex];

			// clear out end entry
			*(propertyTracks->nodePropertyMemoryNextLocation) = NULL;
			propertyTracks->propertyTracks[propertyTracks->propertyTrackIndex] = 0;
		}
		else 
		{
			memoryScanner++;
			scanOffset++;
		}
	}
}

//removes any pointers in the list associated with the node passed in
//used when changing channels which first removes the node, 
//then changes the chID and calls npChSubscribeNode to setup the new channel ptrs
//also called by npNodeDelete()
//------------------------------------------------------------------------------
void npChRemoveNode (pNPnode node, void* dataRef)
{
	pData data = (pData) dataRef;
	pNPch ch = &data->io.ch;
	char* nodeStart = (char*)node;
	char* nodeEnd = (nodeStart + sizeof(struct NPnode));

	npChRemoveMemoryReferences (&ch->floatTracks, nodeStart, nodeEnd);
	npChRemoveMemoryReferences (&ch->ucharTracks, nodeStart, nodeEnd);
	npChRemoveMemoryReferences (&ch->intTracks, nodeStart, nodeEnd);

	//printf("npChRemoveNode, nodePropertyMemoryScanner starts at 0x%x, data->io.ch.nodePropertyMemoryNextLocation is 0%x\n",
	//	&ch->floatTracks.nodePropertyMemoryLocations[0], ch->floatTracks.nodePropertyMemoryNextLocation);
}


//------------------------------------------------------------------------------
// update Node properties using track data
// to test from antz release: 1, P, > 
int counter = 0;
void npUpdateNodes(void *dataRef)
{
	pData data = (pData) dataRef;
	pNPch ch = &data->io.ch;
	float **nodePropertyFloatMemoryScanner = NULL;
	float *trackDataAsFloat = NULL;
	unsigned char **nodePropertyUcharMemoryScanner = NULL;
	unsigned char* ucharTrack = NULL;
	int **nodePropertyIntMemoryScanner = NULL;
	int* intTrack = NULL;
	int channelScanIdx = 0;

	if (ch->updateData)
	{
		ch->updateData = false;

		// update all the float properties
		channelScanIdx = 0;
		nodePropertyFloatMemoryScanner = (float**)&ch->floatTracks.nodePropertyMemoryLocations[0];

		while (nodePropertyFloatMemoryScanner != (float**)ch->floatTracks.nodePropertyMemoryNextLocation)
		{
			trackDataAsFloat = *(ch->trackFloatData + ch->floatTracks.propertyTracks[channelScanIdx]);
			**nodePropertyFloatMemoryScanner++ = *(trackDataAsFloat + ch->channelReadIndex);
			channelScanIdx++;
			if (ch->floatTracks.nodePropertyMemoryNextLocation == &ch->floatTracks.nodePropertyMemoryLocations[0]) break; // if reset while we are in loop
		}

		// update all the unsigned char properties (like r,g,b,a)
		channelScanIdx = 0;
		nodePropertyUcharMemoryScanner = (unsigned char**)&ch->ucharTracks.nodePropertyMemoryLocations[0];
		while (nodePropertyUcharMemoryScanner != (unsigned char**)ch->ucharTracks.nodePropertyMemoryNextLocation) 
		{
			ucharTrack = *(ch->trackUcharData + ch->ucharTracks.propertyTracks[channelScanIdx]);
			**nodePropertyUcharMemoryScanner++ = *(ucharTrack + ch->channelReadIndex);
			channelScanIdx++;
			if (ch->ucharTracks.nodePropertyMemoryNextLocation == &ch->ucharTracks.nodePropertyMemoryLocations[0]) break; // if reset while we are in loop
		}

		// update all the "int" properties (like tagMode)
		channelScanIdx = 0;
		nodePropertyIntMemoryScanner = (int**)&ch->intTracks.nodePropertyMemoryLocations[0];
		while (nodePropertyIntMemoryScanner != (int**)ch->intTracks.nodePropertyMemoryNextLocation) 
		{
			intTrack = *(ch->trackIntData + ch->intTracks.propertyTracks[channelScanIdx]);
			**nodePropertyIntMemoryScanner++ = *(intTrack + ch->channelReadIndex);
			channelScanIdx++;
			if (ch->intTracks.nodePropertyMemoryNextLocation == &ch->intTracks.nodePropertyMemoryLocations[0]) break; // if reset while we are in loop
		}
	}
}

//(un)freeze a node in the list (or should we just add and del..) debug zz

//typically called once per cycle
//1st, update new input stream data, copy the last cycle into the track buffers
//2nd, update/calculate logical tracks, they may use raw tracks as inputs
//3rd, update nodes using track to node-attrib ptr lists
//lastly, output streams, such as live IO devices buffers or to a file
//------------------------------------------------------------------------------
void npUpdateCh (void* dataRef)
{
	pData data = (pData) dataRef;
	pNPch ch = &data->io.ch;
	clock_t now = 0;
	int nextReadIndex = 0;

	now = clock();

	if (data->io.ch.pause) return;  // nothing to update if we have paused
	if (now > ch->channelNextDataChangeTime)								//zz debug, add more precise timing
	{
		//printf("time for update... channelReadIndex %d, channelWriteIndex %d, channelDataSize %d\n", data->io.ch.channelReadIndex, data->io.ch.channelWriteIndex, data->io.ch.channelDataSize);
		ch->channelNextDataChangeTime += ch->channelSampleRate;
		nextReadIndex = (ch->channelReadIndex + 1) % (ch->channelDataSize);

		// if we have cycled back to beginning, and that behavior was not specified, there is no data so we do nothing
		if (( nextReadIndex == 0 ) && !loopAtEndOfTrack ) return;

		// if there is data to read, update the read index, and set the flag
		// that causes the updates
		if (nextReadIndex != ch->channelWriteIndex)
		{
			ch->updateData = true;
			ch->channelReadIndex = nextReadIndex;
//zz-JJ		
			printf("Reading %d, channelWriteIndex %d, channelDataSize %d\n", data->io.ch.channelReadIndex, data->io.ch.channelWriteIndex, data->io.ch.channelDataSize);
		}
	}
	npUpdateNodes(data);	//zz-JJ
}


//--- will add some stream functions
//multiple types DB, CSV, Network UDP & IP, LiveIO - audio, video, TNG3B, mouse
//updated with npUpdateCh() 
//------------------------------------------------------------------------------
void npChOpenInputStream (int needToFigureOut, void* dataRef)
{
	return;
}

//------------------------------------------------------------------------------
void npChCloseInputStream (int needToFigureOut, void* dataRef)
{
	return;
}

//------------------------------------------------------------------------------
void npChOpenOutputStream (int needToFigureOut, void* dataRef)
{
	return;
}

//------------------------------------------------------------------------------
void npChCloseOutStream (int needToFigureOut, void* dataRef)
{
	return;
}

//------------------------------------------------------------------------------
void npchPause (void* dataRef)	//zz-JJ
{
	pData data = (pData) dataRef;

	//toggle the pause state
	data->io.ch.pause = 1 - data->io.ch.pause;

	// reset next time to change, needed when resuming
	data->io.ch.channelNextDataChangeTime = clock();
}


//zz-JJ
//------------------------------------------------------------------------------
void npSyncChMap (void* dataRef)
{
	//delete entire channel pointer map list
	pData data = (pData) dataRef;
	npInitPropertyTracks (&data->io.ch.floatTracks);
	npInitPropertyTracks (&data->io.ch.ucharTracks);
	npInitPropertyTracks (&data->io.ch.intTracks);

	//add all nodes with channels back to the pointer map list
	npTraverseMap (npChSubscribeNode, dataRef);
}


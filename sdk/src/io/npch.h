/* -----------------------------------------------------------------------------
*
*  npch.h
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

#ifndef NPCH_H_
#define NPCH_H_

#include "../npdata.h"

#include "video/npvideo.h"		///< live video capture, record and playback

//JJ

//channel methods allow live IO or file based track data to animate the scene
//node attribute such as position, color, geometry, etc. are modulated with time
//the node->channel retains which node belongs to which channel
//antzchmap000x.csv contains the sets of channel mapping of attributes to tracks
//antzch000x.csv is the time based raw track data
//antzchmeta000x.csv is metadata that describes track sample rate, size, etc.
//------------------------------------------------------------------------------
void npInitCh (void* dataRef);
void npCloseCh (void* dataRef);

//load track metadata
void npLoadChMeta (const char* filePath, void* dataRef);

//read file into buffer then call map or track import based on file type
void npLoadChMap (const char* filePath, void* dataRef);
void npSaveChMap (const char* filePath, void* dataRef);

// for loading track data
void npLoadChTracks (const char* filePath, void* dataRef);

// support for reading in track data
// read in CSV line and convert it to list of string pointers one for each comma delimited entry
char** npChMapTokens (pNPch ch, void* dataRef);
void npMapTokens( char** tokens, pData data );
char** npChTrackTokens (pNPch ch, void* dataRef);
char** npChReadTrackTokens (pNPch ch, void* dataRef);		
// copy a string to a new memory location and return a pointer to the new copy
char* npNewStrcpy (const char* str, void* dataRef);							

// for cycling through channels
int npChGetChannelAfter(pNPch ch, int channelNumber);
int npChGetChannelBefore(pNPch ch, int channelNumber);

//parses CSV formatted buffer
void npChMapImport (char* buffer, int size, void* dataRef);
	//void npChDataImport (char* buffer, int size, void* dataRef);
void npChTrackImport (char* buffer, int size, void* dataRef);

//fills buffer with CSV formatted contents
void npChMapExport (char* buffer, int *size, void* dataRef);
	//void npChDataExport (char* buffer, int *size, void* dataRef);
void npChTrackExport (char* buffer, int size, void* dataRef);

//allows for changing the attrib to track mapping, allows a blank channel
void npChMapAttributeAdd (int channel, int attribute, int track, void* dataRef);
void npChMapAttributeDelete (int channel, int attribute, void* dataRef);

//void npNewChannel (int channel, void* dataRef);
int npChNew (void* dataRef);
void npChDelete (int channel, void* dataRef);

//subscribes the node to the channel node->channel...
void npChSubscribeNode (pNPnode node, void* dataRef);
void npChRemoveNode (pNPnode node, void* dataRef);

void npUpdateCh (void* dataRef);
void npUpdateNodes(void* dataRef);

//--- will add stream functions such as local IO and UDP
void npChOpenInputStream (int needToFigureOut, void* dataRef);
void npChCloseInputStream (int needToFigureOut, void* dataRef);
void npChOpenOutputStream (int needToFigureOut, void* dataRef);
void npChCloseOutStream (int needToFigureOut, void* dataRef);

void npchPause (void* dataRef);			//zz-JJ
void npSyncChMap (void* dataRef); 		//zz-JJ

//JJ-end

#endif


/* -----------------------------------------------------------------------------
*
*  npdata.h
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

#ifndef npdata_H_
#define npdata_H_


#ifdef __cplusplus
	extern "C" {
#endif

//add the namespace, debug zz

// #define NP_LINUX_
#define NP_MSW_
// #define NP_OSX_

#ifdef NP_MSW_
#define _USE_MATH_DEFINES
#endif

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>


#include "data/nptypes.h"
#include "data/npnode.h"
#include "data/npmap.h"


//--- Functions ----------------------------------------------------------------

void* npInitData (int argc, char** argv);		// inits global data map
void npCloseData();

void npUpdateData (void* dataRef);

void* npGetDataRef();

char* npGetDataTypeName (int type);
char* npGetDataTypeDesc (int type);

void npDataPreset (int preset, void* dataRef);

void npDataCameraPreset (int preset, NPnodePtr node, void* dataRef);

void npInitTextTag (pNPtextTag tag, void* dataRef);
void npInitDataRecordTag (pNPrecordTag recordTag, void* dataRef);

void npPostMsg (char* message, int type, void* dataRef);

void npInitPropertyTracks (pNPnodePropertyTracks propertyTracks); //JJ - added, is this right place?


#ifdef __cplusplus
	}
#endif
#endif


/* -----------------------------------------------------------------------------
*
*  npdata.h
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

#ifndef NPDATA_H_
#define NPDATA_H_

#ifdef __cplusplus
	extern "C" {
#endif

#ifdef _WIN32
	#define NP_MSW_
#else
	#define NP_POSIX_

	#ifdef __APPLE__
		#define NP_OSX_		//zz debug should be defined in makefile or project
	#else
		#define NP_LINUX_
	#endif
#endif

#ifdef NP_MSW_
	#define _USE_MATH_DEFINES
#endif

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>

#include "data/nptypes.h"
#include "data/npmath.h"
#include "data/npstr.h"
#include "data/npnode.h"
#include "data/npmap.h"


//--- Functions ----------------------------------------------------------------

void* npInitData( int argc, char** argv );		// inits global data map
void npCloseData( void* dataRef );
void npUpdateData( void* dataRef );

void npSystemConsoleHelp( int argc, char** argv );

void* npGetDataRef();

char* npGetDataTypeName( int type );
char* npGetDataTypeDesc( int type );

void npDataPreset( int preset, void* dataRef );

void npDataCameraPreset( int preset, pNPnode node, void* dataRef );

void npInitDataTags( pNPtags tags, void* dataRef );
void npInitTextTag( pNPtag tag, void* dataRef );
void npInitDataRecordTag( pNPrecordTag recordTag, void* dataRef );

void npPostMsg( char* message, int type, void* dataRef );

void npInitPropertyTracks( pNPnodePropertyTracks propertyTracks ); //JJ - added, is this right place


pNPdbHost npInitHostDB( void );	//zzd
pNPdatabase npInitDatabase( void );

#ifdef __cplusplus
	}
#endif
#endif


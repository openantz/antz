/* -----------------------------------------------------------------------------
*
*  npstr.h
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

#ifndef NPSTR_H_
#define NPSTR_H_

#include "../npdata.h"

/* -----------------------------------------------------------------------------
*  --- high performance string conversion ---					//zz debug, in progress
*
*  converts native C data structs to and from human readable strings
*  parses a defined table chunk size based on either item count or buffer size
*
*  fast string to number conversion and splits up chunks for parallel processing
*
*  supports bi-directional flow control between internal CPU process and IO
*  native string struct is formatted to be byte-for-byte compatible with MySQL
*
*  string struct is handedled by target format process for file, DB or network
*  target formats for import and export include CSV, JSON, XML, KML, OBJ...
*
*  data container and tree structure described separately
*
*  maps all elements of the global data context structure
*  organization of the data tree and element groups is handled by npmap
*
*  JSON formatted data and commands are packaged with OSC for UDP streaming 
*  GIS using GDAL for KML and OBJ files.
*
* --------------------------------------------------------------------------- */

//--- atoi and atof optimized for antz ----------------------------------------
int npatoi (const char *p);				//32bit int
float npatof (const char *p);			//32bit float

long npatol (const char *p);			//64bit long is akin to atoi
double npatod (const char *p);			//64bit double is akin to atof

//based on strtod, but sets buffer to endPtr + 1
//designed to increment past the comma to next field for CSV processing
//does not increment past characters other then comma, ie stops at line returns
int	npstrtoi (char** buffer);			//32bit int
float npstrtof (char** buffer);			//32bit float

long npstrtol( char** buffer );			//64bit long is akin to strtol
double npstrtod( char** buffer );		//64bit double is akin to strtod
	
// copy a string to a new memory location and return a pointer to the new copy
char* npNewStrcpy( const char* str, void* dataRef );							

char* npNextWhiteSpace( const char* buffer, int size );
char* npSkipWhiteSpace( const char* buffer, int size );
int npNextField( const char* buffer );	//zz64 need to change to long int
int npNextLine( const char* buffer );
int npNextLineLimit( const char* buffer, int size );
int npLastEOL( const char* buffer, int size );

int npStrDigitIsNext( const char* str, int size );

int npStrToRange( int* lower, int* upper, const char* str, int size );

#endif


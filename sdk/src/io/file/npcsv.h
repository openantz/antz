/* -----------------------------------------------------------------------------
*
*  npcsv.h
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

#ifndef NPCSV_H_
#define NPCSV_H_


#include "../../npdata.h"


//------------------------------------------------------------------------------
void npInitCSV (void* dataRef);
void npCloseCSV (void* dataRef);

// these functions wrap the standard C file methods on Linux, MSW and OSX
// opens or creates the file in the specified mode "rb+" read/write binary..
FILE* npOpenCSV (const char* fileName, const char* mode, void* dataRef);

FILE* npCSVNew (const char* fileName, const char* mode, void* dataRef);

FILE* npCSVDialog (const char* fileName, int dialogType, void* dataRef);

// if not saved then opens SaveAs dialog before reseting app
void npCloseCSV (void* dataRef);

// wrappers for fread(), fwrite() and rewind()
//void npReadFile (void* writeBuffer, int wordSize, int size, void* dataRef);
int npReadCSV (void* readBuffer, int elementSize, int elementCount, 
				 FILE* file, void* dataRef);
int npWriteCSV (const void* str, int wordSize, int size, FILE* file, void* dataRef);

// 
void npSaveCSV (FILE* file, void* dataRef);

void npImportCSV (FILE* file, int type, void* dataRef);
void npExportCSV (FILE* file, void* dataRef);

// file and directory management
void npGetAttribCSV (int size, void* dataRef);
void npSetAttribCSV (int size, void* dataRef);

void npTestCSV (void* fileName, int wordSize, int size, void* dataRef);

void npCSVtoMap (FILE* file, int type, void*dataRef);

//zz globals
int npMapGlobalsFromCSV (char* csvStr, int size, void* dataRef);
int npMapGlobalsToCSV (char* csvStr, int size, void* dataRef);

int npSaveMapToCSV (char* filePath, int mapTypeID, void* dataRef);
int npOpenGlobalsCSV (char* filePath, int wordSize, int size, void* dataRef);

void npMapTypeInit (void* dataRef);		//zz debug move thes to npmap.h etc...

pNPmapLink npMapAddressToMapTypeItem (const char* str, void* dataRef);

void* npMapAddressToPtr (const char* str, void* dataRef);

char* npMapTypeName( int mapType, void* dataRef );

#endif


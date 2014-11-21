/* -----------------------------------------------------------------------------
*
*  npmapfile.h
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
/* formats the data for import and export of CSV, TDT, XML, JSON, KML, OBJ, etc
*  we map all of our data into tables and describe the variable types separately
*  this is done using a JSON schema formatted to be compatible with export and
*  import to XML (office 2003 format), also generates native binary, CSV, TDT 
*  and JSON files. GIS using GDAL for KML and OBJ files.
*  also generates native binary for performance and support GIS using GDAL lib.
*  *note that this description is not fully implemented at this time
* --------------------------------------------------------------------------- */

#ifndef NPMAPFILE_H_
#define NPMAPFILE_H_

#include "../npdata.h"


void npInitMapFile (void* dataRef);
void npCloseMapFile (void* dataRef);

// these functions wrap the standard C file methods on Linux, MSW and OSX
// opens or creates the file in the specified mode "rb+" read/write binary..
FILE* npOpenMapFile (const char* fileName, const char* mode, void* dataRef);

FILE* npNewMapFile (const char* fileName, const char* mode, void* dataRef);

FILE* npDialogMapFile (const char* fileName, int dialogType, void* dataRef);

// if not saved then opens SaveAs dialog before reseting app
void npCloseMapFile (void* dataRef);

// wrappers for fread(), fwrite() and rewind()
//void npReadFile (void* writeBuffer, int wordSize, int size, void* dataRef);
int npReadMapFile (void* readBuffer, int elementSize, int elementCount, 
				 FILE* file, void* dataRef);
int npFileOpenAuto (const char* filePath, FILE* file, void* dataRef);

void npSaveMapFile (FILE* file, void* dataRef);

int npImportMapFile (FILE* file, int type, void* dataRef);
int npExportMapFile (FILE* file, int type, void* dataRef);

int npWriteMapHeader (char* bufferIndex, int count, int type, void* dataRef); // New, lde

// file and directory management
int npGetMapFile (char* buffer, int wordSize, int size, void* dataRef);

int npFileOpenMap (char* filePath, int wordSize, int size, void* dataRef);
int npFileSaveMap (char* filePath, int wordSize, int size, void* dataRef);

void npLoadMapFile (char* buffer, int wordSize, int size, void* dataRef);

int npWriteNode (const char* buffer, pNPnode node, int format, void* dataRef);

int npFileVersion (char* buffer, int wordSize, int size);



void npThreadFileOpenMap (void* dataRef);

//move to npmapstr and npcsv
//------------------------------------------------------------------------------
int npLoadTags (const char* buffer, int size, void* dataRef);
int npFileSaveTag (const char* filePath, int wordSize, int size, void* dataRef);

int npMapTraverseTree (char* buffer, pNPnode node, int format, void* dataRef);

//zzsql
int npMapTraverseTreeLL(char* buffer, int* index, pNPnode node, int format, void* dataRef);


#endif


/* -----------------------------------------------------------------------------
*
*  npmsw.h
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

#ifndef NPMSWFILE_H_
#define NPMSWFILE_H_

#include "../../npdata.h"


void npMSWGetAppPath (char* buffer, int* size);

void npMSWGetCWD (char* buffer, int* size);
void npMSWSetCWD (char* buffer);

void npMSWGetOpenFilePath (char* buffer, int* size, void* dataRef);
void npMSWSetOpenFilePath (char* buffer, void* dataRef);

FILE* npMSWFileDialog (const char* fileName, int dialogType, void* dataRef);

int npMSWShowCursor (int hide);
int npMSWSetCursorPos (int x, int y);

double npMSWGetTime (void);
void npMSWUpdateTime (void* dataRef);
void npMSWSleep (double seconds);

void npMSWTimeStampCSV (char* fileName);

void npMSWBeginThread (voidFnPtr vfp, void* dataRef);
void npMSWEndThread (void);


#endif


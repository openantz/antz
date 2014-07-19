/* -----------------------------------------------------------------------------
*
*  npos.h
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

#ifndef NPOS_H_
#define NPOS_H_

#include "../npdata.h"

//#ifdef NP_LINUX_						//zz debug may be able to remove these
	#include "linux/nplinux.h"
//#endif

//#ifdef NP_MSW_
	#include "msw/npmsw.h"
//#endif

//#ifdef NP_OSX_
	#include "osx/nposx.h"
//#endif

//------------------------------------------------------------------------------

void nposGetAppPath (char* buffer, int* size);

void nposGetCWD (char* buffer, int* size);
void nposSetCWD (char* buffer);

void nposGetOpenFilePath (char* buffer, int* size, void* dataRef);
void nposSetOpenFilePath (char* buffer, void* dataRef);

FILE* nposFileDialog (const char* fileName, int dialogType, void* dataRef);

int nposShowCursor (int hide);
int nposSetCursorPos (int x, int y);

double nposGetTime (void);
void nposUpdateTime (void* dataRef);
void nposSleep (double seconds);

void nposTimeStampCSV (char* fileName);

//JJ begin threading code here
void nposBeginThread (voidFnPtr vfp, void* dataRef);
void nposEndThread (void);
bool nposSupportsAntzThreads (void);
//JJ-end

#endif


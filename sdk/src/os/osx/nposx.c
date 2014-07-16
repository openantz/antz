/* -----------------------------------------------------------------------------
*
*  nposx.c
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

#include "nposx.h"


#ifdef __APPLE__

#include <sys/time.h>                // for gettimeofday()

// current working directory is OS specific
//-----------------------------------------------------------------------------
void nposxGetCWD (char* buffer, int* size)
{
	//using the linux method
}

//-----------------------------------------------------------------------------
void nposxUpdateTime (void* dataRef)
{
	//using the linux method
}

//-----------------------------------------------------------------------------
void nposxSleep (double seconds)
{
	return;																//debug zz
}

//-----------------------------------------------------------------------------
FILE* nposxFileDialog (const char* fileName, int dialogType, void* dataRef)
{
	return;																//debug zz
}

//-----------------------------------------------------------------------------
void nposxTimeStampCSV (char* fileName)
{
	return;																//debug zz
}

//-----------------------------------------------------------------------------
void nposxSetCursorPos (int x, int y)
{
	return;																//debug zz
}

//-----------------------------------------------------------------------------
void nposxShowCursor (int hide)
{
	return;																//debug zz
}


#endif


/* -----------------------------------------------------------------------------
*
*  nposx.c
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

#include "nposx.h"


#ifdef NP_OSX_

#include <sys/time.h>                // for gettimeofday()

// current working directory is OS specific
//-----------------------------------------------------------------------------
void npOSXGetCWD (char* buffer, int* size)
{
	//using the linux method
}

//-----------------------------------------------------------------------------
void npOSXUpdateTime (void* dataRef)
{
	//using the linux method
}

//-----------------------------------------------------------------------------
void npOSXSleep (double seconds)
{
	return;																//debug zz
}

//-----------------------------------------------------------------------------
FILE* npOSXFileDialog (const char* fileName, int dialogType, void* dataRef)
{
	return;																//debug zz
}

//-----------------------------------------------------------------------------
void npOSXTimeStampCSV (char* fileName)
{
	return;																//debug zz
}

//-----------------------------------------------------------------------------
void npOSXSetCursorPos (int x, int y)
{
	return;																//debug zz
}

//-----------------------------------------------------------------------------
void npOSXShowCursor (int hide)
{
	return;																//debug zz
}


#endif


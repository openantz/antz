/* -----------------------------------------------------------------------------
*
*  npos.c
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

#include "npos.h"



//-----------------------------------------------------------------------------
double nposGetTime (void)
{
#ifdef NP_LINUX_
		return npLinuxGetTime ();
#endif
#ifdef NP_MSW_
		return npMSWGetTime ();
#endif
#ifdef NP_OSX_
		return npLinuxGetTime ();
		//return npOSXGetTime ();	//OSX can often use the linux method
#endif
}

//-----------------------------------------------------------------------------
void nposUpdateTime (void* dataRef)
{
#ifdef NP_LINUX_
		npLinuxUpdateTime (dataRef);
#endif
#ifdef NP_MSW_
		npMSWUpdateTime (dataRef);
#endif
#ifdef NP_OSX_
		npLinuxUpdateTime (dataRef);
		//npOSXUpdateTime (dataRef);	//OSX can often use the linux method
#endif
}

//-----------------------------------------------------------------------------
void nposSleep (double seconds)
{
#ifdef NP_LINUX_
		npLinuxSleep (seconds);
#endif
#ifdef NP_MSW_
		npMSWSleep (seconds);
#endif
#ifdef NP_OSX_
		npLinuxSleep (seconds);
		//npOSXSleep (seconds);
#endif
}

//-----------------------------------------------------------------------------
void nposGetAppPath (char* buffer, int* size)
{
#ifdef NP_LINUX_
		npLinuxGetAppPath (buffer, size);
#endif
#ifdef NP_MSW_
		npMSWGetAppPath (buffer, size);
#endif
#ifdef NP_OSX_
		npLinuxGetAppPath (buffer, size);
//		npOSXGetAppPath (buffer, size);
#endif
}

//-----------------------------------------------------------------------------
void nposGetCWD (char* buffer, int* size)
{
#ifdef NP_LINUX_
		npLinuxGetCWD (buffer, size);
#endif
#ifdef NP_MSW_
		npMSWGetCWD (buffer, size);
#endif
#ifdef NP_OSX_
		npLinuxGetCWD (buffer, size);
//		npOSXGetCWD (buffer, size);
#endif
}

//-----------------------------------------------------------------------------
void nposSetCWD (char* buffer)
{
#ifdef NP_LINUX_
		npLinuxSetCWD (buffer);
#endif
#ifdef NP_MSW_
		npMSWSetCWD (buffer);
#endif
#ifdef NP_OSX_
		npLinuxSetCWD (buffer);
//		npOSXSetCWD (buffer);
#endif
}

//-----------------------------------------------------------------------------
void nposGetOpenFilePath (char* buffer, int* size, void* dataRef)
{
#ifdef NP_LINUX_
		npLinuxGetOpenFilePath (buffer, size, dataRef);
#endif
#ifdef NP_MSW_
		npMSWGetOpenFilePath (buffer, size, dataRef);
#endif
#ifdef NP_OSX_
		npLinuxGetOpenFilePath (buffer, size, dataRef);
//		npOSXGetOpenFilePath (buffer, size, dataRef);
#endif
}

//-----------------------------------------------------------------------------
void nposSetOpenFilePath (char* buffer, void* dataRef)
{
#ifdef NP_LINUX_
		npLinuxSetOpenFilePath (buffer, dataRef);
#endif
#ifdef NP_MSW_
		npMSWSetOpenFilePath (buffer, dataRef);
#endif
#ifdef NP_OSX_
		npLinuxSetOpenFilePath (buffer, dataRef);
//		npOSXSetOpenFilePath (buffer, dataRef);
#endif
}

//-----------------------------------------------------------------------------
FILE* nposFileDialog (const char* fileName, int dialogType, void* dataRef)
{
#ifdef NP_LINUX_
 return npLinuxFileDialog (fileName, dialogType, dataRef);
#endif
#ifdef NP_MSW_
 return npMSWFileDialog (fileName, dialogType, dataRef);
#endif
#ifdef NP_OSX_
 return npOSXGetFileDialog (fileName, dialogType, dataRef);
#endif
}

//-----------------------------------------------------------------------------
int nposSetCursorPos (int x, int y)
{
#ifdef NP_LINUX_
 return npLinuxSetCursorPos (x,y);
#endif
#ifdef NP_MSW_
 return npMSWSetCursorPos (x,y);
#endif
#ifdef NP_OSX_
 return npOSXSetCursorPos (x,y);
#endif
}

//-----------------------------------------------------------------------------
int nposShowCursor (int hide)
{
#ifdef NP_LINUX_
 return npLinuxShowCursor (hide);
#endif
#ifdef NP_MSW_
 return npMSWShowCursor (hide);
#endif
#ifdef NP_OSX_
 return npOSXShowCursor (hide);
#endif
}

//replace this with a get nposGetSystemTime(), format CSV in npfile,		debug zz
//-----------------------------------------------------------------------------
void nposTimeStampCSV (char* fileName)
{
#ifdef NP_LINUX_
		npLinuxTimeStampCSV (fileName);
#endif
#ifdef NP_MSW_
		npMSWTimeStampCSV (fileName);
#endif
#ifdef NP_OSX_
		npLinuxTimeStampCSV (fileName);
//		npOSXTimeStampCSV (fileName);
#endif
}


//-----------------------------------------------------------------------------
//zz-JJ
bool nposSupportsAntzThreads()
{
	#ifdef NP_LINUX_
		return false;
	#endif
	#ifdef NP_MSW_
		return true;
	#endif
	#ifdef NP_OSX_
		return false;
	#endif
}

//zz-JJ---------------------------------------------------------------------------
void nposBeginThread(voidFnPtr vfp, void *dataRef)
{
	#ifdef NP_LINUX_
		return; // implement POSIX threads
	#endif
	#ifdef NP_MSW_
		npMSWBeginThread (vfp, dataRef);
	#endif
	#ifdef NP_OSX_
		return; // implement POSIX threads
	#endif
}

//zz-JJ---------------------------------------------------------------------------
void nposEndThread()
{
	#ifdef NP_LINUX_
		return; // implement POSIX threads
	#endif
	#ifdef NP_MSW_
		npMSWEndThread();
	#endif
	#ifdef NP_OSX_
		return; // implement POSIX threads
	#endif
}
//JJ-end


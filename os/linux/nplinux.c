/* -----------------------------------------------------------------------------
*
*  nplinux.c
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

#include "nplinux.h"

#include "../../npdata.h"

#ifdef NP_LINUX_

#include <time.h>                // for gettimeofday()
// #include <unistd.h>			//debug zz

//-----------------------------------------------------------------------------
void signal_callback_handler(int signum)
{
  printf("Caught signal %d\n", signum);
  // Clean up and close stuff here

  // Terminate program
  exit(signum);
}

void npLinuxGetAppPath (char* buffer, int* size)
{
	static bool firstTimeCalled = true;
	static char appPath[kNPmaxPath];

	//if first time then set the app path using the CWD method
	//should replace with actual get app path OS function, zz debug
	if (firstTimeCalled)
	{
		npLinuxGetCWD (appPath, size);
		strcat (appPath, "/");
		firstTimeCalled = false;
	}
	
	//copy the app path to the passed in buffer
	strcpy (buffer, appPath);

	//set the length
	*size = strlen(buffer); 
}

// current working directory is OS specific
//-----------------------------------------------------------------------------
void npLinuxGetCWD (char* buffer, int* size)
{
	getcwd (buffer, size);
}

//-----------------------------------------------------------------------------
void npLinuxSetCWD (char* buffer)
{
	//	setcwd (buffer); // debug, voidptr1 build out
}

//-----------------------------------------------------------------------------
void npLinuxGetOpenFilePath (char* buffer, int* size, void* dataRef)
{
	getcwd (buffer, size);
}

//-----------------------------------------------------------------------------
void npLinuxSetOpenFilePath (char* buffer, void* dataRef)
{
	return;
}

//-----------------------------------------------------------------------------
double npLinuxGetTime (void)									//untested, debug zz
{
	double seconds;
    struct timeval tval;
	
	// get time of day in micro seconds
    gettimeofday(&tval, NULL);

	// convert us to seconds
	seconds = (double)tval.tv_sec + (double)tval.tv_usec / 1000000.0;

	return seconds;
}

//-----------------------------------------------------------------------------
void npLinuxUpdateTime (void* dataRef)										//debug zz
{
    struct timeval tval;
	pData data = (pData) dataRef;
	
	double seconds;
	
	// get time of day in micro seconds
    gettimeofday(&tval, NULL);

	// convert us to seconds
	seconds = (double)tval.tv_sec + (double)tval.tv_usec / 1000000.0;

	if (data->io.timeStart == 0.0)
		data->io.timeStart = seconds;
	else
		data->io.cycleDelta = seconds - data->io.time;
	
	data->io.time = seconds;

	//need to handle the clock rolling over at midnight,					debug zz
}

//-----------------------------------------------------------------------------
void npLinuxSleep (double seconds)
{
	return;																//debug zz
}

//-----------------------------------------------------------------------------
FILE* npLinuxFileDialog (const char* fileName, int dialogType, void* dataRef)
{
	return;																//debug zz
}

//-----------------------------------------------------------------------------
void npLinuxTimeStampCSV (char* fileName)
{
	return;																//debug zz
}

//-----------------------------------------------------------------------------
int npLinuxSetCursorPos (int x, int y)
{
	return 0;																//debug zz
}

//-----------------------------------------------------------------------------
int npLinuxShowCursor (int hide)
{
	return 0;																//debug zz
}


#endif


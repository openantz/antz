/* -----------------------------------------------------------------------------
*
*  posix/npos.c
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

/* -----------------------------------------------------------------------------
*  THERE are TWO separate npos.c files!!!
* 
*  THIS IS posix/npos.c to use rename from npos_posix.c to npos.c			//zz-os
*  
*  the other .c file is located at msw/npos.c
*
* --------------------------------------------------------------------------- */

/* -----------------------------------------------------------------------------
*
* posix default environment uses mostly linux methods
* uses nposx.h for some __Apple__ procedures
* could add support for UNIX or linux variants
* CentOS is our development posix environment
* --------------------------------------------------------------------------- */

#ifndef _WIN32					//not windows then assume posix

#include "../../npdata.h"

//functions shared by OSX and Linux 

// #ifdef NP_POSIX_				//zz debug

// #ifdef NP_LINUX_				//zz debug

#ifdef __Apple__
	#include "nposx.h"
#else
	#include <time.h>			// for gettimeofday()
//	#include <unistd.h>			//debug zz
#endif

// current working directory is OS specific
//-----------------------------------------------------------------------------
void nposGetCWD (char* buffer, int* size)
{
	getcwd (buffer, size);
}

//-----------------------------------------------------------------------------
void nposGetAppPath (char* buffer, int* size)
{
	static bool firstTimeCalled = true;
	static char appPath[kNPmaxPath];

	//if first time then set the app path using the CWD method
	//should replace with actual get app path OS function, zz debug
	if (firstTimeCalled)
	{
		nposGetCWD (appPath, size);
		strcat (appPath, "/");
		firstTimeCalled = false;
	}
	
	//copy the app path to the passed in buffer
	strcpy (buffer, appPath);

	printf("\n%s\n", buffer);
	//set the length
	*size = strlen(buffer); 
}



//-----------------------------------------------------------------------------
void nposSetCWD (char* buffer)
{
	//	setcwd (buffer); // debug, voidptr1 build out
}

//-----------------------------------------------------------------------------
void nposGetOpenFilePath (char* buffer, int* size, void* dataRef)
{
	getcwd (buffer, size);
}

//-----------------------------------------------------------------------------
void nposSetOpenFilePath (char* buffer, void* dataRef)
{
	return;
}

//-----------------------------------------------------------------------------
double nposGetTime (void)									//untested, debug zz
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
void nposUpdateTime (void* dataRef)										//debug zz
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

	//need to handle clock roll-over at midnight						//zz debug
}

//-----------------------------------------------------------------------------
void nposSleep (double seconds)
{
//	printf("z");
	sleep((unsigned int)seconds);
	return;																//debug zz
}

//-----------------------------------------------------------------------------
/* // lde, temp
FILE* nposFileDialog (const char* fileName, int dialogType, void* dataRef)
{
	return;																//debug zz
}
*/

//-----------------------------------------------------------------------------
void nposTimeStampName (char* fileName)
{
	return;																//debug zz
}

//-----------------------------------------------------------------------------
int nposSetCursorPos (int x, int y)
{
	return 0;																//debug zz
}

//-----------------------------------------------------------------------------
int nposShowCursor (int hide)
{
	return 0;																//debug zz
}

//-----------------------------------------------------------------------------
/* // temp lde
int nposGetKey (void)
{
	return getch();																//debug zz
}
*/

#endif


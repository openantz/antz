/* -----------------------------------------------------------------------------
*
*  nposx.c
*
*  ANTz - realtime 3D data visualization tools for the real-world, based on NPE.
*
*  ANTz is hosted at http://openantz.com and NPE at http://neuralphysics.org
*
*  Written in 2010-2016 by Shane Saxon - saxon@openantz.com
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
*  with this software (license file named LICENSE.txt). If not, see
*  http://creativecommons.org/publicdomain/zero/1.0/
*
* --------------------------------------------------------------------------- */

#include "nposx.h"


#ifdef __APPLE__
#include <Carbon/Carbon.h>
#include <sys/time.h>                // for gettimeofday()
#include <pthread.h>
#include <dlfcn.h> // For dynamic linking

/* Windows function
void* nposLoadLibrary( char* filePath )
{
	void* library = LoadLibrary( filePath );
	if( !library )
	{
		printf( "err 9933 - failed to load library: %s\n", filePath);
		return NULL;
	}
	
	return library;
}
*/

//-----------------------------------------------------------------------------
// Take file path returned from microsoft createFile, put into fopen and return File Pointer
FILE* nposFileDialog( char* fileChosen, const char* initialDir,
					  int dialogType, void* dataRef )
{	
	FILE *filePtr;
	
	switch (dialogType)
	{
		case kNPfileDialogNew :
			nposFileDialog( fileChosen, initialDir, kNPfileDialogOpen, dataRef);
			break;
			
		case kNPfileDialogOpen : 
			filePtr = openFileDialog (fileChosen, initialDir, kNPfileDialogOpen, dataRef); 
			break;
			
		case kNPfileDialogClose : 
			nposFileDialog (fileChosen, initialDir, kNPfileDialogSaveAs, dataRef);
			break;
			
		case kNPfileDialogSave : 
			nposFileDialog (fileChosen, initialDir, kNPfileDialogSaveAs, dataRef); 
			break;
			
		case kNPfileDialogSaveAs : 
			//filePtr = SaveFileDialog(fileChosen, initialDir);  // @todo lde fix
			break;
			
		case kNPfileDialogImport : 
			nposFileDialog (fileChosen, initialDir, kNPfileDialogOpen, dataRef);
			break;
		case kNPfileDialogExport : 
			nposFileDialog (fileChosen, initialDir, kNPfileDialogSaveAs, dataRef); 
			break;
			
		default : break;
	}
	
	return filePtr;
}

void* nposLoadLibrary( char* filePath )
{
	void* library = NULL;
	library = dlopen(filePath, RTLD_NOW);
	
	if(!library)
	{
		printf( "err 9933 - failed to load library: %s\n", filePath);
		printf("\nError : %s", dlerror());
		printf("\nlibrary : %p", library);
		dlerror(); // library will be NULL at this point
	}

	return library;	
}

void* nposGetLibSymbol(void* library, char* symbolName) // New function, lde
{
	void* symbol = nposxGetLibSymbol(library, symbolName);
//	printf("\nlibrary : %p", library);
//	printf("\nsymbolName : %s", symbolName);
	return symbol;
}

void* nposxGetLibSymbol(void* library, char* symbolName) // New function, lde
{
	void* symbol = dlsym(library, symbolName);
//	printf("\ndbLibrary ptr : %p", library);
//	printf("\nsymbol : %p", symbol);
//	printf("\ndlerror : %s", dlerror());
	dlerror();
	return symbol;
}

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
	FILE* filePtr = NULL;
	return filePtr;																//debug zz
}


//-----------------------------------------------------------------------------
FILE* openFileDialog (const char* fileName, int dialogType, void* dataRef)
{																			//zz-osx debug
	FILE* filePtr = NULL;
	
	NavDialogCreationOptions dialogOptions;
	NavDialogRef dialog;
	NavReplyRecord replyRecord;
	CFURLRef fileAsCFURLRef = NULL;
	FSRef fileAsFSRef;
	OSStatus status;
	bool result = false;
	
	unsigned char filePath[1024];
	char msg[4096];
	
	// Get the standard set of defaults
	status = NavGetDefaultDialogCreationOptions (&dialogOptions);
	require_noerr( status, CantGetNavOptions );
	
	// Make the	window app-wide modal
	dialogOptions.modality = kWindowModalityAppModal;
	
	//	dialogOptions.location = fileName;
	
	// Create the dialog
	status = NavCreateGetFileDialog (&dialogOptions, NULL, NULL, NULL, NULL, NULL, &dialog);
	require_noerr( status, CantCreateDialog );
	
	// Show it
	status = NavDialogRun (dialog);
	require_noerr( status, CantRunDialog );
	
	// Get the reply
	status = NavDialogGetReply (dialog, &replyRecord);
	require( ((status == noErr) || (status == userCanceledErr)), CantGetReply );
	
	// If the user clicked "Cancel", just bail
	if ( status == userCanceledErr ) goto UserCanceled;
	
	// Get the file
	status = AEGetNthPtr ( &(replyRecord.selection), 1, typeFSRef, NULL, NULL, &fileAsFSRef, sizeof(FSRef), NULL);
	require_noerr( status, CantExtractFSRef );
	
	// Convert it to a CFURL
	fileAsCFURLRef = CFURLCreateFromFSRef(NULL, &fileAsFSRef);
	
	result = CFURLGetFileSystemRepresentation(fileAsCFURLRef, true, filePath, 1024);
	
	if (!result)
		npPostMsg("err 9824 - cannot convert file dialog path", kNPmsgErr, dataRef);
	else
	{
		// printf ("\nFile Path: %s\n", filePath);
		
		sprintf (msg, "%s", filePath);
		
		filePtr = fopen (msg, "r");
		
		if (filePtr != NULL)
		{
			sprintf (msg, "File Open: %s", filePath);
			npPostMsg (msg, kNPmsgCtrl, dataRef);
		}
		else
			npPostMsg ("err 2995 - File Pointer is NULL", kNPmsgErr, dataRef);
		
		return filePtr;
	}
	
	// Cleanup
CantExtractFSRef:
UserCanceled:
	verify_noerr( NavDisposeReply(&replyRecord) );
CantGetReply:
CantRunDialog:
	NavDialogDispose(dialog);
CantCreateDialog:
CantGetNavOptions:
	// return fileAsCFURLRef;	//part of original sample, does not apply here
	
	return NULL;
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

bool nposSupportsAntzThreads (void)
{
	return 1;
}

void nposEndThread (void)
{
	pthread_exit(NULL);
	//_endthread();
}

void nposBeginThread (voidFnPtr vfp, void *dataRef)
{
	pthread_t thread;
	pthread_create(&thread, NULL, vfp, dataRef);
}

int nposGetKey (void)
{
	//	return getch();																//debug zz
	return pause();
}

#endif


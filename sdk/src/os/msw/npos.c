/* -----------------------------------------------------------------------------
*
*  msw/npos.c
*
*  ANTz - realtime 3D data visualization tools for the real-world, based on NPE.
*
*  ANTz is hosted at http://openantz.com and NPE at http://neuralphysics.org
*
*  Written in 2010-2015 by Shane Saxon - saxon@openantz.com
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

/* -----------------------------------------------------------------------------
*
*  THERE are TWO separate npos.c files!!!
* 
*  THIS IS msw/npos.c and NOT posix/npos.c
*
* --------------------------------------------------------------------------- */

//avoid non-WIN32 environments trying to compile this file
#ifdef _WIN32

//zz alternate method to report an err
//
// throw a compiler err if trying to compile for wrong OS
// #ifdef _WIN32
//	#error 8686 - tried to compile "os/msw/npos.c" for wrong OS environmnet
// #endif

#include "../npos.h"		//single header used by all operating systems

#include "../../npdata.h"

#include <windows.h>
#include <process.h>
#include <io.h>
#include <wchar.h>
#include <conio.h>		//console io keys and interaction

//#include "wglext.h"
//#include "winbase.h"

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

void* npmswGetLibSymbol(void* library, char* symbolName)
{
	void* symbol = NULL;
	symbol = GetProcAddress(library, symbolName);
//zzd	printf("\nlibrary : %p", library);
//zzd	printf("\nsymbolName : %s", symbolName);
	return symbol;
}

void* nposGetLibSymbol(void* library, char* symbolName) // New function, lde
{
	void* symbol = npmswGetLibSymbol(library, symbolName);
//zzd	printf("\nlibrary : %p", library);
//zzd	printf("\nsymbolName : %s", symbolName);
	return symbol;
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
		firstTimeCalled = false;
	}
	
	//copy the app path to the passed in buffer
	strcpy (buffer, appPath);

	//set the length
	*size = strlen(buffer); 
}

// current working directory is OS specific
//-----------------------------------------------------------------------------
void nposGetCWD (char* buffer, int* size)
{
	int value = 0;
	int i = 0;
	DWORD length = 256;
	HINSTANCE hInstance = GetModuleHandle(NULL);
	*size = GetModuleFileName(hInstance, (LPCH)buffer, length);
//	char *chPtr = buffer;

//	*size *= 2;   // debug, ll	//debug, zz //stereo 3D

//	printf("Size is %d", (*size)/2); // debug, ll
	if (*size > 1)
		if (buffer[1] == 0)						// remove the spaces from UTF16,   debug zz
		{
			for(i = 0; 2 * i < *size; i++)
				buffer[i] = buffer[i*2];			// remove the spaces from UTF16,   debug zz

			*size /= 2;
		}

		// ll, Need to do some string processing, remove current running application filename from path
		for(i = (*size)-1; i >= 0; i--)
		{	
	//		printf("\nbuffer[%d] : %c\n", i, buffer[i]);

			if ( buffer[i] == '.' || buffer[i] == '\\' )
			{
	//			printf("Found %c", buffer[i]);
			
				if ( buffer[i] == '.' )
					buffer[i] = '\0';
				else
					break;

			}
			else
				buffer[i] = '\0';

		}
		// ll , End

	//	printf("File Path Size : %d\n", strlen(str)); 

	printf("CWD Path: ");
	for(i = 0; i < *size; i++)
		printf ("%c", buffer[i]);
	printf("\n");
}

//-----------------------------------------------------------------------------
void nposSetCWD (char* buffer)
{
	printf ("SetCWD: %d\n", SetCurrentDirectory ((LPCSTR)buffer));
}

//-----------------------------------------------------------------------------
void nposGetOpenFilePath (char* buffer, int* size, void* dataRef)
{
//	getcwd (buffer, size);
	return;
}

//-----------------------------------------------------------------------------
void nposSetOpenFilePath (char* buffer, void* dataRef)
{
	return;
}


/*
//-----------------------------------------------------------------------------
FILE* nposFileDialog (const char* fileName, int dialogType, void* dataRef)
{

}
*/




/*
Todo :
XML Formatting
Delete Directory
Delete File
Get/Set File Attributes
Save / Save As File Name
*/
/*
void importFile(char *filename)
{
	char file[2000];
	int returnedValue;
	int index = 0;

	fopen(filename, "r");



	while( returnedValue = fgetc() != EOF )
	{
		file[index] = returnedValue;
		printf("%c", file[index]);
		index++;
	}

}
*/
FILE* SaveFileDialog()
{
	FILE* filePtr;
	OPENFILENAME tmpOfn;
	HANDLE hwnd = NULL;
//	char fileBuff[260];	// buffer for file name
	LPTSTR testString[260];
	LPCTSTR* filterStr = (LPCTSTR*)"All\0*.*\0\0";
//	const TCHAR* filterStr = "All\0*.*\0\0";
	
	int i = 0;

	ZeroMemory(&tmpOfn, sizeof(tmpOfn));
	tmpOfn.lStructSize = sizeof(tmpOfn);
	tmpOfn.hwndOwner = hwnd;
	tmpOfn.lpstrFile = (LPTSTR)testString;
	tmpOfn.lpstrFile[0] = '\0';
	tmpOfn.nMaxFile = sizeof(testString);
//	tmpOfn.lpstrFilter = (LPWSTR)"All\0*.*\0Text\0*.TXT\0";
	tmpOfn.lpstrFilter = NULL;
	tmpOfn.nFilterIndex = 1;
	tmpOfn.lpstrFileTitle = NULL;
	tmpOfn.nMaxFileTitle = 0;
	tmpOfn.lpstrInitialDir = NULL;
	tmpOfn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;

	printf("\nSave File Dialog Function Called\n");

	if ( GetSaveFileName(&tmpOfn) == TRUE )
	{

		if( ( filePtr = _wfopen((wchar_t*)tmpOfn.lpstrFile, L"w") ) != NULL)
		{
			printf("NOT NULL");
			return filePtr;
		}
		else
		{
			printf("NULL");
			return NULL;
		}

/*
		if(filePtr == NULL)
		{
			printf("filePtr is NULL");
		}
*/


		printf("\nSave Returned True\n");
		return filePtr;
	}
	else
	{
		printf("\nSave Returned False\n");
		return NULL;
	}
}
/*
enum
{
	parameterNULL,  
	includeAllFiles,
	kNPfileDialogCount

};
*/

//zz debug make this return the fileName (includes path) instead of opening the FILE*
//------------------------------------------------------------------------------
// temp, lde
FILE* OpenFileDialog (const char* fileName, int kNPfileDialogOpen, void* dataRef)
{
	FILE* filePtr = NULL;
	int h = 0;
	int pathSize = 0;
	char msg[kNPmaxPath + 64];
	
	OPENFILENAME ofn;			// common dialog box structure
	char szFile[MAX_PATH];		// buffer for file name
//	char dirPath[MAX_PATH];		// buffer for file name
	HWND hwnd = NULL;			// owner window
	HANDLE hf;					// file handle


	szFile[0] = '\0';

//	nposSetCWD ("C:\\data\\");					//zz debug
//	GetCurrentDirectory (pathSize, dirPath);

//	nposGetCWD (dirPath, &pathSize);
//	strcat (dirPath, "antzcsv\\");

	// Initialize OPENFILENAME
	ZeroMemory(&ofn, sizeof(ofn));
	ofn.lStructSize = sizeof(ofn);
	ofn.hwndOwner = hwnd;		//hf
	ofn.lpstrFile = (LPSTR)szFile;
	// Set lpstrFile[0] to '\0' so that GetOpenFileName does not	//zz ???
	// use the contents of szFile to initialize itself.
//	ofn.lpstrFile[0] = '\0';
	ofn.nMaxFile = sizeof(szFile);
	ofn.lpstrFilter = NULL;// "All\0*.*\0Text\0*.TXT\0";
	ofn.nFilterIndex = 1;
	ofn.lpstrFileTitle = NULL;
	ofn.nMaxFileTitle = 0;
	ofn.lpstrInitialDir = (LPCSTR)fileName;
	ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;
	
	// Display the Open dialog box.
	if (GetOpenFileName(&ofn) == TRUE)
		hf = CreateFile(ofn.lpstrFile,
						GENERIC_READ,
						0,
						(LPSECURITY_ATTRIBUTES) NULL,
						OPEN_EXISTING,
						FILE_ATTRIBUTE_NORMAL,
						(HANDLE) NULL);
	else
	{
		npPostMsg ("File Open Dialog Cancelled", kNPmsgCtrl, dataRef);
		return NULL;
	}

	//prevents crash if app exited while dialog is open
	if (hf == INVALID_HANDLE_VALUE)	
	{
		npPostMsg ("err 2794 - File Invalid", kNPmsgErr, dataRef);
		return NULL;
	}

	h = _open_osfhandle((long) hf, 0);

	filePtr = _fdopen(h, "r");

	if (filePtr != NULL)
	{
		sprintf (msg, "File Open: %s", &szFile);
		npPostMsg (msg, kNPmsgCtrl, dataRef);
	}
	else
		npPostMsg ("err 2795 - File Pointer is NULL", kNPmsgErr, dataRef);

	return filePtr;
}


//-----------------------------------------------------------------------------
// Take file path returned from microsoft createFile, put into fopen and return File Pointer
FILE* nposFileDialog (const char* fileName, int dialogType, void* dataRef)
{	
	FILE *filePtr;

	switch (dialogType)
	{
		case kNPfileDialogNew :
			nposFileDialog (fileName, kNPfileDialogOpen, dataRef);
			break;

		case kNPfileDialogOpen : 
			filePtr = OpenFileDialog (fileName, kNPfileDialogOpen, dataRef); 
			break;

		case kNPfileDialogClose : 
			nposFileDialog (fileName, kNPfileDialogSaveAs, dataRef);
			break;

		case kNPfileDialogSave : 
			nposFileDialog (fileName, kNPfileDialogSaveAs, dataRef); 
			break;

		case kNPfileDialogSaveAs : 
			filePtr = SaveFileDialog(fileName); 
			break;

		case kNPfileDialogImport : 
			nposFileDialog (fileName, kNPfileDialogOpen, dataRef);
			break;
		case kNPfileDialogExport : 
			nposFileDialog (fileName, kNPfileDialogSaveAs, dataRef); 
			break;

		default : break;
	}

	return filePtr;
}

//-----------------------------------------------------------------------------
int nposShowCursor (int hide)
{
	if (hide)
		return ShowCursor(TRUE);
	else
		return ShowCursor(FALSE);
}
//-----------------------------------------------------------------------------
int nposSetCursorPos (int x, int y)
{
	return SetCursorPos (x,y);
}


//-----------------------------------------------------------------------------
// // temp lde
int nposGetKey (void)
{
	return _getch();
}


//-----------------------------------------------------------------------------
double nposGetTime (void)
{	
	LARGE_INTEGER frequency;		// ticks per second
	LARGE_INTEGER ticks;			// ticks
	double seconds;
	
	// get ticks per second
	QueryPerformanceFrequency(&frequency);
	
	// get the count in ticks
	QueryPerformanceCounter(&ticks);
	
	// convert ticks to seconds
	if (frequency.QuadPart)
		seconds = (double) ((double)ticks.QuadPart / (double)frequency.QuadPart);

	return seconds;
}

//-----------------------------------------------------------------------------
void nposUpdateTime (void* dataRef)
{
	pData data = (pData) dataRef;
	
	LARGE_INTEGER frequency;		// ticks per second
	LARGE_INTEGER ticks;			// ticks
	double seconds;
	
	// get ticks per second
	QueryPerformanceFrequency(&frequency);
	
	// get the count in ticks
	QueryPerformanceCounter(&ticks);
	
	// convert ticks to seconds
	if (frequency.QuadPart)
		seconds = (double) ((double)ticks.QuadPart / (double)frequency.QuadPart);

	if (data->io.timeStart == 0.0)
		data->io.timeStart = seconds;
	else
		data->io.cycleDelta = seconds - data->io.time;
	
	data->io.time = seconds;
}

//-----------------------------------------------------------------------------
void nposSleep (double seconds)
{
	Sleep((int)(seconds * 1000.0));
}


//------------------------------------------------------------------------------
void nposTimeStampName (char* fileName)
{
	SYSTEMTIME time;

	GetLocalTime(&time);
	sprintf (fileName, "antz%02d%02d%02d%02d%02d%02d",
			(time.wYear - 2000),	//we subtract 2000 to just get the last 2 digits
			time.wMonth, time.wDay, time.wHour, time.wMinute, time.wSecond);
}

//zz debug, add support for platform specific GL extension methods
//------------------------------------------------------------------------------
bool WGLExtensionSupported (const char *extension_name)
{
/*  
	http://stackoverflow.com/questions/589064/how-to-enable-vertical-sync-in-opengl
	
	// this is pointer to function which returns pointer to string with list of all wgl extensions
    PFNWGLGETEXTENSIONSSTRINGEXTPROC _wglGetExtensionsStringEXT = NULL;

    // determine pointer to wglGetExtensionsStringEXT function
    _wglGetExtensionsStringEXT = (PFNWGLGETEXTENSIONSSTRINGEXTPROC) wglGetProcAddress("wglGetExtensionsStringEXT");

    if (strstr(_wglGetExtensionsString(), extension_name) == NULL)
    {
        // string was not found
        return false;
    }
*/
    // extension is supported 
	return true;
}

//zz-JJ---------------------------------------------------------------------------
void nposBeginThread (voidFnPtr vfp, void *dataRef)
{
	_beginthread (vfp, 0, dataRef);
}


//zz-JJ---------------------------------------------------------------------------
void nposEndThread (void)
{
	_endthread();
}

bool nposSupportsAntzThreads (void)
{
	return 1;
}

//JJ-end

#endif



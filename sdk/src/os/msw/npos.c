/* -----------------------------------------------------------------------------
*
*  msw/npos.c
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


void npInitFileRef( pNPfileRef fileRef );


//------------------------------------------------------------------------------
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
FILE* SaveFileDialog( char* fileChosen, const char* initialDir )
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

int OpenFileDialog( char* fileChosen, const char* initialDir, 
					int dialogType, void* dataRef );
//------------------------------------------------------------------------------
int OpenFileDialog( char* fileChosen, const char* initialDir, 
					int dialogType, void* dataRef )
{
	FILE* filePtr = NULL;
	int h = 0;
	int pathSize = 0;
	char msg[kNPmaxPath];
	
	OPENFILENAME ofn;					// common dialog box structure
	char szFile[MAX_PATH] = {'\0'};		// buffer for file name

	HWND hwnd = NULL;			// owner window
//	HANDLE hf;					// file handle

//  initialDir does not work properly on Win 7
//	Windows 7 behavior is to only use this first time the dialog is opened
//  http://stackoverflow.com/questions/16164637/initial-directory-is-not-working

	// Initialize OPENFILENAME
	ZeroMemory(&ofn, sizeof(ofn));
	ofn.lStructSize = sizeof(ofn);
	ofn.hwndOwner = hwnd;		//hf
	ofn.lpstrFile = (LPSTR)szFile;
	ofn.nMaxFile = sizeof(szFile);
	ofn.lpstrFilter = NULL;			// "All\0*.*\0Text\0*.TXT\0";
	ofn.nFilterIndex = 1;
	ofn.lpstrFileTitle = NULL;
	ofn.nMaxFileTitle = 0;
	ofn.lpstrInitialDir = (LPCSTR)initialDir;
	ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;
	
	/// Display the Open dialog box.
	if( GetOpenFileName(&ofn) != TRUE )
		return false;

	sprintf( msg, "Open: %s", szFile );
	npPostMsg( msg, kNPmsgCtrl, dataRef );
	strcpy( fileChosen, szFile );

	return true;	// success

	// previous code returned FILE*
	//	hf = CreateFile(ofn.lpstrFile, GENERIC_READ, 0, 
	//					(LPSECURITY_ATTRIBUTES) NULL, OPEN_EXISTING,
	//					FILE_ATTRIBUTE_NORMAL, (HANDLE) NULL);			
	 //prevents crash if app exited while dialog is open
	//	hf = CreateFile(ofn.lpstrFile, GENERIC_READ, 0, (LPSECURITY_ATTRIBUTES)
	//			NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, (HANDLE) NULL);
	//	if (hf == INVALID_HANDLE_VALUE) return NULL;
	//	h = _open_osfhandle((long) hf, 0);
	//	filePtr = _fdopen(h, "r");
	//	fclose(filePtr);
}


//-----------------------------------------------------------------------------
// Take file path returned from microsoft createFile, put into fopen and return File Pointer
int nposFileDialog( char* fileChosen, const char* initialDir,
					  int dialogType, void* dataRef )
{	
	int i = 0;

	switch (dialogType)
	{
		case kNPfileDialogNew :
			i = nposFileDialog (fileChosen, initialDir, kNPfileDialogOpen, dataRef);
			break;

		case kNPfileDialogOpen : 
			i = OpenFileDialog (fileChosen, initialDir, kNPfileDialogOpen, dataRef); 
			break;

		case kNPfileDialogClose : 
			i = nposFileDialog (fileChosen, initialDir, kNPfileDialogSaveAs, dataRef);
			break;

		case kNPfileDialogSave : 
			i = nposFileDialog (fileChosen, initialDir, kNPfileDialogSaveAs, dataRef); 
			break;

		case kNPfileDialogSaveAs : 
			SaveFileDialog(fileChosen, initialDir); /// @todo make this return int
			break;

		case kNPfileDialogImport : 
			i = nposFileDialog (fileChosen, initialDir, kNPfileDialogOpen, dataRef);
			break;
		case kNPfileDialogExport : 
			i = nposFileDialog (fileChosen, initialDir, kNPfileDialogSaveAs, dataRef); 
			break;

		default : break;
	}

	return i;
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
Note that we will probably add GLEW lib to handle our extensions

 http://stackoverflow.com/questions/589064/how-to-enable-vertical-sync-in-opengl
	
 pointer to a function that returns pointer to a string with all wgl extensions
  PFNWGLGETEXTENSIONSSTRINGEXTPROC _wglGetExtensionsStringEXT = NULL;

 // determine pointer to wglGetExtensionsStringEXT function
 _wglGetExtensionsStringEXT =
(PFNWGLGETEXTENSIONSSTRINGEXTPROC) wglGetProcAddress("wglGetExtensionsStringEXT");

    if (strstr(_wglGetExtensionsString(), extension_name) == NULL)
    {
        // string was not found
        return false;
    }
*/
    // extension is supported 
	return true;
}

//zz-JJ-------------------------------------------------------------------------
void nposBeginThread (voidFnPtr vfp, void *dataRef)
{
	_beginthread (vfp, 0, dataRef);
}


//zz-JJ-------------------------------------------------------------------------
void nposEndThread (void)
{
	_endthread();
}

bool nposSupportsAntzThreads (void)
{
	return 1;
}
//JJ-end

//------------------------------------------------------------------------------
void npInitFileRef( pNPfileRef fileRef )
{
	if( fileRef == NULL )
		return;

	fileRef->handle = NULL;
	fileRef->name = NULL;
	
	fileRef->isDir = false;
	fileRef->sizeHi = 0;
	fileRef->sizeLo = 0;
}

//------------------------------------------------------------------------------
pNPfileRef nposNewFileRef( void* dataRef)
{
	pNPfileRef fileRef = NULL;
	
	fileRef = npMalloc( 0, sizeof(NPfileRef), dataRef );

	npInitFileRef( fileRef );

	return fileRef;
}

//------------------------------------------------------------------------------
int nposFindNextFile( pNPfileRef fileRef )
{
	int result = 0;
	
	result = FindNextFile( fileRef->handle, &fileRef->fdFile );
	if( !result )
		return 0;

	// FindFirstFile will return '.' and '..' as first two dir items.
	if( strcmp( fileRef->fdFile.cFileName, ".") == 0 )
		result = FindNextFile( fileRef->handle, &fileRef->fdFile );
	if( !result )
		return 0;

	if( strcmp( fileRef->fdFile.cFileName, "..") == 0 )
		result = FindNextFile( fileRef->handle, &fileRef->fdFile );
	if( !result )
		return 0;

	fileRef->name = fileRef->fdFile.cFileName;
	fileRef->isDir = fileRef->fdFile.dwFileAttributes &FILE_ATTRIBUTE_DIRECTORY;
	fileRef->sizeHi = fileRef->fdFile.nFileSizeLow;
	fileRef->sizeLo = fileRef->fdFile.nFileSizeHigh;

	return result;
}

//------------------------------------------------------------------------------
void nposFindClose( pNPfileRef fileRef, void* dataRef )
{
	FindClose( fileRef->handle );

	npFree( fileRef, dataRef );
}

/// Takes a base directory path as argument and returns a handle to the first file
//------------------------------------------------------------------------------
int nposFindFirstFile( pNPfileRef fileRef, const char* dirPath, 
					   const char* fileFilter, void* dataRef )
{
	int result = 0;
    char sPath[kNPmaxPath];

	// Specify the file mask as *.* to get everything!
	if( fileFilter == NULL )
		sprintf( sPath, "%s/*.*", dirPath );
	else
		sprintf( sPath, "%s/%s", dirPath, fileFilter );

	fileRef->handle = FindFirstFile( sPath, &fileRef->fdFile);
	if( fileRef->handle == INVALID_HANDLE_VALUE )
    {
        printf("err 7357 - INVALID_HANDLE_VALUE path: %s\n", dirPath);
        return -1;
    }

	// FindFirstFile will return '.' and '..' as first two items.
	if( strcmp( fileRef->fdFile.cFileName, ".") == 0 )
	{	result = FindNextFile( fileRef->handle, &fileRef->fdFile );
		if( !result )
		{
			printf("err 7358 - nposFindFirstFile nonsense: %s\n", dirPath);
			return -1;
		}
	}

	if( strcmp( fileRef->fdFile.cFileName, "..") == 0 )
	{	result = FindNextFile( fileRef->handle, &fileRef->fdFile );
		if( !result )
			return 0;	//empty directory
	}

	fileRef->name = fileRef->fdFile.cFileName;
	fileRef->isDir = fileRef->fdFile.dwFileAttributes &FILE_ATTRIBUTE_DIRECTORY;
	fileRef->sizeHi = fileRef->fdFile.nFileSizeLow;
	fileRef->sizeLo = fileRef->fdFile.nFileSizeHigh;

	return 1;
}

//------------------------------------------------------------------------------
void nposConsoleStart( void* sysCmd )
{
	system( (char*)sysCmd );
}

//------------------------------------------------------------------------------
bool nposCreateDir( const char* dir, void* dataRef)
{
	return CreateDirectory( dir, NULL);
}

//zz models begin
char nposGetFolderDelimit(void)
{
	return '\\';
}

/// new function lv
bool nposDirExists(const char* dir, void* dataRef)
{
	unsigned int attribs = 0;

	if(dir[0] == '\0')
		return false;

	attribs = GetFileAttributes(dir);
	if(attribs == INVALID_FILE_ATTRIBUTES)
		return false; /// Invalid Path

	if(attribs & FILE_ATTRIBUTE_DIRECTORY)
		return true; /// is a directory

	return false;
}

bool nposFileExists(const char* filepath, void* dataRef)
{
	unsigned int attribs = 0;

	attribs = GetFileAttributes(filepath);
	if( (attribs != INVALID_FILE_ATTRIBUTES) && (attribs != 0) && (attribs != FILE_ATTRIBUTE_DIRECTORY) )
		return true;

	return false;
}

bool nposFileExistsAtDir(const char* dir, char* filename, void* dataRef)
{
	bool dirExists = false;
	char filepath[256] = {'\0'};
	unsigned int attribs = 0;

	dirExists = nposDirExists(dir, dataRef);

	if(dirExists == true)
	{
		sprintf(filepath, "%s%s", dir, filename);
		attribs = GetFileAttributes(filepath);
		if( (attribs != INVALID_FILE_ATTRIBUTES) )
			return true;

//		if( (attribs != INVALID_FILE_ATTRIBUTES) && (attribs != 0) && (attribs != FILE_ATTRIBUTE_DIRECTORY) )
//			return true;
	}
	else
	{
		return false;
	}

	return false;
}
//zz models end

//MSW file attributes
/*

 Use FileTimeToSystemTime() to convert FILETIME

	DWORD dwFileAttributes;	
    FILETIME ftCreationTime;
    FILETIME ftLastAccessTime;
    FILETIME ftLastWriteTime;
    DWORD nFileSizeHigh;
    DWORD nFileSizeLow;
    DWORD dwReserved0;
    DWORD dwReserved1;
    CHAR   cFileName[ MAX_PATH ];
    CHAR   cAlternateFileName[ 14 ];

	#define FILE_SHARE_READ                 0x00000001  
	#define FILE_SHARE_WRITE                0x00000002  
	#define FILE_SHARE_DELETE               0x00000004  
	#define FILE_ATTRIBUTE_READONLY             0x00000001  
	#define FILE_ATTRIBUTE_HIDDEN               0x00000002  
	#define FILE_ATTRIBUTE_SYSTEM               0x00000004  
	#define FILE_ATTRIBUTE_DIRECTORY            0x00000010  
	#define FILE_ATTRIBUTE_ARCHIVE              0x00000020  
	#define FILE_ATTRIBUTE_DEVICE               0x00000040  
	#define FILE_ATTRIBUTE_NORMAL               0x00000080  
	#define FILE_ATTRIBUTE_TEMPORARY            0x00000100  
	#define FILE_ATTRIBUTE_SPARSE_FILE          0x00000200  
	#define FILE_ATTRIBUTE_REPARSE_POINT        0x00000400  
	#define FILE_ATTRIBUTE_COMPRESSED           0x00000800  
	#define FILE_ATTRIBUTE_OFFLINE              0x00001000  
	#define FILE_ATTRIBUTE_NOT_CONTENT_INDEXED  0x00002000  
	#define FILE_ATTRIBUTE_ENCRYPTED            0x00004000  
	#define FILE_ATTRIBUTE_VIRTUAL              0x00010000  

	HANDLE WINAPI FindFirstChangeNotification(
							 _In_  LPCTSTR lpPathName,
							 _In_  BOOL bWatchSubtree,
							 _In_  DWORD dwNotifyFilter );

	#define FILE_NOTIFY_CHANGE_FILE_NAME    0x00000001   
	#define FILE_NOTIFY_CHANGE_DIR_NAME     0x00000002   
	#define FILE_NOTIFY_CHANGE_ATTRIBUTES   0x00000004   
	#define FILE_NOTIFY_CHANGE_SIZE         0x00000008   
	#define FILE_NOTIFY_CHANGE_LAST_WRITE   0x00000010   
*/


#endif


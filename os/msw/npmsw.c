/* -----------------------------------------------------------------------------
*
*  npmsw.c
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

#include "npmsw.h"

#include "../../npdata.h"


//this allows for using one set of project files across platforms
#ifdef NP_MSW_

#include <windows.h>
#include <process.h>
#include <io.h>
#include <wchar.h>

//#include "wglext.h"
//#include "winbase.h"


//-----------------------------------------------------------------------------
void npMSWGetAppPath (char* buffer, int* size)
{
	static bool firstTimeCalled = true;
	static char appPath[kNPmaxPath];

	//if first time then set the app path using the CWD method
	//should replace with actual get app path OS function, zz debug
	if (firstTimeCalled)
	{
		npMSWGetCWD (appPath, size);
		firstTimeCalled = false;
	}
	
	//copy the app path to the passed in buffer
	strcpy (buffer, appPath);

	//set the length
	*size = strlen(buffer); 
}

// current working directory is OS specific
//-----------------------------------------------------------------------------
void npMSWGetCWD (char* buffer, int* size)
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
void npMSWSetCWD (char* buffer)
{
	printf ("SetCWD: %d\n", SetCurrentDirectory (buffer));
}

//-----------------------------------------------------------------------------
void npMSWGetOpenFilePath (char* buffer, int* size, void* dataRef)
{
//	getcwd (buffer, size);
	return;
}

//-----------------------------------------------------------------------------
void npMSWSetOpenFilePath (char* buffer, void* dataRef)
{
	return;
}


/*
//-----------------------------------------------------------------------------
FILE* npMSWFileDialog (const char* fileName, int dialogType, void* dataRef)
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

		if( ( filePtr = _wfopen(tmpOfn.lpstrFile, L"w") ) != NULL)
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
FILE* OpenFileDialog (const char* fileName, int kNPfileDialogOpen, void* dataRef)
{
	FILE* filePtr = NULL;
	int h = 0;
	int pathSize = 0;
	
	OPENFILENAME ofn;			// common dialog box structure
	char szFile[MAX_PATH];		// buffer for file name
//	char dirPath[MAX_PATH];		// buffer for file name
	HWND hwnd = NULL;			// owner window
	HANDLE hf;					// file handle


	szFile[0] = '\0';

//	npMSWSetCWD ("C:\\data\\");					//zz debug
//	GetCurrentDirectory (pathSize, dirPath);

//	npMSWGetCWD (dirPath, &pathSize);
//	strcat (dirPath, "antzcsv\\");

	// Initialize OPENFILENAME
	ZeroMemory(&ofn, sizeof(ofn));
	ofn.lStructSize = sizeof(ofn);
	ofn.hwndOwner = hwnd;		//hf
	ofn.lpstrFile = szFile;
	// Set lpstrFile[0] to '\0' so that GetOpenFileName does not	//zz ???
	// use the contents of szFile to initialize itself.
//	ofn.lpstrFile[0] = '\0';
	ofn.nMaxFile = sizeof(szFile);
	ofn.lpstrFilter = NULL;// "All\0*.*\0Text\0*.TXT\0";
	ofn.nFilterIndex = 1;
	ofn.lpstrFileTitle = NULL;
	ofn.nMaxFileTitle = 0;
	ofn.lpstrInitialDir = fileName;
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
		printf("File Open Dialog cancelled by user\n");
		return NULL;
	}

	h = _open_osfhandle((long) hf, 0);

	filePtr = _fdopen(h, "r");

	if (filePtr != NULL)
		printf("File Open\n");

	printf("path opened: %s\n", &szFile);

	return filePtr;
}


//-----------------------------------------------------------------------------
// Take file path returned from microsoft createFile, put into fopen and return File Pointer
FILE* npMSWFileDialog (const char* fileName, int dialogType, void* dataRef)
{	
	FILE *filePtr;

	switch (dialogType)
	{
		case kNPfileDialogNew :
			npMSWFileDialog (fileName, kNPfileDialogOpen, dataRef);
			break;

		case kNPfileDialogOpen : 
			filePtr = OpenFileDialog (fileName, kNPfileDialogOpen, dataRef); 
			break;

		case kNPfileDialogClose : 
			npMSWFileDialog (fileName, kNPfileDialogSaveAs, dataRef);
			break;

		case kNPfileDialogSave : 
			npMSWFileDialog (fileName, kNPfileDialogSaveAs, dataRef); 
			break;

		case kNPfileDialogSaveAs : 
			filePtr = SaveFileDialog(fileName); 
			break;

		case kNPfileDialogImport : 
			npMSWFileDialog (fileName, kNPfileDialogOpen, dataRef);
			break;
		case kNPfileDialogExport : 
			npMSWFileDialog (fileName, kNPfileDialogSaveAs, dataRef); 
			break;

		default : break;
	}

	return filePtr;
}

//-----------------------------------------------------------------------------
int npMSWShowCursor (int hide)
{
	if (hide)
		return ShowCursor(TRUE);
	else
		return ShowCursor(FALSE);
}
//-----------------------------------------------------------------------------
int npMSWSetCursorPos (int x, int y)
{
	return SetCursorPos (x,y);
}

//-----------------------------------------------------------------------------
double npMSWGetTime (void)
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
void npMSWUpdateTime (void* dataRef)
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
void npMSWSleep (double seconds)
{
	Sleep((int)(seconds * 1000.0));
}


//------------------------------------------------------------------------------
void npMSWTimeStampCSV (char* fileName)
{
	SYSTEMTIME time;

	GetLocalTime(&time);
	sprintf (fileName, "ANTz%02d%02d%02d%02d%02d%02d.CSV",
			(time.wYear - 2000),	//we subtract 2000 to just get the last 2 digits
			time.wMonth, time.wDay, time.wHour, time.wMinute, time.wSecond);
}


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
void npMSWBeginThread (voidFnPtr vfp, void *dataRef)
{
	_beginthread (vfp, 0, dataRef);
}


//zz-JJ---------------------------------------------------------------------------
void npMSWEndThread (void)
{
	_endthread();
}
//JJ-end


#endif


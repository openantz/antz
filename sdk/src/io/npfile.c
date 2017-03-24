/* -----------------------------------------------------------------------------
*
*  npfile.c
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

#include "npfile.h"

#include "npch.h"
#include "npconsole.h"
#include "../os/npos.h"
#include "../npctrl.h"
#include "../io/npgl.h"			//used for npPostTool()						zz, debug
#include "../io/gl/nptags.h"
#include "gl/nptexmap.h"		//zz tex
#include "file/npdot.h"			// parses DOT files from doxygen with graphviz


bool npValidateURL( const char* urlASCII, void* dataRef );
bool npNodeFilePath( char* buildPath, pNPnode node, void* dataRef );
bool npOpenNodeFile( pNPnode node, void* dataRef );
bool npOpenNode( pNPnode node, void* dataRef );

#define kNPtblGlobals "globals"
//-----------------------------------------------------------------------------
void npInitFile (void* dataRef)
{
	int result = 0;
	char filePath[kNPmaxPath];

	pData data = (pData) dataRef;


	// builds soft-coded default routing maps using mapItem descriptors
	npMapTypesInit(dataRef);									//zz debug, move to npmap.c

	/// Format the default globals table pathname
	sprintf( filePath, "%santz%s.csv", data->io.file.csvPath, kNPtblGlobals );					//globals.csv");

	/// Load the globals table, if none found then save one using defaults
	//result = npOpenMapCSV( filePath, kNPmapMapGlobals, dataRef);
	result = npOpenGlobalsCSV (filePath, 1, 15, dataRef);
	if (!result)
	{
		npSaveMapToCSV ("antz", kNPmapGlobals, dataRef);
		printf("Saved %s\n\n", filePath);
	}
	else
		printf("Loaded %s\n\n", filePath);

//	result = npOpenMapCSV ("antzosc.csv", kNPmapMapOSC, dataRef);

/*						//zz osc this worked but now broken... err 4427
	// if no globals csv file then create one
	strcpy (filePath, data->io.file.csvPath);
	strcat (filePath, "antzosc.csv");
	if (!result)
	{
		npSaveMapToCSV ("antz", kNPmapOSC, dataRef);
		printf("Saved %s\n", filePath);
	}
	else
		printf("Loaded %s\n", filePath);
*/
	printf("TEST 22\n\n");
	return;
}

//-----------------------------------------------------------------------------
void npCloseFile (void* dataRef)
{
//	terminate all file io threads gracefully then kill if needed
//	iterate through and close all open files
	return;
}

//-----------------------------------------------------------------------------
FILE* npGetFileRef (int index)
{
	static FILE* file[kNPfileMax];
	static FILE* fileCount = 0;

	return file[index];
}

//-----------------------------------------------------------------------------
// mode usage: 3 parameters "r/w/a" followed by optional "b" and optional "+"
// example mode = "rb+"
// r open file for reading
// w open file for overwriting. Create new file if doesn't already exist
// a open file for appending data. 
// b open file as a binary file
// + open file for read and writing
//-----------------------------------------------------------------------------
FILE* npFileOpen( const char* fileName, const char* mode, void* dataRef )
{
	int length = 0;

	FILE* file = NULL;
	char* msg = NULL;

	// ascert fileName is valid string
	//length = strnlen( fileName, 261 );
	length = strlen(fileName); // lde
	if(length < 1 || length >= 260 )
	{
		npPostMsg( "warn 7249 - invalid fileName", kNPmsgFile, dataRef );
		return NULL;
	}

	// open the file based on mode passed in
	file = fopen( fileName, mode ); 

	if ( file == NULL )
	{
		msg = (char*) npMalloc( 0, 512, dataRef );
		sprintf( msg, "warn 7888 - could not open file: %.261s", fileName );
		npPostMsg( msg, kNPmsgFile, dataRef );
		npFree( msg,dataRef );
		return NULL;
	}

	// add code to add the file ptr to the data->map							debug, zz

	return file;
}

//-----------------------------------------------------------------------------
FILE* npFileNew (const char* fileName, const char* mode, void* dataRef)
{
	char* file[1024];

	if (fileName == NULL)
	{
		*file = "antz0001.csv";
	// get the 
	// get CWD
	// check if "antz0001.csv" exists then increment until
	// put CWD together with fileName
	//	npFileDialogNew ("antz0001.csv", );
	}

	return npFileOpen (fileName, "w+", dataRef);
}

//zz replace the OS file dialog with custom in GL
//-----------------------------------------------------------------------------
int npFileDialog( char* fileChosen, const char* initialDir, 
					int dialogType, void* dataRef )
{	
	int i = 0;
	char nullStr[kNPmaxPath] = {'\0'};
	char* filePath = nullStr;

	// points to dummy nullStr if no fileChosen otherwise point to fileChosen
	if( fileChosen != NULL )
		filePath = fileChosen;

	switch (dialogType)
	{
		case kNPfileDialogNew : 
			i = nposFileDialog (filePath, initialDir, dialogType, dataRef); break;

		case kNPfileDialogOpen : 
			i = nposFileDialog (filePath, initialDir, dialogType, dataRef); break;
		case kNPfileDialogClose : 
			i = nposFileDialog (filePath, initialDir, dialogType, dataRef); break;

		case kNPfileDialogSave : 
			i = nposFileDialog (filePath, initialDir, dialogType, dataRef); break;
		case kNPfileDialogSaveAs : 
			i = nposFileDialog (filePath, initialDir, dialogType, dataRef); break;

		case kNPfileDialogImport : 
			i = nposFileDialog (filePath, initialDir, dialogType, dataRef); break;
		case kNPfileDialogExport : 
			i = nposFileDialog (filePath, initialDir, dialogType, dataRef); break;

		// pass through unknown dialog types to the os file handler
		default :
			i = nposFileDialog (filePath, initialDir, dialogType, dataRef); break;
	}

	return i;
}

//-----------------------------------------------------------------------------

// if not saved then opens SaveAs dialog before reseting app
int npFileClose (FILE* file, void* dataRef)
{
	int result = 0;
	
	result = fclose (file);

	if (result)
		printf(" err 4286 - file close result: %d\n", result, file);

	return result;
}


/*
* The total number of elements successfully read is returned.
* If this number differs from the count parameter, either a reading err occurred
* or the end-of-file was reached while reading. In both cases, the proper 
* indicator is set, which can be checked with ferror and feof, respectively.
* If either size or count is zero, the function returns zero and both the 
* stream state and the content pointed by ptr remain unchanged.
*/
//-----------------------------------------------------------------------------
int npFileRead (void* readBuffer, int elementSize, int elementCount, 
				 FILE* file, void* dataRef)
{
	int count = 0;

	count = fread (readBuffer, elementSize, elementCount, file);
	
	if ( count <= 0 )
		printf(" warn 7218 - empty file\n", file);

	return count;
}

//-----------------------------------------------------------------------------
void npFileRewind (FILE* file)
{
	rewind(file);
}

//-----------------------------------------------------------------------------
int npFileSeek (FILE* file, long offSet, int origin)
{
	// fseek(pFile, 0, SEEK_END);
	
	return fseek(file, offSet, origin);
}

//-----------------------------------------------------------------------------
long npFileTell (FILE* file)
{
	return ftell(file);			// returns the file index position
}


//-----------------------------------------------------------------------------
int npFileWrite (const void* str, int wordSize, int size, FILE* file, void* dataRef)
{
	if (size <= 0)
		return 0;

	return fwrite( str, wordSize, size, file );

	//return fwrite( str, 1, strlen(str), file );
}


//-----------------------------------------------------------------------------
void npGetMap (int mapType, int format, void* dataRef)
{
	return;
}


//-----------------------------------------------------------------------------
void npSave (FILE* file, void* dataRef)
{
//	FILE* file = NULL;

	if (file != NULL)
	{
		// save current state to open file
		// default file is compatible with csv, tdt, xml, and json
	}
	// create new default file and save state
	else 
	{
//		file = npFileNew ((const char*)"antz0001.xml", "w+", dataRef);
//		npSaveCSV (npGetMap (kNPmapAll, kNPtypeCSV, dataRef), dataRef);
	}
}


//-----------------------------------------------------------------------------
void npSaveAs (int size, void* dataRef)
{
//	char* filePath = NULL;

	size = 16;	// strlength 

//	file = npGetOpenFilePath(file, size, dataRef);

	npFileDialog( NULL, NULL, kNPfileDialogSaveAs, dataRef);

	// if file invalid call err dialog
	// save current state to file named by user
	// if quitting close file
	// else, 
	// open new copy of file and increment the file name 

}

//-----------------------------------------------------------------------------
void npFileImport (const char* fileName, void* dataRef)
{
	return;
}

//-----------------------------------------------------------------------------
void npFileExport (int size, void* dataRef)
{
	return;
}


//zz tex
//------------------------------------------------------------------------------
/*! Load directory with optional file name filter and category.
	@param path Directory path to search for file(s) including sub-diretories.
	@param fileFilter Only load files with specified filter, ie: 'map*.jpg'
	@param fileCat Load files by category, use kNPfileCatNull for all.
	@param dataRef Global scene graph pointer.

*/
void npLoadDir( char* path, char* fileFilter, int fileCat, void* dataRef )
{
	// if( kNPfileCatNull )

	switch( fileCat )
	{
		case kNPfileCatImage :
			npLoadTexDir( path, fileFilter, dataRef );
			break;
		default : break;
	}
}
//------------------------------------------------------------------------------
/*! Load directory with sub-directories, optional file name filter and category.
	@param path Directory path to search for file(s) including sub-diretories.
	@param fileFilter Only load files with specified filter, ie: 'map*.jpg'
	@param fileCat Load files by category, use kNPfileCatNull for all.
	@param dataRef Global scene graph pointer.
*/
void npLoadDirBranch( char* path, char* fileFilter, int fileCat, void* dataRef )
{
	npLoadTexDir( path, fileFilter, dataRef );
}


//-----------------------------------------------------------------------------
// file and directory management
// current working directory is OS specific
//-----------------------------------------------------------------------------
void npGetOpenFilePath (char* buffer, int* size, void* dataRef)
{
	return;
}

//-----------------------------------------------------------------------------
void npGetCWD (char* buffer, int* size)					
{	
	nposGetCWD (buffer, size);
	
	//size = strlen(buffer);
}

//-----------------------------------------------------------------------------
void npMakeDir ( char* pathName, void* dataRef)
{
	return;
}

//-----------------------------------------------------------------------------
void npDelDir ( char* pathName, void* dataRef)
{
	return;
}

//-----------------------------------------------------------------------------
void npListDir ( char* pathName, void* dataRef)
{
	return;
}

//-----------------------------------------------------------------------------
int npGetFileAttrib ( char* pathName, int attrib, void* dataRef)
{
	return 0;
}

//return true if able to set attributes of the specified pathName file or dir
//-----------------------------------------------------------------------------
bool npSetFileAttrib ( char* pathName, int attrib, void* dataRef)
{
	return false;
}

//-----------------------------------------------------------------------------
/// Open the URL using the OS 'start' command.
/// We recognize tag formats for both local files and remote websites.
/// Tag can be HTML 'href=' format or start with either www or http.
void npOpenURL (const char* command, void* dataRef)							//zz html replace entire function
{
	int i = 0;

	pData data = (pData) dataRef;

	pNPnode node = data->map.currentNode;
	pNPtag tag = node->tag;
	char* title = tag->title;

//	char sysCmd[kNPurlMax] = {"start \0"};	//generally 2048 is max URL length
	char* sysCmd = NULL;
	sysCmd = malloc(kNPurlMax);
	if(!sysCmd)
		return;

	strcpy( sysCmd, "start \0");

	/// First check for local files and custom web URL formatting.
	/// If no custom URL then open default webpage using the node record_id.
	if ( !strncmp( title, "<a href=\"", 9 ) )
	{
		i = 9;		/// Find endpoint of URL text between double quotes
		while ( title[i] != '\"' && i < kNPtagTitleMax ) i++;
		
		strncat( sysCmd, &title[9], i - 9 );
	}	
	else if( !strncmp( title, "www.", 4 ) || !strncmp( title, "http", 4 ) )
		strcat( sysCmd, title );
	else						
	{
		if( node->tableID >= kNPgitvizTableID )								//zz html comment out this
			sprintf( sysCmd, "start https://github.com/%s/issues/%d", 
						data->io.gitvizURL, node->recordID );
		else																//zz html comment out this - end
			sprintf( sysCmd, "start %s%d", data->io.url, node->recordID );	//zz html leave this
	}

	// post system start string to GUI console
	npPostMsg( sysCmd, kNPmsgCtrl, data );

	/// call 'system( sysCmd );' to open item using OS default app / browser.
	nposBeginThread( nposConsoleStart, sysCmd );
}

//-----------------------------------------------------------------------------
void npOpenApp (const char* command, void* dataRef)
{
	return;
}

//-----------------------------------------------------------------------------
void npOpenAntz (const char* command, void* dataRef)
{
	return;
}


//url str in ASCII text, must be null terminated '\0' cstr
//returns true if starts with 'http' or '<a '		  //zz debug build this out
//-----------------------------------------------------------------------------
bool npValidateURL( const char* urlASCII, void* dataRef )
{
	if( strncmp( urlASCII, "http", 4 ) == 0
	  || strncmp( urlASCII, "<a ", 3 ) == 0
	  || strncmp( urlASCII, "www.", 4 ) == 0)
		return true;

	return false;
}

#define kNPmaxFolderDepth 128
//-----------------------------------------------------------------------------
bool npNodeFilePath( char* buildPath, pNPnode node, void* dataRef )
{
	int i = 0;
	pNPnode routeNode = node;
	char* rootRoute[kNPmaxFolderDepth];

	buildPath[0] = '\0';	//ascert empty null cstr before using strncat

	//first check to see if the node title is a full path C:/data/code/antz
	if( node->tag->title[1] == ':' )	//if 2nd char is : then assume X:\	//zz debug better full path validate
		//|| urlASCII[1] == '/' //check for various relative paths
	{
		strncat( buildPath, node->tag->title, kNPtagTitleMax );
		return true;
	}
	else
		return false; //exit condition to open file dialog

	do	//creates a list of the root route individual folder names
	{
		rootRoute[routeNode->branchLevel] = routeNode->tag->title;
		routeNode = routeNode->parent;
	} 
	while( routeNode->branchLevel > 0 );

	// build the path from the routeRoute list
	for( i=0; i <= node->branchLevel; i++)
		strncat(buildPath, rootRoute[i], kNPtagTitleMax);

//	if ( !npNodeFilePath( buildPath, node, dataRef ) )
//		return false;

	return true;
}

/// Open native file types in the app, others opened with OS default handler.
/// URL paths are opened using the OS default browser.
/// The OSX 'open' or MSW 'start' command opens a file (or a directory or URL),
/// just as if you had double-clicked the file's icon. If no application name
/// is specified, the default application as determined via LaunchServices is
/// used to open the specified files.
//-----------------------------------------------------------------------------
bool npOpenNodeFile( pNPnode node, void* dataRef )
{
	int fileType = 0, fileCat = 0;

	char nodePath[kNPurlMax];
	char browserURL[kNPurlMax + 9];	// +9 for 'xdg-open ' followed by URL

	if ( npNodeFilePath( nodePath, node, dataRef ) )
	{
		npPostMsg( nodePath, kNPmsgFile, dataRef );	//tell the user

		fileType = npGetFileTypeCat( &fileCat, nodePath, dataRef );

		switch( fileCat )
		{
			case kNPfileCatBin :	///< Run executable files directly.
				system( nodePath );
				break;
			case kNPfileCatTable :	///< Open CSV tables within app.
				npFileOpenAuto( nodePath, NULL, dataRef );
				break;
			default :	///< URL and other files opened with default OS handler
				/// @todo maybe move fileviz node handling to npos.h
#ifdef NP_MSW_				
				sprintf( browserURL, "start %s", nodePath );
#endif
#ifdef NP_OSX_				
				sprintf( browserURL, "open %s", nodePath );
#endif
#ifdef NP_LINUX_				
				sprintf( browserURL, "xdg-open %s", nodePath );
#endif 
				system ( browserURL );
				break;
	}	}

	return true;
}

//-----------------------------------------------------------------------------
bool npOpenNode( pNPnode node, void* dataRef )
{
	char* path = NULL;

	//identify node by tag title and record_id

	//if URL launch in system browser					//zz debug add in-app web browser
	if( npValidateURL( node->tag->title, dataRef ) )
	{
		npCtrlCommand( kNPcmdOpenURL, dataRef );
		return true;
	}

	//if its a file path then construct it
	if( npOpenNodeFile( node, dataRef ) )
		return true;

	return false;
}

//choose a folder, file or DB, can select multiple items
//launches the OS file dialog from a thread
//------------------------------------------------------------------------------
void npFileBrowser( void* dataRef )
{
	int result = false;
	static firstTime = true;
	char* initialDir = NULL;
	//FILE* file = NULL;
	char filePath[kNPmaxPath] = {'\0'};

	pData data = (pData) dataRef;

	/// First time we open the dialog to the 'usr/csv' folder.
	/// After that we open the previous location chosen by the user, using NULL.
	//zz debug, not working with W7
	if( firstTime )
	{
		initialDir = data->io.file.csvPath;
		firstTime = false;
	}
	
	/// Leave fullscreen so the OS file dialog box isn't fighting to be on top.
	if( data->io.gl.fullscreen )
	{	
		npCtrlCommand( kNPcmdFullscreen, data );
		result = npFileDialog( filePath, initialDir, kNPfileDialogOpen, data );
		npCtrlCommand( kNPcmdFullscreen, data );
	}
	else
		result = npFileDialog( filePath, initialDir, kNPfileDialogOpen, data );
	
	if( !result )
		npPostMsg ("File Dialog Cancelled", kNPmsgCtrl, dataRef);
	else
		npFileOpenAuto( filePath, NULL, data );
}

//zz-JJ
// track metadata, sample rate, data type, etc...
//------------------------------------------------------------------------------
void npFileOpenChMeta (const char* filePath, void* dataRef)
{
	npLoadChMeta (filePath, dataRef);
}

//------------------------------------------------------------------------------
void npFileOpenChTracks (const char* filePath, void* dataRef)
{
	npLoadChTracks (filePath, dataRef);
}

//------------------------------------------------------------------------------
void npFileOpenChMap (const char* filePath, void* dataRef)
{
	npLoadChMap (filePath, dataRef);
}

//------------------------------------------------------------------------------
void npChSetBuildPath(char* path, char* csvPath, char* filePrefix, 
					  char* setNumber, char* extension)
{
	strcpy (path, csvPath);
	strcat (path, filePrefix);
	strcat (path, setNumber);
	strcat (path, extension);
}

//------------------------------------------------------------------------------
void npFileOpenChSet (const char* filePath, void* dataRef)
{
	char path[kNPmaxPath];
	char setNumberCharacters[5];

	pData data = (pData) dataRef;
	sprintf (&setNumberCharacters[0], "%04d", data->io.ch.setNumberToLoad);		//zz debug 

	npChSetBuildPath(path, data->io.file.csvPath, "antzchmeta", &setNumberCharacters[0], ".csv");	//zz debug 
	npFileOpenChMeta (path, dataRef);			//metadata

	npChSetBuildPath(path, data->io.file.csvPath, "antzch", &setNumberCharacters[0], ".csv");
	npFileOpenChTracks (path, dataRef);			//tracks

	npChSetBuildPath(path, data->io.file.csvPath, "antzchmap", &setNumberCharacters[0], ".csv");
	npFileOpenChMap (path, dataRef);			//ch mapping

	//subscribe existing nodes with channels to the channel map
	npSyncChMap (dataRef);
}
//zz-JJ-End


int npSaveScene( int format, char* datasetName, void* dataRef)
{
	int		result = 0;
	pData data = (pData) dataRef;
	char pathName[kNPmaxPath] = {'\0'};

	/// @todo create directory file sets with tables, images and 3D models
	// file set based on (user) selected table types
	//	if ( data->io.file.saveMapSet[kNPmapGlobals] )

	sprintf( pathName, "usr/images/%s", datasetName);
	npScreenGrabThumb( pathName, kNPfileTIFF,
						0, 0, kNPthumbWidth, kNPthumbHeight, data );

	result += npSaveMapToCSV( datasetName, kNPmapGlobals, data ); 

	result += npSaveMapToCSV( datasetName, kNPmapNode, data );

	result += npSaveMapToCSV( datasetName, kNPmapTag, data );

//zz	result += npSaveMapToCSV( datasetName, kNPmapTextures, data ); //lv save textures

//zz	result += npSaveMapToCSV( datasetName, kNPmapModels, data ); //lv save models

	return result;
}
int npSaveScene2( int format, char* datasetName, void* dataRef)
{
	int	result = 0;
	char msg[512];
	char dir[kNPmaxPath];

	pData data = (pData) dataRef;

	/// @todo set the CWD to the location of the app

	// if no 'usr' dir then create it
	if( !nposDirExists( kNPpathUser, data ) )
	{
		if( !nposCreateDir( kNPpathUser, data) )	//create the usr dir
		{
			sprintf( msg, "err 4288 - failed to create 'usr' directory", 
					 datasetName, data);
			npPostMsg( msg, kNPmsgErr, data);
			return -1;
		}
	}

	// if dataset dir exists then overwrite it, else create it
	sprintf( dir, "%s%s/%s", data->io.file.appPath, kNPpathUser, datasetName);
	if( nposDirExists( dir, data) )
	{
		sprintf( msg, "Overwriting dataset: %s", datasetName, data);
		npPostMsg( msg, kNPmsgFile, data);
	}
	else if( !nposCreateDir( dir, data) )	//create the dir
	{
		sprintf( msg, "err 4289 - failed to create directory: %s", 
				 datasetName, data);
		npPostMsg( msg, kNPmsgErr, data);
		return -1;
	}
	
	sprintf( dir, "%s%s/%s/%s", data->io.file.appPath, kNPpathUser, datasetName, kNPpathCSV);
	if( nposDirExists( dir, data) )
	{
		sprintf( msg, "Overwriting dataset: %s", datasetName, data);
		npPostMsg( msg, kNPmsgFile, data);
	}
	else if( !nposCreateDir( dir, data) )	//create the dir
	{
		sprintf( msg, "err 4290 - failed to create directory: %s", 
				 datasetName, data);
		npPostMsg( msg, kNPmsgErr, data);
		return -1;
	}

	printf("\n\nPATH: %s\n\n",dir);


	result += npSaveMapToCSV2( dir, kNPmapGlobals, data ); 

	result += npSaveMapToCSV2( dir, kNPmapNode, data );

	result += npSaveMapToCSV2( dir, kNPmapTag, data );

	// construct the images folder, used for saving a thumbnail
	sprintf( dir, "%s%s/%s/%s", data->io.file.appPath, kNPpathUser, datasetName, kNPpathImages);
	if( nposDirExists( dir, data) )
	{
		sprintf( msg, "Overwriting dataset: %s", datasetName, data);
		npPostMsg( msg, kNPmsgFile, data);
	}
	else if( !nposCreateDir( dir, data) )	//create the dir
	{
		sprintf( msg, "err 4290 - failed to create directory: %s", 
				 datasetName, data);
		npPostMsg( msg, kNPmsgErr, data);
		return -1;
	}

	// save thumbnail
	strcat( dir, "/");
	strcat( dir, datasetName);
	npScreenGrabThumb( dir, kNPfileDDS,
						0, 0, kNPthumbWidth, kNPthumbHeight, data );

//zz	result += npSaveMapToCSV( datasetName, kNPmapTextures, data ); //lv save textures

//zz	result += npSaveMapToCSV( datasetName, kNPmapModels, data ); //lv save models

	return result;
}

/*
int npOpenCSV( int format, char* datasetName, void* dataRef );
//------------------------------------------------------------------------------
int npOpenCSV( int format, char* datasetName, void* dataRef )
{
	int		result = 0;
	char*	dirPath = NULL;
	char	filePath[kNPmaxPath];
	char	msg[256];

	pData data = (pData) dataRef;

	dirPath = data->io.file.csvPath;
	data->io.file.loading = true;


	sprintf( filePath, "%s%s%s", dirPath, datasetName, 
			 npMapTypeName( mapTypeID, data ), ".csv" );
	
	sprintf( msg, "Loading: %s", filePath );
	npPostMsg( msg, kNPmsgCtrl, data );
	result += npOpenGlobalsCSV( filePath, 1, 15, data );

	sprintf( filePath, "%s%s%s", dirPath, datasetName, "node.csv" );
	sprintf( msg, "Loading: %s", filePath );
	npPostMsg (msg, kNPmsgCtrl, data );
	result += npFileOpenAuto( filePath, NULL, data );

	sprintf( filePath, "%s%s%s", dirPath, datasetName, "tag.csv" );
	sprintf( msg, "Loading: %s", filePath );
	npPostMsg( msg, kNPmsgCtrl, data );
	result += npFileOpenAuto( filePath, NULL, data );

	return result;
}
*/


//zz msw unicode sample code below
/*
bool ListDirectoryContents(const wchar_t *sDir)
{ 
    WIN32_FIND_DATA fdFile; 
    HANDLE hFind = NULL; 

    wchar_t sPath[2048]; 

    //Specify a file mask. *.* = We want everything! 
    wsprintf(sPath, L"%s\\*.*", sDir); 

    if((hFind = FindFirstFile(sPath, &fdFile)) == INVALID_HANDLE_VALUE) 
    { 
        wprintf(L"Path not found: [%s]\n", sDir); 
        return false; 
    } 

    do
    { 
        //Find first file will always return "."
        //    and ".." as the first two directories. 
        if(wcscmp(fdFile.cFileName, L".") != 0
                && wcscmp(fdFile.cFileName, L"..") != 0) 
        { 
            //Build up our file path using the passed in 
            //  [sDir] and the file/foldername we just found: 
            wsprintf(sPath, L"%s\\%s", sDir, fdFile.cFileName); 

            //Is the entity a File or Folder? 
            if(fdFile.dwFileAttributes &FILE_ATTRIBUTE_DIRECTORY) 
            { 
                wprintf(L"Directory: %s\n", sPath); 
                ListDirectoryContents(sPath); //Recursion, I love it! 
            } 
            else{ 
                wprintf(L"File: %s\n", sPath); 
            } 
        }
    } 
    while(FindNextFile(hFind, &fdFile)); //Find the next file. 

    FindClose(hFind); //Always, Always, clean things up! 

    return true; 
} 
*/

//zz posix directory listing sample code below
/*
//#include <unistd.h>
#include <sys/types.h>
#include <dirent.h>
#include <stdio.h>

void listdir(const char *name, int level)
{
    DIR *dir;
    struct dirent *entry;

    if (!(dir = opendir(name)))
        return;
    if (!(entry = readdir(dir)))
        return;

    do {
        if (entry->d_type == DT_DIR) {
            char path[1024];
            int len = snprintf(path, sizeof(path)-1, "%s/%s", name, entry->d_name);
            path[len] = 0;
            if (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0)
                continue;
            printf("%*s[%s]\n", level*2, "", entry->d_name);
            listdir(path, level + 1);
        }
        else
            printf("%*s- %s\n", level*2, "", entry->d_name);
    } while (entry = readdir(dir));
    closedir(dir);
}
*/

int npLoadScene( int format, char* datasetName, void* dataRef)
{
	NPfloatXY xy = {0.0f,0.0f};
	int		i = 0;
	int		result = 0;
	char*	dirPath = NULL;
	char	filePath[kNPmaxPath];
	char	msg[kNPmaxPath];

	pData data = (pData) dataRef;

	dirPath = data->io.file.csvPath;
	data->io.file.loading = true;

	//zz could list directory contents and/or display dir node tree
	// for(i=0; i < count; i++) npFileOpenAuto( tbl[i], NULL, data );

	sprintf( filePath, "%s%s%s%s", dirPath, datasetName, 
			npMapTypeName( kNPmapGlobals, data ), ".csv" );
	sprintf( msg, "File Open: %s", filePath );
	npPostMsg( msg, kNPmsgCtrl, data );
	//result += npFileOpenAuto( filePath, NULL, data );	// fullscreen issue #111
	result += npOpenGlobalsCSV( filePath, 1, 0, data );

	sprintf( filePath, "%s%s%s%s", dirPath, datasetName, 
			 npMapTypeName( kNPmapNode, data ), ".csv" );
	result += npFileOpenAuto( filePath, NULL, data );

	sprintf( filePath, "%s%s%s%s", dirPath, datasetName, 
			 npMapTypeName( kNPmapTag, data ), ".csv" );
	result += npFileOpenAuto( filePath, NULL, data );

	return result;
}

//zz models begin
void npGetFileNameFromPath(char* filepath, char* filename, void* dataRef)
{
	char* p_filepath = NULL; // lv, p_ prefix denotes pointer
	char delimit[1] = "\\";
	int pathLen = 0; // lv, Len suffix denotes length
	int pathX = 0; // lv, X suffix denotes an index

	filename[0] = '\0';

	if(filepath == NULL)
	{
		printf("filepath is NULL");
		return;
	}
	
	pathLen = strlen(filepath); // lv, Len suffix denotes length
	if(pathLen == 0)
	{
		printf("\nString has zero length");
		return;
	}

	p_filepath = strstr(filepath, ".");
	if(!p_filepath)
	{
		printf("\nfile does not have extension");
		return;
	}

	pathX = pathLen - strlen(p_filepath);

//	delimit = nposGetFolderDelimit();

	while( (*p_filepath) != '\\' && pathX >= 0)
	{
		p_filepath--;
		pathX--;
	}

	p_filepath++;
//	delimit = nposGetFolderDelimit();
//	delimit = "/";
	if(pathX == -1 && (strstr(filepath, "/") != NULL) )
	{
//		delimit = '/';
		p_filepath = strstr(filepath, ".");
		pathX = pathLen - strlen(p_filepath);	

		while( (*p_filepath) != '/' && pathX >= 0)
		{
			p_filepath--;
			pathX--;
		}
		p_filepath++;
		
	}

	pathX++;

	strcpy(filename, p_filepath);
}

char* npSearchPathsForFile(char* filename, void* dataRef)
{
	char* filepath = NULL;
	char* searchpaths[4];
	int index = 0;
	pData data = (pData) dataRef;

//	searchpaths
	searchpaths[0] = data->io.file.appPath;
	searchpaths[1] = data->io.file.modelPath;
	searchpaths[2] = data->io.file.csvPath;
	searchpaths[3] = data->io.file.mapPath;

	if(filename[0] != '\0' && filename != NULL)
	{
		for(index = 0; index < 4; index++)
		{
			if( nposFileExistsAtDir(searchpaths[index], filename, dataRef ) == true )
			{
				filepath = malloc(sizeof(char) *  (strlen(searchpaths[index]) + strlen(filename) + 1) );
				if(filepath != NULL)
				{
					filepath[0] = '\0';
					sprintf(filepath, "%s%s", searchpaths[index], filename);
//					printf("\nfilepath : %s", filepath);
					return filepath;
				}
				else
				{
					return "\0";
				}
			}
		}
	}

	return "\0"; 
}

char* npFilePathRelToAbs(char* rel, void* dataRef)
{
	pData data = (pData) dataRef;
	char* abs = NULL;
	abs = malloc(sizeof(char) * 256);
	abs[0] = '\0';
	strcpy(abs, data->io.file.appPath);
	strcat(abs, rel);
	return abs;
}

char* npFilePathAbsToRel(char* abs, void* dataRef)
{
	pData data = (pData) dataRef;
	char* rel = NULL;
	char tmp[256] = {'\0'};
	char* z = NULL;
	int i = 0;
	int len = 0;
	rel = malloc(sizeof(char) * 256);
	
	strcpy(tmp, abs);	
	len = strlen(tmp);
	for(;i < len; i++)
		tmp[i] = tolower(tmp[i]);
	
	if( strcmp(tmp, data->io.file.appPath) == 0 )
		rel[0] = '\0';
	else if( strcmp(tmp, data->io.file.appPath) > 0 )
	{
		rel[0] = '\0';
		strcpy(rel, &tmp[strlen(data->io.file.appPath)]);
		//printf("rel path : %s\n", rel);

	}
//	z = strstr(temp, data->io.file.appPath);

	return rel;
}
//zz models end



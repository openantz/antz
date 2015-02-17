/* -----------------------------------------------------------------------------
*
*  npfile.c
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

#include "npfile.h"

#include "npch.h"
#include "npconsole.h"
#include "../os/npos.h"
#include "../npctrl.h"
#include "../io/npgl.h"		   //used for npPostTool()						zz, debug
#include "../io/gl/nptags.h"
#include "file/npdot.h"		   //<! parses DOT files from doxygen with graphviz

//-----------------------------------------------------------------------------
FILE* npGetFileRef (int index)
{
	static FILE* file[kNPfileMax];
	static FILE* fileCount = 0;

	return file[index];
}


//-----------------------------------------------------------------------------
void npInitFile (void* dataRef)
{
	int result = 0;
	char filePath[kNPmaxPath];

	pData data = (pData) dataRef;


	// builds soft-coded default routing maps using mapItem descriptors
	npMapTypeInit(dataRef);									//zz debug, move to npmap.c

	// save the globals csv file
	strcpy (filePath, data->io.file.csvPath);
	strcat( filePath, "antzglobals.csv" );					//globals.csv");

	result = npOpenGlobalsCSV (filePath, 1, 15, dataRef);

	// if no globals csv file then create one
	if (!result)
	{
		npSaveMapToCSV ("antz", kNPmapGlobals, dataRef);
		printf("Saved %s\n", filePath);
	}
	else
		printf("Loaded %s\n", filePath);

//	result = npOpenMapCSV ("antzosc.csv", kNPmapMapOSC, dataRef);

/*								//zz osc this worked but now broken... err 4427
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
FILE* npFileDialog (const char* fileName, int dialogType, void* dataRef)
{	
	FILE* file = NULL;

	switch (dialogType)
	{
		case kNPfileDialogNew : 
			file = nposFileDialog (fileName, dialogType, dataRef); break;

		case kNPfileDialogOpen : 
			file = nposFileDialog (fileName, dialogType, dataRef); break; // temp, lde
			//file = OpenFileDialog(fileName, dialogType, dataRef); break;
		case kNPfileDialogClose : 
			file = nposFileDialog (fileName, dialogType, dataRef); break;

		case kNPfileDialogSave : 
			file = nposFileDialog (fileName, dialogType, dataRef); break;
		case kNPfileDialogSaveAs : 
			file = nposFileDialog (fileName, dialogType, dataRef); break;

		case kNPfileDialogImport : 
			file = nposFileDialog (fileName, dialogType, dataRef); break;
		case kNPfileDialogExport : 
			file = nposFileDialog (fileName, dialogType, dataRef); break;

		// pass through unknown dialog types to the os file handler
		default : file = nposFileDialog (fileName, dialogType, dataRef); break;
	}

	return file;
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

	npFileDialog (NULL, kNPfileDialogSaveAs, dataRef);

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
void npOpenURL (const char* command, void* dataRef)
{
	int i = 0, j = 0;

	pData data = (pData) dataRef;

	pNPnode node = data->map.currentNode;
	char* title = &node->tag->title[0];

	char sysCmd[kNPurlMax];	//generally 2048 is max URL length
	char url[kNPurlMax];
//	char msg[kNPurlMax];

	// compose string using the recordID & URL passed in from the command line
	// or the hard coded default in npdata.c,
	// MSW uses 'start' to call browser
	if( strncmp( title, "www", 3 ) == 0 )
	{
		i = 7;		//skip over '=' to first quote "
		while ( title[i] != '\"' && i < kNPtagTitleMax ) i++;	//zz debug add proper html href parsing
		
		j = ++i; 
		while ( title[j] != '\"' && j < kNPtagTitleMax ) j++;

		printf( "TEST i: %d  j: %d", i, j );
	//	j=i=9;
	}
	if ( strncmp( title, "<a href", 7 ) == 0 )
	{
		i = 7;							//skip over '=' to first quote "
		while ( title[i] != '\"' && i < kNPtagTitleMax ) i++;	//zz debug add proper html href parsing
		
		j = ++i;
		while ( title[j] != '\"' && j < kNPtagTitleMax ) j++;

	//	printf( "TEST i: %d  j: %d", i, j );
	}

	//look for 'http' after 'href'
	if ( strncmp( &title[i], "http", 4 ) == 0 )
	{
		if( !j )
			while ( title[j] != ' ' && j < kNPtagTitleMax ) j++;	//zz debug needs to detect end line, tab, etc.	
	//	npNextWhiteSpace( node->tag->title[j], kNPtagTitleMax - j );
	
		strncpy( url, &title[i], j - i );
		url[j - i] = '\0';

		sprintf( sysCmd, "start %s", url ); //&node->tag->title[9] );//url );
	}
	else
	{
		//strncpy( url, data->io.url, kNPurlMax );
		sprintf( sysCmd, "start %s%d", data->io.url, 
				data->map.currentNode->recordID );
		//return;
	}

	/// system call with 'start' to open browser with the composed URL
	system( sysCmd );
	npPostMsg( sysCmd, kNPmsgCtrl, data );
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
bool npValidateURL( const char* urlASCII, void* dataRef );
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
bool npNodeFilePath( char* buildPath, pNPnode node, void* dataRef );
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

bool npOpenNodeFile( pNPnode node, void* dataRef );
//-----------------------------------------------------------------------------
bool npOpenNodeFile( pNPnode node, void* dataRef )
{
	int result = false;

	char sysCmd[kNPurlMax + 8];	//+8 for 'start '...url
	char buildPath[kNPurlMax];

	if ( npNodeFilePath( buildPath, node, dataRef ) )
	{
		//tell the user
		npPostMsg( buildPath, kNPmsgFile, dataRef );

		// lde, @todo generalize this to be os independent
		//open with antz or open using system default app for file type
		if(0) // ".csv" || ".CSV"
		{}
		else
		{		// call system to open file using default app
			printf( "%s\n", &buildPath[strlen(buildPath) - 4] == 0 );

			if( strncmp( ".exe", &buildPath[strlen(buildPath) - 4], 4 ) == 0
			  || strncmp( ".bat", &buildPath[strlen(buildPath) - 4], 4 ) == 0 )
			{
				sprintf( sysCmd, "%s", buildPath );
			}
			else if ( strncmp( ".csv", &buildPath[strlen(buildPath) - 4], 4 ) == 0 )
			{
				return npFileOpenAuto( buildPath, NULL, dataRef );
			}
			else
				sprintf( sysCmd, "start %s", buildPath );
			
			system ( sysCmd );
			return true;
		}
	}

	return false;
}

bool npOpenNode( pNPnode node, void* dataRef );
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
	FILE* file = NULL;

	pData data = (pData) dataRef;

	//if node is a URL, DB rec or file path it will open it or run system app
	if ( npOpenNode( data->map.currentNode, data ) )
		return;

	//leave fullscreen so the OS file dialog box isn't fighting to be on top
	if( data->io.gl.fullscreen )										//zz debug workaround
	{	
		npCtrlCommand( kNPcmdFullscreen, data );
		file = npFileDialog( data->io.file.csvPath, kNPfileDialogOpen, data );
		npCtrlCommand( kNPcmdFullscreen, data );
	}
	else
		file = npFileDialog( NULL, kNPfileDialogOpen, data );

	npFileOpenAuto( NULL, file, data );
//	npCSVtoMap (file, kNPmapNode, dataRef);								//zz debug move this
	npPostTool( NULL, data );											//zz-s debug, remove... or change defaults
	
	printf( "Sync Tags...\n" );
		npSyncTags( dataRef );											//zz debug upgrade this
		npPostMsg( "Sync Done!", kNPmsgCtrl, dataRef );

	return;
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
//	char	msg[256];

	pData data =(pData) dataRef;

	//file set based on (user) selected table types
//	if ( data->io.file.saveMapSet[kNPmapGlobals] )

//		sprintf( filePath, "%s%s%s", dirPath, name, "globals.csv" );
//		sprintf( msg, "Saving: %s", filePath );
//		npPostMsg( msg, kNPmsgCtrl, data );

	//	data->io.gl.screenGrab = 2;	//zz debug triggers a screenGrab timestamp not synced!!!		
		npScreenGrabThumb( datasetName, kNPformatDDS,
							0, 0, kNPthumbWidth, kNPthumbHeight, data );

		result += npSaveMapToCSV( datasetName, kNPmapGlobals, data );
	
		result += npSaveMapToCSV( datasetName, kNPmapNode, data );

		result += npSaveMapToCSV( datasetName, kNPmapTag, data );

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


void npPathNameToTag( pNPnode node, char* path );
//-----------------------------------------------------------------------------
void npPathNameToTag( pNPnode node, char* path )
{

	int pathMax = 64;//kNPtagTitleMax;
	int trim = 0;
	int	pathSize = 0;

	pNPtag tag = node->tag;


	//auto assing a recordID, if none
	if ( !node->recordID )
	{
		node->recordID = node->id;
		node->tableID = 0;
	}
	node->tag->tableID = node->tableID;
	node->tag->recordID = node->recordID;


	//set trim point to the last slash, just before the filename
	tag->title[0] = '\0';
	//trim = pathSize = strnlen (path, kNPmaxPath);
	trim = strlen(path); // lde
	trim -= 2;
	while (	trim > 0 
			&& path[trim] != '/' 
			&& path[trim] != '\\' )
		trim--;
	trim++;

	//copy dir name to tag title
	pathSize = pathSize - trim;
	if( pathSize < kNPtagTitleMax )
		strncat( tag->title, &path[trim], pathSize );
	else
		strncat( tag->title, &path[trim], kNPtagTitleMax );
	//	strcpy( node->tag->title, "..." );		

	npUpdateTag( tag );
}


pNPnode npNewDirNode( pNPnode parent, char* path, int type, int view, void* dataRef );
//MSW file attributes
/*
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
//creates a new node attached to parent, NULL parent creates root pin
//sets the new node attributes based on the file/dir path string and type
//default file type = 0 and dir = 1, app/exe = 2, file.txt = 3, csv, jpg.. 
#include "../../libs/soil/src/SOIL.h"	
//---------------------------------------------------------------------------
pNPnode npNewDirNode( pNPnode node, char* path, int type, int view, void* dataRef )
{
	int texID = 0;
	unsigned int soilFlags = 0;
	
	pData data = (pData) dataRef;

	//set node attrib's based on file path, name, type, size, date, content
	if ( type )//== kNPfileDir )
	{
		if( node->branchLevel > 0 )
		{
			node->topo = kNPtopoCylinder;
			node->geometry = kNPgeoCylinder;
		}
		node->textureID++;

		node->colorIndex = node->branchLevel;

		npSetIndexColor( &node->color, &node->colorIndex, NULL );
	
		return node;
	}
	else	//default file viz node
	{
//		node->topo = kNPtopoCylinder; //kNPtopoPin;
//		node->geometry = kNPgeoCylinder; //kNPgeoPin;
		node->colorIndex = node->branchLevel;
//		npSetColorIndex(
	}

//	node->colorIndex = 3; //blue 
	node->color.a = 180;  //dark transparent blue
//		npSetIndexColor( &node->color, &node->colorIndex, NULL );
//	return node;
	//node->translate.x = 360.0f / dirItemCount;
	//node->translate.y = 15.0f * latOffset(thisItem, totalItems)

	printf("file ext: %0.60s\n", &node->tag->title[node->tag->titleSize - 4] );
	if( strncmp( &node->tag->title[node->tag->titleSize - 4], ".csv", 4 ) == 0 )
	{
	//	node->topo = kNPtopoSphere;
		node->geometry = kNPgeoSphere;		//kNPtopoTree kNPtopoForest
	}
	else if( strncmp( &node->tag->title[node->tag->titleSize - 4], ".dds", 4 ) == 0
		  || strncmp( &node->tag->title[node->tag->titleSize - 4], ".bmp", 4 ) == 0
		  || strncmp( &node->tag->title[node->tag->titleSize - 4], ".tga", 4 ) == 0
		  || strncmp( &node->tag->title[node->tag->titleSize - 4], ".jpg", 4 ) == 0
		  || strncmp( &node->tag->title[node->tag->titleSize - 5], ".jpeg", 5 ) == 0 )
	{
		
		if( view )
		{
	//		if( data->io.gl.texture.mipmaps inverY ntscSafeRGB compressDXT )
			soilFlags = SOIL_FLAG_INVERT_Y | SOIL_FLAG_MIPMAPS;
				// | SOIL_FLAG_MIPMAPS			//disabling breaks RGBA textures
				// | SOIL_FLAG_NTSC_SAFE_RGB	//we want the entire RGB spectrum
				// | SOIL_FLAG_COMPRESS_TO_DXT	//no lossy compression, faster too
			texID = SOIL_load_OGL_texture
			(
				path,
				SOIL_LOAD_AUTO,
				SOIL_CREATE_NEW_ID,
				soilFlags
			);
		
			//the last texture loaded is the texture count, non-loads return a texture=0
			if( texID )
			{
				node->textureID = texID;
				data->io.gl.textureCount = texID;
				printf ("Loaded textureID: %d\n", texID);
			}

			node->colorIndex = 19;		//zz use color names, kNPwhite = 19
			node->color.a = 255;		//full opacity
		}

			node->topo = kNPtopoCylinder;
			node->geometry = kNPgeoCylinder;
			
			if ( node->branchLevel > 0 )
			{
				if( node->size < 1048576 )
					node->scale.x = node->scale.y = node->scale.z = 0.5f;

				node->translateRate.x = 0.1f;
				node->translateRate.y = 0.05f;
				
				if( node->id%2 == 0 )
					node->rotateRate.z = 0.125f;	//id is even
				else
					node->rotateRate.z = -0.125f;	//odd
			}
		

		//node->topo = kNPtopoCylinder;
		node->geometry = kNPgeoCylinder;
	}
	else if( strncmp( &node->tag->title[node->tag->titleSize - 2], ".h", 2 ) == 0 )
	{
		//tokenize #define #include and function declarations
		//create link nodes for the #include files

	//	node->topo = kNPtopoPin;
		node->geometry = kNPgeoTetrahedron;
		node->colorIndex = 1;
	}
	else if( strncmp( &node->tag->title[node->tag->titleSize - 2], ".c", 2 ) == 0 )
	{	
		//tokenize 
	//	node->topo = kNPtopoPin;
		node->geometry = kNPgeoDodecahedronWire;
		node->colorIndex = 2;
	}
	else if( strncmp( &node->tag->title[node->tag->titleSize - 4], ".txt", 4 ) == 0 )
	{
		//raster the text to create a preview texture map
	//	node->topo = kNPtopoPin;
		node->geometry = kNPgeoCube;
		node->colorIndex = 0;
	}

	npSetIndexColor( &node->color, &node->colorIndex, NULL );
	//set transparency based on age of item updated/modified/created
	//node->color.a = (unsigned char)( 256.0f * timeSinceUpdated / timeRange );
	return node;
}


//builds a node tree of the file directory structure
//---------------------------------------------------------------------------
/* temp lde
bool npNewDirTree(const char *basePath, pNPnode parent, void* dataRef)
{
    WIN32_FIND_DATA fdFile;
    HANDLE hFind = NULL;

    char sPath[kNPmaxPath];
	bool isDir = false;
	
	unsigned long fileSizeHi = 0;
	unsigned long fileSizeLo = 0;

	int maxItems = 0;	//maximum total items to add
	int depth = 0;		//how many levels deep to go
	int breadth = 0;	//how wide (siblings) to go
						//zz add advanced tree search/filter/query logic

	int loadFamiliarFiles = 0;		//zz upgrade with filter logic
	int viewContents = true;		//preview of file contents

	pData data = (pData) dataRef;
	pNPnode node = NULL;

    //Specify a file mask. *.* = We want everything!
    sprintf(sPath, "%s/*.*", basePath);
//	sprintf(sPath, "%s\\*.*", basePath);

    if((hFind = FindFirstFile(sPath, &fdFile)) == INVALID_HANDLE_VALUE)
    {
        printf("err 7357 - Path not found: [%s]\n", basePath);
        return false;
    }

    do
    {
		isDir = fdFile.dwFileAttributes &FILE_ATTRIBUTE_DIRECTORY;
		fileSizeHi = fdFile.nFileSizeLow;
		fileSizeLo = fdFile.nFileSizeHigh;

        //Find first file will always return "."
        //    and ".." as the first two directories.
        if(strcmp(fdFile.cFileName, ".") != 0
                && strcmp(fdFile.cFileName, "..") != 0)
        {
            //append current file/dir item to the basePath (parent dir)
            sprintf(sPath, "%s/%s", basePath, fdFile.cFileName);    // "%s\\%s"
				// printf("fullPath: %s\n", sPath);

			node = npNodeNew( kNodePin, parent, dataRef );
			node->size = (int)fileSizeLo;
			
			npPathNameToTag( node, sPath );		//set the nodes tag title
	
			npNewDirNode( node, sPath, isDir, true, dataRef );		//apply file attrib's to node
	
			if( node->id < 100 )
				printf( "id: %6d level: %3d name: %.60s\n",
						node->id, node->branchLevel, node->tag->title );

			//if Folder (not file) then recursively call to create dir tree
			if( isDir )
				 npNewDirTree( sPath, node, dataRef ); //Recursion, I love it!
		}
    }
    while(FindNextFile(hFind, &fdFile)); //Find the next file.

    FindClose(hFind); //Always, Always, clean things up!

    return true;
}
lde */ 

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
	char	msg[256];

	pData data = (pData) dataRef;

	dirPath = data->io.file.csvPath;
	data->io.file.loading = true;

 //   listdir(".", 0);

//ListDirectoryContents("C:\\data\\code\\antz\\sdk", dataRef);
//	ListDirectoryContents("C:/data/code/antz/sdk", dataRef);
	
// npNewDirTree("C:/data/code", NULL, dataRef);
//	npNewDirTree(data->io.file.appPath, NULL, dataRef);

	//	npSelectNode( node, data );


	sprintf( filePath, "%s%s%s%s", dirPath, datasetName, 
			npMapTypeName( kNPmapGlobals, data ), ".csv" );
	
	sprintf( msg, "Loading: %s", filePath );
	npPostMsg( msg, kNPmsgCtrl, data );
	result += npOpenGlobalsCSV( filePath, 1, 15, data );

	sprintf( filePath, "%s%s%s%s", dirPath, datasetName, 
			npMapTypeName( kNPmapNode, data ), ".csv" );
	sprintf( msg, "Loading: %s", filePath );
	npPostMsg (msg, kNPmsgCtrl, data );
	result += npFileOpenAuto( filePath, NULL, data );

	sprintf( filePath, "%s%s%s%s", dirPath, datasetName, 
			npMapTypeName( kNPmapTag, data ), ".csv" );
	sprintf( msg, "Loading: %s", filePath );
	npPostMsg( msg, kNPmsgCtrl, data );
	result += npFileOpenAuto( filePath, NULL, data );

	return result;
}


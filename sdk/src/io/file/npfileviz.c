/* -----------------------------------------------------------------------------
*
*  npfileviz.c
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

#include "npfileviz.h"

#include "../../data/npmapfile.h"
#include "../npgl.h"
#include "SOIL.h"

void npPathNameToTag (pNPnode node, char* path, void* dataRef );
pNPnode npNewDirNode (pNPnode parent, char* path, int type, int view, void* dataRef);

/// @todo separate out OS specific code and texture loading

//builds a node tree of the file directory structure
//---------------------------------------------------------------------------
int npNewDirTree (const char *basePath, pNPnode parent, int i, void* dataRef)
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
		i++;

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
			
			npPathNameToTag( node, sPath, dataRef );		//set the nodes tag title
	
			npNewDirNode( node, sPath, isDir, true, dataRef );		//apply file attrib's to node
	
			if( i <= 12 )
				printf( "id: %6d level: %2d name: %.50s\n",
						node->id, node->branchLevel,
						&node->tag->title[node->tag->labelHead] );
			else if( i % 100 == 0 )
				printf( "." );			///< print a dot every 100 files

			//if Folder (not file) then recursively call to create dir tree
			if( isDir )
				 npNewDirTree( sPath, node, i, dataRef ); //Recursion, I love it!
		}
    }
    while( FindNextFile( hFind, &fdFile) ); //Find the next file.

    FindClose( hFind); //Always, Always, clean things up!

    return true;
}


//-----------------------------------------------------------------------------
void npPathNameToTag( pNPnode node, char* path, void* dataRef )
{
	int	pathSize = 0;				///< Length of the complete file path
	int filenameSize = 0;			///< Length of the filename
	int formatSize = 0;				///< Length of the HTML formatting
	int trim = 0;					///< Position of the filenames first char

	pNPtag tag = node->tag;

	//auto assing a recordID, if none
	if ( !node->recordID )
	{
		node->recordID = node->id;
		node->tableID = 0;
	}
	node->tag->tableID = node->tableID;
	node->tag->recordID = node->recordID;

	//trim = pathSize = strnlen (path, kNPmaxPath);
	pathSize = trim = strlen(path); // lde
	trim -= 2;
	while (	trim > 0 && path[trim] != '/' && path[trim] != '\\' )
		trim--;
	trim++;

	filenameSize = pathSize - trim + 1;
	formatSize = 16;	///< Number of additional char's to format as HTML

	/// Format the tag as HTML with URL path and filename as the label,
	/// check that the HTML formatted path and filename is not to big.
	if( kNPtagTitleMax >= (pathSize + filenameSize + formatSize) )
	{
		//set trim point to the last slash, just before the filename
		strcpy( tag->title, "<a href=\"\0" );
		strcat( tag->title, path );
		strcat( tag->title, "\">" );
		strcat( tag->title, &path[trim] );
		strcat( tag->title, "</a>" );
	}
	else
	{
		strncat( tag->title, &path[trim], kNPtagTitleMax );
		npPostMsg( "warn 7502 - file tag exceeds kNPtagTitleMax", kNPmsgWarn, dataRef );
	}

	/// Update the tag display properties
	npUpdateTag( tag );
}


//creates a new node attached to parent, NULL parent creates root pin
//sets the new node attributes based on the file/dir path string and type
//default file type = 0 and dir = 1, app/exe = 2, file.txt = 3, csv, jpg.. 

//---------------------------------------------------------------------------
pNPnode npNewDirNode( pNPnode node, char* path, int type, int view, void* dataRef )
{
	int fileType = 0;
	int texID = 0;
	unsigned int soilFlags = 0;
	char fileName[kNPmaxName] = {'\0'};
	
	pData data = (pData) dataRef;
	pNPtag tag = node->tag;

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

//	printf("file ext: %0.4s\n", &node->tag->title[node->tag->titleSize - 8] );

	// determine file type by extension
	strncat( fileName, &tag->title[tag->labelHead], 
			 (tag->labelTail - tag->labelHead + 1) );

	fileType = npGetFileType(fileName, dataRef);
//	printf( "%d   %s\n", fileType, fileName );
	switch( fileType )//npGetFileType(fileName, dataRef) )
	{
		case kNPfileH :
			//tokenize #define #include and function declarations
			//create link nodes for the #include files
			//	node->topo = kNPtopoPin;
			node->geometry = kNPgeoTetrahedron;
			node->colorIndex = 1;
			break;
		case kNPfileC :
		case kNPfileCPP :
			node->geometry = kNPgeoDodecahedronWire;
			node->colorIndex = 2;
			break;
		case kNPfileCSV :
			node->geometry = kNPgeoSphere;
			break;
		case kNPfileTXT :
			node->geometry = kNPgeoCube;
			node->colorIndex = 0;
			break;
		case kNPfileBMP :
		case kNPfileDDS :
		case kNPfileGIF :
		case kNPfileJ2K :
		case kNPfileJPG :
		case kNPfilePNG :
		case kNPfileRAW :
		case kNPfileTGA :
		case kNPfileTIFF :

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

					
			break;
		default :
			
			break;
	}

	npSetIndexColor( &node->color, &node->colorIndex, NULL );
	//set transparency based on age of item updated/modified/created
	//node->color.a = (unsigned char)( 256.0f * timeSinceUpdated / timeRange );
	return node;
}


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


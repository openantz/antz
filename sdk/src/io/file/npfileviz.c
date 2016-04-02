/* -----------------------------------------------------------------------------
*
*  npfileviz.c
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

#include "npfileviz.h"

#include "../../os/npos.h"
#include "../../data/npmapfile.h"
#include "../npgl.h"
#include "SOIL.h"

void npPathNameToTag (pNPnode node, char* path, void* dataRef );
pNPnode npNewFileVizNode (pNPnode parent, char* path, int type, int view, void* dataRef);


/// Recursively build node trees of the directory structure
//------------------------------------------------------------------------------
int npNewFileViz (const char *basePath, pNPnode parent, int i, void* dataRef)
{
	int result = 0;
	pNPfileRef fRef = NULL;

    char sPath[kNPmaxPath];

	//zz add advanced tree search/filter/query logic
	int maxItems = kNPnodeMax;	//maximum total items to add
	int depth = 27;				//how many directory levels deep to go
	int breadth = 1024;			//how many items per directory to display

	pData data = (pData) dataRef;
	pNPnode node = NULL;

	fRef = nposNewFileRef( data );

	result = nposFindFirstFile( fRef, basePath, "*.*", data );
	if( result != 1 )
		return false;	// err or empty folder

	do
    {
		i++;

		// append current file/dir item to the basePath (parent dir)
        sprintf(sPath, "%s/%s", basePath, fRef->fdFile.cFileName);

		// create a new node with parameters based on the file attributes
		node = npNewFileVizNode( parent, sPath, fRef->isDir, true, data );

		// print a few of the filenames then dots for every 100 files
		if( i <= 12 )
			printf( "id: %7d level: %2d name: %.50s\n",
					node->id, node->branchLevel, fRef->fdFile.cFileName );
		else if( i % 100 == 0 )
			printf( "." );

		// if Folder (not a file) then recursively call to create dir tree
		if( fRef->isDir && node->branchLevel < depth )
			 npNewFileViz( sPath, node, i, data );			// recursion
    }
	while( nposFindNextFile( fRef )
			&& i < breadth
			&& data->map.nodeCount < maxItems );	// next file within limits

    nposFindClose( fRef, data );		// always clean up!

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
pNPnode npNewFileVizNode( pNPnode parent, char* path, int type, int view, void* dataRef )
{
	int fileType = 0;
	int texID = 0;
	unsigned int soilFlags = 0;
	char fileName[kNPmaxName] = {'\0'};
	
	pData data = (pData) dataRef;
	pNPnode node = NULL;
	pNPtag tag = NULL;

	node = npNodeNew( kNodePin, parent, data );
	tag = node->tag;
	//node->size = (int)fRef->sizeLo;
		
	npPathNameToTag( node, path, data );		//set the nodes tag title

//	node = parent;
//	tag = node->tag;

	//set node attrib's based on file path, name, type, size, date, content
	if ( type )//== kNPfileDir )
	{
		if( node->branchLevel > 0 && parent->childCount > 1 )
		{
			node->rotate.x = 30.0f;
			node->rotate.y = 30.0f * parent->childCount;
		}
		
		node->topo = kNPtopoRod;
		node->geometry = kNPgeoCylinder;

		node->textureID++;

		node->colorIndex = node->branchLevel;

		npSetIndexColor( &node->color, &node->colorIndex, NULL );
	
		return node;
	}
	else	//default file viz node
	{
		node->colorIndex = node->branchLevel;
	}

//	node->colorIndex = 3; //blue 
	node->color.a = 180;  //dark transparent blue

//	printf("file ext: %0.4s\n", &node->tag->title[node->tag->titleSize - 8] );

	// determine file type by extension
	strncat( fileName, &tag->title[tag->labelHead], 
			 (tag->labelTail - tag->labelHead + 1) );

	fileType = npGetFileTypeCat( NULL, fileName, dataRef );
//	printf( "%d   %s\n", fileType, fileName );
	switch( fileType )
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

			/// Load file as a texture map and apply to file node
			if( view )
			{
				node->textureID = npLoadTexture( path, fileType, data );

				node->colorIndex = 19;		//zz use color names, kNPwhite = 19
				node->color.a = 255;		//full opacity
			}

			node->topo = kNPtopoCube;
			node->geometry = kNPgeoCube;
			
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
					
			break;
		default :
			node->topo = kNPtopoRod;
			break;
	}

	npSetIndexColor( &node->color, &node->colorIndex, NULL );
	//set transparency based on age of item updated/modified/created
	//node->color.a = (unsigned char)( 256.0f * timeSinceUpdated / timeRange );
	return node;
}


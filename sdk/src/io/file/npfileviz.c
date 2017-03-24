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


void npPathNameToTag( pNPnode node, char* path, void* dataRef);
pNPnode npFileVizNode( pNPnode parent, char* path, pNPfileRef fRef, 
						  int preview, void* dataRef);


/// Recursively build node trees of the directory structure
//------------------------------------------------------------------------------
int npNewFileViz ( char *dirPath, pNPnode parent, int i, void* dataRef)
{
	int result = 0;
	pNPfileRef fRef = NULL;

    char sPath[kNPmaxPath];

	//zz add advanced tree search/filter/query logic
	int maxItems = kNPnodeMax;		// max total items to add
	int depth = kNPbranchLevelMax;	// max directory level depth

	pData data = (pData) dataRef;
	pNPnode node = NULL;

	/// @todo add global init for fileviz and separate recursive function
	node = data->map.node[kNPnodeRootGrid];
	node->textureID = 0;

	fRef = nposNewFileRef( data );

	result = nposFindFirstFile( fRef, dirPath, "*.*", data );
	if( result != 1 )
		return false;	// err or empty folder

	do
    {
		i++;

		// create a new node with parameters based on the file attributes
		node = npFileVizNode( parent, dirPath, fRef, false, data );

		// print a few of the filenames then dots for every 100 files
		if( i <= 5 )
			printf( "id: %7d level: %2d name: %.50s\n",
					node->id, node->branchLevel, fRef->fdFile.cFileName );
		else if( i % 100 == 0 )
			printf( "." );

		// if Folder (not a file) then recursively call to create dir tree
		if( fRef->isDir && node->branchLevel < depth )
		{
			// append current file/dir item to the basePath (parent dir)
			sprintf(sPath, "%s/%s", dirPath, fRef->fdFile.cFileName);
			npNewFileViz( sPath, node, i, data );					// recursion
		}
    }
	while( nposFindNextFile( fRef )
//			&& i < breadth
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
//pNPnode npNewFileVizNode( pNPnode parent, char* path, int type, int view, void* dataRef )
pNPnode npFileVizNode( pNPnode parent, char* path, pNPfileRef fRef, 
						int preview, void* dataRef )
{
	int fileType = 0;
	int fileCat = 0;
	int texID = 0;
	float scale = 1.0f;
	char fileName[kNPmaxName] = {'\0'};
	char filePath[kNPmaxPath] = {'\0'};
	
	pData data = (pData) dataRef;
	pNPnode node = NULL;
	pNPtag tag = NULL;

	node = npNodeNew( kNodePin, parent, data );
	tag = node->tag;

	node->size = (int)fRef->sizeLo;
		
	/// @todo make relative FileViz paths, tags would be just the file name
	// directories node names should be full path or partial?
	sprintf( filePath, "%s/%s", path, fRef->fdFile.cFileName );
	npPathNameToTag( node, filePath, data );		//set the nodes tag title

	/// default node color based on level
	node->colorIndex = node->branchLevel;

	//set node attrib's based on file path, name, type, size, date, content
	if ( fRef->isDir )
	{
		if( node->branchLevel > 0 && parent->childCount > 1 )
		{
			node->rotate.x = 35.0f;
			node->rotate.y = 140.0f * node->id;
		}
		
		node->topo = kNPtopoRod;
		node->geometry = kNPgeoCylinder;

		node->tagMode = 1;

		node->colorIndex = node->branchLevel;
		npSetIndexColor( &node->color, &node->colorIndex, NULL );
		npSetTagOffset( node );

		return node;
	}

//	node->colorIndex = 3; //blue 
	node->color.a = 180;  //dark transparent blue

//	printf("file ext: %0.4s\n", &node->tag->title[node->tag->titleSize - 8] );

	// determine file type by extension
	strncat( fileName, &tag->title[tag->labelHead], 
			 (tag->labelTail - tag->labelHead + 1) );

	fileType = npGetFileTypeCat( &fileCat, fileName, dataRef );

	switch( fileCat )
	{	case kNPfileCatCode :
			switch( fileType )
			{	case kNPfileH :
					//tokenize #define #include and function declarations
					//create link nodes for the #include files
					//	node->topo = kNPtopoPin;
					node->geometry = kNPgeoTetrahedronWire;
					node->colorIndex = 2;
					break;
				case kNPfileC :
					node->geometry = kNPgeoDodecahedron;
					scale = (float)sqrt( ((float)fRef->fdFile.nFileSizeLow / 100000.0f) 
								 * ((float)fRef->fdFile.nFileSizeLow / 100000.0f) )
								 + 0.3f;
					node->scale.x = node->scale.y = node->scale.z = scale;
					node->colorIndex = 1;
					break;
				case kNPfileCPP :
					node->geometry = kNPgeoOctahedron;
					node->colorIndex = 1;
					break;
				default :
					node->geometry = kNPgeoOctahedronWire;
					node->colorIndex = 3;
					break;
			}
			break;
		case kNPfileCatTable : //kNPfileCSV :
			node->geometry = kNPgeoSphere;
			break;
		case kNPfileCatText :
			node->geometry = kNPgeoCube;
			node->colorIndex = 0;
			node->scale.x = node->scale.y = node->scale.z = 0.5f;
			break;
		case kNPfileCatImage :
			/// Load file as a texture map and apply to file node
			if( preview )
			{
				node->textureID = npLoadTexture( filePath, data );

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
			//node->topo = kNPtopoRod;
			break;
	}

	npSetIndexColor( &node->color, &node->colorIndex, NULL );
	npSetTagOffset( node );

	/// invert large objects to help see potential hidden geos inside
	if(node->scale.z > 1.1f )
		node->scale.z *= -1.0f;

	//set transparency based on age of item updated/modified/created
	//node->color.a = (unsigned char)( 256.0f * timeSinceUpdated / timeRange );
	return node;
}


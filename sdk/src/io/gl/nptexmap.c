/* -----------------------------------------------------------------------------
*
*  nptexmap.c
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

#include "nptexmap.h"

#include "../../npdata.h"
#include "../../os/npos.h"
#include "../npglut.h"
#include "../../data/npmapfile.h"
#include "../file/npfreeimage.h"	// needs to be after other includes
#include "../npfile.h"

#include "SOIL.h"					// used for DDS files and screengrab


//------------------------------------------------------------------------------
#define kNPtexListMax 2000
void npInitTexMap (void* dataRef)
{
	pData data = (pData) dataRef;
	pNPtexmap texmap = NULL; 
	int i = 0;

	for(i = 0; i < kNPtexListMax; i++)
	{
		texmap = &data->io.gl.texmap[i];
		texmap->loaded = 0;
		texmap->path[0] = '\0';
		texmap->filename[0] = '\0';
		texmap->channels = 0;
		texmap->height = 0;
		texmap->width = 0;
		texmap->image = NULL;
		texmap->extTexId = 0;
		texmap->intTexId = 0;
	}

	return;
}

#define kNPextId 1
#define kNPintId 2


pNPtexmap npTexlistSearchFile(char* filename, char* path, void* dataRef)
{
	pData data = (pData) dataRef;
	int count = data->io.gl.textureCount;
	pNPtexmap tex = NULL;
	int i = 0;

	for(i = 0; i < kNPtexListMax; i++)
	{
		tex = &data->io.gl.texmap[i];
		if( ( strcmp(tex->filename, filename) == 0 ) && ( strcmp(tex->path, path) == 0 ) )  
			return tex;
	}

	return NULL;
}

pNPtexmap npTexlistSearchId(int var, void* val, void* dataRef);
pNPtexmap npTexlistSearchId(int var, void* val, void* dataRef)
{
	pData data = (pData) dataRef;
	pNPtexmap tex = NULL; 
	int count = data->io.gl.textureCount;
	int i = 0;
	int* valInt = NULL;

	switch(var)
	{
		case kNPextId:
			valInt = (int*)val;		
			for(; i < kNPtexListMax; i++)
			{
				tex = &data->io.gl.texmap[i];
				if( tex->extTexId == (*valInt) )
					return tex;
			}
			break;	
		case kNPintId:
			valInt = (int*)val;		
			for(; i < kNPtexListMax; i++)
			{
				tex = &data->io.gl.texmap[i];
				if( tex->intTexId == (*valInt) )
					return tex;
			}
			break;
	}

	return NULL;
}

int npGetUnusedExtTexId(void* dataRef);
int npGetUnusedExtTexId(void* dataRef)
{
	pData data = (pData) dataRef;
	pNPtexmap texmap = NULL;
	int i = 1;
	int extId = 1;

	for(i = 1; i < kNPtexListMax; i++)
	{
		texmap = &data->io.gl.texmap[i];
		if(texmap->extTexId == extId)
		{
			extId++;
			if(extId == kNPtexListMax)
				return 0;

			i = 1;
		}
	}

	for( i = 1; i < kNPtexListMax; i++)
	{
		if( data->io.gl.texmap[i].extTexId == 0 )
			break;
	}

	data->io.gl.texmap[i].extTexId = extId;

//	printf("New ext tex id : %d\n", extId);

	return extId;
}

pNPtexmap npGetUnusedTexMap(void* dataRef)
{
	pData data = (pData) dataRef;
	pNPtexmap texmap = NULL;
	pNPtexmap defTex = &data->io.gl.texmap[0];
	int i = 0;
	int res = 0;
	int texSize = sizeof(NPtexmap);


	/// texmap[0] is default

	for(i = 1; i < kNPtexListMax; i++)
	{
		texmap = &data->io.gl.texmap[i];
		res = memcmp(defTex,texmap,texSize); 
		if(res == 0)
			return texmap;

	}

	return NULL;
}

pNPtexmap npAddTexMap(int extTexId, char* filename, char* path, void* dataRef)
{
	pData data = (pData) dataRef;
	pNPtexmap texmap = &data->io.gl.texmap[0];
	pNPtexmap newTex = NULL;
	pNPtexmap texA = NULL;
	pNPtexmap texB = NULL;
	int i = 0;
	char* foundPath = NULL;

	if(!filename)
		return NULL;

	if(extTexId == 0)
		extTexId = npGetUnusedExtTexId(dataRef);

	texA = npTexlistSearchId(kNPextId, &extTexId, dataRef);
	if(texA)
	{
		if(texA->filename[0] == '\0' && filename[0] != '\0') 
			strcpy(texA->filename, filename);

		if(texA->path[0] == '\0' && path[0] != '\0')
			strcpy(texA->path, path);

		return texA;
	}

	texB = npTexlistSearchFile(filename, path, dataRef);
	if(texB)
	{
		return texB;
	}


	newTex = npGetUnusedTexMap(dataRef);
	if(newTex)
	{
		newTex->extTexId = extTexId;	
		strcpy(newTex->filename, filename);
		if(path[0] == '\0' && filename[0] != '\0')
		{

			foundPath = npSearchPathsForFile(filename, dataRef);
			foundPath[ (strlen(foundPath) - strlen(filename)) ] = '\0';

			if( (foundPath[strlen(foundPath)-1] == '\\') && (foundPath[strlen(foundPath)-2] == '\\') )
			{
				foundPath[strlen(foundPath)-1] = '\0';
				printf("modified found path : %s\n", foundPath);
			}

			strcpy(newTex->path, foundPath);
		}
		else
		{
			if(path[strlen(path)-1] == '\\' && path[strlen(path)-2] == '\\')
			{
				path[strlen(path)-1] = '\0';
			}

			strcpy(newTex->path, path);
		}

		newTex->intTexId = 0;
		newTex->loaded = 0;
	}

	return newTex;
}

//------------------------------------------------------------------------------
void npCloseTexMap (void* dataRef)		//clean-up
{
	return;
}

//------------------------------------------------------------------------------
void npUpdateTexMap (void* dataRef)							//add to ctrl loop, debug zz
{
	pData data = (pData) dataRef;
	pNPtexmap texmap = NULL;
	char filepath[256] = {'\0'};
	char* foundPath = NULL;
	int i = 0;

//	printf("npUpdateTexMap\n");
	for( i = 0; i < 2000; i++ )
	{
		texmap = &data->io.gl.texmap[i];
		if(texmap->extTexId > 0 && texmap->intTexId == 0 && texmap->loaded == 0 &&
			texmap->filename[0] != '\0' && texmap->path[0] != '\0')
		{
			/*
			printf("texmap->extTexId : %d\n", texmap->extTexId);
			printf("texmap->path : %s\n", texmap->path);
			printf("texmap->filename : %s\n", texmap->filename);
			*/
			sprintf(filepath, "%s%s", texmap->path, texmap->filename);
			texmap->intTexId = npLoadTexture(filepath, 0, dataRef);
			if(texmap->intTexId > 0)
				texmap->loaded = 1;
			else if(texmap->intTexId == 0)
			{
				texmap->loaded = 0;
				foundPath = npSearchPathsForFile(texmap->filename, dataRef);
				if(foundPath != NULL)
				{
					foundPath[strlen(foundPath)-strlen(texmap->filename)] = '\0';	
					texmap->path[0] = '\0';
					strcpy(texmap->path, foundPath);
					free(foundPath);
				}
			}
		}
	}

	return;
}
/*
*/
/// Load a texture map from the specified file path of specified image type.
/// If fileType = 0 then image type determined by the file extension.
//------------------------------------------------------------------------------
int npLoadTexture( const char* filePath, int fileType, void* dataRef)
{
	int textureID = 0;
	int i = 0;
	pData data = (pData) dataRef;
	pNPtexmap tex = NULL;
	char filename[256] = {'\0'};
	char path[256] = {'\0'};

	if( filePath[0] != '\0' )
	{
		npGetFileNameFromPath( filePath, &filename[0], dataRef);
		strcpy(path, filePath);
		path[( strlen(filePath) - strlen(filename) )] = '\0';
		tex = npTexlistSearchFile(filename, path, dataRef);

		if(tex != NULL && tex->intTexId)
		{
			/// tex search found it
//			printf("235 found texture 235\n");
		//	return tex->intTexId;	
		}
	}

	

	// determine the file type
	if( !fileType )
		fileType = npGetFileTypeCat( NULL, filePath, dataRef );

	/// Using SOIL for efficient direct memory DDS file loading.
	if( fileType == kNPfileDDS ) 
	{
		textureID = SOIL_load_OGL_texture ( filePath,
			SOIL_LOAD_AUTO,
			SOIL_CREATE_NEW_ID,
			SOIL_FLAG_INVERT_Y |
			SOIL_FLAG_MIPMAPS );	//disabling breaks RGBA textures
	}
	else	
	{	/// Using FreeImage (addon) for all other image types.
#ifdef NP_ADDON_FREEIMAGE
	textureID = npfiLoadTexture( filePath, data );
#else
	textureID = SOIL_load_OGL_texture
	(
		filePath,
		SOIL_LOAD_AUTO,
		SOIL_CREATE_NEW_ID,
		SOIL_FLAG_INVERT_Y |
		SOIL_FLAG_MIPMAPS //|			//disabling breaks RGBA textures
		// | SOIL_FLAG_NTSC_SAFE_RGB	//we want the entire RGB spectrum
		// | SOIL_FLAG_COMPRESS_TO_DXT	//no lossy compression, faster too
	);
#endif
	}

	//the last texture loaded is the texture count, non-loads return a texture=0
	if( textureID )
	{
		data->io.gl.textureCount = textureID;
		if(tex == NULL)
		{
			tex = npGetUnusedTexMap(dataRef);
			tex->channels = 0;
			tex->extTexId = npGetUnusedExtTexId(dataRef);
			strcpy(tex->filename, filename);
			for(i = 0; i < (int)strlen(tex->filename); i++)
			{
				tex->filename[i] = tolower(tex->filename[i]);
			}

			strcpy(tex->path, path);
			tex->height = 0;
			tex->width = 0;
			tex->image = NULL;
			tex->intTexId = textureID;
			tex->loaded = 1;
		}
		else
			tex->intTexId = textureID;
	}
	else
		printf ("err 4301 - failed to load image: %s\n", filePath);

	return textureID;
}

//Textures, fonts, display lists, etc... can all be shared provided that:
//All rendering contexts of a shared display list must use an identical pixel format.
//http://www.opengl.org/discussion_boards/ubbthreads.php?ubb=showflat&Number=79299&page=1
//wglShareLists() supports texture sharing across GL contexts
/// @ todo update legacy support to use texture table to match map*.jpg to id
//------------------------------------------------------------------------------

void npLoadTextures(void* dataRef) 
{
	int i = 0;
	int result = 0;
	int fileType = 0;
	int textureSize = 0;
	int fileNumber = 1;

	pNPfileRef fRef = NULL;

	char* filename = (char*)malloc(4096);

	unsigned int textureID;		//zz debug, allow loading textures at runtime
									//detect changes to data->io.file.mapPath
	pData data = (pData) dataRef;


	glGetIntegerv (GL_MAX_TEXTURE_SIZE, &textureSize);
	data->io.gl.maxTextureSize = textureSize;
	printf ("\nMax Texture Size: %dx%d\n", textureSize, textureSize);
	printf ("Loading Textures...\n");
	//	printf ("Larger textures down converted\n", textureSize);

	fRef = nposNewFileRef( data );
	
	/// Legacy support where we first load map*.jpg files then all others
	result = nposFindFirstFile( fRef, "usr/images/", "map*.jpg", data );
	if( result != 1 )
		return;		// err or empty folder

	do
    {
		i++;

		// print a few of the filenames then dots for every 100 files
		if( i <= 5 )
			printf( "%.70s\n", fRef->name );
		else if( i < 100 || i % 100 == 0 )
			printf( "." );

		sprintf(filename, "%susr\\images\\%s", data->io.file.appPath, fRef->name );
		//printf("file : %s\n", filename);

		// if Folder (not a file) then recursively call to create dir tree
		if( fRef->isDir )
		{
		//	npAddTexMap(dataRef); // recursion
		}
		else
		{
			printf("filename : %s\n", filename);
			textureID = npLoadTexture( filename, 0, data );
		}
    }
	while( nposFindNextFile( fRef ) );	// next file within limits

	/// Now we load all other textures
	result = nposFindFirstFile( fRef, "usr/images/", "*.*", data );
	if( result != 1 )
		return;		// err or empty folder

	do
    {
		// handle legacy support by skipping the ones we just laoded
		if( strncmp(fRef->name, "map", 3) == 0
			&& npGetFileTypeCat(NULL, fRef->name, data) == kNPfileJPG )
			continue;

		i++;


		// print a few of the filenames then dots for every 100 files
		if( i <= 5 )
			printf( "%.70s\n", fRef->name );
		else if(  i < 100 || i % 100 == 0 )
			printf( "." );

//		printf("fRef->name : %s\n", fRef->name);
		sprintf(filename, "%s%s%s", data->io.file.appPath ,"usr\\images\\", fRef->name );
//		printf("2 filename : %s\n", filename);

		// if Folder (not a file) then recursively call to create dir tree
		if( fRef->isDir )
		{
		//	npLoadTextures( sPath, data );			// recursion
		}
		else
			textureID = npLoadTexture( filename, 0, data );
    }
	while( nposFindNextFile( fRef ) );	// next file within limits

    nposFindClose( fRef, data );		// always clean up!

	if( data->io.gl.textureCount )
		printf ("\nDone Loading Textures\n\n");
	else
		printf ("No Textures Found!!!\n\n");
}

void npLoadGeos(void* dataRef);
void npLoadGeos(void* dataRef) 
{
	int i = 0;
	int result = 0;
	int fileType = 0;
	int textureSize = 0;
	int fileNumber = 1;

	pNPfileRef fRef = NULL;

	char* filename = (char*)malloc(4096);
	char path[256] = {'\0'};

	unsigned int textureID;		//zz debug, allow loading textures at runtime
									//detect changes to data->io.file.mapPath
	pData data = (pData) dataRef;

	printf ("Loading Geos...\n");

	fRef = nposNewFileRef( data );
	
	result = nposFindFirstFile( fRef, "usr/model/", "*.*", data );
	if( result != 1 )
		return;		// err or empty folder

	do
    {
		i++;

		// print a few of the filenames then dots for every 100 files
		if( i <= 5 )
			printf( "%.70s\n", fRef->name );
		else if(  i < 100 || i % 100 == 0 )
			printf( "." );

		printf("fRef->name : %s\n", fRef->name);
		sprintf(filename, "%s/%s", "usr/model/", fRef->name );
		printf("!!!!!!!-----filename : %s!!!!!!!!-------\n", filename);

		// if Folder (not a file) then recursively call to create dir tree
		if( fRef->isDir )
		{
		//	npLoadTextures( sPath, data );			// recursion
		}
		else
		{
			sprintf(path, "%s%s", data->io.file.appPath, "usr\\model\\");
			printf("---------123 path : %s--------\n", path);
			if( (strcmp(fRef->name, ".DS_Store") == 0) || (strcmp(fRef->name, "README") == 0) )
				continue;

			printf("\nnpAddGeo(0,0,0, NULL, %s, %s, dataRef)\n", fRef->name, path);
			npAddGeo(0,0,0, NULL, fRef->name, path, dataRef);
		}
    }
	while( nposFindNextFile( fRef ) );	// next file within limits

    nposFindClose( fRef, data );		// always clean up!

	/*
	if( data->io.gl.numModels )
		printf ("\nDone Loading Models\n\n");
	else
		printf ("No Models Found!!!\n\n");
	*/
}



/// grabs back buffer from current OpenGL context
/// this function needs to be called just before glSwapBuffers()
//------------------------------------------------------------------------------
int npScreenGrab( char* filePath, int type, int x, int y, int w, int h,  void* dataRef )
{
	int i = 0;
	int err = 0;
	int temp = 0; //store console level
	unsigned char* pixelBuf = NULL;
//	char filePath[kNPmaxPath];
	static char timeStamp[64];

	static unsigned char tempLine[65535];	//max fixed array size is 65535

	w = glutGet( GLUT_WINDOW_WIDTH );
	h = glutGet( GLUT_WINDOW_HEIGHT );

	pixelBuf = (unsigned char*)npMalloc( 0, w*h*3, dataRef );
	if (!pixelBuf) return 1;
   
    glPixelStorei( GL_PACK_ALIGNMENT, 1 );
    glReadPixels( 0, 0, w, h, GL_RGB, GL_UNSIGNED_BYTE, pixelBuf );
	
	//RGBA is good for DB thumbnails... see-through dataset snapshot
	//backbuffer needs to be black with alpha = 0
	//glReadPixels( 0, 0, w, h, GL_RGBA, GL_UNSIGNED_BYTE, &buf[0] );	 

	for (i=0; i < (h / 2); i++);
	{	
		memcpy( tempLine, &pixelBuf[i * w * 3], w );
		memcpy( &pixelBuf[i * w * 3], &pixelBuf[(h-i) * w * 3], w );
		memcpy( &pixelBuf[(h-i) * w * 3], tempLine, w );
	}

	nposTimeStampName( timeStamp );
	sprintf( filePath, "usr/images/%s.bmp", timeStamp );
    SOIL_save_image
		(
        filePath,
        SOIL_SAVE_TYPE_BMP, //SOIL_SAVE_TYPE_TGA, //SOIL_SAVE_TYPE_DDS
        w, h, 3,
        pixelBuf
        );

	return 0; //zz debug add error handling
}
/*
{
	w = glutGet( GLUT_WINDOW_WIDTH );
	h = glutGet( GLUT_WINDOW_HEIGHT );

	w = tempW = data->io.gl.width;
	h = tempH = data->io.gl.height;

	//construct the filename path
	nposTimeStampName( timeStamp );
	strcpy( filePath, data->io.file.mapPath );
	strcat( filePath, timeStamp );
	
	//resize to thumbnail and turn off HUD
	if( data->io.gl.screenGrab == 2 )
	{
		filePath[0] = '\0';
		strncat( filePath, data->io.gl.datasetName, kNPmaxPath - 4  );
		strcat( filePath, ".dds" );

		w = 480; // 320 // 160 // data->map.thumbSize.width
		h = 270; // 180 // 90  // data->map.thumbSize.height
		temp = data->io.gl.hud.console.level;
		data->io.gl.hud.console.level = 0;
		npGLResizeScene( w, h );
		npGLDrawScene( data );
		
		err = SOIL_save_screenshot		//grab backbuffer and write to file
			(
			filePath,
			//SOIL_SAVE_TYPE_BMP,
			//SOIL_SAVE_TYPE_TGA,
			SOIL_SAVE_TYPE_DDS, 
			0, 0, w, h
			);

		//restore screen size and re-render
		data->io.gl.hud.console.level = temp;
		npGLResizeScene(tempW, tempH);
		npGLDrawScene (data);
		sprintf(msg, "Saved Thumbnail: %s", filePath);
	}
	else
	{
		strcat( filePath, ".bmp" );
	//	strcat( filePath, ".tga" );
	
		npGLDrawScene (data);

		err = SOIL_save_screenshot		//grab backbuffer and write to file
			(
			filePath,
			SOIL_SAVE_TYPE_BMP,
			//SOIL_SAVE_TYPE_TGA,
			//SOIL_SAVE_TYPE_DDS, 
			0, 0, w, h
			);
		sprintf(msg, "Saved Screenshot: %s", filePath);
	}
	npPostMsg( msg, kNPmsgFile, data );

	data->io.gl.screenGrab = false;
}
*/

/// Grab a screenshot of the window and save it to a file as a thumbnail.
int npScreenGrabThumb( char* name, int type, int x, int y, int w, int h, void* dataRef )
{
	pData data = (pData) dataRef;

	char pathName[kNPmaxPath];

	pathName[0] = '\0';
	strncat( pathName, data->io.file.mapPath, kNPmaxPath );
	strncat( pathName, name, kNPmaxPath - strlen( pathName ) );
	
	data->io.gl.datasetName[0] = '\0';
	strncat( data->io.gl.datasetName, pathName, kNPmaxPath );
	data->io.gl.screenGrab = 2;	//zz debug triggers a screenGrab timestamp not synced!!!			

	return 0; //zz add error handling
}


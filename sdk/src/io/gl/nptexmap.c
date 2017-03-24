/* -----------------------------------------------------------------------------
*
*  nptexmap.c
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

#include "nptexmap.h"

#include "../../npdata.h"
#include "../../os/npos.h"
#include "../npgl.h"
#include "../npfile.h"				//zz models

#ifdef NP_ADDON_FREEIMAGE
	#include "../file/npfreeimage.h"	// needs to be after other includes
#endif
#ifdef NP_ADDON_SOIL
	#include "SOIL.h"
#endif


/// @todo support sRGBA using GL_EXT_texture_sRGB
//------------------------------------------------------------------------------
/// Load texture maps from default folders into the GPU
void npInitTexMap (void* dataRef)							//zz tex
{
	int i = 0;
	char msg[128];

	pData data = (pData) dataRef;
	pNPtex tex = (pNPtex)&data->io.gl.tex;

#if (!defined(NP_ADDON_FREEIMAGE) && !defined(NP_ADDON_SOIL))
	npPostMsg( "Warn 4303 - No Texture Map Library!!!", kNPmsgFile, data );
	return;
#endif

	/// Tightly pack pixels, used by glReadPixels(), solves issue #232
	/// For discussion, see section 7. Watch Your Pixel Store Alignment' at
	/// https://www.opengl.org/archives/resources/features/KilgardTechniques/oglpitfall/
	glPixelStorei( GL_UNPACK_ALIGNMENT, 1);
	glPixelStorei( GL_PACK_ALIGNMENT, 1);

	/// Get the maximum texture size dimensions supported by GL
	glGetIntegerv( GL_MAX_TEXTURE_SIZE, &tex->maxSize);
	sprintf( msg, "Max Texture Size: %dx%d", tex->maxSize, tex->maxSize);
	npPostMsg( msg, kNPmsgFile, data );
	printf("\n");

	/// Legacy support for loading textures named 'map00001.jpg' first
	npPostMsg( "texture search: usr/images/map*.jpg", kNPmsgFile, data);
	npLoadDir( kNPpathLegacyImages, "map*.jpg", kNPfileCatImage, dataRef );

	/// Load all images from the texture paths list
	for( i=0; i < tex->dirCount; i++)
	{
		sprintf( msg, "texture search: %s", tex->dirPaths[i] );
		npPostMsg( msg, kNPmsgFile, data);
		npLoadDirBranch( tex->dirPaths[i], NULL, kNPfileCatImage, data);
	}

	if( !tex->count )
		npPostMsg( "No textures loaded!!!", kNPmsgFile, data );
	else
	{
		sprintf( msg, "texture count: %d", tex->count ); 
		npPostMsg( msg, kNPmsgFile, data );
	}

	printf("\n");
}


//------------------------------------------------------------------------------
/// Close open (video) files, release GPU texture memory and stop threads
void npCloseTexMap (void* dataRef)		//clean-up
{
	return;
}

//------------------------------------------------------------------------------
/*! Load texture maps from specified folder to the GPU.
	@param dirPath Directory path to search for file(s).
	@param fileFilter Only load files with specified filter, ie: 'map*.jpg'
	@param dataRef Global scene graph pointer.

	@todo Thread texture map loading.
*/
void npAddDirPath( int* dirID, char** dirList, const char* dirPath, void* dataRef)
{
		/// Skip files that have already been loaded
	return;
}

int npAddFile( int* fileID, int dirID, char* fileName, void* dataRef)
{
		/// Skip files that have already been loaded
		return 1;
}

//------------------------------------------------------------------------------
/*! Load texture maps from specified folder to the GPU.
	@param dirPath Directory path to search for file(s).
	@param fileFilter Only load files with specified filter, ie: 'map*.jpg'
	@param dataRef Global scene graph pointer.

	@todo Thread texture map loading.
*/
void npLoadTexDir( const char* dirPath, char* fileFilter, void* dataRef)
{
	int i = 0;
	int result = 0;
	int fileCat = 0;
	int fileID = 0;
	int dirID = 0;
	int texID = 0;
	pNPfileRef fRef = NULL;

	char filePath[kNPmaxPath];

	pData data = (pData) dataRef;
	pNPtex tex = &data->io.gl.tex;


	if( !dirPath )
	{
		npPostMsg( "err 4330 - npLoadTexDir null dirPath", kNPmsgErr, data);
		return;
	}

	/// Add the directory path to the list, okay if already present
	npAddDirPath( &dirID, tex->dirPaths, dirPath, data);

	fRef = nposNewFileRef( data );

	/// Find the first file in the directory
	result = nposFindFirstFile( fRef, dirPath, fileFilter, data );
	if( !result )
	{
		printf( "warn 4331 - err or empty folder: %s\n", dirPath);
		return;
	}

	/// Loop through directory contents and process each file
	do
    {	//zz debug legacy workaround until npFileAdd is implemented
		if( !fileFilter && !strncmp(fRef->name, "map0", 4) )
				continue;
		
		sprintf( filePath, "%s/%s", dirPath, fRef->name );

		/// If Folder (not a file) then recursively call to create dir tree
		if( fRef->isDir )
		{
			npLoadTexDir( filePath, fileFilter, data );	/// recursion
		}
		else
		{
			npGetFileTypeCat( &fileCat, fRef->name, data);
			if( fileCat == kNPfileCatImage )
			{
			//	result = npAddFile( tex->files, dirID, fRef->name, data); //zz debug
				if( !result )
					continue;	/// skip files already loaded

				if( i++ < 5 )	// print the first few file names then dots
					printf( " %.70s\n", fRef->name );
				else if( i % 10 == 0 )
					printf( "." );
			
				texID = npLoadTexture( filePath, data );
			}
		}
    }
	while( nposFindNextFile( fRef ) );	// next file within limits

    nposFindClose( fRef, data );		// always clean up!

	if( i > 5 )
		printf( "\n" );
}


//Textures, fonts, display lists, etc... can all be shared provided that:
//All rendering contexts of a shared display list must use an identical pixel format.
//http://www.opengl.org/discussion_boards/ubbthreads.php?ubb=showflat&Number=79299&page=1
//wglShareLists() supports texture sharing across GL contexts

/// Load a texture map from the specified file path of specified image type.
/// If fileType = 0 then image type determined by the file extension.
//------------------------------------------------------------------------------
int npLoadTexture( const char* filePath, void* dataRef)
{
	int texID = 0;					///< texture_id
	int fileCat = 0;				///< file category 
	pData data = (pData) dataRef;
	
	/// prefer FreeImage (addon) if available, otherwise SOIL or nothing
#ifdef NP_ADDON_FREEIMAGE
		texID = npfiLoadTexture( filePath, data );
#else
#ifdef NP_ADDON_SOIL
		texID = SOIL_load_OGL_texture ( filePath,
			SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID,
			SOIL_FLAG_INVERT_Y | SOIL_FLAG_MIPMAPS * kNPglMipmaps );
#endif
#endif

	/// report err if no texture_id and category is kNPfileCatImage
	if( !texID )
	{
		npGetFileTypeCat( &fileCat, filePath, data);
		if( fileCat == kNPfileCatImage )
		{
			printf("\n");
			npPostMsg( "err 4332 - failed to load image:", kNPmsgErr, data);
			npPostMsg( filePath, kNPmsgErr, data);
		}
	}
	else
		data->io.gl.tex.count++;	// success

	return texID;
}

/*!
* @param dataRef is the command line argument count.
*
* Save a screenshot, either full window or thumbnail.
*/
//------------------------------------------------------------------------------
void npScreenShot (void* dataRef)
{
	int err = 0;
	int temp = 0; //store console level
    int w = 0, tempW = 0;
    int h = 0, tempH = 0;

	char filePath[kNPmaxPath];
	char timeStamp[64];
	char msg[kNPmaxPath];
		
	pData data = (pData) npGetDataRef();
	
	data->io.gl.width = glutGet( GLUT_WINDOW_WIDTH );
	data->io.gl.height = glutGet( GLUT_WINDOW_HEIGHT );
	w = tempW = data->io.gl.width;
	h = tempH = data->io.gl.height;

#ifdef NP_ADDON_FREEIMAGE

	//resize to thumbnail and turn off HUD
	if( data->io.gl.screenGrab == kNPscreenshotThumb )
	{
		sprintf( filePath, "%s.tiff", data->io.gl.datasetName );

		w = kNPthumbWidth; // 320 // 160 // data->map.thumbSize.width
		h = kNPthumbHeight; // 180 // 90  // data->map.thumbSize.height
		temp = data->io.gl.hud.console.level;
		data->io.gl.hud.console.level = 0;
		npGLResizeScene( w, h );
		npGLDrawScene( data );
		
		//grab backbuffer and write to file
		err = npfiScreenshot( filePath, kNPfileTIFF, kNPthumbAlpha, data);
		if( !err )
			sprintf(msg, "Save: %s", filePath);

		//restore screen size and re-render
		data->io.gl.hud.console.level = temp;
		npGLResizeScene( tempW, tempH);
		npGLDrawScene( data);
	}
	else // kNPscreenshotFull
	{
		// construct the file path
		nposTimeStampName( timeStamp );
		sprintf( filePath, "%s%s.jpg", data->io.file.mapPath, timeStamp );

		npGLDrawScene (data);
		
		/// grab the window display buffer and save to file
		err = npfiScreenshot( filePath, kNPfileJPG, kNPscreenshotAlpha, data);	

		sprintf(msg, "Save Screenshot: %s", filePath);
	}
#endif

	if( err )
		sprintf(msg, "err 4334 - screenshot failed: %s", filePath);

	npPostMsg( msg, kNPmsgFile, data );

	data->io.gl.screenGrab = kNPscreenshotOFF;	//reset screenGrab flag
}


/// Initiate a screenshot thumbnail by setting the flag mode to kNPscreenshotThumb
int npScreenGrabThumb( char* name, int type, int x, int y, int w, int h, void* dataRef )
{
	pData data = (pData) dataRef;

/*	char pathName[kNPmaxPath];

	pathName[0] = '\0';
	strncat( pathName, data->io.file.mapPath, kNPmaxPath );
	strncat( pathName, name, kNPmaxPath - strlen( pathName ) );
*/	
	data->io.gl.datasetName[0] = '\0';
	strncat( data->io.gl.datasetName, name, kNPmaxPath );
	data->io.gl.screenGrab = kNPscreenshotThumb;			

	return 0; //zz add error handling
}


//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
//------------------------------------------------------------------------------


//zz models begin
void npLoadGeos(void* dataRef);
void npLoadGeos(void* dataRef)
{
	int i = 0;
	int result = 0;
	int fileType = 0;
	int textureSize = 0;
	int fileNumber = 1;
	static int extTexId = 0;
	int geoId = 0;

	pNPfileRef fRef = NULL;

	char* filename = (char*)malloc(4096);
	char path[256] = {'\0'};

//	unsigned int textureID;		//zz debug, allow loading textures at runtime
//									//detect changes to data->io.file.mapPath
	pData data = (pData) dataRef;
	NPfloatXYZ center, rotate, scale;
	
	center.x = 0; rotate.x = 0; scale.x = 0;
	center.y = 0; rotate.y = 0; scale.y = 0;
	center.z = 0; rotate.z = 0; scale.z = 0;


	printf ("Loading Geos... : %s\n", data->io.file.appPath);

	fRef = nposNewFileRef( data );

	result = nposFindFirstFile( fRef, "usr/global/models/", "*.*", data );
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

		//printf("fRef->name : %s\n", fRef->name);
		sprintf(filename, "%s/%s", "usr/global/models/", fRef->name );
		//printf("!!!!!!!-----filename : %s!!!!!!!!-------\n", filename);

		// if Folder (not a file) then recursively call to create dir tree
		if( fRef->isDir )
		{
		//	npLoadTextures( sPath, data );			// recursion
		}
		else
		{
			sprintf(path, "%s%s", data->io.file.appPath, "usr\\global\\models\\");
		//	printf("---------123 path : %s--------\n", path);
			if( ( strcmp(fRef->name, "models-notes.txt") == 0) 
				|| (strcmp(fRef->name, ".DS_Store") == 0) 
				|| (strcmp(fRef->name, "README") == 0)  )
				continue;

		//	printf("\nnpAddGeo(0,0,0, NULL, %s, %s, dataRef)\n", fRef->name, path);
		//	npAddGeo(0,0,0, NULL, fRef->name, path, dataRef);
		//	extTexId = npGetUnusedExtTexId(dataRef);
			
			extTexId = 0;
			npAddGeo(&geoId, &extTexId, 0, &center, &rotate, &scale, NULL, fRef->name, path, dataRef);
			
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


/*
//zz models
	npInitTexMap
	pNPtexmap texmap = NULL;
	data->io.gl.texmapCount = 0;

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
		texmap->reserved = 0;
	}
	//	npLoadTexturesLegacy(dataRef);
	//	npLoadTextures(dataRef);
//zz models
*/
#define kNPextId 1
#define kNPintId 2


pNPtexmap npTexlistSearchFile(char* filename, char* path, void* dataRef)
{
	pData data = (pData) dataRef;
	int count = data->io.gl.tex.count;
	pNPtexmap tex = NULL;
	int i = 0;

	for(i = 0; i < kNPtexListMax; i++)
	{
		tex = &data->io.gl.texmap[i];
		if( ( strcmp(tex->filename, filename) == 0 ) && ( strcmp(tex->path, path) == 0 ) )
			return tex;
	}

	for(i = 0; i < kNPtexListMax; i++)
	{
		tex = &data->io.gl.texmap[i];
		if( ( strcmp(tex->filename, filename) == 0 ) )
			return tex;
	}

	return NULL;
}

pNPtexmap npTexlistSearchId(int var, void* val, void* dataRef);
pNPtexmap npTexlistSearchId(int var, void* val, void* dataRef)
{
	pData data = (pData) dataRef;
	pNPtexmap tex = NULL;
	int count = data->io.gl.tex.count;
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


int npGetUnusedExtTexId(void* dataRef)
{
	pData data = (pData) dataRef;
	pNPtexmap texmap = NULL;
	int i = 1;
	int extId = 1;

//	printf("NPgetUNUSEDextTEXid\n");
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
//		data->io.gl.texmap[i].extTexId = extId; // lv, temp
		if( data->io.gl.texmap[i].filename[0] == '\0' && data->io.gl.texmap[i].path[0] == '\0'
			&& data->io.gl.texmap[i].extTexId == 0 && data->io.gl.texmap[i].intTexId == 0 &&
			data->io.gl.texmap[i].loaded == 0)
			break;
		/*
		if( data->io.gl.texmap[i].extTexId == 0 )
			break;
		*/
	}

//	data->io.gl.texmap[i].extTexId = extId; // lv, temp
	printf("extMapMe assigning %d", extId);
	data->io.gl.extMapMe[extId] = &data->io.gl.texmap[i];
	data->io.gl.extMapMe[extId]->extTexId = extId;	

	printf("  New ext tex id : %d\n", extId);

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

pNPtexmap npTexTest(void* dataRef)
{
	pData data = (pData) dataRef;
	pNPtexmap t = NULL;
	pNPgeolist geo = NULL;
	int extId = 1;
	int geoId = 0;
	int i = 1;
	int intTexId = 0;

	t = npAddTexMap(&extId, "jeep1.jpg", "usr\\images\\", dataRef);
	t = npAddTexMap(&extId, "m_rifl.bmp", "", dataRef);
	extId = 2;
	t = npAddTexMap(&extId, "house.jpg", "", dataRef);
	t = npAddTexMap(&extId, "katze.jpg", "", dataRef);

	npUpdateTexMap(dataRef);
	
	printf("S\n");

	for(i=1; i <= 2; i++)
		npExtTexToIntTexId(i, dataRef);

	extId = 1;
//	geo = npAddGeo(&geoId, &extId, 0, "", "jeep1.3ds", "usr\\model\\", dataRef);
//	geo = npAddGeo(&geoId, &extId, 0, NULL, NULL, NULL, "", "jeep1.3ds", "", dataRef);

	npLoadModel(geo, dataRef);
//	npUpdateGeoList(dataRef);

	intTexId = npGeoTexId(geoId, dataRef);
	printf("intTexId for %d is %d\n", geoId, intTexId);

	extId = 0;
//	geo = npAddGeo(&geoId, &extId, 0, "", "mar_rifle.3ds", "", dataRef);
	npLoadModel(geo, dataRef);

	return t;
}

/*
	npTexMapFind(0, fName, path, dataRef) 

	pNPtexmap npTexFind(int* extId, char* fName, char* path, void* dataRef);
	behavior:
	- first searches for fName/path pair
	- if found
	    return tex;
*/
pNPtexmap npTexFind(int* extId, char* fName, char* path, void* dataRef);
pNPtexmap npTexFind(int* extId, char* fName, char* path, void* dataRef)
{
	pData data = (pData) dataRef;
	pNPtexmap t = &data->io.gl.texmap[0];
	int i = 1;

	/*
	if( (fName == NULL) && (path == NULL) )
	{
		for(i = 1; i < 2000; i++)
		{
			t = &data->io.gl.texmap[i];
			if(t->extTexId == (*extId))
				return t;
		}

		return NULL;
	}
	*/

	for(i = 1; i < kNPtexMax; i++)
	{
		t = &data->io.gl.texmap[i];

		if( (t->filename[0] !=  '\0') && (t->path[0] != '\0') )
		{
			if( (strcmp(t->filename, fName) == 0) && (strcmp(t->path, path) == 0) )	
			{
				//(*extId) = t->extTexId; // lv temp
				return t;
			}
		}	
	}

	for(i = 1; i < kNPtexMax; i++)
	{
		t = &data->io.gl.texmap[i];

		if( t->filename[0] !=  '\0' )
		{
			if( strcmp(t->filename, fName) == 0 )
			{
				//(*extId) = t->extTexId; // lv temp
				return t;
			}
		}	
	}

	return NULL;
}

// returns index into texlist :: if failure, then 0
int npUsedTex(int extId, void* dataRef);
int npUsedTex(int extId, void* dataRef)
{
	pData data = (pData) dataRef;
	pNPtexmap t = NULL;
	int i = 1;

	for(i = 1; i < kNPtexMax; i++)
	{
		t = &data->io.gl.texmap[i];
		if(t->extTexId == extId)
			return i;
	}

	return 0;
}

int npLoadTex(pNPtexmap tex, void* dataRef);
int npLoadTex(pNPtexmap tex, void* dataRef)
{
	pData data = (pData) dataRef;
	pNPtexmap texA = NULL;
	pNPgeolist geo = NULL;
	char filepath[256] = {'\0'};

	texA = npTexlistSearchId(kNPextId, &tex->extTexId, dataRef);
	if(texA && (texA->intTexId > 0))
	{
		tex->intTexId = texA->intTexId;
		return tex->intTexId;
	}
	
	if(texA && (texA->intTexId == 0))
	{
		sprintf(filepath, "%s%s", tex->path, tex->filename);
	//	printf("filepath : %s\n", filepath);
		tex->intTexId = npLoadTexture(filepath, dataRef);
		if(tex->intTexId > 0)
		{
	//		printf("ext %d to int %d\n", tex->extTexId, tex->intTexId);
			tex->loaded = 1;
		}
		else if(tex->intTexId == 0)
		{
			geo = npSearchGeolistExtTexId(tex->extTexId, dataRef);
			if(geo)
			{
				filepath[0] = '\0';
				sprintf(filepath, "%s%s", geo->modelPath, tex->filename);
				tex->loaded = 0;
			}

		}
	}
	

	return tex->intTexId;
}


pNPtexmap npTexNew(int* extId, char* fName, char* path, void* dataRef);
pNPtexmap npTexNew(int* extId, char* fName, char* path, void* dataRef)
{
	pData data = (pData) dataRef;
	pNPtexmap t = NULL;
	int i = 1;

//	t = npGetUnusedTexMap(dataRef);

	if( (*extId) > 0 )
	{ // check if extId is used, if so, replace entry with new data
		//t->extTexId = 
		if( (i = npUsedTex((*extId), dataRef)) )
		{
			// replace
			t = &data->io.gl.texmap[i];
			t->filename[0] = '\0';
			t->path[0] = '\0';
			strcpy(t->filename, fName);
			strcpy(t->path, path);
			t->intTexId = 0;
			t->loaded = 0;
		}
		else
		{
			// didn't find extId, safe to use
			t = npGetUnusedTexMap(dataRef);
			if(t)
			{
				t->extTexId = (*extId);
				printf("Using t->extTexId : %d\n", t->extTexId);
				t->filename[0] = '\0';
				t->path[0] = '\0';
				strcpy(t->filename, fName);
				strcpy(t->path, path);
				t->intTexId = 0;
				t->loaded = 0;
				npLoadTex(t, dataRef);
				/*
				if(geoAutoLoad)
				{
					npLoadTex(t, dataRef);
				}
				*/

			}
			else
			{
				printf("could not get texmap\n");
			}
		}

	}
	else if( (*extId) == 0 )
	{
		//t->extTexId
		t = npGetUnusedTexMap(dataRef);
		//	t->extTexId = (*extId);
			t->extTexId = npGetUnusedExtTexId(dataRef);
			(*extId) = t->extTexId;
			t->filename[0] = '\0';
			t->path[0] = '\0';
			strcpy(t->filename, fName);
			strcpy(t->path, path);
			t->intTexId = 0;
			t->loaded = 0;
	}
/*
	if( (*extId) == 0 )
	{
		t->extTexId = npGetUnusedExtTexId(dataRef);
	}
*/
//	t->

	return t;
}

pNPtexmap npAddTexMap(int* extId, char* fName, char* path, void* dataRef);
pNPtexmap npAddTexMap(int* extId, char* fName, char* path, void* dataRef)
{
	pData data = (pData) dataRef;
	pNPtexmap t = NULL;
	int i = 1;
	int oldId = (*extId);
	char* abs = NULL;
	char intPath[256] = {'\0'};

	printf("57261 npAddTexMap\n");

	if( npPathIsRel(path, dataRef) )
	{
		abs = npFilePathRelToAbs(path, dataRef);
		strcpy(intPath, abs);
	}
	else
		strcpy(intPath, path);

	// check if filename / path pair already exists in list
	// if npTexFind can't find pair then it looks for only an fName match
	t = npTexFind(extId, fName, intPath, dataRef);
	if(t)
	{
		printf("npTexFind Found\n");
		printf("fName %s || path %s\n", fName, intPath);
		return t;
	}

	printf("npTexFind Not Found\n");
	printf("fName %s || path %s\n", fName, intPath);


	t = npTexNew(extId, fName, intPath, dataRef);
	

	return t;
}

/*
pNPtexmap npAddTexMap(int extTexId, char* filename, char* path, void* dataRef)
{
	pData data = (pData) dataRef;
	pNPtexmap texmap = &data->io.gl.texmap[0];
	pNPtexmap newTex = NULL;
	pNPtexmap texA = NULL;
	pNPtexmap texB = NULL;
	pNPtexmap texC = NULL;
	int i = 0;
	char* foundPath = NULL;

	if(!filename)
		return NULL;

	printf("TEXC 77383929012019\n");
	texC = npTexlistSearchFile(filename, data->io.file.appPath, dataRef);
	if(texC)
	{
		printf("---texC---\n");
		
		return texC;
	}

	if(extTexId == 0)
		extTexId = npGetUnusedExtTexId(dataRef);
	else
		texA = npTexlistSearchId(kNPextId, &extTexId, dataRef);

	printf("7L npAddTexMap(%d,%s,%s,void* dataRef)\n", extTexId, filename, path, dataRef);

	
	if(texA)
	{
		printf("--235----- replace texmap : %d\n", extTexId);
		if(texA->filename[0] != '\0' && filename[0] != '\0')
			strcpy(texA->filename, filename);

		if(texA->path[0] != '\0' && path[0] != '\0')
			strcpy(texA->path, path);

		texA->loaded = 0;
		texA->intTexId = 0;
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

			if( (foundPath[strlen(foundPath)-1] == '\\') 
				&& (foundPath[strlen(foundPath)-2] == '\\') )
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
*/

/// @todo lv
pNPtexmap npTexReserve(int extId, void* dataRef)
{
	pData data = (pData) dataRef;
	pNPtexmap t = NULL;
	
	t = npTexFind(&extId, NULL, NULL, dataRef);
	if(t)
		return NULL;

	t = npTexNew(&extId, NULL, NULL, dataRef);
	//t->reserved =
	t->reserved = 1;

	
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
	for( i = 1; i < kNPtexMax; i++ )
	{
		texmap = &data->io.gl.texmap[i];
		if(texmap->intTexId == 0 && texmap->loaded == 0 &&
			texmap->filename[0] != '\0' && texmap->path[0] != '\0')
		{
		//	printf("Loading extTexId : %d -- intTexId : %d -- %s%s\n",
		//	texmap->extTexId, texmap->intTexId, texmap->path, texmap->filename);
			/*
			printf("texmap->extTexId : %d\n", texmap->extTexId);
			printf("texmap->path : %s\n", texmap->path);
			printf("texmap->filename : %s\n", texmap->filename);
			*/
			sprintf(filepath, "%s%s", texmap->path, texmap->filename);
			texmap->intTexId = npLoadTexture(filepath, dataRef);
			//texmap->intTexId = npLoadTex(texmap, dataRef);

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

void npLoadExtTexMaps(void* dataRef)
{
	pData data = (pData) dataRef;
	char fp[256] = {'\0'};
	int i;

	
	for(i = 1; i < 100; i++)
	{
		if( data->io.gl.extMapMe[i] &&
			data->io.gl.extMapMe[i]->loaded == 0
			&&
			data->io.gl.extMapMe[i]->filename[0] != '\0'
			&&
			data->io.gl.extMapMe[i]->path[0] != '\0'
			&&
			data->io.gl.extMapMe[i]->intTexId == 0)
		{
			sprintf(fp, "%s%s", data->io.gl.extMapMe[i]->path,
				data->io.gl.extMapMe[i]->filename);
			data->io.gl.extMapMe[i]->intTexId = npLoadTexture(fp, dataRef);
			if(data->io.gl.extMapMe[i]->intTexId > 0)
				data->io.gl.extMapMe[i]->loaded = 1;
		}
	}

}
//zz models end

//#define GL_GPU_MEM_INFO_TOTAL_AVAILABLE_MEM_NVX 0x9048
//#define GL_GPU_MEM_INFO_CURRENT_AVAILABLE_MEM_NVX 0x9049
//	GL_GPU_MEM_INFO_TOTAL_AVAILABLE_MEM_NVX
//	glGetIntegerv( GL_GPU_MEM_INFO_CURRENT_AVAILABLE_MEM_NVX,


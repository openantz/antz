/* -----------------------------------------------------------------------------
*
*  npmodels.c
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

#include "npmodels.h"
//#include "npassimp.h"
#include "../gl/npgeometry.h" // lv, models
#include "../gl/nptexmap.h" // lv, models

#include "../../npdata.h"

/** Locks the Geolist, no models can be added to the list (if locked/true) 
	@param dataRef is a global map reference instance.
*/
void npGeolistLock(void* dataRef)
{
	pData data = (pData) dataRef;
	data->io.gl.geoLock = true;
}

/** Unlocks the Geolist, models can be added to the list (if unlocked/false) 
	@param dataRef is a global map reference instance.
*/
void npGeolistUnlock(void* dataRef)
{
	pData data = (pData) dataRef;
	data->io.gl.geoLock = false;
}

/** Inits the Geolist 
	@param dataRef is a global map reference instance.
*/
void npInitModels (void* dataRef)
{
	npInitGeoList(dataRef);
	return;
}

/** Unloads all the models
	@param dataRef is a global map reference instance.	
*/
void npCloseModels (void* dataRef)
{
	/// @todo npCloseModels
	return;
}

/** Sets the Geolist's length
	@param length to set geolist
	@param dataRef is a global map reference instance.
*/
void npGeolistSetLen(int length, void* dataRef)
{	
	pData data = (pData) dataRef;
	data->io.gl.geoLen = length;
}

pNPgeolist npSearchGeolistExtTexId(int extTexId, void* dataRef);
pNPgeolist npSearchGeolistExtTexId(int extTexId, void* dataRef)
{
	pData data = (pData) dataRef;
	pNPgeolist geo = NULL;
	int i = 0;

	if(extTexId == 0)
		return NULL;

	for(i = 1; i < kNPgeoListMax; i++)
	{
		geo = &data->io.gl.geolist[i];
		if(geo->textureId == extTexId)
		{
			return geo;
		}
	}

	
	return NULL;
}

pNPgeolist npSearchGeolistModelFilePath(char* file, char* path, void* dataRef);
pNPgeolist npSearchGeolistModelFilePath(char* file, char* path, void* dataRef)
{
	pData data = (pData) dataRef;
	pNPgeolist geo = NULL;
	int i = 0;

	for(i = 0; i < kNPgeoListMax; i++)
	{
		geo = &data->io.gl.geolist[i];
		if( (strcmp(geo->modelFile, file) == 0) 
			&& (strcmp(geo->modelPath, path) == 0) )
		{
			return geo;
		}
	}

	return NULL;
}

pNPgeolist npGetUnusedGeo(void* dataRef)
{
	pData data = (pData) dataRef;
	pNPgeolist geolist = NULL;
	pNPgeolist geodef = &data->io.gl.geolist[0];
	int i = 1;

	for(i = 1; i < kNPgeoListMax; i++)
	{
		geolist = &data->io.gl.geolist[i];
		if( 0 == memcmp(geodef, geolist, sizeof(NPgeolist)) )
			return geolist;
	}
}


int npSearchGeosId(int geoId, void* dataRef);
int npSearchGeosId(int geoId, void* dataRef)
{
	pData data = (pData) dataRef;
	pNPgeolist geolist = NULL; 
	int i = 1;	

//	if(geoId == 0)
//		geoId++;

	for(; i < kNPgeoListMax; i++)
	{
		geolist = &data->io.gl.geolist[i];	
		if(geolist->geometryId == geoId)
			return geoId;
	}

	return 0;
}

void npNewGeoId(int* geoId, void* dataRef);
void npNewGeoId(int* geoId, void* dataRef)
{
	int x = 0;
	while( (x = npSearchGeosId((*geoId), dataRef) ) != 0 )
		(*geoId)++;
}

pNPgeolist npNewGeo(int* geoId, int* extTexId, char* geoname, char* filename, char* path, void* dataRef)
{
	pData data = (pData) dataRef;
	pNPgeolist geo = NULL;

	npNewGeoId(geoId, dataRef);

	geo = npGetUnusedGeo(dataRef);

	geo->geometryId = geoId;

	if(extTexId == 0)
		geo->textureId = npGetUnusedExtTexId(dataRef);
	else
		geo->textureId = extTexId;

	strcpy(geo->modelFile, filename);
	strcpy(geo->modelPath, path);

	if(geoname != NULL)
		strcpy(geo->name, geoname);
	else
		strcpy(geo->name, "");

	geo->loaded = 0;
	geo->modelId = 0;
	geo->modelTextureFile[0] = '\0';
	geo->modelTexturePath[0] = '\0';
	

	return geo;
}

/** Loads models that are in the geolist 
	@param geo is an element of the geolist
	@param dataRef is a global map reference instance.
	@return the model id
*/
pNPgeolist npAddGeo(int geoId, int extTexId, int type, char* object_name, char* file_name, char* path, void* dataRef)
{
	pData data = (pData) dataRef;
	pNPgeolist geolist = &data->io.gl.geolist[0];
	pNPgeolist geoA = NULL;
	pNPgeolist geoB = NULL;
	pNPgeolist geo = NULL;
	pNPtexmap texA = NULL;
	int i = 0;
	int x = 0;
//	data->io.gl.geolist

	geoA = npSearchGeolistModelFilePath(file_name, path, dataRef);
	if(geoA)
		return geoA;

	geoB = npSearchGeolistExtTexId(extTexId, dataRef);
	if(geoB)
		return geoB;

	if(geoId == 0)
	{
		geoId = 1000;
//		geoId++;
	}

//	printf("path : %s\n", path);
	
//	geo = npNewGeo(&geoId, &extTexId, object_name, file_name, path, dataRef);

	npNewGeoId(&geoId, dataRef);
	
	geo = npGetUnusedGeo(dataRef);
	
	geo->geometryId = geoId;

	if(extTexId == 0)
		geo->textureId = npGetUnusedExtTexId(dataRef);
	else
		geo->textureId = extTexId;

	strcpy(geo->modelFile, file_name);
	strcpy(geo->modelPath, path);

	if(object_name != NULL)
		strcpy(geo->name, object_name);
	else
		strcpy(geo->name, "");

	return geo;
}

int npExtTexToIntTexId(int extTexId, void* dataRef)
{
	pData data = (pData) dataRef;
//	pNPgeolist geolist = &data->io.gl.geolist[0];
	pNPtexmap texmap = &data->io.gl.texmap[0];
	int i = 0;

	/// kNPtexListMax 2000
	for(i = 1; i < 2000; i++)
	{
		texmap = &data->io.gl.texmap[i];
		if(texmap->extTexId == extTexId)
			return texmap->intTexId;
	}

	return 0;
}


int npNewModelId(int* geoId, void* dataRef);
int npNewModelId(int* geoId, void* dataRef)
{
	pData data = (pData) dataRef;
	pNPgeolist geolist = &data->io.gl.geolist[0];
	int i = 0;
	int x = 0;
	
	while( (x = npSearchGeosId((*geoId), dataRef)) != (*geoId) )
		(*geoId)++;

	if( x >= 1000 )
		return (*geoId)-999;

	return 0;
}

int npLoadModel(pNPgeolist geo, void* dataRef)
{
	pData data = (pData) dataRef;
	pNPassimp assimp = (pNPassimp)data->io.assimp;
	pNPgeolist geolist = &data->io.gl.geolist[0];
	pNPgeolist geoMatch = NULL;
	char filePath[256] = {'\0'};
	char fileName[256] = {'\0'};
	char path[256] = {'\0'};
	int modelId = 0;
	int i = 0;
	int geolistIndexMatch = 0;
	
	filePath[0] = '\0';

	if( geo->modelPath[strlen(geo->modelPath)-1] == '\\' )
	{
		/// has slash
		printf("slash\n");
	}
	else
	{
		/// has no slash
		printf("!slash\n");
	}

	sprintf(filePath, "%s%s", geo->modelPath, geo->modelFile);
	modelId = npNewModelId(&geo->geometryId, dataRef);
	assimp->scene[modelId] = npModelImport(filePath, dataRef);
	if(assimp->scene[modelId] == NULL)
	{
		///@todo:npDelGeo
		geo->geometryId = 0;
		geo->loaded = 0;
		return 0;
	}

	assimp->path = npAssimpGetTexturePath(modelId, dataRef);
	npGetFileNameFromPath(&assimp->path.data[0], fileName, dataRef);
	strcpy(path, filePath);
	path[strlen(filePath) - strlen(geo->modelFile)] = '\0';
	printf("---------path : %s-------\n", path);
	for(i = 0; i < strlen(fileName); i++)
	{
		fileName[i] = tolower(fileName[i]);
	}

	npAddTexMap(geo->textureId, fileName, path, dataRef);
	path[0] = '\0';
	assimp->path.data[0] = '\0';

	return modelId;
}

/// @todo pNPgeolist npGeolistGetGeo(int geometryId, void* dataRef)

int npGeoNull(void)
{
	return 0;
}

void npSetSelectedNodeGeoId(int* geoId, void* dataRef )
{
	npSetSelectedNodes( kNPgeometry, geoId, dataRef );
}

/** Increments the geolist length
	@param dataRef is a global map reference instance.
	@return the geolist length
*/
int npGeolistIncLen(void* dataRef)
{
	pData data = (pData) dataRef;	
	return (data->io.gl.geoLen++);	
}

/** Increments the geolist index 
	@param dataRef is a global map reference instance.
*/
void npGeolistIncX(void* dataRef)
{	
	pData data = (pData) dataRef;
	data->io.gl.geoX++;
}

/** Sets the geolist index 
	@param X is geolist index
	@param dataRef is a global map reference instance.
*/
void npGeolistSetX(int X, void* dataRef)
{	
	pData data = (pData) dataRef;
	data->io.gl.geoX = X;
}

/** Gets the geolist index
	@param dataRef is a global map reference instance.
	@return the geolist index
*/
int	npGeolistGetX(void* dataRef) 
{
	pData data = (pData) dataRef;
	return data->io.gl.geoX;
}

/** Gets the geolist lock status
	@param dataRef is a global map reference instance.
	@return geolock status as boolean (true/false)
*/
bool npGeolistLockStatus(void* dataRef)
{
	pData data = (pData) dataRef;
	return data->io.gl.geoLock;
}

pNPgeo npGeoInit(void* dataRef);
pNPgeo npGeoInit(void* dataRef)
{
	pNPgeo geo = NULL;
	geo = malloc(sizeof(NPgeo));
	return geo;
}

char* npTextureNewType(char* idVal, int* type, void* dataRef);
char* npTextureNewType(char* idVal, int* type, void* dataRef)
{
	(*type) = npstrtoi(&idVal);
	return idVal; 
}

char* npTextureNewExtId(char* idVal, int* extId, void* dataRef);
char* npTextureNewExtId(char* idVal, int* extId, void* dataRef)
{
	(*extId) = npstrtoi(&idVal);
	/*
//	pNPgeolist geolist = npGetGeolist(dataRef);
	pNPgeolist geolist = NULL;
	int match = 0;
	(*geoId) = npstrtoi(&idVal); // lv, if letter, returns 0

	if( (*geoId) < 1000 || (*geoId) > 2000 )
	{
		printf("Invalid Value : %d", (*geoId));
		(*geoId) = 999;

//		while( geolist[(*geoId)++].geometryId != 0 );
		while( geolist[++(*geoId)].geometryId != 0 );
		if((*geoId) <= 2000)
			geolist[(*geoId)].geometryId = (*geoId);
		else
			printf("geolist limit hit\n");

		return idVal;
	}

	/// @todo search geolist to see if geometryId is already used
	// if not found in geolist then return csv's geometryId value.

	/// @todo npGeolistFindId
	*/
	return idVal;
}

char* npTextureNewFilename(char* stringVal, int maxSize, char* fileName, void* dataRef);
char* npTextureNewFilename(char* stringVal, int maxSize, char* fileName, void* dataRef)
{
	char* curs = stringVal;
	char** read = NULL;
	int strLength = 0;
	int match = 0;
	int i = 0;
	char* ext = NULL;
//	strLength = maxSize - (curs - *read);
	strLength = maxSize;
	/*
	if (strLength >= kNPmodelFileName)
		strLength = kNPmodelFileName;
	*/
	if (strLength >= 200 )
		strLength = 200;
	
	fileName[0] = '\0';

	npCSVstrncpy(fileName, &curs, strLength);

	ext = strstr(fileName, ".");
	if( ext == NULL )
	{
		// texture file has no extension, cannot load
		printf("texture file has no extension, cannot load\n");
/// @todo geolist[geoId].loadModel = false;	
	}

	
//	match = npGeolistFindFileName(fileName, dataRef);
	

	return curs;
}

char* npTextureNewFilePath(char* stringVal, int maxSize, char* filePath, void* dataRef);
char* npTextureNewFilePath(char* stringVal, int maxSize, char* filePath, void* dataRef)
{
	pData data = (pData) dataRef;
	char path[256] = {'\0'};
	char* curs = stringVal;
	char** read = NULL;
	char* dslash = NULL;
	int strLength = 0;
	int len = 0;
	int i = 0;
	int count = 0;
//	strLength = maxSize - (curs - *read);
	strLength = maxSize;
	/*
	if (strLength >= kNPmodelFilePath)
		strLength = kNPmodelFilePath;
	*/
	if (strLength >= 200 )
		strLength = 200;
	
	filePath[0] = '\0';

	npCSVstrncpy(path, &curs, strLength);
	i = len = strlen(path);

	if(len == 0)
		return curs;

	if(path[strlen(path)-1] != '\\')
	{
		path[strlen(path)+1] = '\0';
		path[strlen(path)] = '\\';
	}


	if( (strstr(path, "usr/") != NULL) && (strlen(path) < 15) )
	{
		for(i = 0; i < len; i++)
		{
			if(path[i] == '/')
				path[i] = '\\';
		}

		count = sprintf(filePath, "%s%s", data->io.file.appPath, path);
		filePath[count-1] = '\0';
	}
	else
	{
		strcpy(filePath, path);
	}

	/// Don't assume a trailing slash
	/// if it doesn't have one, add it.
//	printf("%c\n", filePath[i]);
	/*
	if( filePath[i] != nposGetFolderDelimit() )
		filePath[i] = nposGetFolderDelimit();
	*/
	/*
	if( filePath[i-1] != '\\' )
		filePath[i-1] = nposGetFolderDelimit();
	*/

	if( (dslash = strstr(filePath, "//")) != NULL )
	{
		dslash[1] = '\0';
//		printf("dslash : %s\n", dslash);
//		printf("filePath : %s\n", filePath);
	}

	/*
	while( filePath[i] != nposGetFolderDelimit() && i >= 0 )
	{
		i--;
	}

	filePath[i+1] = '\0';
	*/

	return curs;
}

void npTextureNew(char* tex_csvline, void* dataRef);
void npTextureNew(char* tex_csvline, void* dataRef)
{	
	pNPgeo geo = NULL;
//	char* curs = &model_csvline;
	char* curs = tex_csvline;
	char* abs = NULL;
	char objectName[256] = {'\0'};
	char fileName[256] = {'\0'};
	char filePath[256] = {'\0'};
	int geometryId = 0;
	int textureId = 0;
//	int typeId = 0;
	int extId = 0;
	int type = 0;

	curs = npTextureNewExtId( curs, &extId, dataRef );
	curs = npTextureNewType( curs, &type, dataRef );
	curs = npTextureNewFilename( curs, 200, &fileName[0], dataRef );
	curs = npTextureNewFilePath( curs, 200, &filePath[0], dataRef );
		
	if( npPathIsRel(filePath, dataRef))
	{
		abs = npFilePathRelToAbs(filePath, dataRef);
		filePath[0] = '\0';
		strcpy(filePath, abs);
		free(abs);
	}

	npAddTexMap(extId, fileName, filePath, dataRef);
//	curs = npModelNewGeoId( curs, &geometryId, dataRef );
//	curs = npModelNewTextureId( curs, &textureId, dataRef ); // lv, temp
//	curs = npModelNewTypeId( curs, &typeId, dataRef );


//	curs = npModelNewObjectName(curs, 200, &objectName[0], dataRef);
//	curs =  npModelNewFileName(curs, 200, &fileName[0], dataRef);
//	curs = npModelNewFilePath(curs, 200, &filePath[0], dataRef); 

	/// @todo npGeoMalloc(dataRef)
	/*
	geo = npGeoInit(dataRef);


	geo->geometryId = geometryId;
	geo->modelId = geometryId - 999;
	geo->textureId  = textureId;
//	geo->type = typeId; /// @todo geoType
	strcpy(geo->name, objectName);
	strcpy(geo->modelFile, fileName);
	strcpy(geo->modelPath, filePath);
	
//	if( npGeolistSearchGeo(geo, dataRef) 
//	npGeolistSearchGeo(geo, dataRef); // lv, temp
//	match = npGeolistSearch
	*/	
	return;
}

/// @todo : npModelNew
pNPgeo npModelNew(char* model_csvline, void* dataRef)
{	
	pNPgeo geo = NULL;
//	char* curs = &model_csvline;
	char* curs = model_csvline;
	char* foundPath = NULL;
	char objectName[256] = {'\0'};
	char fileName[256] = {'\0'};
	char filePath[256] = {'\0'};
	int geometryId = 0;
	int textureId = 0;
	int typeId = 0;

	curs = npModelNewGeoId( curs, &geometryId, dataRef );
	curs = npModelNewTextureId( curs, &textureId, dataRef ); // lv, temp
	curs = npModelNewTypeId( curs, &typeId, dataRef );


	curs = npModelNewObjectName(curs, 200, &objectName[0], dataRef);
	curs =  npModelNewFileName(curs, 200, &fileName[0], dataRef);
	curs = npModelNewFilePath(curs, 200, &filePath[0], dataRef); 

	if(filePath[0] == '\0')
	{
		foundPath = npSearchPathsForFile(fileName, dataRef);
		if(foundPath[0] != '\0')
		{
			foundPath[strlen(foundPath)-strlen(fileName)] = '\0';
			strcpy(filePath, foundPath);
		}
	}
	/// @todo npGeoMalloc(dataRef)
	npAddGeo(geometryId, textureId, 0, objectName, fileName, filePath, dataRef);
	
	return geo;
}

int npPathIsRel(char* path, void* dataRef);
int npPathIsRel(char* path, void* dataRef)
{
	char t[256] = {'\0'};
	strcpy(t, path);
	t[0] = tolower(t[0]);
	if(t[0] >= 'a' && t[0] <= 'z' && t[1] == ':' && t[2] == '\\')
		return 0;

	return 1;
}

char* npModelNewFilePath(char* stringVal, int maxSize, char* filePath, void* dataRef)
{
	char* curs = stringVal;
	char** read = NULL;
	char* abs = NULL;
	int strLength = 0;
	int len = 0;
	int i = 0;
//	strLength = maxSize - (curs - *read);
	strLength = maxSize;
	/*
	if (strLength >= kNPmodelFilePath)
		strLength = kNPmodelFilePath;
	*/
	if (strLength >= 200 )
		strLength = 200;
	
	filePath[0] = '\0';

	npCSVstrncpy(filePath, &curs, strLength);
	i = len = strlen(filePath);
	
	if(filePath[strlen(filePath)-1] != '\\' && len > 0)
	{
		filePath[strlen(filePath)+1] = '\0';
		filePath[strlen(filePath)] = '\\';
	}

	if(len == 0)
		return curs;
		
	//npFilePathIsRel(filePath, dataRef);
	if( npPathIsRel(filePath, dataRef))
	{
		abs = npFilePathRelToAbs(filePath, dataRef);
		filePath[0] = '\0';
		strcpy(filePath, abs);
		free(abs);
//		strcpy(filePath,
	}

	return curs;
}




char* npModelNewFileName(char* stringVal, int maxSize, char* fileName, void* dataRef)
{
	char* curs = stringVal;
	char** read = NULL;
	int strLength = 0;
	int match = 0;
	int i = 0;
	char* ext = NULL;
//	strLength = maxSize - (curs - *read);
	strLength = maxSize;
	/*
	if (strLength >= kNPmodelFileName)
		strLength = kNPmodelFileName;
	*/
	if (strLength >= 200 )
		strLength = 200;
	
	fileName[0] = '\0';

	npCSVstrncpy(fileName, &curs, strLength);

	ext = strstr(fileName, ".");
	if( ext == NULL )
	{
		// model file has no extension, cannot load
		printf("model file has no extension, cannot load\n");
/// @todo geolist[geoId].loadModel = false;	
	}
	
	return curs;
}

char* npModelNewObjectName(char* stringVal, int maxSize, char* objectName, void* dataRef)
{
//	char* objectName = NULL;
	char* curs = stringVal;
	char** read = NULL;
	int strLength = 0;
	int match = 0;
//	strLength = maxSize - (curs - *read);
	strLength = 200;

	if (strLength >= 200 )
		strLength = 200;
	
	objectName[0] = '\0';

	npCSVstrncpy(objectName, &curs, maxSize);

	if( objectName[0] == '\0' )
	{
		/// @todo: get objectName from model file
	}
	
	/// @todo npGeoSame(pNPgeo geo1, pNPgeo geo2, void* dataRef);	
	match = npGeolistFindObjectName(objectName, dataRef);

	return curs;
}

//curs = npModelNewTypeId(curs, &typeId, dataRef);
char* npModelNewTypeId(char* csv_typeId, int* typeId, void* dataRef)
{
	(*typeId) = npstrtoi(&csv_typeId);	
	return csv_typeId;
}


char* npModelNewTextureId(char* idVal, int* textureId, void* dataRef)
{
	pData data = (pData) dataRef;
//	pNPgeolist geolist = npGetGeolist(dataRef);
	int match = 0;

	(*textureId) = npstrtoi(&idVal);

	if( (*textureId) < 0 )
	{
		printf("\ntextureId is < 0");
		/// @todo: Find unused textureId from internal/external textureId map
	}


	/// @todo if textureId exists in geolist return it,
	///		  if not then return csv's textureId
///	@todo match = npGeolistFindTextureId(textureId, dataRef);
	if(match != 0)
		(*textureId) = match;

	return idVal;
}

//int npGeolistMatch(

/// Deprecated
/// @todo npGeolistFind
// rename this to npGeolistGet
int npGeolistFind(int geoId, void* dataRef)
{
//	pNPgeolist geolist = npGetGeolist(dataRef);
	pNPgeolist geolist = NULL;
	
	if( geolist[geoId].geometryId == geoId )
		return geoId;

	return 0;
}


int npGeolistFindFilePath(char* filepath, void* dataRef)
{
//	pNPgeolist geolist = npGetGeolist(dataRef);
	pNPgeolist geolist = NULL; // lv, temp
	pNPgeo geo = NULL;
	char testpath[256] = {'\0'};
	int i = 0;

	for(i = 1000; i < 2000; i++)
	{
		geo = &geolist[i];
		/// Has to exist
		if( geo->geometryId == i )
		{
			sprintf(testpath, "%s%s", geo->modelPath, geo->modelFile);	
			if( strcmp(testpath, filepath) == 0 )
				return i;
		}
	}

	return 0;
}

//	npGeolistFindObjectName(objectName, dataRef);
/// returns geoId
int npGeolistFindObjectName(char* objectName, void* dataRef)
{
	pData data = (pData) dataRef;
//	pNPgeolist geolist = npGetGeolist(dataRef);
	pNPgeolist geolist = NULL;
	pNPgeo geo = NULL;
	int i = 0;

	if( objectName[0] == '\0')
		return 0;

//	data->io.gl.nameMatch = 0;
	for(i = 1000; i < 2000; i++)
	{
		geo = &geolist[i];	
		if( geo->name[0] != '\0' )
		{
			if( strcmp( objectName, geo->name ) == 0 )
			{	// Identical
			//	data->io.gl.nameMatch = i;
				return i; //return geo->geometryId
			}
			
		}
	}

	return 0;
}

int npGeolistNewId(void* dataRef)
{
	int geoId = 999;
//	pNPgeolist geolist = npGetGeolist(dataRef);
	pNPgeolist geolist = NULL;
	
	while( geolist[++geoId].geometryId != 0 );
	if(geoId > 2000)
		return 0;
	
	return geoId;
}


/// returns modelId

/// returns modelId
char* npModelNewGeoId(char* idVal, int* geoId, void* dataRef)
{
//	pNPgeolist geolist = npGetGeolist(dataRef);
	pNPgeolist geolist = NULL;
	int match = 0;
	(*geoId) = npstrtoi(&idVal); // lv, if letter, returns 0

	/*
	if( (*geoId) < 1000 || (*geoId) > 2000 )
	{
		printf("Invalid Value : %d", (*geoId));
		(*geoId) = 999;

//		while( geolist[(*geoId)++].geometryId != 0 );
		while( geolist[++(*geoId)].geometryId != 0 );
		if((*geoId) <= 2000)
			geolist[(*geoId)].geometryId = (*geoId);
		else
			printf("geolist limit hit\n");

		return idVal;
	}
	*/

	/*
	if( npGeolistIdTaken( (*geoId), dataRef ) )
	{
		/// true, assign new geolist id
		(*geoId) = npGeolistNewId(dataRef);
	}
	else
		geolist[(*geoId)].geometryId = (*geoId);
	*/

	/// @todo search geolist to see if geometryId is already used
	// if not found in geolist then return csv's geometryId value.

	/// @todo npGeolistFindId
	/* // lv temp
	match = npGeolistFind((*geoId), dataRef);	
	if(match != 0)
	{
		(*geoId) = match;
//		(*geoId)++;
		//while( geolist[geoId]->
	//	while( geolist[(*geoId)++].geometryId != 0 );
	//	printf("Found unused geometryId\n");
	}
	*/

	return idVal;
}

void npUpdateGeoList( void* dataRef )
{
	pData data = (pData) dataRef;
	pNPassimp assimp = (pNPassimp)data->io.assimp;
	pNPgeolist geolist = NULL;
	int i = 1;
	int modelId = 0;

	for(; i < kNPgeoListMax; i++)
	{
		geolist = &data->io.gl.geolist[i];
		if( (geolist->geometryId != 0) && (geolist->loaded == 0) )
		{
			/*
			printf("\n");
			printf("geolist->geometryId : %d\n", geolist->geometryId);
			printf("geolist->modelFile : %s\n", geolist->modelFile);
			printf("geolist->modelPath : %s\n", geolist->modelPath);
			printf("\n");
			*/
			modelId = npLoadModel(geolist, dataRef);
			if(modelId)
			{
				npModelStoreDL(assimp->scene[modelId], geolist->geometryId, dataRef);
				geolist->loaded = 1;
				data->io.gl.numModels++;
			}
			else
			{
				/// can't load
				geolist->loaded = 0;
				data->io.gl.numModels--;
			}
		}
	}

	return;
}

/** Gets the length of the geolist
	@param dataRef is a global map reference instance.
	@return the geolist length
*/
int npGeolistGetLen(void* dataRef)
{
	pData data = (pData) dataRef;
	return data->io.gl.geoLen;
}

/** Imports an assimp scene from a file
	@param filePath, full path to file
	@param dataRef is a global map reference instance.
*/
struct aiScene* npModelImport(char* filePath, void* dataRef)
{
	struct aiScene* scene = NULL;

	scene = aiImportFile(filePath, 0);

	if( scene == NULL )
		printf("\naiImportFile returned null");

	return scene;
}
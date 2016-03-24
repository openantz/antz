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

/** Loads models that are in the geolist 
	@param geo is an element of the geolist
	@param dataRef is a global map reference instance.
	@return the model id
*/ 
/*
int npLoadModel(pNPgeolist geo, void* dataRef)
{
	pData data = (pData) dataRef;
	pNPassimp assimp = (pNPassimp)data->io.assimp;
	pNPgeolist geolist = &data->io.gl.geolist;
	pNPgeolist geoMatch = NULL;
	char filePath[256] = {'\0'};
	int modelId = 0;
	int geolistIndexMatch = 0;

	sprintf(filePath, "%s%s", geo->modelPath, geo->modelFile);
	modelId = npAssimpLoadModel(filePath, &geolistIndexMatch, dataRef);
	//if(geolistIndexMatch != 0 && modelId != 0) 
	if(modelId != 0) 
	{
		geoMatch = &geolist[geolistIndexMatch];	
		if(geoMatch->modelTexturePath[0] != '\0')
		{
		//	strcpy(geo->modelTextureFile, geoMatch->modelTextureFile);
			strcpy(geo->modelTexturePath, geoMatch->modelTexturePath);
		}

	}

	if(modelId != 0)
		data->io.gl.numModels++;	
	
	return modelId;
}
*/

/// @todo pNPgeolist npGeolistGetGeo(int geometryId, void* dataRef)
/*
pNPgeolist npGeolistGetGeo(int geometryId, void* dataRef)
{
	pData data = (pData) dataRef;
	pNPgeolist geolist = NULL;
	geolist = npGetGeolist(dataRef);

	return &geolist[geometryId];
}
*/

/*
pNPgeolist npGetGeolist(void* dataRef)
{
	pData data = (pData) dataRef;
	return &data->io.gl.geolist[0];
}
*/

	//modelId = npModelGetNewId(dataRef);
/*
int npModelNewId(int geoId, void* dataRef)
{
	pData data = (pData) dataRef;
	pNPgeolist geolist = &data->io.gl.geolist[0];

	return geoId - 999;

//	return (++data->io.gl.modelId); /// @todo lv, add modelId to gl struct
}
*/

int npGeoNull(void)
{
	return NULL;
}

/*
int npGeolistNewGeoId(void* dataRef)
{
	pNPgeolist geolist = npGetGeolist(dataRef);
	pNPgeo geo = NULL;
	int i = 0;

	for(i = 1000; i < 2000; i++)
	{
		geo = &geolist[i];
		// if i and geometryId are identical, geo is taken
		// if not, geo is safe to work with.
		if( i != geo->geometryId )
			return i;
	}

	return 0;
}
*/

void npSetSelectedNodeGeoId(int* geoId, void* dataRef )
{
	npSetSelectedNodes( kNPgeometry, geoId, dataRef );
}

/*
pNPgeolist npLoadModelFromFile(char* filepath, void* dataRef)
{
	pData data = (pData) dataRef;	
	pNPgl gl = &data->io.gl;
	pNPgeolist geolist = &data->io.gl.geolist[0];
	char filename[256] = {'\0'};
	int geometryId = 0;
	static int modelId = 1;
	int geoId = 0;
	int match = 0;

	match = npGeolistFindFilePath(filepath, dataRef);
	if(match)
		return;

	geoId = npGeolistNewGeoId(dataRef);

	geolist[geoId].modelId = npModelNewId(geoId, dataRef);

//	geoId = geometryId;

	/// Check if geolist geoId is at end of array, if so, return.
	if( geoId == 2000 )
	{
		printf("\nMax geoId hit");
		system("pause");
		return;
	}

	geolist[geoId].geometryId = geoId;
	geolist[geoId].modelId = npModelNewId(geoId, dataRef);

	npGetFileNameFromPath(filepath, &filename[0], dataRef);
	strcpy(geolist[geoId].modelFile, filename); /// @todo : npGeolistSetModelFile(int geometryId, char* modelFile, void* dataRef); 
	filepath[strlen(filepath) - strlen(filename)] = '\0';

	strcpy(geolist[geoId].modelPath, filepath);

	npGeolistUnlock(dataRef);
	npGeolistIncLen(dataRef);

	return &geolist[geoId];
}
*/
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

/*
bool npGeolistIdTaken(int geoId, void* dataRef)
{
	pNPgeolist geolist = npGetGeolist(dataRef);
	
	if( geolist[geoId].geometryId == geoId )
		return true;
	else
		return false;

	return false;
}
*/

/*
int npGeolistSearchGeo(pNPgeo geo, void* dataRef) 
{
	pNPgeolist geolist = npGetGeolist(dataRef);
	pNPgeo geoTest = NULL;
	int i = 0;


//	if( npGeolistIdTaken(geo->geometryId, dataRef) )
//	{
		/// true, 
//	}


//	for(i = 1000; i < 2000; i++)
//	{
//		geoTest = &geolist[i];
//
//		if( strcmp(geoTest->modelPath, geo->modelPath) == 0 )
//		{
//			if( strcmp(geoTest->modelFile, geo->modelFile) == 0 )
//			{
//				return i;
//			}
//		}
//	}

	return 0;
}
*/

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
		printf("dslash : %s\n", dslash);
		printf("filePath : %s\n", filePath);
	//	system("pause");
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
	char objectName[256] = {'\0'};
	char fileName[256] = {'\0'};
	char filePath[256] = {'\0'};
	int geometryId = 0;
	int textureId = 0;
	int typeId = 0;

	curs = npModelNewGeoId( curs, &geometryId, dataRef );
//	curs = npModelNewTextureId( curs, &textureId, dataRef ); // lv, temp
	curs = npModelNewTypeId( curs, &typeId, dataRef );


	curs = npModelNewObjectName(curs, 200, &objectName[0], dataRef);
	curs =  npModelNewFileName(curs, 200, &fileName[0], dataRef);
	curs = npModelNewFilePath(curs, 200, &filePath[0], dataRef); 

	/// @todo npGeoMalloc(dataRef)
	/*
		pNPgeo npGeoMalloc(void* dataRef)
		{
			pNPgeo geo = NULL;
			geo = malloc(sizeof(NPgeo));
			return geo;
		}

	*/
	/*
	geo = malloc(sizeof(NPgeo));
	if(geo == NULL)
		return NULL;
	*/
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
	
	return geo;
	/*
//	fileName = npModelNewFileName(stringVal, maxSize, dataRef)
	fileNamePath = npModelNewFilePath(stringVal, maxSize, dataRef)
	*/
}


char* npModelNewFilePath(char* stringVal, int maxSize, char* filePath, void* dataRef)
{
	char* curs = stringVal;
	char** read = NULL;
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

	if(len == 0)
		return curs;
		
	/// Don't assume a trailing slash
	/// if it doesn't have one, add it.
//	printf("%c\n", filePath[i]);
	/*
	if( filePath[i] != nposGetFolderDelimit() )
		filePath[i] = nposGetFolderDelimit();
	*/
	if( filePath[i-1] != '\\' )
		filePath[i-1] = nposGetFolderDelimit();

	/*
	while( filePath[i] != nposGetFolderDelimit() && i >= 0 )
	{
		i--;
	}

	filePath[i+1] = '\0';
	*/

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

	
//	match = npGeolistFindFileName(fileName, dataRef);
	

	return curs;
}

/*
int npModelNewTypeId(char* typeVal, void* dataRef)
{
	int typeId = 0;
	typeId = npstrtoi(&typeVal);

	/// @todo recognize valid type id

	return typeId;
}
*/

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
	
	/// @todo npGeoIsSame(pNPgeo geo1, pNPgeo geo2);	
	match = npGeolistFindObjectName(objectName, dataRef);

	return curs;
}

//curs = npModelNewTypeId(curs, &typeId, dataRef);
char* npModelNewTypeId(char* csv_typeId, int* typeId, void* dataRef)
{
	(*typeId) = npstrtoi(&csv_typeId);	
	return csv_typeId;
}

/*
char* npModelNewTextureId(char* idVal, int* textureId, void* dataRef)
{
	pData data = (pData) dataRef;
	pNPgeolist geolist = npGetGeolist(dataRef);
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
*/
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
/** Adds model info to the Geolist, does not load the model.
	@param geometryId
	@param modelId
	@param modelName
	@param fileName
	@param modelPath
	@param dataRef is a global map reference instance.
*/
/*
void npGeolistAddModel(int geometryId, int modelId, char* modelName, char* fileName, char* modelPath, void* dataRef) 
{
	pData data = (pData) dataRef;
	pNPgeolist p_geo = &data->io.gl.geolist[geometryId];

	p_geo->modelId = modelId;
	p_geo->geometryId = geometryId;
	strcpy(p_geo->name, modelName);
	strcpy(p_geo->modelFile, fileName);
	strcpy(p_geo->modelPath, modelPath);

	npGeolistIncLen(dataRef);
//	p_geo->textureId = textureId;
}
*/

/** Updates the geolist (if unlocked) then locks it	
	@param dataRef is a global map reference instance.
*/
/*
void npUpdateGeoList( void* dataRef ){
	pData data = (pData) dataRef;
	char modelfilePath[256] = {'\0'}; 
	pNPgl gl = &data->io.gl;
	pNPgeolist p_geo = NULL;
	int geoLen = gl->geoLen; /// lv geolist length
	int i = 1000;
	pNPgeolist geolist = gl->geolist;
	pNPassimp assimp = (pNPassimp)data->io.assimp;

//			if( p_geo->textureId == 0
//				&& p_geo->modelFile[0] != '\0')
//				&& p_geo->modelPath[0] != '\0')

				//&& p_geo->modelPath[0] != '\0')
			//if( p_geo->geometryId == npGeolistGetX(dataRef) )

	/// @todo lv implement geo hash table
	if(npGeolistLockStatus(dataRef) == false) {
		for(i = 1000 ; i < 2000; i++)
		{
			p_geo = &gl->geolist[i];
			if( p_geo->geometryId == i )
			{
				p_geo->modelId = npLoadModel(p_geo, dataRef);
				if(p_geo->modelTexturePath[0] != '\0' && p_geo->modelId != 0)
					p_geo->textureId = npLoadTexture(p_geo->modelTexturePath, 0, dataRef);

				if(p_geo->modelId != 0)
				{
//					npModelStoreDL(assimp->scene[p_geo->modelId], p_geo->geometryId, dataRef);
					npModelStoreDL(assimp->scene[p_geo->geometryId - 999], p_geo->geometryId, dataRef);
				}
				else
					gl->geolist[p_geo->geometryId].geometryId = 0;

			}

		}


//		for(npGeolistSetX(1000, dataRef); npGeolistGetX(dataRef) < (npGeolistGetLen(dataRef)+1000); npGeolistIncX(dataRef))
//		{
//			p_geo = &gl->geolist[npGeolistGetX(dataRef)];
//			if( p_geo->textureId == 0
//				&& p_geo->modelFile[0] != '\0')
//			{
//				p_geo->modelId = npLoadModel(p_geo, dataRef); /// @todo lv, now
//				if(p_geo->modelTexturePath[0] != '\0' && p_geo->modelId != 0)
//					p_geo->textureId = npLoadTexture(p_geo->modelTexturePath, 0, dataRef);
//
//				if(p_geo->modelId != 0)
//				{
//					npModelStoreDL(assimp->scene[p_geo->modelId], p_geo->geometryId, dataRef);
//					npModelStoreDL(assimp->scene[p_geo->geometryId - 999], p_geo->geometryId, dataRef);
//				}
//				else
//					gl->geolist[p_geo->geometryId].geometryId = 0;
//
//			}
//		}

		npGeolistLock(dataRef);
	}
}
*/
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
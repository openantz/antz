/* -----------------------------------------------------------------------------
*
*  npmodels.c
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
#include "npmodels.h"

#include "npassimp.h"
#include "../gl/npgeometry.h"
#include "../gl/nptexmap.h"
#include "../npfile.h"
#include "../../data/npmapfile.h"

#include "../../npdata.h"


int npGeolistFindObjectName(char* objectName, void* dataRef);


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

void npInitGeoMap(void* dataRef);
void npInitGeoMap(void* dataRef)
{
	pData data = (pData) dataRef;
	//pNPgeolist geo = data->io.gl.geoMa;
	int i = 0;

//	data->io.gl.geoMap[0] = &data->io.gl.geolist[0];
	

	for(i = 0; i < kNPgeoMax; i++)
	{
	//	geo = data->io.gl.geoMap[i];
		data->io.gl.geoMap[i] = NULL;

	}
}

/** Inits the Geolist 
	@param dataRef is a global map reference instance.
*/
void npInitModels (void* dataRef)
{
	npInitGeoList(dataRef);
	npInitGeoMap(dataRef);
	//npLoadGeos(dataRef);
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

int npGeoLessId(int geometryId, void* dataRef)
{
	pData data = (pData) dataRef;
	int x = 0;

    while ( (x = npSearchGeosId(geometryId, dataRef)) == 0 && (geometryId > 20) )
		geometryId--;
	
	printf("x : %d\n", x);


	return geometryId;
}


int npGeoMoreId(int geometryId, void* dataRef)
{
	pData data = (pData) dataRef;
	int x = 0;

	if(geometryId == 20)
	{
		geometryId = 1000;
	}

	if(geometryId < 20)
	{
		return geometryId;
	}

	if( (x = npSearchGeosId(geometryId, dataRef)) != 0 && (geometryId > 20) && (geometryId < kNPgeoMax) )
	{
		return geometryId;
	}
	else if(x == 0)
	{
		while ( (x = npSearchGeosId(geometryId, dataRef)) == 0 && (geometryId > 20) && (geometryId < kNPgeoMax) )
			geometryId++;
	}

/*
    while ( (x = npSearchGeosId(geometryId, dataRef)) == 0 && (geometryId > 20) && (geometryId < 2000) )
		geometryId++;
*/

	if(geometryId == kNPgeoMax)
		return 0;

	printf("x : %d\n", x);


	return geometryId;
}

/*
int npGeoLessId(int geometryId, void* dataRef)
{
	pData data = (pData) dataRef;
	int x = 0;

    while ( (x = npSearchGeosId(geometryId, dataRef)) == 0 && (geometryId > 20) )
		geometryId--;
	
	printf("x : %d\n", x);


	return geometryId;
}
*/

// node->texture = npGeoTexId( node->geometry, dataRef );
int npGeoTexId( int geometryId, void* dataRef )
{
	pData data = (pData) dataRef;
	pNPgeolist geolist = &data->io.gl.geolist[0];
	int i = kNPgeoListMax;
	int geoId = geometryId;
	
	//npSearchGeosId(geometryId, dataRef)
	for(; i > 0; i--)
	{
		geolist = &data->io.gl.geolist[i];
		if(geolist->geometryId == geoId)
		{
			return geolist->textureId;
		//	return npExtTexToIntTexId(geolist->textureId, dataRef);	
		}

			//return geolist->textureId;
	}

	return 0;
}

pNPgeolist npSearchGeolistExtTexId(int extTexId, void* dataRef)
{
	pData data = (pData) dataRef;
	pNPgeolist geo = NULL;
	int i = 0;

	if(extTexId == 0)
		return geo;

	for(i = 1; i < kNPgeoListMax; i++)
	{
		geo = &data->io.gl.geolist[i];
		if(geo->textureId == extTexId)
		{
			return geo;
		}
	}

	geo = NULL;
	return geo; 
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

	return NULL;
}



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

/// return a new GeoId
int npNewGeoId(void* dataRef);
int npNewGeoId(void* dataRef)
{
	static int geoId = 999;

	geoId++;
	printf("623982901 geo id %d\n", geoId);
	
	return geoId;
}

/// returns the geoCount
/*
int npNewGeoId(int* geoId, void* dataRef);
int npNewGeoId(int* geoId, void* dataRef)
{
	static int geoCount = 0;
	int x = 0;
	if( (*geoId) < 1000)
		(*geoId) = 1000;

	while( (x = npSearchGeosId((*geoId), dataRef) ) != 0 )
		(*geoId)++;

	geoCount++;
	printf("623982901 geo count %d\n", geoCount);
	return geoCount;
}
*/

pNPgeolist npSearchGeolistFile(char* fn, void* dataRef)
{
	pData data = (pData) dataRef;
	pNPgeolist g = NULL;
	int i = 1;

	printf("81 npSearchGeolistFile : %s\n", fn);

	for(i = 1; i < kNPgeoListMax; i++)
	{
		g = &data->io.gl.geolist[i];
		if( strcmp(g->modelFile, fn) == 0 )
		{

			return g;
		}
	}

	return NULL;
}

pNPgeolist npNewGeo(int* geoId, int* extTexId, char* geoname, pNPfloatXYZ center, pNPfloatXYZ rotate, pNPfloatXYZ scale ,char* filename, char* path, void* dataRef)
{
	pData data = (pData) dataRef;
	pNPgeolist geo = NULL;
	pNPgeolist geoA = NULL;
	char fileP[256] = {'\0'};
	int i = 1;

	//npNewGeoId(geoId, dataRef);
	if( (*geoId) == 0)
		(*geoId) = npNewGeoId(dataRef);

	geo = npGetUnusedGeo(dataRef);

	geo->geometryId = (*geoId);

	if((*extTexId) == 0)
	{
		geoA = npSearchGeolistFile(filename, dataRef);

		if(geoA && geoA->textureId > 0)
		{
			printf("33 FOUND IT.....\n");
			geo->textureId = geoA->textureId;
		}
		else
		{
		//	geo->textureId = npGetUnusedExtTexId(dataRef);
		//	geo->textureId = 0;
			/*
			sprintf(fileP, "%s%s", path, filename);
			geo->textureId = npLoadTexture(fileP, 0, dataRef);
			printf("32 npNewGeo textureId : %d", geo->textureId);
			*/
		}
	}
	else
	{
		for( i = 1; i < kNPgeoMax; i++)
		{
			if( data->io.gl.texmap[i].extTexId == 0 )
				break;
		}

		data->io.gl.texmap[i].extTexId = (*extTexId);

		geo->textureId = (*extTexId); // todo check if extTexId already used
	}

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

	if(center && rotate && scale)
	{
		geo->center.x = center->x;
		geo->center.y = center->y;
		geo->center.z = center->z;

		geo->rotate.x = rotate->x;
		geo->rotate.y = rotate->y;
		geo->rotate.z = rotate->z;
	
		geo->scale.x = scale->x;
		geo->scale.y = scale->y;
		geo->scale.z = scale->z;
	}

	return geo;
}

// geometryId
int npGeoUsedId(int geoId, void* dataRef);
int npGeoUsedId(int geoId, void* dataRef)
{
	pData data = (pData) dataRef;
	pNPgeolist geolist = NULL;
	int i = 1;

	for(i = 1; i < kNPgeoListMax; i++)
	{
		geolist = &data->io.gl.geolist[i];
		if(geolist->geometryId == geoId)
			return i;
	}

	return 0;
}


pNPgeolist npGeoFind(int geoId, int extTexId, int type, char* objName, char* file, char* path, void* dataRef)
{
	pData data = (pData) dataRef;
	pNPgeolist g = &data->io.gl.geolist[0];
	int i = 1;
	
	for( i = 1; i < kNPgeoListMax; i++)
	{
		if( (g[i].geometryId == geoId) && (g[i].textureId == extTexId) && (strcmp(g[i].modelFile, file) == 0) && ( strcmp(g[i].modelPath, path) == 0))
			return &g[i];
	}

	return NULL;
}

int npPathIsRel(char* path, void* dataRef)
{
	char t[256] = {'\0'};
	strcpy(t, path);
	t[0] = tolower(t[0]);
	if(t[0] >= 'a' && t[0] <= 'z' && t[1] == ':' && t[2] == '\\')
		return 0;

	return 1;
}


pNPgeolist npAddGeo(int* geoId, int* extTexId, int type, pNPfloatXYZ center, pNPfloatXYZ rotate, pNPfloatXYZ scale, char* object_name, char* file_name, char* path, void* dataRef)
{
	pData data = (pData) dataRef;
	pNPgeolist geolist = &data->io.gl.geolist[0];
	pNPgeolist geo = NULL;
	pNPgeolist geoB = NULL;
	char* abs = NULL;
	char intPath[256] = {'\0'};
	int i = 0;

//	printf("npAddGeo(%d, %d, %d, NULL, %s, %s, void* dataRef)\n", (*geoId), (*extTexId), type, file_name, path);

	if( npPathIsRel(path, dataRef) )
	{
		abs = npFilePathRelToAbs(path, dataRef);
		strcpy(intPath, abs);
	}
	else
		strcpy(intPath, path);

	
	geo = npGeoFind( (*geoId), (*extTexId), type, object_name, file_name, intPath, dataRef);
	if(geo)
	{
		printf("676767676 npGeoFind Found 6767676767\n");
		printf("fName %s || path %s\n", file_name, intPath);
		return geo;
	}

			printf("626262626 npGeoFind Not Found 6262626262\n");
		printf("fName %s || path %s\n", file_name, intPath);

	geo = npNewGeo(geoId, extTexId, object_name, center, rotate, scale, file_name, intPath, dataRef);

	//npTexReserve(extTexId, dataRef);
	//npTexReserve(
	//t = npTexFind(extId, NULL,  NULL,  , dataRef);
	/*
		if not found, reserve it
		npTexReserve(extId, dataRef); ---> tex->reserved = 1;

	*/

	

	return geo;
	//return geolist;
}

int npExtTexToIntTexId(int extTexId, void* dataRef)
{
	pData data = (pData) dataRef;
//	pNPgeolist geolist = &data->io.gl.geolist[0];
	pNPtexmap texmap = &data->io.gl.texmap[0];
	int i = 0;

	if(extTexId == 0)
	{
		printf("no ext tex\n");
		return 0;
	}
	
	/// kNPtexListMax 2000
	for(i = 1; i < kNPtexMax; i++)
	{
		texmap = &data->io.gl.texmap[i];
		if(texmap->extTexId == extTexId)
		{
			printf("---int %d to ext %d---\n", texmap->intTexId, extTexId);
			printf("---%s%s---\n", texmap->path, texmap->filename);
			return texmap->intTexId;
		}
	}

	return 0;
}


int npIntTexToExtTexId(int intTexId, void* dataRef)
{
	pData data = (pData) dataRef;
//	pNPgeolist geolist = &data->io.gl.geolist[0];
	pNPtexmap texmap = &data->io.gl.texmap[0];
	int i = 0;

//	printf("npIntTexToExtTexId\n");
	/// kNPtexListMax 2000
	for(i = 1; i < kNPtexMax; i++)
	{
		texmap = &data->io.gl.texmap[i];
		if(texmap->intTexId == intTexId)
		{
//			printf("---int %d to ext %d---\n", intTexId, texmap->extTexId);
//			printf("---%s %s---\n", texmap->path, texmap->filename);
			return texmap->extTexId;
		}
	}

	return 0;
}

void npTexMapPrint(void* dataRef);
void npTexMapPrint(void* dataRef)
{
	pData data = (pData) dataRef;
	pNPtexmap texmap = &data->io.gl.texmap[0];
	int i = 1;

	for(i = 1; i < kNPtexMax; i++)
	{
		texmap = &data->io.gl.texmap[i];
		if(texmap->extTexId > 0)
			printf("\nextTexId : %d\nintTexId : %d\n", texmap->extTexId, texmap->intTexId);
	}
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
	pNPtexmap tex = NULL;
	char filePath[256] = {'\0'};
	char fileName[256] = {'\0'};
	char path[256] = {'\0'};
	char* absModelPath = NULL;
	char texfp[256] = {'\0'};
	int modelId = 0;
	int i = 0;
	int geolistIndexMatch = 0;
	int fLen = 0;
	
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

//	absModelPath = npFilePathRelToAbs(geo->modelPath, dataRef);
	if( npPathIsRel(geo->modelPath, dataRef) )
	{
		absModelPath = npFilePathRelToAbs(geo->modelPath, dataRef);
		geo->modelPath[0] = '\0';
		strcpy(geo->modelPath, absModelPath);
		free(absModelPath);
	}

	filePath[0] = '\0';
	sprintf(filePath, "%s%s", geo->modelPath, geo->modelFile);
//	free(absModelPath);
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
//	printf("---------path : %s-------\n", path);
	geo->modelTextureFile[0] = '\0';
	geo->modelTexturePath[0] = '\0';
	strcpy(geo->modelTextureFile, fileName);
	strcpy(geo->modelTexturePath, path);
	tex = npTexlistSearchFile(geo->modelTextureFile, geo->modelTexturePath, dataRef);
	if(tex)
	{
		printf("-=-=-=-=-=-=- geo test npTexlistSearchFile -=-=-=-=-=\n");
		sprintf(texfp, "%s%s", geo->modelTexturePath, geo->modelTextureFile);
		
		if(tex->loaded == 1)
			geo->textureId = tex->intTexId;
		else
			geo->textureId = tex->intTexId = npLoadTexture(texfp, dataRef);

		//return modelId;
	}


	fLen = strlen(fileName);

	for(i = 0; i < fLen; i++)
	{
		fileName[i] = tolower(fileName[i]);
	}

	/// npLoadTex

	if(tex == NULL)
	{
		//tex = npAddTexMap(&geo->textureId, fileName, path, dataRef);
	//	tex = npLoadTexture(texfp, 0, dataRef);
	//	path[0] = '\0';
	//	assimp->path.data[0] = '\0';
	}
	else
	{
		//geo->textureId = tex->intTexId;
	//	geo->textureId = npLoadTexture(tex->fil
		if(tex->intTexId == 0)
			geo->textureId = npLoadTexture(texfp, dataRef);
		else
			geo->textureId = tex->intTexId;

	}

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

//curs = npTextureNewTypeB( curs, &type, extId, dataRef );
char* npTextureNewTypeB(char* idVal, int* type, int extId, void* dataRef);
char* npTextureNewTypeB(char* idVal, int* type, int extId, void* dataRef)
{
	pData data = (pData) dataRef;

	(*type) = npstrtoi(&idVal);
	return idVal;
}

char* npTextureNewType(char* idVal, int* type, int i, void* dataRef);
char* npTextureNewType(char* idVal, int* type, int i, void* dataRef)
{
	pData data = (pData) dataRef;
	pNPtexmap tex = &data->io.gl.texmap[i];
	tex->type = (*type) = npstrtoi(&idVal);
//	pNPtexmap tex =
//	(*type) = npstrtoi(&idVal);
	

	return idVal; 
}

/// @todo lv new func
char* npTextureNewId(char* idVal, int* id, void* dataRef);
char* npTextureNewId(char* idVal, int* id, void* dataRef)
{
	pData data = (pData) dataRef;
	pNPtexmap tex = NULL;
	static int i = 0;
	i++;

	(*id) = npstrtoi(&idVal);
	tex = &data->io.gl.texmap[i];
	tex->extTexId = (*id);
	printf("222 npTextureNewId : %d\n", tex->extTexId);
	//extMap[i] = (*id);
	data->io.gl.extMap[i] = (*id);
	
	printf("333 || extMap ind %d -> %d\n", i, (*id));

	tex->filename[0] = '\0';
	tex->path[0] = '\0';
//	(*id) = i;
	/// tex count++;

	return idVal;

}

//curs = npTextureNewExtIdB( curs, &extId, dataRef );
char* npTextureNewExtIdB( char* idVal, int* extId, void* dataRef)
{
	pData data = (pData) dataRef;
	pNPtexmap tex = NULL;

	(*extId) = npstrtoi(&idVal);
	return idVal;
}

char* npTextureNewExtId(char* idVal, int* extId, void* dataRef);
char* npTextureNewExtId(char* idVal, int* extId, void* dataRef)
{
	pData data = (pData) dataRef;
	pNPtexmap tex = NULL;
	pNPgeolist geo = NULL;
	int i = 1;
	int oldId = 0;
	int tempId = 0;

	data->io.gl.texmapCount++;
	tex = &data->io.gl.texmap[data->io.gl.texmapCount];

	printf("texmap count : %d\n", data->io.gl.texmapCount);

	(*extId) = npstrtoi(&idVal);
	tempId = (*extId);
//	oldId = (*extId);

//	while( data->io.gl.extMapMe[tempId] != NULL )
//		tempId++;

//	(*extId) = tempId;
	printf("2222222222222222222222 New Ext Id is %d\n", (*extId));
	data->io.gl.extMapMe[(*extId)] = tex;
	tex->extTexId = (*extId);
	/*
	if( data->io.gl.extMapMe[(*extId)] == NULL)
		data->io.gl.extMapMe[(*extId)] = tex; 
	*/

	return idVal;
}

char* npTextureNewFilenameB(char* stringVal, int maxSize, char* fileName, int extId, void* dataRef);
char* npTextureNewFilenameB(char* stringVal, int maxSize, char* fileName, int extId, void* dataRef)
{
	pData data = (pData) dataRef;
	pNPtexmap tex = NULL;
	char* curs = stringVal;
	char** read = NULL;
	int strLength = 0;
	int match = 0;
	//int i = 0;
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

//	npCSVstrncpy(fileName, &curs, strLength);
	npCSVstrncpy(fileName, &curs, strLength);
	//strcpy(tex->filename, fileName);
	strcpy(data->io.gl.extMapMe[extId]->filename, fileName);

//	npCSVstrncpy(

	ext = strstr(fileName, ".");
	if( ext == NULL )
	{
		// texture file has no extension, cannot load
		printf("texture file has no extension, cannot load\n");
/// @todo geolist[geoId].loadModel = false;	
	}

	
//	match = npGeolistFindFileName(fileName, dataRef);
//	data->io.gl.texmap[i].filename[0] = '\0';
//	strcpy(data->io.gl.texmap[i].filename, fileName);

	return curs;
}

//	curs = npTextureNewFilePathC( curs, 200, &filePath[0], dataRef );
char* npTextureNewFilePathC(char* idVal, int maxSize, char* filePath, void* dataRef);
char* npTextureNewFilePathC(char* idVal, int maxSize, char* filePath, void* dataRef)
{
	pData data = (pData) dataRef;
	char path[256] = {'\0'};
	char* curs = idVal;
	char** read = NULL;
	char* dslash = NULL;
	char* abs = NULL;
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
//	strcpy(tex->path, path);

	i = 0;
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
//	data->io.gl.texmap[index].path[0] = '\0';
//	strcpy(data->io.gl.texmap[index].path, path);

//char* npFilePathRelToAbs(char* rel, void* dataRef)
	if( npPathIsRel(filePath, dataRef) )
	{
		abs = npFilePathRelToAbs(filePath, dataRef);
		filePath[0] = '\0';
		strcpy(filePath, abs);
		free(abs);
	}

	return curs;
}

char* npTextureNewFilenameC(char* stringVal, int maxSize, char* fileName, void* dataRef);
char* npTextureNewFilenameC(char* stringVal, int maxSize, char* fileName, void* dataRef)
{	
	pData data = (pData) dataRef;
	pNPtexmap tex = NULL;
	char* curs = stringVal;
	char** read = NULL;
	int strLength = 0;
	int match = 0;
	//int i = 0;
	char* ext = NULL;
//	strLength = maxSize - (curs - *read);
	strLength = maxSize;
	
//	tex = &data->io.gl.texmap[i];
	/*
	if (strLength >= kNPmodelFileName)
		strLength = kNPmodelFileName;
	*/
	if (strLength >= 200 )
		strLength = 200;
	
	fileName[0] = '\0';

//	npCSVstrncpy(fileName, &curs, strLength);
	npCSVstrncpy(fileName, &curs, strLength);

//	npCSVstrncpy(

	ext = strstr(fileName, ".");
	if( ext == NULL )
	{
		// texture file has no extension, cannot load
		printf("texture file has no extension, cannot load\n");
/// @todo geolist[geoId].loadModel = false;	
	}

	
//	match = npGeolistFindFileName(fileName, dataRef);
//	data->io.gl.texmap[i].filename[0] = '\0';
//	strcpy(data->io.gl.texmap[i].filename, fileName);

	return curs;
}

char* npTextureNewFilename(char* stringVal, int maxSize, char* fileName, int i, void* dataRef);
char* npTextureNewFilename(char* stringVal, int maxSize, char* fileName, int i, void* dataRef)
{
	pData data = (pData) dataRef;
	pNPtexmap tex = NULL;
	char* curs = stringVal;
	char** read = NULL;
	int strLength = 0;
	int match = 0;
	//int i = 0;
	char* ext = NULL;
//	strLength = maxSize - (curs - *read);
	strLength = maxSize;
	
	tex = &data->io.gl.texmap[i];
	/*
	if (strLength >= kNPmodelFileName)
		strLength = kNPmodelFileName;
	*/
	if (strLength >= 200 )
		strLength = 200;
	
	fileName[0] = '\0';

//	npCSVstrncpy(fileName, &curs, strLength);
	npCSVstrncpy(fileName, &curs, strLength);
	strcpy(tex->filename, fileName);

//	npCSVstrncpy(

	ext = strstr(fileName, ".");
	if( ext == NULL )
	{
		// texture file has no extension, cannot load
		printf("texture file has no extension, cannot load\n");
/// @todo geolist[geoId].loadModel = false;	
	}

	
//	match = npGeolistFindFileName(fileName, dataRef);
//	data->io.gl.texmap[i].filename[0] = '\0';
//	strcpy(data->io.gl.texmap[i].filename, fileName);

	return curs;
}

char* npTextureNewFilePathB(char* stringVal, int maxSize, char* filePath, int extId, void* dataRef);
char* npTextureNewFilePathB(char* stringVal, int maxSize, char* filePath, int extId, void* dataRef)
{
	pData data = (pData) dataRef;
	pNPtexmap tex = NULL;
	char path[256] = {'\0'};
	char* curs = stringVal;
	char** read = NULL;
	char* dslash = NULL;
	char* abs = NULL;
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
//	strcpy(tex->path, path);


	i = 0;
	i = len = strlen(path);

	if(len == 0)
	{
		if( npPathIsRel(filePath, dataRef) )
		{
			abs = npFilePathRelToAbs(filePath, dataRef);
			strcpy(data->io.gl.extMapMe[extId]->path, abs);
			free(abs);
		}
		else
			strcpy(data->io.gl.extMapMe[extId]->path, filePath);

		return curs;
	}

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

	if( npPathIsRel(filePath, dataRef) )
	{
		abs = npFilePathRelToAbs(filePath, dataRef);
		strcpy(data->io.gl.extMapMe[extId]->path, abs);
		free(abs);
	}
	else
		strcpy(data->io.gl.extMapMe[extId]->path, filePath);

		
	/*
	while( filePath[i] != nposGetFolderDelimit() && i >= 0 )
	{
		i--;
	}

	filePath[i+1] = '\0';
	*/
//	data->io.gl.texmap[index].path[0] = '\0';
//	strcpy(data->io.gl.texmap[index].path, path);

	return curs;
}

char* npTextureNewFilePath(char* stringVal, int maxSize, char* filePath, int i, void* dataRef);
char* npTextureNewFilePath(char* stringVal, int maxSize, char* filePath, int i, void* dataRef)
{
	pData data = (pData) dataRef;
	pNPtexmap tex = &data->io.gl.texmap[i];
	char path[256] = {'\0'};
	char* curs = stringVal;
	char** read = NULL;
	char* dslash = NULL;
	int strLength = 0;
	int len = 0;
//	int i = 0;
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
	strcpy(tex->path, path);

	i = 0;
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
//	data->io.gl.texmap[index].path[0] = '\0';
//	strcpy(data->io.gl.texmap[index].path, path);

	return curs;
}



void npTextureNew(char* tex_csvline, void* dataRef)
{	
	pData data = (pData) dataRef;
	pNPtexmap tex = NULL;
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
	int i = 0;
	int extId = 0;
	int type = 0;
	int index = 0;

	printf("99 npTextureNew\n");
	curs = npTextureNewExtId( curs, &extId, dataRef );
	curs = npTextureNewTypeB( curs, &type, extId, dataRef );
	curs = npTextureNewFilenameB( curs, 200, &fileName[0], extId, dataRef);
	curs = npTextureNewFilePathB( curs, 200, &filePath[0], extId, dataRef );
	printf("99 npTextureNew CLOSE\n", npTextureNew);
	return;
}

void npTextureNewB(char* tex_csvline, void* dataRef)
{
	pData data = (pData) dataRef;
	char* curs = tex_csvline;
	char fileName[256] = {'\0'};
	char filePath[256] = {'\0'};
	pNPtexmap tex = NULL;
	pNPtexmap extMapMe = NULL;
	int tempId = 0;
	int extId = 0;
	int type = 0;
	printf("100 npTextureNew 100\n");




	curs = npTextureNewExtIdB( curs, &extId, dataRef );
	curs = npTextureNewTypeB( curs, &type, extId, dataRef );
	curs = npTextureNewFilenameC( curs, 200, &fileName[0], dataRef );
	curs = npTextureNewFilePathC( curs, 200, &filePath[0], dataRef );

	extMapMe = data->io.gl.extMapMe[extId];
	
	if(extMapMe != NULL) // check if already exists
	{ 
		if(extMapMe->extTexId == extId && strcmp(extMapMe->filename, fileName) == 0
		&& strcmp(extMapMe->path, filePath) == 0 )
		{
			// Already in list
			return;
		}
		else
		{
			// Not in list, add
			data->io.gl.texmapCount++;
			tex = &data->io.gl.texmap[data->io.gl.texmapCount];

			tempId = extId;
			while( data->io.gl.extMapMe[tempId] != NULL )
				tempId++;

			extId = tempId;
			printf("2222222222222222222222 New Ext Id is %d\n", extId);
			data->io.gl.extMapMe[extId] = tex;
			tex->extTexId = extId;
			tex->type = type;
			strcpy(tex->filename, fileName);
			strcpy(tex->path, filePath);
			//tex->
		}

	}
	else
	{
		// Isn't in use, use

		data->io.gl.texmapCount++;
		tex = &data->io.gl.texmap[data->io.gl.texmapCount];

//		data->io.gl.extMapMe[extId] = tex;
		extMapMe = tex;

		extMapMe->extTexId = extId;
		strcpy(extMapMe->filename, fileName);
		strcpy(extMapMe->path, filePath);
		extMapMe->type = type;
		extMapMe->loaded = 0;
	}

	

//	curs = npTextureNewFilenameC( curs, 200, &fileName[0], dataRef);
/*
	curs = npTextureNewExtIdB( curs, &extId, dataRef );
	curs = npTextureNewTypeC( curs, &type, dataRef);
	curs = npTextureNewFilenameC( curs, 200, &fileName[0], dataRef);
	curs = npTextureNewFilePathC( curs, 200, &filePath[0], dataRef);

	if(extMapMe[extId])
	{
		if( strcmp(extMapMe[extId].fileName, fileName) == 0
		&& strcmp(extMapMe[extId].filePath, filePath) == 0 )
		{
			return;
		}
		else
		{
			tempId = extId;
			while( data->io.gl.extMapMe[tempId] != NULL )
				tempId++;

			(*extId) = tempId;
			printf("2222222222222222222222 New Ext Id is %d\n", extId);
			data->io.gl.extMapMe[(*extId)] = tex;
			tex->extTexId = (*extId);
		}

	}
*/

}

char* npModelNewCenter(char* curs, float* x, float* y, float* z, void* dataRef);
char* npModelNewCenter(char* curs, float* x, float* y, float* z, void* dataRef)
{
//	pNPgeolist geolist = npGetGeolist(dataRef);
	pNPgeolist geolist = NULL;

	//(*geoId) = npstrtoi(&idVal); // lv, if letter, returns 0
	(*x) = npstrtof(&curs);
	(*y) = npstrtof(&curs);
	(*z) = npstrtof(&curs);

	return curs;
}

char* npModelNewGeoIdB( char* idVal, int* geometryId, void* dataRef)
{
//	pNPgeolist geolist = npGetGeolist(dataRef);
	pData data = (pData) dataRef;

	(*geometryId) = npstrtoi(&idVal);
//	count = npNewGeoId(geoId, dataRef);
	/*
	(*geoId) = npNewGeoId(dataRef);
	while( data->io.gl.geolist[(*x)].geometryId != 0)
		(*x)++;

	data->io.gl.geolist[(*x)].geometryId = (*geoId);
	printf("2839 x %d\n", x);
	*/
	//printf("74920 new geo id %d\n", (*geoId));
	/*
	(*geoId) = npstrtoi(&idVal); // lv, if letter, returns 0
	geo = &data->io.gl.geolist[i-999];
	geo->geometryId = (*geoId);
	printf("npModelNewGeoId : %d\n", (*geoId));;
	// @todo add (*geoId) to extGeoList

	(*geoId) = i - 999;
	*/

	return idVal;
	
}


char* npModelNewTextureIdB(char* idVal, int* textureId, void* dataRef)
{
	pData data = (pData) dataRef;
//	pNPgeolist geo = &data->io.gl.geolist[i];
	pNPtexmap tex = NULL;

	printf("82934 npModelNewTextureId\n");

	(*textureId) = npstrtoi(&idVal);

	/*
	tex = npTexlistSearchId(1, textureId, dataRef);
	if(tex)
	{
		(*textureId) = tex->intTexId;
		geo->textureId = (*textureId);
	}
	*/

	/*
//	pNPgeolist geolist = npGetGeolist(dataRef);
	pNPtexmap tex = NULL;
	int match = 0;

	(*textureId) = npstrtoi(&idVal);

	if( (*textureId) < 0 )
	{
		printf("\ntextureId is < 0");
		/// @todo: Find unused textureId from internal/external textureId map
	}

	tex = npTexlistSearchId(1, textureId, dataRef);
	if(tex && (tex->intTexId > 0))
	{
		(*textureId) = tex->extTexId;
	//	(*intId) = tex->intTexId;
	//	printf("model csv tex (int %d) (ext %d)\n", (*intId), (*textureId));
	}

	/// @todo if textureId exists in geolist return it,
	///		  if not then return csv's textureId
///	@todo match = npGeolistFindTextureId(textureId, dataRef);
	if(match != 0)
		(*textureId) = match;
	*/
	return idVal;
}


pNPgeolist npGeoId(int action, int* xGeoId, char* path, char* file, void* dataRef)
{
	pData data = (pData) dataRef;
	pNPgeolist geoMap = NULL;
//	pNPgeolist geoMap = data->io.gl.geoMap[(*xGeoId)];
	int geoCount = data->io.gl.geoLen;
	static int gGeoId = 999;
	char fp[256] = {'\0'};
	pNPgeolist geo = &data->io.gl.geolist[geoCount];
	
	geoMap = data->io.gl.geoMap[(*xGeoId)];

	if(action == kNPgeoLoad)
	{
		npLoadModel(geoMap, dataRef);
	}

	if(action == kNPgeoLoadTex)
	{
		if(geoMap && geoMap->loaded == 0
			&& geoMap->geometryId > 0)
		{
			sprintf(fp, "%s%s", geoMap->modelTexturePath, geoMap->modelTextureFile);
			geoMap->textureId = npLoadTexture(fp, dataRef);
			return geoMap;
		}
		
		return geoMap;
	}
	

	if( action == kNPgetGeoId)
	{
		///@todo check file/path match
		if(geoMap)
		{
			if(  (strcmp(geoMap->modelPath, path) == 0) &&
				 (strcmp(geoMap->modelFile, file) == 0) )
					return geoMap;
			else
				(*xGeoId++); return NULL;
		}
			
		return NULL;
	}
			
	if( action == kNPsetGeoId )
	{
		// check if already mapped
		if( !(geo = npGeoId(kNPgetGeoId, xGeoId, path, file, dataRef)) )
		{
			// geo not set, set npSetGeo
			geoCount = (++data->io.gl.geoLen);
			geo = &data->io.gl.geolist[geoCount];
			
			strcpy(geo->modelPath, path); strcpy(geo->modelFile, file);
			
			if( (*xGeoId) <= 0 )
				geo->geometryId = (*xGeoId) = (++gGeoId);
			else
				geo->geometryId = (++gGeoId);

			data->io.gl.geoMap[(*xGeoId)] = geo;
			return geo;
		}
		else
		{
			
		}

	}

	return NULL;

	/*
		pNPgeolist npGeoId(int action, int xGeoId, char* path, char* filename, void* dataRef)
			vars: pNPgeolist geo
			- actions
				Get
				{
					if(geoMap[xGeoId])
						return geoMap[xGeoId];
					else
						return NULL;
				}

				Set
				{
					/// check if exists
					if( geo = npGeoId(kNPgeoGet, xGeoId, path, filename, dataRef) )
					{
						return
					}

					geoMap[xGeoId].extId = xGeoId;
					

				}

		pNPgeolist geoMap[2000]
			geoMap[xGeoId]
			*/
}

void npLoadExtGeos(void* dataRef)
{
	pData data = (pData) dataRef;
	char fp[256] = {'\0'};
	int i;

	
	for(i = kNPgeoMax / 2; i < kNPgeoMax; i++)
	{
		if( data->io.gl.geoMap[i] &&
			data->io.gl.geoMap[i]->loaded == 0
			&&
			data->io.gl.geoMap[i]->modelFile[0] != '\0'
			&&
			data->io.gl.geoMap[i]->modelPath[0] != '\0'
			&&
			data->io.gl.geoMap[i]->textureId == 0)
		{
			printf("924 loading geoMap[%d]\n", i);
			npLoadModel(data->io.gl.geoMap[i], dataRef);
		}
	}
	/*
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
			sprintf(fp, "%s%s", data->io.gl.extMapMe[i]->path, data->io.gl.extMapMe[i]->filename);
			data->io.gl.extMapMe[i]->intTexId = npLoadTexture(fp, 0, dataRef);
			if(data->io.gl.extMapMe[i]->intTexId > 0)
				data->io.gl.extMapMe[i]->loaded = 1;
		}
	}
	*/

}


pNPgeo npModelNew(char* model_csvline, void* dataRef)
{	
	pData data = (pData) dataRef;
	pNPgeo geo = NULL;
//	char* curs = &model_csvline;
	char* curs = model_csvline;
	char* foundPath = NULL;
	char objectName[256] = {'\0'};
	char fileName[256] = {'\0'};
	char filePath[256] = {'\0'};
	int geometryId = 0;
	int textureId = 0;
	int intId = 0;
	int typeId = 0;
//	int cx,cy,cz = 0;
//	int rx,ry,rz = 0;
//	int sx,sy,sz = 0;
	int i = 0;
	int x = 0;
	NPfloatXYZ center;
	NPfloatXYZ rotate;
	NPfloatXYZ scale;

	printf("339 npModelNew\n");

	printf("npModelNewGeoId\n");
//	curs = npModelNewGeoId( curs, &geometryId, &x, dataRef ); // always return unique
	curs = npModelNewGeoIdB(curs, &geometryId, dataRef);
	//	i = geometryId-999;

	printf("npModelNewTextureId\n");
//	curs = npModelNewTextureId( curs, &textureId, x, dataRef ); // lv, temp
	curs = npModelNewTextureIdB(curs, &textureId, dataRef);

	printf("npModelNewTypeId\n");
	curs = npModelNewTypeId( curs, &typeId, dataRef );
	
	curs = npModelNewCenter( curs, &center.x, &center.y, &center.z, dataRef);
	curs = npModelNewCenter( curs, &rotate.x, &rotate.y, &rotate.z, dataRef);
	curs = npModelNewCenter( curs, &scale.x,  &scale.y,  &scale.z,  dataRef);

	curs = npModelNewObjectName(curs, 200, &objectName[0], dataRef);
	curs =  npModelNewFileName(curs, 200, &fileName[0], x, dataRef);
	curs = npModelNewFilePath(curs, 200, &filePath[0], x, dataRef); 

//	pNPgeolist npGeoId(int action, int xGeoId, char* path, char* file, void* dataRef)
	geo = npGeoId(kNPsetGeoId, &geometryId, filePath, fileName, dataRef); // generalize
	if(!geo)
		return NULL;
	else
	{
		geo->center.x = center.x; geo->center.y = center.y; geo->center.z = center.z;
		geo->rotate.x = rotate.x; geo->rotate.y = rotate.y; geo->rotate.z = rotate.z;
		geo->scale.x = scale.x; geo->scale.y = scale.y; geo->scale.z = scale.z;
		
		if( data->io.gl.extMapMe[textureId] && (data->io.gl.extMapMe[textureId]->extTexId > 0) )
		{
			geo->extTexId = textureId;	
			/*
			if(data->io.gl.extMapMe[textureId]->intTexId > 0)
				geo->textureId = data->io.gl.extMapMe[textureId]->intTexId;
			else
				data->io.gl.extMapMe[textureId]->extTexId = geo->textureId;
			*/
		}
	}

	/// @todo geoMap like extMap

	// check geolist
	/*
	for(i = 1; i < 2000; i++)
	{
		geo = &data->io.gl.geolist[i];
		if(geo->geometryId == geometryId)
		{
			if(strcmp(geo->modelFile, fileName) == 0 &&
			strcmp(geo->modelPath, filePath) == 0)
			{	
				printf("68889 geo already in geolist\n");
				return geo;
			}
			else
			{ // increment geometryId and look again starting at the beginning of the loop
				geometryId++;
				i++;
			}
		}
		*/
		/*
		if(geo->geometryId == geometryId &&
			strcmp(geo->modelFile, fileName) == 0 &&
			strcmp(geo->modelPath, filePath) == 0)
		{
			printf("68889 geo already in geolist\n");
			return geo;
		}
		*/
	//}

	if(i == kNPgeoMax)
	{
		geo = npGetUnusedGeo(dataRef);
	//	geo->geometryId = 
	}

	/*
	void npModelNewCSR(int* cx, int* cy, int* cz, int* rx, int* ry, int* rz, int* sx, int* sy, int* sz, void* dataRef)
		npModelNewXYZ(kNPcenter, cx, cy, cz, dataRef);
		npModelNewXYZ(kNPscale, sx, sy, sz, dataRef);
		npModelNewXYZ(kNProtate, rx, ry, rz, dataRef);

	npModelNewXYZ(int type, int* x, int* y, int* z, void* dataRef);
	{
		switch(type)
		{
			case:kNPcenter
				npModelNewCenter(char* curs, int* x, int* y, int* z, void* dataRef);
				break;
			case:kNPscale
				npModelNewScale(char* curs, int* x, int* y, int* z, void* dataRef);
				break;
			case:kNProtate
				npModelNewRotate(char* curs, int* x, int* y, int* z, void* dataRef);
				break;
			npModelNewRotate(char* curs, int* x, int* y, int* z, void* dataRef);
		}
	}


	*/


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
//	npAddGeo(&geometryId, &textureId, 0, &center, &rotate, &scale, objectName, fileName, filePath, dataRef); // lv temp comment
	
	return geo;
}



char* npModelNewFilePath(char* stringVal, int maxSize, char* filePath, int i, void* dataRef)
{
	pData data = (pData) dataRef;
//	pNPgeolist geo = &data->io.gl.geolist[i];
	char* curs = stringVal;
	char** read = NULL;
	char* abs = NULL;
	int strLength = 0;
	int len = 0;
//	int i = 0;
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
//	strcpy(geo->modelPath, filePath);

	i = len = strlen(filePath);
	
	if(filePath[strlen(filePath)-1] != '\\' && len > 0)
	{
		filePath[strlen(filePath)+1] = '\0';
		filePath[strlen(filePath)] = '\\';
	}
/*
	if(len == 0)
		return curs;
*/		
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




char* npModelNewFileName(char* stringVal, int maxSize, char* fileName, int i, void* dataRef)
{
	pData data = (pData) dataRef;
	pNPgeolist geo = &data->io.gl.geolist[i];
	char* curs = stringVal;
	char** read = NULL;
	int strLength = 0;
	int match = 0;
//	int i = 0;
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
//	strcpy(geo->modelFile, fileName);

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






char* npModelNewTextureId(char* idVal, int* textureId, int i, void* dataRef)
{
	pData data = (pData) dataRef;
	pNPgeolist geo = &data->io.gl.geolist[i]; // i ??
	pNPtexmap tex = NULL;

	printf("82934 npModelNewTextureId\n");

	(*textureId) = npstrtoi(&idVal);
	if(data->io.gl.extMapMe[(*textureId)] && data->io.gl.extMapMe[(*textureId)]->extTexId == (*textureId)
		&& data->io.gl.extMapMe[(*textureId)]->intTexId != 0)
	{
		printf("TEX id %d exists in extMapMe with intTexId of %d\n", (*textureId), data->io.gl.extMapMe[(*textureId)]->intTexId);
		printf("%s %s\n", data->io.gl.extMapMe[(*textureId)]->filename, data->io.gl.extMapMe[(*textureId)]->path );
		(*textureId) = data->io.gl.extMapMe[(*textureId)]->intTexId;
		geo->textureId = (*textureId);
	//	printf("Geo : %s %s", geo->

	}
	/*
	tex = npTexlistSearchId(1, textureId, dataRef);
	if(tex)
	{
		(*textureId) = tex->intTexId;
		geo->textureId = (*textureId);
	}
	*/

	/*
//	pNPgeolist geolist = npGetGeolist(dataRef);
	pNPtexmap tex = NULL;
	int match = 0;

	(*textureId) = npstrtoi(&idVal);

	if( (*textureId) < 0 )
	{
		printf("\ntextureId is < 0");
		/// @todo: Find unused textureId from internal/external textureId map
	}

	tex = npTexlistSearchId(1, textureId, dataRef);
	if(tex && (tex->intTexId > 0))
	{
		(*textureId) = tex->extTexId;
	//	(*intId) = tex->intTexId;
	//	printf("model csv tex (int %d) (ext %d)\n", (*intId), (*textureId));
	}

	/// @todo if textureId exists in geolist return it,
	///		  if not then return csv's textureId
///	@todo match = npGeolistFindTextureId(textureId, dataRef);
	if(match != 0)
		(*textureId) = match;
	*/
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

	for(i = kNPgeoMax / 2; i < kNPgeoMax; i++)
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
	for(i = kNPgeoMax / 2; i < kNPgeoMax; i++)
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
	int geoId = (kNPgeoMax / 2) - 1;
//	pNPgeolist geolist = npGetGeolist(dataRef);
	pNPgeolist geolist = NULL;
	
	while( geolist[++geoId].geometryId != 0 );
	if(geoId > kNPgeoMax)
		return 0;
	
	return geoId;
}


char* npModelNewGeoId(char* idVal, int* geoId, int* x, void* dataRef)
{
//	pNPgeolist geolist = npGetGeolist(dataRef);
	pData data = (pData) dataRef;
	pNPgeolist geo = NULL;
	int match = 0;
//	int x = 1;
	int count = 0;
	static int i = 999;
	i++;

	(*x) = 1;

	(*geoId) = npstrtoi(&idVal);
//	count = npNewGeoId(geoId, dataRef);
	(*geoId) = npNewGeoId(dataRef);
	while( data->io.gl.geolist[(*x)].geometryId != 0)
		(*x)++;

	data->io.gl.geolist[(*x)].geometryId = (*geoId);
	printf("2839 x %d\n", x);

	printf("74920 new geo id %d\n", (*geoId));
	/*
	(*geoId) = npstrtoi(&idVal); // lv, if letter, returns 0
	geo = &data->io.gl.geolist[i-999];
	geo->geometryId = (*geoId);
	printf("npModelNewGeoId : %d\n", (*geoId));;
	// @todo add (*geoId) to extGeoList

	(*geoId) = i - 999;
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
				npModelStoreDL(assimp->scene[modelId], geolist, dataRef);
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

	scene = (struct aiScene*) aiImportFile( filePath, 0);

	if( scene == NULL )
		printf("err 4088 - aiImportFile returned null");

	return scene;
}



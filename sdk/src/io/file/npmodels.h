/* -----------------------------------------------------------------------------
*
*  npmodels.h
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

#ifndef NPMODELS_H_
#define NPMODELS_H_

#include "../../npdata.h"
//#include "npassimp.h"		//zz models

/// List of models table keeps track of file name, path, scale, orientation,
/// polygon count, GPU memory required, etc.
struct NPmodels {
	int		id;		///< this models ID
	int		geometryID;		///< geomtery ID assigned for the mesh
	int		textureID;		///< texture map ID

	char	fileName[kNPmaxName];
	char	fileDir[kNPmaxPath];

	long long int gpuSize;		///< GPU memory required for the model geometry	
};							
typedef struct NPmodels NPmodels;
typedef struct NPmodels *pNPmodels;

/// Contains the file name, geometry_id and texture_id of the model
/// Separate 'models' table keeps track of additional attributes
/*
struct NPgeo {
	int		id;				///< geomtery ID assigned for the mesh
	int		textureID;		///< assigned texture ID or 0 for default
	int		modelsID;		///< models_id for getting additional attributes
};							
typedef struct NPgeo NPgeo;
typedef struct NPgeo *pNPgeo;
*/

//------------------------------------------------------------------------------
void npInitModels (void* dataRef);
void npCloseModels (void* dataRef);
//pNPgeolist npAddGeo(int* geoId, int* extTexId, int type, char* object_name, char* file_name, char* path, void* dataRef);
pNPgeolist npAddGeo(int* geoId, int* extTexId, int type, pNPfloatXYZ center, pNPfloatXYZ rotate, pNPfloatXYZ scale, char* object_name, char* file_name, char* path, void* dataRef);

//pNPmodels npLoadModel( const char* path, char* filename, void* dataRef );

int npLoadModel(pNPgeolist geo, void* dataRef);
pNPgeolist npLoadModelFromFile(char* filepath, void* dataRef);

void npUpdateGeoList( void* dataRef );

int	npGeolistGetX(void* dataRef); 
void npGeolistSetX(int X, void* dataRef);
void npGeolistIncX(void* dataRef);

int npGeolistGetLen(void* dataRef);
void npGeolistSetLen(int length, void* dataRef);
int npGeolistIncLen(void* dataRef);

void npGeolistLock(void* dataRef);
void npGeolistUnlock(void* dataRef);

bool npGeolistLockStatus(void* dataRef);
int npGeoMoreId(int geometryId, void* dataRef);
int npGeoLessId(int geometryId, void* dataRef);

struct aiScene* npModelImport(char* filePath, void* dataRef);

int npExtTexToIntTexId(int extTexId, void* dataRef);

pNPgeolist npGetGeolist(void* dataRef);

pNPgeo npModelNew(char* model_csvline, void* dataRef);

int npSearchGeosId(int geoId, void* dataRef);

//char* npModelNewGeoId(char* idVal, int* geoId, void* dataRef);
char* npModelNewGeoId(char* idVal, int* geoId, int* x, void* dataRef);

char* npModelNewTextureId(char* idVal, int* textureId, int i, void* dataRef);

char* npModelNewTypeId(char* csv_typeId, int* typeId, void* dataRef);

char* npModelNewObjectName(char* stringVal, int maxSize, char* objectName, void* dataRef);

char* npModelNewFileName(char* stringVal, int maxSize, char* fileName, int i, void* dataRef);

char* npModelNewFilePath(char* stringVal, int maxSize, char* filePath, int i, void* dataRef);

void npSetSelectedNodeGeoId( int* geoId, void* dataRef );

int npGeolistNewGeoId(void* dataRef);

int npIntTexToExtTexId(int intTexId, void* dataRef);

int npPathIsRel(char* path, void* dataRef);

void npTextureNew(char* tex_csvline, void* dataRef);
void npTextureNewB(char* tex_csvline, void* dataRef);

void npLoadExtGeos(void* dataRef);

#define kNPgetGeoId 0
#define kNPsetGeoId 1
#define kNPgeoLoadTex 2
#define kNPgeoLoad 3
pNPgeolist npGeoId(int action, int* xGeoId, char* path, char* file, void* dataRef);

pNPgeolist npSearchGeolistExtTexId(int extTexId, void* dataRef); //zz models

#endif


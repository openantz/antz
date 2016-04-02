/* -----------------------------------------------------------------------------
*
*  npmodels.h
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

#ifndef NPMODELS_H_
#define NPMODELS_H_

#include "../../npdata.h"

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
pNPgeolist npAddGeo(int geoId, int extTexId, int type, char* object_name, char* file_name, char* path, void* dataRef);
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

struct aiScene* npModelImport(char* filePath, void* dataRef);

pNPgeolist npGetGeolist(void* dataRef);
char* npModelNewGeoId(char* idVal, int* geoId, void* dataRef);
char* npModelNewTextureId(char* idVal, int* textureId, void* dataRef);
char* npModelNewTypeId(char* csv_typeId, int* typeId, void* dataRef);
char* npModelNewObjectName(char* stringVal, int maxSize, char* objectName, void* dataRef);
char* npModelNewFileName(char* stringVal, int maxSize, char* fileName, void* dataRef);
char* npModelNewFilePath(char* stringVal, int maxSize, char* filePath, void* dataRef);
void npSetSelectedNodeGeoId( int* geoId, void* dataRef );
int npGeolistNewGeoId(void* dataRef);

#endif


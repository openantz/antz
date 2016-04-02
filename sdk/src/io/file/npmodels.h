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
struct NPgeo {
	int		id;				///< geomtery ID assigned for the mesh
	int		textureID;		///< assigned texture ID or 0 for default
	int		modelsID;		///< models_id for getting additional attributes
};							
typedef struct NPgeo NPgeo;
typedef struct NPgeo *pNPgeo;

//------------------------------------------------------------------------------
void npInitModels (void* dataRef);
void npCloseModels (void* dataRef);
pNPmodels npLoadModels( const char* filePath, void* dataRef );

#endif


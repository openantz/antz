/* -----------------------------------------------------------------------------
*
*  npassimp.h
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
#ifndef NPASSIMP_H_
#define NPASSIMP_H_


#include "../../npdata.h"
#include "../../os/npos.h"

//zz models begin
#include "../../../libs/assimp/include/assimp/cimport.h"
#include "../../../libs/assimp/include/assimp/material.h"
#include "../../../libs/assimp/include/assimp/postprocess.h"
#include "../../../libs/assimp/include/assimp/scene.h"
//zz models end

#define maxScenes 1001
struct NPassimp {
	bool	running; //! flag
	struct	aiString path; //! texture path, access with assimp->path.data
	// char	filepath[200];
	struct	aiScene *scene[maxScenes];
	int		numOfScenes;
	struct	aiMesh *mesh[500];
	int		numOfMeshes;
};
typedef struct NPassimp NPassimp;
typedef NPassimp* pNPassimp;


struct aiString npAssimpGetTexturePath(int sceneIndex, void* dataRef);
int npAssimpLoadModel(char* filePath, int* geolistIndexMatch, void* dataRef);
void npAssimpInit(void* dataRef);

#endif

#include "npassimp.h"
#include "../gl/npgeometry.h"
#include "../gl/nptexmap.h"
#include "../npfile.h"
#include "SOIL.h"					//zz debug should just be SOIL.h

// ----------------------------------------------------------------------------
/** Initialize Assimp (Model Library).
	@param dataRef is a global map reference instance.
*/
void npAssimpInit(void* dataRef)
{
	pData data = (pData) dataRef;
	pNPassimp assimp = data->io.assimp;
	int index = 0;
	
	assimp = (pNPassimp)malloc(sizeof(NPassimp));
	if(assimp == NULL)
	{
		printf("\nmalloc failed (npAssimpInit)");
		assimp->running = false;
		return;
	}

	data->io.assimp = assimp;
	assimp->numOfScenes = 0;

	for(index = 0; index <= 49; index++)
	{
		assimp->scene[index] = NULL;
	}

	assimp->running = true;
	return;
}

/** Gets the texture path for the first model in the scene
	@param sceneIndex references an assimp scene
	@param dataRef is a global map reference instance.
	@returns An aiString containing the texture path	
*/
struct aiString npAssimpGetTexturePath(int sceneIndex, void* dataRef)
{
	pData data = (pData) dataRef;
	pNPassimp assimp = (pNPassimp)data->io.assimp;
	struct aiScene* scene = NULL;
	struct aiMaterial* material = NULL;
    static const int types[] = {
          aiTextureType_NONE,
          aiTextureType_DIFFUSE,
          aiTextureType_SPECULAR,
          aiTextureType_AMBIENT,
          aiTextureType_EMISSIVE,
          aiTextureType_HEIGHT,
          aiTextureType_NORMALS,
          aiTextureType_SHININESS,
          aiTextureType_OPACITY,
          aiTextureType_DISPLACEMENT,
          aiTextureType_LIGHTMAP,
          aiTextureType_REFLECTION,
          aiTextureType_UNKNOWN
    };
	unsigned int type = 0;
	unsigned int idx = 0;
	unsigned int total = 0;

	assimp->path.data[0] = '\0';

//	if(sceneIndex < 1 || sceneIndex > 
	/*
	if(sceneIndex < 1 || sceneIndex > assimp->numOfScenes)
	{
		printf("\nInvalid Scene Index");
		return assimp->path;
	}
	*/
	if( assimp->scene[sceneIndex] == NULL)
	{
		printf("\nScene not loaded");
		return;
	}

	scene = assimp->scene[sceneIndex]; /// assimp->scene[0] is dummy value (NULL) 
	material = scene->mMaterials[0];
	
	for(type = 0; type < sizeof(types)/sizeof(types[0]); ++type)
	{
	//	printf("\ntype : %d", type);
		for(idx = 0; AI_SUCCESS==aiGetMaterialString(scene->mMaterials[0],
			AI_MATKEY_TEXTURE(types[type],idx),&assimp->path); idx++)
		{
//				printf("%s\n    \'%s\'",(total++?"":"\nTexture Refs:" ),assimp->path.data);
			total++;
		}
	}

	return assimp->path;
}

int npAssimpLoadModel2(char* filePath, void* dataRef);
int npAssimpLoadModel2(char* filePath, void* dataRef)
{
	pData data = (pData) dataRef;
	char filename[256] = {'\0'};
	/*
	int i = 0;
	int modelId = 0;
	int textureId = 0;
	*/
	pNPassimp assimp = (pNPassimp)data->io.assimp;

	if( (assimp->running == false) || (assimp == NULL) ) 
		return 0;
	
	npModelImport(filePath, dataRef);

	npGetFileNameFromPath(filePath, &filename[0], dataRef);
	


	return 0;
}

/** Loads the assimp model 
	@param filePath, path or filename
	@param dataRef is a global map reference instance.
	@return the model id
*/

int npAssimpLoadModel(char* filePath, int* geolistIndexMatch, void* dataRef)
{
	char filename[256] = {'\0'};
	char modelPath[256] = {'\0'};
	int modelPathLen = 0;
	char* foundPath = NULL;
	char* endOfModelPath = NULL;
	char delimit = NULL;
	char* result = NULL;
	struct aiScene *scene = NULL;
	struct aiMaterial *material = NULL;
	pData data = (pData) dataRef;
	int i = 0;
	int modelId = 0;
	int textureId = 0;
	pNPassimp assimp = (pNPassimp)data->io.assimp;

	if( (assimp->running == false) || (assimp == NULL) ) 
		return 0;

//	(*geolistIndexMatch) = 0;
	filename[0] = '\0';
	npGetFileNameFromPath(filePath, &filename[0], dataRef);
//	endOfModelPath = strstr(filePath, filename);
//	end
	
	delimit = nposGetFolderDelimit();

//	result = strstr(filePath, &delimit);
	result = strstr(filePath, "\\");
	if( result != NULL)	
		strcpy(modelPath, filePath);
	else
	{ /// Not a filePath, has no delimiters
	  
	}

	modelPathLen = strlen(modelPath);
	while(modelPath[modelPathLen] != '\\' && modelPathLen >= 0)
		modelPathLen--;

//	while(modelPath[modelPathLen] != '\\' && modelPathLen-- >= 0);

	modelPath[modelPathLen+1] = '\0';

	if(filename[0] == '\0') /// Nothing more can be done, return
		return 0;

#define kNPgeoLenMax 2000
#define kNPgeoAssimpIndex 1000
		/// search the geolist for this model file

	if(modelPath[0] == '\0')
	{
		printf("stop");
	}



//		for(i = kNPgeoAssimpIndex; i < kNPgeoLenMax; i++)
//		{
//			if( strcmp(data->io.gl.geolist[i].modelFile, filename) == 0 ) /// Found it
//			{
//				if( strcmp(data->io.gl.geolist[i].modelPath, modelPath) == 0)
//				{
//					(*geolistIndexMatch) = i;
//					break;
//				}
//			}
//		}



	///Check if search failed
	if(i == kNPgeoLenMax) // no match made
	{
		printf("\nModel File is not in the geolist");
//			system("pause");
		if(data->io.gl.geoLen <= kNPgeoLenMax)
		{				
//			npModelAddToGeolist(filename, geometryId, dataRef);
		}

		/// @todo : 
		return 0;
	}

	modelId = data->io.gl.geolist[i].modelId;
//	modelId = 1;
	if(modelId <= 0 || modelId > 1001)
	{
		printf("Model ID 0 is reserved, cannot use\n");
		printf("Valid Model ID range : [1,1001]\n");
		return modelId;
	}
	else
	{
//		printf("Successful");
	}

//	modelId = (*geolistIndexMatch)-999;

	if(filePath[0] != '\0') // change filePath to modelPath
	{
		/// Search for the model file
		npGetFileNameFromPath(filePath, &filename[0], dataRef);
		foundPath = npSearchPathsForFile(filename, dataRef);
		if(foundPath[0] != '\0')
		{
			/// lv, use the modelId as index into scene array
			//assimp->scene[++(assimp->numOfScenes)] = npModelImport(foundPath, dataRef); 
			if(assimp->scene[modelId] == NULL)
			{
			assimp->scene[modelId] = npModelImport(foundPath, dataRef); 
			if( assimp->scene[modelId] != NULL)
				assimp->numOfScenes++;

			if( data->io.gl.geolist[npGeolistGetX(dataRef)].modelPath[0] == '\0')
				strcpy(data->io.gl.geolist[npGeolistGetX(dataRef)].modelPath, foundPath);

			}
			else
			{
				foundPath[0] = '\0';
				free(foundPath);
				printf("\nmodel already loaded");
				return modelId;
			}

			foundPath[0] = '\0';
			free(foundPath);
		}
	}
	else ///filepath is empty 
		return 0;

	//if( assimp->scene[assimp->numOfScenes] != NULL )
	if( assimp->scene[modelId] != NULL )
	{
		scene = assimp->scene[modelId]; /// assimp->scene[0] is dummy value (NULL) 
		material = scene->mMaterials[0];
		
		assimp->path.data[0] = '\0';
		assimp->path = npAssimpGetTexturePath(modelId, dataRef);
		/// @todo : check if assimp->path is valid,
		// assimp->path is often a path on another system

		/// check for empty path
		if(assimp->path.data[0] == '\0')
		{
			/// no texture references, get out of here
			return modelId;
		}


//		if(data->io.gl.geolist[i].modelTextureFile

		/// Search for the texture file path
		npGetFileNameFromPath(assimp->path.data, &filename[0], dataRef);
		foundPath = npSearchPathsForFile(filename, dataRef);
		data->io.gl.geolist[i].modelTexturePath[0] = '\0';
		if(foundPath[0] != '\0' )
		{
			strcpy(data->io.gl.geolist[i].modelTexturePath, foundPath);
			foundPath[0] = '\0';
			free(foundPath);
		}

	}
	else
	{
		// model failed to load, decrement numOfScenes
		assimp->numOfScenes--;
		printf("\nModel failed to load : %s", aiGetErrorString());
		return 0;
	}

	return modelId;
}

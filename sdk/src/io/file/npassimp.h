#ifndef NPASSIMP_H_
#define NPASSIMP_H_


#include "../../npdata.h"
#include "../../os/npos.h"
#include <assimp/cimport.h>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#define maxScenes 1001
struct NPassimp {
 bool running; //! flag
 struct aiString path; //! texture path, access with assimp->path.data
// char filepath[200];
 struct aiScene *scene[maxScenes];
 int numOfScenes;
 struct aiMesh *mesh[500];
 int numOfMeshes;
};
typedef struct NPassimp NPassimp;
typedef NPassimp* pNPassimp;


struct aiString npAssimpGetTexturePath(int sceneIndex, void* dataRef);
int npAssimpLoadModel(char* filePath, int* geolistIndexMatch, void* dataRef);
void npAssimpInit(void* dataRef);

#endif

/* -----------------------------------------------------------------------------
*
*  npjson.h
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

#ifndef NPJSON_H_
#define NPJSON_H_

#include <jansson.h>	

#include "../../data/nptypes.h"

#ifdef NP_PLUGIN_CURL
	#include "../net/npgithub.h" /// @todo temp
#endif

/*
struct NPjson {
    json_t *root;
    json_t *data, *url, *commit, *message;
    json_t *id;
    json_t *number;
    json_error_t* error;
};
typedef struct NPjson NPjson;
typedef NPjson* pNPjson;
*/

//------------------------------------------------------------------------------
void npInitJSON (new2_pNPjson json, void* dataRef);
void npCloseJSON (void* dataRef);


void npJSON_loads(pNPjson json ,char* input, void* dataRef);
void new_npJSON_loads(new2_pNPjson json, char* input, void* dataRef);
void npJSONprocessArray(new2_pNPjson json, json_t* jArray, void* dataRef);
void npJSON_arrayExtend(pNPjson json, json_t* other, void* dataRef);
void npJSONprocess(new2_pNPjson json, json_t* toProcess, void* dataRef);
pNPjsonObject npJSONprocessObject(new2_pNPjson json, json_t* toProcess, void* dataRef);
int npJSONunpackObject(new2_pNPjson json, pNPjsonObject jsonObject, json_t* jObject_t, void* dataRef);
int npjsonSetInput(new2_pNPjson json, char* input, int index, void* dataRef);
int npjsonLoadInput(new2_pNPjson json, void* dataRef);
int npjsonUnpackRoot(new2_pNPjson json, void* dataRef);
pNPjsonArray npjsonArrayInit(void* parent, void* dataRef);
int npJSONgetObjectKeyIndex(pNPjsonObject object, char* key, void* dataRef);
pNPjsonObject npjsonObjectInit(void* parent, int parentType, void* dataRef);
int npJSONunpackArray(new2_pNPjson json, pNPjsonArray jsonArray, json_t* jArray_t, void* dataRef);

#endif


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

#include "jansson.h"	
#include "../net/npgithub.h" /// @todo temp
#include "../../data/nptypes.h"


//------------------------------------------------------------------------------
void npInitJSON (pNPjson json, void* dataRef);
void npCloseJSON (void* dataRef);


//void npJSON_loads(pNPjson json ,char* input, void* dataRef);
void new_npJSON_loads(pNPjson json, char* input, void* dataRef);
void npJSONprocessArray(pNPjson json, json_t* jArray, void* dataRef);
//void npJSON_arrayExtend(pNPjson json, json_t* other, void* dataRef);
void npJSONprocess(pNPjson json, json_t* toProcess, void* dataRef);
pNPjsonObject npJSONprocessObject(pNPjson json, json_t* toProcess, void* dataRef);
int npJSONunpackObject(pNPjson json, pNPjsonObject jsonObject, json_t* jObject_t, void* dataRef);
int npjsonSetInput(pNPjson json, char* input, int index, void* dataRef);
int npjsonLoadInput(pNPjson json, void* dataRef);
int npjsonUnpackRoot(pNPjson json, void* dataRef);
pNPjsonArray npjsonArrayInit(void* parent, void* dataRef);
int npJSONgetObjectKeyIndex(pNPjsonObject object, char* key, void* dataRef);
pNPjsonObject npjsonObjectInit(void* parent, int parentType, void* dataRef);
int npJSONunpackArray(pNPjson json, pNPjsonArray jsonArray, json_t* jArray_t, void* dataRef);
void* npJSONgetObjectC(pNPjsonObject object, char* key, int* type, void* dataRef);
#endif


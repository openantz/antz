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
void npInitJSON (pNPjson json, void* dataRef);
void npCloseJSON (void* dataRef);


void npJSON_loads(pNPjson json ,char* input, void* dataRef);
void npJSON_arrayExtend(pNPjson json, json_t* other, void* dataRef);


#endif


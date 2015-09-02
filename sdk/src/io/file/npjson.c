/* -----------------------------------------------------------------------------
*
*  npjson.c
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

#include "npjson.h"


//-----------------------------------------------------------------------------
void npInitJSON (pNPjson json, void* dataRef)
{
	json->commit  = NULL;
	json->data    = NULL;
	json->error   = NULL;
	json->id      = NULL;
	json->message = NULL;
	json->number  = NULL;
	json->root    = NULL;
	json->url     = NULL;

	return;
}

//-----------------------------------------------------------------------------
void npCloseJSON (void* dataRef)
{
	return;
}

void npJSON_loads(pNPjson json, char* input, void* dataRef){
	json->root = json_loads(input, 0, json->error);
	if(json->root == NULL)
	{
		printf("\nnpJSON_loads returned NULL");
		json->array_size = 0;
	}

	json->array_size = json_array_size(json->root);

}

void npJSON_arrayExtend(pNPjson json, json_t* other, void* dataRef){
	int error = -1;
	error = json_array_extend(json->root, other);
	if(error == -1)
	{
		printf("\njson_array_extend returned an error");
	}
}

void npJSONloadCURLdata(pNPcurl curl, pNPjson json, void* dataRef)
{
	pData data = (pData) dataRef;
	json_t* other;
	int index;

	npJSON_loads(&data->io.json, data->io.curl.mem[0].memory, dataRef);
	
	for(index = 2; index < 3; index++)
	{
		other = json_loads(data->io.curl.mem[index-1].memory, 0, NULL); 
		npJSON_arrayExtend(&data->io.json.root, other, dataRef);
		data->io.json.array_size = json_array_size(data->io.json.root);
	}
}

/*
int npCURLgetJSONdata(pNPcurl curl, pNPjson json, void* dataRef)
{
	pData data = (pData) dataRef;
	json_t* other;
	int index;

	npJSON_loads(&data->io.json, data->io.curl.mem[0].memory, dataRef);
	
	for(index = 2; index < 3; index++)
	{
		other = json_loads(data->io.curl.mem[index-1].memory, 0, NULL); 
		npJSON_arrayExtend(&data->io.json.root, other, dataRef);
		data->io.json.array_size = json_array_size(data->io.json.root);
	}
}
*/
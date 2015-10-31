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
#include <assert.h>

//-----------------------------------------------------------------------------
void npInitJSON (new2_pNPjson json, void* dataRef)
{
	int index = 0;

	for(index = 0; index < 30; index++){
		json->input[index] = NULL;
	}

	json->input_current = NULL;
	json->input_index = -1;
	json->latest = NULL;
	json->root   = NULL;

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

int npjsonArrayExtend(new2_pNPjson json, json_t* to_extend, void* dataRef)
{
	int err = -1;
	err = json_array_extend(json->jRoot.root, to_extend); 
	return err; 
}

void npJSON_arrayExtend(pNPjson json, json_t* other, void* dataRef){
	int error = -1;
	error = json_array_extend(json->root, other);
	if(error == -1)
		printf("\njson_array_extend returned an error");

}

void npJSONcustom(new2_pNPjson json, pNPjsonArray jsonArray, json_t* jArray_t, void* dataRef)
{
	
}

#define kNPnothing 0

enum {
	kNPjsonNull = 0,
	kNPjsonObject = 1,
	kNPjsonArray = 2
};

pNPjsonObject npjsonObjectInit(void* parent, int parentType, void* dataRef)
{
	pNPjsonObject object = NULL;	
	int index;

	object = malloc(sizeof(NPjsonObject));
	if(object == NULL)
	{
		return NULL;
	}

	object->numNameValuePairs = 0;
	object->parent = NULL;
	object->parentType = kNPnothing;
	
	if(parent != NULL)
	{
		object->parent = parent;	
		object->parentType = kNPnothing;	
	}

	
	for(index = 0; index < kNPjsonMaxKeyValuePairs; index++)
	{
		object->jsonKey[index].key = NULL;
		object->jsonValue[index].c_type  = -1;
		object->jsonValue[index].c_value = NULL;
		object->jsonValue[index].j_type  = -1;
		object->jsonValue[index].j_value = NULL;
	}

	return object;
}

pNPjsonArray npjsonArrayInit(void* parent, void* dataRef)
{
	pNPjsonArray jsonArray = NULL;
	pNPjsonValue element = NULL;
//	int index;
	jsonArray = malloc(sizeof(NPjsonArray));

//	jsonArray->
	jsonArray->numElements = 0;
	jsonArray->parent = parent; 

	return jsonArray;
}


#define kNPmallocFailed -1
#define kNPjsonArraySizeZero -10
#define kNParrayNull -11
#define kNPjsonArrayIndexOutOfRange -12


int npJSONunpackArray(new2_pNPjson json, pNPjsonArray jsonArray, json_t* jArray_t, void* dataRef)
{
	int arraySize = 0;
	int index = 0;
	int err = 0;
	json_t* element = NULL;

	if(jArray_t == NULL)
	{
		return kNParrayNull;
	}

	arraySize = json_array_size(jArray_t);
	jsonArray->numElements = arraySize;
	if(arraySize == 0)
		return kNPjsonArraySizeZero;

	while(index < arraySize) /// @todo reference jsonArray->numElements instead
	{
		jsonArray->element[index] = malloc(sizeof(NPjsonValue));
		if(jsonArray->element[index] == NULL)
		{
			printf("\nnpJSONunpackArray :: malloc failed");
			return kNPmallocFailed;
		}

		jsonArray->element[index]->c_type = -1;
		jsonArray->element[index]->c_value = NULL;
		jsonArray->element[index]->j_type = -1;
		jsonArray->element[index]->j_value = NULL;

		jsonArray->element[index]->j_value = json_array_get(jArray_t, index);
		if( jsonArray->element[index]->j_value == NULL )
		{
			return kNPjsonArrayIndexOutOfRange;
		}

		switch( json_typeof(jsonArray->element[index]->j_value) )
		{
			case JSON_ARRAY:
				jsonArray->element[index]->c_value = npjsonArrayInit(jsonArray, dataRef);
				if( jsonArray->element[index]->c_value == NULL )
				{
					printf("\nnpJSONunpackArray :: malloc failed");
					return kNPmallocFailed;
				}

				err = npJSONunpackArray(json, jsonArray->element[index]->c_value, jsonArray->element[index]->j_value, dataRef);
				if(err != 0)
				{
				//	printf("\nnpjsonUnpackArray returned error : %d", err);
					getchar();
				}

				((pNPjsonArray)jsonArray->element[index]->c_type)->parent = jsonArray;	
				break;
			case JSON_OBJECT:
				jsonArray->element[index]->c_value = npjsonObjectInit(jsonArray, kNPjsonArray, dataRef);
				if(jsonArray->element[index]->c_value == NULL)
				{
					printf("\nnpJSONunpackArray :: malloc failed");
					return kNPmallocFailed;
				}
				err = -1; /// @todo what is this about?

				err = npJSONunpackObject(json, jsonArray->element[index]->c_value, jsonArray->element[index]->j_value, dataRef);
				if(err != 0)
				{
					printf("\nnpJSONunpackObject Failed");
					getchar();
				}

				break;
			case JSON_STRING:
				jsonArray->element[index]->c_value = (char*)json_string_value(jsonArray->element[index]->j_value);
				jsonArray->element[index]->j_type = JSON_STRING;
				break;
			case JSON_INTEGER:
				jsonArray->element[index]->c_value = malloc(sizeof(NPjsonInteger));
				if(jsonArray->element[index]->c_value == NULL)
				{
					printf("\nnpJSONunpackArray :: malloc failed");
					return kNPmallocFailed ;
				}

				((pNPjsonInteger)jsonArray->element[index]->c_value)->j_int = (int)json_integer_value(jsonArray->element[index]->j_value); 
				break;
			case JSON_REAL:
				jsonArray->element[index]->c_value = malloc(sizeof(NPjsonReal));
				if(jsonArray->element[index]->c_value == NULL)
				{
					printf("\nnpJSONunpackArray :: malloc failed");
					return kNPmallocFailed;
				}

				((pNPjsonReal)jsonArray->element[index]->c_value)->j_real = json_real_value(jsonArray->element[index]->j_value);	
				break;
			case JSON_TRUE:
				jsonArray->element[index]->c_value = malloc(sizeof(NPjsonBoolean));
				if(jsonArray->element[index]->c_value == NULL)
				{
					printf("\nnpJSONunpackArray :: malloc failed");
					return kNPmallocFailed;
				}

				((pNPjsonBoolean)jsonArray->element[index]->c_value)->j_bool = json_boolean_value(jsonArray->element[index]->j_value);	
				break;
			case JSON_FALSE:
				jsonArray->element[index]->c_value = malloc(sizeof(NPjsonBoolean));
				if(jsonArray->element[index]->c_value == NULL)
				{
					printf("\nnpJSONunpackArray :: malloc failed");
					return kNPmallocFailed;
				}

				((pNPjsonBoolean)jsonArray->element[index]->c_value)->j_bool = json_boolean_value(jsonArray->element[index]->j_value);	
				break;
			case JSON_NULL: // JSON_NULL processing
				jsonArray->element[index]->c_value = NULL;
				jsonArray->element[index]->j_type = JSON_NULL;
				jsonArray->element[index]->j_value = NULL;
				break;
			default:
				printf("Error: Impossible Case");
				return -1;
				break;
		}

		index++;
	}

	return 0;
}

/* /// @todo : npjsonObjectAdd
void npjsonObjectAdd(pNPjsonObject jsonObject, json_t* jObject_t , void* dataRef)
{
	int index = 0;
	char* key = NULL;
	json_t* value = NULL;

	assert(jsonObject != NULL);
	assert(jObject_t != NULL);
	assert(dataRef != NULL);

}
*/

int npJSONunpackObject(new2_pNPjson json, pNPjsonObject jsonObject, json_t* jObject_t, void* dataRef)
{
	int index = 0;
	char* key = NULL;
	json_t* value = NULL;
	int err = 0;
	
	if(json == NULL || jsonObject == NULL || jObject_t == NULL)
	{
		printf("error : NULL ptr passed as argument");
		getchar();
	}

	json_object_foreach(jObject_t, (const char*)key, value) {
		jsonObject->numNameValuePairs++;
		jsonObject->jsonKey[index].key = key;				
		jsonObject->jsonValue[index].j_value = value;
		jsonObject->jsonValue[index].j_type = json_typeof(value);

		switch(jsonObject->jsonValue[index].j_type)
		{
			case JSON_OBJECT:
				jsonObject->jsonValue[index].c_value = npjsonObjectInit(NULL, kNPjsonNull, dataRef); 
				if(jsonObject->jsonValue[index].c_value == NULL)
				{
					printf("\nnpJSONunpackObject :: malloc failed");
					return -1;
				}

				npJSONunpackObject(json, jsonObject->jsonValue[index].c_value, jsonObject->jsonValue[index].j_value, dataRef);
				break;
			case JSON_ARRAY:
				jsonObject->jsonValue[index].c_value = npjsonArrayInit(NULL, dataRef);
				if(jsonObject->jsonValue[index].c_value == NULL)
				{
					printf("\nnpJSONunpackObject :: malloc failed");
					return -1;
				}

				err = npJSONunpackArray(json, jsonObject->jsonValue[index].c_value, jsonObject->jsonValue[index].j_value, dataRef);
				if(err != 0)
				{
					//printf("\nnpjsonUnpackArray returned error : %d", err);
				}
				break;
			case JSON_INTEGER:
				jsonObject->jsonValue[index].c_value = malloc(sizeof(NPjsonInteger)); 
				if(jsonObject->jsonValue[index].c_value == NULL)
				{
					printf("\nnpJSONunpackObject :: malloc failed");
					return -1;
				}

				((pNPjsonInteger)jsonObject->jsonValue[index].c_value)->j_int = (int)json_integer_value(jsonObject->jsonValue[index].j_value);
				jsonObject->jsonValue[index].j_type = JSON_INTEGER;
				break;
			case JSON_STRING:
				jsonObject->jsonValue[index].c_value = (char*)json_string_value(jsonObject->jsonValue[index].j_value);	
				if(jsonObject->jsonValue[index].c_value == NULL)
				{
					printf("\nnpJSONunpackObject :: malloc failed");
					return -1;
				}

				jsonObject->jsonValue[index].j_type = JSON_STRING;
				break;
			case JSON_REAL:
				printf("\nJSON_REAL not yet supported");
				return -1; // remove this when supported
				jsonObject->jsonValue[index].c_value = malloc(sizeof(NPjsonReal)); 
				if(jsonObject->jsonValue[index].c_value == NULL)
				{
					printf("\nnpJSONunpackObject :: malloc failed");
					return -1;
				}

//				((pNPjsonReal)jsonObject->jsonValue[index].c_value)->j_real = json_integer_value(jsonObject->jsonValue[index].j_value);
//				jsonObject->jsonValue[index].c_value = json_real_value(jsonObject->jsonValue[index].j_value);	
				break;
			case JSON_TRUE:
				jsonObject->jsonValue[index].c_value = malloc(sizeof(NPjsonBoolean));
				if(jsonObject->jsonValue[index].c_value == NULL)
				{
					printf("\nnpJSONunpackObject :: malloc failed");
					return -1;
				}

				((pNPjsonBoolean)jsonObject->jsonValue[index].c_value)->j_bool = json_boolean_value(jsonObject->jsonValue[index].j_value);
				break;
			case JSON_FALSE:
				jsonObject->jsonValue[index].c_value = malloc(sizeof(NPjsonBoolean));
				if(jsonObject->jsonValue[index].c_value == NULL)
				{
					printf("\nnpJSONunpackObject :: malloc failed");
					return -1;
				}
				
				((pNPjsonBoolean)jsonObject->jsonValue[index].c_value)->j_bool = json_boolean_value(jsonObject->jsonValue[index].j_value);
				break;
			case JSON_NULL:
				jsonObject->jsonValue[index].c_value = NULL;
				break;
		}
		
		index++;
	}

	return 0;
}

int npjsonClearInput(new2_pNPjson json, int index, void* dataRef)
{
	char* input = json->input[index];
	
	input = strset(input, '\0');

	return 0;
}

int npjsonSetInput(new2_pNPjson json, char* input, int index, void* dataRef)
{
	json->input[index] = input;
	json->input_index = index;
	json->input_current = input;
	return 0;
}

int npjsonLoadInput(new2_pNPjson json, void* dataRef)
{
	pData data = (pData) dataRef;
	json_t* to_extend = NULL;
	int error = 0;

	if(json->input_index > 0 && json->jRoot.root != NULL)
	{
		to_extend = json_loads(json->input[json->input_index], 0, &json->error);	
		error = npjsonArrayExtend(json, to_extend, dataRef);
	}
	else
	{
		json->jRoot.root = json_loads(json->input[json->input_index], 0, &json->error);
	}

	if(json->jRoot.root == NULL)
	{
//		printf("\njson_loads failed");
		return -1;
	}

	return 0;
}


int npjsonUnpackRoot(new2_pNPjson json, void* dataRef)
{
	int err = 0;

	switch( json_typeof(json->jRoot.root) )
	{
		case JSON_OBJECT:
			json->jRoot.jsonRoot = npjsonObjectInit(NULL, kNPjsonNull, dataRef);
			if(json->jRoot.jsonRoot == NULL)
			{
				printf("\nnpjsonUnpackRoot :: malloc failed");
				return -1;
			}

			npJSONunpackObject(json, json->jRoot.jsonRoot, json->jRoot.root, dataRef);
		//	((pNPjsonObject)json->jRoot.jsonRoot)->parent = NULL; // is root, has no parent.
			break;
		case JSON_ARRAY:
			json->jRoot.jsonRoot = npjsonArrayInit(NULL, dataRef);
			if(json->jRoot.jsonRoot == NULL)
			{
				printf("\nnpjsonUnpackRoot :: malloc failed");
				return -1;
			}

			err = npJSONunpackArray(json, json->jRoot.jsonRoot, json->jRoot.root, dataRef);
			if(err != 0)
			{
				//printf("\nnpjsonUnpackArray returned error : %d", err);
			//	getchar();
			}

			json->jRoot.type = JSON_ARRAY;
//			((pNPjsonArray)json->jRoot.jsonRoot)->parent = NULL; // is root, has no parent.
			break;
	}
	
	return 0;
}

void* npJSONgetObjectC(pNPjsonObject object, char* key, int* type, void* dataRef)
{
	int index = 0;

	if(key == NULL)
		return NULL;

	index = npJSONgetObjectKeyIndex(object, key, dataRef);
	if(index == -1 || index >= 30) /// 30 is max object size
		return NULL;

	(*type) = object->jsonValue[index].j_type;

	return object->jsonValue[index].c_value;
}

int npJSONgetObjectKeyIndex(pNPjsonObject object, char* key, void* dataRef)
{
	int index = 0;

	if(key == NULL)
		return -1;

	if(object == NULL)
	{
	//	printf(" (key : %s)", key);
		return -1;
	}

	if(object->numNameValuePairs >= 30) /// @todo add define kNPmaxNumNameValuePairs
	{
		printf("\nnumNameValuePairs exceeds max");
		return -10;
	}

	for(index = 0; index < object->numNameValuePairs; index++)
	{
		if( strcmp(object->jsonKey[index].key, key) == 0)
		{ /// Found it
			return index;
		}
	}	

	/// Didn't find it
	return -1;
}

json_t* npjsonGetRoot(new2_pNPjson json, void* dataRef)
{
	return json->jRoot.root;
}

void new_npJSON_loads(new2_pNPjson json, char* input, void* dataRef)
{
	pData data = (pData) dataRef;
	int index = 0;
	json->jRoot.root = json_loads(input, 0, &json->error);
	if(json->jRoot.root == NULL)
	{
		printf("\nnpJSON_loads returned NULL");
		return;
	}
	
	json->jRoot.type = json_typeof(json->jRoot.root);

	switch( json_typeof(json->jRoot.root) )
	{
		case JSON_OBJECT:
			json->jRoot.jsonRoot = malloc(sizeof(NPjsonObject));
			npJSONunpackObject(json, json->jRoot.jsonRoot, json->jRoot.root, dataRef);
			((pNPjsonObject)json->jRoot.jsonRoot)->parent = NULL; // is root, has no parent.
			break;
		case JSON_ARRAY:
			json->jRoot.jsonRoot = malloc(sizeof(NPjsonArray));
			npJSONunpackArray(json, json->jRoot.jsonRoot, json->jRoot.root, dataRef);
			((pNPjsonArray)json->jRoot.jsonRoot)->parent = NULL; // is root, has no parent.
			break;
		case JSON_STRING:
			break;
		case JSON_INTEGER:
			break;
		case JSON_REAL:
			break;
		case JSON_TRUE:
			break;
		case JSON_FALSE:
			break;
		case JSON_NULL:
			break;
	}

	if(json->jRoot.type == JSON_ARRAY)
	{
		data->io.github.issues->count = ((pNPjsonArray)json->jRoot.jsonRoot)->numElements;	
		for(index = 0; index < data->io.github.issues->count; index++)
		{
			npGithubGetIssue(json, data->io.github.issues, index, dataRef);
		}
	}
}
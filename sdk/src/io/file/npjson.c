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
//	json->error = 0; // temp

	for(index = 0; index < 30; index++){
		json->input[index] = NULL;
	}

	json->input_current = NULL;
	json->input_index = -1;
	json->latest = NULL;
	json->root   = NULL;

	/*
	json->commit  = NULL;
	json->data    = NULL;
	json->error   = NULL;
	json->id      = NULL;
	json->message = NULL;
	json->number  = NULL;
	json->root    = NULL;
	json->url     = NULL;
	*/
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

	/*
	printf("\nnpjsonArrayExtend");
	printf("\njson->jRoot.root : %p", json->jRoot.root);
	printf("\nto_extend : %p", to_extend);
	*/
	err = json_array_extend(json->jRoot.root, to_extend); 
	//printf(" : %d\n", json->error);
	return err; 
}

void npJSON_arrayExtend(pNPjson json, json_t* other, void* dataRef){
	int error = -1;
	error = json_array_extend(json->root, other);
	if(error == -1)
	{
		printf("\njson_array_extend returned an error");
	}
}

/*
void npJSONloadCURLdata(pNPcurl curl, pNPjson json, void* dataRef)
{
	pData data = (pData) dataRef;
	json_t* other;
	int index;

//	npJSON_loads(&data->io.json, data->io.curl.mem[0].memory, dataRef);
	printf("\nbefore new_npJSON_loads");
	new_npJSON_loads(&data->io.json2, data->io.curl.mem[0].memory, dataRef);
	printf("\nafter new_npJSON_loads");
	
	

	for(index = 2; index < 3; index++)
	{
		other = json_loads(data->io.curl.mem[index-1].memory, 0, NULL); 
		npJSON_arrayExtend(&data->io.json.root, other, dataRef);
		data->io.json.array_size = json_array_size(data->io.json.root);
	}	
}
*/

/*
pNPjsonObject npJSONprocessObject(new2_pNPjson json, json_t* toProcess, void* dataRef)
{
	void *current;
	char* key;
	json_t* value;
	int index = 0;
	pNPjsonObject jsonObject = malloc(sizeof(NPjsonObject));

	switch( json->all_values.type )
	{
		case JSON_OBJECT:
			break;
		case JSON_ARRAY:
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

	printf("\nnpJSONprocessObject");
	json_object_foreach(toProcess, key, value) {
//		printf("\nKey : %s , typeof : %d", key, json_typeof(value));
		jsonObject->jsonKey[index].key = key;	
//		jsonObject->jsonValue[index].value
		printf("\nKey : %s , typeof : %d", jsonObject->jsonKey[index].key , json_typeof(value));
		index++;
		npJSONprocess(json, value, dataRef);
	}

	return jsonObject;

}
*/

/*
void npJSONprocess(new2_pNPjson json, json_t* toProcess, void* dataRef)
{		
	void* obj = NULL;

//	switch( json_typeof(json->root) )
	switch( json_typeof(toProcess) )
	{
		case JSON_OBJECT:
			obj = npJSONprocessObject(json, toProcess, dataRef);
			printf("\nJSON_OBJECT");
			break;	
		case JSON_ARRAY:
			printf("\nJSON_ARRAY");
			npJSONprocessArray(json, toProcess, dataRef);
			break;
		case JSON_STRING:
			printf("\nJSON_STRING");
			break;
		case JSON_INTEGER:
			printf("\nJSON_INTEGER");
			break;
		case JSON_REAL:
			printf("\nJSON_REAL");
			break;
		case JSON_TRUE:
			printf("\nJSON_TRUE");
			break;
		case JSON_FALSE:
			printf("\nJSON_FALSE");
			break;
	}

}
*/



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
//	int arraySize = json_array_size(jArray_t);
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
	{
		return kNPjsonArraySizeZero;
	}


	while(index < arraySize) /// reference jsonArray->numElements instead
	{
//		printf("\nINDEX : %d", index);
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

//		element = jsonArray->element[index];
		switch( json_typeof(jsonArray->element[index]->j_value) )
		{
			case JSON_ARRAY:
//				printf("\nJSON_ARRAY_1");
//				jsonArray->element[index]->c_value = malloc(sizeof(NPjsonArray));
				jsonArray->element[index]->c_value = npjsonArrayInit(jsonArray, dataRef);
				if( jsonArray->element[index]->c_value == NULL )
				{
					printf("\nnpJSONunpackArray :: malloc failed");
					return kNPmallocFailed;
				}

				err = npJSONunpackArray(json, jsonArray->element[index]->c_value, jsonArray->element[index]->j_value, dataRef);
				if(err != 0)
				{
					printf("\nnpjsonUnpackArray returned error : %d", err);
				//	getchar();
				}

				((pNPjsonArray)jsonArray->element[index]->c_type)->parent = jsonArray;	
				////getchar();
				break;
			case JSON_OBJECT:
//				printf("\nJSON_OBJECT");
				jsonArray->element[index]->c_value = npjsonObjectInit(jsonArray, kNPjsonArray, dataRef);
				if(jsonArray->element[index]->c_value == NULL)
				{
					printf("\nnpJSONunpackArray :: malloc failed");
					return kNPmallocFailed;
				}
				err = -1;

//				printf("\nBefore npJSONunpackObject");
				err = npJSONunpackObject(json, jsonArray->element[index]->c_value, jsonArray->element[index]->j_value, dataRef);
				if(err != 0)
				{
					printf("\nnpJSONunpackObject Failed");
					getchar();
				}

//				printf("\nAfter npJSONunpackObject");
//				((pNPjsonArray)jsonArray->element[index]->c_value)->parent = jsonArray;	
//				printf("\nasdf");
				////getchar();
				break;
			case JSON_STRING:
//				printf("\nJSON_STRING npJSONunpackArray");
				//jsonObject->jsonValue[index].c_value = json_string_value(jsonObject->jsonValue[index].j_value);	
				jsonArray->element[index]->c_value = (char*)json_string_value(jsonArray->element[index]->j_value);
				jsonArray->element[index]->j_type = JSON_STRING;
				//getchar();
				break;
			case JSON_INTEGER:
//				printf("\nJSON_INTEGER npJSONunpackArray");
				jsonArray->element[index]->c_value = malloc(sizeof(NPjsonInteger));
				if(jsonArray->element[index]->c_value == NULL)
				{
					printf("\nnpJSONunpackArray :: malloc failed");
					return kNPmallocFailed ;
				}

				((pNPjsonInteger)jsonArray->element[index]->c_value)->j_int = (int)json_integer_value(jsonArray->element[index]->j_value); 
				//jsonObject->jsonValue[index].c_value = json_integer_value(jsonObject->jsonValue[index].j_value);
//				jsonArray->element[index]->c_value = 
				//getchar();
				break;
			case JSON_REAL:
				jsonArray->element[index]->c_value = malloc(sizeof(NPjsonReal));
				if(jsonArray->element[index]->c_value == NULL)
				{
					printf("\nnpJSONunpackArray :: malloc failed");
					return kNPmallocFailed;
				}

//				printf("\nJSON_REAL npJSONunpackArray");
				((pNPjsonReal)jsonArray->element[index]->c_value)->j_real = json_real_value(jsonArray->element[index]->j_value);	
				//getchar();
				break;
			case JSON_TRUE:
//				printf("\nJSON_TRUE npJSONunpackArray");
				jsonArray->element[index]->c_value = malloc(sizeof(NPjsonBoolean));
				if(jsonArray->element[index]->c_value == NULL)
				{
					printf("\nnpJSONunpackArray :: malloc failed");
					return kNPmallocFailed;
				}

				((pNPjsonBoolean)jsonArray->element[index]->c_value)->j_bool = json_boolean_value(jsonArray->element[index]->j_value);	
				//getchar();
				break;
			case JSON_FALSE:
//				printf("\nJSON_FALSE npJSONunpackArray");
				jsonArray->element[index]->c_value = malloc(sizeof(NPjsonBoolean));
				if(jsonArray->element[index]->c_value == NULL)
				{
					printf("\nnpJSONunpackArray :: malloc failed");
					return kNPmallocFailed;
				}

				((pNPjsonBoolean)jsonArray->element[index]->c_value)->j_bool = json_boolean_value(jsonArray->element[index]->j_value);	
				//getchar();
				break;
			case JSON_NULL: /// @todo JSON_NULL processing
//				printf("\nJSON_NULL npJSONunpackArray");
				jsonArray->element[index]->c_value = NULL;
//				jsonArray->element[index]->c_value = -1;
				jsonArray->element[index]->j_type = JSON_NULL;
				jsonArray->element[index]->j_value = NULL;
				//getchar();
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
/*
void npJSONunpackInteger(new2_pNPjson json, pNPjsonObject jsonObject, json_t* jInteger_t, void* dataRef)
{
				printf("\nJSON_INTEGER npJSONunpackObject : %I64d", json_integer_value(jsonObject->jsonValue[index].j_value) );
				jsonObject->jsonValue[index].c_value = malloc(sizeof(NPjsonInteger)); 
				if(jsonObject->jsonValue[index].c_value == NULL)
				{
					printf("\nnpJSONunpackObject :: malloc failed");
					return -1;
				}

				((pNPjsonInteger)jsonObject->jsonValue[index].c_value)->j_int = json_integer_value(jsonObject->jsonValue[index].j_value);
				//getchar();
}
*/

void npjsonObjectAdd(pNPjsonObject jsonObject, json_t* jObject_t , void* dataRef)
{
	int index = 0;
	char* key = NULL;
	json_t* value = NULL;

	assert(jsonObject != NULL);
	assert(jObject_t != NULL);
	assert(dataRef != NULL);


	

}

/// @todo : void npjsonUnpackInteger(

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

	//printf("\nInitial Value Ptr : %p", value);
//   jsonObject->numNameValuePairs = 0;

//	printf("\nNew Object");
//	getchar();
	json_object_foreach(jObject_t, (const char*)key, value) {
		jsonObject->numNameValuePairs++;
//		printf("\nKey : %s & Index : %d & object size : %d", key, index, json_object_size(jObject_t));
//		printf("\njsonObject->numnameValuePairs == %d :: index == %d", jsonObject->numNameValuePairs, index);	
	//	getchar();
		jsonObject->jsonKey[index].key = key;				
//		printf("\nValue Ptr : %p", value);
	
		jsonObject->jsonValue[index].j_value = value;
		jsonObject->jsonValue[index].j_type = json_typeof(value);
		switch(jsonObject->jsonValue[index].j_type)
		{
			case JSON_OBJECT:
//				printf("\nJSON_OBJECT_2 npJSONunpackObject");
				jsonObject->jsonValue[index].c_value = npjsonObjectInit(NULL, kNPjsonNull, dataRef); 
				if(jsonObject->jsonValue[index].c_value == NULL)
				{
					printf("\nnpJSONunpackObject :: malloc failed");
					return -1;
				}

				npJSONunpackObject(json, jsonObject->jsonValue[index].c_value, jsonObject->jsonValue[index].j_value, dataRef);
				//////getchar();
				
				break;
			case JSON_ARRAY:
//				printf("\nJSON_ARRAY_2 npJSONunpackObject");
				jsonObject->jsonValue[index].c_value = npjsonArrayInit(NULL, dataRef);
				if(jsonObject->jsonValue[index].c_value == NULL)
				{
					printf("\nnpJSONunpackObject :: malloc failed");
					return -1;
				}

				err = npJSONunpackArray(json, jsonObject->jsonValue[index].c_value, jsonObject->jsonValue[index].j_value, dataRef);
				if(err != 0)
				{
					printf("\nnpjsonUnpackArray returned error : %d", err);
				//	getchar();
				}
				////getchar();
				break;
			case JSON_INTEGER:
//				printf("\nJSON_INTEGER npJSONunpackObject : %I64d", json_integer_value(jsonObject->jsonValue[index].j_value) );
				jsonObject->jsonValue[index].c_value = malloc(sizeof(NPjsonInteger)); 
				if(jsonObject->jsonValue[index].c_value == NULL)
				{
					printf("\nnpJSONunpackObject :: malloc failed");
					return -1;
				}

				((pNPjsonInteger)jsonObject->jsonValue[index].c_value)->j_int = (int)json_integer_value(jsonObject->jsonValue[index].j_value);
				jsonObject->jsonValue[index].j_type = JSON_INTEGER;
				//getchar();
				break;
			case JSON_STRING:
				jsonObject->jsonValue[index].c_value = (char*)json_string_value(jsonObject->jsonValue[index].j_value);	
				if(jsonObject->jsonValue[index].c_value == NULL)
				{
					printf("\nnpJSONunpackObject :: malloc failed");
					return -1;
				}

				jsonObject->jsonValue[index].j_type = JSON_STRING;
//				printf("\nJSON_STRING npJSONunpackObject : %s", jsonObject->jsonValue[index].c_value );
//				printf("\nAFTER npJSONunpackObject");
				//getchar();
				break;
			case JSON_REAL:
				printf("\nJSON_REAL not yet supported");
				return -1;
//				printf("\nJSON_REAL npJSONunpackObject");	
				jsonObject->jsonValue[index].c_value = malloc(sizeof(NPjsonReal)); 
				if(jsonObject->jsonValue[index].c_value == NULL)
				{
					printf("\nnpJSONunpackObject :: malloc failed");
					return -1;
				}

//				((pNPjsonReal)jsonObject->jsonValue[index].c_value)->j_real = json_integer_value(jsonObject->jsonValue[index].j_value);

				//getchar();
//				jsonObject->jsonValue[index].c_value = json_real_value(jsonObject->jsonValue[index].j_value);	
				break;
			case JSON_TRUE:
//				printf("\nJSON_TRUE npJSONunpackObject");
//				jsonObject->jsonValue[index].c_value = json_boolean_value(jsonObject->jsonValue[index].j_value);	
				jsonObject->jsonValue[index].c_value = malloc(sizeof(NPjsonBoolean));
				if(jsonObject->jsonValue[index].c_value == NULL)
				{
					printf("\nnpJSONunpackObject :: malloc failed");
					return -1;
				}

				((pNPjsonBoolean)jsonObject->jsonValue[index].c_value)->j_bool = json_boolean_value(jsonObject->jsonValue[index].j_value);

				/*
				jsonArray->element[index]->c_value = malloc(sizeof(NPjsonBoolean));
				((pNPjsonBoolean)jsonArray->element[index]->c_value)->j_bool = json_boolean_value(jsonArray->element[index]->j_value);	
				*/
				//getchar();
				break;
			case JSON_FALSE:
//				printf("\nJSON_FALSE npJSONunpackObject");
				jsonObject->jsonValue[index].c_value = malloc(sizeof(NPjsonBoolean));
				if(jsonObject->jsonValue[index].c_value == NULL)
				{
					printf("\nnpJSONunpackObject :: malloc failed");
					return -1;
				}
				
				((pNPjsonBoolean)jsonObject->jsonValue[index].c_value)->j_bool = json_boolean_value(jsonObject->jsonValue[index].j_value);
//				jsonObject->jsonValue[index].c_value = json_boolean_value(jsonObject->jsonValue[index].j_value);	
				//getchar();
				break;
			case JSON_NULL: /// I'm not properly parsing JSON_NULL values
//				printf("\nJSON_NULL npJSONunpackObject");
				jsonObject->jsonValue[index].c_value = NULL;
				
				//getchar();
				break;
		}
		
		index++;
	}

//	printf("Final Index : %d", index);
//	printf("\nFinal numNameValuePairs : %d", jsonObject->numNameValuePairs);
//	getchar();

	return 0;
}

/*
void npJSONprocessArray(new2_pNPjson json, json_t* jArray, void* dataRef) 
{
	int index = 0; 
//	pNPjsonArray jsonArray = json->all_values.value; 
	pNPjsonArray jsonArray = NULL; 
	json_t* temp_json = NULL;
	int temp_type = 0;

	jsonArray = malloc(sizeof(NPjsonArray));

//	json->all_values.type = JSON_ARRAY;
//	printf("\nbefore malloc");
//	json->all_values.value = malloc(sizeof(NPjsonArray));
//	printf("\nafter malloc");

//	jsonArray = (json_t*)json->all_values.value;
//	jsonArray = json->latest;

	jsonArray->numElements = json_array_size(jArray);
	printf("\njsonArray->numElements : %d\n", jsonArray->numElements);
	////getchar(); 
	
	for(index = 0; index < jsonArray->numElements; index++){
		temp_json = json_array_get(jArray, index);
		temp_type = json_typeof(temp_json);
		printf("\nIndex : %d -- temp_type : %d", index, temp_type);
		jsonArray->element[index] = malloc(sizeof(NPjsonValue));
		switch( temp_type )
		{
			case JSON_OBJECT:
				jsonArray->element[index]->value = malloc(sizeof(NPjsonObject));
				printf("\nJSON_OBJECT");
				break;	
			case JSON_ARRAY:
				jsonArray->element[index]->value = malloc(sizeof(NPjsonArray));
				printf("\nJSON_ARRAY");
				break;
			case JSON_STRING:
				printf("\nJSON_STRING");
				break;
			case JSON_INTEGER:
				printf("\nJSON_INTEGER");
				break;
			case JSON_REAL:
				printf("\nJSON_REAL");
				break;
			case JSON_TRUE:
				printf("\nJSON_TRUE");
				break;
			case JSON_FALSE:
				printf("\nJSON_FALSE");
				break;
		}

		npJSONprocess(json, temp_json, dataRef);

//		jsonArray->
//		printf("\njsonArray->element[index] : %p", jsonArray->element[index]);
//		jsonArray->element[index]->value = (json_t*)json_array_get(jArray, index);
//		jsonArray->element[index]->type = json_typeof((json_t*)jsonArray->element[index]->value);
//		npJSONprocess(json, jsonArray->element[index]->value, dataRef);
	}

//	(pNPjsonArray)json->all_values.value->numElements = 0;

}
*/

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

//	printf("\njson->input_index : %d", json->input_index);

//	printf("\ninput : %s\n", json->input[json->input_index]);

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
	
/*
	switch( json_typeof(json->jRoot.root) )
	{
		case JSON_OBJECT:
			printf("\nJSON_OBJECT");
			json->jRoot.jsonRoot = malloc(sizeof(NPjsonObject));
			npJSONunpackObject(json, json->jRoot.jsonRoot, json->jRoot.root, dataRef);
			((pNPjsonObject)json->jRoot.jsonRoot)->parent = NULL; // is root, has no parent.
			break;
		case JSON_ARRAY:
			printf("\nJSON_ARRAY");
			json->jRoot.jsonRoot = malloc(sizeof(NPjsonArray));
			npJSONunpackArray(json, json->jRoot.jsonRoot, json->jRoot.root, dataRef);
			((pNPjsonArray)json->jRoot.jsonRoot)->parent = NULL; // is root, has no parent.
			break;
	}
*/

	return 0;
}


int npjsonUnpackRoot(new2_pNPjson json, void* dataRef)
{
	int err = 0;

//	printf("\nnpjsonUnpackRoot : json->jRoot.root : "); 
//	printf(" %d", json_typeof(json->jRoot.root));
	switch( json_typeof(json->jRoot.root) )
	{
		case JSON_OBJECT:
//			printf("\nJSON_OBJECT");
//			json->jRoot.jsonRoot = malloc(sizeof(NPjsonObject));
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
//			printf("\nJSON_ARRAY");
//			json->jRoot.jsonRoot = malloc(sizeof(NPjsonArray));
			json->jRoot.jsonRoot = npjsonArrayInit(NULL, dataRef);
			if(json->jRoot.jsonRoot == NULL)
			{
				printf("\nnpjsonUnpackRoot :: malloc failed");
				return -1;
			}

			err = npJSONunpackArray(json, json->jRoot.jsonRoot, json->jRoot.root, dataRef);
			if(err != 0)
			{
				printf("\nnpjsonUnpackArray returned error : %d", err);
			//	getchar();
			}

			json->jRoot.type = JSON_ARRAY;
//			((pNPjsonArray)json->jRoot.jsonRoot)->parent = NULL; // is root, has no parent.
			break;
	}
	
	return 0;
}

/// @todo move function to npjson.c & .h
int npJSONgetObjectKeyIndex(pNPjsonObject object, char* key, void* dataRef)
{
	int index = 0;

//	printf("\nobject ptr : %p", object);
	if(key == NULL)
	{
	//	printf("\nKey is NULL");
		return -1;
	}

	if(object == NULL)
	{
	//	printf("\nobject does not exist");
	//	printf(" (key : %s)", key);
		return -1;
	}

//	printf("\nobject->numNameValuePairs : %d", object->numNameValuePairs);

	if(object->numNameValuePairs >= 30)
	{
		printf("\nnumNameValuePairs exceeds max");
		return -10;
	}

	for(index = 0; index < object->numNameValuePairs; index++)
	{
//		json_issue->jsonKey[index].key		
		if( strcmp(object->jsonKey[index].key, key) == 0)
		{
			//printf("\nFound it : %s : %s", key, object->jsonValue[index].c_value);
	//		printf("\nType is %d\n", object->jsonValue->j_type);
			return index;
		}
	}	

	printf("\n");
	/// Didn't find it
	return -1;
}

json_t* npjsonGetRoot(new2_pNPjson json, void* dataRef)
{
	return json->jRoot.root;
}

/*
int npjsonRun(new2_pNPjson json, void* dataRef)
{

}
*/

/// @todo extending json root
void new_npJSON_loads(new2_pNPjson json, char* input, void* dataRef)
{
	pData data = (pData) dataRef;
	int index = 0;
//	json->root = json_loads(input, 0, json->error);
	json->jRoot.root = json_loads(input, 0, &json->error);
	if(json->jRoot.root == NULL)
	{
		printf("\nnpJSON_loads returned NULL");
		return;
//		json->array_size = 0;
	}

//	printf("\njson->root typeof : %d\n", json_typeof(json->jRoot.root));	
	
	json->jRoot.type = json_typeof(json->jRoot.root);

	switch( json_typeof(json->jRoot.root) )
	{
		case JSON_OBJECT:
//			printf("\nJSON_OBJECT");
			json->jRoot.jsonRoot = malloc(sizeof(NPjsonObject));
			npJSONunpackObject(json, json->jRoot.jsonRoot, json->jRoot.root, dataRef);
			((pNPjsonObject)json->jRoot.jsonRoot)->parent = NULL; // is root, has no parent.
			break;
		case JSON_ARRAY:
//			printf("\nJSON_ARRAY");
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
		printf("\njson->jRoot.type : %s", "JSON_ARRAY");
//		((pNPjsonArray)json->jRoot.jsonRoot)->
	
		//getchar();
//		npGithubGetIssues(dataRef);
		printf("0 num labels : %d", data->io.github.issues->issue[0].num_of_labels);
		printf("\n1 num labels : %d", data->io.github.issues->issue[1].num_of_labels);
		printf("\njson ptr : %p", json);
//		printf("\nnum of issues : %d" ,((pNPjsonArray)json->jRoot.jsonRoot)->numElements);	
		data->io.github.issues->count = ((pNPjsonArray)json->jRoot.jsonRoot)->numElements;	
		printf("\nnum of issues : %d" , data->io.github.issues->count );	
		getchar();
		for(index = 0; index < data->io.github.issues->count; index++)
		{
			npGithubGetIssue(json, data->io.github.issues, index, dataRef);
		}

	//	theNew_npGitViz(&data->io.github, dataRef);
	}

//	npJSONprocess(json, json->root, dataRef);	

	////getchar();
	
//	json->array_size = json_array_size(json->root);

}

/*
void npJSONloadCustom(new_pNPjson json, void* dataRef)
{
	size_t index;
	json_t* value;
	json_t* objectValue;
	char* key;
	json_t* object;
	void* iter;
	
	index = 0;


	json_array_foreach(json->root, json->jsonArrayIndex, value)
	{
		json->jsonArray[json->jsonArrayIndex] = value;
		json->numOfJsonArrays = (json->jsonArrayIndex) + 1;
	}


	for(json->jsonArrayIndex = 0; json->jsonArrayIndex < json->numOfJsonArrays; json->jsonArrayIndex++)
	{
		json->currentArray = json->jsonArray[json->jsonArrayIndex];
		iter = json_object_iter(json->currentArray);
		if(iter == NULL) /// @todo report error: object is empty
			return;

		/// @todo init keyList.index to zero
		json->KeyList.keys[json->KeyList.index] = json_object_iter_key(iter);	
		json->Key->value = json_object_iter_value(iter);
	
		json->KeyList.index++;	

	}

	/// Create structure to unpack everything into and pass function a jansson format specifier string
//	for(
	
}
*/

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
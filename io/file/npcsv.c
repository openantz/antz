/* -----------------------------------------------------------------------------
*
*  npcsv.c
*
*  ANTz - realtime 3D data visualization tools for the real-world, based on NPE.
*
*  ANTz is hosted at http://openantz.com and NPE at http://neuralphysics.org
*
*  Written in 2010-2012 by Shane Saxon - contact@saxondigital.net
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
*  with this software (license file named COPYING.txt). If not, see
*  http://creativecommons.org/publicdomain/zero/1.0/
*
* --------------------------------------------------------------------------- */

#include "npcsv.h"


#include "../../os/npos.h"
#include "../npfile.h"


//-----------------------------------------------------------------------------
void npInitCSV (void* dataRef)
{
	return;
}

//-----------------------------------------------------------------------------
void npCloseCSV (void* dataRef)
{
	return;
}

//-----------------------------------------------------------------------------
// mode usage: 3 parameters "r/w/a" followed by optional "b" and optional "+"
// example mode = "rb+"
// r open file for reading
// w open file for overwriting. Create new file if doesn't already exist
// a open file for appending data. 
// b open file as a binary file
// + open file for read and writing
//-----------------------------------------------------------------------------
FILE* npOpenCSV (const char* fileName, const char* mode, void* dataRef)
{
	FILE* file = NULL;

	file = fopen(fileName, mode); 

	if ( file == NULL )
	{
		perror("Failed to open/create file");
		return NULL;
	}

	// add code to add the file ptr to the data->map							debug, zz

	return file;
}


//-----------------------------------------------------------------------------
int npReadCSV (void* readBuffer, int elementSize, int elementCount, 
				 FILE* file, void* dataRef)
{
	int bytes = 0;

	bytes = fread (readBuffer, elementSize, elementCount, file);
	
	if ( bytes == 0 )
		printf("Failed Reading\n");
	else
		printf("Success Reading\n");

	return bytes;
}


//-----------------------------------------------------------------------------
void npTestCSV (void* fileName, int wordSize, int size, void* dataRef)
{
	FILE *file;
	size_t count;
	char *str = (char*)fileName;
	int i = 0;


	char buffer[256];
/*
	// get the Current Working Directory
	npGetCWD (buffer, &size);

	printf("CWD path : ");
	for(i = 0; i < size; i++)
		printf ("%c", buffer[i]);

	printf("\n");
*/
	// open/create and write to the file, "w" overwrites existing files
	file = npOpenCSV (fileName, "r+", dataRef);

	// write the file path in the file as text
	count = npWriteCSV (buffer, 1, strlen(buffer), file, dataRef);

	printf("wrote %d bytes.\n", count); 

	// read the file
//	npFileRewind(file);

	size = npReadCSV (buffer, 1, 256, file, dataRef);


	printf("file contents : \n");
	for(i = 0; i < size; i++)
		printf ("%c", buffer[i]);

//mkdir(strB, S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH);

}

//-----------------------------------------------------------------------------
FILE* npCSVDialog (const char* fileName, int dialogType, void* dataRef)
{	
	
	switch (dialogType)
	{
		case kNPfileDialogNew : ; break;

		case kNPfileDialogOpen : ; break;
		case kNPfileDialogClose : ; break;

		case kNPfileDialogSave : ; break;
		case kNPfileDialogSaveAs : ; break;

		case kNPfileDialogImport : ; break;
		case kNPfileDialogExport : ; break;

		default : break;
	}

	return NULL;
}


//-----------------------------------------------------------------------------
int npWriteCSV (const void* str, int wordSize, int size, FILE* file, void* dataRef)
{
	return fwrite( str, 1, strlen(str), file );
}



void npSaveCSV (FILE* file, void* dataRef)
{
	return;
}


//-----------------------------------------------------------------------------
void npImportCSV (FILE* file, int type, void* dataRef)
{
	return;
}

//-----------------------------------------------------------------------------
void npExportCSV (FILE* file, void* dataRef)
{
	return;
}

//-----------------------------------------------------------------------------

// if not saved then opens SaveAs dialog before reseting app
void npCloseFileCSV (int size, void* dataRef)
{
	return;
}

//-----------------------------------------------------------------------------
void npCSVGetAttrib (int size, void* dataRef)
{
	return;
}

//-----------------------------------------------------------------------------
void npCSVSetAttrib (int size, void* dataRef)
{
	return;
}

//-----------------------------------------------------------------------------
//passed an open State File and reads it in based on type
void npCSVtoMap (FILE* file, int type, void*dataRef)
{
	size_t count = 0;
	int i = 0;
	int byteCount = 0;
	char* bufferIndex = 0;
	
	pData data = (pData) dataRef;
	NPnodePtr node = NULL;


	char* buffer = (char*)malloc(kNPmapFileBufferMax);
	if (buffer == NULL)
	{
		printf ("err 4285 - malloc failed, cannot write file");
		return;
	}

	if( file != NULL )   // check to see if file exists
	{
		printf("Reading File...\n");
		count = npFileRead (buffer, 1, kNPmapFileBufferMax, file, dataRef);
		printf("Bytes Read: %d\n", count);

		npFileClose (file, dataRef);	//done reading, close file
		
		if (count > 0)
		{
			printf("Loading File\n");			//add table type handling, debug zz
			npLoadMapFile (buffer, 1, count, dataRef);	//process data
			printf ("Done\n\n");
		}
		else
			printf("err 4297 - zero bytes read\n");
	}
	else
		printf("err 4298 - cannot open file \n");


	free (buffer);

	return;
}


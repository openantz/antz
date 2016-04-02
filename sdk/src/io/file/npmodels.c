/* -----------------------------------------------------------------------------
*
*  npmodels.c
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

#include "npmodels.h"

#include "../../npdata.h"


//-----------------------------------------------------------------------------
void npInitModels (void* dataRef)
{
	return;
}

//-----------------------------------------------------------------------------
void npCloseModels (void* dataRef)
{
	return;
}

void npDataInitModels( pNPmodels models, void* dataRef );
//-----------------------------------------------------------------------------
void npDataInitModels( pNPmodels models, void* dataRef )
{
	if( models = npMalloc( 0, sizeof(NPmodels), dataRef) )
	{
		models->id = 0;
		models->geometryID = 0;
		models->textureID = 0;
		models->fileName[0] = '\0';
		models->fileDir[0] = '\0';
	}
}

//-----------------------------------------------------------------------------
pNPmodels npLoadModels( const char* filePath, void* dataRef )
{
	pNPmodels models = NULL;

	npDataInitModels( models, dataRef );

	npPostMsg("3D Models not yet supported!!!", kNPmsgErr, dataRef);
	return models;
}


/* -----------------------------------------------------------------------------
*
*  nppio.c
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

#include "npio.h"

#include "io/npfile.h"
#include "io/npch.h"
#include "io/npmouse.h"


//-----------------------------------------------------------------------------
void npInitIO (void* dataRef)
{
	npInitMouse (dataRef);
	npInitFile (dataRef);			// may update default init from pref file
	npInitCh (dataRef);	//zz-JJ
}


//-----------------------------------------------------------------------------
void npCloseIO (void* dataRef)
{
	npCloseFile (dataRef);
	npCloseMouse (dataRef);
	npCloseCh (dataRef);	//zz-JJ
}


// update IO devices such as keyboard, mice, video, TNG3B and other inputs
//------------------------------------------------------------------------------
void npUpdateIO (void* dataRef)
{
	pData data = (pData) dataRef;

	data->io.cycleCount++;

	//we double buffer the mouse delta movement to maintain engine cycle sync
	npUpdateMouse (dataRef);

	npUpdateCh (dataRef);	//zz-JJ
}



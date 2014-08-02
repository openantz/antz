/* -----------------------------------------------------------------------------
*
*  npio.h
*
*  ANTz - realtime 3D data visualization tools for the real-world, based on NPE.
*
*  ANTz is hosted at http://openantz.com and NPE at http://neuralphysics.org
*
*  Written in 2010-2014 by Shane Saxon - saxon@openantz.com
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

#ifndef NPIO_H_
#define NPIO_H_

#ifdef __cplusplus
	extern "C" {
#endif

#include "npdata.h"
#include "io/npconsole.h"

void npInitIO (void* data);			// may update default init from pref file

void npCloseIO (void* data);

void npUpdateIO (void* dataRef);

void npViewer (void* dataRef);
void npConsole (void* dataRef);

//
void npPostNodeID( pNPnode node, void* dataRef );	//zz debug move -> io/...

pNPmenu npdbGetMenu (void* dataRef);

#ifdef __cplusplus
	}
#endif
#endif


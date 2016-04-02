/* -----------------------------------------------------------------------------
*
*  npselect.h
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

#ifndef NPSELECT_H_
#define NPSELECT_H_

#include "../npdata.h"

//---------------------------------------------------------------------------

/// npSelect.h encapsulates selection tools
void npSelectNodeByID (int id, void* dataRef);


/// Select node by type: all, cam, grid, glyph, link, 
void npNodeSelectionOn( pNPnode node, void* dataRef);
void npNodeSelectionOff( pNPnode node, void* dataRef);
void npSelectNode (pNPnode node, void* dataRef);
void npSelectAll (int nodeType, void* dataRef);
void npSelectNone (int nodeType, void* dataRef);
void npSelectInvert (int nodeType, void* dataRef);


void npInitSelect (void* dataRef);
void npUpdateSelect (void* dataRef);
void npCloseSelect (void* dataRef);

//void npSelectAdd( npNodeList, void* dataRef );
//void npSelectRemove( npNodeList, void* dataRef );

//void npSelectAttrib( npNodeFilter filter, void* dataRef );
void npSelectFilterFunc( void* dataRef );

/// npRegion struch{ int type; npFloatXYZ hi, lo };
// region on terrain, screen, spherical....3t
//void npSelectRegion( pNPset found, npRegion region, void* dataRef );

void npSetNew( void* dataRef );
void npSetAdd( void* dataRef );
void npSetRemove( void* dataRef );
void npSetCopy( void* dataRef );
void npSetMerge( void* dataRef );
void npSetDel( void* dataRef );
//void npSelectSetNext( pNPselectSet set, void* dataRef );

#endif


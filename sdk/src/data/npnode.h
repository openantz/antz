/* -----------------------------------------------------------------------------
*
*  npnode.h
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

#ifndef npnode_H_
#define npnode_H_


#include "../npdata.h"


//--- Functions ---------------------------------------------------------------

void npInitNode (void* dataRef);
void npCloseNode (void* dataRef);

void npInitNodeDefault (pNPnode node);
void npInitNodeCamera (pNPnode node);

int	npNewNodeID();

pNPnode npNodeNew (int nodeType, pNPnode nodeParent, void* dataRef);
pNPnode npNodeNewLink (pNPnode linkA, pNPnode linkB, void* dataRef);
bool npNodeAttach (pNPnode node, pNPnode parent, void* dataRef);
void npNodeDelete (pNPnode node, void* dataRef);
void npNodeRemove (bool freeNode, pNPnode node, void* dataRef);

void npSetTagOffset (pNPnode node);

pNPnode npGetActiveCam (void* dataRef);

//void Spiral( int X, int Y);
NPfloatXY npGridSpiralXY( int xNum, int yNum, float dX, float dY, int index );

#endif


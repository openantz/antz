/* -----------------------------------------------------------------------------
*
*  npmap.h
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

#ifndef NPMAP_H_
#define NPMAP_H_

#include "../npdata.h"


//--- Functions ---------------------------------------------------------------

void npInitMap (void* dataRef);
void npCloseMap (void* dataRef);

void* npMapNew (int mapType, void* dataRef);
void npMapDelete (NPmapTypePtr map, void* dataRef);

void npGetMaps (void* dataRef);

void npGetMapIO (void* dataRef); //Ports, GL (Sort Visible, Sort-Z, screen 1/2/3 )
void npGetMapNPE (void* dataRef); // collision, rotation, translation...

int npGetRootIndex (NPnodePtr node, void* dataRef);
void npSelectNode (NPnodePtr node, void* dataRef);

void npTraverseTree (void (*nodeFunc)(NPnodePtr node, void* dataRef), 
					 NPnodePtr node, void* dataRef);
void npTraverseMap (void (*nodeFunc)(NPnodePtr node, void* dataRef),
				   void* dataRef);

#endif


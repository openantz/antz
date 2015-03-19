/* -----------------------------------------------------------------------------
*
*  npgldraw.h
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

#ifndef NPGLDRAW_H_
#define NPGLDRAW_H_


#include "../../npdata.h"

//------------------------------------------------------------------------------

void npInitGLDraw (void* dataRef);
void npCloseGLDraw (void* dataRef);

void npDrawNodes (void* dataRef);
void npDrawHUD (void* dataRef);

void npIDtoRGB (int id, int* r, int* g, int* b);			//MB-PICK
int npRGBtoID (int r, int g, int b);						//MB-PICK

float npDistance (NPfloatXYZ vecA, NPfloatXYZ vecB);		//MB-TRANS

#endif


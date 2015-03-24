/* -----------------------------------------------------------------------------
*
*  npgeometry.h
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

#ifndef NPGL_H_
#define NPGL_H_


#include "../npdata.h"

#include "npglut.h"

#include "gl/npgldraw.h"
#include "gl/npgeometry.h"
#include "gl/npcolor.h"



//MB//
//--- Data -----
int selectedID;
//MB-end//

//--- Functions ----------------------------------------------------------------

void npInitGL (void* dataRef);
void npCloseGL (void* dataRef);

void npGLDrawScene (void* dataRef);

void npGLResizeScene (int width, int height);

void npPick (int x, int y, void* dataRef);

void npResizeConsole (void* dataRef);

void npPostMode (pNPnode node, void* dataRef);
void npPostTool (pNPnode node, void* dataRef);

void npMenuRefresh ( pNPhud hud, void* dataRef );

void npTagEditMode( bool tagMode, void* dataRef );

npScreenGrabThumb( char* name, int type, int x, int y, int w, int h, void* dataRef );

#endif


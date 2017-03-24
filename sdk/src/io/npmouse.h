/* -----------------------------------------------------------------------------
*
*  npmouse.h
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

#ifndef NPMOUSE_H_
#define NPMOUSE_H_

#include "../npdata.h"

void npInitMouse (void* dataRef);
void npCloseMouse (void* dataRef);

void npUpdateMouse (void* dataRef);

void npMouseEntry( int entry );
void npMouseEvent (int button, int state, int x, int y);
void npMouseMotion (int x, int y);
void npMousePosition (int x, int y);
void npMouseWheel (int wheel, int direction, int x, int y);

void npSetCamTarget (void* dataRef);

float npLowPassFilter( float start, float target, float fac );


#endif


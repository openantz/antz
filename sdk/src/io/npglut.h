/* -----------------------------------------------------------------------------
*
*  npglut.h
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

#ifndef NPGLUT_H_
#define NPGLUT_H_

#include "../npdata.h"

#ifdef NP_MSW_
	#include <freeglut.h>
#endif
#ifdef NP_OSX_
	#include <GLUT/glut.h>
	#include <OpenGL/gl.h>
#endif
#ifdef NP_LINUX_
	#include <GL/freeglut.h>
#endif


void npInitGlut (int argc, char **argv, void* dataRef);
void npCloseGlut (void* dataRef);

void npAppLoopGlut (void* dataRef);

void npglFullscreen (void* dataRef);

void npGLSolidSphere (GLdouble radius, GLint slices, GLint stacks);
void npGLWireSphere (GLdouble radius, GLint slices, GLint stacks);
void npGLSolidCone (GLdouble base, GLdouble height, GLint slices, GLint stacks);
void npGLWireCone (GLdouble base, GLdouble height, GLint slices, GLint stacks);
void npGLSolidTorus (GLdouble innerRadius, GLdouble outerRadius, 
					GLint slices, GLint stacks);
void npGLWireTorus (GLdouble innerRadius, GLdouble outerRadius, 
					GLint slices, GLint stacks);

void npGlutPrimitive (int primitive);

#endif

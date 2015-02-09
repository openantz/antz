/* -----------------------------------------------------------------------------
*
*  npcolor.h
*
*  ANTz - realtime 3D data visualization tools for the real-world, based on NPE.
*
*  ANTz is hosted at http://openantz.com and NPE at http://neuralphysics.org
*
*  Written in 2010-2014 by Shane Saxon - saxon@openantz.com
*
*  Please see main.c for a list of additional code contributors.
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

#ifndef NPCOLOR_H_
#define NPCOLOR_H_


#include "../../npdata.h"

/// @todo Support multiple color palletes with load, save and user sedit tools.
//------------------------------------------------------------------------------

void npInitColor (void* dataRef);
void npCloseColor (void* dataRef);

// NOTE x,y,z does not refer to XYZ color unless type = kNPcolorXYZtoRGB
NPfloatRGBA npColorPicker (float x, float y, float z, int type, void* data);
float npHueToRGB (float v1, float v2, float Hue);
void npHSLtoRGB (float H, float S, float L, float* R, float* G, float* B); //pNPfloatRGB rgbOut
NPfloatRGBA npHSLtoRGBA( float H, float S, float L, float alpha );

void npSetIndexColor (pNPubyteRGBA color, int *colorIndex, pNPmapColor palette);

// void npIDtoRGB (int id, int* r, int* g, int* b);			//MB-PICK
// int npRGBtoID (int r, int g, int b);						//MB-PICK

//------------------------------------------------------------------------------
enum
{
	kNPcolorRGB8,
	kNPcolorRGB16,
	kNPcolorXYtoRGB,
	kNPcolorXYtoHSB,
	kNPcolorHSB,		//HSB == HSV
	kNPcolorLAB,
	kNPcolorYCtoRGB,
	kNPcolorYUVtoRGB,
	kNPcolorCount
};
//------------------------------------------------------------------------------


#endif


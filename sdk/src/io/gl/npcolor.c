/* -----------------------------------------------------------------------------
*
*  npgldraw.c
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

#include "npcolor.h"



//------------------------------------------------------------------------------
void npInitColor( void* dataRef )
{
	pData data = (pData) dataRef;

	return;
}

/*

//MB//
// converts the ID value to an RGB value for picking objects by color
//------------------------------------------------------------------------------
void npIDtoRGB( int id, int* r, int* g, int* b )
{
	int remain = id;
	
	*r = remain / (256*256);	remain = remain - *r * (256*256);
	*g = remain / 256;			remain = remain - *g * 256;
	*b = remain;
}

//convert RGB pixel value back to the encoded node ID
int npRGBtoID( int r, int g, int b )
{
	int id;
	id = (256*256)*r + 256*g + b;

	return id;
}
//MB-End//



//zz debug, update to allow loading palette from CSV file and user defined color
// sets the node RGB color based on index, preserves existing alpha value
//------------------------------------------------------------------------------
void npSetIndexColor( NPubyteRGBA *color, int *colorIndex )
{	
	int index = *colorIndex;
									//kNPpaletteSize stored in nptypes.h
	static const GLubyte colorPalette[kNPpaletteSize][3] = {
											{50,  101, 101},
											{0,   255, 0},
											{255,   0, 0},
											{0,     0, 255},
											{255, 255, 0},
											{152,   0, 255},
											{255, 168, 0},
											{0,   255, 255},
											{255,   0, 255},
											{0,   153, 0},
											{185, 153, 102},
											{255, 180, 255},
											{0,   152, 255},
											{185, 255, 0},
											{152,   0, 0},
											{127, 127, 127},
											{127, 127, 255},
											{197,  82, 0},
											{0,     0, 0},
											{255, 255, 255}
											};
	//re-maps out of bounds colorIndex
	if (index >= kNPpaletteSize)
	{
		while (index >= kNPpaletteSize)
			index -= kNPpaletteSize;	//subtracts palette sized increments
		*colorIndex = index;
	}
	else if (index < 0)
	{
		while (index < 0)
			index += kNPpaletteSize;
		*colorIndex = index;			//adds palette sized increments		
	}

	//now set the color
	color->r = colorPalette[index][0];
	color->g = colorPalette[index][1];
	color->b = colorPalette[index][2];
}
*/
//zz debug, update to allow loading palette from CSV file and user defined color
// sets the node RGB color based on index, preserves existing alpha value
//------------------------------------------------------------------------------
void npSetIndexColor( NPubyteRGBA *color, int *colorIndex, pNPmapColor palette )
{	
	int index = *colorIndex;

	static const NPubyte colorPalette[kNPpaletteSize][3] = {
											{50,  101, 101},
											{0,   255, 0},
											{255,   0, 0},
											{0,     0, 255},
											{255, 255, 0},
											{152,   0, 255},
											{255, 168, 0},
											{0,   255, 255},
											{255,   0, 255},
											{0,   153, 0},
											{185, 153, 102},
											{255, 180, 255},
											{0,   152, 255},
											{185, 255, 0},
											{152,   0, 0},
											{127, 127, 127},
											{127, 127, 255},
											{197,  82, 0},
											{0,     0, 0},
											{255, 255, 255}
											};

//	if( !palette )
//		palette = currentPalette;


	//re-maps out of bounds colorIndex
	if (index >= kNPpaletteSize)
	{
		while (index >= kNPpaletteSize)
			index -= kNPpaletteSize;	//subtracts palette sized increments
		*colorIndex = index;
	}
	else if (index < 0)
	{
		while (index < 0)
			index += kNPpaletteSize;
		*colorIndex = index;			//adds palette sized increments		
	}

	//now set the color
	color->r = colorPalette[index][0];
	color->g = colorPalette[index][1];
	color->b = colorPalette[index][2];
}

// NOTE x,y,z does not refer to XYZ color unless type = kNPcolorXYZtoRGB
NPfloatRGBA npColorPicker (float x, float y, float z, int type, void* data);
float npHueToRGB (float v1, float v2, float Hue);
void npHSLtoRGB (float H, float S, float L, float* R, float* G, float* B); //pNPfloatRGB rgbOut

//------------------------------------------------------------------------------
float npHueToRGB( float v1, float v2, float vH )             //Function Hue_2_RGB
{
   if ( vH < 0.0 ) vH += 1.0f;
   if ( vH > 1.0 ) vH -= 1.0f;
   if ( (6.0f * vH) < 1.0f ) return (v1 + (v2 - v1) * 6.0f * vH);
   if ( (2.0f * vH) < 1.0f ) return (v2);
   if ( (3.0f * vH) < 2.0f ) return (v1 + (v2 - v1) * ((2.0f/3.0f) - vH) * 6.0f);
   return ( v1 );
}

// maps HSL (Hue Saturation Luminance) to RGB 0 to 255 ubyte color space
// HSL is also called HSV or other variants such as HSB
// calls npHueToRGB() for each component
//------------------------------------------------------------------------------
void npHSLtoRGB8 (float H, float S, float L,
				  unsigned char* R, unsigned char* G, unsigned char* B)
{
//	NPfloatRGB rgb;
//	pNPfloat pRGB = &rgb;
	float v1, v2;
	
	//HSL
	if ( S == 0.0f )							//HSL from 0 to 1
	{
		*R = (unsigned char)(L * 255.0f);		//RGB results from 0 to 255
		*G = (unsigned char)(L * 255.0f);
		*B = (unsigned char)(L * 255.0f);
	}
	else
	{
		if ( L < 0.5f )
			v2 = L * ( 1.0f + S );
		else
			v2 = ( L + S ) - ( S * L );
	   
	   v1 = 2.0f * L - v2;
	   
	   *R = (unsigned char)(255.0f * npHueToRGB(v1, v2, H + 0.3333333f) );
	   *G = (unsigned char)(255.0f * npHueToRGB(v1, v2, H) );
	   *B = (unsigned char)(255.0f * npHueToRGB(v1, v2, H - 0.3333333f) );
	}

	// return rgb;
}

// maps HSL (Hue Saturation Luminance) to RGB 0 to 1 float color space
// HSL is also called HSV or other variants such as HSB
// calls npHueToRGB() for each component
//------------------------------------------------------------------------------
void npHSLtoRGB( float H, float S, float L, float* R, float* G, float* B ) //pNPfloatRGB rgbOut
{
	float v1, v2;
	
	//HSL
	if ( S == 0 )						//HSL from 0 to 1
	{
		*R = L;	//R * 255 for ubyte		//RGB results from 0 to 255
		*G = L;	// * 255;
		*B = L;	// * 255;
	}
	else
	{
		if ( L < 0.5f )
			v2 = L * ( 1.0f + S );
		else
			v2 = ( L + S ) - ( S * L );
	   
	   v1 = 2.0f * L - v2;
	   
	   *R = npHueToRGB( v1, v2, H + 0.3333333f );	//R * 255 for ubyte
	   *G = npHueToRGB( v1, v2, H );				//G * 255 for ubyte
	   *B = npHueToRGB( v1, v2, H - 0.3333333f );	//B * 255 for ubyte
	}
}

NPfloatRGBA npHSLtoRGBA( float H, float S, float L, float alpha )
{
	NPfloatRGBA color; 

	// regular 'npHSLtoRGB()' color functions located in 'npcolor.h' 
	// alternate 'HSLtoRGB()' here for convienance
	npHSLtoRGB( H, S, L, &color.r, &color.g, &color.b);

	color.a = alpha;

	return color;
}

// NOTE x,y,z does not refer to XYZ color unless type = kNPcolorXYZtoRGB
//------------------------------------------------------------------------------
NPfloatRGBA npColorPicker( float x, float y, float z, int type, void* dataRef)
{
	NPfloatRGBA color = {0.0f,0.0f,0.0f};

	//zz add gradients and ability to fly color spaces... think video toaster
	//zz add function to get nearest index color (int) npGetColorIndex(R,G,B,A);

	switch (type)
	{
//		case kNPcolorRGB8 : break;		//bits per channel
//		case kNPcolorRGB16 : break;
		case kNPcolorXYtoRGB :		//set to 100% saturation S = 1
			npHSLtoRGB (x, 1.0f, y, &color.r, &color.g, &color.b);
			break;
//		case kNPcolorXYtoHSB : break;
//		case kNPcolorHSB : break;
//		case kNPcolorLAB : break;
//		case kNPcolorYCtoRGB : break;
//		case kNPcolorYUVtoRGB : break;
		default : 
			npHSLtoRGB (x, 1.0f, y, &color.r, &color.g, &color.b);
			break;//RGB
	}

	return color;
}

//---
// mouse / 6-DOE color picker... CMYK, RGB, lab, YUV, YC, Grayscale
// tracks velocity (over space time) for 6-DOE HID or regular XY mouse
// analogous to a 6 component color printer + white (alpha)
// maps up to 7 components with translation based on type
// control light color (power, wavelength, spread)
// polorization (u,v,w... could do more...)
// returns an RGBA color
NPfloatRGBA npColorPicker6DOE ( float x, float y, float z,
								float u, float v, float w, float t,
								int type, void* dataRef )
{
	//npColorPicker( u, v, w, type, dataRef);

	return npColorPicker( x, y, z, type, dataRef);
}
/*
int npColorID(ubyte r, ubyte g, ubyte b, int paletteID, void* dataRef);
// returns nearest colorID match(es) based on RGB, YUV, YC...
// colorID can be dynamically mapped to different palettes
// will not change color unless you also call npGetColorIndex(id)
// changing the colorIndex of a custom color maintains a phase offset!

// ---------------------------------------------------------------------------
int npColorID(ubyte r, ubyte g, ubyte b, int paletteID, void* dataRef)
{ return 0;
}
z
*/




/*
// ---------------------------------------------------------------------------
// ---------------------------------------------------------------------------
// ---------------------------------------------------------------------------

//MB-Transp
//------------------------------------------------------------------------------
//Distance between two points
float npDistance( NPfloatXYZ vecA, NPfloatXYZ vecB )
{
	float x = vecA.x - vecB.x;
	float y = vecA.y - vecB.y;
	float z = vecA.z - vecB.z;
	
//	float len = x*x + y*y + z*z;
//	len = sqrtf( len );
//	return len;

	return sqrtf( x*x + y*y + z*z );
}

//------------------------------------------------------------------------------
//QSort Compare function
int comparePinsDistanceFromCamera (const void* a, const void* b)
{
	pNPnode pinA = *(pNPnode*)a;
	pNPnode pinB = *(pNPnode*)b;
	
	//optimized for the common case where neither a nor b is null
	if( pinA == NULL || pinB == NULL )	//Null element in pinA & pinB: Nothing or Grid or Camera
	{
		if( pinA != NULL )				//Null element in pinB: Nothing or Grid or Camera
			return +1;					//pinB is at beginning of list... therefore pinA is "greater than" pinB. Return +1
		if( pinB != NULL )				//Null element in pinA: Nothing or Grid or Camera
			return -1;					//pinA is at beginning of list... therefore pinA is    "less than" pinB. Return -1
		return 0;						//These are "equal". Both should be at the beginning of the list.
	}

	if( pinA->distFromCamera   >  pinB->distFromCamera )	//pinA is furter from camera than pinB.
		return -1;											//Therefore should be at the beginning of the list. ie "less than"... Return -1
	if( pinA->distFromCamera   <  pinB->distFromCamera )	//pinA is closer from camera than pinB.
		return +1;											//Therefore should be at the end of the list.    ie "greater than"... Return +1
	if( pinA->distFromCamera  ==  pinB->distFromCamera )	//pinA & pinB are exactly the same distance from the camera. (impossible-ish)
		return 0;											//Return 0;

	//return ( *(int*)a - *(int*)b );
	return -1;
}

int sortCount = 0;	//zzhp
void npDrawSortThread(void* dataRef);
//------------------------------------------------------------------------------
void npDrawSortThread(void* dataRef)										 //zzhp
{
	int waitForSwap = 0;
	pData data = (pData) dataRef;

	int* killThread = false; //data->map.sortKillThread;		//zz debug

	while (!killThread)
	{
		//swap flag set by draw routine when its reaches the end of sort buffer
		//loading flag signals not to sort 
		while(data->map.sortSwapFlag || data->io.file.loading
				|| data->io.clearIndex)		//zzhp test flag pauses sorting, rigged to background colorIndex
			nposSleep(0.001);

		if (data->map.sortSwap)
		{
			data->map.sortCountA = data->map.nodeRootCount;
			npDrawSort (data->map.sortA, data->map.sortCountA, data);
		}
		else
		{
			data->map.sortCountB = data->map.nodeRootCount;
			npDrawSort (data->map.sortB, data->map.sortCountB, data);
		}

		data->map.sortSwapFlag = true;		//flag that sortDone
	}

	nposEndThread();
}

//------------------------------------------------------------------------------
void npDrawSort(void** sort, int count, void* dataRef)						//zzhp
{
	int i = 0;

	pData data = (pData) dataRef;

	pNPnode node = NULL;

	pNPnode camNode = data->map.currentCam;
	NPfloatXYZ camVec = camNode->translate;
	
	int nodeRootCount = data->map.nodeRootCount;
//	sortCount = nodeRootCount;											//zzhp

	//add root node thread locking and node delete synchro											//zz debug 

	//could pre-subsample the node list when large nodeRootCount > 100K		//zz debug

	//update root node distance from camera
	for( i = 0; i < count; i++ )		//zzhp
	{
		if( i < kNPnodeRootPin )	//Nothing, Camera, Grid, HUD...
		{
			sort[i] = NULL;
			continue;
		}
		
		node = data->map.node[i];
		node->distFromCamera = npDistance(camVec, node->translate);

		sort[i] = node;
	}

	if (count > data->map.nodeRootCount)
	{
		printf("err 2399 - count > data->map.nodeRootCount by: %d\n",
				(count - data->map.nodeRootCount));
		return;
	}
	//create z-sorted list, back to front
	//QuickSort based on Distance From Camera, back to front z-sorted
	qsort( sort, count, sizeof(pNPnode),
			comparePinsDistanceFromCamera );
}
*/


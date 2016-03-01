/* -----------------------------------------------------------------------------
*
*  npfreeimage.c
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

#include "npfreeimage.h"

#include "../npglut.h"		///< Need to include before FreeImage.h
#include "../../os/npos.h"  ///< Need to include before FreeImage.h

#include "FreeImage.h"		///< FreeImage library header

//-----------------------------------------------------------------------------
void npInitFreeImage (void* dataRef)
{
#ifndef __BIG_ENDIAN
	npPostMsg("err 4477 - FreeImage ENDIAN not supported", kNPmsgErr, dataRef);
#endif

	return;
}

//-----------------------------------------------------------------------------
void npCloseFreeImage (void* dataRef)
{
	return;
}


//zz debug update to properly support GIF files
//-----------------------------------------------------------------------------
int npfiLoadTexture( const char* filename, void* dataRef)
{
	char msg[kNPmaxPath] = {'\0'};

	int textureID = 0;					// GL texture ID to return on success
	int image_format = GL_BGR_EXT;		///< BGR for intel CPU, RGB for ARM
	int internal_format = GL_RGB8;		///< default set to RGB (not RGBA)
	int mipLevel = 0;					// GL mipmap level
	int border = 0;						// GL must be 0
	int width = 0, height = 0;			// GL image size

	const GLfloat sgenparams[] = { 1.0f, 0.0f, 0.0f, 0.0f };
	const GLfloat tgenparams[] = { 0.0f, 1.0f, 0.0f, 0.0f };

	FREE_IMAGE_FORMAT fiFormat = FIF_UNKNOWN;	// image format
	FIBITMAP *dib = NULL;						// image data
	BYTE *pixels = NULL;						// pixel data

	pData data = (pData) dataRef;
	int maxTexSize = data->io.gl.maxTextureSize;

	// get format from file signature
	fiFormat = FreeImage_GetFileType( filename, 0 );

	// if unknown then try using the file extension to determine format
	if( fiFormat == FIF_UNKNOWN ) 
		fiFormat = FreeImage_GetFIFFromFilename( filename );

	if( fiFormat == FIF_UNKNOWN )
	{
		sprintf( msg, "err 4489 - FreeImage unknown file format: %s", filename );
		npPostMsg( msg, kNPmsgErr, dataRef );
		return 0;
	}

	// load the file to memory
	if( FreeImage_FIFSupportsReading( fiFormat ) )
		dib = FreeImage_Load( fiFormat, filename, 0 );

	if( !dib )
	{
		sprintf( msg, "err 4490 - FreeImage failed to load: %s", filename );
		npPostMsg( msg, kNPmsgErr, dataRef );
		return 0;
	}

	/// If image has alpha then set GL types to GL_BGRA_EXT and GL_RGBA8
	if( FreeImage_GetColorType( dib ) == FIC_RGBALPHA )
	{
		image_format = GL_BGRA_EXT;		// BGRA for intel CPU's otherwise RGB
		internal_format = GL_RGBA8;	
	}

	// get pointer pixel data
	pixels = FreeImage_GetBits( dib );

	// get width and height
	width = FreeImage_GetWidth( dib );
	height = FreeImage_GetHeight( dib );

	if( pixels == 0 || width == 0 || height == 0 )
	{
		sprintf( msg, "err 4491 - FreeImage has 0 pixels for file: %s", filename );
		npPostMsg( msg, kNPmsgErr, dataRef );
		return 0;
	}

	/// @todo support downsampling when using FreeImage (SOIL is automatic)
	if( width > maxTexSize || height > maxTexSize )
	{
		sprintf( msg, "err 4492 - texture file to big: %s", filename );
		npPostMsg( msg, kNPmsgErr, dataRef );
		return 0;
	}

	// generate a texture and bind the texture id
	glGenTextures( 1, &textureID );
	glBindTexture( GL_TEXTURE_2D, textureID );

	//zz debug for unknown reason, without these the glTexParam... thetexture will 
	// not display on objects, unless the texture has been displayed on the grid 
	// at which point it works on the objects too, very strange.
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

	// load the texture map to the OpenGL GPU
	glTexImage2D( GL_TEXTURE_2D, mipLevel, internal_format, width, height,
		border, image_format, GL_UNSIGNED_BYTE, pixels );

	// free memory allocated by FreeImage
	FreeImage_Unload( dib );

	return textureID;
}


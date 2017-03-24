/* -----------------------------------------------------------------------------
*
*  npfreeimage.c
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

#include "npfreeimage.h"

#include "../npglut.h"		///< Need to include before FreeImage.h
#include "../../os/npos.h"  ///< Need to include before FreeImage.h

#include "FreeImage.h"		///< FreeImage library header

// A (possible) method to get alpha from transparent GIF images is to use
// FreeImage_GetTransparentIndex(dib) to get the transparency color
// http://stackoverflow.com/questions/14952138/loading-images-with-transparency-under-freeimage
//
// The (missing) texture loading sample code with notes (C++).
// http://r3dux.org/2014/10/how-to-load-an-opengl-texture-using-the-freeimage-library-or-freeimageplus-technically/


//------------------------------------------------------------------------------
void npInitFreeImage (void* dataRef)
{
	/// @todo add BIG ENDIAN support for RISC CPU's
#ifndef __BIG_ENDIAN
	npPostMsg("err 4477 - FreeImage BIG ENDIAN problem", kNPmsgErr, dataRef);
#endif

	return;
}

//------------------------------------------------------------------------------
void npCloseFreeImage (void* dataRef)
{
	return;
}

//zz debug merge GIF < 24bit support into npfiLoadTexture2()
// Method to load an image into a texture using the freeimageplus library.
// Returns the texture ID or dies trying.
//------------------------------------------------------------------------------
int npfiLoadTexture( const char* filename, void* dataRef )
{
	int srcImageBPP = 0;			///< Source image pixel depth (BPP).
	int width  = 0;
	int height = 0;

	FREE_IMAGE_FORMAT format = 0;	///< Image file format.
	FIBITMAP* bitmap = NULL;		///< Source image data.
	FIBITMAP* bitmap32 = NULL;		///< Used for BPP conversion.

	GLubyte* textureData = NULL;	///< Raw pixel data formatted for GL.
	GLuint textureID = 0;			///< OpenGL texture ID.
	GLenum err = 0;					///< OpenGL error code.

	char msg[kNPurlMax];

	/// Get the image format from the file contents.
	format = FreeImage_GetFileType( filename, 0 );
	if( format == -1 )
		return 0;	// could not open file

    /// If failed to get format from contents then try using the file extension.
	if( format == FIF_UNKNOWN )
	{
		/// This method is slower and less reliable.
		format = FreeImage_GetFIFFromFilename( filename );

		/// Check if FIF_UNKNOWN, which means we can't read the file.
		if( !FreeImage_FIFSupportsReading( format ) )
			return 0;
	}

	/// Found the format, so load the image.
	bitmap = FreeImage_Load( format, filename, 0 );

	/// Determine the source image pixel depth, aka: Bits Per Pixel (BPP).
	srcImageBPP =  FreeImage_GetBPP( bitmap );

	/// If the BPP is 24bit or 32bit then load as is, otherwise convert to 32bit.
	if( srcImageBPP == 24 || srcImageBPP == 32 )
		bitmap32 = bitmap;
    else
		bitmap32 = FreeImage_ConvertTo32Bits( bitmap );

	width  = FreeImage_GetWidth( bitmap32 );
	height = FreeImage_GetHeight( bitmap32 );

	// Get a pointer to the texture data as an array of unsigned bytes.
	// Note: At this point bitmap32 ALWAYS holds a 32-bit colour version of our image - so we get our data from that.
	textureData = FreeImage_GetBits( bitmap32 );

	// Generate a texture ID and bind to it
	glGenTextures( 1, &textureID );
	glBindTexture( GL_TEXTURE_2D, textureID );

	//zz moved to npInitTexMap()
	//glPixelStorei(GL_PACK_ALIGNMENT, 1); //zz issue #

	// Construct the texture.
	// Note: The 'Data format' is the format of the image data as provided by the image library. FreeImage decodes images into
	// BGR/BGRA format, but we want to work with it in the more common RGBA format, so we specify the 'Internal format' as such.
    if( srcImageBPP == 24 )
		glTexImage2D( GL_TEXTURE_2D,	// Type of texture
					0,					// Mipmap level (0 being the top level i.e. full size)
					GL_RGB,				// Internal format
					width,				// Width of the texture
					height,				// Height of the texture,
					0,					// Border in pixels
					GL_BGR_EXT,			// Data format
					GL_UNSIGNED_BYTE,	// Type of texture data
					textureData );		// The image data to use for this texture
	else
		glTexImage2D(	GL_TEXTURE_2D,
					0,
					GL_RGBA,
					width,
					height,
					0,
					GL_BGRA_EXT,
					GL_UNSIGNED_BYTE,
					textureData );

	// Specify our minification and magnification filters, otherwise bug #
	glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR );
	glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );


	/// If min or mag filter is LINEAR or NEAREST then create mipmaps.
    /// @todo Requires OpenGL 3.0 or greater, add GLEW checking.
//	if( kNPglMipmaps )
//		glGenerateMipmap( GL_TEXTURE_2D );
//	printf( "OpenGL ver: %s\n"(const char*)glGetString(GL_VERSION) );

	// Check for OpenGL texture creation errors such as...
	// GL_INVALID_ENUM GL_INVALID_VALUE GL_INVALID_OPERATION
	if( err = glGetError() )
	{

		sprintf(msg, "err 4494 - GL err: %d  image file: %s", err, filename);
		npPostMsg( msg, kNPmsgErr, dataRef );
	}


	// Convert our image up to 32 bits (8 bits per channel, Red/Green/Blue/Alpha) -
	// but only if the image is not already 32 bits (i.e. 8 bits per channel).
	// Note: ConvertTo32Bits returns a CLONE of the image data - so if we
	// allocate this back to itself without using our bitmap32 intermediate
	// we will LEAK the original bitmap data, and valgrind will show things like this:
	//
	// LEAK SUMMARY:
	//  definitely lost: 24 bytes in 2 blocks
	//  indirectly lost: 1,024,874 bytes in 14 blocks    <--- Ouch.
	//
	/// Free secondary converted color-space buffer.
	if( bitmap != bitmap32 )
		FreeImage_Unload( bitmap );

	/// Free primary image buffer bitmap.
	FreeImage_Unload( bitmap32 );

	return textureID;
}

//zz debug update to properly support GIF files
// merge GIF < 24bit support from npfiLoadTexture()
//------------------------------------------------------------------------------
int npfiLoadTexture2( const char* filename, void* dataRef)
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
	int maxTexSize = data->io.gl.tex.maxSize;

	// get format from file signature
	fiFormat = FreeImage_GetFileType( filename, 0 );

	// if unknown then try using the file extension to determine format
	if( fiFormat == FIF_UNKNOWN ) 
		fiFormat = FreeImage_GetFIFFromFilename( filename );

	if( fiFormat == FIF_UNKNOWN )
	{
		sprintf( msg, "err 4489 - FreeImage unknown file format: %s", filename);
		npPostMsg( msg, kNPmsgErr, dataRef );
		return 0;
	}

	// load the file to memory
	if( FreeImage_FIFSupportsReading( fiFormat ) )
		dib = FreeImage_Load( fiFormat, filename, 0 );

	if( !dib )
	{
		sprintf( msg, "err 4490 - FreeImage could not load: %s", filename );
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
		sprintf( msg, "err 4491 - FreeImage has 0 pixels file: %s", filename );
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

	//zz debug for unkown reason, without these the glTexParam... thetexture will 
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

int npfiConvertFormat( int format);
//-----------------------------------------------------------------------------
/*!
* @param format input is of type kNPfile... ie: kNPfileDDS
* @return FreeImage format such as FIF_BMP
*
* Convert from native kNPfile... type to FreeImage type FIF_...
*/
int npfiConvertFormat( int format)
{
	if( !format)
		return FIF_BMP;	/// make BMP the default for 0

	switch( format)
	{
		case kNPfileBMP : return FIF_BMP; break;
		case kNPfileCUT : return FIF_CUT; break;
		case kNPfileDDS : return FIF_DDS; break;
		case kNPfileEXR : return FIF_EXR; break;
		case kNPfileFAX : return FIF_FAXG3; break;	
		case kNPfileGIF : return FIF_GIF; break;
		case kNPfileHDR : return FIF_HDR; break;
		case kNPfileICO : return FIF_ICO; break;
		case kNPfileIFF : return FIF_IFF; break;
		case kNPfileJ2K : return FIF_J2K; break;
		case kNPfileJP2 : return FIF_JP2; break;
		case kNPfileJPG : return FIF_JPEG; break;
		case kNPfileJNG : return FIF_JNG; break;
		case kNPfileJXR : return FIF_JXR; break;
		case kNPfileKOA : return FIF_KOALA; break;
		case kNPfileLBM : return FIF_LBM; break;
		case kNPfileMNG : return FIF_MNG; break;
		case kNPfilePBM : return FIF_PBM; break;
		case kNPfilePBMRAW : return FIF_PBMRAW; break;
		case kNPfilePCD : return FIF_PCD; break;
		case kNPfilePCX : return FIF_PCX; break;	
		case kNPfilePFM : return FIF_PFM; break;
		case kNPfilePICT : return FIF_PICT; break;
		case kNPfilePGM : return FIF_PGM; break;
		case kNPfilePGMRAW : return FIF_PGMRAW; break;
		case kNPfilePNG : return FIF_PNG; break;
		case kNPfilePPM : return FIF_PPM; break;
		case kNPfilePPMRAW : return FIF_PPMRAW; break;
		case kNPfilePSD : return FIF_PSD; break;
		case kNPfileRAS : return FIF_RAS; break;
		case kNPfileRAW : return FIF_RAW; break;
		case kNPfileSGI : return FIF_SGI; break;
		case kNPfileTGA : return FIF_TARGA; break;
		case kNPfileTIFF : return FIF_TIFF; break;
		case kNPfileWBMP : return FIF_WBMP; break;
		case kNPfileWEBP : return FIF_WEBP; break;
		case kNPfileXBM : return FIF_XBM; break;
		case kNPfileXPM : return FIF_XPM; break;
	}

	npPostMsg("err 4493 - FIF format unknown", kNPmsgErr, NULL);
	return FIF_UNKNOWN;
}
//-----------------------------------------------------------------------------
/*!
* @param dataRef global scene graph reference.
* @param filePath screenshot file name with path.
* @return 
*
* Save a screenshot, either full window or thumbnail.
*/
int npfiScreenshot( const char* filePath, int format, int alpha, void* dataRef)
{
	int result = 0;

	FIBITMAP* image = NULL;
	GLubyte* pixels = NULL;
		
	pData data = (pData) npGetDataRef();

	int w = data->io.gl.width;
	int h = data->io.gl.height;

	int fiFormat = npfiConvertFormat( format);
	
	// used by glReadPixels() moved to npInitTexMap()
	// glPixelStorei( GL_PACK_ALIGNMENT, 1);

	/// capture screenshot with alpha, RGBA
	if( alpha )
	{
		/// create RGBA pixel buffer = 4 bytes X width X height
		pixels = malloc( 4 * w * h * sizeof(GLubyte) );
		if( !pixels ) return -1;

		/// Transfer GPU buffer to our pixels array (in CPU RAM)
		/// Use the GL_BGRA_EXT param, otherwise RED and BLUE get swapped
		glReadPixels( 0, 0, w, h, GL_BGRA_EXT, GL_UNSIGNED_BYTE, pixels);

		/// Convert to FreeImage format & save to file
		image = FreeImage_ConvertFromRawBits( pixels, w, h, w * 4, 32,
			FI_RGBA_RED_MASK, FI_RGBA_GREEN_MASK, FI_RGBA_BLUE_MASK , false);

		/// Save screenshot as RGBA  
		result = FreeImage_Save( FIF_TIFF, image, filePath, 0);
		}
	else /// else capture without alpha, RGB = 3 bytes X width X height
	{
		pixels = malloc( 3 * w * h * sizeof(GLubyte) );
		if( !pixels ) return -1;

		glReadPixels( 0, 0, w, h, GL_BGR_EXT, GL_UNSIGNED_BYTE, pixels);

		image = FreeImage_ConvertFromRawBits( pixels, w, h, w * 3, 24,
										0xFF0000, 0x00FF00, 0x0000FF, false);
	
		/// Save screenshot as JPEG  
		result = FreeImage_Save( FIF_JPEG, image, filePath, 0);
	
		//zz	result = FreeImage_Save( npfiFormat(format), image, filePath, 0);
	}

	// free resources
	FreeImage_Unload( image);
	free (pixels);

	if( !result )
		return -2;
	else
		return 0;
}


/* -----------------------------------------------------------------------------
*
*  npgl.c
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

#include "npgl.h"

#include "../lib/soil/src/SOIL.h"						//zz debug should just be SOIL.h

#include "gl/nptags.h"

#include "../npctrl.h"
#include "../os/npos.h"

#include "watchtools.h"


void npGLResizeScene (int width, int height);
void npCopyGPUtoCPU (void* dataRef);


//Textures, fonts, display lists, etc... can all be shared provided that:
//All rendering contexts of a shared display list must use an identical pixel format.
//http://www.opengl.org/discussion_boards/ubbthreads.php?ubb=showflat&Number=79299&page=1
//wglShareLists() supports texture sharing across GL contexts
//------------------------------------------------------------------------------
void npLoadTextures(void* dataRef) 
{
	int textureSize = 0;
	int fileNumber = 1;
	char* filename = (char*)malloc(4096);

	unsigned int textureID;									// texture, debug zz

											//zz debug, allow loading textures at runtime, 
											//use data->io.file.mapPath
	pData data = (pData) dataRef;

	glGetIntegerv (GL_MAX_TEXTURE_SIZE, &textureSize);
	printf ("\nMax Texture Size: %dx%d\n", textureSize, textureSize);
	printf ("Larger textures down converted\n", textureSize);

	printf ("\nSearching For Textures\n");
	// load our texture		// texture, debug zz
	for (fileNumber = 1; fileNumber <= kNPtextureMax; fileNumber++)
	{
		//append the file number, deals with the leading zeros
		if (fileNumber < 10)
			sprintf (filename, "antzmaps/map0000%d.jpg", fileNumber);
		else if (fileNumber < 100)
			sprintf (filename, "antzmaps/map000%d.jpg", fileNumber);
		else if (fileNumber < 1000)
			sprintf (filename, "antzmaps/map00%d.jpg", fileNumber);
		else if (fileNumber < 10000)
			sprintf (filename, "antzmaps/map0%d.jpg", fileNumber);

		textureID = SOIL_load_OGL_texture
		(
			filename,
			SOIL_LOAD_AUTO,
			SOIL_CREATE_NEW_ID,
			SOIL_FLAG_MIPMAPS | SOIL_FLAG_INVERT_Y
			| SOIL_FLAG_NTSC_SAFE_RGB | SOIL_FLAG_COMPRESS_TO_DXT
		);																// texture, debug zz
		
		//the last texture loaded is the texture count, non-loads return a texture=0
		if (textureID)
		{
			data->io.gl.textureCount = textureID;
			printf ("Loaded textureID: %d\n", textureID);
		}
	}

	if (data->io.gl.textureCount)
		printf ("Done Loading Textures\n\n");
	else
		printf ("Could Not Find Textures\n\n");

}


//------------------------------------------------------------------------------
void npInitGL(void* dataRef) 
{
	npInitGLDraw (dataRef);

	npInitGLPrimitive (dataRef);

	npInitTags (dataRef);		//do this before loading textures

	npLoadTextures (dataRef);

//	glShadeModel (GL_FLAT);
//	glDepthFunc (GL_LEQUAL);
//	glHint (GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);
}

//------------------------------------------------------------------------------
void npCloseGL (void* dataRef) 
{
	npCloseTags (dataRef);

	//	npFreeTextures (dataRef);		//debug, zz
}


// currently just single window support, add multi-screen capability later... debug zz
// redbook has info on how to calculate the projections
//------------------------------------------------------------------------------
void npGLResizeScene (int width, int height)							
{
	static int resizeCount = 0;
	char msg[128];

	int camIndex = 1;	//update with list in data->io->view.camIndex[]		debug zz
	GLfloat ratio = 1.0;

	pData data = npGetDataRef();

	NPnodePtr camNode = data->map.node[camIndex];
	NPcameraPtr camData = camNode->data;

	data->io.gl.width = width;
	data->io.gl.height = height;

//	if (resizeCount++ > 2)
	{
		sprintf(msg, "GL Resize: %dx%d\n", width, height);
		npPostMsg(msg, kNPmsgGL, data);
	}

	if (height > 0)							//prevent divide by zero
		camData->aspectRatio = (GLfloat)width / (GLfloat)height;

	// Reset the coordinate system before modifying
	glMatrixMode (GL_PROJECTION);
	glLoadIdentity ();
	
	// Set the viewport to be the entire window
    glViewport(0, 0, width, height);

	// Set the clipping volume
	gluPerspective (camData->fov, camData->aspectRatio, 
					camData->clipNear, camData->clipFar);
	glMatrixMode (GL_MODELVIEW);

	npResizeConsole(data);

	npGLDrawScene (data);
}

//setup lighting, alpha mode transparency and background
//------------------------------------------------------------------------------
void npGLLighting (void* dataRef)
{
	GLfloat positionA[] = { -1500.0f, 1200.0f, -1800.0f, 1.0f };
	GLfloat positionB[] = { 1500.0f, -1200.0f, 1800.0f, 1.0f };

	GLfloat white[] = {1.0f, 1.0f, 1.0f};
	GLfloat black[] = {0.0f, 0.0f, 0.0f};

	//see old versions, early 2011, for interesting materials lighting
//	GLfloat mShininess[] = {128}; //set the shininess of the material
//	GLfloat globalAmbient[] = { 0.1f, 0.1f, 0.1f, 1.0f };
//	GLfloat redNotGreenNotBlue[] = {1.0f, -1.0f, -1.0f}; //negative light, red

	pData data = (pData) dataRef;

	//needed for transparency
	glEnable (GL_DEPTH_TEST);		//MB-Transp
	glDepthFunc (GL_LESS);

	if( data->io.gl.pickPass )
	{
		glDisable (GL_LIGHTING);		//disable lighting and alpha for pickPass
		glDisable (GL_BLEND);
		glClearColor(0.0f, 0.0f, 0.0f, 1.0f); //background id 0 mapped to black
	}
	else
	{
		glEnable (GL_LIGHTING);		//MB-Transp
		glEnable (GL_BLEND);
		glClearColor(data->io.clear.r, data->io.clear.g, data->io.clear.b, 1.0f);
	}

	//clear background using the clear color, set to black for pickPass
	glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	//set the alphamode
	switch (data->io.gl.alphaMode)
	{
		case kNPalphaModeNull :
			glDisable (GL_BLEND);		//no transparency, disable GL_BLEND
			break;
		case kNPalphaModeSubtractive :
			glBlendFunc (GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);	//MB-Transp
			break;
		case kNPalphaModeAdditive :
			glBlendFunc (GL_SRC_ALPHA, GL_DST_COLOR);
			break;
		case kNPalphaModeDark :
			glBlendFunc (GL_SRC_COLOR, GL_ONE_MINUS_SRC_ALPHA);
			break;
		default :
			glBlendFunc (GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);	//MB-Transp
			break;
	}

	glEnable (GL_COLOR_MATERIAL);	//without everything is grayscale

	glEnable (GL_LIGHT0);
	glEnable (GL_LIGHT1);

	glLightfv (GL_LIGHT0, GL_POSITION, positionA);
	glLightfv (GL_LIGHT0, GL_SPECULAR, white);
    glLightfv (GL_LIGHT0, GL_AMBIENT, black);
    glLightfv (GL_LIGHT0, GL_DIFFUSE, white);

	glLightfv (GL_LIGHT1, GL_POSITION, positionB);
	glLightfv (GL_LIGHT1, GL_SPECULAR, white);
    glLightfv (GL_LIGHT1, GL_AMBIENT, black);
    glLightfv (GL_LIGHT1, GL_DIFFUSE, white);
}


//------------------------------------------------------------------------------
void npGLShading (dataRef)
{
	pData data = (pData) dataRef;

	
	//do not render back-faces, faster
	glEnable (GL_CULL_FACE);

	//set shading for flat or smooth
	if (data->io.gl.shade == kNPglShadeSmooth)
		glShadeModel (GL_SMOOTH);
	else
		glShadeModel (GL_FLAT);

	//recalculate normals for non-uniform scaling or rescale normals for speed
	//if disabled then small objects appear brighter and large dimmer
	switch (data->io.gl.normal)
	{
		case kNPglNormalize :				//geo-computationally slow
	//		glDisable (RESCALE_NORMAL);
			glEnable (GL_NORMALIZE);	//recalc normals for non-uniform scaling
			break;
		case kNPglRescaleNormal :			//fast								//debug, zz
			glDisable (GL_NORMALIZE);
			data->io.gl.normal++;			//workaround until RESCALE_NORMAL fixed, zz
	//		glEnable (RESCALE_NORMAL);	//rescale normals, assumes uniform scaling
			break;
		default :							//fastest
			glDisable (GL_NORMALIZE);
	//		glDisable (RESCALE_NORMAL);
			break;
	}
}

//------------------------------------------------------------------------------
void npGLDrawScene (void* dataRef) 
{
	int err = 0;
	//MB-LABEL
	NPfloatXYZ	trans;
	NPfloatXYZA rot;
	//MB-END
	
	int	camIndex = 1;
	GLfloat upX = 0.0f, upY = 0.0f, upZ = 1.0f;
	GLfloat angle = 0.0;

	pData data = (pData) dataRef;
	NPnodePtr camNode = npGetActiveCam (data);

	//MB-LABEL
	trans.x = 0.0f;	
	trans.y = 0.0f;
	trans.z = 100.0f;
	rot.x	= 0.0f;
	rot.y	= 0.0f;
	rot.z	= -1.0f;
	rot.angle = 0.0f;
	
#define BUGFIX_MATRIX 0.0000000001

	glMatrixMode(GL_MODELVIEW);

	//stereo 3D
	if (data->io.gl.stereo)
	{
	//	glMatrixMode(GL_MODELVIEW);	//outside of if 
		glDrawBuffer(GL_BACK_RIGHT);
		glLoadIdentity();
		
		gluLookAt (	camNode->translate.x, camNode->translate.y, camNode->translate.z,
					camNode->translate.x + camNode->rotateVec.x,
					camNode->translate.y + camNode->rotateVec.y + BUGFIX_MATRIX,
					camNode->translate.z + camNode->rotateVec.z,
					upX, upY, upZ );

		npGLLighting (dataRef);
		npGLShading (dataRef);
		npDrawNodes (dataRef);				//draws mesh, graphs, pins, etc...

		glMatrixMode(GL_MODELVIEW);
		glDrawBuffer(GL_BACK_LEFT);
	}

	glLoadIdentity();
	gluLookAt (	camNode->translate.x, camNode->translate.y, camNode->translate.z,
				camNode->translate.x + camNode->rotateVec.x,
				camNode->translate.y + camNode->rotateVec.y + BUGFIX_MATRIX,
				camNode->translate.z + camNode->rotateVec.z,
				upX, upY, upZ );

	npGLLighting (dataRef);				//set lights
	npGLShading (dataRef);				//set transparency
	npDrawNodes (dataRef);				//draws mesh, graphs, pins, etc...


	//	glOrtho ( -20.0f, 20.0f, -20.0f, 20.0f, -1.0f, 1.0f); // gluOrtho2D
	//HUD LookAt Camera
	glPushMatrix();
		glLoadIdentity();
		gluLookAt (trans.x, trans.y, trans.z, 
				   trans.x + rot.x,
				   trans.x + rot.y + BUGFIX_MATRIX,
				   trans.z + rot.z,
				   upX, upY, upZ );

		npDrawHUD (data);

	glPopMatrix();
	//MB-LABEL-END	

	// copy display buffer to main CPU memory
	npCopyGPUtoCPU (dataRef);

	err = glGetError();
	if (err)
		printf("err: 2388 - OpenGL error: %d\n", err);
}


// copy the rasterized display to the main system memory (CPU RAM or Drive)
//------------------------------------------------------------------------------
void npCopyGPUtoCPU (void* dataRef)
{
	return;
}

//------------------------------------------------------------------------------
void npHideSubNodes (bool hide, NPnodePtr node, void* dataRef)
{
	int i = 0;
	NPnodePtr child = NULL;

	for (i=0; i < node->childCount; i++)
	{
		child = node->child[i];

		// recursively call this function
		npHideSubNodes (hide, child, dataRef);
		
		// apply passed in function operation to node
		child->hide = hide;
	}
}

//set picked node params based on the mouse button and tool mode
//------------------------------------------------------------------------------
void npPickTool (NPnodePtr node, void* dataRef)
{
	char msg[256];
	pData data = (pData) dataRef;
	
	if (data->io.mouse.tool == kNPtoolNull)
	{
		if (data->io.mouse.buttonR)
			node->selected = 1 - node->selected;	//toggle value
		else
		{
			//if node not selected then clear selection, then select
			if (!node->selected)
				npCtrlCommand (kNPcmdSelectNone, data);

			node->selected = true;
		}
	}

	if (data->io.mouse.tool == kNPtoolInfo)
	{
		if (data->io.mouse.buttonR)
			node->tagMode = 0;
		else
			node->tagMode++;
		if (node->tagMode >= kNPtagModeCount)					//debug zz
			node->tagMode = 1;
		sprintf(msg, "text tag type: %d", node->tagMode);
		npPostMsg (msg, kNPmsgCtrl, dataRef);
	}

	if (data->io.mouse.tool == kNPtoolHide)
	{
		if (data->io.mouse.buttonR)
		{
		//	node->hide = true;
			npHideSubNodes (true, node, data);
			sprintf(msg, "Hide Branches");
		}
		else
		{
		//	node->hide = false;
			npHideSubNodes (false, node, data);
			sprintf(msg, "Show Branches");
		}
		
		npPostMsg (msg, kNPmsgCtrl, dataRef);
	}

	/*
	node->tagMode++;
	if (node->tagMode >= kNPtagModeCount)					//debug zz
		node->tagMode = 0;
	sprintf(msg, "text tag mode: %d", node->tagMode);
	npPostMsg (msg, kNPmsgCtrl, dataRef);
*/
}

//MB-CAMERA
// 2^24 = 16 million max id number for picking algorithm
// add methods to allow grabbing all objects in a circular or rectangular region
// and add a nearest mode which picks the object closest to the point clicked
//------------------------------------------------------------------------------
void npPick (int x, int y, void* dataRef)
{
	int id = 0;
	char msg[256];

	GLubyte r,g,b;	//Unsigned Byte 8-bit Range: 0-255 // changed from u_int8_t, zz debug
	GLubyte pixels[4];		//4 Bytes
	
	NPnodePtr node = NULL;
	pData data = (pData) dataRef;
	
	//set pickPass flag, causes draw routines to use node ID based flat-color
	data->io.gl.pickPass = true;

	npGLDrawScene (data);			//optimize to draw only picked region, debug zz

	//read the pixel that was picked
	glReadPixels( x,y, 1,1, GL_RGBA, GL_UNSIGNED_BYTE, pixels );
	
	r = pixels[0];
	g = pixels[1];
	b = pixels[2];			// 2^24 rgb limit, does not use alpha
	
	id = npRGBtoID( r,g,b );

	if (id >= kNPnodeMax)	//mouse selection crashing workaround, debug zz
		id = 0;				//add additional handling for bad picking IDs
	
	data->io.gl.pickID = id;

	if( id != 0 )
	{	
		node = data->map.nodeID[id];	//get node by id

		if (node != NULL)
		{
			npPickTool (node, data);

			//set active and selected node pointers unless in cam mode
			npSelectNode (node, data);

			//if recordID exists then display it, otherwise use node->id
			if (node->recordID)
				sprintf(msg, "recordID: %d", node->recordID);
			else
				sprintf(msg, "id: %d", node->id);
			npPostMsg(msg, kNPmsgCtrl, data);
		}
		else
			data->io.gl.pickID = 0;	//if invalid node then set to default camera
	}

	data->io.gl.pickPass = false;	//clear the flag to draw normal colors
}


//------------------------------------------------------------------------------
void npResizeConsole (void* dataRef)
{
	pData data = (pData) dataRef;
	pNPconsole console = (pNPconsole) &data->io.gl.hud.console;

	//add left, right, center, top, bottom handling...
	//for now assume bottom left
	console->screen.x = console->translate.x - (float)data->io.gl.width * 0.5f;
	console->screen.y = console->translate.y - (float)data->io.gl.height * 0.5f;

	//update to handle different fonts and line counts
	switch (console->mode)
	{
		case kNPconsoleModeOne :
			console->box.y = 1.0f * 15.0f + 8.0f;
			break;
		case kNPconsoleModeThree :
			console->box.y = 3.0f * 15.0f + 8.0f;
			break;
		case kNPconsoleModeMax :
			console->box.y = (float)kNPconsoleLineMax * 15.0f + 8.0f;
		break;
	}
}


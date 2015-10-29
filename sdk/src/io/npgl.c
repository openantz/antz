/* -----------------------------------------------------------------------------
*
*  npgl.c
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

#include "npgl.h"

#include "gl/nptags.h"

#include "../npio.h"
#include "../npctrl.h"
#include "../os/npos.h"

#include "watchtools.h"


void npGLResizeScene (int width, int height);
void npCopyGPUtoCPU (void* dataRef);


//------------------------------------------------------------------------------
void npInitGL(void* dataRef) 
{
	npInitGLDraw (dataRef);

	npInitGLPrimitive (dataRef);

	npInitTags (dataRef);		//do this before loading textures

//	nposBeginThread (npLoadTextures, dataRef);	//zz debug, add thread worker func wrapper
	npLoadTextures(dataRef);

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

	GLfloat ratio = 1.0;

	pData data = npGetDataRef();

	pNPnode camNode = data->map.currentCam;
	NPcameraPtr camData = camNode->data;

	data->io.gl.width = width;
	data->io.gl.height = height;

	if(!data->io.gl.fullscreen)
	{
		data->io.gl.windowSize.x = width;
		data->io.gl.windowSize.y = height;
	}

	if (resizeCount++ > 2)
	{
		sprintf(msg, "Resize: %dx%d", width, height);
		npPostMsg(msg, kNPmsgGL, data);
	}
	else if (resizeCount == 2)			//startup message
		npPostMsg("hint: click the Indicators to change XYZ Axes, Mode or Tool (also Mouse Wheel)",
					kNPmsgCtrl, data);

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

//	npGLDrawScene (data);	//zz debug, seems unecessary
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
		//case kNPalphaModeInverse :
		//	glBlendFunc(GL_ONE_MINUS_DST_COLOR, GL_ONE_MINUS_SRC_COLOR);
		//	break;
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
void npGLShading (void* dataRef)
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
	
	GLfloat upX = 0.0f, upY = 0.0f, upZ = 1.0f;
	GLfloat angle = 0.0;

	pData data = (pData) dataRef;
	pNPnode camNode = npGetActiveCam (data);
	NPcameraPtr camData = camNode->data;

	//MB-LABEL
	NPfloatXYZ	trans;
	NPfloatXYZA rot;

	trans.x = 0.0f;	
	trans.y = 0.0f;
	trans.z = 100.0f;
	rot.x	= 0.0f;
	rot.y	= 0.0f;
	rot.z	= -1.0f;
	rot.angle = 0.0f;
	//MB-END
	
	if (data->io.mouse.tool == kNPtoolSelect)		//zz select
		glutSetCursor(GLUT_CURSOR_CROSSHAIR);
	else
		glutSetCursor(GLUT_CURSOR_RIGHT_ARROW);


#define BUGFIX_MATRIX 0.0000000001	//prevents 0 vector issue

	glMatrixMode(GL_MODELVIEW);

	//stereo 3D
	if (data->io.gl.stereo3D)
	{
	//	glMatrixMode(GL_MODELVIEW);	//outside of if 
		glDrawBuffer( GL_BACK_RIGHT );
		glLoadIdentity();
		
		gluLookAt (	camNode->translate.x, camNode->translate.y, camNode->translate.z,
					camNode->translate.x + camNode->rotateVec.x,
					camNode->translate.y + camNode->rotateVec.y + BUGFIX_MATRIX,
					camNode->translate.z + camNode->rotateVec.z,
					upX, upY, upZ );
	
		//inverse view matrix used to convert local coordinates to world coords
		glGetFloatv( GL_MODELVIEW_MATRIX, camData->matrix );
		npInvertMatrixf( camData->matrix, camData->inverseMatrix );

		npGLLighting( dataRef );
		npGLShading( dataRef );
		npDrawNodes( dataRef );				//draws mesh, graphs, pins, etc...

		glMatrixMode( GL_MODELVIEW );
		glDrawBuffer( GL_BACK_LEFT );
	}

	glLoadIdentity();
	gluLookAt (	camNode->translate.x, camNode->translate.y, camNode->translate.z,
				camNode->translate.x + camNode->rotateVec.x,
				camNode->translate.y + camNode->rotateVec.y + BUGFIX_MATRIX,
				camNode->translate.z + camNode->rotateVec.z,
				upX, upY, upZ );
		
	//inverse view matrix used to convert local coordinates to world coordinates
	glGetFloatv (GL_MODELVIEW_MATRIX, camData->matrix);
	npInvertMatrixf (camData->matrix, camData->inverseMatrix);

	npGLLighting (dataRef);				//set lights
	npGLShading (dataRef);				//set transparency
	npDrawNodes (dataRef);				//draws mesh, graphs, pins, etc...

	//	glOrtho ( -20.0f, 20.0f, -20.0f, 20.0f, -1.0f, 1.0f); // gluOrtho2D	//zz debug

	//MB-LABEL
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
void npHideSubNodes (bool hide, pNPnode node, void* dataRef)
{
	int i = 0;
	bool allSubNodesHidden = true;
	pNPnode child = NULL;

	//normally clicked node should not be hidden, but just in case...
	if (!hide)
		node->hide = false;

	for (i=0; i < node->childCount; i++)
	{
		child = node->child[i];

		//picked node should be hidden only if all sub nodes already hidden
		if (!child->hide)
			allSubNodesHidden = false;

		// recursively call this function
		npHideSubNodes (hide, child, dataRef);
		
		// apply passed in function operation to node
		child->hide = hide;
	}

	//picked node should be hidden only if all sub nodes already hidden
	if (hide && allSubNodesHidden)
		node->hide = true;
	else
		node->hide = false;
}

//move this to npgldraw
//------------------------------------------------------------------------------
void npPostTool (pNPnode node, void* dataRef)
{
	pData data = (pData) dataRef;

	char msg[128];

	//workaround for exiting tag edit mode, does not catch all exit conditions
	if(	data->io.gl.hud.console.mode == kNPconsoleTag							//zz tag
			  && data->io.mouse.tool != kNPtoolTag )
				npTagEditMode(false, data);

	strcpy (msg, "tool: ");

	switch (data->io.mouse.tool)
	{
		case kNPtoolNull :
			strcat (msg, "None     ");
			break;
		case kNPtoolCombo :
			strcat( msg, "Combo    ");
			npPostMsg("L-drag background for Camera Examiner XY, L+R-drag for XZ, R-drag for Fly",kNPmsgHint,data);
			npPostMsg("L-click Pin select, R-click multiple, L/R/L+R-drag to move, rotate, scale, ratio",kNPmsgHint,data);	//insert blank space
			break;

		case kNPtoolCreate :
			strcat (msg, "Create   ");
			npPostMsg("L-drag or R-drag objects (same as Combo Tool), mouse-drag background for Camera",kNPmsgHint,data);
			npPostMsg("L-click creates New node, R-click to Delete",kNPmsgHint,data);
			break;
//		case kNPtoolDelete :
//			strcat (msg, "Delete   ");
//			strcpy (hint,"delete all selected nodes and all branches");
//			break;
		case kNPtoolLink :
			strcat (msg, "Link     ");
			npPostMsg("",kNPmsgHint,data);
			npPostMsg("L-click to Select A then Select B, R-click to Cancel (re-select A)",kNPmsgHint,data);
			break;	
		case kNPtoolMove :
			strcat (msg, "Move     ");
			npPostMsg("",kNPmsgHint,data);
			npPostMsg("L-drag Move in XY, R-drag for XZ",kNPmsgHint,data);
			break;
		case kNPtoolRotate :
			strcat (msg, "Rotate   ");
			npPostMsg("",kNPmsgHint,data);
			npPostMsg("L-drag Rotate XY, R-drag for Z",kNPmsgHint,data);
			break;
		case kNPtoolSize :
			strcat (msg, "Size     ");
			npPostMsg("",kNPmsgHint,data);
			npPostMsg("L-drag Scale XYZ axes or R-drag for torus Ratio",kNPmsgHint,data);
			break;

		case kNPtoolSelect :
			strcat (msg, "Select   ");
			npPostMsg("",kNPmsgHint,data);
			npPostMsg("L-click picks object, R-click group, Drag Region",kNPmsgHint,data);
			break;
		case kNPtoolTag :
			strcat (msg, "Text Tag ");
			npPostMsg("",kNPmsgHint,data);
			npPostMsg("L-click selects tag to edit, R-click hides Tag",kNPmsgHint,data);
			break;
		case kNPtoolHide :
			strcat (msg, "Hide     ");
			npPostMsg("",kNPmsgHint,data);
			npPostMsg("'~' key to Show ALL, '4' Hide ALL at branch level > 1",kNPmsgHint, data); //zz debug
			npPostMsg("L-click Show branches, R-click Hide branches",kNPmsgHint,data);
			break;

		case kNPtoolTopo :
			strcat (msg, "Topo     ");
			npPostMsg("",kNPmsgHint,data);
			npPostMsg("L-click next Topo type, R-click previous",kNPmsgHint,data);
			break;
		case kNPtoolGeometry :
			strcat (msg, "Geometry ");
			npPostMsg("Geometry does not effect Topology",kNPmsgHint, data); //zz debug
			npPostMsg("L-click next Geometry type, R-click previous",kNPmsgHint,data);
			break;
//		case kNPtoolSegments :
//			strcat (msg, "Segments ");
//			break;

		case kNPtoolColor :
			strcat (msg, "Color    ");
			npPostMsg("",kNPmsgHint,data);
			npPostMsg("L-click next Color index, R-click previous",kNPmsgHint,data);
			break;
		case kNPtoolAlpha :
			strcat (msg, "Alpha    ");
			npPostMsg("",kNPmsgHint,data);
			npPostMsg("L-click reduces Alpha tranparency color, R-click increases opacity",kNPmsgHint,data);
			break;
		case kNPtoolTexture :
			strcat (msg, "Texture  ");
			npPostMsg("",kNPmsgHint,data);
			npPostMsg("L-click next Topo type, R-click previous",kNPmsgHint,data);
			break;
		case kNPtoolChannel :
			strcat (msg, "Channel  ");
			npPostMsg("",kNPmsgHint,data);
			npPostMsg("L-click next Channel, R-click previous",kNPmsgHint,data);
			break;

		case kNPtoolFreeze :
			strcat (msg, "Freeze   ");
			npPostMsg("",kNPmsgHint,data);
			npPostMsg("L-click Freezes node, R-click un-Freezes",kNPmsgHint,data);
			break;
		case kNPtoolSetpointHi :
			strcat (msg, "Set High ");
			npPostMsg("",kNPmsgHint,data);
			npPostMsg("L-click to Set Upper Limit, R-click clears Setpoint",kNPmsgHint,data);
			break;
		case kNPtoolSetpointLo :
			strcat (msg, "Set Low  ");
			npPostMsg("",kNPmsgHint,data);
			npPostMsg("L-click to Set Lower Limit, R-click clears Setpoint",kNPmsgHint,data);
			break;

		default :
			strcat (msg, "Other    ");
			break;
	}

	//update the GUI tool type indicator when NULL
	if (node == NULL)
	{
		node = data->map.node[kNPnodeRootHUD];	//select Root HUD node
		node = node->child[kNPhudTool];			//select Tool node
	}
	strcpy (node->tag->title, msg);
	npUpdateTag (node->tag);

	//post the tool type message
	npPostMsg (msg, kNPmsgCtrl, dataRef);
}

//update the GUI tool type indicator when NULL
//------------------------------------------------------------------------------
void npMenuRefresh( pNPhud hud, void* dataRef )
{
	pNPnode node = NULL;
	pData data = (pData) dataRef;

//	if (hud == NULL)
	{
		node = data->map.node[kNPnodeRootHUD];	//select Root HUD node
		node = node->child[kNPhudTool];			//select Tool node
	}
	sprintf( node->tag->title, "tool:         " );
	npUpdateTag (node->tag);
}

//------------------------------------------------------------------------------
void npPostMode (pNPnode node, void* dataRef)
{
	char msg[128];
	pData data = (pData) dataRef;
	
	strcpy (msg, "mode: ");

	switch (data->io.mouse.pickMode)
	{
		case kNPmodeNull :
			strcat (msg, "None     ");
			break;
		case kNPmodeCamera :
			npPostMsg("Camera mode allows navigating without effecting objects",kNPmsgHint,data);
			npPostMsg("'N' key will create New sub-object Cameras",kNPmsgHint,data);
			strcat (msg, "Camera   ");
			break;
		case kNPmodeGrid :
			npPostMsg("the Grid currently only responds to keyboard commands",kNPmsgHint,data);
			npPostMsg("pressing 'Z' or Shift+Z will expand or compact entire scene height",kNPmsgHint,data);
			strcat (msg, "Grid     ");
			break;
		case kNPmodePin :
			npPostMsg("use either the mouse or keyboard to create, delete or modify objects",kNPmsgHint,data);
			npPostMsg("all 'tool' types effect Pin objects",kNPmsgHint,data);
			strcat (msg, "Pin      ");
			break;
		default :
			strcat (msg, "Other    ");
			break;
	}

	//update the GUI mode type indicator
	strcpy (node->tag->title, msg);
	npUpdateTag (node->tag);

	//post the tool type message
	npPostMsg (msg, kNPmsgCtrl, dataRef);
}

//------------------------------------------------------------------------------
void npPostAxes (void* dataRef)
{
	char msg[64];
	pData data = (pData) dataRef;
	
	strcpy (msg, "axes: ");

	if (data->io.axes.x)
		strcat (msg, "X");
	if (data->io.axes.y)
		strcat (msg, "Y");
	if (data->io.axes.z)
		strcat (msg, "Z");

	if (!data->io.axes.x && !data->io.axes.y && !data->io.axes.z)
		strcat (msg, "None");

	npPostMsg (msg, kNPmsgCtrl, dataRef);
}

//zz select
//------------------------------------------------------------------------------
void npSetMode( int mode, void* dataRef )
{
	pData data = (pData) dataRef;

	if (data->io.mouse.pickMode != mode)
	{
		data->io.mouse.pickMode = mode;
		switch (mode)
		{
			case kNPmodeCamera :
				npCtrlCommand( kNPcmdCamera, data );
				break;
			case kNPmodeGrid :
				npCtrlCommand( kNPcmdGrid, data );
				break;
			case kNPmodePin :
				npSelectNode(data->map.selectedPinNode, data);
				//npCtrlCommand( kNPcmdPin, data );
				break;
		}
	//	npPostMode (data->io.gl.hud., data);
	}
}

//------------------------------------------------------------------------------
void npTagEditMode( bool tagEditMode, void* dataRef )
{
	pData data = (pData) dataRef;

	pNPconsole console = &data->io.gl.hud.console;

	if( tagEditMode )
	{
		data->map.previousNode = data->map.currentNode;	//zz debug ???
					//zz could also restore console->mode to return to previous mode...
		console->mode = kNPconsoleTag;
		console->cursorShow = true;
	
		npConsole( data );			//clears prompt, curserShow, console->mode
		npUpdateConsoleUserText(console,data);
	//	npConsoleKeyEvent( kKeyCodeRight, kKeyDownSpecial, data );
	//	console->inputIndex--;
	//	console->inputIndex--;
	}
	else
	{
		//set to console msg mode
		console->mode = kNPconsoleMessage;
		console->cursorShow = false;
	}
}

//------------------------------------------------------------------------------
void npPickHUD (pNPnode node, void* dataRef)
{
	int temp = 0;
//	char msg[256];
	pData data = (pData) dataRef;


	switch (node->hudType)
	{
		case kNPhudCompass :
			node->childIndex = 1 - node->childIndex;						//zz debug
			if (node->childIndex)
				npPostMsg("Display Heading",kNPmsgCtrl,data);
			else
				npPostMsg("Display Compass",kNPmsgCtrl,data);
			break;
		case kNPhudAngle :	//change to angle								zz debug
			node->childIndex = 1 - node->childIndex;						//zz debug
			if (node->childIndex)
				npPostMsg("Display Ratio",kNPmsgCtrl,data);
			else
				npPostMsg("Display Tilt",kNPmsgCtrl,data);
			break;
		case kNPhudCoordX : 
			data->io.axes.x = 1 - data->io.axes.x;
			npPostAxes (data);
			break;
		case kNPhudCoordY :
			data->io.axes.y = 1 - data->io.axes.y;
			npPostAxes (data);
			break;
		case kNPhudCoordZ :
			data->io.axes.z = 1 - data->io.axes.z;
			npPostAxes (data);
			break;
		case kNPhudMode :
			data->io.mouse.pickMode++;
			if (data->io.mouse.pickMode >= kNPmodeCount)
				data->io.mouse.pickMode = kNPmodeNull + 1;
			if (data->io.mouse.pickMode == kNPmodeCamera)
			{
				npSelectNode(data->map.currentCam, data);
				data->io.mouse.tool = kNPtoolMove;			//zz select
			}
			if (data->io.mouse.pickMode == kNPmodeGrid)	//update to not need thes, zz debug
				npSelectNode(data->map.selectedGrid, data);
			if (data->io.mouse.pickMode == kNPmodePin)
				npSelectNode(data->map.selectedPinNode, data);
			npPostMode (node, data);
			npPostTool (NULL, data);
			break;
		case kNPhudTool :
			if (data->io.mouse.pickMode != kNPmodeCamera)
			{
				if (data->io.mouse.buttonR)
					data->io.mouse.tool--;
				else
					data->io.mouse.tool++;

				if (data->io.mouse.tool >= kNPtoolCount)
					data->io.mouse.tool = kNPtoolNull + 1;
				if (data->io.mouse.tool <= kNPtoolNull)
					data->io.mouse.tool = kNPtoolCount - 1;

				npPostTool (node, data);
			}
			break;
		case kNPhudSelect :									//zz select
			//if (data->io.mouse.pickMode != kNPmodePin)
				npSetMode( kNPmodePin, data );
			data->io.mouse.tool = kNPtoolSelect;
			npPostTool(NULL, data);
			break;
		case kNPhudSave :										//zzf
			if (data->io.mouse.pickMode != kNPmodeCamera)
			{
				npPostMsg("Save Selected Items", kNPmsgCtrl, data);
				temp = data->io.key.modAlt;
				data->io.key.modAlt = true;
				data->io.file.saveSelect = true;
		//		node->color.r = 255;
				npCtrlCommand(kNPcmdFileSave, data);
				data->io.key.modAlt = temp;
		//		data->io.file.saveSelect = false;
		//		node->color.r = 127;
	//			npPostMsg("Drag Mouse to Select Region", kNPmsgCtrl, data);
	//			npPostTool (node, data);
			}
			break;												//zzf end
	
		case kNPhudCreate :									//zz select
			if (data->io.mouse.pickMode != kNPmodePin)
				npSetMode( kNPmodePin, data );
			data->io.mouse.tool = kNPtoolCreate;
			npPostTool(NULL, data);
			break;
		case kNPhudLink :									//zz select
			if (data->io.mouse.pickMode != kNPmodePin)
				npSetMode( kNPmodePin, data );
			data->io.mouse.tool = kNPtoolLink;
			npPostTool(NULL, data);
			break;
		case kNPhudCombo :									//zz select
			if (data->io.mouse.pickMode != kNPmodePin)
				npSetMode( kNPmodePin, data );
			data->io.mouse.tool = kNPtoolCombo;
			npPostTool(NULL, data);
			break;
		case kNPhudMove :									//zz select
			if (data->io.mouse.pickMode != kNPmodePin)
				npSetMode( kNPmodePin, data );
			data->io.mouse.tool = kNPtoolMove;
			npPostTool(NULL, data);
			break;
		case kNPhudRotate :									//zz select
			if (data->io.mouse.pickMode != kNPmodePin)
				npSetMode( kNPmodePin, data );
			data->io.mouse.tool = kNPtoolRotate;
			npPostTool(NULL, data);
			break;
		case kNPhudSize :									//zz select
			if (data->io.mouse.pickMode != kNPmodePin)
				npSetMode( kNPmodePin, data );
			data->io.mouse.tool = kNPtoolSize;
			npPostTool(NULL, data);
			break;
		case kNPhudTopo:									//zz select
			if (data->io.mouse.pickMode != kNPmodePin)
				npSetMode( kNPmodePin, data );
			data->io.mouse.tool = kNPtoolTopo;
			npPostTool(NULL, data);
	//		npCtrlCommand(kNPcmdTool, data);
			break;
		case kNPhudGeo :									//zz select
			if (data->io.mouse.pickMode != kNPmodePin)
				npSetMode( kNPmodePin, data );
			data->io.mouse.tool = kNPtoolGeometry;
			npPostTool(NULL, data);
			break;
		case kNPhudColor :									//zz select
			if (data->io.mouse.pickMode != kNPmodePin)
				npSetMode( kNPmodePin, data );
			data->io.mouse.tool = kNPtoolColor;
			npPostTool(NULL, data);
			break;
		case kNPhudTexture :									//zz select
			if (data->io.mouse.pickMode != kNPmodePin)
				npSetMode( kNPmodePin, data );
			data->io.mouse.tool = kNPtoolTexture;
			npPostTool(NULL, data);
			break;
		case kNPhudHide :									//zz select
			if (data->io.mouse.pickMode != kNPmodePin)
				npSetMode( kNPmodePin, data );
			data->io.mouse.tool = kNPtoolHide;
			npPostTool(NULL, data);
			break;
		case kNPhudTag :									//zz select
			if (data->io.mouse.pickMode != kNPmodePin)
				npSetMode( kNPmodePin, data );
			data->io.mouse.tool = kNPtoolTag;	
			npPostTool(NULL, data);
			npTagEditMode( true, data );
			break;
		default : break;
	}
}

//set picked node params based on the mouse button and tool mode
//------------------------------------------------------------------------------
void npPickTool (pNPnode node, void* dataRef)
{
	char msg[256];

	pNPnode nodeTemp = NULL;
	pData data = (pData) dataRef;
	
	if( node->type == kNodeHUD )
	{
		if( data->io.mouse.tool == kNPtoolTag )
			npTagEditMode( false, data );		  //exits console tag edit mode //zz tag

		npPickHUD( node, data );
		return;
	}

	//camera mode locks out modifying nodes
	if (data->io.mouse.pickMode == kNPmodeCamera)
		return;

	switch (data->io.mouse.tool)
	{
		case kNPtoolCombo :			//in conjuction with picking
		case kNPtoolMove :			//in conjuction with picking
		case kNPtoolRotate :		//in conjuction with picking
		case kNPtoolSize :			//in conjuction with picking
		case kNPtoolSelect :
			if (data->io.mouse.buttonR)	 //zz select, debug click de-select and drag operation combo
				node->selected = 1 - node->selected;	//toggle value
			else
			{		
				//if node not selected then clear selection, then select
				if (!node->selected)
					npCtrlCommand (kNPcmdSelectNone, data);

				node->selected = true;
			}
			break;

		case kNPtoolCreate :
			if (!node->selected)
				npCtrlCommand (kNPcmdSelectNone, data);
			data->io.mouse.createEvent = true;							//zz-s debug
			break;
		// case kNPtoolNew : npCtrlCommand (kNPcmdNew, data); break;
		// case kNPtoolDelete : npCtrlCommand (kNPcmdDelete, data); break;
		case kNPtoolLink :
			if (node->type == kNodeLink)
			{
				npPostMsg ("Graph (edge) link ends must be nodes", kNPmsgCtrl, data);
				break;
			}
			if (data->io.mouse.linkA == NULL)				//first pick A
			{
				if (node->childCount < kNPnodeChildMax)
				{
					data->io.mouse.linkA = node;
					if (node->recordID)
						sprintf(msg,"Link node A selected id: %d record: %d - now pick B",
								node->id, node->recordID);
					else
						sprintf(msg,"Link node A selected id: %d - now pick B",
								node->id);
					npPostMsg (msg, kNPmsgCtrl, data);
				}
				else
				{
					sprintf(msg,"Cannot Link node A id: %d - exceeds child max",
							node->id);
					npPostMsg (msg, kNPmsgCtrl, data);
					npPostMsg ("Pick new Link A", kNPmsgCtrl, data);
					data->io.mouse.linkA = NULL;			//reset the process
				}
			}
			else											//create link
			{
				if (   node->childCount < kNPnodeChildMax
					&& data->io.mouse.linkA->childCount < kNPnodeChildMax)
				{
					node = npNodeNewLink (data->io.mouse.linkA, node, data);
					if (node != NULL)
					{
						sprintf(msg, "New Link id: %d from id: %d to id: %d", 
								node->id, node->parent->id, 
								node->child[node->childIndex]->id);
						npPostMsg (msg, kNPmsgCtrl, data);
					}
					data->io.mouse.linkA = NULL;				//reset the process
				}
				else
				{
					sprintf(msg,"Cannot Link node B id: %d - exceeds child max",
							node->id);
					npPostMsg (msg, kNPmsgCtrl, data);
					npPostMsg ("Pick different Link B - or R-click to Cancel", kNPmsgCtrl, data);
				}
			}
			break;
/*
		case kNPtoolTag :
			if (data->io.mouse.buttonR)
				node->tagMode = 0;
			else
				node->tagMode++;
			if (node->tagMode >= kNPtagModeCount)							//debug zz
				node->tagMode = 1;
		//	sprintf(msg, "Text Tag Mode: %d", node->tagMode);
			if (node->recordID)
				sprintf (msg, "record: %d", node->recordID);
			else
				sprintf (msg, "id: %d", node->id);
			npPostMsg (msg, kNPmsgCtrl, dataRef);
			npPostMsg ("Type any text and hit Enter to set the tag name", kNPmsgCtrl, dataRef);
			npPostMsg (">", kNPmsgCtrl, dataRef);
			break;
*/
		case kNPtoolTag :
			if (data->io.mouse.buttonR)
			{
				node->tagMode = 0;
				//zz debug add exit console here
				npTagEditMode( false, data );
			}
			else
			{
				if (data->io.gl.hud.console.mode != kNPconsoleTag)
				{
			//	
					npTagEditMode( true, data );

					//activate command console
					//	npConsole (dataRef);

				/*	if (node->recordID)
						sprintf (msg, "record: %d", node->recordID);
					else
						sprintf (msg, "id: %d", node->id);
				//	npPostMsg (msg, kNPmsgCtrl, dataRef);
				//	npPostMsg ("Type any text and hit Enter to set the tag name", kNPmsgCtrl, dataRef);
				*/
				}
				else if ( data->io.gl.hud.console.inputStr[0] != '\0' )
				{		//zz debug, this is a quick hack to workaround npconsole
					// apply any text left in the console to previous selection... //zz debug
					nodeTemp = data->map.currentNode;	
					data->map.currentNode = data->map.previousNode;
					
			//		npConsoleCmdText( &data->io.gl.hud.console, data );
					
					data->map.currentNode = nodeTemp;	//restore currentNode
				}

		//		node->tagMode++;
		//		if (node->tagMode >= kNPtagModeCount)							//debug zz
				if( data->map.currentNode == data->map.previousNode )
					node->tagMode++;
				if( !node->tagMode || node->tagMode >= kNPtagModeCount )
					node->tagMode = 1;

				//	sprintf(msg, "Text Tag Mode: %d", node->tagMode);
			//	npPostMsg ("a>", kNPmsgCtrl, dataRef);
			}
			break;

		case kNPtoolHide :
			if (data->io.mouse.buttonR)
			{
				npHideSubNodes (true, node, data);
				sprintf(msg, "Hide Branches");
			}
			else
			{
				npHideSubNodes (false, node, data);
				sprintf(msg, "Show Branches");
			}
			npPostMsg (msg, kNPmsgCtrl, dataRef);
			break;

		case kNPtoolFreeze : npCtrlCommand (kNPcmdFreeze, data); break;

		case kNPtoolSetpointHi :
			if (data->io.mouse.buttonR)
				npCtrlCommand (kNPcmdSetpointHiOff, data);
			else
				npCtrlCommand (kNPcmdSetpointHi, data);
			break;
		case kNPtoolSetpointLo :
			if (data->io.mouse.buttonR)
				npCtrlCommand (kNPcmdSetpointLoOff, data);
			else
				npCtrlCommand (kNPcmdSetpointLo, data);
			break;

		case kNPtoolTopo :
			if (data->io.mouse.buttonR)
				npCtrlCommand (kNPcmdTopoDown, data);
			else
				npCtrlCommand (kNPcmdTopo, data); 
			break;
		case kNPtoolGeometry :
			if (data->io.mouse.buttonR)
				npCtrlCommand (kNPcmdPrimitiveDown, data);
			else
				npCtrlCommand (kNPcmdGeometry, data);
			break;

//		case kNPtoolSegments :	npCtrlCommand (kNPcmdSegments, data); break;

		case kNPtoolColor :
			if (data->io.mouse.buttonR)
				npCtrlCommand (kNPcmdColorDown, data);
			else
				npCtrlCommand (kNPcmdColorUp, data);
			break;
		case kNPtoolAlpha :
			if (data->io.mouse.buttonR)
				npCtrlCommand (kNPcmdAlphaUp, data);
			else
				npCtrlCommand (kNPcmdAlphaDown, data);
			break;
		case kNPtoolTexture :
			if (data->io.mouse.buttonR)
				npCtrlCommand (kNPcmdTextureDown, data);
			else
				npCtrlCommand (kNPcmdTexture, data);
			break;
		case kNPtoolChannel :
			if (data->io.mouse.buttonR)
				npCtrlCommand (kNPcmdChannelDown, data);
			else
				npCtrlCommand (kNPcmdChannelUp, data);
			break;

		default : break;
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
//	char msg[256];

	GLubyte r,g,b;	//Unsigned Byte 8-bit Range: 0-255 // changed from u_int8_t, zz debug
	GLubyte pixels[4];		//4 Bytes
	
	pNPnode node = NULL;
	pData data = (pData) dataRef;
	
	if (data->io.mouse.buttonR 
		&& data->io.mouse.tool == kNPtoolLink
		&& data->io.mouse.linkA != NULL)
	{
		data->io.mouse.linkA = NULL;
		npPostMsg ("Cancelled, select a new Link A", kNPmsgCtrl, data);
		return;
	}

	//pickPass flag tells all object draw routines to use node ID mapped color
	data->io.gl.pickPass = true;

	//draw entire scene as flat 100% ambient with color mapped by node ID
	//think of it as a draw picture by numbers coloring book
	npGLDrawScene (data);			//optimize to draw only picked region, debug zz

		//upgrade to read a 9x9 region as to allow picking near an object, zz debug
		//then spiral out from the center looking for any object that is close
		//must handle edges of screen
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
			if ( data->io.gl.hud.console.mode == kNPconsoleTag )	//zz fixes bug 116
				npConsoleCmdText(&data->io.gl.hud.console, data);

			if (node->type != kNodeHUD)
			{
				if (node != data->map.currentNode)
				{
					//set active and selected node pointers unless in cam mode
					npSelectNode (node, data);

					//if recordID exists then display it, otherwise use node->id
			/*		if (node->recordID)
						sprintf(msg, "picked id: %d   recordID: %d", 
								node->id, node->recordID);
					else
						sprintf(msg, "picked id: %d", node->id);
					npPostMsg(msg, kNPmsgCtrl, data);
			*/
					if ( data->io.gl.hud.console.mode != kNPconsoleTag )	//zz tag
						npPostNodeID( node, data );
				}
				else
					npSelectNode (node, data);	//zz debug, remove this and test to verify ok
			}
			//applies action to nodes including HUD items, pins, cam, etc
			npPickTool (node, data);

			//refresh GUI command console prompt with picked item
			npConsolePrompt( &data->io.gl.hud.console, data );				//zz tag
		}
		else
			data->io.gl.pickID = 0;	//if invalid node then set to default camera
	}
	else if (data->io.mouse.tool == kNPtoolCreate 
				&& !data->io.mouse.buttonR
				&& data->io.mouse.pickMode != kNPmodeCamera)						 //zz-s debug, more testing!
	{
		data->io.mouse.createEvent = true; //used to create click, but not drag so cam works
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
	switch (console->level)
	{
		case kNPconsoleLevelOne :
			console->box.y = 1.0f * 15.0f + 8.0f;
			break;
		case kNPconsoleLevelThree :
			console->box.y = 3.0f * 15.0f + 8.0f;
			break;
		case kNPconsoleLevelMax :
			console->box.y = (float)kNPconsoleLineMax * 15.0f + 8.0f;
		break;
	}
}


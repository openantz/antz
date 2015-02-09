/* -----------------------------------------------------------------------------
*
*  npmouse.c
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

#include "npmouse.h"

#include "../npctrl.h"
#include "../npdata.h"
#include "../os/npos.h"

#include "npgl.h"

#include "watchtools.h"


#define kNPavgCount 10		//the number of cycles to average mouse movements

 
float npLowPassFilter( float start, float target, float fac );

//------------------------------------------------------------------------------
void npInitMouse (void* dataRef)
{
	pData data = (pData) dataRef;

	//zz-CAMERA
	//compare intialization values with working project, debug zz

	data->io.mouse.target.x = 0.0f;
	data->io.mouse.target.y = 0.0f;
	data->io.mouse.target.z = 0.0f;

	data->io.mouse.targetDest.x = 0.0f;
	data->io.mouse.targetDest.y = 0.0f;
	data->io.mouse.targetDest.z = 0.0f;

	data->io.mouse.targetRadius = 0.0f;
	data->io.mouse.targetRadiusDest = 0.0f;

	data->io.mouse.targeting = false;

	data->io.mouse.x = 0;						//raw coordinate
	data->io.mouse.y = 0;
	data->io.mouse.z = 0;						//typically the scroll wheel

	data->io.mouse.camMode = kNPmouseModeNull;
//	data->io.mouse.pickMode = kNPmodeNull;
//	data->io.mouse.pickMode = kNPmodeCamera;
	data->io.mouse.pickMode = kNPmodePin;	//needs to match npInitTools, zz debug
	data->io.mouse.tool = kNPtoolCombo;		//needs to match npInitTools, zz debug

/*
									 // mouse -> user -> tool -> selection
	data->ctrl.user.tool.select[id];
	data->ctrl.user[id];			//IO devices, input device mapping to tool modes and selection sets

	tool = user->tool[id];
	tool.type = kNPtoolSelect;

	data->io.mouse

	npDrawTool()
	{
		switch (tool->type) {
			case kNPtoolSelectRegion :
				switch (tool->shape) {
					case kNPgeoRectangle :		//pair of XY coords, map to either screen or world
						break;
					case kNPgeoEllipse :		// XY center, XY radius components
						break;
					case kNPgeoContour :		//line segments, maybe bezier curve too
						break;
				}
				break;
			case kNPtoolSelectVolume :
				switch (tool->shape) {
					case kNPgeoSphere :			// XYZ center, XYZ scale
						break;
					case kNPgeoCube :			//XYZ corner to corner
						break;
					case kNPgeoCone :			//  bottom center & radi, height
						break;
				}
				break;
		}
	}

	data->io.gl.hud.tool;
	user = data->io.user[userID];
	select = user->select[setID];
	tool = user->tool[];				//related to control
//	hud = user->hud[];					//draw drag rectangle, cube, sphere, sect planes...	
	tool->scale //translate geometry
*/
	data->io.mouse.buttonL = false;					//true when pressed
	data->io.mouse.buttonC = false;
	data->io.mouse.buttonR = false;

	data->io.mouse.window.x = 0;					//screen position in pixels
	data->io.mouse.window.y = 0;
	data->io.mouse.window.z = 0;

	data->io.mouse.previous.x = 0;
	data->io.mouse.previous.y = 0;
	data->io.mouse.previous.z = 0;

	data->io.mouse.cmDX = 0.0f;
	data->io.mouse.cmDY = 0.0f;

	data->io.mouse.delta.x = 0.0f;					//mouse vector
	data->io.mouse.delta.y = 0.0f;					//clears every mouse event
	data->io.mouse.delta.z = 0.0f;					//z is typically scroll wheel

	data->io.mouse.deltaSum.x = 0.0f;				//clears each NPE period
	data->io.mouse.deltaSum.y = 0.0f;
	data->io.mouse.deltaSum.z = 0.0f;

	data->io.mouse.pinSelected = false;

	data->io.mouse.createEvent = false;
	data->io.mouse.singleClick = false;
	data->io.mouse.doubleClick = false;

	data->io.mouse.linkA = NULL;
	
	data->io.mouse.size = sizeof(NPmouse);
}


//-----------------------------------------------------------------------------
void npCloseMouse (void* dataRef)
{
	return;
}


// we double buffer the mouse delta to maintain engine sync
// mouse position is averaged over several cycles to smooth
//-----------------------------------------------------------------------------
void npUpdateMouse (void* dataRef)
{
	int i = 0;
	float totalX = 0.0f;
	float totalY = 0.0f;
	static float x[kNPavgCount] = {0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f};
	static float y[kNPavgCount] = {0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f};
	static int count =0;

	pData data = (pData) dataRef;


	x[count] = data->io.mouse.deltaSum.x;
	y[count] = data->io.mouse.deltaSum.y;

	for (i=0; i < kNPavgCount; i++)
	{
		totalX += x[i];
		totalY += y[i];
	}

	data->io.mouse.delta.x = totalX / (float)kNPavgCount;
	data->io.mouse.delta.y = totalY / (float)kNPavgCount;

	if (data->io.mouse.camMode != kNPmouseModeCamFlyA)
		data->io.mouse.deltaSum.x = 0.0f;
	
	data->io.mouse.deltaSum.y = 0.0f;
	

	count++;
	if (count >= kNPavgCount)
		count = 0;
}


//deltaSums and stores the mouse delta coordinates
//------------------------------------------------------------------------------
void npMouseMotion (int x, int y)
{
	pData data = (pData) npGetDataRef();
	pNPnode tool = data->ctrl.user[0]->tool;

	static float localSum = 0.0f;

	//deltaSum mouse delta coords
	data->io.mouse.deltaSum.x += (GLfloat)(x - data->io.mouse.previous.x);
	data->io.mouse.deltaSum.y += (GLfloat)(y - data->io.mouse.previous.y);

	data->io.mouse.previous.x = x;
	data->io.mouse.previous.y = y;

	// works in conjuction with time based single click to satisfy power users
	// this is used to detect the difference between a click and drag operation
	// used by mouse tool Create
	// we want to see more then a pixel of movement to know the users intention
	// we square the delta to make the result positive
	localSum += data->io.mouse.deltaSum.x * data->io.mouse.deltaSum.x
				+ data->io.mouse.deltaSum.y * data->io.mouse.deltaSum.y;
	if ( localSum > 20.0f )	//requires about 5 pixels of movement
	{
		data->io.mouse.singleClick = false;
		data->io.mouse.createEvent = 0;			//used for Create tool, //zz-s
		localSum = 0.0f;
	}

	//	npSelectRegion(coordA, coordB, data);			//zz select
	if (data->io.mouse.tool == kNPtoolSelect)
	{
		tool->translateVec.x = (float)x - tool->translate.x;
		tool->translateVec.y = (float)(data->io.gl.height - y) - tool->translate.y;

		//reset select tool
		printf( "X: %4d  Y: %d\n", (int)tool->translateVec.x, (int)tool->translateVec.y );
	}

	//zz select
	if( tool->translateVec.x > 0.0f )
	{
		tool->setHi.x = tool->translate.x + tool->translateVec.x;
		tool->setLo.x = tool->translate.x;
	}
	else
	{
		tool->setHi.x = tool->translate.x;
		tool->setLo.x = tool->translate.x + tool->translateVec.x;
	}

	if( tool->translateVec.y > 0.0f )
	{
		tool->setHi.y = tool->translate.y + tool->translateVec.y;
		tool->setLo.y = tool->translate.y;
	}
	else
	{
		tool->setHi.y = tool->translate.y;
		tool->setLo.y = tool->translate.y + tool->translateVec.y;
	}
}

void npSetCamTarget (void* dataRef)
{
	float dX = 0.0f, dY = 0.0f, dZ = 0.0f;
	float cdX = 0.0f, cdY = 0.0f, cdZ = 0.0f;

	pData data = (pData) dataRef;

	pNPnode cam = data->map.currentCam;
	pNPnode node = data->map.selectedPinNode; //.node[data->map.selectedPinIndex];
	pNPnode rootGrid = data->map.node[kNPnodeRootGrid];
	pNPmouse mouse = (pNPmouse) &data->io.mouse;

	if (mouse->pickMode == kNPmodeGrid)									//zz debug
		node = data->map.selectedGrid;

	if (node == NULL)
		return;
/*
	mouse->targetDest.x = node->translate.x;
	mouse->targetDest.y = node->translate.y;
	mouse->targetDest.z = node->translate.z + kNPoffsetPin * node->scale.z;

	dX = cam->translate.x - mouse->targetDest.x;
	dY = cam->translate.y - mouse->targetDest.y;
	dZ = cam->translate.z - mouse->targetDest.z;

	mouse->targetRadius = sqrtf(dX*dX + dY*dY + dZ*dZ);

	//true on first pass
	if (1)//cam->proximity.x == 0.0f)
		cam->proximity.x = mouse->targetRadius;

	//set target based on radius
	cdX = -sinf(cam->rotate.y / kRADtoDEG) * cam->proximity.x;
	cdY = -cosf(cam->rotate.y / kRADtoDEG) * cam->proximity.x;
	cdZ = sinf(cam->rotate.x / kRADtoDEG) * cam->proximity.x;

	mouse->target.x = cdX + mouse->targetDest.x;
	mouse->target.y = cdY + mouse->targetDest.y;
	mouse->target.z = cdZ + mouse->targetDest.z;
*/

	mouse->targetDest.x = node->world.x;
	mouse->targetDest.y = node->world.y;
	mouse->targetDest.z = node->world.z;

	//set target of root pin or rod topo types to top of object
	//sub-node world coordinates are already corrected for the top of object
	if( node->branchLevel == 0)
	{
		if ( node->topo == kNPtopoPin 
			&& ( node->geometry == kNPgeoPin || node->geometry == kNPgeoPinWire ) )
			mouse->targetDest.z += kNPoffsetPin * node->scale.z;
		else if ( node->topo == kNPtopoRod ) 
			mouse->targetDest.z += kNPoffsetRod * node->scale.z;
	}

	dX = cam->translate.x - mouse->targetDest.x;
	dY = cam->translate.y - mouse->targetDest.y;
	dZ = cam->translate.z - mouse->targetDest.z;

	mouse->targetRadius = sqrtf(dX*dX + dY*dY + dZ*dZ);

	//true on first pass
	if (cam->proximity.x == 0.0f)
		cam->proximity.x = mouse->targetRadius;

	//set target based on radius
	cdX = -sinf(cam->rotate.y / kRADtoDEG) * cam->proximity.x;
	cdY = -cosf(cam->rotate.y / kRADtoDEG) * cam->proximity.x;
	cdZ = sinf((-(cam->rotate.x) + 90.0f) / kRADtoDEG) * cam->proximity.x;

	mouse->target.x = cdX + mouse->targetDest.x;
	mouse->target.y = cdY + mouse->targetDest.y;
	mouse->target.z = cdZ + mouse->targetDest.z;

	mouse->targeting = true;
	
/*	printf ("cdX: %f  %f  %f \n", cdX,cdY,cdZ);

	printf ("%f  %f  %f  r: %f  rD: %f\n", mouse->targetDest.x,
							mouse->targetDest.y,
							mouse->targetDest.z,
							mouse->targetRadius,
							cam->proximity.x);
*/
}


//------------------------------------------------------------------------------
int npHitTestHUD (int x, int y, void* dataRef)
{
	printf("%d  %d \n",x,y);
	if( x > 5 && x < 150 && y > 20 && y < 380)
		return 1;
	else
		return 0;
}

//------------------------------------------------------------------------------
void npMouseEvent (int button, int state, int x, int y)
{
	pData data = (pData) npGetDataRef();
	pNPnode cam = data->map.currentCam;
	pNPnode tool = data->ctrl.user[0]->tool;							//zz select
	pNPmouse mouse = &data->io.mouse;

	int restoreMode = 0;
	static double deltaTimeL = 0.0f, deltaTimeR = 0.0f;

	pNPnode node = NULL;												//zz-s debug
	char msg[256];
	char msgPart[256];

	//set previous current positiong and zero out the mouse delta
	mouse->previous.x = x;
	mouse->previous.y = y;			//probably causing dual button fly bug, zz debug
	mouse->delta.x = 0.0f;
	mouse->delta.y = 0.0f;
	mouse->cmDX = 0.0f;
	mouse->cmDY = 0.0f;

	//	glutWarpPointer(150,150);	//debug, zz


	//store the mouse button state, and detect single click
	if (button == GLUT_LEFT_BUTTON)
		if (state == GLUT_DOWN)
		{
			mouse->buttonL = true;
			deltaTimeL = nposGetTime();
			mouse->singleClick = false;
		}
		else
		{
			mouse->buttonL = false;
			if ( (nposGetTime() - deltaTimeL) < kNPsingleClickTime)
				mouse->singleClick = true;
		}

	if (button == GLUT_RIGHT_BUTTON)
		if (state == GLUT_DOWN)
		{
			mouse->buttonR = true;
			deltaTimeR = nposGetTime();
			mouse->singleClick = false;
		}
		else
		{
			mouse->buttonR = false;
			if ( (nposGetTime() - deltaTimeR) < kNPsingleClickTime)
				mouse->singleClick = true;
		}

	//zz select
	//select tool drag region
	if (data->io.mouse.tool == kNPtoolSelect)
	{
		if (mouse->singleClick)						//zz debug, might cause issues with create
		{
			tool->hide = true;

			//reset select tool
			//pick using current mouse position, invert y axis to screen coord
			npPick (x, data->io.gl.height - y, data);

			//if cam not picked then set cam target to the selected node
			if (data->io.gl.pickID != 0)
				npSetCamTarget (data);

			//if cam mode then set pickID = 0 to maintain cam selection
			if (mouse->pickMode == kNPmodeCamera)
				data->io.gl.pickID = 0;
		}
		else
		{
			if (state == GLUT_DOWN)// && !mouse->buttonR)	//drag from point A
			{
				tool->translate.x = (float)x;
				tool->translate.y = (float)(data->io.gl.height - y);
				tool->translateVec.x = 0.0f;
				tool->translateVec.y = 0.0f; 
				
				if( npHitTestHUD(x, y, data) == false )
					tool->hide = false;

				sprintf(msg, "Origin X: %4d  Y: %d", x, (int)tool->translate.y);
				npPostMsg(msg, kNPmsgCtrl, data);
			}
			else	//drag to point B
			{
				//tool = data->ctrl.user[0]->tool;
				//	npSelectRegion(coordA, coordB, data);
				tool->translateVec.x = (float)x - tool->translate.x;
				tool->translateVec.y = (float)(data->io.gl.height - y) - tool->translate.y;
				tool->hide = true;

				//reset select tool
				sprintf( msg, "Region W: %4d  H: %d", x - (int)tool->translate.x,
								(data->io.gl.height - y) - (int)tool->translate.y );
				npPostMsg(msg, kNPmsgCtrl, data);

			//	npSelectRegion(tool->translate, tool->translateVec, data);
				if( tool->translateVec.x > 0.0f )
				{
					tool->setHi.x = tool->translate.x + tool->translateVec.x;
					tool->setLo.x = tool->translate.x;
				}
				else
				{
					tool->setHi.x = tool->translate.x;
					tool->setLo.x = tool->translate.x + tool->translateVec.x;
				}

				if( tool->translateVec.y > 0.0f )
				{
					tool->setHi.y = tool->translate.y + tool->translateVec.y;
					tool->setLo.y = tool->translate.y;
				}
				else
				{
					tool->setHi.y = tool->translate.y;
					tool->setLo.y = tool->translate.y + tool->translateVec.y;
				}

				tool->triggerHi.x = true;	//used to flag selection process

			//	npPostMsg("Select Found: 0",kNPmsgCtrl,data);
			}
			return;
		} //Pick object if single button down, but not if two buttons pressed
	} // if (state == GLUT_UP && mouse->singleClick && !(mouse->buttonR || mouse->buttonL))	//zz select
	else if (state == GLUT_DOWN && !(mouse->buttonR && mouse->buttonL) )
	{
		//pick using current mouse position, invert y axis to screen coord
		npPick (x, data->io.gl.height - y, data);

		//if cam not picked then set cam target to the selected node
		if (data->io.gl.pickID != 0)
			npSetCamTarget (data);

		//if cam mode then set pickID = 0 to maintain cam selection
		if (mouse->pickMode == kNPmodeCamera)
			data->io.gl.pickID = 0;
	}

	//used to create click, but not drag so cam works
								//all of this should be moved over to npcrtl, or npnode, zz debug
	if (mouse->singleClick && data->io.mouse.createEvent)
	{
		mouse->singleClick = false;
		data->io.mouse.createEvent = false;

		if (state == GLUT_UP && button == GLUT_LEFT_BUTTON)
		{	
		//	data->io.mouse.createEvent = false;
			//clear the event state
			if (data->map.currentNode != data->map.selectedPinNode)
			{
				node = npNodeNew (kNodePin, NULL, data);					//zz-s debug messy clean this up
				npNodeNew (node->type, node, data);						//move this to npCtrl.h
				npSetCamTarget (data);	//sets camera target to selected node

				if (node->type == kNodeGrid)
					sprintf (msgPart, "New grid");
				if (node->type == kNodePin)
					sprintf (msgPart, "New pin");
				sprintf (msg, "%s id: %6d level: %d", msgPart, node->id, node->branchLevel + 1);
				npPostMsg (msg, kNPmsgCtrl, data);

				npPostMsg ("Mouse IF", kNPmsgCtrl, data);
			}
			else
			{
				node = data->map.currentNode;

				if	(node->branchLevel == 0)
				{
					node = npNodeNew (node->type, node, data);						//move this to npCtrl.h
					npSetCamTarget (data);	//sets camera target to selected node

					if (node->type == kNodeGrid)
						sprintf (msgPart, "New grid ");
					if (node->type == kNodePin)
						sprintf (msgPart, "New pin ");
					sprintf (msg, "%sid: %6d level: %d", msgPart, node->id, node->branchLevel + 1);
					npPostMsg (msg, kNPmsgCtrl, data);

					npPostMsg ("Mouse IF", kNPmsgCtrl, data);
				}
				else
					npCtrlCommand (kNPcmdNew, data);	
			}
		}
		else if (state == GLUT_UP && button == GLUT_RIGHT_BUTTON)
		{
	//		data->io.mouse.createEvent = false;
			npCtrlCommand (kNPcmdDelete, data);
		}
	}

	//calculate mouse operation mode
	if (data->io.gl.pickID == 0)	//picked background == camera
	{
		if (mouse->buttonL && !mouse->buttonR)		//left button
		{
			if (1)//mouse->camMode != kNPmouseModeCamFlyA)//not in fly mode
				mouse->camMode = kNPmouseModeCamExamXY;
			else
				mouse->camMode = kNPmouseModeCamLook;
		}

		if (mouse->buttonL && mouse->buttonR)		//both buttons
		{
			if (1)//mouse->camMode != kNPmouseModeCamFlyA )//not in fly mode
				mouse->camMode = kNPmouseModeCamExamXZ;

			npSetCamTarget (data);
		//	else
		//		mouse->camMode = kNPmouseModeCamLook;
		}

		if ( !mouse->buttonL && mouse->buttonR 			//right button
			&& ( mouse->camMode == kNPmouseModeNull 
				|| mouse->camMode == kNPmouseModeCamLook 
				|| mouse->camMode == kNPmouseModeCamFlyA ) )
		{
			mouse->camMode = kNPmouseModeCamFlyA;
			cam->proximity.x = 0.0f;			//prevents cam snapping back
		}

		// select the camera if not selected and button down
		if (state == GLUT_DOWN && data->map.currentCam != data->map.currentNode) //data->map.nodeRootIndex != kNPnodeRootCamera) //zz debug
		{
	//		restoreMode = mouse->pickMode;			//store pickMode	//zz debug
			npSelectNode (data->map.currentCam, data);		//select camera
	//		mouse->pickMode = restoreMode;			//restore pickMode
		}
	}
	else							//picked an object
	{
		if ( mouse->buttonL && !mouse->buttonR )	//left button
			mouse->camMode = kNPmouseModeDragXY;
		if ( mouse->buttonL && mouse->buttonR )		//both buttons
			mouse->camMode = kNPmouseModeDragXZ;
		if ( !mouse->buttonL && mouse->buttonR )	//right button
			mouse->camMode = kNPmouseModeDragXZ;
	}

	// if no buttons then restore selection, state == GLUT_UP
	if ( !mouse->buttonL && !mouse->buttonR )
	{	
		if (data->io.mouse.pickMode != kNPmodeCamera	//not in cam mode
			&& data->io.gl.pickID == 0)						//but moving a cam
		{	
			if (mouse->camMode != kNPmouseModeCamFlyA && mouse->camMode != kNPmouseModeCamLook )
			{
				if (data->io.mouse.pickMode == kNPmodeGrid)
					npCtrlCommand (kNPcmdGrid, data);
				else 
					//npCtrlCommand (kNPcmdPin, data);
					npSelectNode(data->map.selectedPinNode, data);
			}
			if ( //mouse->camMode == kNPmouseModeCamLook ||
				 mouse->camMode == kNPmouseModeCamFlyA )
			{
				cam->proximity.x = 0.0f;		//prevents cam snapping back
			}
		}

		mouse->camMode = kNPmouseModeNull;
	}
}

//------------------------------------------------------------------------------
void npMouseWheel (int wheel, int direction, int x, int y)
{
	pData data = (pData) npGetDataRef();

	//inverted direction, this way scrolling down goes down the list
	if (direction < 0)
		npCtrlCommand (kNPcmdTool, data);
	else
		npCtrlCommand (kNPcmdToolDown, data);
}


//------------------------------------------------------------------------------
float npLowPassFilter( float start, float target, float fac )
{
	return (float)(fac*target + (1.0f-fac)*start);
}

/*
//------------------------------------------------------------------------------
NPfloatXYZ lookVec(void* dataRef)
{
	pData data = (pData) dataRef;
	pNPnode camNode = data->map.currentCam;
	
	NPfloatXYZ look;
	NPfloatXYZ eCent = data->io.mouse.target; //examinerCenterDest;
	
	float dX = eCent.x - camNode->translate.x;
	float dY = eCent.y - camNode->translate.y;
	float dZ = eCent.z - camNode->translate.z;

	//normalize
	float len = (float)sqrt( dX*dX + dY*dY + dZ*dZ );
	dX /= len;
	dY /= len;
	dZ /= len;
	look.x = dX;
	look.y = dY;
	look.z = dZ;
	
	return look;
}

float angleBetweenVectors( float aX,float aY,  float bX, float bY )
{
	//	atan2(v2.y,v2.x) - atan2(v1.y,v1.x)
	float angle = atan2f( bY,bX ) - atan2f( aY,aX );
	return angle;
} 

void cartToSphere( float x, float y, float z, float* th, float* ph )
{
	float r;

	r = sqrtf( x*x + y*y + z*z );
	
//	*ph = acosf(  z/r );
	*ph = atan2f( sqrtf( x*x + y*y ), z );
	*th = atan2f( x,y );						//X & Y Swap
}
*/


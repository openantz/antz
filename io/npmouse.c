/* -----------------------------------------------------------------------------
*
*  npmouse.c
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

#include "npmouse.h"

#include "../npctrl.h"
#include "../npdata.h"

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

	data->io.mouse.translating = false;

	data->io.mouse.x = 0;						//raw coordinate
	data->io.mouse.y = 0;
	data->io.mouse.z = 0;						//typically the scroll wheel

	data->io.mouse.mode = kNPmouseModeNull;
//	data->io.mouse.pickMode = kNPpickModeNull;
	data->io.mouse.pickMode = kNPpickModePin;						//zz debug
	data->io.mouse.tool = kNPtoolNull;

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

	if (data->io.mouse.mode != kNPmouseModeCamFlyA)
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

	//deltaSum mouse delta coords
	data->io.mouse.deltaSum.x += (GLfloat)(x - data->io.mouse.previous.x);
	data->io.mouse.deltaSum.y += (GLfloat)(y - data->io.mouse.previous.y);

	data->io.mouse.previous.x = x;
	data->io.mouse.previous.y = y;
}

void npSetCamTarget (void* dataRef)
{
	float dX = 0.0f, dY = 0.0f, dZ = 0.0f;
	float cdX = 0.0f, cdY = 0.0f, cdZ = 0.0f;

	pData data = (pData) dataRef;

	NPnodePtr cam = data->map.activeCam;
	NPnodePtr node = data->map.node[data->map.selectedPinIndex];
	NPnodePtr rootGrid = data->map.node[kNPnodeRootGrid];
	pNPmouse mouse = (pNPmouse) &data->io.mouse;

	if (mouse->pickMode == kNPpickModeGrid)									//zz debug
		node = data->map.selectedGrid;

	if (node == NULL)
		return;
/*
	mouse->targetDest.x = node->translate.x;
	mouse->targetDest.y = node->translate.y;
	mouse->targetDest.z = node->translate.z + kNPpinHeight * node->scale.z;

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

	//set the target to the node, scale height based on pin and root grid
	mouse->targetDest.x = node->translate.x;
	mouse->targetDest.y = node->translate.y;
	mouse->targetDest.z = node->translate.z * rootGrid->scale.z 
						  + kNPpinHeight * node->scale.z;

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

	mouse->translating = true;
	
/*	printf ("cdX: %f  %f  %f \n", cdX,cdY,cdZ);

	printf ("%f  %f  %f  r: %f  rD: %f\n", mouse->targetDest.x,
							mouse->targetDest.y,
							mouse->targetDest.z,
							mouse->targetRadius,
							cam->proximity.x);
*/
}


//------------------------------------------------------------------------------
void npMouseEvent (int button, int state, int x, int y)
{
	int restoreMode = 0;

	pData data = (pData) npGetDataRef();
	NPnodePtr cam = data->map.activeCam;
	pNPmouse mouse = &data->io.mouse;


	//set previous current positiong and zero out the mouse delta
	mouse->previous.x = x;
	mouse->previous.y = y;	
	mouse->delta.x = 0.0f;
	mouse->delta.y = 0.0f;
	mouse->cmDX = 0.0f;
	mouse->cmDY = 0.0f;

	//	glutWarpPointer(150,150);	//debug, zz


	//store the mouse button state
	if (button == GLUT_LEFT_BUTTON)
		if (state == GLUT_DOWN)
			mouse->buttonL = true;
		else
			mouse->buttonL = false;

	if (button == GLUT_RIGHT_BUTTON)
		if (state == GLUT_DOWN)
			mouse->buttonR = true;
		else
			mouse->buttonR = false;

	//Pick object if single button down, but not if two buttons pressed
	if (state == GLUT_DOWN && !(mouse->buttonR && mouse->buttonL) )
	{
		//if not in cam mode then do PickPass
	//	if (mouse->pickMode != kNPpickModeCamera)
		{
			npPick (x, data->io.gl.height - y, data); //invert y to screen coord

			//if cam not picked then set cam target to the selected node
			if (data->io.gl.pickID != 0)
				npSetCamTarget (data);
		}
		if (mouse->pickMode == kNPpickModeCamera)
			data->io.gl.pickID = 0;
	}

	//calculate mouse operation mode
	if (data->io.gl.pickID == 0)	//picked background == camera
	{
		if (mouse->buttonL && !mouse->buttonR)		//left button
		{
			if (mouse->mode != kNPmouseModeCamFlyA)//not in fly mode
				mouse->mode = kNPmouseModeCamExamXY;
			else
				mouse->mode = kNPmouseModeCamLook;
		}

		if (mouse->buttonL && mouse->buttonR)		//both buttons
		{
			if (mouse->mode != kNPmouseModeCamFlyA )//not in fly mode
				mouse->mode = kNPmouseModeCamExamXZ;
			else
				mouse->mode = kNPmouseModeCamLook;
		}

		//if free fly mode then overide by setting target to current position 
		//this results in a mouse look mode
		if (mouse->mode == kNPmouseModeCamLook)
		{
										//zz debug jerry-rigged
			data->io.gl.pickID = 0;
		}

		if ( !mouse->buttonL && mouse->buttonR 			//right button
			&& ( mouse->mode == kNPmouseModeNull 
				|| mouse->mode == kNPmouseModeCamLook 
				|| mouse->mode == kNPmouseModeCamFlyA ) )
		{
			mouse->mode = kNPmouseModeCamFlyA;
			cam->proximity.x = 0.0f;			//prevents cam snapping back
		}

		// select the camera if not selected and button down
		if (state == GLUT_DOWN && data->map.nodeRootIndex != kNPnodeRootCamera)
		{
	//		restoreMode = mouse->pickMode;			//store pickMode	//zz debug
			npSelectNode (data->map.activeCam, data);		//select camera
	//		mouse->pickMode = restoreMode;			//restore pickMode
		}
	}
	else							//picked an object
	{
		if ( mouse->buttonL && !mouse->buttonR )	//left button
			mouse->mode = kNPmouseModeDragXY;
		if ( mouse->buttonL && mouse->buttonR )		//both buttons
			mouse->mode = kNPmouseModeDragXZ;
		if ( !mouse->buttonL && mouse->buttonR )	//right button
			mouse->mode = kNPmouseModeDragXZ;
	}

	// if no buttons then restore selection, state == GLUT_UP
	if ( !mouse->buttonL && !mouse->buttonR )
	{	
		if (data->io.mouse.pickMode != kNPpickModeCamera	//not in cam mode
			&& data->io.gl.pickID == 0)						//but moving a cam
		{	
			if (mouse->mode != kNPmouseModeCamFlyA && mouse->mode != kNPmouseModeCamLook )
			{
				if (data->io.mouse.pickMode == kNPpickModeGrid)
					npCtrlCommand (kNPcmdGrid, data);
				else 
					//npCtrlCommand (kNPcmdPin, data);
					npSelectNode(data->map.selectedPinNode, data);
			}
			if ( //mouse->mode == kNPmouseModeCamLook ||
				 mouse->mode == kNPmouseModeCamFlyA )
			{
				cam->proximity.x = 0.0f;		//prevents cam snapping back
			}
		}

		mouse->mode = kNPmouseModeNull;
	}
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
	NPnodePtr camNode = data->map.activeCam;
	
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


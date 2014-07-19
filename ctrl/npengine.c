/* -----------------------------------------------------------------------------
*
*  npengine.c
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

#include "npengine.h"

#include "../io/npgl.h"
#include "../npdata.h"
#include "../npio.h"
#include "../io/npmouse.h"

void UpdateNode (NPnodePtr node, pData dataRef);
void UpdateNodeChild (NPnodePtr node, pData dataRef);

void UpdateNodeDefault (NPnodePtr node, pData dataRef);
void UpdateNodeCamera (NPnodePtr node, pData dataRef);
void UpdateNodeSurface (NPnodePtr node, pData dataRef);
void UpdateNodePoints (NPnodePtr node, pData dataRef);
void UpdateNodePin (NPnodePtr node, pData dataRef);
void UpdateNodeVideo (NPnodePtr node, pData dataRef);
void UpdateNodeGrid (NPnodePtr node, pData dataRef);
void UpdateNodeData (NPnodePtr node, pData dataRef);

void UpdateNodeWithMouse( NPnodePtr node, pData dataRef);	//zz-CAMERA

//------------------------------------------------------------------------------
void npInitEngine (void* dataRef)
{
	return;
}


//------------------------------------------------------------------------------
void npCloseEngine (void* dataRef)
{
	return;
}


// traverses all nodes, updates translation, rotation, scale, color, physics...
//------------------------------------------------------------------------------
void npUpdateEngine (void* dataRef)
{
	int i = 0;

	pData data = (pData) dataRef;

	NPnodePtr node = NULL;


	//start at 1 and skip the kNPnodeRootNull, temp workaround				debug zz
	for (i=1; i < data->map.nodeRootCount; i++)
	{
		node = data->map.node[i];

		if (node->childCount)
			UpdateNodeChild(node, dataRef);					// recursive function

//		UpdateNodeData (node, dataRef);		//replaced by npch.h			debug zz
		UpdateNode (node, dataRef);
	}
}

// recursively calls this function to traverse entire tree
//------------------------------------------------------------------------------
void UpdateNodeChild (NPnodePtr node, pData dataRef)
{
	int i = 0;

	for (i=0; i < node->childCount; i++)
	{
		UpdateNodeChild (node->child[i], dataRef);			// recursively call function

		UpdateNodeData (node->child[i], dataRef);

		UpdateNode(node->child[i], dataRef);
	}
}


//------------------------------------------------------------------------------
//zz-JJ this is being replaced by your ptr to ptr method
void UpdateNodeData (NPnodePtr node, pData dataRef)
{
	float colorShift = 0;
	float scale = 0;
	NPubyteRGBA colorTmp;

	pData data = (pData) dataRef;

	if (node->colorShift == 0.0f)	//do not update if no color shift
		return;

	if (node->freeze)	//do not update if frozen
		return;

	colorTmp.r = 127;
	colorTmp.g = 127;
	colorTmp.b = 127;
	colorTmp.a = 127;

	//  retrieve original color from index
	SetIndexColor (&node->color, &node->colorIndex);

	if (colorShift < 0.0f)
		colorShift = 0.0f;
	if (colorShift > 255.0f)				//zz debug, update this to work from -180 to 180
		colorShift = 255.0f;

	colorTmp.r = (GLubyte) ((GLfloat)node->color.g * (colorShift / 255.0f) 
				+ (GLfloat)node->color.r * ( 1.0f - (colorShift / 255.0f)));
	colorTmp.g = (GLubyte) ((GLfloat)node->color.b * (colorShift / 255.0f)
				+ (GLfloat)node->color.g * ( 1.0f - (colorShift / 255.0f)));
	colorTmp.b = (GLubyte) ((GLfloat)node->color.r * (colorShift / 255.0f)
				+ (GLfloat)node->color.b * ( 1.0f - (colorShift / 255.0f)));

	node->color.r = colorTmp.r;
	node->color.g = colorTmp.g;
	node->color.b = colorTmp.b;
}


//------------------------------------------------------------------------------
void UpdateNode (NPnodePtr node, pData dataRef)
{
	if (node->freeze)			//do not update if frozen
		return;

	switch (node->type)
	{
		case kNodeDefault : UpdateNodeDefault (node, dataRef); break;
		case kNodeCamera : UpdateNodeCamera (node, dataRef); break;
		case kNodeSurface : UpdateNodeSurface (node, dataRef); break;
		case kNodePoints : UpdateNodePoints (node, dataRef); break;
		case kNodePin : UpdateNodePin (node, dataRef); break;
		case kNodeVideo : UpdateNodeVideo (node, dataRef); break;
		case kNodeGrid : UpdateNodeGrid (node, dataRef); break;
	}
}


//------------------------------------------------------------------------------
void UpdateNodeDefault (NPnodePtr node, pData dataRef)
{
	float i = 0.0f;

	NPnodePtr parent = node->parent;
	pData data = (pData) dataRef;

	// if selected update node with mouse movement
	if ( ( node->selected || node == data->map.currentNode ) 
		&& data->map.nodeRootIndex >= kNPnodeRootPin )		//update for grid, debug zz
	{
		UpdateNodeWithMouse (node, dataRef);
	}

	//update translation
	node->translate.x += node->translateRate.x;
	node->translate.y += node->translateRate.y;
	node->translate.z += node->translateRate.z;

	//update rotation about XYZ axis and keep inbounds of 2 PI
	node->rotate.x += node->rotateRate.x;	// * 2.0f;	// * 2 to make faster then cam
	node->rotate.y += node->rotateRate.y;
	node->rotate.z += node->rotateRate.z;

	//update XYZ scale
	node->scale.x *= 1.0f + node->scaleRate.x;
	node->scale.y *= 1.0f + node->scaleRate.y;
	node->scale.z *= 1.0f + node->scaleRate.z;

	//apply node specific limit such as not going below the surface
	if (node->triggerLo.x)
		if (node->translate.x < node->setLo.x)											// debug, zz
			node->translate.x = node->setLo.x;
	if (node->triggerHi.x)
		if (node->translate.x > node->setHi.x)											// debug, zz
			node->translate.x = node->setHi.x;
	if (node->triggerLo.y)
		if (node->translate.y < node->setLo.y)											// debug, zz
			node->translate.y = node->setLo.y;
	if (node->triggerHi.y)
		if (node->translate.y > node->setHi.y)											// debug, zz
			node->translate.y = node->setHi.y;
	if (node->triggerLo.z)
		if (node->translate.z < node->setLo.z)											// debug, zz
			node->translate.z = node->setLo.z;
	if (node->triggerHi.z)
		if (node->translate.z > node->setHi.z)											// debug, zz
			node->translate.z = node->setHi.z;

	//rotation limits
	if (node->rotate.x > 180.0f)
		node->rotate.x = 180.0f;			// no rollover
	if (node->rotate.x < 0.0f)
		node->rotate.x = 0.0f;
	if (node->rotate.y > 360.0f)
		node->rotate.y -= 360.0f;			// rollover
	if (node->rotate.y < 0.0f)
		node->rotate.y += 360.0f;
	if (node->rotate.z > 180.0f)
		node->rotate.z -= 360.0f;			// rollover
	if (node->rotate.z < -180.0f)
		node->rotate.z += 360.0f;

	//apply topo specific limits, after node limits
	if (parent != NULL)
	{
		if (parent->topo == kNPtopoCube)
		{
			if (node->translate.x > 180.0f)
			{									//invert rate at top & bottom, debug zz
				node->translate.x += -360.0f;
				switch (node->facet)
				{
					case 1 : node->facet = 3; break;
					case 2 : node->facet = 4; break;
					case 3 : node->facet = 2; break;
					case 4 : node->facet = 1; break;
					case 5 : node->facet = 1; break;
					case 6 : node->facet = 2; break;
					default : node->facet = 3; break;	//if 0 assume facet 1
											//perhaps assign facet number if 0, debug zz	
				}
				printf ("id: %d   facet: %d\n", node->id, node->facet);
			}
			else if (node->translate.x < -180.0f)
			{	
				node->translate.x += 360.0f;
				switch (node->facet)
				{
					case 1 : node->facet = 4; break;
					case 2 : node->facet = 3; break;
					case 3 : node->facet = 1; break;
					case 4 : node->facet = 2; break;
					case 5 : node->facet = 2; break;
					case 6 : node->facet = 1; break;
					default : node->facet = 4; break;	//if 0 assume facet 1					
				}
				printf ("id: %d   facet: %d\n", node->id, node->facet);
			}

			if (node->translate.y > 180.0f)
			{
				node->translate.y += -360.0f;
				if (node->facet > 4)
					node->facet = 3;
				else
					node->facet = 5;
				printf ("id: %d   facet: %d\n", node->id, node->facet);
			}
			else if (node->translate.y < -180.0f)
			{
				node->translate.y += 360.0f;
				if (node->facet > 4)
					node->facet = 4;
				else
					node->facet = 6;
				printf ("id: %d   facet: %d\n", node->id, node->facet);
			}
		}
		else if (node->topo == kNPtopoSphere || node->topo == 0 
				|| node->topo == kNPtopoTorus || parent->topo == kNPtopoPin)
		{
			if (node->translate.x > 180.0f)
				node->translate.x += -360.0f;
			if (node->translate.x < -180.0f)
				node->translate.x += 360.0f;
			if (node->translate.y > 180.0f)
				node->translate.y += -360.0f;
			if (node->translate.y < -180.0f)
				node->translate.y += 360.0f;
		}
			//add triangle boundary to support more topo types,			debug zz
	}
}


//------------------------------------------------------------------------------
void npCameraTranslate (void* nodeRef, void* dataRef )
{
	pData data = (pData) dataRef;
	NPnodePtr cam = (NPnodePtr) nodeRef;
	NPnodePtr targetNode = data->map.node[data->map.selectedPinIndex];
	NPnodePtr rootGrid = data->map.node[kNPnodeRootGrid];

	float radius = 0.0f;
	float cdX = 0.0f, cdY = 0.0f, cdZ = 0.0f;
	float tX = 0.0f, tY = 0.0f, tZ = 0.0f;
	float dX = 0.0f, dY = 0.0f, dZ = 0.0f;

	if (data->io.mouse.pickMode == kNPpickModeGrid)									//zz debug
		targetNode = data->map.selectedGrid;

	//global position of entire scene is relative to the root grid scale.z
	//cam target factors in root grid scale.z and pin scale.z times pin height
	if (targetNode != NULL)
	{
		data->io.mouse.targetDest.x = targetNode->translate.x;
		data->io.mouse.targetDest.y = targetNode->translate.y;
		data->io.mouse.targetDest.z = targetNode->translate.z * rootGrid->scale.z
									+ kNPpinHeight * targetNode->scale.z;
	}
	
	if (data->io.mouse.mode == kNPmouseModeCamExamXZ)
		cam->proximity.x *= 1.0f + data->io.mouse.delta.y * 0.005f;

	// set target based on radius, KML compliant, invert and 90 deg offset for rotate.x
	cdX = -sinf(cam->rotate.y / kRADtoDEG) * cam->proximity.x * cosf((cam->rotate.x - 90.0f) / kRADtoDEG);
	cdY = -cosf(cam->rotate.y / kRADtoDEG) * cam->proximity.x * cosf((cam->rotate.x - 90.0f) / kRADtoDEG);
	cdZ = sinf((-90.0f - cam->rotate.x) / -kRADtoDEG) * cam->proximity.x;

	data->io.mouse.target.x = npLowPassFilter ( data->io.mouse.target.x, data->io.mouse.targetDest.x, 0.05f );
	data->io.mouse.target.y = npLowPassFilter ( data->io.mouse.target.y, data->io.mouse.targetDest.y, 0.05f );
	data->io.mouse.target.z = npLowPassFilter ( data->io.mouse.target.z, data->io.mouse.targetDest.z, 0.05f );

	tX = cdX + data->io.mouse.targetDest.x;
	tY = cdY + data->io.mouse.targetDest.y;
	tZ = cdZ + data->io.mouse.targetDest.z;
//	tX = cdX + data->io.mouse.target.x;
//	tY = cdY + data->io.mouse.target.y;
//	tZ = cdZ + data->io.mouse.target.z;

	dX = cam->translate.x - tX;
	dY = cam->translate.y - tY;
	dZ = cam->translate.z - tZ;

	radius = sqrtf (dX*dX+dY*dY+dZ*dZ);
	if (radius < 0.1f)	//clamp
		data->io.mouse.translating = false;

	if (data->io.mouse.translating)
	{
		cam->translate.x = npLowPassFilter ( cam->translate.x, tX, 0.05f );
		cam->translate.y = npLowPassFilter ( cam->translate.y, tY, 0.05f );
		cam->translate.z = npLowPassFilter ( cam->translate.z, tZ, 0.05f );
	}
	else
	{
		cam->translate.x = tX;
		cam->translate.y = tY;
		cam->translate.z = tZ;
	}
}


//------------------------------------------------------------------------------
void npUpdateCameraMouse (void* nodeRef, void* dataRef )
{
	float sinY = 0.0f, cosY = 0.0f;

	pData data = (pData) dataRef;
	NPnodePtr node = (NPnodePtr) nodeRef;
	pNPmouse mouse = (pNPmouse) &data->io.mouse;

	switch (data->io.mouse.mode)
	{
		case kNPmouseModeCamLook :
			node->rotate.x += mouse->delta.y * -0.085f;	//invert y-axis, debug zz
			node->rotate.y += mouse->delta.x * 0.2f;
			break;

		case kNPmouseModeCamFlyA :
			// left-right view rotation
			node->rotate.y += mouse->delta.x * 0.0015f;
			
			// translation based on delta.y and view rotation
			mouse->cmDY += mouse->delta.y * -0.002f; //invert axis

			sinY = (float) sin(node->rotate.y / kRADtoDEG);
			cosY = (float) cos(node->rotate.y / kRADtoDEG);

			node->translate.x += mouse->cmDY * sinY;
			node->translate.y += mouse->cmDY * cosY;
			break;
		
		case kNPmouseModeCamFlyB : break;

		case kNPmouseModeDragXZ : break;
		case kNPmouseModeDragXY : break;
		case kNPmouseModeDragWorld : break;

		case kNPmouseModeCamExamXZ :
			node->rotate.y += mouse->delta.x * 0.2f;
			break;
		case kNPmouseModeCamExamXY :
			node->rotate.x += mouse->delta.y * -0.2f;	//invert axis
			node->rotate.y += mouse->delta.x * 0.2f;
			break;

		default :
			break;
	}
}


//zz-CAMERA
// camera updated relative to the viewport x y plane
// x tilts up & down, y looks left & right, z rolls the camera
// z may be fixed or not, defaults to horizon vs flightsim roll
// translateRate x pans left & right, y for up & down, and z for forward & back
//------------------------------------------------------------------------------
void UpdateNodeCamera (NPnodePtr node, pData dataRef)
{
	float sinX = 0.0f, cosX = 0.0f;
	float sinY = 0.0f, cosY = 0.0f;
	float sinZ = 0.0f, cosZ = 0.0f;

	pData data = (pData) dataRef;

	// update active camera with mouse movement
	if (data->map.currentNode == node)
		npUpdateCameraMouse (node, data);			//zz-CAMERA

	if (node == data->map.activeCam && node->proximity.x != 0.0f	//do we need to check radius, debug, zz
		&& data->io.mouse.mode != kNPmouseModeCamFlyA)
		npCameraTranslate(node,data);

	// update rotation about XYZ axis and keep inbounds of 2 PI radians
	node->rotate.x += node->rotateRate.x;
	if (node->rotate.x > 180.0f)	// limit to +/- 90 deg
		node->rotate.x = 180.0f;
	if (node->rotate.x < 0.0f)
		node->rotate.x = 0.0f;

	node->rotate.y += node->rotateRate.y;
	if (node->rotate.y > 360.0f)
		node->rotate.y += -360.0f;
	if (node->rotate.y < 0.0f)
		node->rotate.y += 360.0f;

	node->rotate.z += node->rotateRate.z;
	if (node->rotate.z > 180.0f)
		node->rotate.z += -360.0f;
	if (node->rotate.z < -180.0f)
		node->rotate.z += 360.0f;
/*
		node->rotate.x += node->rotateRate.x;
	if (node->rotate.x >= kPos2PI / 4.0f)	// limit to +/- 90 deg
		node->rotate.x = kPos2PI / 4.0f;
	if (node->rotate.x <= kNeg2PI / 4.0f)
		node->rotate.x = -kPos2PI / 4.0f;

	node->rotate.y += node->rotateRate.y;
	if (node->rotate.y >= kPos2PI)
		node->rotate.y -= kPos2PI;
	if (node->rotate.y <= kNeg2PI)
		node->rotate.y += kPos2PI;

	node->rotate.z += node->rotateRate.z;
	if (node->rotate.z >= kPos2PI)
		node->rotate.z -= kPos2PI;
	if (node->rotate.z < 0.0f)
		node->rotate.z += kPos2PI;
*/
	//calculate the rotation vector				// optimize with lookup tables, zz
	sinX = sinf( (-node->rotate.x + 90.0f) / kRADtoDEG);
	cosX = cosf( (-node->rotate.x + 90.0f) / kRADtoDEG);
	sinY = sinf(node->rotate.y / kRADtoDEG);
	cosY = cosf(node->rotate.y / kRADtoDEG);
//	sinZ = sinf(node->rotate.z / kRADtoDEG);			//not implemented yet, debug zz
//	cosZ = cosf(node->rotate.z / kRADtoDEG);

	//stores as unit vector with angle
	node->rotateVec.x = sinY * cosX;
	node->rotateVec.y = cosY * cosX;
	node->rotateVec.z = -sinX;
	node->rotateVec.angle = node->rotate.z;

	//update XYZ scale, effects ortho view and z changes FOV for perspective, debug zz
	node->scale.x += node->scaleRate.x;	//change to *= so scaling is relative, zz
	node->scale.y += node->scaleRate.y;
	node->scale.z += node->scaleRate.z;
	
	//update the translateVec for selected objects
	node->translateVec.x = sinY * node->translateRate.y
						  + cosY * node->translateRate.x;

	node->translateVec.y = cosY * node->translateRate.y
						  - sinY * node->translateRate.x;

	//z ignores the angles just move in global coordinates, 0.5 to reduce speed
	node->translateVec.z = node->translateRate.z * 0.5f;

	//update the position by adding the vector to existing position
	node->translate.x += node->translateVec.x;
	node->translate.y += node->translateVec.y;
	node->translate.z += node->translateVec.z;


	//limit from going below the surface
	if (node->triggerLo.z)
		if (node->translate.z < node->setLo.z)											// debug, zz
			node->translate.z = node->setLo.z;

	//limit from going below the surface
	if (node->triggerHi.z)
		if (node->translate.z > node->setHi.z)											// debug, zz
			node->translate.z = node->setHi.z;
}


//------------------------------------------------------------------------------
void UpdateNodeSurface (NPnodePtr node, pData dataRef)
{
	UpdateNodeDefault (node, dataRef);
}


//------------------------------------------------------------------------------
void UpdateNodePoints (NPnodePtr node, pData dataRef)
{
	UpdateNodeDefault (node, dataRef);
}


//------------------------------------------------------------------------------
void UpdateNodePin (NPnodePtr node, pData dataRef)
{
	pData data = (pData) dataRef;

	if (1)//node->branchLevel == 0)
	{
		UpdateNodeDefault (node, dataRef);
		return;
	}
	
	// update mouse movement if selected
	if ( ( node->selected || node == data->map.currentNode ) 
		&& data->map.nodeRootIndex >= kNPnodeRootPin )
	{
		if (data->io.mouse.buttonR)
		{
			node->ratio += data->io.mouse.delta.x * -0.005f;
			if (node->ratio < 0.01f)
				node->ratio = 0.01f;
			if (node->ratio > 1.0f)
				node->ratio = 1.0f;

			if (data->io.axes.x)
				node->scale.x += data->io.mouse.delta.y * 0.01f;		// invert the Y axes	
			if (data->io.axes.y)
				node->scale.y += data->io.mouse.delta.y * 0.01f;		// insert check for active axes, debug, zz
			if (data->io.axes.z)
				node->scale.z += data->io.mouse.delta.y * 0.01f;
		}
		else
		{
			node->rotate.x += data->io.mouse.delta.x * -0.01f;	// invert for CW
			node->rotate.y += data->io.mouse.delta.y * -0.01f;
		}
	}

	node->rotate.x += node->rotateRate.x * 2.0f;	// * 2 to make faster then cam
	node->rotate.y += node->rotateRate.y * 2.0f;

	if (node->rotate.x >= kPos2PI)
		node->rotate.x -= kPos2PI;
	if (node->rotate.x <= kNeg2PI)
		node->rotate.x += kPos2PI;

	if (node->rotate.y >= kPos2PI)
		node->rotate.y -= kPos2PI;
	if (node->rotate.y <= kNeg2PI)
		node->rotate.y += kPos2PI;

	node->rotateVec.angle = node->rotate.x * kRADtoDEG;		//debug, zz

	node->translate.x += node->translateRate.x * 0.1f;		//reduce velocity to 1/10
	node->translate.y += node->translateRate.y * 0.1f;
	node->translate.z += node->translateRate.z * 0.1f;
//	node->translate.y += node->translateRate.y;	//irrelevant until new topologies
//	node->translate.z += node->translateRate.z;	

	//update XYZ scale
	node->scale.x += node->scaleRate.x;
	node->scale.y += node->scaleRate.y;
	node->scale.z += node->scaleRate.z;
}


//------------------------------------------------------------------------------
void UpdateNodeVideo (NPnodePtr node, pData dataRef)
{
	UpdateNodeDefault (node, dataRef);
}


//------------------------------------------------------------------------------
void UpdateNodeGrid (NPnodePtr node, pData dataRef)
{
	pData data = (pData) dataRef;

	//lock out translation and rotationof root grid, just update XYZ scale
	if (node == data->map.node[kNPnodeRootGrid])
	{
		node->scale.x = node->scale.x * (1.0f + node->scaleRate.x);
		node->scale.y = node->scale.y * (1.0f + node->scaleRate.y);
		node->scale.z = node->scale.z * (1.0f + node->scaleRate.z);
	}
	else
		UpdateNodeDefault (node, dataRef);
}

//------------------------------------------------------------------------------
//MB-CAMERA//
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
void UpdateNodeWithMouse (NPnodePtr node, pData dataRef)
{
	float distFactor = 1.0f;
	float dX = 0.0f, dY = 0.0f, dZ = 0.0f;
	float sinY = 0.0f, cosY = 0.0f;

	pData data = (pData) dataRef;
	NPnodePtr cam = data->map.activeCam;	//get the current camera
	NPnodePtr parent = node->parent;

	pNPmouse mouse = (pNPmouse) &data->io.mouse;

	if (mouse->mode == kNPmouseModeNull)	//fixes mouse slip bug
		return;

	sinY = sinf(cam->rotate.y / kRADtoDEG);
	cosY = cosf(cam->rotate.y / kRADtoDEG);

	// different factor for X and Y helps with the feel with typical widescreen
	dX = data->io.mouse.delta.x * 0.012f;

	// apply the delta.y to the chosen axes
	if (data->io.mouse.mode == kNPmouseModeDragXZ)
		dZ = data->io.mouse.delta.y * -0.022f;		//invert the axis
	else
		dY = data->io.mouse.delta.y * 0.022f;
	
	//data->io.mouse.mode
	//parent->topo && node->branchLevel
	if (node->branchLevel == 0)			//default
	{
		distFactor = cam->proximity.x * 0.1f; // movement scaled by distance to cam

		// update the translateVec for selected objects
		// optimize by calculating once (at camera?) and use for all nodes, debug, zz
		node->translate.x += ( cosY * dX - sinY * dY ) * distFactor;			
		node->translate.y += ( -sinY * dX - cosY * dY ) * distFactor;
		node->translate.z += dZ * distFactor;
	}
	else
	{
		if (parent == NULL)
		{
			printf ("err 2458 - id: %d parent node is NULL\n", node->id);
			return;
		}
/*
		switch (parent->topo)
		{
//			case kNPtopoNull : break;	//topo based on geometery type, default
			case kNPtopoCube : break;
			case kNPtopoSphere : break;
			case kNPtopoTorus : break;
			case kNPtopoSurface : break;
			case kNPtopoPin : break;
			case kNPtopoGrid : break;

			default : 
				distFactor = cam->proximity.x * 0.1f; // movement scaled by distance to cam
				// update the translateVec for selected objects
				// optimize by calculating once (at camera?) and use for all nodes, debug, zz
				node->translate.x += ( cosY * dX - sinY * dY ) * distFactor;			
				node->translate.y += ( -sinY * dX - cosY * dY ) * distFactor;
				node->translate.z += dZ * distFactor;
				break;			//topo based on geometery type, default
		}
	*/	
		if (node->branchLevel >= 1)
		{
			distFactor = cam->proximity.x; // movement scaled by distance to cam

			if (	parent->topo == kNPtopoPin 
				|| (parent->topo == kNPtopoNull && parent->geometry == kNPprimitiveSolidPin) )
			{
				//if Drag XY then slide vertically up down pin
				if (data->io.mouse.mode == kNPmouseModeDragXY)
				{
					node->translate.x -= dY * distFactor;	//invert mouse y
					if (node->translate.x > 180.0f)			//limit top bottom
						node->translate.x = 180.0f;
					if (node->translate.x < -180.0f)
						node->translate.x = -180.0f;
				}
				else
				{	if (data->io.axes.x)
						node->scale.x += data->io.mouse.delta.y * -0.005f;		// invert the Y axes	
					if (data->io.axes.y)
						node->scale.y += data->io.mouse.delta.y * -0.005f;		// insert check for active axes, debug, zz
					if (data->io.axes.z)
						node->scale.z += data->io.mouse.delta.y * -0.005f;
				
					node->ratio += data->io.mouse.delta.x * 0.001f;
					if (node->ratio < 0.01f)
						node->ratio = 0.01f;
					if (node->ratio > 1.0f)
						node->ratio = 1.0f;
				}
			}
			else	//not a pin
			{
				distFactor = 50.0f;		// fixed movement
				if (data->io.mouse.mode == kNPmouseModeDragXY)
				{
					node->translate.x += dX * distFactor;			
					node->translate.y += -dY * distFactor;
				}
				else
				{
					node->ratio += data->io.mouse.delta.x * 0.001f;
					if (node->ratio < 0.01f)
						node->ratio = 0.01f;
					if (node->ratio > 1.0f)
						node->ratio = 1.0f;

					if (data->io.axes.x)
						node->scale.x += data->io.mouse.delta.y * -0.005f;		// invert the Y axes	
					if (data->io.axes.y)
						node->scale.y += data->io.mouse.delta.y * -0.005f;		// insert check for active axes, debug, zz
					if (data->io.axes.z)
						node->scale.z += data->io.mouse.delta.y * -0.005f;
				}
			}
		}
		else if (node->branchLevel >= 2)
		{
			if (data->io.mouse.mode == kNPmouseModeDragXZ)
			{
				node->ratio += data->io.mouse.delta.x * 0.001f;
				if (node->ratio < 0.01f)
					node->ratio = 0.01f;
				if (node->ratio > 1.0f)
					node->ratio = 1.0f;

				if (data->io.axes.x)
					node->scale.x += data->io.mouse.delta.y * -0.005f;		// invert the Y axes	
				if (data->io.axes.y)
					node->scale.y += data->io.mouse.delta.y * -0.005f;		// insert check for active axes, debug, zz
				if (data->io.axes.z)
					node->scale.z += data->io.mouse.delta.y * -0.005f;
			}
			else // kNPmouseModeDragXY
			{
				distFactor = 50.0f;		// fixed movement
				node->translate.x += dX * distFactor;			
				node->translate.y += -dY * distFactor;
			}
		}
	}

}


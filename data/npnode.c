/* -----------------------------------------------------------------------------
*
*  npnode.c
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

#include "npnode.h"

#include "../npctrl.h"
#include "../io/gl/nptags.h"
#include "../io/npch.h"


void InitNodeSurface (NPnodePtr node);
void InitNodePoints (NPnodePtr node);
void InitNodePin (NPnodePtr node);
void InitNodeVideo (NPnodePtr node);
void InitNodeGrid (NPnodePtr node);


//------------------------------------------------------------------------------
void npInitNode (void* dataRef)
{
	int i = 0;

	pData data = (pData) dataRef;

	return;
}


//------------------------------------------------------------------------------
void npCloseNode(void* dataRef)
{
	int i = 0;

	pData data = (pData) dataRef;

	for (i=0; i < data->map.nodeRootCount; i++)
		npNodeDelete (data->map.node[i], dataRef);
}

//set the text tag offset based on geometry type
//------------------------------------------------------------------------------
void npSetTagOffset (NPnodePtr node)
{
	node->tagOffset.x = 0.0f;
	node->tagOffset.y = 0.0f;
	node->tagOffset.z = 0.0f;

	//note the missing 'break' statments allowing a function to fall to the next
	switch (node->geometry)
	{
		case kNPglutWireCube :
		case kNPglutSolidCube : 
//			break;
		case kNPglutWireSphere :
		case kNPglutSolidSphere :

		case kNPglutWireCone :
		case kNPglutSolidCone :
			node->tagOffset.z = 1.0f;
			break;
		case kNPglutWireTorus :
		case kNPglutSolidTorus :
			node->tagOffset.y = 1.5;
			break;
		case kNPglutWireDodecahedron :
		case kNPglutSolidDodecahedron :
		case kNPglutWireOctahedron :
		case kNPglutSolidOctahedron :
		case kNPglutWireTetrahedron :
		case kNPglutSolidTetrahedron :
		case kNPglutWireIcosahedron :
		case kNPglutSolidIcosahedron :
			node->tagOffset.z = 1.0f;
			break;
		case kNPprimitiveSolidPin :
		case kNPprimitiveWirePin :
			node->tagOffset.z = 5.5f;
			break;
		default : 
			if (node->branchLevel == 0)
				node->tagOffset.z = 5.5f;
			else
				node->tagOffset.y = 1.5;
			break;
	}
}

// should make this static and add locking for delete to be thread safe,      debug zz
// creates a new node and attaches it as a child of the nodeParent
// if nodeParent is NULL then creates a root node
//------------------------------------------------------------------------------
void*  npNodeNew (int nodeType, NPnodePtr nodeParent, void* dataRef)
{
	NPnodePtr node = NULL;

	pData data = (pData) dataRef;

	// check to see if max total nodes hit
	if (data->map.nodeCount >= kNPnodeMax)
	{
		printf ("err 4487 - kNPnodeMax hit, max new nodes hit limit\n");
		return NULL;
	}

	if (nodeParent != NULL)
	{
		if (nodeParent->childCount >= kNPnodeChildMax)
		{
			printf ("err 4488 - kNPnodeChildMax hit, cannot create child node\n");
			return NULL;
		}
	}
	else
	{
		if (data->map.nodeRootCount >= kNPnodeRootMax)
		{
			printf ("err 4489 - kNPnodeRootMax - max root nodes hit limit\n");
			return NULL;
		}
	}

	//allocate memory to create node and init defaults
	node = (NPnodePtr) malloc (sizeof(NPnode));
	if (data == NULL)
	{
		printf ("err 4490 - malloc failed, cannot create node\n");
		return NULL;
	}

	npInitNodeDefault (node);

	//give it an ID and set type
	node->id = npNewNodeID();
	node->type = nodeType;	

	// if root node then add to end of node array and increment root count
	if (nodeParent == NULL)
		data->map.node[data->map.nodeRootCount++] = node;
	else
	{								// attach child to nodeParent
		node->parent = nodeParent;
		node->branchLevel = nodeParent->branchLevel + 1;

		if (nodeParent != data->map.node[kNPnodeRootNull]) //debug, zz
		{
			nodeParent->child[nodeParent->childCount] = node;
			nodeParent->childIndex = nodeParent->childCount++;
		}
	}
	data->map.nodeCount++;

	// set the unique node type properties
	switch (nodeType)
	{
		case kNodeCamera :	
			npInitNodeCamera (node);
			break;
		case kNodeGrid :
			InitNodeGrid (node);
			if (node->parent != NULL)
			{
				//origin grid typically locked, second is vertical along y axis
				if (nodeParent->childCount == 1)
				{
					node->rotate.x += 90.0f;
					node->rotate.y += 90.0f;
					node->rotate.z += 180.0f;
				}
				//third is vertical along x axis
				if (nodeParent->childCount == 2)
				{
					node->rotate.x += 90.0f;
					node->rotate.z += 180.0f;
				}
				//four through eight make a cube lattice
				if (nodeParent->childCount == 3)
				{
					node->rotate.x += 90.0f;
					node->rotate.y += 90.0f;
					node->rotate.z += 180.0f;
					node->translate.x += 90.0f;
				}
				if (nodeParent->childCount == 4)
				{
					node->rotate.x += 90.0f;
					node->rotate.y += 90.0f;
					node->rotate.z += 180.0f;
					node->translate.x -= 90.0f;
				}
				if (nodeParent->childCount == 5)
				{
					node->rotate.x += 90.0f;
					node->rotate.z += 180.0f;
					node->translate.y += 90.0f;
				}
				if (nodeParent->childCount == 6)
				{
					node->rotate.x += 90.0f;
					node->rotate.z += 180.0f;
					node->translate.y -= 90.0f;
				}
				if (nodeParent->childCount == 7)
					node->translate.z += 90.0f;
				if (nodeParent->childCount == 8)
					node->translate.z -= 90.0f;
				//nine and up default to parent grid position
			}
			break;
		case kNodeVideo :	InitNodeVideo (node); break;
		case kNodeSurface :	InitNodeSurface (node); break;
		case kNodePoints :	InitNodePoints (node); break;
		case kNodePin :		
			InitNodePin (node);
			// position node based on branchLevel and sibling count
			if (node->branchLevel == 0)
			{
				// distributes 264 root nodes to fill a 24x12 grid at 15 unit spacing
				node->translate.x = -15.0f * (float)(data->map.nodeRootCount - 4);
				if ( data->map.nodeRootCount <= 171 )		// northern hemisphere
					node->translate.y = 15.0f * ((data->map.nodeRootCount - 4) / 24);
				else if ( data->map.nodeRootCount <= 315 )	// southern hemisphere
					node->translate.y = 90.0f - 15.0f * ((data->map.nodeRootCount - 4) / 24);
				else		// after grid filled place additional nodes on x axis
					node->translate.y = 0.0f;
			}
			else
			{
				if ( nodeParent->topo == kNPtopoTorus || nodeParent->topo == kNPtopoPin
					|| nodeParent->topo == kNPtopoNull)
				{
					node->topo = kNPtopoTorus;
					node->geometry = kNPglutSolidTorus;		//change geometry too
				}
				else 
					node->topo = kNPtopoPin;	//node->geometry = kNPprimitiveSolidPin;

				// distributes 266 child nodes to fill a sphere at 15 deg spacing
				node->translate.x = -15.0f * (float)(nodeParent->childCount - 1);
				if (nodeParent->childCount <= 145 )		// northern hemisphere
					node->translate.y = 15.0f * ((nodeParent->childCount - 1) / 24);
				else if (nodeParent->childCount <= 266)	// southern hemisphere
				{
					node->translate.x += 15.0f;
					node->translate.y = 75.0f - 15.0f * ((nodeParent->childCount - 2) / 24);
				}
			}
			//distribute inbounds, y wraps a sphere twice, fills a cube
			//add triangle boundary to support more topo types,			debug zz
			while (node->translate.x > 180.0f)
				node->translate.x += -360.0f;
			while (node->translate.x < -180.0f)
				node->translate.x += 360.0f;
			while (node->translate.y > 180.0f)
				node->translate.y += -360.0f;
			while (node->translate.y < -180.0f)
				node->translate.y += 360.0f;

			break;
		default : break;
	}

	//select the new node and make it active
	if (node->type == kNodePin && node->branchLevel > 0)
		npSelectNode (node->parent, data);
	else
		npSelectNode (node, data);

	//creates a map that maps node ID back to a ptr to the node, debug zz
	if (node->id < kNPnodeMax)
		data->map.nodeID[node->id] = node;
	else
		printf("err 4289 - node ID exceeds size of kNPnodeMax\n");

	npSetTagOffset (node);
	npTagNode (node, data);

	return (void*)node;
}


//------------------------------------------------------------------------------
void npNodeDelete (NPnodePtr node, void* dataRef)
{
	npNodeRemove (1, node, dataRef);
}

// removes node from tree and recursively traverses tree to free memory
//------------------------------------------------------------------------------
void npNodeRemove (int freeNode, NPnodePtr node, void* dataRef)
{
	int i = 0;
	static char msg[128];

	pData data = (pData) dataRef;
	NPnodePtr parent = NULL;

	if (node == NULL)
	{
		npPostMsg("err 8552 - npNodeRemove, NULL node", kNPmsgErr, data);
		return;
	}

	//do not remove root null, camera or grid
	if ( node == data->map.node[kNPnodeRootNull] 
		|| node == data->map.node[kNPnodeRootCamera]
		|| node == data->map.node[kNPnodeRootGrid] )
		{
			sprintf(msg, "Root id: %d Locked - Delete Disabled", node->id);
			npPostMsg(msg, kNPmsgWarn, data);
			return;
		}
	
	//if deleting then recursive call to free memory of all child nodes
	if (freeNode)
		for (i = 0; i < node->childCount; i++)
			npNodeRemove (freeNode, node->child[i], data);


	//if child, remove node from parent data structure
	if (node->branchLevel > 0)
	{
		parent = node->parent;
		
		if (node->parent != NULL)		//if not orphan node
		{
			//remove node ptr from child array compact the gap
			parent->childCount--;
			for (i = parent->childIndex; i < parent->childCount; i++)
				parent->child[i] = parent->child[i+1];

			parent->child[parent->childCount] = NULL;

			if (parent->childIndex >= parent->childCount
				&& parent->childCount > 0 )
				parent->childIndex = parent->childCount - 1;
		}
		else
		{
			sprintf(msg,"err 3464 - id: %d branchLevel: %d parent is NULL\n",
					node->id, node->branchLevel);
			npPostMsg(msg, kNPmsgErr, data);
		}
	}
	else	//process root node update data map structure
	{
		//compact the gap by updating root node array, could optimize, zz
		data->map.nodeRootCount--;	//decrement the nodeRootCount first
		for (i = data->map.nodeRootIndex; i < data->map.nodeRootCount; i++)
			data->map.node[i] = data->map.node[i+1];

		//set the leftover end of node array pointer to NULL
		data->map.node[data->map.nodeRootCount] = NULL;

		//ascert nodeRootIndex is inbounds
		if (data->map.nodeRootIndex >= data->map.nodeRootCount)
			npSelectNode (data->map.node[data->map.nodeRootCount - 1], data);
	}

	//update selectedGrid activeCam selectedPin...
	//restore active and selection index, after recursive call
	if (node->branchLevel == 0)		//must be a pin
	{
		//if deleting last pin then select camera
		if (data->map.nodeRootCount <= kNPnodeRootPin)
		{
			data->map.selectedPinIndex = 0;
			data->map.selectedPinNode = NULL;

			//select root camera to restore initial state
			npSelectNode (data->map.node[kNPnodeRootCamera], data);
		}
		else
			npSelectNode (data->map.node[data->map.nodeRootIndex], data);
	}
	else if (parent != NULL)	//check if null in case of orphan node
	{
		if (parent->childCount <= 0)
			npSelectNode (parent, data);
		else if (parent->child[parent->childIndex] != NULL)
			npSelectNode (parent->child[parent->childIndex], data);
		else
			npPostMsg("err 9422 - npNodeRemove child is NULL", kNPmsgErr, data);
	}
	else
		npPostMsg("err 9423 - npNodeRemove parent is NULL", kNPmsgErr, data);

	npChRemoveNode (node, data);	//zz-JJ

	sprintf(msg, "Delete id: %d   Select id: %d", node->id, 
			data->map.currentNode->id);
	npPostMsg (msg, kNPmsgCtrl, dataRef);

	//will free all node data including child nodes on the tree
	if (freeNode)
	{
		data->map.nodeCount--;
		free (node->data);
		free (node);
	}
}


//------------------------------------------------------------------------------
void npInitNodeDefault (NPnodePtr node)
{
	int i = 0;

//	node->id = npNewNodeID();	// called by npNew and preserved during reset

	node->type			= kNodeDefault;		//node type, point, pin, cam, video

	node->data			= NULL;				//node type specific data

	node->selected		= false;			//defualt is not selected
	
	node->parent		= NULL;				//parent node, binary tree of nodes
	node->branchLevel	= 0;			//0 is the trunk pin (ice-cream cone)

	for (i=0; i < kNPnodeChildMax; i++)
		node->child[i] = NULL;

	node->childIndex	= 0;
	node->childCount	= 0;				//no children by default

	node->chInputID		= 0;				//maps channels to antz fields
	node->chOutputID	= 0;
	node->chLastUpdated	= 0;

	node->average		= kAverageNull;		//averaging type applied to data
	node->interval		= 1;				//the interval to be averaged

	node->auxA.x		= 0;				//last updated channel index
	node->auxA.y		= 0;
	node->auxA.z		= 0;
	node->auxB.x		= 0;				//last updated channel index
	node->auxB.y		= 0;
	node->auxB.z		= 0;

	node->colorShift	= 0.0f;

	node->rotateVec.angle	= 0.0f;			//orientation vector
	node->rotateVec.x		= 0.0f;
	node->rotateVec.y		= 0.0f;
	node->rotateVec.z		= 1.0f;
	
	node->scale.x		= 1.0f;				//scale node XYZ
	node->scale.y		= 1.0f;
	node->scale.z		= 1.0f;
	
	node->translate.x	= 0.0f;				//location relative to origin
	node->translate.y	= 0.0f;
	node->translate.z	= 0.0f;

	node->tagOffset.x	= 0.0f;				//origin relative to parent origin
	node->tagOffset.y	= 0.0f;
	node->tagOffset.z	= 0.0f;
	
	node->rotateRate.x	= 0.0f;
	node->rotateRate.y	= 0.0f;
	node->rotateRate.z	= 0.0f;

	node->rotate.x		= 0.0f;
	node->rotate.y		= 0.0f;
	node->rotate.z		= 0.0f;
	
	node->scaleRate.x	= 0.0f;				//scale rate 
	node->scaleRate.y	= 0.0f;
	node->scaleRate.z	= 0.0f;

	node->translateRate.x = 0.0f;			//pans the location of the node, pronunciar en Espanol, lol
	node->translateRate.y = 0.0f;
	node->translateRate.z = 0.0f;

	node->translateVec.x = 0.0f;			//pans the location of the node, pronunciar en Espanol, lol
	node->translateVec.y = 0.0f;
	node->translateVec.z = 0.0f;
	
	node->shader		= 0;				// texture ID and render type
	node->geometry		= 0;				//FFT layers, mesh, sphere...

	node->lineWidth		= 1.0f;				//draws lines, zero for off
	node->pointSize		= 0.0f;				//draws dots, zero for off
	node->ratio			= kNPdefaultRatio;	//torus inner radius

	node->colorIndex	= 0;
	
	node->color.r		= 50;				//default color assigned to new data
	node->color.g		= 101;
	node->color.b		= 101;
	node->color.a		= 255;				//fully opaque

	node->colorFade		= false;
	node->textureID		= 0;				//zero for no texture

	node->hide			= false;			//hide the node, data stays active
	node->freeze		= false;			//freezes data and physics updates

	node->topo			= 0;				//default topo based on node type
	node->facet			= 0;				//default to facet 0

	node->autoZoom.x	= false;			//scales node to fit screen
	node->autoZoom.y	= false;
	node->autoZoom.z	= false;
	
	node->triggerHi.x	= false;			//triggers are multipurpose
	node->triggerHi.y	= false;
	node->triggerHi.z	= false;

	node->triggerLo.x = false;
	node->triggerLo.y = false;
	node->triggerLo.z = true;			//prevent from going below ground
	
	node->setHi.x = 0.0f;
	node->setHi.y = 0.0f;
	node->setHi.z = 0.0f;

	node->setLo.x = 0.0f;
	node->setLo.y = 0.0f;
	node->setLo.z = 0.0f;
	
	node->proximity.x = 0.0f;
	node->proximity.y = 0.0f;
	node->proximity.z = 0.0f;

	node->proximityMode.x			= 0;				//available 
	node->proximityMode.y			= 0;
	node->proximityMode.z			= 0;

	node->segments.x	= 16;
	node->segments.y	= 16;
	node->segments.z	= 0;

	node->tagMode		= 0;
	node->formatID		= 0;			//maps db fields and translates
	node->tableID		= 0;			//stores the source table location
	node->recordID		= 0;			//source db record
	
	node->size = 420;//sizeof(NPnode);		//causes problem with CSV loading, debug zz

	// below used for transparency and tags, not needed in CSV or DB file
	node->tag				= NULL;
	node->screen.x		= 0.0f;							//MB-LABEL
	node->screen.y		= 0.0f;
	node->screen.z		= 0.0f;
	node->distFromCamera	= 0.0f;							//MB-Transp
}


// cameras can be GL scene cameras or real-world cameras
// video is attached as a daughter of the recNode
//-----------------------------------------------------------------------------
void npInitNodeCamera (NPnodePtr node)
{
	NPcameraPtr data = NULL;
	
	//allocate memory for camera data struct if does not already exist
	if (node->data == NULL)
	{
		data = (NPcameraPtr) malloc (sizeof(NPcamera));
		if (data == NULL)
		{
			printf ("err 4490 - malloc failed, out of memory\n");
			return;
		}
		node->data  = (void*) data;
	}
	else
		data = node->data;

	//main node params
	node->type			= kNodeCamera;			//defines the node type
	node->triggerLo.z = false;				//allows cam below surface

	node->rotateVec.x = 0.0f;
	node->rotateVec.y = 0.0f;
	node->rotateVec.z = 1.0f;
	node->rotateVec.angle = 0.0f;

	//init cam data
	data->id			= node->id;
	data->type			= kNodeCamera;

	data->format		= kVideoFormat720p;
	data->interlaced	= kVideoFieldNull;		//progressive scan, no fields
	data->stereo		= kVideoStereoNull;		//not stereoscopic
	data->aspectRatio	= 1.7777778f;				//1.77778 is approx 16:9
	data->colorSpace	= kColorSpaceRGB_8;		//8bit RGB is what GPU's output
	data->fps			= 59.94f;
	data->width			= 1280;
	data->height		= 720;

	data->fov			= 35.0f;				//35mm, 70mm...
	data->clipNear		= 0.2f;					//clipping planes
	data->clipFar		= 2000.0f;

	data->aperture		= 1.0f;					//F stop
	data->exposure		= 0.01f;				//in seconds
	data->sensorType	= kCameraSensorNull;	//3CCD, Debayer pattern...

	data->size = sizeof(NPcamera);
}


//-----------------------------------------------------------------------------
void InitNodeVideo (NPnodePtr node)
{
	NPvideoPtr data = (NPvideoPtr) malloc (sizeof(NPvideo));
	if (data == NULL)
	{
		printf ("\n 4214 error malloc failed cannot write file \n");
		return;
	}

	node->data  = (void*) data;

	node->type			= kNodeVideo;

	data->id			= node->id;
	data->type			= kNodeVideo;

	data->format		= kVideoFormat720p;
	data->interlaced	= kVideoFieldNull;		//progressive scan, no fields
	data->stereo		= kVideoStereoNull;		//not stereoscopic
	data->aspectRatio	= 1.77778f;				//1.77778 is approx 16:9
	data->colorSpace	= kColorSpaceRGB_8;		//8bit RGB is what GPU's output
	data->fps			= 59.97f;
	data->width			= 1280;
	data->height		= 720;

	data->brightness	= 1.0f;				// 1.0 center, range 0.0 to 100.0
	data->contrast		= 1.0f;				// 1.0 center, range 0.0 to 100.0
	data->hue			= 0.0f;				// 0.0 center, range -1.0 to +1.0
	data->saturation	= 1.0f;				// 1.0 center, 0.0 is BW, max 100.0
	data->blackLevel	= 0.0f;				// 0.0 and 7.5 IRE typical

	data->bias.r		= 0.0f;				//brightness shift, 0.0 no change
	data->bias.g		= 0.0f;
	data->bias.b		= 0.0f;
	data->bias.a		= 0.0f;
	data->gain.r		= 1.0f;				//color multiplier, 1.0 no change
	data->gain.g		= 1.0f;
	data->gain.b		= 1.0f;
	data->gain.a		= 1.0f;

	data->size = sizeof(NPvideo);
}


//------------------------------------------------------------------------------
void InitNodeSurface (NPnodePtr node)
{
	NPsurfacePtr data = (NPsurfacePtr) malloc (sizeof(NPsurface));
	if (data == NULL)
	{
		printf ("\n 4215 error malloc failed cannot write file \n");
		return;
	}

	node->data  = (void*) data;

	node->type			= kNodeSurface;

	data->id			= node->id;
	data->type			= kNodeSurface;

	data->vertexArray	= NULL;
	data->normalArray	= NULL;
	data->colorArray	= NULL;
	data->vertexCount	= 0;
	data->normalCount	= 0;
	data->arrayIndex	= 0;

	data->size = sizeof(NPsurface);
}


//-----------------------------------------------------------------------------
void InitNodePoints (NPnodePtr node)
{
	NPpointsPtr data = (NPpointsPtr) malloc (sizeof(NPpoints));
	if (data == NULL)
	{
		printf ("\n 4216 error malloc failed cannot write file \n");
		return;
	}

	node->data  = (void*) data;
	
	node->type			= kNodePoints;

	data->id			= node->id;
	data->type			= kNodePoints;

	data->vertexArray	= NULL;				//the array of data points
	data->colorArray	= NULL;				//each point gets color assigned
	data->vertexCount	= 0;				//number of points in the node
	data->arrayIndex	= 0;				//current vertex index in the array

	data->size = sizeof(NPpoints);
}


//-----------------------------------------------------------------------------
void InitNodePin (NPnodePtr node)
{
	NPpinPtr data = (NPpinPtr) malloc (sizeof(NPpin));
	if (data == NULL)
	{
		printf ("\n 4217 error malloc failed cannot write file \n");
		return;
	}

	node->data  = (void*) data;


	node->type			= kNodePin;
	node->geometry		= kNPprimitiveSolidPin;		//ice-cream cone shape

	data->id			= node->id;

	data->innerRadius	= 0.1f;			//inner radius of the toroid
	data->outerRadius	= 1.0f;			//outer radius of the toroid

	// 4 * outerRadius / innerRadius -> int
	data->radiusRatioIndex = 40;			//sorts pre-loaded GPU toriods
	data->screenSizeIndex = 8;				//same as above for detail level

	data->slices		= 7;				//calculated from screenSizeIndex
	data->stacks		= 16;

	data->translateTex.x = 0.0f;			//position offset of texture map
	data->translateTex.y = 0.0f;
	data->translateTex.z = 0.0f;

	data->scaleTex.x	= 1.0f;
	data->scaleTex.y	= 1.0f;
	data->scaleTex.z	= 1.0f;

	data->rotateTex.x	= 0.0f;
	data->rotateTex.y	= 0.0f;
	data->rotateTex.z	= 0.0f;
	data->rotateTex.angle = 0.0f;			//orientation of texture map

	data->size = sizeof(NPpin);
}


//------------------------------------------------------------------------------
void InitNodeGrid (NPnodePtr node)
{
	NPgridPtr gridData = (NPgridPtr) malloc (sizeof(NPgrid));

	if (gridData == NULL)
	{
		printf ("\n 4218 error malloc failed cannot write file \n");
		return;
	}

	node->data = gridData;

	node->type			= kNodeGrid;

	node->triggerLo.z = false;		//turn off limit

	node->colorIndex	= 3;
	node->color.r		= 0;
	node->color.g		= 0;
	node->color.b		= 255;
	node->color.a		= 150;

	gridData->id			= node->id;
	gridData->type			= kNodeGrid;

	node->segments.x = kNPgridSegmentsX;
	node->segments.y = kNPgridSegmentsY;
	node->segments.z = 0;

	gridData->spacing.x		= kNPgridSpacing;
	gridData->spacing.y		= kNPgridSpacing;
	gridData->spacing.z		= kNPgridSpacing;

	gridData->overlay		= false;

	gridData->size			= sizeof(NPgrid);
}

//------------------------------------------------------------------------------
int npNewNodeID()				//improve this to re-use nodeID upon delete, debug zz
{
	static int nodeID = 0;

	nodeID++;
												// 2^24 max for picking algorithm
	if (nodeID >= kNPnodeMax)
		nodeID = kNPnodeMax - 1;

	return nodeID;
}

	
//return the selected camera based on the root camera childIndex
//------------------------------------------------------------------------------
NPnodePtr npGetActiveCam (void* dataRef)
{
	pData data = (pData) dataRef;

	//get the active cam if NULL then handle err by setting it to root cam
	if (data->map.activeCam == NULL)
	{
		data->map.activeCam = data->map.node[kNPnodeRootCamera];
		npPostMsg ("err 2354 - activeCam is NULL", kNPmsgErr, data);
	}
	
	return data->map.activeCam;
}


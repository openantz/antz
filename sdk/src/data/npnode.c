/* -----------------------------------------------------------------------------
*
*  npnode.c
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

#include "npnode.h"

#include "../npctrl.h"
#include "../io/gl/nptags.h"
#include "../io/npch.h"


void InitNodeSurface (pNPnode node);
void InitNodePoints (pNPnode node);
void InitNodePin (pNPnode node);
void InitNodeVideo (pNPnode node);
void InitNodeGrid (pNPnode node);
void npInitNodeHUD (pNPnode node);

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
void npSetTagOffset (pNPnode node)
{
	node->tagOffset.x = 0.0f;
	node->tagOffset.y = 0.0f;
	node->tagOffset.z = 0.0f;

	//exit if not a pin
	if (node->type != kNodePin)
		return;

	//note the missing 'break' statments allowing a function to fall to the next
	switch (node->geometry)
	{
		case kNPgeoCubeWire :
		case kNPgeoCube : 
		//	break;
		case kNPgeoSphereWire :
		case kNPgeoSphere :
		//	break;
		case kNPgeoConeWire :
		case kNPgeoCone :
			node->tagOffset.z = 1.0f;
			break;
		case kNPgeoTorusWire :
		case kNPgeoTorus :
			node->tagOffset.y = 1.5;
			break;
		case kNPgeoDodecahedronWire :
		case kNPgeoDodecahedron :
		case kNPgeoOctahedronWire :
		case kNPgeoOctahedron :
		case kNPgeoTetrahedronWire :
		case kNPgeoTetrahedron :
		case kNPgeoIcosahedronWire :
		case kNPgeoIcosahedron :
			node->tagOffset.z = 1.0f;
			break;
		case kNPgeoPin :
		case kNPgeoPinWire :
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

// http://stackoverflow.com/questions/3437404/min-and-max-in-c
// http://stackoverflow.com/questions/398299/looping-in-a-spiral
// returns the XY coordinate of the item placed by id
//-----------------------------------------------------------------------------
NPfloatXY npGridSpiralXY( int nX, int nY, float dX, float dY, int index )
//void Spiral( int X, int Y)
{
	NPfloatXY coordXY = { 0.0f, 0.0f };

    int x, y, dx, dy;
	int i = 0, j = 0;
   
    int t = 0;
    int maxI = 0;

	x = y = dx = 0;
    dy = -1;
	
	// set t = max(x,y)
	if( nX > nY ) t = nX; else t = nY;

	maxI = t*t;

	for(i=0; i < maxI; i++)
	{
        if ((-nX/2 <= x) && (x <= nX/2) && (-nY/2 <= y) && (y <= nY/2))
		{
			// DO STUFF... // printf("x: %4d y: %d \n", x, y );
			if( j++ == index )
			{
				coordXY.x = (float)x * dX;
				coordXY.y = (float)y * dY;
				return coordXY;
			}
        }
        if( (x == y) || ((x < 0) && (x == -y)) || ((x > 0) && (x == 1-y)))
		{
            t = dx;
            dx = -dy;
            dy = t;
        }
        x += dx;
        y += dy;
    }

	return coordXY;
}

// should make this static and add locking for delete to be thread safe,      debug zz
// creates a new node and attaches it as a child of the nodeParent
// if nodeParent is NULL then creates a root node
//-----------------------------------------------------------------------------
pNPnode npNodeNew (int nodeType, pNPnode nodeParent, void* dataRef)
{
	pData data = (pData) dataRef;
	pNPnode node = NULL;
	NPfloatXY xy = {0.0f,0.0f};
	char msg[256];

	// check to see if max total nodes hit
	if (data->map.nodeCount >= kNPnodeMax)
	{
		sprintf(msg,"err 4487 - kNPnodeMax hit: %d max", kNPnodeMax);
		npPostMsg(msg, kNPmsgErr, data);
		return NULL;
	}

	if (nodeParent != NULL)
	{
		if (nodeParent->childCount >= kNPnodeChildMax)
		{
			sprintf(msg,"kNPnodeChildMax Hit: %d max", kNPnodeChildMax);
			npPostMsg(msg, kNPmsgWarn, data);
			return NULL;
		}
	}
	else
	{
		if (data->map.nodeRootCount >= kNPnodeRootMax)
		{
			sprintf(msg,"err 4489 - kNPnodeRootMax hit: %d max", kNPnodeRootMax);
			npPostMsg(msg, kNPmsgErr, data);
			return NULL;
		}
	}

	//allocate memory to create node and init defaults
	node = (pNPnode) malloc (sizeof(NPnode));
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
			{	//grids form a cubicle structure
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
				xy = npGridSpiralXY(49, 25, 7.5f, 7.5f, data->map.nodeRootCount - kNPnodeRootPin - 1);

				node->translate.x = xy.x;
				node->translate.y = xy.y;

		/*		node->translate.x = -15.0f * (float)(data->map.nodeRootCount - 5);
				if ( data->map.nodeRootCount <= 172 )		// northern hemisphere
					node->translate.y = 15.0f * ((data->map.nodeRootCount - 5) / 24);
				else if ( data->map.nodeRootCount <= 316 )	// southern hemisphere
					node->translate.y = 90.0f - 15.0f * ((data->map.nodeRootCount - 5) / 24);
				else		// after grid filled place additional nodes on x axis
					node->translate.y = 0.0f;
		*/
			}
			else
			{
				if (   nodeParent->topo == kNPtopoPin 
					|| nodeParent->topo == kNPtopoRod
					|| nodeParent->topo == kNPtopoTorus )
				{
					node->topo = kNPtopoTorus;
					node->geometry = kNPgeoTorus;
				}

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
		case kNodeLink :	
			node->topo = kNPtopoNull;
			node->geometry = kNPgeoCylinder;
			break;
		case kNodeHUD :	
			npInitNodeHUD (node);
			break;
		default : break;
	}

	//select the new node and make it active
//	if (node->type == kNodePin && node->branchLevel > 0)
//		npSelectNode (node->parent, data);							//zz-s
//	else 

	node->world = node->translate;	//zz debug, workaround for cam jump
	node->world.z += kNPoffsetPin;

	if(0)//!data->io.file.loading)		//resolved in n^2 behavior during load	//zzhp
	{
		if (node->type != kNodeHUD && node->type != kNodeDefault
			&& node->branchLevel == 0)			//zz-s						//zz debug
			npSelectNode (node, data);
	}

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
bool npNodeAttach (pNPnode node, pNPnode parent, void* dataRef)
{
	if (parent->childCount >= kNPnodeChildMax)
	{
		npPostMsg ("err 2828 - Could not attach node, kNPnodeChildMax limit",
				kNPmsgErr, dataRef);
		return false;
	}

	parent->childIndex = parent->childCount++;
	parent->child[parent->childIndex] = node;

	return true;
}

//------------------------------------------------------------------------------
pNPnode npNodeNewLink (pNPnode linkA, pNPnode linkB, void* dataRef)
{
	pData data = (pData) dataRef;
	pNPnode parent = NULL;
	pNPnode node = NULL;

	bool result = false;

	node = npNodeNew (kNodeLink, linkA, data); 
	
	if (node == NULL)
	{
		npPostMsg ("err 2838 - Could not create link", kNPmsgErr, dataRef);
		return NULL;
	}

	result = npNodeAttach (node, linkB, data);
	if (!result)
		return NULL;	//err reported by npNodeAttach

	//parent switched by attach, set back to linkA
	node->parent = linkA;		//link A is the parent of link node

	node->child[0] = linkB;		//link B is made a child of the link node
//	node->childCount++;

	return node;
}

//------------------------------------------------------------------------------
void npNodeDelete (pNPnode node, void* dataRef)
{
	pData data = (pData) dataRef;
	pNPnode parent = NULL;

	data->io.mouse.linkA = NULL;	//in case of delete while using link tool

	npNodeRemove (true, node, dataRef);	//set 

	if (node->type != kNodeLink && node->parent != NULL)
	{
		parent = node->parent;

		if (parent != NULL)	//check if null in case of orphan node
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
	}
}

// removes node from tree and recursively traverses tree to free memory
//------------------------------------------------------------------------------
void npNodeRemove (bool freeNode, pNPnode node, void* dataRef)
{
	int i = 0;
	int count = 0;
	static char msg[128];

	pData data = (pData) dataRef;
	pNPnode parent = NULL;
	pNPnode child = NULL;													//zz-s

	printf("id: %d\n", node->id);

	if (node == NULL)
	{
		npPostMsg("err 8552 - npNodeRemove, NULL node", kNPmsgErr, data);
		return;
	}

	//do not remove root null, camera or grid
	if ( node == data->map.node[kNPnodeRootNull] 
		|| node == data->map.node[kNPnodeRootCamera]
		|| node == data->map.node[kNPnodeRootGrid] 
		|| node == data->map.node[kNPnodeRootHUD])							//zz debug
		{
			sprintf(msg, "Root id: %d  type: %d Locked - Delete Disabled",
					node->type, node->id);
			npPostMsg(msg, kNPmsgWarn, data);
			return;
		}

	if (node->type == kNodeHUD)												//zz debug
	{
		sprintf(msg, "err 8553 - HUD id: %d  hudType: %d Locked - Delete Disabled",
			node->type, node->hudType);
		npPostMsg(msg, kNPmsgWarn, data);
		return;
	}
	
	//recursively deletes all child branches, except for link nodes
	if (freeNode && node->type != kNodeLink)						//zz debug, should be safe to remove kNodeLink exception
	{
		//if deleting then recursive call to free memory of all child nodes
		sprintf(msg, "id: %d   childCount: %d", node->id, node->childCount);	//zz-s
		npPostMsg (msg, kNPmsgCtrl, dataRef);	

		//extra care required, this is complex routine to call recursively		//zzhp
		count = node->childCount;		//childCount changes during deletion
		for (i=0; i < count; i++)
		{
			child = node->child[0];											//zz-s
			sprintf(msg, "Delete Child id: %d   parent id: %d", child->id, node->id);
			npPostMsg (msg, kNPmsgCtrl, dataRef);							//zz-s
			npNodeRemove (freeNode, node->child[0], data);					//zz debug
		}
	}

	//if child, remove node from parent data structure
	if (node->branchLevel == 0)		//process root node update data map structure
	{
		//compact the gap by updating root node array, could optimize, zz
		data->map.nodeRootCount--;	//decrement the nodeRootCount first
		for (i = data->map.nodeRootIndex; i < data->map.nodeRootCount; i++)
			data->map.node[i] = data->map.node[i+1];

		//set the leftover end of node array pointer to NULL
		data->map.node[data->map.nodeRootCount] = NULL;

		//ascert nodeRootIndex is inbounds
		if (data->map.nodeRootIndex < kNPnodeRootPin)						//zz debug
			npSelectNode (data->map.currentCam, data);
		else if (data->map.nodeRootIndex >= data->map.nodeRootCount)
			npSelectNode (data->map.node[data->map.nodeRootCount - 1], data);

		//update selectedGrid currentCam selectedPin...
		//restore active and selection index, after recursive call
		//if deleting last pin then select camera
		if (data->map.nodeRootCount <= kNPnodeRootPin)						//zz debug
		{
			data->map.selectedPinIndex = 0;
			data->map.selectedPinNode = NULL;

			//select root camera to restore initial state
			npSelectNode (data->map.currentCam, data);
		}
		else
			npSelectNode (data->map.node[data->map.nodeRootIndex], data);
	}
	else	//child node
	{
		parent = node->parent;
		
		if (node->parent != NULL)		//if not orphan node
		{
			//below methods are desinged to work with recursive delete branches
			//find the childIndex to this node									//zz-s
			for (i = 0; i < parent->childCount; i++)
				if (parent->child[i] == node)
					parent->childIndex = i;

			//remove node ptr from child array compact the gap
			parent->childCount--;					//zz-s this has to be handled with care with recursive call
			for (i = parent->childIndex; i < parent->childCount; i++)
				parent->child[i] = parent->child[i+1];

			//set the left over child ptr to null
			parent->child[parent->childCount] = NULL;

			//ascert the childIndex is valid
			if (parent->childCount <= 0)
				parent->childIndex = 0;
			else if (parent->childIndex >= parent->childCount)
				parent->childIndex = parent->childCount - 1;

			//if a link then also need to remove from parent at the B end
			if (node->type == kNodeLink)
			{
				parent = node->child[0];	//link B is a child[0] of the link
			
				//methods are desinged to work with recursive delete branches

				//find the childIndex to this node									//zz-s
				for (i = 0; i < parent->childCount; i++)
					if (parent->child[i] == node)
						parent->childIndex = i;

				//remove node ptr from child array compact the gap
				parent->childCount--;					//zz-s this has to be handled with care with recursive call
				for (i = parent->childIndex; i < parent->childCount; i++)
					parent->child[i] = parent->child[i+1];

				//set the left over child ptr to null
				parent->child[parent->childCount] = NULL;

				//ascert the childIndex is valid
				if (parent->childCount <= 0)
					parent->childIndex = 0;
				else if (parent->childIndex >= parent->childCount)
					parent->childIndex = parent->childCount - 1;
			}
		}
		else
		{
			sprintf(msg,"err 3464 - id: %d level: %d parent is NULL\n",
					node->id, node->branchLevel );
			npPostMsg(msg, kNPmsgErr, data);
		}

/*		if (node->type != kNodeLink)
		{
			if (parent != NULL)	//check if null in case of orphan node
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
		}
*/	}

	npChRemoveNode (node, data);	//zz-JJ

	//update the nodeID map													//zz-s
	data->map.nodeID[node->id] = NULL;

	sprintf (msg, "Delete id: %d  Select id: %d", node->id, 
									data->map.currentNode->id );
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
void npInitNodeDefault (pNPnode node)
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

	node->screen.x			= 0.0f;							//MB-LABEL
	node->screen.y			= 0.0f;
	node->screen.z			= 0.0f;

	node->world.x			= 0.0f;
	node->world.y			= 0.0f;
	node->world.z			= 0.0f;

	node->distFromCamera	= 0.0f;							//MB-Transp
	node->hudType			= 0;

	node->linkFlag			= false;
}


// cameras can be GL scene cameras or real-world cameras
// video is attached as a daughter of the recNode
//-----------------------------------------------------------------------------
void npInitNodeCamera (pNPnode node)
{
	int i = 0;
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
	data->stereo3D		= kVideoStereoNull;		//not stereoscopic
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

	for (i=0; i < 16; i++);
		data->matrix[i]= 0.0f;

	data->size = sizeof(NPcamera);
}


//-----------------------------------------------------------------------------
void InitNodeVideo (pNPnode node)
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
	data->stereo3D		= kVideoStereoNull;		//not stereoscopic
	data->aspectRatio	= 1.77778f; 			//1.77778 is approx 16:9
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
void InitNodeSurface (pNPnode node)
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
void InitNodePoints (pNPnode node)
{
	NPpointsPtr data = (NPpointsPtr) malloc (sizeof(NPpoints));
	if (data == NULL)
	{
		printf ("\n 4317 error malloc failed cannot write file \n");
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
void InitNodePin (pNPnode node)
{
	NPpinPtr data = (NPpinPtr) malloc (sizeof(NPpin));
	if (data == NULL)
	{
		printf ("\n 4217 error malloc failed cannot write file \n");
		return;
	}

	node->data  = (void*) data;


	node->type			= kNodePin;
	node->topo			= kNPtopoPin;
	node->geometry		= kNPgeoPin;		//ice-cream cone shape

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
void InitNodeGrid (pNPnode node)
{
	NPgridPtr gridData = (NPgridPtr) malloc (sizeof(NPgrid));

	if (gridData == NULL)
	{
		printf ("\n 4219 error malloc failed cannot write file \n");
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
void npInitNodeHUD (pNPnode node)
{
	node->type			= kNodeHUD;

	node->triggerLo.z = false;		//turn off limit

	node->colorIndex	= 15;		//grey
	node->color.r		= 127;
	node->color.g		= 127;
	node->color.b		= 127;
	node->color.a		= 127;		//opacity is overwridden by npDrawNodeTags()

	node->tagMode = kNPtagModeBoxOutlineHUD;
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
pNPnode npGetActiveCam (void* dataRef)
{
	pData data = (pData) dataRef;

	//get the active cam if NULL then handle err by setting it to root cam
	if (data->map.currentCam == NULL)
	{
		data->map.currentCam = data->map.node[kNPnodeRootCamera];
		npPostMsg ("err 2354 - currentCam is NULL", kNPmsgErr, data);
	}
	
	return data->map.currentCam;
}


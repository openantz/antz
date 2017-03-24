/* -----------------------------------------------------------------------------
*
*  npselect.c
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

#include "npselect.h"


/* -----------------------------------------------------------------------------
*
* control command actions processed here
* input event command mapping to target context and flow control by npctrl.h
*
* --------------------------------------------------------------------------- */


//------------------------------------------------------------------------------
void npInitSelect (void* dataRef)
{
	return;
}


//------------------------------------------------------------------------------
void npCloseSelect (void* dataRef)
{
	return;
}

//------------------------------------------------------------------------------
void npUpdateSelect (void* dataRef)
{
	return;
}


//------------------------------------------------------------------------------
void npSelectNode (pNPnode node, void* dataRef)
{
	int rootIndex = 0;
	pData data = (pData) dataRef;

	pNPnode parent = NULL;
//	char msg[256];																//zz-s

	//return if NULL, not considered an err
	if (node == NULL)
	{
		//zz debug, not really an error
//		npPostMsg("Err 6841 - invalid node selected", kNPmsgErr, dataRef);		//zz-s
		
		//select the camera
		node = data->map.currentCam;

		if (node == NULL)
		{
			npPostMsg("Err 6841 - currentCam is NULL", kNPmsgErr, dataRef);
			return;
		}
	}

	
	if ( data->map.previousNode != data->map.currentNode )
		data->map.previousNode = data->map.currentNode;
	

/*	if (node->parent != NULL)													//zz-s	
	{
		parent = node->parent;
		sprintf(msg,"id: %d  type: %d  parent: %d", node->id, node->type, parent->id); 
	}
	else
		sprintf(msg,"node id: %d  type: %d  parent NULL", node->id, node->type); 
	npPostMsg(msg, kNPmsgErr, dataRef);											//zz-s
*/	
	//traverses up the tree to the pin root and gets the pin index
	rootIndex = npGetRootIndex (node, dataRef);

	//if a pin
	switch (node->type)
	{	
	//	case kNodeDefault : break;
		case kNodeCamera :
			data->map.currentCam = node; 
			data->map.currentNode = node;
			data->map.nodeRootIndex = rootIndex;
			break;
		case kNodePin :
		case kNodeLink :
			data->map.selectedPinIndex = rootIndex;
			data->map.selectedPinNode = node;
			data->map.currentNode = node;
			data->map.nodeRootIndex = rootIndex;
			break;
		case kNodeGrid :
			data->map.selectedGrid = node; 
			data->map.currentNode = node;
			data->map.nodeRootIndex = rootIndex;
			break;
		case kNodeHUD : data->map.selectedHUD = node; break;
		default : 
			npPostMsg("Err 6842 - node type unknown", kNPmsgErr, dataRef);
			
			//select the camera
		/*	rootIndex = 1;
			node = data->map.currentCam;
			data->io.mouse.pickMode = kNPmodeCamera;
			data->map.currentNode = node;
			data->map.nodeRootIndex = rootIndex;
		*/
			break;
	}
}

//------------------------------------------------------------------------------
void npNodeSelection( pNPnode node, void* dataRef)
{
	if( node->type != kNodePin )
		return;

	node->selected = true;
}

//------------------------------------------------------------------------------
void npNodeSelectionOff( pNPnode node, void* dataRef)
{
	node->selected = false;
}

/// Select ALL nodes for given node type.
//------------------------------------------------------------------------------
void npSelectAll (int nodeType, void* dataRef)
{
	int i = 0;
	int commandTemp = 0;

	pData data = (pData) dataRef;
	pNPnode node = data->map.currentNode;


	/// @todo implement selecting nodes by type
	if( nodeType == kNodeCamera )
	{
		npPostMsg( "err 3034 - npSelectAll camera not implemented", 0, data);
		return;
	}
	else if( nodeType == kNodeGrid )
	{
		npPostMsg( "err 3035 - npSelectAll grid not implemented", 0, data);
		return;
	}

	if( data->io.mouse.tool == kNPtoolHide ) 
	{
		data->io.hideLevel--;
		if( data->io.hideLevel < -1 )
			data->io.hideLevel = kNPhideLevelMax;

		npTraverseMap (npNodeHideLevel, dataRef);
	}
	else if( data->io.mouse.tool == kNPtoolTag )
		npTraverseMap (npNodeTagModeOff, dataRef);
	else	
	{	/// select or clear all nodes
		data->map.selectAll = 1 - data->map.selectAll; //toggle selectAll

		if( data->map.selectAll )
			npTraverseMap (npNodeSelection, dataRef);
		else
			npTraverseMap (npNodeSelectionOff, dataRef);
	}
}

/// Clear node selection for given node type.
//------------------------------------------------------------------------------
void npSelectNone( int nodeType, void* dataRef)
{

}

/// Invert node selection for given node type.
//------------------------------------------------------------------------------
void npSelectInvertNode( pNPnode node )
{
	node->selected = 1 - node->selected;
}

/// Invert node selection for given node type.
//------------------------------------------------------------------------------
void npSelectInvertAll( int nodeType, void* dataRef )
{
	int i = 0, nodeCount = 0;
	pNPnode node = NULL;
	pNPnode* nodeList = NULL;

	pData data = (pData) dataRef;

	// get the list
	nodeCount = data->map.nodeCount;

	// invert all nodes in list
	for( i = 0; i < nodeCount; i++ )
	{
		node = nodeList[i];
		node->selected = 1 - node->selected;
	}
}

//------------------------------------------------------------------------------
void npSelectNodeByID (int id, void* dataRef)
{
	pNPnode node = NULL;

	node = npGetNodeByID( id, dataRef);	//get node by id

	npSelectNode( node, dataRef);
}



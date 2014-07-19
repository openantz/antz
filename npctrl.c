/* -----------------------------------------------------------------------------
*
*  npctrl.c
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

#include "npdata.h"
#include "npio.h"
#include "npctrl.h"

#include "data/npnode.h"
#include "io/npgl.h"
#include "io/npfile.h"
#include "io/npch.h"
#include "io/npmouse.h"
#include "ctrl/npengine.h"
#include "os/npos.h"

void npCtrlFile (int command, void* dataRef);

void npCtrlVector (int command, void* dataRef);
void npCtrlSelect (int command, void* dataRef);
void npCtrlProperty (int command, void* dataRef);
void npCtrlGlobal (int command, void* dataRef);
void npCtrlChannel (int command, NPnodePtr node, void* dataRef);

void TraverseTree (int command, NPnodePtr node, void* dataRef);

void npDispatchMessages (void* dataRef);

//------------------------------------------------------------------------------
void npInitCtrl (void* dataRef)
{
	npInitEngine (dataRef);
}

//------------------------------------------------------------------------------
void npCloseCtrl (void* dataRef)
{
	npCloseEngine (dataRef);
}

//------------------------------------------------------------------------------
int npAppLoop (void* dataRef)
{
	int err = 0;

#ifdef NP_LINUX_
	//	MainAppLoopLinux();
	npAppLoopGlut (dataRef);
#endif

#ifdef NP_MSW_
	//	MainAppLoopMSW();
	npAppLoopGlut (dataRef);
#endif

#ifdef NP_OSX_
	//	MainAppLoopOSX();
	npAppLoopGlut (dataRef);
#endif

	return err;
}


//------------------------------------------------------------------------------
void TraverseTree (int command, NPnodePtr node, void* dataRef)
{
	int i = 0;
	
	pData data = (pData) dataRef;

	// recursively iterate through all nodes in the tree
	for (i=0; i < node->childCount ; i++)
		TraverseTree (command, node->child[i], dataRef);
	
	if (data->io.mouse.tool == kNPtoolInfo)
	{
		//hide everything except root pins and primary torus
		if (command == kNPcmdSelectOff)
			node->tagMode = 0;		//turn all tags off
		else
		{
			node->tagMode++;		//otherwise toggle tags on
			if (node->tagMode >= kNPtagModeCount)		//zz debug, nature of ...SelectOn
				node->tagMode = kNPtagModeBoxOutlineHUD;
		}
	}
	else if (data->io.mouse.tool == kNPtoolHide)
	{
		//hide everything except root pins and primary torus
		if (node->branchLevel > 1 && command == kNPcmdSelectOff)
			node->hide = true;
		else
			node->hide = false;		//otherwise unhide all
	}
	
	if (data->io.mouse.tool == kNPtoolNull
		|| !(command == kNPcmdSelect || command == kNPcmdSelectOff) )
	{
		if ( node->selected 
			 || command == kNPcmdSelect 
			 || command == kNPcmdSelectOff )
		{
			data->map.currentNode = node;
			npCtrlCommandSet (command, node, dataRef);
		}
	}
}

//------------------------------------------------------------------------------
void npCtrlCommand (int command, void* dataRef)
{
	int i = 0;
	int indexTemp = 0;

	pData data = (pData) dataRef;
	NPnodePtr node = data->map.currentNode;


	switch (command)
	{
		// kCmdGlobal... global commands, draw indicators/HUD, quit
//		case kQuit : npCtrlGlobal (command, dataRef); break;

		case kNPfileNew : npCtrlFile (command, dataRef); break;
		case kNPfileOpen : npCtrlFile (command, dataRef); break;
		case kNPfileClose : npCtrlFile (command, dataRef); break;
		case kNPfileSave : npCtrlFile (command, dataRef); break;
		case kNPfileSaveAs : npCtrlFile (command, dataRef); break;
		case kNPfileImport : npCtrlFile (command, dataRef); break;
		case kNPfileExport : npCtrlFile (command, dataRef); break;
		case kNPfileMapOne : npCtrlFile (command, dataRef); break;
		case kNPfileMapTwo : npCtrlFile (command, dataRef); break;
		case kNPfileMapThree : npCtrlFile (command, dataRef); break;

		case kNPopenURL : npCtrlFile (command, dataRef); break;
		case kNPopenApp : npCtrlFile (command, dataRef); break;
		case kNPopenAntz : npCtrlFile (command, dataRef); break;

		case kNPcmdMenu : npCtrlGlobal (command, dataRef); break;
		case kNPcmdFullscreen : npCtrlGlobal (command, dataRef); break;
		case kNPcmdClearFrameBuffer : npCtrlGlobal (command, dataRef); break;
		case kNPcmdBackground : npCtrlGlobal (command, dataRef); break;		
		
		case kNPcmdNormalMode : npCtrlGlobal (command, dataRef); break;
		case kNPcmdShaderMode : npCtrlGlobal (command, dataRef); break;
		case kNPcmdAlphaMode : npCtrlGlobal (command, dataRef); break;

		case kNPcmdPause : npCtrlGlobal (command, dataRef); break;

		// kCmdSelect.. modify selection / scope (navigate the map/space)	
		// Selects the active axes and nodes to manipulate
		case kNPcmdSelectAxes : npCtrlSelect (command, dataRef); break;

		case kNPcmdGrid : npCtrlSelect (command, dataRef); break;
		case kNPcmdCamera : npCtrlSelect (command, dataRef); break;
		case kNPcmdPin : npCtrlSelect (command, dataRef); break;

		case kNPcmdNextBranch : npCtrlSelect (command, dataRef); break;

		case kNPcmdNew : npCtrlSelect (command, dataRef); break;
		case kNPcmdDelete : npCtrlSelect (command, dataRef); break;

		case kNPcmdSelect : npCtrlSelect (command, dataRef); break;	// broken, debug zz
		case kNPcmdSelectOff : npCtrlSelect (command, dataRef); break;	// broken, debug zz
		case kNPcmdSelectToggle : npCtrlSelect (command, dataRef); break;	// broken, debug zz

		case kNPcmdSelectAll : npCtrlSelect (command, dataRef); break;		// broken, debug zz
		case kNPcmdSelectNone : npCtrlSelect (command, dataRef); break;
		
		case kNPcmdSelectOne : npCtrlSelect (command, dataRef); break;
		case kNPcmdSelectTwo : npCtrlSelect (command, dataRef); break;
		case kNPcmdSelectThree : npCtrlSelect (command, dataRef); break;
		case kNPcmdSelectSetNone : npCtrlSelect (command, dataRef); break;

		case kNPcmdPresetOne : npDataPreset (command, dataRef); break;
		case kNPcmdPresetTwo : npDataPreset (command, dataRef); break;
		case kNPcmdPresetThree : npDataPreset (command, dataRef); break;
		case kNPcmdPresetFour : npDataPreset (command, dataRef); break;
		case kNPcmdPresetFive : npDataPreset (command, dataRef); break;
		case kNPcmdPresetSix : npDataPreset (command, dataRef); break;
		case kNPcmdPresetSeven : npDataPreset (command, dataRef); break;
		case kNPcmdPresetEight : npDataPreset (command, dataRef); break;
		

		default :
				//add proper key handling for modifier keys to map to any command, //zz debug
				//the command should not be handling the modifier, vice versa
			if (data->io.key.modAlt)
			{
				npCtrlSelect (command, dataRef);
				break;
			}

			// not a global command
			// exclude global commands or only 1 selected node from applying to all nodes
			// ie. allows for adding an unsellected node to the current group
			if ( !node->selected || data->map.nodeRootIndex < kNPnodeRootPin ) //&& node != data->map.activeCam)//zz debug
			{
				npCtrlCommandSet (command, node, data);
			}
			else
			{									//apply to all selected nodes
				for (i = kNPnodeRootPin; i < data->map.nodeRootCount; i++)
					TraverseTree (command, data->map.node[i], dataRef);

				npSelectNode (node, data);				//restore active node
			}	
		break;
	}
}

//------------------------------------------------------------------------------
void npCtrlCommandSet (int command, void* nodeRef, void* dataRef)
{
	pData data = (pData) dataRef;
	NPnodePtr node = (NPnodePtr) nodeRef;


	switch (command)
	{
		case kNPcmdSelect : npCtrlSelect (command, dataRef); break;	// broken, debug zz
		case kNPcmdSelectOff : npCtrlSelect (command, dataRef); break;	// broken, debug zz
		case kNPcmdSelectToggle : npCtrlSelect (command, dataRef); break;	// broken, debug zz
		
		//translate the selected objects relative to the camera
		// kCmd... apply to current selection / scope (X/Y/XY/Z/XYZ)
		case kNPcmdXdecrease : npCtrlVector (command, dataRef); break;
		case kNPcmdXdecreaseOff : npCtrlVector (command, dataRef); break;
		case kNPcmdXincrease : npCtrlVector (command, dataRef); break;
		case kNPcmdXincreaseOff : npCtrlVector (command, dataRef); break;

		case kNPcmdYdecrease : npCtrlVector (command, dataRef); break;
		case kNPcmdYdecreaseOff : npCtrlVector (command, dataRef); break;
		case kNPcmdYincrease : npCtrlVector (command, dataRef); break;
		case kNPcmdYincreaseOff : npCtrlVector (command, dataRef); break;

		case kNPcmdZdecrease : npCtrlVector (command, dataRef); break;
		case kNPcmdZdecreaseOff : npCtrlVector (command, dataRef); break;
		case kNPcmdZincrease : npCtrlVector (command, dataRef); break;
		case kNPcmdZincreaseOff : npCtrlVector (command, dataRef); break;

		case kNPcmdRotateLeft : npCtrlVector (command, dataRef); break;
		case kNPcmdRotateLeftOff : npCtrlVector (command, dataRef); break;
		case kNPcmdRotateRight : npCtrlVector (command, dataRef); break;
		case kNPcmdRotateRightOff : npCtrlVector (command, dataRef); break;

		case kNPcmdRotateUp : npCtrlVector (command, dataRef); break;
		case kNPcmdRotateUpOff : npCtrlVector (command, dataRef); break;
		case kNPcmdRotateDown : npCtrlVector (command, dataRef); break;
		case kNPcmdRotateDownOff : npCtrlVector (command, dataRef); break;

		case kNPcmdRotateCCW : npCtrlVector (command, dataRef); break;
		case kNPcmdRotateCCWOff : npCtrlVector (command, dataRef); break;
		case kNPcmdRotateCW : npCtrlVector (command, dataRef); break;
		case kNPcmdRotateCWOff : npCtrlVector (command, dataRef); break;

		case kNPcmdZoomOn : npCtrlVector (command, dataRef); break;
		case kNPcmdZoomOff : npCtrlVector (command, dataRef); break;

		// Change node property
		case kNPcmdChannelUp : npCtrlProperty (command, dataRef); break;
		case kNPcmdChannelDown : npCtrlProperty (command, dataRef); break;

		case kNPcmdColorUp : npCtrlProperty (command, dataRef); break;
		case kNPcmdColorDown : npCtrlProperty (command, dataRef); break;
		case kNPcmdAlphaUp : npCtrlProperty (command, dataRef); break;
		case kNPcmdAlphaDown : npCtrlProperty (command, dataRef); break;
			
		case kNPcmdTexture : npCtrlProperty (command, dataRef); break;
		case kNPcmdCenter : npCtrlProperty (command, dataRef); break;
		case kNPcmdLines : npCtrlProperty (command, dataRef); break;
		case kNPcmdPoints : npCtrlProperty (command, dataRef); break;
		case kNPcmdShader : npCtrlProperty (command, dataRef); break;
		case kNPcmdPrimitive : npCtrlProperty (command, dataRef); break;
		case kNPcmdTopo : npCtrlProperty (command, dataRef); break;
		case kNPcmdMesh : npCtrlProperty (command, dataRef); break;
		case kNPcmdScroll : npCtrlProperty (command, dataRef); break;
		case kNPcmdFreeze : npCtrlProperty (command, dataRef); break;
		case kNPcmdHide : npCtrlProperty (command, dataRef); break;
		case kNPcmdClear : npCtrlProperty (command, dataRef); break;

		case kNPcmdTagType : npCtrlProperty (command, dataRef); break;

		case kNPcmdSegments : npCtrlProperty (command, dataRef); break;

		case kNPcmdSetpointLo : npCtrlProperty (command, dataRef); break;
		case kNPcmdSetpointHi : npCtrlProperty (command, dataRef); break;

		case kNPcmdRatio : npCtrlProperty (command, dataRef); break;	
			
		default : break;
	}
}

//------------------------------------------------------------------------------
void npCtrlFile (int command, void* dataRef)
{
	int result = 0;
	static char msg[kNPmaxPath];
	static char fileName[512];
	static char filePath[kNPmaxPath];

	FILE* file = NULL;

	pData data = (pData) dataRef;

	fileName[0] = '\0';
	filePath[0] = '\0';


	switch (command)
	{
		case kNPfileNew : npFileDialog (NULL, kNPfileDialogNew, dataRef); break;

		case kNPfileOpen :
			if (data->io.gl.fullScreen)	//workaround, gl file dialog conflict
			{	
				npCtrlCommand(kNPcmdFullscreen, data);
				file = npFileDialog (data->io.file.csvPath, kNPfileDialogOpen, dataRef);
				npCtrlCommand(kNPcmdFullscreen, data);
			}
			else
				file = npFileDialog (NULL, kNPfileDialogOpen, dataRef);
			npCSVtoMap (file, kNPtableNode, dataRef);			//move this, debug zz
			break;
		case kNPfileClose : npFileDialog (NULL, kNPfileDialogClose, dataRef); break;

		case kNPfileSave :
			strcpy (filePath, data->io.file.csvPath);
			nposTimeStampCSV (fileName);
			strcat (filePath, fileName);
			sprintf(msg, "Saving: %s", filePath);
			npPostMsg (msg, kNPmsgCtrl, dataRef);
			result = npFileSaveMap (filePath , 1, (int)strlen(filePath), dataRef);
			if (result) npPostMsg ("Done!", kNPmsgCtrl, dataRef);
			break;

		case kNPfileSaveAs : npFileDialog (NULL, kNPfileDialogSaveAs, dataRef); break;

		case kNPfileMapOne :
			strcpy (filePath, data->io.file.csvPath);
			strcat (filePath, "ANTzTag0001.csv");
			result = npFileOpenTag (filePath, 1, strlen(filePath), dataRef);
			if (result) npPostMsg ("Text Tags Loaded: ANTzTag0001.csv", kNPmsgCtrl, dataRef);
			if (data->io.key.modAlt)	//if alt held then load only the tags
				break;

			strcpy (filePath, data->io.file.csvPath);
			strcat (filePath, "ANTz0001.csv");
			if (data->io.key.modShift)
			{
				sprintf (msg, "Saving: %s", filePath);
				npPostMsg (msg, kNPmsgCtrl, dataRef);
				result = npFileSaveMap (filePath, 1, strlen(filePath), dataRef);
			}
			else
			{
				sprintf(msg, "Loading: %s", filePath);
				npPostMsg (msg, kNPmsgCtrl, dataRef);
				result = npFileOpenMap (filePath, 1, strlen(filePath), dataRef);
			}
			if (result) npPostMsg ("Done!", kNPmsgCtrl, dataRef);
			break;
		case kNPfileMapTwo :
			strcpy (filePath, data->io.file.csvPath);
			strcat (filePath, "ANTz0002.csv");
			if (data->io.key.modShift)
			{
				sprintf (msg, "Saving: %s", filePath);
				npPostMsg (msg, kNPmsgCtrl, dataRef);
				result = npFileSaveMap (filePath, 1, strlen(filePath), dataRef);
			}
			else
			{
				sprintf(msg, "Loading: %s", filePath);
				npPostMsg (msg, kNPmsgCtrl, dataRef);
				result = npFileOpenMap (filePath, 1, strlen(filePath), dataRef);
			}
			if (result) npPostMsg ("Done!", kNPmsgCtrl, dataRef);
			break;
		case kNPfileMapThree :
			strcpy (filePath, data->io.file.csvPath);
			strcat (filePath, "ANTz0003.csv");
			if (data->io.key.modShift)
			{
				sprintf (msg, "Saving: %s", filePath);
				npPostMsg (msg, kNPmsgCtrl, dataRef);
				result = npFileSaveMap (filePath, 1, strlen(filePath), dataRef);
			}
			else
			{
				sprintf(msg, "Loading: %s", filePath);
				npPostMsg (msg, kNPmsgCtrl, dataRef);
				result = npFileOpenMap (filePath, 1, strlen(filePath), dataRef);
			}
			if (result) npPostMsg ("Done!", kNPmsgCtrl, dataRef);
			break;
		//zz-JJ
		case kNPfileImport :
			strcpy (filePath, data->io.file.csvPath);
			if (data->io.key.modShift)
			{
				strcat (filePath, "ANTzChMap0002.csv");
				npSaveChMap (filePath, dataRef);
			}
			else			// load set of channel files, Metadata, Tracks and ChMap
			{
				strcat (filePath, "ANTzChSet0001.csv");			
				npFileOpenChSet (filePath, dataRef);
			}
			break; //npFileDialog (NULL, kNPfileDialogImport, dataRef); break;
		case kNPfileExport : npFileDialog (NULL, kNPfileDialogExport, dataRef); break;


		case kNPopenURL : npOpenURL (NULL, dataRef); break;
		case kNPopenApp : npOpenApp (NULL, dataRef); break;
		case kNPopenAntz : npOpenAntz (NULL, dataRef); break;

		default : break;
	}
}


// update the translateVec for selected objects
// translateVec is relative to global coordinates
// translateRate is relative to the camera
//------------------------------------------------------------------------------
void npCtrlVector (int command, void* dataRef)
{
	float velocity = 0.0f;

	pData data = (pData) dataRef;
	NPnodePtr node = data->map.currentNode;		//apply command to currentNode


	//exit if in cam mode and this is not a camera node
	if (data->io.mouse.pickMode == kNPpickModeCamera)
		if (node != data->map.activeCam)
			return;

	// if shift pressed then use fast velocity, otherwise slow
	if (data->io.key.modShift)
		velocity = data->ctrl.fast;
	else
		velocity = data->ctrl.slow;

	switch (command)
	{								//normalize and change to 0.0f - velocity,	debug zz
		case kNPcmdXdecrease : node->translateRate.x = -velocity; break;
		case kNPcmdXdecreaseOff : 
			if (node->translateRate.x < 0.0f)	//graceful multi-key behavior
				node->translateRate.x = 0.0f; 
			break;
		case kNPcmdXincrease : node->translateRate.x = velocity; break;
		case kNPcmdXincreaseOff : 
			if (node->translateRate.x > 0.0f)
				node->translateRate.x = 0.0f; 
			break;

		case kNPcmdYdecrease : node->translateRate.y = -velocity; break;
		case kNPcmdYdecreaseOff : 
			if (node->translateRate.y < 0.0f)
				node->translateRate.y = 0.0f;
			break;
		case kNPcmdYincrease : node->translateRate.y = velocity; break;
		case kNPcmdYincreaseOff : 
			if (node->translateRate.y > 0.0f)
				node->translateRate.y = 0.0f;
			break;

		case kNPcmdZdecrease : node->translateRate.z = -velocity; break;
		case kNPcmdZdecreaseOff : 
			if (node->translateRate.z < 0.0f)
				node->translateRate.z = 0.0f;
			break;
		case kNPcmdZincrease : node->translateRate.z = velocity; break;
		case kNPcmdZincreaseOff : 
			if (node->translateRate.z > 0.0f)
				node->translateRate.z = 0.0f;
			break;
		
		case kNPcmdRotateLeft : node->rotateRate.y = -velocity; break;
		case kNPcmdRotateLeftOff : 
			if (node->rotateRate.y < 0.0f)
				node->rotateRate.y = 0.0f;
			break;
		case kNPcmdRotateRight : node->rotateRate.y = velocity; break;
		case kNPcmdRotateRightOff : 
			if (node->rotateRate.y > 0.0f)
				node->rotateRate.y = 0.0f;
			break;

		case kNPcmdRotateUp : node->rotateRate.x = velocity; break;
		case kNPcmdRotateUpOff :
			if (node->rotateRate.x > 0.0f)
				node->rotateRate.x = 0.0f;
			break;
		case kNPcmdRotateDown : node->rotateRate.x = -velocity; break;
		case kNPcmdRotateDownOff :
			if (node->rotateRate.x < 0.0f)
				node->rotateRate.x = 0.0f;
			break;

		case kNPcmdRotateCW : node->rotateRate.z = velocity; break;
		case kNPcmdRotateCWOff : 
			if (node->rotateRate.z > 0.0f)
				node->rotateRate.z = 0.0f;
			break;
		case kNPcmdRotateCCW : node->rotateRate.z = -velocity; break;
		case kNPcmdRotateCCWOff : 
			if (node->rotateRate.z < 0.0f)
				node->rotateRate.z = 0.0f;
			break;
	

		case kNPcmdZoomOn :
			if (data->io.key.modShift)		//scales the object
			{
				if (data->io.axes.x == true )
					node->scaleRate.x = -0.003f;
				if (data->io.axes.y == true )
					node->scaleRate.y = -0.003f;
				if (data->io.axes.z == true )
					node->scaleRate.z = -0.003f;			
			}
			else
			{
				if (data->io.axes.x == true )
					node->scaleRate.x = 0.003f;
				if (data->io.axes.y == true )
					node->scaleRate.y = 0.003f;
				if (data->io.axes.z == true )
					node->scaleRate.z = 0.003f;			
				}
			break;
		case kNPcmdZoomOff :
			node->scaleRate.x = 0.0f;
			node->scaleRate.y = 0.0f; 
			node->scaleRate.z = 0.0f; 
			break;

		default : break;
	}
}

//------------------------------------------------------------------------------
void npCtrlSelect (int command, void* dataRef)
{
	int i = 0;
	int commandTemp = 0;

	static char msg[256];
	static char msgPart[256];

	pData data = (pData) dataRef;
	NPnodePtr node = data->map.currentNode;
	NPnodePtr nodeChild = NULL;
	NPnodePtr nodeParent = NULL;


	msgPart[0] = '\0';

	switch (command)
	{						// add ability to copy existing node parameters, debug zz
		case kNPcmdNew :
			//if camera selected and no pins exist then create 1st pin
			if (//data->map.nodeRootIndex == kNPnodeRootCamera &&		//zz debug, workaround for cam move grid bug
				 data->map.nodeRootCount <= kNPnodeRootPin )
			{
				node = npNodeNew (kNodePin, NULL, dataRef);
				npNodeNew (node->type, node, dataRef);
			}
			else
			{
				//if camera or grid
				if (node->type == kNodeCamera || node->type == kNodeGrid)
				{
					if ( node->branchLevel == 1  && !data->io.key.modShift )
						node = npNodeNew (node->type, node->parent, dataRef);
					else			
						node = npNodeNew (node->type, node, dataRef);
				}
				else if (node->type == kNodePin)
				{	//if root create new pin, if shift create child
					if (node->branchLevel == 0 && !data->io.key.modShift
						&& node->childCount > 0 )
					{
						node = npNodeNew (kNodePin, NULL, dataRef);
						npNodeNew (node->type, node, dataRef);
					} else
						node = npNodeNew (node->type, node, dataRef);
				}
			}
			npSetCamTarget (data);	//sets camera target to selected node

			if (node->type == kNodeCamera)
				sprintf (msgPart, "New camera ");
			if (node->type == kNodeGrid)
				sprintf (msgPart, "New grid ");
			if (node->type == kNodePin)
				sprintf (msgPart, "New pin ");
			sprintf (msg, "%sid: %d   branchLevel: %d", msgPart, node->id, node->branchLevel);
			npPostMsg (msg, kNPmsgCtrl, dataRef);
			break;

		case kNPcmdDelete :
			npNodeDelete (node, dataRef);
			npSetCamTarget (data);	//sets camera target to selected node
			break;
			
		case kNPcmdPin :
			if (data->io.mouse.pickMode != kNPpickModePin)
			{
				data->io.mouse.pickMode = kNPpickModePin;
				npPostMsg("mode: Pin", kNPmsgCtrl, dataRef);
			}
			if (data->map.nodeRootCount <= kNPnodeRootPin)		// if no nodes select cam
			{
				npSelectNode (data->map.activeCam, data);
				break;
			}
			if (data->map.nodeRootIndex < kNPnodeRootPin)		//camera or grid selected
			{
				npSelectNode (data->map.selectedPinNode, data); //restore previous node
				npSetCamTarget (data);
			}
			else
			{
				if (node->branchLevel == 0)	//root pin
				{
					//get next root node selection
					if  (data->io.key.modShift)
						data->map.nodeRootIndex--;
					else
						data->map.nodeRootIndex++;

					//keep inbounds and rollover if necessary
					if (data->map.nodeRootIndex >= data->map.nodeRootCount)
							data->map.nodeRootIndex = kNPnodeRootPin;
					else if (data->map.nodeRootIndex < kNPnodeRootPin)	//select last pin
						data->map.nodeRootIndex = data->map.nodeRootCount - 1;

					//select next node by root index
					node = data->map.node[data->map.nodeRootIndex];
					npSelectNode (node, data);

					// set the examiner target on the selected root node
					npSetCamTarget (data);
				}
				else
				{						//select next sibling
					nodeParent = node->parent;

					if  (data->io.key.modShift)
						nodeParent->childIndex--;
					else
						nodeParent->childIndex++;
		
					// keep in bounds and roll over between first and last
					if (nodeParent->childIndex >= nodeParent->childCount)
						nodeParent->childIndex = 0;
					else if (node->childIndex < 0)
						nodeParent->childIndex = nodeParent->childCount - 1;

					// select our next sibling node and make active
					node = nodeParent->child[nodeParent->childIndex];
					npSelectNode (node, data);
				}
			}
			node = data->map.currentNode;

			if (node->recordID)
				sprintf(msg, "pin id: %d   recordID: %d", node->id, node->recordID);
			else
				sprintf(msg, "pin id: %d", node->id);
			npPostMsg (msg, kNPmsgCtrl, dataRef);
			break;

		case kNPcmdNextBranch : 
			if (data->io.key.modShift)
			{
				if (node->branchLevel > 0)		// do nothing if at the trunk
				{
					// traverse up 1 level to the childs parent
					if (node->parent == NULL)
						npPostMsg ("err 3582 - child node has NULL parent", kNPmsgErr, dataRef);
					else
						node = node->parent;
				}
			}		// traverse down 1 level if selected child exists
			else if (node->childCount)
			{
				if (node->child[node->childIndex] != NULL)
					node = node->child[node->childIndex];
				else
					npPostMsg("err 3583 - child is NULL",kNPmsgErr,data);
			}

			npSelectNode (node, data);
			
			if (node->type == kNodeCamera)
				sprintf(msgPart, "camera ");
			if (node->type == kNodeGrid)
				sprintf(msgPart, "grid ");
			if (node->type == kNodePin)
				sprintf(msgPart, "pin ");

			if (node->recordID)
				sprintf(msg, "%sid: %d   branchLevel: %d   recordID: %d", msgPart,
						node->id, node->branchLevel, node->recordID);
			else
				sprintf(msg, "%sid: %d   branchLevel: %d", msgPart,
						node->id, node->branchLevel);
			npPostMsg (msg, kNPmsgCtrl, dataRef);
			break;
				
		case kNPcmdGrid :
			if (data->io.mouse.pickMode != kNPpickModeGrid)
			{
				data->io.mouse.pickMode = kNPpickModeGrid;
				npPostMsg("mode: Grid", kNPmsgCtrl, dataRef);
			}
			if (data->map.nodeRootIndex == kNPnodeRootGrid)	// grid selected
			{
				if (node == data->map.node[kNPnodeRootGrid])
				{
					if (node->child[0] != NULL)
					{
						node->childIndex = 0;		//select first child cam
						node = node->child[node->childIndex];
					}
				}
				else 
				{
					nodeParent = node->parent;
					if (nodeParent != NULL)
					{								// select next grid
						nodeParent->childIndex++;
						if (nodeParent->childIndex >= nodeParent->childCount)
						{
							nodeParent->childIndex = 0;
							node = data->map.node[kNPnodeRootGrid];
						}
						else
							node = nodeParent->child[nodeParent->childIndex];
					}
					else	//only happens if addtiional root gris created
					{
						node = data->map.node[kNPnodeRootGrid];
						sprintf(msgPart, "Select root ");
					}
				}	
				npSelectNode (node, data);		//update grid selection
			}
			else	// make the selected grid active
			{
				node = data->map.selectedGrid;
				if (node == NULL)
				{
					node = data->map.node[kNPnodeRootGrid];
					npPostMsg("err 7288 - selectedGrid is NULL",kNPmsgErr,data);
				}
				npSelectNode (node, data);		//update grid selection
			}

			sprintf(msg, "%sgrid id: %d", msgPart, node->id);
			npPostMsg (msg, kNPmsgCtrl, dataRef);
			break;

		case kNPcmdCamera :
			if (data->io.mouse.pickMode != kNPpickModeCamera)
			{
				data->io.mouse.pickMode = kNPpickModeCamera;
				npPostMsg("mode: Camera", kNPmsgCtrl, dataRef);
			}
			if (data->map.nodeRootIndex == kNPnodeRootCamera)	//camera selected
			{
				nodeParent = node->parent;
				// reset the camera if shift held, else switch cameras
				if (data->io.key.modShift)
				{
					if (node->parent != NULL)
						npDataCameraPreset (nodeParent->childIndex, node, data);
					else
						npDataCameraPreset (0, node, data);

					sprintf(msgPart, "Reset ");
				}
				else
				{
					if (node == data->map.node[kNPnodeRootCamera])
					{
						if (node->child[0] != NULL)
						{
							node->childIndex = 0;		//select first child cam
							node = node->child[node->childIndex];
						}
					} 
					else 
					{
						if (nodeParent != NULL)
						{	// select next camera
							nodeParent->childIndex++;
							if (nodeParent->childIndex >= nodeParent->childCount)
							{
								nodeParent->childIndex = 0;
								node = data->map.node[kNPnodeRootCamera];
							}
							else
								node = nodeParent->child[nodeParent->childIndex];
						}
						else //only would occur if addtiional root cameras created
						{
							node = data->map.node[kNPnodeRootCamera];
							npPostMsg("Select auxillary root",kNPmsgCtrl,data);
						}
					}
					// set active camera and node
					npSelectNode (node, data);		//update cam selection
				}
			}
			else	// select the current camera if already selected
			{
				node = data->map.activeCam;
				if (node == NULL)
				{
					node = data->map.node[kNPnodeRootCamera];
					npPostMsg("err 7289 - activeCam is NULL",kNPmsgErr,data);
				}
				
				//reset the camera if shift held, else switch cameras
				nodeParent = node->parent;
				if (data->io.key.modShift)
				{
					if (nodeParent != NULL)
						npDataCameraPreset (nodeParent->childIndex, node, data);
					else
						npDataCameraPreset (0, node, data);
				}
				else
					npSelectNode (node, data);		//update cam selection
			}
			sprintf(msg, "%scamera id: %d", msgPart, node->id);
			npPostMsg(msg, kNPmsgCtrl, dataRef);
			break;

		case kNPcmdSelectAxes :
			if (data->io.axes.x == true 
				&& data->io.axes.y == true
				&& data->io.axes.z == true )
			{
				data->io.axes.z = false;
				sprintf(msg, "axes: XY");				// make part of HUD debug, zz
			}
			else if (data->io.axes.x == true 
					 && data->io.axes.y == true
					 && data->io.axes.z == false )
			{
				data->io.axes.x = true; 
				data->io.axes.y = false; 
				data->io.axes.z = false;
				sprintf(msg, "axes: X");
			}
			else if (data->io.axes.x == true 
					 && data->io.axes.y == false
					 && data->io.axes.z == false )
			{
				data->io.axes.x = false; 
				data->io.axes.y = true; 
				data->io.axes.z = false;
				sprintf(msg, "axes: Y");
			}
			else if (data->io.axes.x == false 
					 && data->io.axes.y == true
					 && data->io.axes.z == false )
			{
				data->io.axes.x = false; 
				data->io.axes.y = false; 
				data->io.axes.z = true;
				sprintf(msg, "axes: Z");
			}
			else if (data->io.axes.x == false 
					 && data->io.axes.y == false
					 && data->io.axes.z == true )
			{
				data->io.axes.x = true; 
				data->io.axes.y = true; 
				data->io.axes.z = true;
				sprintf(msg, "axes: XYZ");
			}
			npPostMsg (msg, kNPmsgCtrl, dataRef);
			break;

		case kNPcmdSelect : node->selected = true; break;
		case kNPcmdSelectOff : node->selected = false; break;
		case kNPcmdSelectToggle : node->selected = 1 - node->selected; break;

		case kNPcmdSelectAll :
			if (data->io.mouse.tool != kNPtoolNull)	//add more tool handling //zz debug
				commandTemp = kNPcmdSelect;
			else
			{	
				data->map.selectAll = 1 - data->map.selectAll; //toggle selectAll
				if (data->map.selectAll)
					commandTemp = kNPcmdSelect;
				else
					commandTemp = kNPcmdSelectOff;
			}
											//turn ON selected for all nodes
			for (i=kNPnodeRootPin; i < data->map.nodeRootCount; i++)
				TraverseTree (commandTemp, data->map.node[i], data);
			
			npSelectNode (node, data);		//restore currentNode

			if (data->io.mouse.tool == kNPtoolHide)
				npPostMsg("Hide ALL Sub-Nodes with branch_level > 1",kNPmsgCtrl,data);
			else if (data->io.mouse.tool == kNPtoolInfo)
				npPostMsg("Toggle ALL Text Tags",kNPmsgCtrl,data);
			else
				npPostMsg("Select ALL",kNPmsgCtrl,data);
			break;

		case kNPcmdSelectNone :
			data->map.selectAll = false;
											//turn OFF selected for all nodes
			for (i = kNPnodeRootPin; i < data->map.nodeRootCount; i++)
				TraverseTree (kNPcmdSelectOff, data->map.node[i], data);
			
			npSelectNode (node, data);		//restore currentNode
			
			if (data->io.mouse.tool == kNPtoolHide)
				npPostMsg("Show ALL Nodes",kNPmsgCtrl,data);
			else if (data->io.mouse.tool == kNPtoolInfo)
				npPostMsg("Hide ALL Text Tags",kNPmsgCtrl,data);
			else
				npPostMsg("Clear Selection",kNPmsgCtrl,data);
			break;

		case kNPcmdSelectOne :
			data->map.selectSet.x = 1 - data->map.selectSet.x;
			break;
		case kNPcmdSelectTwo :
			data->map.selectSet.y = 1 - data->map.selectSet.y;
			break;
		case kNPcmdSelectThree :
			data->map.selectSet.z = 1 - data->map.selectSet.z;
			break;

		case kNPcmdSelectSetNone :
			data->map.selectSet.x = false;
			data->map.selectSet.y = false;
			data->map.selectSet.z = false;
			break;

		//add proper key handling for modifier keys to map to any command, //zz debug
		case kNPcmdHide :
//			if (data->io.key.modAlt)
			{
				if (data->io.mouse.tool == kNPtoolHide)
				{
					data->io.mouse.tool = kNPtoolNull;
					npPostMsg ("mouse tool: Disable Hide", kNPmsgCtrl, dataRef);
				}
				else
				{
					data->io.mouse.tool = kNPtoolHide;
					npPostMsg ("mouse tool: Hide", kNPmsgCtrl, dataRef);
				}
				break;
			}
		case kNPcmdTagType :
//			if (data->io.key.modAlt)
			{
				if (data->io.mouse.tool == kNPtoolInfo)
				{
					data->io.mouse.tool = kNPtoolNull;
					npPostMsg ("mouse tool: Disable Info", kNPmsgCtrl, dataRef);
				}
				else
				{
					data->io.mouse.tool = kNPtoolInfo;
					npPostMsg ("mouse tool: Info", kNPmsgCtrl, dataRef);
				}
				break;
			}
		default : break;
	}
}

//zz-JJ
//------------------------------------------------------------------------------
void npCtrlChannel (int command, NPnodePtr node, void* dataRef)
{
	char msg[128];
	pData data = (pData) dataRef;

	//when changing channels we need to remove from ch list, unless ch is zero
	if (node->chInputID != 0)
		npChRemoveNode (node, dataRef);

	if (command == kNPcmdChannelUp)
	{
		node->chInputID = npChGetChannelAfter(&data->io.ch, node->chInputID);
		sprintf (msg, "immediately after npChGetChannelAfter, channel: %d", node->chInputID );
		npPostMsg (msg, kNPmsgDebug, dataRef);
	}
	else //assume command == kNPgraphChannelDown
		node->chInputID = npChGetChannelBefore(&data->io.ch, node->chInputID);

	npPostMsg ("..calling npChSubscribeNode", kNPmsgDebug, dataRef);
	npChSubscribeNode (node, dataRef);

	sprintf (msg, "channel: %d", node->chInputID );
	npPostMsg (msg, kNPmsgCtrl, dataRef);
}

//------------------------------------------------------------------------------
void npCtrlProperty (int command, void* dataRef)
{
	static char msg[256];
	static char msgPart[256];

	pData data = (pData) dataRef;
	NPnodePtr node = data->map.currentNode;
	NPnodePtr nodeParent = NULL;


	msgPart[0] = '\0';

	switch (command)
	{
		case kNPcmdChannelUp : npCtrlChannel (command, node, data); break;
		case kNPcmdChannelDown : npCtrlChannel (command, node, data); break;

		case kNPcmdColorUp :
			node->colorIndex++;
			SetIndexColor (&node->color, &node->colorIndex);
			sprintf(msg, "color index: %d   r: %d   g: %d   b: %d",
				node->colorIndex, node->color.r, node->color.g, node->color.b);
			npPostMsg (msg, kNPmsgCtrl, dataRef);
			break;
		case kNPcmdColorDown :
			node->colorIndex--;
			SetIndexColor (&node->color, &node->colorIndex);
			sprintf(msg, "color index: %d   r: %d   g: %d   b: %d",
				node->colorIndex, node->color.r, node->color.g, node->color.b);
			npPostMsg (msg, kNPmsgCtrl, dataRef);
			break;

		case kNPcmdAlphaUp :
			if ((node->color.a + 10) > 255)
				node->color.a = 255;
			else
				node->color.a += 10;
			sprintf(msg, "alpha: %d",node->color.a);
			npPostMsg (msg, kNPmsgCtrl, dataRef);
			break;
		case kNPcmdAlphaDown :
			if ((node->color.a - 10) < 0)
				node->color.a = 0;
			else
				node->color.a -= 10;
			sprintf(msg, "alpha: %d",node->color.a);
			npPostMsg (msg, kNPmsgCtrl, dataRef);
			break;

		case kNPcmdTexture :
			if (data->io.key.modShift)
				node->textureID--;
			else
				node->textureID++;
			if (node->textureID < 0)
				node->textureID = data->io.gl.textureCount;
			if (node->textureID > data->io.gl.textureCount)
				node->textureID = 0;

			sprintf(msg, "textureID: %d", node->textureID);
			npPostMsg (msg, kNPmsgCtrl, dataRef);
			break;

//		case kNPcmdCenter : node->center = 1 - node->center; break;

		case kNPcmdLines :
			node->lineWidth += 1.0f;
			if (node->lineWidth > 5.0f)
				node->lineWidth = 0.0f;
			break;
		case kNPcmdPoints :
			node->pointSize += 1.0f;
			if (node->pointSize > 5.0f)
				node->pointSize = 0;
			break;

		case kNPcmdPrimitive :
			if (data->io.key.modShift)
				node->geometry--;
			else
				node->geometry++;

			if (node->geometry > kNPprimitiveSolidPin)
				node->geometry = 0;
			if (node->geometry < 0)
				node->geometry = kNPprimitiveSolidPin;
			npSetTagOffset (node);
			sprintf(msg, "geometry: %d", node->geometry);
			npPostMsg (msg, kNPmsgCtrl, dataRef);
			break;

		case kNPcmdTopo :
			if (data->io.key.modShift)
			{
				node->facet++;
				if (node->facet > kNPfacetCube)
					node->facet = 1;
				sprintf(msg, "facet: %d", node->facet);
				npPostMsg (msg, kNPmsgCtrl, dataRef);
			}
			else
			{	
				node->topo++;

				if (node->topo > 4)//kNPtopoCount)						//zz debug
					node->topo = 0;
		
				switch (node->topo)
				{
					case kNPtopoNull :
						if (node->branchLevel == 0)
							node->geometry = kNPprimitiveSolidPin;
						else
							node->geometry = kNPglutSolidTorus;
						sprintf(msg, "topo: %d default", node->topo); 
						break;
					case kNPtopoCube : 
						node->geometry = kNPglutSolidCube;
						sprintf(msg, "topo: %d cube", node->topo);
						break;
					case kNPtopoSphere :
						node->geometry = kNPglutSolidSphere;
						sprintf(msg, "topo: %d sphere", node->topo); 
						break;
					case kNPtopoTorus : 
						node->geometry = kNPglutSolidTorus;
						sprintf(msg, "topo: %d torus", node->topo); 
						break;
					case kNPtopoSurface : node->topo++; //skip this type for now debug zz //printf("topo: %d surface\n", node->topo); break;
					case kNPtopoPin : 
						node->geometry = kNPprimitiveSolidPin;
						sprintf(msg, "topo: %d pin", node->topo);
						break;
					case kNPtopoGrid :
				//		node->geometry = kNPgeoGrid;				//zz debug
						sprintf(msg, "topo: %d grid", node->topo);
						break;
					default : sprintf(msg, "topo: %d other", node->topo); break;
				}
				npSetTagOffset (node);
				npPostMsg (msg, kNPmsgCtrl, dataRef);
			}
			break;

		case kNPcmdShader :
			if (data->io.key.modShift)
				node->shader--;
			else
				node->shader++;
			// handle roll-over
			if (node->shader >= kShadingCount)
				node->shader = 0;
			if (node->shader < 0)
				node->shader = kShadingCount - 1;
			break;

//		case kNPcmdMesh : node->geometry = kSurfaceMesh; break;
//		case kNPcmdScroll : node->scroll = 1 - node->scroll; break;
		case kNPcmdFreeze :
			node->freeze = 1 - node->freeze;
			if (node->freeze)
				npPostMsg ("freeze", kNPmsgCtrl, dataRef);
			else
				npPostMsg ("unfreeze", kNPmsgCtrl, dataRef);
			break;
		case kNPcmdHide :
			node->hide = 1 - node->hide;
			if (node->hide)
				npPostMsg ("Hide", kNPmsgCtrl, dataRef);
			else
				npPostMsg ("Un-Hide", kNPmsgCtrl, dataRef);
			break;

		case kNPcmdClear :
			node->triggerLo.x = false;
			node->triggerLo.y = false;
			node->triggerLo.z = false;
			node->triggerHi.x = false;
			node->triggerHi.y = false;
			node->triggerHi.z = false;
			npPostMsg ("Clear Limits", kNPmsgCtrl, dataRef);
			break;

		case kNPcmdSampleInterval : break;

		case kNPcmdSegments : 
			if (data->io.key.modShift)
			{
				if (data->io.axes.x)
				{
					node->segments.x--;
					if (node->segments.x < 0)
						node->segments.x = 0;
				}
				if (data->io.axes.y)
				{
					node->segments.y--;
					if (node->segments.y < 0)
						node->segments.y = 0;
				}
				if (data->io.axes.z)
				{
					node->segments.z--;
					if (node->segments.z < 0)
						node->segments.z = 0;
				}
			}
			else
			{
				if (data->io.axes.x)
					node->segments.x++;
				if (data->io.axes.y)
					node->segments.y++;
				if (data->io.axes.z)
					node->segments.z++;
			}
			sprintf(msg, "segments x: %d   y: %d   z: %d",
					node->segments.x, node->segments.y, node->segments.z );
			npPostMsg (msg, kNPmsgCtrl, dataRef);
			break;

		case kNPcmdSetpointLo :
			sprintf(msg, "Set Lower Limit ");
			if (data->io.key.modShift)
			{
				node->triggerLo.x = false;
				node->triggerLo.y = false;
				node->triggerLo.z = false;
				strcat(msg, "OFF");
			}
			else
			{
				if (data->io.axes.x)
				{
					node->setLo.x = node->translate.x;
					node->triggerLo.x = true;
					sprintf(msgPart, "x: %f   ", node->setLo.x);
					strcat(msg, msgPart);
				}
				if (data->io.axes.y)
				{
					node->setLo.y = node->translate.y;
					node->triggerLo.y = true;
					sprintf(msgPart, "y: %f   ", node->setLo.y);
					strcat(msg, msgPart);
				}
				if (data->io.axes.z)
				{
					node->setLo.z = node->translate.z;
					node->triggerLo.z = true;
					sprintf(msgPart, "z: %f   ", node->setLo.z);
					strcat(msg, msgPart);
				}
			}
			npPostMsg (msg, kNPmsgCtrl, dataRef);
			break;
		case kNPcmdSetpointHi :
			sprintf(msg, "Set Upper Limit ");
			if (data->io.key.modShift)
			{
				node->triggerHi.x = false;
				node->triggerHi.y = false;
				node->triggerHi.z = false;
				strcat(msg, "OFF");
			}
			else
			{
				if (data->io.axes.x)
				{
					node->setHi.x = node->translate.x;
					node->triggerHi.x = true;
					sprintf(msgPart, "x: %f   ", node->setHi.x);
					strcat(msg, msgPart);
				}
				if (data->io.axes.y)
				{
					node->setHi.y = node->translate.y;
					node->triggerHi.y = true;
					sprintf(msgPart, "y: %f   ", node->setHi.y);
					strcat(msg, msgPart);
				}
				if (data->io.axes.z)
				{
					node->setHi.z = node->translate.z;
					node->triggerHi.z = true;
					sprintf(msgPart, "z: %f   ", node->setHi.z);
					strcat(msg, msgPart);
				}
			}
			npPostMsg (msg, kNPmsgCtrl, dataRef);
			break;

		case kNPcmdTagType :
			node->tagMode++;
			if (node->tagMode >= kNPtagModeCount)					//debug zz
				node->tagMode = 0;
			sprintf(msg, "text tag type: %d", node->tagMode);
			npPostMsg (msg, kNPmsgCtrl, dataRef);
			break;

		case kNPcmdRatio :					// torus inner radius 
			if (data->io.key.modShift)
			{
				node->ratio -= 0.01f;
				if (node->ratio < 0.01f)
					node->ratio = 0.01f;
			}
			else
			{
				node->ratio += 0.01f;
				if (node->ratio > 1.0f)
					node->ratio = 1.0f;
			}
			sprintf(msg, "ratio: %f", node->ratio);
			npPostMsg (msg, kNPmsgCtrl, dataRef);
			break;

		default : break;
	}
}

//------------------------------------------------------------------------------
void npCtrlGlobal (int command, void* dataRef)
{
	static char msg[256];

	pData data = (pData) dataRef;

	NPnodePtr node = data->map.node[data->map.nodeRootIndex];

	switch (command)
	{
		case kNPcmdMenu : //data->io.drawMenu = 1 - data->io.drawMenu;
			data->io.gl.hud.console.mode++;
			if (data->io.gl.hud.console.mode >= kNPconsoleModeCount)
				data->io.gl.hud.console.mode = 0;
			npResizeConsole(data);
			break;

		case kNPcmdFullscreen : npglFullscreen(data); break;

		case kNPcmdClearFrameBuffer :
			data->io.clearFrameBuffer = 1 - data->io.clearFrameBuffer;
			break;

		case kNPcmdBackground :
			if (data->io.clearIndex)
			{
				data->io.clear.r = 0.0f;
				data->io.clear.g = 0.0f;
				data->io.clear.b = 0.0f;
				data->io.clear.a = 0.0f;
				
				data->io.clearIndex = 0;
				npPostMsg ("Background Black", kNPmsgCtrl, dataRef);
			}
			else
			{
				data->io.clear.r = 1.0f;
				data->io.clear.g = 1.0f;
				data->io.clear.b = 1.0f;
				data->io.clear.a = 1.0f;

				data->io.clearIndex = 1;
				npPostMsg ("Background White", kNPmsgCtrl, dataRef);
			}
			break;

		case kNPcmdNormalMode : 
			data->io.gl.normal++;
				if (data->io.gl.normal >= kNPglNormalSize)
				{
					data->io.gl.normal = kNPglNormalNull;
					npPostMsg ("Disable Normalize: no rescaling", kNPmsgCtrl, dataRef);
				}
				else
					npPostMsg ("Enable Normalize: rescale normals", kNPmsgCtrl, dataRef);
			break;

		case kNPcmdShaderMode :
			data->io.gl.shade++;
			if (data->io.gl.shade >= kNPglShadeSize)
				data->io.gl.shade = kNPglShadeSmooth;
			sprintf(msg, "shader mode: %d", data->io.gl.alphaMode);
			npPostMsg (msg, kNPmsgCtrl, dataRef);
			break;

		case kNPcmdAlphaMode :
			data->io.gl.alphaMode++;
			if (data->io.gl.alphaMode >= kNPalphaModeCount)
				data->io.gl.alphaMode = kNPalphaModeNull;

			sprintf(msg, "alpha mode: %d Transparency ", data->io.gl.alphaMode);
			switch (data->io.gl.alphaMode)
			{
				case kNPalphaModeNull : strcat(msg, "OFF"); break;
				case kNPalphaModeSubtractive : strcat(msg, "Subtractive"); break;
				case kNPalphaModeAdditive : strcat(msg, "Additive"); break;
				case kNPalphaModeDark : strcat(msg, "Dark"); break;
			}
			npPostMsg (msg, kNPmsgCtrl, dataRef);
			break;

		case kNPcmdPause :	
			npchPause(data);
			sprintf(msg, "pause: %d\n", data->io.ch.pause);
			npPostMsg (msg, kNPmsgCtrl, dataRef);
			break;

		default : break;
	}
}

//------------------------------------------------------------------------------
void npUpdateCtrl (void* dataRef)
{
	bool flag = false;
	int i = 0;
	static int firstRunLoop = true;
	static char msg[256];

	pData data = (pData) dataRef;


	npUpdateIO (dataRef);			//update IO, mouse, sound, txt, video...
	npUpdateData (dataRef);			//update our data map, logical tracks
	npUpdateEngine (dataRef);		//position, physics, interactions...

	npDispatchMessages (dataRef);

	// zero out our mouse to prevent drifting objects
	data->io.mouse.delta.x = 0.0f;											//debug, zz
	data->io.mouse.delta.y = 0.0f;

	// check for command line arguements if first runloop
	if (firstRunLoop)
	{
		firstRunLoop = false;

		// if exist, load command line data files passed in
		for (i=1; i < data->io.argc; i++)		//start at arg 1, 0 is the exe name
		{
			npPostMsg (data->io.argv[i], kNPmsgCtrl, dataRef);
			// check to see if URL, if so set the flag
			// supports both http and local files
			if ( strncmp("http", data->io.argv[i], 4) == 0 )
	//			|| strncmp("file:///", data->io.argv[i], 8) == 0 ) //debug, zz cannot pass parameters to local file
				flag = true;

			sprintf(msg, "flag: %d", flag);
			npPostMsg (msg, kNPmsgCtrl, dataRef);
			// if URL then copy the string, otherwise assume its a CSV file
			if (flag)
				strcpy( data->io.url, data->io.argv[i] );
			else
				npFileOpenMap (data->io.argv[i], 1, 13, dataRef);
		}
	}
}

//process and dispatch new messages based on filter settings
//------------------------------------------------------------------------------
void npDispatchMessages (void* dataRef)
{
	int index = 0;
	int type = 0;
	char* msg = NULL;
	pData data = (pData) dataRef;
	pNPconsole console = (pNPconsole) &data->io.gl.hud.console;


	//insert additional error handling here that requires action, debug zz

	if (data->io.message.queIndex < 1)	//exit if no new messages in the que
		return;

	index = 1;

	while (index <= data->io.message.queIndex)
	{
		msg = &data->io.message.que[index++][0];	//[index].msg;			//zz debug
//		type = &data->io.message.que[index++].type;

		//print to system console, adds newline at end of each msg	
		printf("%s\n", msg);

		//GUI console, formatted as single lines with 80 char max length, for speed
		//add handling for newlines using a loop, debug, zz
		//handle console buffer roll-over
		console->lineIndex++;

		if (console->lineIndex >= kNPconsoleLineMax)
			console->lineIndex = 0;

		//break the message up into individual 80 char lines, remove newlines
		//if (strlen(msg) > kNPconsoleCharPerLine)							//debug zz

		strncpy(&console->line[console->lineIndex][0], msg, kNPconsoleCharPerLine);								
	}

	//log file, adds newline at end of each msg

	data->io.message.queIndex = 0;	//add que locking... for thread safety, debug zz

	return;								//move this to control or maybe data...
}


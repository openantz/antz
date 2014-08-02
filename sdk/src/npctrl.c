/* -----------------------------------------------------------------------------
*
*  npctrl.c
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

#include "npctrl.h"

#include "npdata.h"
#include "npio.h"

#include "data/npnode.h"

#include "io/npgl.h"
#include "io/npfile.h"
#include "io/npch.h"
#include "io/npmouse.h"

#include "io/net/nposc.h"

//#include "io/db/npmysql.h"	//zz dbz
#include "io/npdb.h"			//zz db

#include "ctrl/npcmd.h"
#include "ctrl/npengine.h"

#include "os/npos.h"

#include "io/npfile.h"


void npInitCPU (void* dataRef);

void npCtrlFile (int command, void* dataRef);

void npCtrlVector (int command, void* dataRef);
void npCtrlSelect (int command, void* dataRef);
void npCtrlProperty (int command, void* dataRef);
void npCtrlGlobal (int command, void* dataRef);
void npCtrlChannel (int command, pNPnode node, void* dataRef);

void TraverseTree (int command, pNPnode node, void* dataRef);		//zz debug move this npmap

void npDispatchMessages (void* dataRef);

bool ctrlInitialized = false;		//prevents calls to CtrlCommand before init

//------------------------------------------------------------------------------
void npInitCtrl (void* dataRef)
{
	pData data = (pData) dataRef;

	npInitCPU (dataRef);

	npInitEngine (dataRef);

	ctrlInitialized = true;			//now safe to call CtrlCommand
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
void TraverseTree (int command, pNPnode node, void* dataRef)
{
	int i = 0;
	
	pData data = (pData) dataRef;

	// recursively iterate through all nodes in the tree
	for (i=0; i < node->childCount ; i++)
	{
		if ( node->type != kNodeLink				//prevents infinite loop, zz debug, should be safe to remove now
			&& node->child[i]->child[0] != node )	//prevents double command update
			TraverseTree (command, node->child[i], dataRef);
	}
	
	if (data->io.mouse.tool == kNPtoolTag)
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
	}else 
		
	//	if (//data->io.mouse.tool == kNPtoolCombo ||
	//	 !(command == kNPcmdSelect || command == kNPcmdSelectOff) )
	//{
		if ( node->selected 
			 || command == kNPcmdSelect 
			 || command == kNPcmdSelectOff )
		{
			data->map.currentNode = node;
			npCtrlCommandSet (command, node, dataRef);
		}
	//}
}

//------------------------------------------------------------------------------
void npCtrlCommand (int command, void* dataRef)
{
	int i = 0;
	int indexTemp = 0;

	pData data = (pData) dataRef;
	pNPnode node = data->map.currentNode;

	if ( !ctrlInitialized )	//safety catch for startup procedures
	{
		printf ("err 6238 -  npCtrlCommand called before initialized\n");
		return;
	}

	switch (command)
	{
		// kCmdGlobal... global commands, draw indicators/HUD, quit
//		case kQuit : npCtrlGlobal (command, dataRef); break;

		case kNPcmdFileNew : npCtrlFile (command, dataRef); break;
		case kNPcmdFileOpen : npCtrlFile (command, dataRef); break;
		case kNPcmdFileClose : npCtrlFile (command, dataRef); break;
		case kNPcmdFileSave : npCtrlFile (command, dataRef); break;
		case kNPcmdFileSaveAs : npCtrlFile (command, dataRef); break;
		case kNPcmdFileImport : npCtrlFile (command, dataRef); break;
		case kNPcmdFileExport : npCtrlFile (command, dataRef); break;
		case kNPcmdScreenGrab : npCtrlFile (command, dataRef); break;
		case kNPcmdFileMapOne : npCtrlFile (command, dataRef); break;
		case kNPcmdFileMapTwo : npCtrlFile (command, dataRef); break;
		case kNPcmdFileMapThree : npCtrlFile (command, dataRef); break;

		case kNPcmdOpenURL : npCtrlFile (command, dataRef); break;
		case kNPcmdOpenApp : npCtrlFile (command, dataRef); break;
		case kNPcmdOpenAntz : npCtrlFile (command, dataRef); break;

		case kNPcmdViewer : npCtrlFile (command, dataRef); break;
		case kNPcmdConsole : npCtrlGlobal (command, dataRef); break;

		case kNPcmdMenu : npCtrlGlobal (command, dataRef); break;
		case kNPcmdSubsample : npCtrlGlobal (command, dataRef); break;			//zzhp
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

		case kNPcmdNext : npCtrlSelect (command, dataRef); break;
		case kNPcmdNextOff : npCtrlSelect (command, dataRef); break;
		case kNPcmdPrev : npCtrlSelect (command, dataRef); break;
		case kNPcmdPrevOff : npCtrlSelect (command, dataRef); break;
		case kNPcmdNextBranch : npCtrlSelect (command, dataRef); break;
		case kNPcmdNextBranchOff : npCtrlSelect (command, dataRef); break;
		case kNPcmdPrevBranch : npCtrlSelect (command, dataRef); break;
		case kNPcmdPrevBranchOff : npCtrlSelect (command, dataRef); break;

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

		case kNPcmdTool : npCtrlSelect (command, dataRef); break;
		case kNPcmdToolDown : npCtrlSelect (command, dataRef); break;

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
	//		if (data->io.key.modAlt)
	//		{
	//			npCtrlSelect (command, dataRef);
	//			break;
	//		}

			// not a global command
			// exclude global commands or only 1 selected node from applying to all nodes
			// ie. allows for adding an unsellected node to the current group
			if ( ( !node->selected
				 || data->map.nodeRootIndex < kNPnodeRootPin )
				 && data->io.mouse.tool != kNPtoolSelect ) //&& node != data->map.currentCam)//zz debug
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
	pNPnode node = (pNPnode) nodeRef;


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
		case kNPcmdTextureDown : npCtrlProperty (command, dataRef); break;
		case kNPcmdCenter : npCtrlProperty (command, dataRef); break;
		case kNPcmdLines : npCtrlProperty (command, dataRef); break;
		case kNPcmdPoints : npCtrlProperty (command, dataRef); break;
		case kNPcmdShader : npCtrlProperty (command, dataRef); break;
		case kNPcmdGeometry : npCtrlProperty (command, dataRef); break;
		case kNPcmdPrimitiveDown : npCtrlProperty (command, dataRef); break;
		case kNPcmdTopo : npCtrlProperty (command, dataRef); break;
		case kNPcmdTopoDown : npCtrlProperty (command, dataRef); break;
		case kNPcmdMesh : npCtrlProperty (command, dataRef); break;
		case kNPcmdScroll : npCtrlProperty (command, dataRef); break;
		case kNPcmdFreeze : npCtrlProperty (command, dataRef); break;
		case kNPcmdHide : npCtrlProperty (command, dataRef); break;
		case kNPcmdClear : npCtrlProperty (command, dataRef); break;

		case kNPcmdTagMode : npCtrlProperty (command, dataRef); break;

		case kNPcmdSegments : npCtrlProperty (command, dataRef); break;

		case kNPcmdSetpointLo : npCtrlProperty (command, dataRef); break;
		case kNPcmdSetpointLoOff : npCtrlProperty (command, dataRef); break;
		case kNPcmdSetpointHi : npCtrlProperty (command, dataRef); break;
		case kNPcmdSetpointHiOff : npCtrlProperty (command, dataRef); break;

		case kNPcmdRatio : npCtrlProperty (command, dataRef); break;	
			
		default : break;
	}
}

//------------------------------------------------------------------------------
void npCtrlFile (int command, void* dataRef)
{
	int result = 0;
	static char msg[kNPmaxPath];
	static char name[kNPmaxPath];

	FILE* file = NULL;

	pData data = (pData) dataRef;

	name[0] = '\0';

	switch (command)
	{
		case kNPcmdFileNew :
			npFileDialog (NULL, kNPfileDialogNew, dataRef);
			break;

		//navigate folders, files and DBs
		case kNPcmdViewer :
			if (data->io.key.modAlt )
			{
				if (data->io.key.modShift) //save to default server with default timestamp DB name
					npdbSaveUpdate( dataRef );						//zz db
				else
					//npdbLoadUpdate( dataRef );
					npdbPullScene( dataRef );						//zz db
			}
			else if (data->io.key.modShift) //save to default server with default timestamp DB name
				npdbSaveScene( dataRef );						//zz db
			else
				npViewer( dataRef );	//load scene from menu list
			break;

		case kNPcmdFileOpen :
			npFileBrowser (dataRef);
			break;
		case kNPcmdFileClose :
			npFileDialog (NULL, kNPfileDialogClose, dataRef);
			break;
		case kNPcmdFileSave :
			nposTimeStampName( name );
			npSaveScene( kNPmapCSV, name, data );
			break;
		case kNPcmdFileSaveAs :
			npFileDialog (NULL, kNPfileDialogSaveAs, dataRef);
			break;

		case kNPcmdScreenGrab :
			data->io.gl.screenGrab = 1; //kNPscreenGrabWindow ...Full ..Thumb
			break;

		case kNPcmdFileMapOne :
			strcpy( name, "antz0001" );			//file set root name
			if (data->io.key.modShift)
				npSaveScene( kNPmapCSV, name, data );
			else
				npLoadScene( kNPmapCSV, name, data );
			break;

		case kNPcmdFileMapTwo :
			strcpy( name, "antz0002" );			//file set root name
			if (data->io.key.modShift)
				npSaveScene(kNPmapCSV, name, data);
			else
				npLoadScene(kNPmapCSV, name, data);
			break;

		case kNPcmdFileMapThree :
			strcpy( name, "antz0003" );			//file set root name
			if (data->io.key.modShift)
				npSaveScene(kNPmapCSV, name, data);
			else
				//npLoadScene(kNPmapCSV, name, data);
				npNewDirTree(data->io.file.appPath, NULL, dataRef);		//zz file tree
			break;

		case kNPcmdFileImport :
			strcpy (name, data->io.file.csvPath);
			if (data->io.key.modShift)
			{
				strcat (name, "antzchmap0001.csv");					//zz debug, why 2 and not 1?
				npSaveChMap (name, dataRef);	//this is a chmap TEST save function, thats why 2
			}
			else			// load set of channel files, Metadata, Tracks and ChMap
			{
				strcat (name, "antzchset0001.csv");			
				npFileOpenChSet (name, dataRef);
			}
			npPostTool (NULL, data);												//zz-s
			break; //npFileDialog (NULL, kNPfileDialogImport, dataRef); break;
		case kNPcmdFileExport : npFileDialog (NULL, kNPfileDialogExport, dataRef); break;

		case kNPcmdOpenURL : npOpenURL (NULL, dataRef); break;
		case kNPcmdOpenNodeFile : npOpenNodeFile (NULL, dataRef); break;
		case kNPcmdOpenApp : npOpenApp (NULL, dataRef); break;
		case kNPcmdOpenAntz : npOpenAntz (NULL, dataRef); break;

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
	pNPnode node = data->map.currentNode;		//apply command to currentNode


	//exit if in cam mode and this is not a camera node
	if (data->io.mouse.pickMode == kNPmodeCamera)
		if (node != data->map.currentCam)
			return;

	// if shift pressed then use fast velocity, otherwise slow
	if (data->io.key.modShift)
		velocity = data->ctrl.fast;
	else
		velocity = data->ctrl.slow;

	//zz debug, temp workaround for ALTernate key mapping
	if( data->io.key.modAlt )
	{		  printf("text alt-rotate id: %d\n", node->id);
	  switch( command )
	  {	

		case kNPcmdXdecrease :		command = kNPcmdRotateLeft; break;
		case kNPcmdXdecreaseOff :	command = kNPcmdRotateLeftOff; break;
		case kNPcmdXincrease :		command = kNPcmdRotateRight; break;
		case kNPcmdXincreaseOff :	command = kNPcmdRotateRightOff; break;

		case kNPcmdYdecrease :		command = kNPcmdRotateUp; break;
		case kNPcmdYdecreaseOff :	command = kNPcmdRotateUpOff; break;
		case kNPcmdYincrease :		command = kNPcmdRotateDown; break;
		case kNPcmdYincreaseOff :	command = kNPcmdRotateDownOff; break;

		case kNPcmdZdecrease :		command = kNPcmdRotateCCW; break;
		case kNPcmdZdecreaseOff :	command = kNPcmdRotateCCWOff; break;
		case kNPcmdZincrease :		command = kNPcmdRotateCW; break;
		case kNPcmdZincreaseOff :	command = kNPcmdRotateCWOff; break;
		default : break;
	  }
	}
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
	pNPnode node = data->map.currentNode;
	pNPnode nodeChild = NULL;
	pNPnode nodeParent = NULL;


	msg[0] = '\0';
	msgPart[0] = '\0';


	//workaround to allow camera keyboard fly with arrows for rotation		//zz debug
	if( data->io.mouse.pickMode == kNPmodeCamera )
	{
	  switch( command )
	  {					
		case kNPcmdNext :		command = kNPcmdRotateLeft; break;
		case kNPcmdNextOff :	command = kNPcmdRotateLeftOff; break;
		case kNPcmdPrev :		command = kNPcmdRotateRight; break;
		case kNPcmdPrevOff :	command = kNPcmdRotateRightOff; break;

		case kNPcmdNextBranch :		command = kNPcmdRotateUp; break;
		case kNPcmdNextBranchOff :	command = kNPcmdRotateUpOff; break;
		case kNPcmdPrevBranch :		command = kNPcmdRotateDown; break;
		case kNPcmdPrevBranchOff :	command = kNPcmdRotateDownOff; break;

		default : i = 1;  break;
	  }	
	
	  if( !i )
	  {
		npCtrlVector( command, data );
		return;
	  }
	}	//end workaround

	switch (command)
	{						// add ability to copy existing node parameters, debug zz
		case kNPcmdNew :
			//if camera selected and no pins exist then create 1st pin
			if (//data->map.nodeRootIndex == kNPnodeRootCamera &&		//zz debug, workaround for cam move grid bug
				 data->map.nodeRootCount <= kNPnodeRootPin )
			{
				node = npNodeNew (kNodePin, NULL, dataRef);
				npNodeNew (node->type, node, dataRef);
				npSelectNode (node, data);
				npSetCamTarget (data);	//sets camera target to selected node
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
					npSelectNode (node, data);
					npSetCamTarget (data);	//sets camera target to selected node
				}
				else if (node->type == kNodePin)
				{	//if root create new pin, if shift create child
					if (node->branchLevel == 0 
						&& !data->io.key.modShift
						&& node->childCount > 0 )
					{
						node = npNodeNew (kNodePin, NULL, dataRef);
						npNodeNew (node->type, node, dataRef);
						npSelectNode (node, data);
		//				npCtrlSelect(kNPcmdPin, data);
						npSetCamTarget (data);	//sets camera target to selected node
					}
					else
						node = npNodeNew (node->type, node, dataRef);
				}
			}
	//		npSetCamTarget (data);	//sets camera target to selected node

			if (node->type == kNodeCamera)
				sprintf (msgPart, "New camera ");
			if (node->type == kNodeGrid)
				sprintf (msgPart, "New grid ");
			if (node->type == kNodePin)
				sprintf (msgPart, "New pin ");
			sprintf (msg, "%sid: %-7d level: %d", msgPart, node->id, node->branchLevel + 1);
			npPostMsg (msg, kNPmsgCtrl, dataRef);
			break;

		case kNPcmdDelete :		//update to delete all selected nodes //zz debug
			//	if (data->io.mouse.createEvent == false)			//zz-s debug
				npNodeDelete (node, dataRef);
			npSetCamTarget (data);	//sets camera target to selected node
			break;
			
		case kNPcmdPin :
			if (data->io.mouse.pickMode != kNPmodePin)
			{
				data->io.mouse.pickMode = kNPmodePin;
				npPostMsg("mode: Pin", kNPmsgCtrl, dataRef);
				npPostTool (NULL, data);
			}
			if (data->map.nodeRootCount <= kNPnodeRootPin)		// if no nodes select cam
			{
				npSelectNode (data->map.currentCam, data);
				break;
			}
			if (data->map.nodeRootIndex < kNPnodeRootPin)		//camera or grid selected
			{
				npSelectNode (data->map.selectedPinNode, data); //restore previous node
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
					else if (nodeParent->childIndex < 0)
						nodeParent->childIndex = nodeParent->childCount - 1;

					// select our next sibling node and make active
					node = nodeParent->child[nodeParent->childIndex];
					npSelectNode (node, data);
				}
			}
			// set the examiner target on the selected root node
			npSetCamTarget (data);

			node = data->map.currentNode;

			npSelectNode (node, data);
			npSetCamTarget (data);
			npPostNodeID( node, data );
			break;

		case kNPcmdNext :
			if (data->io.mouse.pickMode != kNPmodePin)
			{
				data->io.mouse.pickMode = kNPmodePin;
				npPostMsg("mode: Pin", kNPmsgCtrl, dataRef);
				npPostTool (NULL, data);
			}
			if (data->map.nodeRootCount <= kNPnodeRootPin)		// if no nodes select cam
			{
				npSelectNode (data->map.currentCam, data);
				break;
			}
			if (data->map.nodeRootIndex < kNPnodeRootPin)		//camera or grid selected
			{
				npSelectNode (data->map.selectedPinNode, data); //restore previous node
			}
			else
			{
				if (node->branchLevel == 0)	//root pin
				{
					//if shift+key held then apply skip interval multiplier
					if( data->io.key.modShift )
					{
						data->io.key.skipCount++;
						data->map.nodeRootIndex += (data->io.key.skipCount * data->io.key.skipCount);
					
						//hold at endpoint
						if (data->map.nodeRootIndex >= data->map.nodeRootCount)
								data->map.nodeRootIndex = data->map.nodeRootCount - 1;		//zz debug handle skip remainder roll-over
						else if (data->map.nodeRootIndex < kNPnodeRootPin)	//select last pin
							data->map.nodeRootIndex = kNPnodeRootPin;
		
					}
					else
					{
						data->map.nodeRootIndex++;
						data->io.key.skipCount = 0;
	
						//keep inbounds and rollover if necessary
						if (data->map.nodeRootIndex >= data->map.nodeRootCount)
								data->map.nodeRootIndex = kNPnodeRootPin;		//zz debug handle skip remainder roll-over
						else if (data->map.nodeRootIndex < kNPnodeRootPin)	//select last pin
							data->map.nodeRootIndex = data->map.nodeRootCount - 1;
					}
		
					//select next node by root index
					node = data->map.node[data->map.nodeRootIndex];
					npSelectNode (node, data);
				}
				else
				{						//select next sibling
					nodeParent = node->parent;

										//if shift+key held then apply skip interval multiplier
					if( data->io.key.modShift )
					{
						data->io.key.skipCount++;
						nodeParent->childIndex += (data->io.key.skipCount * data->io.key.skipCount);
										// keep in bounds and roll over between first and last
						if (nodeParent->childIndex >= nodeParent->childCount)
							nodeParent->childIndex = nodeParent->childCount - 1;
					}
					else
					{
						nodeParent->childIndex++;
						data->io.key.skipCount = 0;

						// keep in bounds and roll over between first and last
						if (nodeParent->childIndex >= nodeParent->childCount)
							nodeParent->childIndex = 0;
					}

					// select our next sibling node and make active
					node = nodeParent->child[nodeParent->childIndex];
					npSelectNode (node, data);
				}
			}
			// set the examiner target on the selected root node
			npSetCamTarget (data);

			node = data->map.currentNode;
			npSelectNode (node, data);
			npSetCamTarget (data);
			npPostNodeID( node, data );
			break;
		case kNPcmdNextOff : break;

		case kNPcmdPrev :
			if (data->io.mouse.pickMode != kNPmodePin)
			{
				data->io.mouse.pickMode = kNPmodePin;
				npPostMsg("mode: Pin", kNPmsgCtrl, dataRef);
				npPostTool (NULL, data);
			}
			if (data->map.nodeRootCount <= kNPnodeRootPin)		// if no nodes select cam
			{
				npSelectNode (data->map.currentCam, data);
				break;
			}
			if (data->map.nodeRootIndex < kNPnodeRootPin)		//camera or grid selected
			{
				npSelectNode (data->map.selectedPinNode, data); //restore previous node
			}
			else
			{
				if (node->branchLevel == 0)	//root pin
				{
					//if shift+key held then apply skip interval multiplier
					if( data->io.key.modShift )
					{
						data->io.key.skipCount++;
						data->map.nodeRootIndex -= (data->io.key.skipCount * data->io.key.skipCount);
					
						//keep inbounds and rollover if necessary
						if (data->map.nodeRootIndex < kNPnodeRootPin)	//select last pin
							data->map.nodeRootIndex = kNPnodeRootPin;
					}
					else
					{
						data->map.nodeRootIndex--;
						data->io.key.skipCount = 0;

						//keep inbounds and rollover if necessary
						if (data->map.nodeRootIndex < kNPnodeRootPin)	//select last pin
							data->map.nodeRootIndex = data->map.nodeRootCount - 1;
					}

					//select next node by root index
					node = data->map.node[data->map.nodeRootIndex];
					npSelectNode (node, data);
				}
				else
				{						//select next sibling
					nodeParent = node->parent;

															//if shift+key held then apply skip interval multiplier
					if( data->io.key.modShift )
					{
						data->io.key.skipCount++;
						nodeParent->childIndex -= (data->io.key.skipCount * data->io.key.skipCount);
						
						if (nodeParent->childIndex < 0)
							nodeParent->childIndex = 0;
					}
					else
					{
						nodeParent->childIndex--;
						data->io.key.skipCount = 0;

						if (nodeParent->childIndex < 0)
							nodeParent->childIndex = nodeParent->childCount - 1;
					}

					// select our next sibling node and make active
					node = nodeParent->child[nodeParent->childIndex];
					npSelectNode (node, data);
				}
			}
			// set the examiner target on the selected root node
			npSetCamTarget (data);

			node = data->map.currentNode;
			npSelectNode (node, data);
			npSetCamTarget (data);
			npPostNodeID( node, data );
			break;
				//zz debug, move logic -> Data Structure logic belongs in data/npmap.h	
		case kNPcmdNextBranch :
			if (data->io.key.modShift)
			{
				while ( node->childCount )		// do nothing if at the trunk
				{
					if ( !node->child[node->childIndex] )
					{
						npPostMsg ("err 3582 - child node is NULL", kNPmsgErr, dataRef);
						break;
					}
					else
						node = node->child[node->childIndex];
				}
			}
			else
			{
				if (node->childCount > 0)	// do nothing if at the trunk
				{							// traverse up 1 level to the childs parent
					if ( !node->child[node->childIndex] )
						npPostMsg ("err 3582 - child node is NULL", kNPmsgErr, dataRef);
					else
						node = node->child[node->childIndex];
				}
				data->io.key.skipCount = 0;
			}

			npSelectNode (node, data);
			npSetCamTarget (data);
			npPostNodeID( node, data );
			break;

		case kNPcmdPrevBranch : 
			if (data->io.key.modShift)
			{
				while ( node->branchLevel )		// do nothing if at the trunk
				{
					// traverse up 1 level to the childs parent
					if ( !node->parent )
					{
						npPostMsg ("err 3584 - parent node has NULL parent", kNPmsgErr, dataRef);
						break;
					}
					else
						node = node->parent;
				}
			}
			else
			{
				if ( node->branchLevel )		// do nothing if at the trunk
				{
					// traverse up 1 level to the childs parent
					if ( !node->parent )
					{
						npPostMsg ("err 3585 - parent node has NULL parent", kNPmsgErr, dataRef);
						break;
					}
					else
						node = node->parent;
				}
				data->io.key.skipCount = 0;
			}

			npSelectNode (node, data);
			npSetCamTarget (data);
			npPostNodeID( node, data );
			break;
		
		case kNPcmdGrid :
			if (data->io.mouse.pickMode != kNPmodeGrid)
			{
				data->io.mouse.pickMode = kNPmodeGrid;
				npPostMsg("mode: Grid", kNPmsgCtrl, dataRef);
				npPostTool (NULL, data);
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
					npPostMsg("err 6288 - selectedGrid is NULL",kNPmsgErr,data);
				}
				npSelectNode (node, data);		//update grid selection
			}

			sprintf(msg, "%sgrid id: %d", msgPart, node->id);
			npPostMsg (msg, kNPmsgCtrl, dataRef);
			break;

		case kNPcmdCamera :
		//	data->ctrl.user[0]->tool->hide = true;
			data->io.mouse.tool = kNPtoolMove;			//zz select
			
			if (data->io.mouse.pickMode != kNPmodeCamera)
			{
				data->io.mouse.pickMode = kNPmodeCamera;
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
				node = data->map.currentCam;
				if (node == NULL)
				{
					node = data->map.node[kNPnodeRootCamera];
					npPostMsg("err 7289 - currentCam is NULL",kNPmsgErr,data);
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
			else
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
			if (data->io.mouse.tool == kNPtoolHide 
				|| data->io.mouse.tool == kNPtoolTag )	//add more tool handling //zz debug
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
				npPostMsg("Hide ALL nodes with level > 1",kNPmsgCtrl,data);
			else if (data->io.mouse.tool == kNPtoolTag)
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
			else if (data->io.mouse.tool == kNPtoolTag)
				npPostMsg("Hide ALL Text Tags",kNPmsgCtrl,data);
			//else //zz debug not when picking clear selection tool/key hit, not when picking
			//	npPostMsg("Clear Selection",kNPmsgCtrl,data);
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

		case kNPcmdTool :
			if (data->io.key.modShift)
				data->io.mouse.tool--;
			else
				data->io.mouse.tool++;
			if (data->io.mouse.tool <= 0)
				data->io.mouse.tool = kNPtoolCount - 1;
			if (data->io.mouse.tool >= kNPtoolCount)
				data->io.mouse.tool = 1;
			npPostTool (NULL, data);
			break;
		case kNPcmdToolDown : 
			data->io.mouse.tool--;
			if (data->io.mouse.tool <= kNPtoolNull)
				data->io.mouse.tool = kNPtoolCount - 1;
			npPostTool (NULL, data);
			break;

		default : break;
	}
}

//zz-JJ
//------------------------------------------------------------------------------
void npCtrlChannel (int command, pNPnode node, void* dataRef)
{
	char msg[128];
	pData data = (pData) dataRef;

	//when changing channels we need to remove from ch list, unless ch is zero
	if (node->chInputID != 0)
		npChRemoveNode (node, dataRef);

	//handles freezing and unfreezing nodes		//zz-JJ
	if (node->freeze)
		return;

	if (command == kNPcmdChannelUp)
	{
		node->chInputID = npChGetChannelAfter(&data->io.ch, node->chInputID);
		sprintf (msg, "immediately after npChGetChannelAfter, channel: %d", node->chInputID );
		npPostMsg (msg, kNPmsgDebug, dataRef);
	}
	else if (command == kNPcmdChannelDown)
		node->chInputID = npChGetChannelBefore(&data->io.ch, node->chInputID);

	npPostMsg ("..calling npChSubscribeNode", kNPmsgDebug, dataRef);
	npChSubscribeNode (node, dataRef);

	sprintf (msg, "channel: %d", node->chInputID );
	npPostMsg (msg, kNPmsgCtrl, dataRef);
}


//------------------------------------------------------------------------------
void npUpdateTopo (pNPnode node, void* dataRef)
{
	char msg[128];
	pData data = (pData) dataRef;

	switch (node->topo)
	{
		case kNPtopoNull :
			if (node->branchLevel == 0)
				node->geometry = kNPgeoPin;
			else
				node->geometry = kNPgeoTorus;
			sprintf(msg, "topo: %d default", node->topo); 
			break;
//		case kNPtopoGrid :
//			node->geometry = kNPgeoGrid;				//zz debug
//			sprintf(msg, "topo: %d default grid", node->topo);
			break;
		case kNPtopoCube : 
			node->geometry = kNPgeoCube;
			sprintf(msg, "topo: %d cube", node->topo);
			break;
		case kNPtopoSphere :
			node->geometry = kNPgeoSphere;
			sprintf(msg, "topo: %d sphere", node->topo); 
			break;
		case kNPtopoTorus : 
			node->geometry = kNPgeoTorus;
			sprintf(msg, "topo: %d torus", node->topo); 
			break;
		case kNPtopoCylinder :
			node->geometry = kNPgeoCylinder;
			sprintf(msg, "topo: %d cylinder", node->topo);
			break;
		case kNPtopoPin : 
			node->geometry = kNPgeoPin;
			sprintf(msg, "topo: %d pin", node->topo);
			break;
		case kNPtopoRod : 
			node->geometry = kNPgeoCylinder;
			sprintf(msg, "topo: %d rod", node->topo);
			break;
		case kNPtopoPoint : 
			node->geometry = kNPgeoOctahedron;
			sprintf(msg, "topo: %d point", node->topo);
			break;
		case kNPtopoCone :
			node->geometry = kNPgeoCone;
			sprintf(msg, "topo: %d cone", node->topo);
			break;
		case kNPtopoPlot :
			node->geometry = kNPgeoPlot;
			sprintf(msg, "topo: %d plot", node->topo);
			break;
		case kNPtopoSurface :
			node->geometry = kNPgeoSurface;
			sprintf(msg, "topo: %d surface", node->topo);
			break;
		case kNPtopoMesh :
			node->geometry = kNPgeoMesh;
			sprintf(msg, "topo: %d mesh", node->topo);
			break;
		default :
			sprintf(msg, "topo: %d", node->topo); 
			break;
	}
	npSetTagOffset (node);
	npPostMsg (msg, kNPmsgCtrl, dataRef);
}


//------------------------------------------------------------------------------
void npCtrlProperty (int command, void* dataRef)
{
	static char msg[256];
	static char msgPart[256];

	pData data = (pData) dataRef;
	pNPnode node = data->map.currentNode;
	pNPnode nodeParent = NULL;


	msgPart[0] = '\0';

	switch (command)
	{
		case kNPcmdChannelUp : npCtrlChannel (command, node, data); break;
		case kNPcmdChannelDown : npCtrlChannel (command, node, data); break;

		case kNPcmdColorUp :
			node->colorIndex++;
			npSetIndexColor( &node->color, &node->colorIndex, NULL ); //node->palette
			sprintf( msg, "color index:%5d RGB: %3d %3d %3d",
				node->colorIndex, node->color.r, node->color.g, node->color.b);
			npPostMsg( msg, kNPmsgCtrl, dataRef );
			break;
		case kNPcmdColorDown :
			node->colorIndex--;
			npSetIndexColor( &node->color, &node->colorIndex, NULL );
			sprintf( msg, "color index:%5d RGB: %3d %3d %3d",
				node->colorIndex, node->color.r, node->color.g, node->color.b, node->color.a);
			npPostMsg( msg, kNPmsgCtrl, dataRef );
			break;

		case kNPcmdAlphaUp :
			if( ( node->color.a + 10 ) > 255 )
				node->color.a = 255;
			else
				node->color.a += 10;
			sprintf( msg, "alpha: %d",node->color.a );
			npPostMsg( msg, kNPmsgCtrl, dataRef );
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
		case kNPcmdTextureDown :
			node->textureID--;
			if (node->textureID < 0)
				node->textureID = data->io.gl.textureCount;

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

		case kNPcmdGeometry :
			if (data->io.key.modShift)
				node->geometry--;
			else
				node->geometry++;

			if (node->geometry >= kNPgeoCount )//kNPgeoPin)	//zz debug
				node->geometry = 0;
			if (node->geometry < 0)
				node->geometry = kNPgeoCount - 1;//kNPgeoPin;
			npSetTagOffset (node);
			sprintf(msg, "geometry: %d", node->geometry);
			npPostMsg (msg, kNPmsgCtrl, dataRef);
			break;
		case kNPcmdPrimitiveDown :
			node->geometry--;
			if (node->geometry < 0)
				node->geometry = kNPgeoPin;
			npSetTagOffset (node);
			sprintf(msg, "geometry: %d", node->geometry);
			npPostMsg (msg, kNPmsgCtrl, dataRef);
			break;

		case kNPcmdTopo :
			if (node->topo == kNPtopoPlot)
			{
				npPostMsg ("topo: link is Locked!", kNPmsgCtrl, dataRef);
				return;
			}
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
				if (node->topo >= kNPtopoCount)  // kNPtopoCount			//zz debug
					node->topo = 1;
				npUpdateTopo (node, data);
			}
			break;
		case kNPcmdTopoDown :
				node->topo--;
				if (node->topo < 1)
					node->topo = kNPtopoCount - 1;  // kNPtopoCount		//zz debug
				npUpdateTopo (node, data);
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
			//if a assigned channel then update subscribe
			if (node->chInputID)
				npCtrlChannel (command, node, data);
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
		case kNPcmdSetpointLoOff :
				node->triggerLo.x = false;
				node->triggerLo.y = false;
				node->triggerLo.z = false;
			npPostMsg ("Set Lower Limit OFF", kNPmsgCtrl, dataRef);
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
		case kNPcmdSetpointHiOff :
				node->triggerHi.x = false;
				node->triggerHi.y = false;
				node->triggerHi.z = false;
			npPostMsg ("Set Upper Limit OFF", kNPmsgCtrl, dataRef);
			break;

		case kNPcmdTagMode :
			node->tagMode++;
			if (node->tagMode >= kNPtagModeCount)					//debug zz
				node->tagMode = 0;
			sprintf(msg, "tag mode: %d", node->tagMode);
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
	void* testOSC[4];		//zz debug
	float* toggle = NULL;	//zz-osc debug test
	
	static char msg[256];

	pData data = (pData) dataRef;

	pNPnode node = data->map.node[data->map.nodeRootIndex];

	switch (command)
	{
		case kNPcmdConsole : npConsole (dataRef); break;				//zz tag

		case kNPcmdMenu : //data->io.drawMenu = 1 - data->io.drawMenu;
			data->io.gl.hud.console.level++;
			if (data->io.gl.hud.console.level >= kNPconsoleLevelCount)
				data->io.gl.hud.console.level = 0;
			npResizeConsole(data);
			break;

		case kNPcmdSubsample : //data->io.drawMenu = 1 - data->io.drawMenu;		//zzhp
			if (data->io.key.modShift)
				data->io.gl.subsample /= 2;
			else
				data->io.gl.subsample *= 2;
			if (data->io.gl.subsample > 4096)
				data->io.gl.subsample = 4096;
			else if (data->io.gl.subsample < 1)
				data->io.gl.subsample = 1;

			sprintf(msg, "subsample nodes: %d", data->io.gl.subsample);	
				npPostMsg (msg, kNPmsgCtrl, dataRef);
			break;															//zzhp-end

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
				npPostMsg( "Background Black", kNPmsgCtrl, dataRef );
			
				testOSC[0] = toggle = &data->io.clear.a;
		//zzsql		npOscTx( 0, "/3/toggle3 ", "f", testOSC, dataRef );	//zz-osc
			
			//	cppTx (0, "/3/xy", "f", &data->io.clear.r, dataRef );	
			}
			else
			{
				data->io.clear.r = 1.0f;
				data->io.clear.g = 1.0f;
				data->io.clear.b = 1.0f;
				data->io.clear.a = 1.0f;

				data->io.clearIndex = 1;
				npPostMsg( "Background White", kNPmsgCtrl, dataRef );
			
				//npTxOSC (0, "/3/toggle1", "f", (void*)&data->io.clear.a, dataRef );
			
				testOSC[0] = toggle = &data->io.clear.a;
		//zzsql		npOscTx( 0, "/3/toggle3 ", "f", testOSC[0], dataRef ); // Contributing to crash on 'b'
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


	static int firstRunLoop = true;
	static char msg[256];

	pData data = (pData) dataRef;

	npUpdateIO (data);			//update IO, mouse, sound, txt, video...
	npUpdateData (data);			//update our data map, logical tracks
	npUpdateEngine (data);		//position, physics, interactions...

	npDispatchMessages (data);

	// zero out our mouse to prevent drifting objects
	data->io.mouse.delta.x = 0.0f;											//debug, zz
	data->io.mouse.delta.y = 0.0f;


	// check for command line arguements if first runloop
	if (firstRunLoop)
	{
		npMainArgcArgvHandler( data->io.argc, data->io.argv, data );
		
		firstRunLoop = false;
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

		//print to system console, adds newline at end of each msg	
		printf("%s\n", msg);

		if (1)//console->mode != kNPconsoleMenu || type == kNPmsgView)		//zzf debug
		{
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
	}

	//log file, adds newline at end of each msg

	data->io.message.queIndex = 0;	//add que locking... for thread safety, debug zz

	return;								//move this to control or maybe data...
}

//------------------------------------------------------------------------------
void npInitCPU (void* dataRef)
{
	pData data = (pData) dataRef;
	pNPcpu cpu = &data->ctrl.cpu;

	//query the number of CPU cores and or threads
	//benchmark and auto-tune CPU, BUS and Memory... the key for smooth runnning
	//lock a process to each of core threads then benchmark RAM, cache and math
	//also determine cache latency, size and levels
	//measure latency and bandwidth of CPU to CPU, core-to-core and thread pairs
	//in theory these stats should be all we need to know...
	//sets the inner process timing and thread counts
	//at a greater scope is inter-app and then inter-system cluster or NUMA HPC
	//this if followed by IO performance tuning

	//test thread count of 1,2,4,6,8,12...4096 until speed per thread drops
	//the point is to determine the optimal number of threads experimentally
	//this can be shared across multiple instances of the app to play friendly
	//ideally would check for other active antz on the system before running
	//benchmarks... to avoid a performance hit from perf testing!

/*
	//zz debug, below is an example of what should result from system query
	//this matches a single socket 3930k cpu, 6 cores, 12MB L3, 256K L2 per core

	//system wide CPU resources
	cpu->multiPartition = 1;	//support large multi-partition NUMA HPC systems
	cpu->systemNodes = 1;		//number of compute blades or server nodes
	cpu->sockets = 1;			//total number of sockets in the system
	cpu->cores = 6;				//total cores in entire system
	cpu->threads = 12;			//total threads in entire partition
		
	//system wide memory
	cpu->multiRAM	= 17179869184;	//same as cpu->RAM for single board servers
	cpu->systemRAM	= 17179869184;	//same as...

	//local compute node CPU resources
	cpu->socketsPerNode = 1;	//number of CPU sockets in compute node
	cpu->coresPerCPU = 6;		//can be even or number of cores
	cpu->threadsPerCPU = 12;	//cores * 2(hyper-threads) = threadsPerCPU

	//local compute node memory resources
	cpu->RAM		= 17179869184;	//local RAM on the compute board
	cpu->appRAM		=  4294967296;	//4GB limit for 32bit app

	cpu->cacheL3	=    12582912;	//total L3 cache per CPU
	cpu->cacheL2	=      262144;	//L2 per core, shared by hyper-threads
	cpu->cacheL1	=       65536;	//32K instruction and 32K of data
*/

	return;
}

void npCmdOpen( char* filePath, void* dataRef)
{
	npFileOpenMap( filePath, 1, strlen(filePath), dataRef );
}


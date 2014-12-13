/* -----------------------------------------------------------------------------
*
*  npmap.c
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

#include "npmap.h"


//------------------------------------------------------------------------------
/** @file npmap.h 
*  Methods for managing the global 'map' data structure.
*  Can traverse the map tree and edit its structure, move branches, attach nodes
*  remove nodes, sort objects and build custom graphs.
*/

/// @param dataRef is a global data reference instance for the scene graph map.
//------------------------------------------------------------------------------
void npInitMap (void* dataRef)
{
	int i = 0;
	pData data = (pData) dataRef;

//------------------------------------------------------------------------------
//move these to npmaproot.h mapbase.h or npmaptree.h
//forms the base data structure, can be augmented at run time
//describes complete data map schema in a CSV/MySQL/JSON compatible manner
//includes globals, data tree graph, node and link types
//table name, field names, type and description... etc.
//------------------------------------------------------------------------------

//#define kNPbaseItemCount 18		//must be updated if below item count changed
	static NPmapType base[] = {

	{ kNPfloat,				kNPfloat,		"float",			"openGL float" },
	{ kNPint,				kNPint,			"int",				"C int" },
	{ kNPubyte,				kNPubyte,		"ubyte",			"C ubyte" },
	{ kNPbool,				kNPbool,		"bool",				"C ubyte boolean" },

	//zz debug remove these?... they are identical to kNP... hmmm
	{ kNPGLboolean,			kNPGLboolean,	"GLboolean",		"openGL boolean" },
	{ kNPGLubyte,			kNPGLubyte,		"GLubyte",			"openGL ubyte" },
	{ kNPGLint,				kNPGLint,		"GLint",			"openGL int" },
	{ kNPGLfloat,			kNPGLfloat,		"GLfloat",			"openGL float" },

	// if both the constant and the data type are equal then it is a fundamental type
	// note can be a structure
	{ kNPfloatXYZA,			kNPfloatXYZA,	"NPfloatXYZA",		"openGL float x,y,z,angle" },
	{ kNPfloatXYZS,			kNPfloatXYZS,	"NPfloatXYZS",		"openGL x,y,z,scaler" },
	{ kNPfloatRGBA,			kNPfloatRGBA,	"NPfloatRGBA",		"openGL float r,g,b,a" },
	{ kNPfloatRGB,			kNPfloatRGB,	"NPfloatRGB",		"openGL float r,g,b" },
	{ kNPintXYZ,			kNPintXYZ,		"NPintXYZ",			"C int x,y,z" },
	{ kNPubyteRGBA,			kNPubyteRGBA,	"NPubyteRGBA",		"openGL ubyte r,g,b,a" },
	{ kNPboolXYZS,			kNPboolXYZS,	"NPboolXYZS",		"openGL boolean angle,x,y,z" },
	{ kNPboolXYZ,			kNPboolXYZ,		"NPboolXYZ",		"openGL boolean x,y,z" },
	{ kNPfloatXY,			kNPfloatXY,		"NPfloatXY",		"openGL float x,y" },
	{ kNPfloatXYZ,			kNPfloatXYZ,	"NPfloatXYZ",		"openGL float x,y,z" }
	};

	// used by the CSV header and DB field names
	// node currently expands to 94 columns or elements such as channel x,y,z
//#define kNPnodeItemCount 52		//must be updated if below item count changed
	static NPmapType node[] = {

	{ kNPid,				kNPint,			"id",				"node ID" },
	{ kNPtype,				kNPint,			"type",				"node type" },
	{ kNPdata,				kNPint,			"data",				"node type specific data" },

	{ kNPselected,			kNPint,			"selected",			"node currently selected" },

	{ kNPparent,			kNPvoidPtr,		"parent_id",		"parent node in a data tree" },
	{ kNPbranchLevel,		kNPint,			"branch_level",		"branch depth on tree, 0 is root node" },
	{ kNPchild,				kNPvoidPtr,		"child_id",			"list ID of attached child nodes" },
	{ kNPchildIndex,		kNPint,			"child_index",		"specifies the current child" }, 
	{ kNPchildCount,		kNPint,			"child_count",		"current number of children" },

	{ kNPchInputID,			kNPint,			"ch_input_id",		"channel input maps tracks to node" },
	{ kNPchOutputID,		kNPint,			"ch_output_id",		"channel output maps node to tracks" },
	{ kNPchLastUpdated,		kNPint,			"ch_last_updated",	"last cycle channel was updated" },

	{ kNPaverage,			kNPint,			"average",			"averaging type applied to data" },
//	{ kNPinterval,			kNPint,			"interval",			"the interval to be averaged" },
	{ kNPinterval,			kNPint,			"sample",			"the interval to be averaged" },
//zzsql could also have sample be samples...

	{ kNPauxA,				kNPintXYZ,		"aux_a",			"reserved for future use" },
	{ kNPauxB,				kNPintXYZ,		"aux_b",			"reserved for future use" },

	{ kNPcolorShift,		kNPfloat,		"color_shift",		"color shift is akin to hue" },
	
	{ kNProtateVec,			kNPfloatXYZS,	"rotate_vec",		"rotation unit vector" },
	{ kNPscale,				kNPfloatXYZ,	"scale",			"scale node XYZ" },
	{ kNPtranslate,			kNPfloatXYZ,	"translate",		"position relative to parent node" },
	{ kNPtagOffset,			kNPfloatXYZ,	"tag_offset",		"text tag offset relative to node center" },

	{ kNProtateRate,		kNPfloatXYZ,	"rotate_rate",		"angular velocity" },
	{ kNProtate,			kNPfloatXYZ,	"rotate",			"orienation such as heading, tilt, roll" },
	{ kNPscaleRate,			kNPfloatXYZ,	"scale_rate",		"scale rate" },
	{ kNPtranslateRate,		kNPfloatXYZ,	"translate_rate",	"velocity vector" },
	{ kNPtranslateVec,		kNPfloatXYZ,	"translate_vec",	"user defined vector" },

	{ kNPshader,			kNPint,			"shader",			"wire, flat, gouraud, phong" },
	{ kNPgeometry,			kNPint,			"geometry",			"cube, sphere, pin, FFT mesh, etc" },

	{ kNPlineWidth,			kNPfloat,		"line_width",		"draws lines, zero for off" },
	{ kNPpointSize,			kNPfloat,		"point_size",		"draws dots, zero for off" },
	{ kNPratio,				kNPfloat,		"ratio",			"geometry ratio, ie torus inner radius" },

	{ kNPcolorIndex,		kNPint,			"color_index",		"specifies the color from the palette"},
	{ kNPcolor,				kNPubyteRGBA,	"color",			"color assigned to new data" },
	{ kNPcolorFade,			kNPint,			"color_fade",		"fades alpha of older data points"},
	{ kNPtextureID,			kNPint,			"texture_id",		"GL texture ID" },

	{ kNPhide,				kNPbool,		"hide",				"hide the node, data stays active" },
	{ kNPfreeze,			kNPbool,		"freeze",			"freezes data and physics updates" },
	
	{ kNPtopo,				kNPint,			"topo",				"topography, cube, sphere, torus, etc" },
	{ kNPfacet,				kNPint,			"facet",			"topo face number" },
	
	{ kNPautoZoom,			kNPintXYZ,		"auto_zoom",		"scales node to fit screen" },
	
	{ kNPtriggerHi,			kNPintXYZ,		"trigger_hi",		"trigger if high setpoint hit" },
	{ kNPtriggerLo,			kNPintXYZ,		"trigger_lo",		"trigger if low setpoint hit" },
	{ kNPsetHi,				kNPfloatXYZ,	"set_hi",			"trigger high setpoint" },
	{ kNPsetLo,				kNPfloatXYZ,	"set_lo",			"trigger low setpoint" },
	{ kNPproximity,			kNPfloatXYZ,	"proximity",		"radius used for proximity detection" },
	{ kNPproximityMode,		kNPintXYZ,		"proximity_mode",	"physics engine collision mode" },

	{ kNPsegments,			kNPintXYZ,		"segments",			"segment count, such as the grid" },

	{ kNPtagMode,			kNPint,			"tag_mode",			"text tag display mode" },
	{ kNPformatID,			kNPint,			"format_id",		"translates values, used for labels" },
	{ kNPmapID,			kNPint,			"table_id",			"location of the records source table" },
	{ kNPrecordID,			kNPint,			"record_id",		"record ID in the source table" },	//debug zz

	{ kNPsize,				kNPint,			"size",				"node size in bytes" }
	};

//#define kNPcameraItemCount 17		//must be updated if below item count changed
	static NPmapType camera[] = {

	{ kNPid,				kNPint,			"id",				"node ID" },
	{ kNPtype,				kNPint,			"type",				"node type" },
		
	{ kNPformat,			kNPint,			"format",			"320p, 480i, 720p, 4K full app" },
	{ kNPinterlaced,		kNPint,			"interlaced",		"interlaced field order" },
	{ kNPstereo,			kNPint,			"stereo_3d",		"stereoscopic 3D" },
	{ kNPaspectRatio,		kNPfloat,		"aspect_ratio",		"1.0, 1.333, 1.777, 1.85, 2.25... " },
	{ kNPfps,				kNPfloat,		"fps",				"15, 24, 29.97, 30, 59.94, 60, 119.88" },
	{ kNPcolorSpace,		kNPint,			"color_space",		"8, 12bit, YUV, RGBA, XYZ, CMYK..." },
	{ kNPwidth,				kNPint,			"width",			"res in pixels" },
	{ kNPheight,			kNPint,			"height",			"res in pixels" },

	{ kNPfov,				kNPfloat,		"fov",				"FOV 35mm, 70mm..." },
	{ kNPclipNear,			kNPfloat,		"clip_near",		"camera near clipping plane" },
	{ kNPclipFar,			kNPfloat,		"clip_far",			"camera far clipping plane" },

	{ kNPaperture,			kNPfloat,		"aperture",			"F stop" },
	{ kNPexposure,			kNPfloat,		"exposure",			"in seconds" },
	{ kNPsensorType,		kNPint,			"sensor_type",		"3CCD, Debayer pattern..." },

	{ kNPsize,				kNPint,			"size",				"data size in bytes" }
	};

//#define kNPgridItemCount 5		//must be updated if below item count changed
	static NPmapType grid[] = {

	{ kNPid,				kNPint,			"id",				"node ID" },
	{ kNPtype,				kNPint,			"type",				"node type" },

	{ kNPsegments,			kNPintXYZ,		"segments",			"number of grid segments" },
	{ kNPoverlay,			kNPbool,		"overlay",			"overlays relative to screen coord" },

	{ kNPsize,				kNPint,			"size",				"node size in bytes" }
	};

//#define kNPpinItemCount 12		//must be updated if below item count changed
	static NPmapType pin[] = {

	{ kNPid,				kNPint,			"id",				"node ID" },
	{ kNPtype,				kNPint,			"type",				"node type" },

	{ kNPinnerRadius,		kNPfloat,		"inner_radius",		"inner radius of the toroid" },
	{ kNPouterRadius,		kNPfloat,		"outer_radius",		"outer radius of the toroid" },
	{ kNPradiusRatioIndex,	kNPint,			"radius_ratio_index", "sorts pre-loaded GPU toriods" },
	{ kNPscreenSizeIndex,	kNPint,			"screen_size_index", "same as above for detail level" },
	{ kNPslices,			kNPint,			"slices",			"calculated from radiusRatioIndex" },
	{ kNPstacks,			kNPint,			"stacks",			"and from the screenSizeIndex" },
	{ kNPtranslateTex,		kNPfloatXYZ,	"translate_tex",	"offsets texture origin" },
	{ kNPscaleTex,			kNPfloatXYZ,	"scale_tex",		"scales the texture" },
	{ kNProtateTex,			kNPfloatXYZS,	"rotate_tex",		"orientation of the texture map" },
	{ kNPsize,				kNPint,			"size",				"node size in bytes" }
	};

	//zzsql
	static NPmapType tag[] = {

		{ kNPid,				kNPint,			"id",				"tag ID" },
		{ kNPrecordID,			kNPint,			"record_id",		"Record ID" },
		{ kNPmapID,				kNPint,			"table_id",			"Table ID" },
		{ kNPtitle,				kNPcharArray,	"title",			"Title" },
		{ kNPdesc,				kNPcharArray,	"description",		"Description" }
	}; // debug db
	
	static NPmapType ChMap[] = {

		{ kNPid,				kNPint,			"id",				"Channel Map ID" },
		{ kNPchannelID,			kNPint,			"channel_id",		"Channel ID"	 },
		{ kNPtrackID,			kNPint,			"track_id",			"Track ID"		 },
		{ kNPattribute,			kNPcharArray,	"attribute",		"Attribute"		 },
		{ kNPtrackTableID,		kNPint,			"track_table_id",	"Track Table ID" },
		{ kNPchMapTableID,		kNPint,			"ch_map_table_id",	"Channel Map Table ID"},
		{ kNPrecordID,			kNPint,			"record_id",		"Record ID"		}
	}; // debug db //zzsql
	
	//zz color
	static NPmapType palette[] = {

		{ kNPid,				kNPint,			"id",			"Color Palette Index" },
		{ kNPrecordID,			kNPint,			"red",			"Red" },
		{ kNPmapID,				kNPint,			"green",		"Green" },
		{ kNPtitle,				kNPint,			"blue",			"Blue" },
		{ kNPdesc,				kNPint,			"alpha",		"Alpha Transparency" }
	};
	

	//-------
	//-------
//	npMapTypeInit(data);						//zz debug, move this from npcsv.h
	//-------

	for (i=0; i < kNPpaletteMax; i++)
		data->map.color[i] = NULL;

	//-------
	// clear the typeMap
	for (i=0; i < kNPdataTypeMax; i++)
		data->map.typeMap[i] = NULL;

	data->map.typeMapBase		= base;
	data->map.typeMapNode		= node;
	data->map.typeMapPin		= pin;
	data->map.typeMapCamera		= camera;
	data->map.typeMapGrid		= grid;
	data->map.typeMapTag		= tag;		// debug db //zzsql
	data->map.typeMapChMap		= ChMap;	// debug db //zzsql
	
	data->map.typeMapPalette	= palette;

	data->map.typeMapGlobals	= NULL;		//zz debug
	
	data->map.mapTypeList = NULL;
	data->map.mapTypeCount = 0;

	// populate the typeMap
	data->map.typeMap[kNPbase]	= base;
	data->map.typeMap[kNPnode]	= node;
//	data->map.typeMap[kNPcmd] = graph;
	data->map.typeMap[kNPpin]	= pin;
	data->map.typeMap[kNPcamera]= camera;
	data->map.typeMap[kNPgrid]	= grid;


	data->map.selectSet.x		= false;
	data->map.selectSet.y		= false;
	data->map.selectSet.z		= false;
	data->map.selectAll			= false;

	//setup the root node array and the nodeID lookup table
	data->map.nodeCount			= 0;
	data->map.nodeRootCount		= 0;
	data->map.nodeRootIndex		= 0;

	data->map.previousNode		= NULL;
	data->map.currentNode		= NULL;
	data->map.currentCam		= NULL;
	
	data->map.selectedGrid		= NULL;
	data->map.selectedPinNode	= NULL;
	data->map.selectedPinIndex	= 0;
	data->map.selectedHUD		= NULL;

	data->map.syncNodes			= false;
	data->map.syncTagsReady		= false;
		
	data->map.sortSwap			= 0;						//zzhp debug
	data->map.sortSwapFlag		= 0;						//zzhp debug

	//clear the node ID array
	data->map.node = (void*) malloc (kNPnodeRootMax * sizeof(void*));
	if (data == NULL)
	{
		printf ("err 4273 - malloc failed to allocate root node array\n");
		exit(EXIT_FAILURE);
	}

	//clear the node ID array
	data->map.sortA = (void*) malloc (kNPnodeMax * sizeof(void*));		//zzhp
	if (data == NULL)
	{
		printf ("err 4274 - malloc failed to allocate sort array\n");
		exit(EXIT_FAILURE);
	}
	data->map.sortB = (void*) malloc (kNPnodeMax * sizeof(void*));		//zzhp
	if (data == NULL)
	{
		printf ("err 4275 - malloc failed to allocate sort array\n");
		exit(EXIT_FAILURE);
	}

	//clear the node ID array
	data->map.nodeID = (void*) malloc (kNPnodeMax * sizeof(void*));
	if (data == NULL)
	{
		printf ("err 4276 - malloc failed to allocate nodeID array\n");
		exit(EXIT_FAILURE);
	}

	//clear the node ID array
	data->map.parentID = (int*) malloc (kNPnodeMax * sizeof(int*));
	if (data == NULL)
	{
		printf ("err 4277 - malloc failed to allocate parentID array\n");
		exit(EXIT_FAILURE);
	}

	data->map.sortID = (void*) malloc (kNPnodeMax * sizeof(void*));
	if (data == NULL)
	{
		printf ("err 4278 - malloc failed to allocate sortID array\n");
		exit(EXIT_FAILURE);
	}

	//clear the node ID array
	data->map.orphanList = (int*) malloc (kNPnodeMax * sizeof(int*));
	if (data == NULL)
	{
		printf ("err 4279 - malloc failed to allocate nodeID array\n");
		exit(EXIT_FAILURE);
	}

	//clear the arrays
	for (i=0; i < kNPnodeRootMax; i++)
	{
		data->map.node[i] = NULL;
		data->map.sortA[i] = NULL;					//zzhp
		data->map.sortB[i] = NULL;					//zzhp
	}
	data->map.sort = data->map.node; //before first z-sort just use node array //zzhp
									 //becomes map.sortA or B
	for (i=0; i < kNPnodeMax; i++)
	{
		data->map.nodeID[i] = NULL;

		data->map.sortID[i] = NULL;
		data->map.parentID[i] = 0;
		data->map.orphanList[i] = 0;
	}
	data->map.orphanCount = 0;
	data->map.sortCount = 0;					//zzhp
	data->map.sortCountA = 0;					//zzhp
	data->map.sortCountB = 0;					//zzhp
}

//------------------------------------------------------------------------------
int npGetRootIndex (pNPnode node, void* dataRef)
{
	int i = 0;
	int index = 0;

	pData data = (pData) dataRef;

	while (node->parent != NULL)
		node = node->parent;

	for (i=0; i < data->map.nodeRootCount; i++)
		if (node == data->map.node[i])
		{
			index = i;
			i = data->map.nodeRootCount;
		}

	return index;
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
pNPnode npMapNodeNext (void* dataRef){return NULL;}			//select next sibling node
pNPnode npMapNodePrevious (void* dataRef){return NULL;}		//previous sibling
pNPnode npMapNodeUp (void* dataRef){return NULL;}				//select parent
pNPnode npMapNodeDown (void* dataRef){return NULL;}			//select 

//------------------------------------------------------------------------------
void* npGetNodeByID (int id, void* dataRef)
{
	pData data = (pData) dataRef;
	pNPnode node = NULL;

	node = data->map.nodeID[id];		//get node by id

	return node;
}

//------------------------------------------------------------------------------
void npSelectNodeByID (int id, void* dataRef)
{
	pNPnode node = NULL;

	node = npGetNodeByID (id, dataRef);	//get node by id

	npSelectNode (node, dataRef);
}

//applies passed in nodeFunc to the node and all of its sub-nodes
//recursive tree traversal process may be configured for parent first or last
//also, can avoid processing the root node by moving nodeFuncPtr inside the loop
//------------------------------------------------------------------------------
void npTraverseTree (void (*pNodeFunc)(pNPnode node, void* dataRef), 
					 pNPnode node, void* dataRef)
{
	int i = 0;

	//	pNodeFunc(node, dataRef);	//process node parent(s) first

	//full tree traversal by recursively calling this function for each child
	for (i=0; i < node->childCount; i++)
	{
		npTraverseTree (pNodeFunc, node->child[i], dataRef);
	}

	pNodeFunc(node, dataRef);		//process node parent last
}

//applies passed in function to all nodes
//------------------------------------------------------------------------------
void npTraverseMap (void (*pNodeFunc)(pNPnode node, void* dataRef),
				   void* dataRef)
{
	int i = 0;
	pData data = (pData) dataRef;

	//loop through all root nodes and traverse all sub-nodes
	//pNodeFunc is applied to each node
	for (i=0; i < data->map.nodeRootCount; i++)
		npTraverseTree (pNodeFunc, data->map.node[i], dataRef);
}

//-----------------------------------------------------------------------------
void npInitMalloc(void* dataRef)
{
	//get max possible REAL memory size
	//allocate initial memory pools
	//data->map.mem.pool[i++] = malloc(kNPmemBlock);
	//data->map.mem.pool[i++] = malloc(kNPmemBlock);
	//data->map.mem.pool[i++] = malloc(kNPmemBlock);
	//data->map.mem.pool[i++] = malloc(kNPmemBlock);
}

//-----------------------------------------------------------------------------
void npCloseMalloc(void* dataRef)
{}

//-----------------------------------------------------------------------------
void npUpdateMalloc(void* dataRef)
{}

//npmemory.h architecture roadmap
//provides a memory pool with intelligent caching between CPU, RAM and storage 
//optimized for large high-bandwidth block transfers and high random IOPS to 
//groups node elements based on thread process with support for NUMA systems
//will use Solid State Arrays and SSD based storage as a NUMA memory pool
//data structure are intelligently mapped to RAM to optimize process order
//including z-depth sorted nodes, hash tables and multiple pre-sorted lists
//
//if you want high performance small size allocations then use malloc directly
//we utilize TCMalloc for higher performance then standard lib
//-----------------------------------------------------------------------------
void* npMalloc(int type, int size, void* dataRef)
{
	pData data = (pData) dataRef;

	void* memPtr = NULL;

	// http://gperftools.googlecode.com/svn/trunk/doc/tcmalloc.html

	//memory is grouped by type and the datamap tree
	//child nodes immediately follow their parent
	//runtime mem block optimizations include z-depth ordering, etc...

	//memory pool pre-requested from system in blocks
	//

	//zz debug we dynamic node size, similar to multi-track interleaving

	switch(type)
	{
		case kNPnode :					//set of all node structs
			memPtr = malloc(size);		//NPnode
			if (memPtr == NULL)
				npPostMsg ("err 1012 - malloc failed NPnode", kNPmsgErr, data);
			break;	
		case kNPtag :
			memPtr = malloc(size);		//NPtag
			if (memPtr == NULL)
				npPostMsg ("err 1013 - malloc failed NPtag", kNPmsgErr, data);
			break;		//tag master list structs
		case kNPfileBlock :
			memPtr = malloc(size);		//kNPfileBufferSize
			if (memPtr == NULL)
				npPostMsg ("err 1014 - malloc failed kNPfileBlockSize", kNPmsgErr, data);
			break;	//supports a set of threaded blocks
		case kNPlist :
			memPtr = malloc(size);		//kNPfileBufferSize
			if (memPtr == NULL)
				npPostMsg ("err 1015 - malloc failed kNPlist", kNPmsgErr, data);
			break;		//such as node lists
		case kNPglobal :
			memPtr = malloc(size);	//global variables (not arrays) 
			if (memPtr == NULL)
				npPostMsg ("err 1011 - malloc failed kNPglobal", kNPmsgErr, data);
			break;
		case kNPque :
			memPtr = malloc(size);	//global variables (not arrays) 
			if (memPtr == NULL)
				npPostMsg ("err 1011 - malloc failed kNPque", kNPmsgErr, data);
			break;
		case kNPqueItem :
			memPtr = malloc(size);	//global variables (not arrays) 
			if (memPtr == NULL)
				npPostMsg ("err 1011 - malloc failed kNPqueItem", kNPmsgErr, data);
			break;
		case kNPmapItem :
			memPtr = malloc(size);	//global variables (not arrays) 
			if (memPtr == NULL)
				npPostMsg ("err 1011 - malloc failed kNPmapItem", kNPmsgErr, data);
			break;
		default : 
			memPtr = malloc(size);
			if (memPtr == NULL)
				npPostMsg ("err 1010 - malloc failed default", kNPmsgErr, data);
			break;
	}
	
	if (!memPtr)
		printf ("err 1010 - malloc failed at size: %d\n", size);

//	data->io.refCount++;
//	printf("\nRefCount : %d", data->io.refCount);
	return memPtr;
}


//-----------------------------------------------------------------------------
void* npCalloc(int type, int size, void* dataRef)
{
	/// note that standard C calloc clears the memory to all 0 bits, is the
	/// same as NULL pointer for most platforms, but not some esoteric ones.
	/// also a float 0.0 is not same as 0 bits.

	return memset (npMalloc (type, size, dataRef), 0, size);
}


//-----------------------------------------------------------------------------
void npFree(void* memory, void* dataRef)
{
/*
	pData data = (pData) dataRef;
	pNPmemory = &data->map.memory;
	int count = memory->listCount;
	void** list = memory->list;

	//zz debug, update with list of known pointers...
	while (count-- > 0)
	{
		if (memory == list[count])
		{
			//only free to system when we have enough memory blocks ready
			if (memoryNotWanted)
				free (memory);

			count = 0;	//exit loop
		}
	}
	//if unknown then report error and don't free
*/

	free(memory);
}


//-----------------------------------------------------------------------------
void npMapSortInit (void** mapRef, void* dataRef)
{
	int i = 0;

	pData data = (pData) dataRef;

/*
	if (data->map.sortID != NULL)
		free (data->map.sortID);

	data->map.sortID = (void*)malloc(kNPnodeMax * sizeof(void*));

	if (data->map.sortID == NULL)
	{
		printf ("err 4833 - malloc failed to allocate sortID array\n");
		return;
	}
*/

	//fill with node pointers to kNPnodeRootNull
	for (i=0; i < kNPnodeMax; i++)
	{
		data->map.sortID[i] = NULL;
		data->map.parentID[i] = 0;
		data->map.orphanList[i] = 0;
	}

	data->map.orphanCount = 0;
	data->map.sortCount = 0;
}

//if id mapped then returns the node, else it returns the kNPnodeRootNull node
//-----------------------------------------------------------------------------
void npMapSortAdd (int id, int parentID, void* nodeRef, void* dataRef)
{
	int count = 0;

	pNPnode node = (pNPnode) nodeRef;
	pData data = (pData) dataRef;

	if (id < 0 || id >= kNPnodeMax)
	{
		printf ("err 4833 - id: %d out of range, npMapSortAdd()\n", id);
		return;
	}
	
	// check if id already used, if so replaces entry in orphanList
	// perhaps change from overwrite and assign a new id update actual node, debug zz
	if (data->map.sortID[id] != NULL)
		printf ("warning 4834 - id: %d previously used in table, overwriting\n", id);

	// add the node to the sort parent lookup
	data->map.sortID[id] = nodeRef;

	// if orphan then add to list
	if (node->parent == data->map.node[kNPnodeRootNull]
		|| node->type == kNodeLink )				//always add link nodes
	{	
		// make sure in bounds of list max
		count = data->map.orphanCount;

		if ( count >= kNPnodeMax - 1 )
		{
			printf ("err 4835 - kNPorphanMax node limit exceeded\n");
			return;
		}

		data->map.orphanList[count] = id;
		data->map.parentID[count] = parentID;
		
		data->map.orphanCount++;

		printf ("orphanID: %d   parentID: %d\n", id, parentID);
	}
}

//-----------------------------------------------------------------------------
pNPnode npMapSortID (int id, void* dataRef)
{
	pData data = (pData) dataRef;

	if (id < 0 || id >= kNPnodeMax)
	{	
		printf ("err 4834 - id: %d out of range, npMapSortAdd()\n", id);
		return data->map.node[kNPnodeRootNull]; //NULL debug zz
	}

	if (data->map.sortID[id] == NULL)
		return data->map.node[kNPnodeRootNull];	// do we need this, debug zz

	//make a special node with no limit on child nodes,						 debug, zz

	return data->map.sortID[id];
}

//update node branchLevel based on parent level, traverses all sub-child nodes
//-----------------------------------------------------------------------------
void npNodeUpdateBranchLevel (pNPnode node)
{
	int i = 0;

	pNPnode parent = node->parent;
	
	if (parent == NULL)
		node->branchLevel = 0;
	else
		node->branchLevel = parent->branchLevel + 1;

	//recursively traverse all child nodes and update them
	for (i=0; i < node->childCount; i++)
	{
		npNodeUpdateBranchLevel (node->child[i]);	// recursive call
	}

//	printf ("id: %d  updated level: %d\n", node->id, node->branchLevel);
}


//-----------------------------------------------------------------------------
void npNodeMoveBranch (pNPnode node, pNPnode newParent, void* dataRef)
{
	int i = 0;
	int childIndex = 0;
	int parentIndex = 0;
	char msg[256];

	pData data = (pData) dataRef;

	if (node == NULL || newParent == NULL)
	{
		printf ("err 4835 - node or nodeParent is NULL\n");
		return;
	}
		
	if (newParent->childCount >= kNPnodeChildMax)
	{
		sprintf(msg, "err 4836 - kNPnodeChildMax hit: %d max", kNPnodeChildMax);
		npPostMsg(msg, kNPmsgErr, data);
		return;
	}

	//remove node from parent old parent
	npNodeRemove (false, node, data);			//npCutBranch()

	//attach to new parent
	node->parent = newParent;
	
	newParent->child[node->childCount] = node;
	newParent->childCount++;

	//updates branchLevel for child and any sub child nodes in the tree
	npNodeUpdateBranchLevel (node);

//	printf ("node id: %d  level: %d  parent id: %d\n", 
//			node->id, node->branchLevel, newParent->id );
}

//attach orhan nodes to their parent after loading all file records
//-----------------------------------------------------------------------------
void npMapSort(void* dataRef)
{
	int i = 0;
	int parentIndex = 0;

	pNPnode node = NULL;
	pNPnode nodeParent = NULL;
	pNPnode child = NULL;			//used for link B
	pData data = (pData) dataRef;

	//iterate through the orphanList
	for (i=0; i < data->map.orphanCount; i++)
	{
		node = data->map.sortID[data->map.orphanList[i]];
		nodeParent = data->map.sortID[data->map.parentID[i]];

		//some special processing for link nodes only
		if (node->type == kNodeLink)
		{
			//we temporarily store link B childID in the childIndex
			child = data->map.sortID[node->childIndex];
			node->childIndex = 0;	//no longer need temp storage
			
			node->child[0] = child;
		//	node->childCount++;

			//attach node to link B end
			npNodeAttach (node, child, data);

			//if this is not an actual orphan the skip the rest
			if (node->parent == nodeParent)
				continue;
		}

		if (nodeParent != data->map.node[kNPnodeRootNull])
		{
			npNodeMoveBranch (node, nodeParent, data);

			//compatability for files prior to 2012-04-22 with topo == 0
			//other half of this procedure is in npReadMapCSVNode()
			if (node->topo == 0 && node->type == kNodePin && nodeParent != NULL)
			{
				if ( nodeParent->topo == kNPtopoPin || nodeParent->topo == 0
					|| nodeParent->topo == kNPtopoTorus )
					node->topo = kNPtopoTorus;
				else
					node->topo = kNPtopoPin;
			}

			printf ("orphan id: %d attached to parent id: %d\n", 
				data->map.orphanList[i], data->map.parentID[i] );
		}
		else
		{
			npNodeDelete (node, dataRef);
			data->map.sortID[i] = NULL;

			printf ("err 4838 - orphan id: %d   missing parent id: %d\n",
				data->map.orphanList[i], data->map.parentID[i] );
		}
	}
}

//-----
//zzs move this to npmap.c file, pull out the CSV ver 1 stuff				debug zz
//add user control to merge scenes vs clean reset before load
//
//logic for building the data map from an unsorted list of nodes
//creates a new node or updates some nodes like exisitng cameras and grids
//need to call npMapSort() after all nodes added to attach orphan nodes
//-----------------------------------------------------------------------------
void* npMapNodeAdd (int id, int type, int branchLevel, int parentID, 
					 int format, void* dataRef)
{
	int i = 0, count = 0;
	int curs = 0;				//Cursor position in Buffer parsing source
//	int scanNumRet = 0;			//sscanf return value. Number of successfuly scanned elements
	int numExpected = 0;		//Expected number for scanNumRet returned from sscanf
	int nodeCount = 0;
	int parentBranchLevel = 0;
	int childIndex = 0;

	//node tree pointers
	pNPnode nodeParent = NULL;
	pNPnode node = NULL;
	pNPnode nodeChild = NULL;
	
	pData data = (pData) dataRef;

	
	//process node logic based on type and branchLevel
	//either updates existing node or create a new one to add to scene
	//update based on user setting or node type and ID
	//if new one, either a child or parent, determined by branchLevel
	//default is to update->camera, update->grid
	//new node for all others

	//root node
	if( branchLevel <= 0 ) // or perhaps parentID == 0, debug zz
	{
		switch (type)
		{
			case kNodeCamera : node = data->map.node[kNPnodeRootCamera]; break; //CSV v1 requires this
			case kNodeGrid : node = data->map.node[kNPnodeRootGrid]; break;	//CSV v1 requires this
			case kNodePin :
				node = npNodeNew (type, NULL, dataRef);
				if (format == kNPmapNodeCSVvOne)					//zz debug, move this out a level
					nodeChild = npNodeNew (type, node, dataRef);	//create primary torus
				break;			//create Pin or other
			case kNodeDefault : 
				if (id == 1 && type == kNodeDefault)
					node = data->map.node[kNPnodeRootNull];
				else
					node = npNodeNew (type, NULL, dataRef);
				break;
			default : printf ("err 4840 - id: %d type: %d unknown\n", id, type); break;
		}
	}

	//first child branchLevel
	if( branchLevel == 1 )
	{
		// if parent is NULL then returns the nodeRootNull
		nodeParent = npMapSortID (parentID, dataRef);

		switch (type)
		{
			case kNodeCamera :
				//if (1)//format == kNPmapNodeCSVvOne)

				nodeParent = data->map.node[kNPnodeRootCamera];

				if (id >= 3 && id <= 6)
				{
					node = nodeParent->child[id - 3];
					if (node == NULL)
						node = npNodeNew (type, nodeParent, dataRef);	//else create new node
				}
				else
					node = npNodeNew (type, nodeParent, dataRef);	//else create new node
				break;
			case kNodeGrid :
				if (format == kNPmapNodeCSVvOne)				//zz debug, move this out a level
					nodeParent = data->map.node[kNPnodeRootGrid];
				node = npNodeNew (type, nodeParent, dataRef);	//create new node
				break;
			case kNodePin :
				if (format == kNPmapNodeCSVvOne && nodeParent->child[0] != NULL) //zz debug, move this out a level
					node = nodeParent->child[0];	//already created primary torus
				else
					node = npNodeNew (type, nodeParent, dataRef);	//create the primary toroid
				break;			//create Pin or other
			default : node = npNodeNew (type, nodeParent, dataRef); break;
		}
	}

	//New Sub Child Toroid
	if( branchLevel >= 2 )		//Second Level (or greater) Toroid Sub Child  
	{
		nodeParent = npMapSortID (parentID, dataRef);	//if DNE return default node
		node = npNodeNew (type, nodeParent, dataRef);
	}


	// add to parent lookup table for sorting orphan child nodes at end
	npMapSortAdd (id, parentID, node, dataRef);

	return node;
}


//parallel qsort for large counts, divides list, assembled with merge sort
//smaller counts use standard single thread qsort, less overhead
//-----------------------------------------------------------------------------
void npSort (void* base, int count, int elementSize, 
			 int(*pCompareFunc)(const void* a, const void* b), void* dataRef )
{
	int i = 0;
	int threads = 0;
	int subsetSize = 0;

	pData data = (pData) dataRef;

	if (0)											//zz debug (count > kNPOverhead)
	{
		//calculate the subsetSize
//		threads = data->map.cpu.coreCount;
		subsetSize = count / threads;

		//divide up into threads each with a subset of the base array
		for (i=0; i < threads; i++)
		{
//			npThread(npSort(), dataRef);
		}

		//wait for the subset to be processed, thread lock
//		npThreadLock(&sortLock);

		//now merge the sorted subsets
//		npMerge();
	}
	else
	{
		//process tags->sort and output to tags->id[]
		qsort(base, count, elementSize, pCompareFunc);
	}
}



//sort the tag list
//-----------------------------------------------------------------------------
void npTagSort (void* dataRef)
{
	pData data = (pData) dataRef;

	if(1)//data->map.tagSortFlag)		//zz debug, check on input if already in order
	{
		printf ("Sort Tags... ");
//		npTagSort(dataRef);
	}

	//sort newly imported tags and then merge sort with existing master list //zz debug
	//this techniqe is useful for adding to existing lists of tags, nodes, etc

	//process tags->sort and output to tags->id[]
//	npSort(base, count, elementSize, npTagCompare);						//zz debug
}

//currently an unsorted list, update to HASH method,						 debug zz
//n^2 function!!!
//-----------------------------------------------------------------------------
void npTagSortAdd (pNPrecordTag recordTag, void* dataRef)
{
	int i = 0;
	pNPrecordTag searchTag = NULL;
	pData data = (pData) dataRef;

	pNPtags tags = &data->io.gl.hud.tags;

	//check max
	if (tags->recordCount >= kNPtagMax - 1)
	{
		npPostMsg ("error 4834 - kNPtagMax exceeded", kNPmsgErr, data);
		return;
	}

	//add record to sort list
	tags->sort[tags->sortCount++] = recordTag;

	//if identical recordID && tableID then replace existing
	for (i=data->io.gl.hud.tags.recordCount; i < kNPtagMax; i++)
//	for (i=data->io.gl.hud.tags.recordCount; i >= 0; i--)
	{
		searchTag = data->io.gl.hud.tags.recordList[i];

		//reached end of list add record
		if (searchTag == NULL)
		{	
			//add the tag to the end of the list
			data->io.gl.hud.tags.recordList[i] = recordTag;
			data->io.gl.hud.tags.recordCount++;

//zz debug		printf("record_id: %d  tag imported %d \n", recordTag->recordID, i);
			i = kNPtagMax;// -1; //exit loop
		}		//if identical record then update it
		else if ( searchTag->recordID == recordTag->recordID
				&& searchTag->tableID == recordTag->tableID )
		{
			data->io.gl.hud.tags.recordList[i] = recordTag;

			free(searchTag);						//zz debug make sure this okay

//zz debug		printf("record_id: %d  tag updated\n", recordTag->recordID);
			i = kNPtagMax;// -1; //exit loop
		}
	}
}

//-----------------------------------------------------------------------------
pNPrecordTag npNewTag(void* dataRef)
{
	pNPrecordTag tag = (pNPrecordTag) malloc(sizeof(NPrecordTag));
	if (tag == NULL)
	{
		printf ("err 4279 - malloc failed to create npNewTag\n");
		return NULL;
	}
	npInitDataRecordTag (tag, dataRef);

	return tag;
}



//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
#define kNPqueDefaultSize 65535

int npQueResize( pNPque que, int newCount, void* dataRef );
void npQueOverflow( pNPque que, void* dataRef );

//------------------------------------------------------------------------------
void npQueOverflow( pNPque que, void* dataRef )
{
	char msg[128];
	
	if ( !que->dynamic )
	{
		sprintf(msg, "err 5335 - fifo overflow, que type: %d", que->type);
		npPostMsg( msg, kNPmsgErr, dataRef );
	}
	else
		npQueResize( que, 0, dataRef );			//newCount = 0 for auto-resize
}

void npQueMapItem( int queID, void* dest, void* srcData, int type, void* dataRef);
// adds the pointer to a specific que for each type
// calls a function based on type, format translation has already been completed
// each thread needs a separate que which is designated by the queID
//------------------------------------------------------------------------------
void npQueMapItem( int queID, void* dest, void* srcData, int type, void* dataRef)
{
	pData data = (pData) dataRef;
	pNPque que = NULL;
	pNPqueItem item = NULL;
	
	// check if queID is inbounds
	if( queID < data->io.fifo.count )
	{
		npPostMsg("err 4677 - npQueMapItem has invalid queID", kNPmsgErr, data);
		return;
	}

	// get que ptr and check if correct type
	que = (pNPque)&data->io.fifo.list[queID];
	if( que->type != kNPfifoMapItem )
	{
		npPostMsg("err 4677 - npQueMapItem has invalid queID", kNPmsgErr, data); 
		return;
	}

	// check to see srcData and dest ptrs exist
	if( !dest || !srcData )
	{
		npPostMsg( "err 5334 - mapItemQue given NULL ptr", kNPmsgErr, data );
		return;
	}

	// increment queItem slot and handle buffer rollover
	que->enque++;
	if ( que->enque >= que->max )
		que->enque = 0;

	// if we catch the tail then handle buffer overflow
	if( que->enque == que->deque )
	{
		npQueOverflow( que, data );
		return;
	}

	// store the dest and srcData ptrs with type in the queItem slot
	item = (pNPqueItem)&que->list[que->enque];
	item->mapPtr = dest;
	item->data = srcData;
	item->type = type;
	
	// increment the index and handle rollover, drop oldest data
	que->enque++;
	if( que->enque >= que->max )
	{
		que->enque = 0;
	}

	return;
}


// resizes the que by allocating a new one and copying contents of old
//zz optimize npMalloc to remove need to copy
//------------------------------------------------------------------------------
int npQueResize( pNPque que, int newCount, void* dataRef )
{
	int size = 0;

	pData data = (pData) dataRef;

	NPqueItem** list = NULL;

	if ( !newCount )
	{	
		newCount = 2 * que->max;			//auto-resize by doubling
	
		if ( !que->max )					//if 0 then set to default que size
			newCount = kNPqueDefaultSize;
	}
	else if ( newCount == que->max )
	{
		npPostMsg( "err 4987 - npQueResize new size == old", kNPmsgWarn, data );
		return 0;
	}

	// npMalloc does not always require a memcpy, memory pools with allowances
	// takes 'quePtr' to set 'que->size'for runtime heuristics
	// que->list = (void**)npMallocResize(quePtr, kNPmemQue, newSize, data);	//zz implement

	// resize the que and copy contents or truncate if necessary
	size = newCount * sizeof( NPqueItem );
	list = (NPqueItem**)npCalloc( kNPque, size, data );
	if( !list ) return 0;

	//if newCount > old then copy contents or if smaller then drops oldest items
	if (newCount > que->max)
		memcpy( list, que->list, que->max * sizeof( NPqueItem ) );
	else
		memcpy( list,		 //grab as much as we can from the end of the list
				&que->list[que->max - newCount], 
				newCount * sizeof( NPqueItem ) );

	que->max = newCount;

	return 1;	//succeeded result = 1 
}

int npQueDel( pNPque que, void* dataRef );
//------------------------------------------------------------------------------
int npQueDel( pNPque que, void* dataRef )
{
	int queID = 0;

	pData data = (pData) dataRef;
	
	//process que item list by freeing the data associated with each element

	return queID;
}

int npQueNew( int queType, int itemCount, void* dataRef );
//------------------------------------------------------------------------------
int npQueNew( int queType, int itemCount, void* dataRef )
{
	int queID = 0;
	int size = 0;

	pData data = (pData) dataRef;
	pNPque que = NULL;
	
	//zz could add routine to dynamically increase queMax
	// check queMax, que[queID] = queID... for speed...
	if( data->io.fifo.count >= data->io.fifo.max )
	{
		npPostMsg( "err 8438 - npNewCmdQue() hit queMax", kNPmsgErr, data );
		return 0;	//queID = 0 indicates failed to create the que
	}

	// select the next empty que[] array slot
	que = (pNPque)data->io.fifo.list[data->io.fifo.count];

	// allocate memory for the que list
	size = itemCount * sizeof(NPqueCmd);
	que->list = (NPqueItem**)npCalloc( kNPqueItem, size, data );
	if( !que->list )
		return 0;

	// increments the queCount after setting it as the array index queID
	que->id = data->io.fifo.count++;

	return queID;
}

void npCmdQue( void* mapPtr, void* valuePtr, int type, void* dataRef );
//------------------------------------------------------------------------------
void npCmdQue( void* funcPtr, void* funcData, int type, void* dataRef )
{//npAddItemToQue()

	pData data = (pData) dataRef;
	pNPque que = (pNPque)data->io.fifo.list[kNPqueItem];
}

struct NPfuncDesc {
	NPtag tag; //char name[128]; //char desc[2048];
	void** paramList;
	int paramCount;
	int returnType;
};
typedef struct NPfuncDesc NPfuncDesc;
typedef struct NPfuncDesc* pNPfuncDesc;


// will post function to the API list and expose any UI controls (if any)
// 
int npCmdFuncNew (void* funcPtr, pNPfuncDesc descriptor, void* dataRef );
//------------------------------------------------------------------------------
int npCmdFuncNew( void* funcPtr, pNPfuncDesc descriptor, void* dataRef )
{
	pData data = (pData) dataRef;
	pNPque que = (pNPque)data->io.fifo.list[kNPqueCmd]; 

	return 0 ;	//funcID = 0 
}

void npProcessQueItem( pNPqueItem item, void* dataRef );
//------------------------------------------------------------------------------
void npProcessQueItem( pNPqueItem item, void* dataRef )
{
	int i = 0;

//	pData data = (pData) dataRef;

//	pNPfloatXYZ floatXYZ = NULL;


	switch( item->type )
	{
		case kNPfloat : *(float*)item->mapPtr = *(float*)item->data; break;
		case kNPvoidPtr : item->mapPtr = item->data; break;
		case kNPint : *(int*)item->mapPtr = *(int*)item->data; break;

		case kNPnode : *(NPnode*)item->mapPtr = *(NPnode*)item->data; break;
		case kNPtag : *(NPtag*)item->mapPtr = *(NPtag*)item->data; break;

		case kNPfloatXYZ :
			*(NPfloatXYZ*)item->mapPtr = *(NPfloatXYZ*)item->data;	//zz will this work?
			break;
		case kNPfloatXYZS :
			*(NPfloatXYZS*)item->mapPtr = *(NPfloatXYZS*)item->data;	//zz will this work?
			break;
		case kNPfloatRGBA :
			*(NPfloatRGBA*)item->mapPtr = *(NPfloatRGBA*)item->data;	//zz will this work?
			break;
		case kNPfloatRGB :
			*(NPfloatRGB*)item->mapPtr = *(NPfloatRGB*)item->data;	//zz will this work?
			break;
		default : 
			npPostMsg( "err 5349 - unknown item in que", kNPmsgErr, dataRef );
			break;
	}

	//need to clear que and free item->data

	printf("err 5333 - unknown mapType");
	return;
}

void npProcessQue(pNPque que, void* dataRef);
//------------------------------------------------------------------------------
void npProcessQue(pNPque que, void* dataRef)
{
	int i = 0;

	pData data = (pData) dataRef;
	pNPqueItem item = NULL;

//	pNPfloatXYZ floatXYZ = NULL;

	item = que->list[0];
	
	//loop through all items in que and process them
	//starts at last deque read index, increments deque while reading
	if ( que->deque > que->enque )
	{
		//read from deque to end of buffer then from beginning up to the enque
		for( i = que->deque; i < que->max; i++)
			npProcessQueItem( que->list[i], data );		//zz optimize this to use ptr (list++)->type
		
		if ( que->enque == 0)
			que->deque = que->max;
		else
			que->deque = 0;
	}

	for( i = que->deque; i < que->enque; i++)
		npProcessQueItem( que->list[i], data );	
	

	//zz debug, need to clear que and free item->data						//zz debug

	return;
}


//------------------------------------------------------------------------------
void npSetMapItem( char* mapPath, char* item, char* element, char* typeTag,
				  char* value, void* dataRef )
{
	int queID = 0;

	void* valuePtr = NULL;
	pNPmapLink mapItem = NULL;

	//get mapPtr
//	mapItem = npGetMapItem( mapPath, item, element, type, dataRef );
	if ( !mapItem )
		return;

	//translate value to native C type
	//zz optimize to use a memory pool, need to call npFree()
//	valuePtr = npCSVtoC( mapItem, value, dataRef );
	if ( !valuePtr )
		return;

	//queID = oscID; //zz debug, OSC listener should specify the queID oscID = queID

	//add mapItem with native (c-struct) value to the que
	npQueMapItem( queID, mapItem->mapPtr, valuePtr, mapItem->typeA, dataRef );
}


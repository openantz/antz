/* -----------------------------------------------------------------------------
*
*  npmap.c
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

#include "npmap.h"

//------------------------------------------------------------------------------
void npInitMap (void* dataRef)
{
	int i = 0;
	pData data = (pData) dataRef;


//#define kNPbaseItemCount 18		//must be updated if below item count changed
	static NPmapType base[] = {

	{ kNPGLboolean,			kNPGLboolean,	"GLboolean",		"openGL boolean" },
	{ kNPGLubyte,			kNPGLubyte,		"GLubyte",			"openGL ubyte" },
	{ kNPGLint,				kNPGLint,		"GLint",			"openGL int" },
	{ kNPGLfloat,			kNPGLfloat,		"GLfloat",			"openGL float" },

	{ kNPboolean,			kNPboolean,		"boolean",			"C boolean" },
	{ kNPubyte,				kNPubyte,		"ubyte",			"C ubyte" },
	{ kNPint,				kNPint,			"int",				"C int" },
	{ kNPfloat,				kNPfloat,		"float",			"openGL float" },

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
	{ kNPchild,				kNPvoidPtr,		"child_list_id",	"list ID of attached child nodes" },
	{ kNPchildIndex,		kNPint,			"child_index",		"specifies the current child" }, 
	{ kNPchildCount,		kNPint,			"child_count",		"current number of children" },

	{ kNPchInputID,			kNPint,			"ch_input_id",		"channel input maps tracks to node" },
	{ kNPchOutputID,		kNPint,			"ch_output_id",		"channel output maps node to tracks" },
	{ kNPchLastUpdated,		kNPint,			"ch_last_updated",	"last cycle channel was updated" },

	{ kNPaverage,			kNPint,			"average",			"averaging type applied to data" },
	{ kNPinterval,			kNPint,			"interval",			"the interval to be averaged" },

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

	{ kNPtagMode,			kNPint,			"tag_mode",			"text mode display method" },
	{ kNPformatID,			kNPint,			"format_id",		"translates values, used for labels" },
	{ kNPtableID,			kNPint,			"table_id",			"location of the records source table" },
	{ kNPrecordID,			kNPint,			"record_id",		"record ID in the source table" },	//debug zz

	{ kNPsize,				kNPint,			"size",				"node size in bytes" }
	};

//#define kNPcameraItemCount 17		//must be updated if below item count changed
	static NPmapType camera[] = {

	{ kNPid,				kNPint,			"id",				"node ID" },
	{ kNPtype,				kNPint,			"type",				"node type" },
		
	{ kNPformat,			kNPint,			"format",			"320p, 480i, 720p, 4K full app" },
	{ kNPinterlaced,		kNPint,			"interlaced",		"interlaced field order" },
	{ kNPstereo,			kNPint,			"stereo",			"stereoscopic 3D" },
	{ kNPaspectRatio,		kNPfloat,		"aspect_ratio",		"1.0, 1.333, 1.777, 1.85, 2.25... " },
	{ kNPfps,				kNPfloat,		"fps",				"15, 24, 29.97, 30, 59.94, 60, 119.88" },
	{ kNPcolorSpace,		kNPint,			"color_space",		"8, 12bit, YUV, RGBA, XYZ, CMYK..." },
	{ kNPwidth,				kNPint,			"width",			"res in pixels" },
	{ kNPheight,			kNPint,			"height",			"res in pixels" },

	{ kNPfov,				kNPfloat,		"fov",				"FOV 35mm, 70mm..." },
	{ kNPclipNear,			kNPfloat,		"clip_near",			"camera near clipping plane" },
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

	//-------
	// clear the typeMap
	for (i=0; i < kNPdataTypeMax; i++)
		data->map.typeMap[i] = NULL;

	data->map.typeMapBase		= base;
	data->map.typeMapNode		= node;
	data->map.typeMapPin		= pin;
	data->map.typeMapCamera		= camera;
	data->map.typeMapGrid		= grid;

	// populate the typeMap
	data->map.typeMap[kNPbase]	= base;
	data->map.typeMap[kNPnode]	= node;
//	data->map.typeMap[kNPcmd] = graph;
	data->map.typeMap[kNPpin]	= pin;
	data->map.typeMap[kNPcamera]= camera;
	data->map.typeMap[kNPgrid]	= grid;


	data->map.selectSet.x	= false;
	data->map.selectSet.y	= false;
	data->map.selectSet.z	= false;
	data->map.selectAll			= false;

	//setup the root node array and the nodeID lookup table
	data->map.nodeCount			= 0;
	data->map.nodeRootCount		= 0;
	data->map.nodeRootIndex		= 0;

	data->map.activeCam			= NULL;
	data->map.currentNode		= NULL;
	data->map.selectedGrid		= NULL;
	data->map.selectedPinNode	= NULL;
	data->map.selectedPinIndex	= 0;

	//clear the node ID array
	data->map.node = (void*) malloc (kNPnodeRootMax * sizeof(void*));
	if (data == NULL)
	{
		printf ("err 4278 - malloc failed to allocate root node array\n");
		exit(EXIT_FAILURE);
	}

	//clear the node ID array
	data->map.sort = (void*) malloc (kNPnodeRootMax * sizeof(void*));
	if (data == NULL)
	{
		printf ("err 4278 - malloc failed to allocate sort array\n");
		exit(EXIT_FAILURE);
	}

	//clear the node ID array
	data->map.nodeID = (void*) malloc (kNPnodeMax * sizeof(void*));
	if (data == NULL)
	{
		printf ("err 4278 - malloc failed to allocate nodeID array\n");
		exit(EXIT_FAILURE);
	}

	//clear the node ID array
	data->map.parentID = (int*) malloc (kNPnodeMax * sizeof(int*));
	if (data == NULL)
	{
		printf ("err 4278 - malloc failed to allocate parentID array\n");
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
		printf ("err 4278 - malloc failed to allocate nodeID array\n");
		exit(EXIT_FAILURE);
	}

	//clear the arrays
	for (i=0; i < kNPnodeRootMax; i++)
	{
		data->map.node[i] = NULL;
		data->map.sort[i] = NULL;
	}

	for (i=0; i < kNPnodeMax; i++)
	{
		data->map.nodeID[i] = NULL;

		data->map.sortID[i] = NULL;
		data->map.parentID[i] = 0;
		data->map.orphanList[i] = 0;
	}
	data->map.orphanCount = 0;
	data->map.sortCount = 0;
}

//------------------------------------------------------------------------------
int npGetRootIndex (NPnodePtr node, void* dataRef)
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
void npSelectNode (NPnodePtr node, void* dataRef)
{
	int rootIndex = 0;
	pData data = (pData) dataRef;

	//return if NULL, not considered an err
	if (node == NULL)
	{
		npPostMsg("Err 6841 - invalid node selected", kNPmsgErr, dataRef);
		
		//select the camera
		node = data->map.activeCam;

		if (node == NULL)
			return;
	}
	
	//traverses up the tree to the pin root and gets the pin index
	rootIndex = npGetRootIndex (node, dataRef);

	//if a pin
	if (node->type == kNodePin)
	{
		data->map.selectedPinIndex = rootIndex;
		data->map.selectedPinNode = node;
	}
	else if (node->type == kNodeCamera)
		data->map.activeCam = node;
	else if (node->type == kNodeGrid)
		data->map.selectedGrid = node;
	else
	{
		npPostMsg("activeCam selected", kNPmsgCtrl, dataRef);
		
		//select the camera
		rootIndex = 1;
		node = data->map.activeCam;
		data->io.mouse.pickMode = kNPpickModeCamera;
	}

	data->map.currentNode = node;
	data->map.nodeRootIndex = rootIndex;
}


//------------------------------------------------------------------------------
NPnodePtr npMapNodeNext (void* dataRef){return NULL;}			//select next sibling node
NPnodePtr npMapNodePrevious (void* dataRef){return NULL;}		//previous sibling
NPnodePtr npMapNodeUp (void* dataRef){return NULL;}				//select parent
NPnodePtr npMapNodeDown (void* dataRef){return NULL;}			//select 

void npSelectNodeID (int id, void* dataRef)
{
	int rootIndex = 0;
	pData data = (pData) dataRef;
}


//applies passed in function to the passed in node and all sub-branches
//------------------------------------------------------------------------------
void npTraverseTree (void (*nodeFunc)(NPnodePtr node, void* dataRef), 
					 NPnodePtr node, void* dataRef)
{
	int i = 0;

	for (i=0; i < node->childCount; i++)
	{
		// recursively call this function
		npTraverseTree (nodeFunc, node->child[i], dataRef);
		
		// apply passed in function operation to node
		//nodeFunc(node->child[i], dataRef); -- JJ, commented this out because it results in double calls, only need single call below
	}
	//process root node last -- JJ, moved from top so now root really is processed last
	nodeFunc(node, dataRef);

}

//applies passed in function to all nodes
//------------------------------------------------------------------------------
void npTraverseMap (void (*nodeFunc)(NPnodePtr node, void* dataRef),
				   void* dataRef)
{
	int i = 0;
	pData data = (pData) dataRef;

	//loop through all nodes and attach tagPtr based on recordID and tableID
	for (i=0; i < data->map.nodeRootCount; i++)
		npTraverseTree (nodeFunc, data->map.node[i], dataRef);
}




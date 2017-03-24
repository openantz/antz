/* -----------------------------------------------------------------------------
*
*  nptypes.c
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

#include "nptypes.h"

#include "npstr.h"		// npNewStrcpy() is handy


void npCopyMapLink( pNPmapLink dest, pNPmapLink src );


/*! Contains methods to create, initialize and delete data types. */ 

/// @todo create external files for hint descriptions and err message text
//  putting lots of strings in the code bloats the app binary

/// @todo loading procedure currently only uses the elementA and not the mapPath
//--------------------------------------------------------------------------
// A-B map link describes both link ends usinng a 3 part address + Type Tag
// 1st part maps the path to the location of the file, DB table, net, RAM
// 2nd part is the row id(s), can be nil = "" or a set {1,2,3,42,42,[50:99]}
// 3rd part is the column field element name
// plus a type tag to desribe parameter value types
//-----------------------------------------------------------------------------
void npMapTypesInit (void* dataRef)
{
	int i = 0;
	int size = 0;

	pData data = (pData) dataRef;

	pNPnode rootGrid = data->map.node[kNPnodeRootGrid];


  NPmapLink mapGlobals[] =
  {								//zz swap id <--> element
		//	data struct ptr			elementTypeID	element		direction	mapPath		index	elementA,		type_tag	title
	{ (void*)NULL,				kNPint,			kNPgNull,			0,	"np",			0,	"null",				"i",	"" }, // "null" },

	{ &data->io.gl.alphaMode,	kNPint,			kNPgAlphaMode,		0, 	"np_gl",		1,	"alpha_mode",		"i",	"" }, // "Transparency Mode" },	
	{ &data->io.clear,			kNPfloatRGBA,	kNPgBackground,		0,	"np_gl",		1,	"background_rgba",	"ffff",	"" }, // "Background Color" },
							
	{ &data->io.gl.fullscreen,	kNPint,			kNPgFullscreen,		0,	"np_gl",		1,	"fullscreen",		"i",	},
	{ &data->io.gl.windowSize,	kNPintXY,		kNPwindowSize,		0,	"np_gl",		1,	"window_size_xy",	"ii",	},
	{ &data->io.gl.position,	kNPintXY,		kNPposition,		0,	"np_gl",		1,	"position_xy",		"ii",	},

	{ &data->io.gl.hud.console.level, kNPint,	kNPhudLevel,		0,	"np_gl",		1,	"hud_level",		"i",	"" }, // "HUD mode level" },
	{ &data->io.gl.subsample,	kNPint,			kNPsubsample,		0,	"np_gl",		1,	"subsample",		"i",	"" }, // "Subsample nodes" },
	
	{ &data->io.mouse.tool,		kNPint,			kNPmouseTool,		0,	"np_mouse",		1,	"tool",				"i",	"" }, // "Tool selected" },
	{ &data->io.mouse.camMode,	kNPint,			kNPmouseCamMode,	0,	"np_mouse",		1,	"cam_mode",			"i",	"" }, // "Mouse Camera Mode" },
	{ &data->io.mouse.pickMode,	kNPint,			kNPmousePickMode,	0,	"np_mouse",		1,	"pick_mode",		"i",	"" }, // "Mouse Pick Mode" },
		
	//zzd { data->io.db.hosts[42],		kNPcstrPtr, kNPloginHostIP,		0, "np_db",	42,	"host_ip",			"s",	"" }, // "Host IP address" },

	{ data->io.db.hosts[1]->ip,			kNPcstrPtr, kNPloginHostIP,		0, "np_db",	1,	"host_ip",			"s",	"" }, // "Host IP address" },
	{ data->io.db.hosts[1]->user,		kNPcstrPtr, kNPloginUser,		0, "np_db",	1,	"user",				"s",	"" }, // "Username" },
	{ data->io.db.hosts[1]->password,	kNPcstrPtr, kNPloginPassword,	0, "np_db",	1,	"password",			"s",	"" }, // "Password" },
	{ data->io.db.hosts[1]->type,		kNPcstrPtr, kNPloginHostType,	0, "np_db",	1,	"db_type",			"s",	"" }, // "Database Type" },

	// OSC IP and port TXRX pairs
	{ &data->io.osc.conns[0].txIP,		kNPcstrPtr,	kNPtxIP,	0,	"np_osc",	1,	"tx_ip",				"s",	"" }, // "Transmit IP" },
	{ &data->io.osc.conns[0].rxIP,		kNPcstrPtr,	kNPrxIP,	0,	"np_osc",	1,	"rx_ip",				"s",	"" }, // "Receive IP" },
	{ &data->io.osc.conns[0].txPort,	kNPint,		kNPtxPort,	0,	"np_osc",	1,	"tx_port",				"i",	"" }, // "TX Port" },
	{ &data->io.osc.conns[0].rxPort,	kNPint,		kNPrxPort,	0,	"np_osc",	1,	"rx_port",				"i",	"" }, // "RX Port" },

	// URL for external Browser record retrieval
	{ data->io.url,				kNPcstrPtr,		kNPgBrowserURL,	0, "np_browser",	1,	"url",				"s",	"" }, // "Browser URL" },
	{ data->io.gitvizURL,		kNPcstrPtr,		kNPgitvizURL,	0, "np_git",		1,	"repo",				"s",	"" }, // "GitViz User/Repo Name" },

	{ &data->map.globalsCount,	kNPint,			kNPitemCount,	1, "np_globals",	1,	"item_count",		"i",	"" } // "Item count for this table"}	//end
  };


  NPmapLink mapOSC[] =
  {
	//	data struct ptr				typeA		elementID  permissions  mapPathA 	itemA			elementA,	  typeTagA	mapPathB itemB elementB		typeTagB ...name desc value
	{ (void*)NULL,					kNPint,		kNPgNull,			0, "osc",		0,					"null",			"",	"osc",		0,	"/np/null", "i" },

	{ &data->ctrl.cmdFunc,		kNPcmdFuncPtr,	kNPcmdTrigger,		0, "np_cmd",	kNPcmdCamera,		"increase_x",	"i", "osc", 0, "/2/push1", "f", "", "", "" },
	{ &data->ctrl.cmdFunc,		kNPcmdFuncPtr,	kNPcmdTrigger,		0, "np_cmd",	kNPcmdSelectToggle, "decrease_x",	"i", "osc", 0, "/2/push2", "f", "", "", "" },
	{ &data->ctrl.cmdFunc,		kNPcmdFuncPtr,	kNPcmdTrigger,		0, "np_cmd",	kNPcmdGrid,			"increase_y",	"i", "osc", 0, "/2/push3", "f", "", "", "" },
	{ &data->ctrl.cmdFunc,		kNPcmdFuncPtr,	kNPcmdTrigger,		0, "np_cmd",	kNPcmdPin,			"decrease_y",	"i", "osc", 0, "/2/push4", "f", "", "", "" },

	{ &data->ctrl.cmdFunc,		kNPcmdFuncPtr,	kNPcmdTrigger,		0, "np_cmd",	kNPcmdColorDown,	"increase_z",	"i", "osc", 0, "/2/push5", "f", "", "", "" },
	{ &data->ctrl.cmdFunc,		kNPcmdFuncPtr,	kNPcmdTrigger,		0, "np_cmd",	kNPcmdColorUp,		"decrease_z",	"i", "osc", 0, "/2/push6", "f", "", "", "" },
//	{ &data->ctrl.cmdFunc,		kNPcmdFuncPtr,	kNPcmdTrigger,		0, "np_cmd",	kNPcmdAltColor,		"select_axes",	"i", "osc", 0, "/2/push7", "f", "", "", "" },
	{ &data->ctrl.cmdFunc,		kNPcmdFuncPtr,	kNPcmdTrigger,		0, "np_cmd",	kNPcmdFullscreen,	"fullscreen",	"i","osc",  0, "/2/push7", "f", "", "", "" },
	{ &data->ctrl.cmdFunc,		kNPcmdFuncPtr,	kNPcmdTrigger,		0, "np_cmd",	kNPcmdTexture,		"rotate_y_right","i","osc", 0, "/2/push8", "f", "", "", "" },
	
	{ &data->ctrl.cmdFunc,		kNPcmdFuncPtr,	kNPcmdTrigger,		0, "np_cmd",	kNPcmdAlphaDown,	"rotate_x_up",	"i", "osc", 0, "/2/push9", "f", "", "", "" },
	{ &data->ctrl.cmdFunc,		kNPcmdFuncPtr,	kNPcmdTrigger,		0, "np_cmd",	kNPcmdAlphaMode,	"rotate_x_down","i", "osc", 0, "/2/push10", "f", "", "", "" },
	{ &data->ctrl.cmdFunc,		kNPcmdFuncPtr,	kNPcmdTrigger,		0, "np_cmd",	kNPcmdHide,			"rotate_z_ccw",	"i", "osc", 0, "/2/push11", "f", "", "", "" },
	{ &data->ctrl.cmdFunc,		kNPcmdFuncPtr,	kNPcmdTrigger,		0, "np_cmd",	kNPcmdAlphaUp,		"rotate_z_cw",	"i", "osc", 0, "/2/push12", "f", "", "", "" },

	{ &data->ctrl.cmdFunc,		kNPcmdFuncPtr,	kNPcmdTrigger,		0, "np_cmd",	kNPcmdNew,			"new",			"i", "osc", 0, "/2/push13", "f", "", "", "" },
	{ &data->ctrl.cmdFunc,		kNPcmdFuncPtr,	kNPcmdTrigger,		0, "np_cmd",	kNPcmdTopo,			"topo",			"i", "osc", 0, "/2/push14", "f", "", "", "" },
	{ &data->ctrl.cmdFunc,		kNPcmdFuncPtr,	kNPcmdTrigger,		0, "np_cmd",	kNPcmdGeometry,		"geometry",		"i", "osc", 0, "/2/push15", "f", "", "", "" },
	{ &data->ctrl.cmdFunc,		kNPcmdFuncPtr,	kNPcmdTrigger,		0, "np_cmd",	kNPcmdDelete,		"delete",		"i", "osc", 0, "/2/push16", "f", "", "", "" },

	

//	{ data->io.url,				kNPcstrPtr,		kNPgBrowserURL,		0, "np_browser", 1,	"url",				 "s",	"osc_np",0,	"/np/browser/1/url", "s" },
	// above is native translation of Globals...
	// below is 3rd party OSC schema				//zz debug duplicate url entries???
	{ data->io.url,				kNPcstrPtr,		kNPgBrowserURL,		0, "np_browser", 1,	"/np/browser/1/url", "s",	"osc",	 0,	"/mrmr/textinput/7/z-zs-iPhone", "s" },
	{ data->io.url,				kNPcstrPtr,		kNPgBrowserURL,		0, "osc_np",	 1,	"/np/browser/1/url", "s",	"osc",	 0,	"/mrmr/textinput/7",			"s" },

	{ &data->io.url,			kNPgBrowserURL,		kNPcstrPtr,		0, "np_globals", 1,	"browser_url",	 "s",	"osc",  1,	"/np/io/browser_url",		"s" },
	// above is a copy of the mapGlobals... 

	
	{ &data->io.clear,			kNPfloatRGBA,	kNPgBackground,		0, "np_gl",	1,	"back_color_rgba",	 "ffff",	"osc",  0,	"/3/xy",					"ff" },
	{ &data->io.clear.r,		kNPfloat,		kNPgBackgroundR,	0, "np_gl",	1,	"back_color_r",			"f",	"osc",  0,	"/1/fader1",				"f" },
	{ &data->io.clear.g,		kNPfloat,		kNPgBackgroundG,	0, "np_gl",	1,	"back_color_g",			"f",	"osc",  0,	"/1/fader2",				"f" },
	{ &data->io.clear.b,		kNPfloat,		kNPgBackgroundB,	0, "np_gl",	1,	"back_color_b",			"f",	"osc",  0,	"/1/fader3",				"f" },
	{ &data->io.clear.a,		kNPfloat,		kNPgBackgroundA,	0, "np_gl",	1,	"back_color_a",			"f",	"osc",  0,	"/1/fader4",				"f" },

	{ &data->map.oscCount,		kNPint,			kNPitemCount,	    1, "osc",	0,	"item_count",			"i",	"osc",  0,	"/np/map/osc/item_count",	"i" } /// item_count designates end of list
	
  }; //end of list

	//add pointer element to the map descriptor
	static NPmapType mapTypeList[] =
	{
		{ 0,	kNPmapNull,		"null",			"" },
		{ 0,	kNPmapGlobals,	"globals",		"" }, // "Accesses global map elements" },
		{ 0,	kNPmapNode,		"node",			"" }, // "Scene Graph tree Node element" },
		{ 0,	kNPmapTag,		"tag",			"" }, // "Text Tag" },
		{ 0,	kNPmapChannel,	"channel",		"" }, // "Channel maps tracks to nodes, globals & commands" },
		{ 0,	kNPmapTrack,	"track",		"" }, // "Tracks animate the scene using files or live IO" },
		{ 0,	kNPmapGL,		"gl",			"" }, // "OpenGL context specific parameters" },
		{ 0,	kNPmapCSV,		"csv",			"" }, // "CSV table descriptor" },
		{ 0,	kNPmapJSON,		"json",			"" }, // "JSON data descriptor" },
		{ 0,	kNPmapCount,	"count",		"" } // "Item count for this list" }
	};

	//scan the list looking for kNPitemCount to dynamically set the list size
	i = 0; while (mapTypeList[i++].type != kNPmapCount) data->map.mapTypeCount++;
	printf( "table map count: %d\n", data->map.mapTypeCount );
	
	// set the ptr to the static array
	data->map.mapTypeList = mapTypeList;

	//scan the list looking for kNPitemCount to dynamically set the list size	//zz debug probably a simpler way to do this?
	i = 0; while (mapGlobals[i++].elementID != kNPitemCount) data->map.globalsCount++;

	// copy the structure to newly allocated RAM								 //zz debug , perhaps a way to avoid this too....
//	size =  -4 * (int)( mapGlobals - *mapGlobals[data->map.globalsCount - 1].value);//sizeof(NPmapLink) * data->map.globalsCount;
	printf( "globals map rows: %d\n", data->map.globalsCount );
	data->map.typeMapGlobals = calloc( data->map.globalsCount, sizeof(NPmapLink)* data->map.globalsCount);
	if( !data->map.typeMapGlobals )
		exit( 8787 );	//exit program if calloc failes during startup
	
	if(1)
	{
		int count = data->map.globalsCount;
		pNPmapLink mapLink = data->map.typeMapGlobals;
	// memcpy(data->map.typeMapGlobals, (const void*)mapGlobals, size);
	
	  //zz note  '<=' helped solve issue #211
	  for( i=0; i <= count; i++)
	  //npCopyMapLink( mapLink[i], mapGlobals[i] );
	  {
		mapLink[i].mapPtr = mapGlobals[i].mapPtr;
		
		mapLink[i].typeA = mapGlobals[i].typeA;
		mapLink[i].elementID = mapGlobals[i].elementID;
		mapLink[i].permissions = mapGlobals[i].permissions;

		mapLink[i].mapPathA = npNewStrcpy( mapGlobals[i].mapPathA, data);
		mapLink[i].itemA = mapGlobals[i].itemA;
		mapLink[i].elementA = npNewStrcpy( mapGlobals[i].elementA, data);
		mapLink[i].typeTagA = npNewStrcpy( mapGlobals[i].typeTagA, data);
		
		mapLink[i].mapPathB = npNewStrcpy( mapGlobals[i].mapPathB, data);
		mapLink[i].itemB = mapGlobals[i].itemB;
		mapLink[i].elementB = npNewStrcpy( mapGlobals[i].elementB, data);
		mapLink[i].typeTagB = npNewStrcpy( mapGlobals[i].typeTagB, data);
	
		mapLink[i].name = npNewStrcpy( "", data);
		mapLink[i].desc = npNewStrcpy( "", data);
		mapLink[i].value = npNewStrcpy( "", data);
	  }
	}

	// REPEAT for mapOSC
	//scan the list looking for kNPcount to dynamically get the size
	i = 0; while (mapOSC[i++].elementID != kNPitemCount) data->map.oscCount++;
	printf( "osc map rows: %d\n", data->map.oscCount );

	data->map.typeMapOSC = calloc( data->map.oscCount, sizeof(NPmapLink ) * data->map.oscCount);
	if( !data->map.typeMapOSC )
		exit( 8788 );	//exit program if calloc failed during startup


	if(1)
	{
		int count = data->map.oscCount;
		pNPmapLink mapLink = data->map.typeMapOSC;

		for( i=0; i < count; i++)
	//		npCopyMapLink( &mapLink[i], &mapGlobals[i] );
		{
			mapLink[i].mapPtr = mapOSC[i].mapPtr;
			
			mapLink[i].typeA = mapOSC[i].typeA;
			mapLink[i].elementID = mapOSC[i].elementID;
			mapLink[i].permissions = mapOSC[i].permissions;

			mapLink[i].mapPathA = npNewStrcpy( mapOSC[i].mapPathA, data);
			mapLink[i].itemA = mapOSC[i].itemA;
			mapLink[i].elementA = npNewStrcpy( mapOSC[i].elementA, data);
			mapLink[i].typeTagA = npNewStrcpy( mapOSC[i].typeTagA, data);
			
			mapLink[i].mapPathB = npNewStrcpy( mapOSC[i].mapPathB, data);
			mapLink[i].itemB = mapOSC[i].itemB;
			mapLink[i].elementB = npNewStrcpy( mapOSC[i].elementB, data);
			mapLink[i].typeTagB = npNewStrcpy( mapOSC[i].typeTagB, data);
		
			mapLink[i].name = npNewStrcpy( "", data);
			mapLink[i].desc = npNewStrcpy( "", data);
			mapLink[i].value = npNewStrcpy( "", data);
		}
	}
printf("TEST 11\n");
//	printf("TEST id: %d\n", npMapStrToID("full_screen\0",data));
//	printf("TEST value: %d\n\n", *(int*)npMapStrToPtr ("full_screen\0", data));	
	//these should change the background color 'b' blue component
//	*(float*)npMapAddressToPtr ("/1/fader3\0", data) = 0.6f;
//	*(float*)npMapAddressToMapTypeItem ("/1/fader3\0", data)->mapPtr = 0.6f;
}

//-----------------------------------------------------------------------------
void npCopyMapLink( pNPmapLink dest, pNPmapLink src )
{
	if( !dest || !src )
		return;

	dest->mapPtr = src->mapPtr;
	
	dest->typeA = src->typeA;
	dest->elementID = src->elementID;
	dest->permissions = src->permissions;

	dest->mapPathA = npNewStrcpy( src->mapPathA, 0);
	dest->itemA = src->itemA;
	dest->elementA = npNewStrcpy( src->elementA, 0);
	dest->typeTagA = npNewStrcpy( src->typeTagA, 0);
	
	dest->mapPathB = npNewStrcpy( src->mapPathB, 0);
	dest->itemB = src->itemB;
	dest->elementB = npNewStrcpy( src->elementB, 0);
	dest->typeTagB = npNewStrcpy( src->typeTagB, 0);

	dest->name = npNewStrcpy( "", 0);
	dest->desc = npNewStrcpy( "", 0);
	dest->value = npNewStrcpy( "", 0);
}

//------------------------------------------------------------------------------
// forms the base data structure, can be augmented at run time
// describes complete data map schema in a CSV/MySQL/JSON compatible manner
// includes globals, node tree graphs, tables, fields, type... etc.
//------------------------------------------------------------------------------
void npInitChMapTypes (void* dataRef)
{
	int i = 0;
	pData data = (pData) dataRef;


//#define kNPbaseItemCount 18		//must be updated if below item count changed
	static NPmapType base[] = {

	{ kNPfloat,				kNPfloat,		"float",			"" }, // "openGL float" },
	{ kNPint,				kNPint,			"int",				"" }, // "C int" },
	{ kNPubyte,				kNPubyte,		"ubyte",			"" }, // "C ubyte" },
	{ kNPbool,				kNPbool,		"bool",				"" }, // "C ubyte boolean" },

	//zz debug remove these?... they are identical to kNP... hmmm
	{ kNPGLboolean,			kNPGLboolean,	"GLboolean",		"" }, // "openGL boolean" },
	{ kNPGLubyte,			kNPGLubyte,		"GLubyte",			"" }, // "openGL ubyte" },
	{ kNPGLint,				kNPGLint,		"GLint",			"" }, // "openGL int" },
	{ kNPGLfloat,			kNPGLfloat,		"GLfloat",			"" }, // "openGL float" },

	// if both the constant and the data type are equal then it is a fundamental type
	// note can be a structure
	{ kNPfloatXYZA,			kNPfloatXYZA,	"NPfloatXYZA",		"" }, // "openGL float x,y,z,angle" },
	{ kNPfloatXYZS,			kNPfloatXYZS,	"NPfloatXYZS",		"" }, // "openGL x,y,z,scaler" },
	{ kNPfloatRGBA,			kNPfloatRGBA,	"NPfloatRGBA",		"" }, // "openGL float r,g,b,a" },
	{ kNPfloatRGB,			kNPfloatRGB,	"NPfloatRGB",		"" }, // "openGL float r,g,b" },
	{ kNPintXYZ,			kNPintXYZ,		"NPintXYZ",			"" }, // "C int x,y,z" },
	{ kNPubyteRGBA,			kNPubyteRGBA,	"NPubyteRGBA",		"" }, // "openGL ubyte r,g,b,a" },
	{ kNPboolXYZS,			kNPboolXYZS,	"NPboolXYZS",		"" }, // "openGL boolean angle,x,y,z" },
	{ kNPboolXYZ,			kNPboolXYZ,		"NPboolXYZ",		"" }, // "openGL boolean x,y,z" },
	{ kNPfloatXY,			kNPfloatXY,		"NPfloatXY",		"" }, // "openGL float x,y" },
	{ kNPfloatXYZ,			kNPfloatXYZ,	"NPfloatXYZ",		"" } // "openGL float x,y,z" }
	};

	// used by the CSV header and DB field names
	// node currently expands to 94 columns or elements such as channel x,y,z
//#define kNPnodeItemCount 52		//must be updated if below item count changed
	static NPmapType node[] = {

	{ kNPid,				kNPint,			"id",				"" }, // "node ID" },
	{ kNPtype,				kNPint,			"type",				"" }, // "node type" },
	{ kNPdata,				kNPint,			"data",				"" }, // "node type specific data" },

	{ kNPselected,			kNPint,			"selected",			"" }, // "node currently selected" },

	{ kNPparent,			kNPvoidPtr,		"parent_id",		"" }, // "parent node in a data tree" },
	{ kNPbranchLevel,		kNPint,			"branch_level",		"" }, // "branch depth on tree, 0 is root node" },
	{ kNPchild,				kNPvoidPtr,		"child_id",			"" }, // "list ID of attached child nodes" },
	{ kNPchildIndex,		kNPint,			"child_index",		"" }, // "specifies the current child" }, 
	{ kNPchildCount,		kNPint,			"child_count",		"" }, // "current number of children" },

	{ kNPchInputID,			kNPint,			"ch_input_id",		"" }, // "channel input maps tracks to node" },
	{ kNPchOutputID,		kNPint,			"ch_output_id",		"" }, // "channel output maps node to tracks" },
	{ kNPchLastUpdated,		kNPint,			"ch_last_updated",	"" }, // "last cycle channel was updated" },

	{ kNPaverage,			kNPint,			"average",			"" }, // "averaging type applied to data" },
	{ kNPinterval,			kNPint,			"samples",			"" }, // "the interval to be averaged" },

/// @todo change kNPintXYZ to kNPfloatXYZ for both aux_a and aux_b
	{ kNPauxA,				kNPintXYZ,		"aux_a",			"" }, // "node type specific parameters" },	//zz grid NO change
	{ kNPauxB,				kNPintXYZ,		"aux_b",			"" }, // "reserved for future use" },

	{ kNPcolorShift,		kNPfloat,		"color_shift",		"" }, // "color shift is akin to hue" },
	
	{ kNProtateVec,			kNPfloatXYZS,	"rotate_vec",		"" }, // "rotation unit vector" },
	{ kNPscale,				kNPfloatXYZ,	"scale",			"" }, // "scale node XYZ" },
	{ kNPtranslate,			kNPfloatXYZ,	"translate",		"" }, // "position relative to parent node" },
	{ kNPtagOffset,			kNPfloatXYZ,	"tag_offset",		"" }, // "text tag offset relative to node center" },

	{ kNProtateRate,		kNPfloatXYZ,	"rotate_rate",		"" }, // "angular velocity" },
	{ kNProtate,			kNPfloatXYZ,	"rotate",			"" }, // "orienation such as heading, tilt, roll" },
	{ kNPscaleRate,			kNPfloatXYZ,	"scale_rate",		"" }, // "scale rate" },
	{ kNPtranslateRate,		kNPfloatXYZ,	"translate_rate",	"" }, // "velocity vector" },
	{ kNPtranslateVec,		kNPfloatXYZ,	"translate_vec",	"" }, // "user defined vector" },

	{ kNPshader,			kNPint,			"shader",			"" }, // "wire, flat, gouraud, phong" },
	{ kNPgeometry,			kNPint,			"geometry",			"" }, // "cube, sphere, pin, FFT mesh, etc" },

	{ kNPlineWidth,			kNPfloat,		"line_width",		"" }, // "draws lines, zero for off" },
	{ kNPpointSize,			kNPfloat,		"point_size",		"" }, // "draws dots, zero for off" },
	{ kNPratio,				kNPfloat,		"ratio",			"" }, // "geometry ratio, ie torus inner radius" },

	{ kNPcolorIndex,		kNPint,			"color_index",		"" }, // "specifies the color from the palette"},
	{ kNPcolor,				kNPubyteRGBA,	"color",			"" }, // "color assigned to new data" },
	{ kNPcolorFade,			kNPint,			"color_fade",		"" }, // "fades alpha of older data points"},
	{ kNPtextureID,			kNPint,			"texture_id",		"" }, // "GL texture ID" },

	{ kNPhide,				kNPbool,		"hide",				"" }, // "hide the node, data stays active" },
	{ kNPfreeze,			kNPbool,		"freeze",			"" }, // "freezes data and physics updates" },
	
	{ kNPtopo,				kNPint,			"topo",				"" }, // "topography, cube, sphere, torus, etc" },
	{ kNPfacet,				kNPint,			"facet",			"" }, // "topo face number" },
	
	{ kNPautoZoom,			kNPintXYZ,		"auto_zoom",		"" }, // "scales node to fit screen" },
	
	{ kNPtriggerHi,			kNPintXYZ,		"trigger_hi",		"" }, // "trigger if high setpoint hit" },
	{ kNPtriggerLo,			kNPintXYZ,		"trigger_lo",		"" }, // "trigger if low setpoint hit" },
	{ kNPsetHi,				kNPfloatXYZ,	"set_hi",			"" }, // "trigger high setpoint" },
	{ kNPsetLo,				kNPfloatXYZ,	"set_lo",			"" }, // "trigger low setpoint" },
	{ kNPproximity,			kNPfloatXYZ,	"proximity",		"" }, // "radius used for proximity detection" },
	{ kNPproximityMode,		kNPintXYZ,		"proximity_mode",	"" }, // "physics engine collision mode" },

	{ kNPsegments,			kNPintXYZ,		"segments",			"" }, // "segment count, such as the grid" },

	{ kNPtagMode,			kNPint,			"tag_mode",			"" }, // "text tag display mode" },
	{ kNPformatID,			kNPint,			"format_id",		"" }, // "translates values, used for labels" },
	{ kNPmapID,				kNPint,			"table_id",			"" }, // "location of the records source table" },
	{ kNPrecordID,			kNPint,			"record_id",		"" }, // "record ID in the source table" },	//debug zz

	{ kNPsize,				kNPint,			"size",				"" } // "node size in bytes" }
	};

//#define kNPcameraItemCount 17		//must be updated if below item count changed
	static NPmapType camera[] = {

	{ kNPid,				kNPint,			"id",				"" }, // "node ID" },
	{ kNPtype,				kNPint,			"type",				"" }, // "node type" },
		
	{ kNPformat,			kNPint,			"format",			"" }, // "320p, 480i, 720p, 4K full app" },
	{ kNPinterlaced,		kNPint,			"interlaced",		"" }, // "interlaced field order" },
	{ kNPstereo,			kNPint,			"stereo_3d",		"" }, // "stereoscopic 3D" },
	{ kNPaspectRatio,		kNPfloat,		"aspect_ratio",		"" }, // "1.0, 1.333, 1.777, 1.85, 2.25... " },
	{ kNPfps,				kNPfloat,		"fps",				"" }, // "15, 24, 29.97, 30, 59.94, 60, 119.88" },
	{ kNPcolorSpace,		kNPint,			"color_space",		"" }, // "" }, // "8, 12bit, YUV, RGBA, XYZ, CMYK..." },
	{ kNPwidth,				kNPint,			"width",			"" }, // "res in pixels" },
	{ kNPheight,			kNPint,			"height",			"" }, // "res in pixels" },

	{ kNPfov,				kNPfloat,		"fov",				"" }, // "FOV 35mm, 70mm..." },
	{ kNPclipNear,			kNPfloat,		"clip_near",		"" }, // "camera near clipping plane" },
	{ kNPclipFar,			kNPfloat,		"clip_far",			"" }, // "camera far clipping plane" },

	{ kNPaperture,			kNPfloat,		"aperture",			"" }, // "F stop" },
	{ kNPexposure,			kNPfloat,		"exposure",			"" }, // "in seconds" },
	{ kNPsensorType,		kNPint,			"sensor_type",		"" }, // "3CCD, Debayer pattern..." },

	{ kNPsize,				kNPint,			"size",				"" } // "data size in bytes" }
	};

//#define kNPgridItemCount 5		//must be updated if below item count changed
	static NPmapType grid[] = {

	{ kNPid,				kNPint,			"id",				"" }, // "node ID" },
	{ kNPtype,				kNPint,			"type",				"" }, // "node type" },

	{ kNPsegments,			kNPintXYZ,		"segments",			"" }, // "number of grid segments" },
	{ kNPoverlay,			kNPbool,		"overlay",			"" }, // "overlays relative to screen coord" },

	{ kNPsize,				kNPint,			"size",				"" } // "node size in bytes" }
	};

//#define kNPpinItemCount 12		//must be updated if below item count changed
	static NPmapType pin[] = {

	{ kNPid,				kNPint,			"id",				"" }, // "node ID" },
	{ kNPtype,				kNPint,			"type",				"" }, // "node type" },

	{ kNPinnerRadius,		kNPfloat,		"inner_radius",		"" }, // "inner radius of the toroid" },
	{ kNPouterRadius,		kNPfloat,		"outer_radius",		"" }, // "outer radius of the toroid" },
	{ kNPradiusRatio,		kNPint,			"radius_ratio",		"" }, // "sorts pre-loaded GPU toriods" },
	{ kNPscreenSize,		kNPint,			"screen_size",		"" }, // "same as above for detail level" },
	{ kNPslices,			kNPint,			"slices",			"" }, // "calculated from radiusRatioIndex" },
	{ kNPstacks,			kNPint,			"stacks",			"" }, // "and from the screenSizeIndex" },
	{ kNPtranslateTex,		kNPfloatXYZ,	"translate_tex",	"" }, // "offsets texture origin" },
	{ kNPscaleTex,			kNPfloatXYZ,	"scale_tex",		"" }, // "scales the texture" },
	{ kNProtateTex,			kNPfloatXYZS,	"rotate_tex",		"" }, // "orientation of the texture map" },
	{ kNPsize,				kNPint,			"size",				"" } // "node size in bytes" }
	};

	//zzsql
	static NPmapType tag[] = {

		{ kNPid,			kNPint,			"id",				"" }, // "tag ID" },
		{ kNPrecordID,		kNPint,			"record_id",		"" }, // "Record ID" },
		{ kNPmapID,			kNPint,			"table_id",			"" }, // "Table ID" },
		{ kNPtitle,			kNPcharArray,	"title",			"" }, // "Title" },
		{ kNPdesc,			kNPcharArray,	"description",		"" } // "Description" }
	}; // debug db
	
	static NPmapType ChMap[] = {

		{ kNPid,			kNPint,			"id",				"" }, // "Channel Map ID" },
		{ kNPchannelID,		kNPint,			"channel_id",		"" }, // "Channel ID"	 },
		{ kNPtrackID,		kNPint,			"track_id",			"" }, // "Track ID"		 },
		{ kNPattribute,		kNPcharArray,	"attribute",		"" }, // "Attribute"		 },
		{ kNPtrackTableID,	kNPint,			"track_table_id",	"" }, // "Track Table ID" },
		{ kNPchMapTableID,	kNPint,			"ch_map_table_id",	"" }, // "Channel Map Table ID"},
		{ kNPrecordID,		kNPint,			"record_id",		"" } // "Record ID"		}
	}; // debug db //zzsql
	
	//zz color
	static NPmapType palette[] = {

		{ kNPid,			kNPint,			"id",				"" }, // "Color Palette Index" },
		{ kNPrecordID,		kNPint,			"red",				"" }, // "Red" },
		{ kNPmapID,			kNPint,			"green",			"" }, // "Green" },
		{ kNPtitle,			kNPint,			"blue",				"" }, // "Blue" },
		{ kNPdesc,			kNPint,			"alpha",			"" } // "Alpha Transparency" }
	};
/*	
	//zz tex
	static NPmapType texture[] = {

		{ kNPid,			kNPint,			"np_texture_id",	"Texture ID" },
		{ kNPtype,			kNPint,			"type",				"Type: 2D, 3D, Cubemap, Video" },
		{ kNPfileName,		kNPcharArray,	"file_name",		"File Name" },
		{ kNPpath,			kNPcharArray,	"path",				"File Path" }
	};
	
	//zz mod
	static NPmapType models[] = {

		{ kNPid,			kNPint,			"np_models_id",		"3D Models ID" },
		{ kNPid,			kNPint,			"np_geometry_id",	"Geometry ID" },
		{ kNPid,			kNPint,			"np_texture_id",	"Texture ID" },
		{ kNPtype,			kNPint,			"type",				"Type: Mesh, Terrain, etc." },
		{ kNPobjName,		kNPcharArray,	"object_name",		"3D Scene Object Name" },
		{ kNPfileName,		kNPcharArray,	"file_name",		"File Name" },
		{ kNPpath,			kNPcharArray,	"path",				"File Path" }
	};
*/	


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
//	data->map.typeMapTexture	= texture;	//zz tex
//	data->map.typeMapModels		= models;	//zz mod

	data->map.typeMapGlobals	= NULL;		//zz debug
	
	data->map.mapTypeList = NULL;
	data->map.mapTypeCount = 0;

	// populate the typeMap
//	data->map.typeMap[kNPcmd]	= command;
	data->map.typeMap[kNPbase]	= base;
	data->map.typeMap[kNPnode]	= node;
	data->map.typeMap[kNPpin]	= pin;
	data->map.typeMap[kNPcamera]= camera;
	data->map.typeMap[kNPgrid]	= grid;

}


//--------------
//-------------
//---------
//--

	// DB Table Formats
	// CSV - our 'native' external format... Lowest-Common-Denominator (LCD)
	// directly translates to our internal C-structs and optimized for GL draw
	// ...
	// MySQL tables are directly (loss-less) inter-changable with our CSV files
	// JSON allows efficiently describing data tree structure and its members
	// XML is less efficient then JSON but similar in many ways

	// OSC - our 'native' Network PROTOCAL
	// OSC is an oddball... its a 'keep it simple stupid' approach (to a fault)
	// typically OSC is sent over UDP/IP, but can be sent over any network comm
	// does NOT provide quality-of-service(QOS) but works with AVB to do so
	// OSC-MIDI bridge (3rd party software modules and standalone hardware)
	
	// REALTIME Video Network Hardware Layers (QOS)
	// Serial such as RS232, RS422, serial over USB and SPI for chip-to-chip
	// DVB - Digital Video Broadcasting transmits over satellite, fiber, coax
	// AVB - Audio Video Broadcasting provides QOS for AVB Ethernet devices
	// SDI - Serial Digital Interface for video and audio or coax, has metadata

	// HIGH Performance Networking
	// FC - Fibre Channel is the HIGHEST performance, 8Gbps 250K IOPS
	// Infiniband - Bandwidth similar to FC, IOPS are about 25K IOPS
	// 10GigE - Bandwidth performance varies 6-10Gbps (typical) 10K IOPS

	// ATM - A-sync Transfer M...

	// In general, FC is best for HPC environments such as VR Caves
	//----

	//----
		// itemID = can be a specific item number '42', a range '[42:100]', a mixed list '{42, 43, 44, 45, [55:62]}'	//zz debug, change mapType->item_id to string
		// or the '*' (wildcard) can be used to assign (default init) values to all items
		// id = 'active' applies to all currently active (selected) item(s)
		// item_id = 0 is a valid index
		// negative id values behaviour is undefined and will generate a warning... for example '[3-10]' is an invalid format
		// single instance items (globals) without an id use item_id = 0, for 'nil' 
		// item_id meaning is based on the map_path format...
		// OSC uses the ID field to represent the connection ID where as 'np_...' uses it as an array index or record_id
		
		// bias = 0 for full duplex, output = TX = +1.0 for A to B, input = RX = -1.0 for B to A
		// bias  0.5 = 100% A to B and 50% B to A ... ie mapping XY to RGBA color
		//
		// key value (record_id) is a combo of the map_path (file path, DB name and table_id...) and id

		//build descriptor table for globals with pointer to specific elements
		//note that mapItemID = element = address (CSV field name for native types)

			
	//	{ &data->io,				kNPstrList,		kNPmainArgcArgv,	0,	"globals",		0,	"main_argc_argv",	"is",	},
	//	{ &data->io.mouse.PickMode,	kNPint,			kNPmouseCamMode,	0,	"np_mouse",		1,	"cam_mode",			"i",	"Mouse Camera Mode" },
	//	{ data->io.osc.list[].protocal,	kNPcstrPtr,	kNPprotocal,		0,	"np_connect",	1,	"protocal",			"s",	"Protocal ie: mysql osc vrpn snmp" },

	// tables require the element list to be unique, but only within the table scope
	// Play/Pause tracks

	// antz native csv table field 'elementB' name matches the 'elementA' name
	// other 3rd party CSV table fields can be mapped and would NOT match
	// OSC most often uses a float normalized 0.0 to 1.0 for sliders, buttons...


//JJ-zz-osc

	// go through all nptypes.h lists and make one for elements and one for paths
	// may add connection specific OSC maps for greater 3rd party support //zz debug
	//zz-JJ planning to add base_type(globals,node...) and record_id columns
	//make sure all the mapPath constants are in a single enumerated list

	// list processed from beginning to end, new items added to end 
	// this means they are processed first and override earlier entries... similar to c++ class inheritance
	// shortcut addresses are used, perhaps expand this to auto-complete partial address, return close matches, suggest...
	// some elements trigger function calls such as fullscreen, screen_size, etc... 


//--------------
//-------------
//---------
//--

	/*
	{ &data->ctrl.cmdFunc,		kNPcmdFuncPtr,	kNPcmdTrigger,		0, "np_cmd",	kNPcmdCamera,		"increase_x",	"i", "osc", 0, "/2/push1", "f", "", "", "" },
	//alternate layout comparing set A with set B
	{ &data->io.gl.alphaMode,	kNPint,			kNPgAlphaMode,							//C struct
							0, 	"np_gl",	1,	"alpha_mode",				"i",			//set A C-struct converted to String Version
								"osc",		0,	"/np/gl/1/alpha_mode",		"f"			},	//set B external mapping type such as OSC or CSV														
											//  kNPcolorRGBA	..						//clear_color													//back_color_rgba 
	{ &data->io.clear,			kNPfloatRGBA,	kNPgBackground,		0, "np_gl",		1,	"color_rgba",	"ffff",	"osc",  0,	"/np/gl/1/color_rgba", "ffff" , "Background Color" },

	{ &data->io.clear.r,		kNPfloat,		kNPgBackgroundR,	0, "np_gl",		1,	"color_r",		"f",	"osc",  0,	"/np/gl/1/color_r",	"f" , "Background Color R" },
	{ &data->io.clear.g,		kNPfloat,		kNPgBackgroundG,	0, "np_gl",		1,	"color_g",		"f",	"osc",  0,	"/np/gl/1/color_g",	"f" , "Background Color G" },
	{ &data->io.clear.b,		kNPfloat,		kNPgBackgroundB,	0, "np_gl",		1,	"color_b",		"f",	"osc",  0,	"/np/gl/1/color_b",	"f" , "Background Color B" },
	{ &data->io.clear.a,		kNPfloat,		kNPgBackgroundA,	0, "np_gl",		1,	"color_a",		"f",	"osc",  0,	"/np/gl/1/color_a",	"f" , "Background Color A" },

	
	{ &data->map.node,			kNPint,			kNPrecordID,	    1, "np_node",		1,	"record_id",	"i",	"osc",  0,	"/np/node/42/record_id",	"i" },	//should be a double insted of 'i'
																					//0 denotes wildcard, ie: * = "selected" for selected node(s) kNPcmdTag makes a tag for currently selected items
																					// or "active" for current active target, or "set*" for all selection sets
*/ //	{ &data->map.node,			kNPtagPtr,		kNPtag,	    1, "np_node",	0,	"tag",				"iss",	"osc",  0,	"/np/node/*/tag",		"iss" },	//get or set tag, create tag if none
/*
	// setting and instance of a node or tag that does not exist will create it on the fly
*/ //	{ &data->map.node,			kNPint,			kNPtagID,			1, "np_node",	0,	"tag_id",			"i",	"osc",  0,	"/np/node/*/tag_id",	"i" },		//get or set tagID
//	{ &data->map.node,			kNPcstrPtr,		kNPtagName,			1, "np_node",	1,	"tag_name",			"s",	"osc",  0,	"/np/node/*/tag_name",	"s", "redirect: /np/tag/[../node/*/tag_id]/name" },
//	{ &data->map.node,			kNPcstrPtr,		kNPtagDesc,			1, "np_node",	1,	"tag_desc",			"s",	"osc",  0,	"/np/node/*/tag_desc",	"s" },

//	{ data->io.url,				kNPcstrPtr,		kNPgBrowserURL,		0, "np_browser", 1,	"/np/browser/1/url", "s",	"osc",	 0,	"/mrmr/textinput/7/z-zs-iPhone", "s" },


	// data->io.gl.fullscreen +1 direction status flag is OUT (only)
	// +1 direction status flag denotes A to B and that its an output (read only)
	// data->io.g[id] context 'id' = 1 is the main GL window, 0 = system console
//	{ &data->io.gl,				kNPint,			kNPfullscreen,	    1, "np_gl",		1,	"fullscreen",		"i",	"osc",  0,	"/np/cmd/fullscreen",	"f" },
//	{ &data->ctrl.cmdFunc,		kNPcmdFuncPtr,	kNPcmdTrigger,		0, "np_cmd",	kNPcmdFullscreen, "fullscreen",		"i",	"osc",  0,	"/2/push7",	"f" },

	// kNPcmdFullscreen will toggle the fullscreen mode of the active user window
	// and is the same as pressing 'ctrl_cmd' + F or 'esc' key
	// alternatively, kNPcmdFullscreenID will go fullscreen on a specific gl_context_id //zz implement

	// active user window context can be set separately from the active command context
/*	{ &data->ctrl.cmdFunc,		kNPcmdFuncPtr,	kNPcmdContext,		0, "np_cmd",	kNPcmdContext,	"context_next",		"i",	"osc",  0,	"/np/cmd/context_next",	"f" },
	{ &data->ctrl.cmdFunc,		kNPcmdFuncPtr,	kNPcmdContextID,	0, "np_cmd",	kNPcmdContextID, "context_id",		"i",	"osc",  0,	"",	"f" },
	{ &data->ctrl.cmdFunc,		kNPcmdFuncPtr,	kNPcmdUserFocus,	0, "np_cmd",	kNPcmdUserFocus, "user_focus",		"i",	"osc",  0,	"",	"f" },

//	{ &data->io.clear.a,		kNPfloat,		kNPposition,		0, "np_gl",		1,	"position_xy",		"f",	"osc",  0,	"/np/gl/1/background_a",	"f" },
//	{ &data->io.clear.a,		kNPfloat,		kNPscreen,			0, "np_gl",		1,	"screen_xy",		"fff",	"osc",  0,	"/np/gl/1/background_a",	"f" },

//	{ &data->io.clear.a,		kNPfloat,		kNPdimensions,		0, "np_gl",		1,	"dimensions",		"fff",	"osc",  0,	"/np/gl/1/background_a",	"f" },
//	{ &data->io.clear.a,		kNPfloat,		kNPdimensions,		0, "np_gl",		1,	"dimensions_height","f",	"osc",  0,	"/np/gl/1/dimensions_height",	"f" },
//	{ &data->io.clear.a,		kNPfloat,		kNPdimensions,		0, "np_gl",		1,	"dimensions_depth",	"f",	"osc",  0,	"/np/gl/1/dimensions_depth",	"f" },

	
	{ &rootGrid->scale.z,		kNPfloat,		kNPnodeRootGrid,	0, "np_node",	 0, "root_grid_scale_z", "f", "osc", 0, "/1/fader4", "f", "", "applies to entire scene" },

//	{ &data->io.gl.subsample,	kNPint,			kNPcmdSubsample,	0, "np_gl",		1, "subsample", "i", "osc", 0, "/1/fader5", "f", "", "applies to entire scene" },

//	{ &data->ctrl->cmdFuncList,	kNPint,			kNPcmdTrigger,		0, "np_cmd",	0, "select_all_on", "f", "osc", 0, "/1/toggle1", "f", "", "applies to entire scene" },
	{ &data->ctrl.cmdFunc,		kNPcmdFuncPtr,	kNPcmdTrigger,		0, "np_cmd",	kNPcmdSelectAll, "select_all", "i", "osc", 0, "/1/toggle4", "f", "", "applies to entire scene" },
	{ &data->ctrl.cmdFunc,		kNPcmdFuncPtr,	kNPcmdTrigger,		0, "np_cmd",	kNPcmdSelectAll, "select_all", "i", "osc", 0, "/2/toggle4", "f", "", "applies to entire scene" },
	{ &data->ctrl.cmdFunc,		kNPcmdFuncPtr,	kNPcmdTrigger,		0, "np_cmd",	kNPcmdSelectAll, "select_all", "i", "osc", 0, "/3/toggle4", "f", "", "applies to entire scene" },
	{ &data->ctrl.cmdFunc,		kNPcmdFuncPtr,	kNPcmdTrigger,		0, "np_cmd",	kNPcmdSelectAll, "select_all", "i", "osc", 0, "/4/toggle4", "f", "", "applies to entire scene" },
*//*
	{ &data->ctrl.cmdFunc,		kNPcmdFuncPtr,	kNPcmdTrigger,		0, "np_cmd",	kNPcmdXincrease, "increase_x", "i", "osc", 0, "/2/push1", "f", "", "", "" },
	{ &data->ctrl.cmdFunc,		kNPcmdFuncPtr,	kNPcmdTrigger,		0, "np_cmd",	kNPcmdXdecrease, "decrease_x", "i", "osc", 0, "/2/push2", "f", "", "", "" },
	{ &data->ctrl.cmdFunc,		kNPcmdFuncPtr,	kNPcmdTrigger,		0, "np_cmd",	kNPcmdYincrease, "increase_y", "i", "osc", 0, "/2/push3", "f", "", "", "" },
	{ &data->ctrl.cmdFunc,		kNPcmdFuncPtr,	kNPcmdTrigger,		0, "np_cmd",	kNPcmdYdecrease, "decrease_y", "i", "osc", 0, "/2/push4", "f", "", "", "" },
	{ &data->ctrl.cmdFunc,		kNPcmdFuncPtr,	kNPcmdTrigger,		0, "np_cmd",	kNPcmdZincrease, "increase_z", "i", "osc", 0, "/2/push5", "f", "", "", "" },
	{ &data->ctrl.cmdFunc,		kNPcmdFuncPtr,	kNPcmdTrigger,		0, "np_cmd",	kNPcmdZdecrease, "decrease_z", "i", "osc", 0, "/2/push6", "f", "", "", "" },

	{ &data->ctrl.cmdFunc,		kNPcmdFuncPtr,	kNPcmdTrigger,		0, "np_cmd",	kNPcmdRotateLeft, "rotate_y_left",	"i", "osc", 0, "/2/push7", "f", "", "", "" },
	{ &data->ctrl.cmdFunc,		kNPcmdFuncPtr,	kNPcmdTrigger,		0, "np_cmd",	kNPcmdRotateRight,"rotate_y_right",	"i", "osc", 0, "/2/push8", "f", "", "", "" },
	{ &data->ctrl.cmdFunc,		kNPcmdFuncPtr,	kNPcmdTrigger,		0, "np_cmd",	kNPcmdRotateUp,	  "rotate_x_up",	"i", "osc", 0, "/2/push9", "f", "", "", "" },
	{ &data->ctrl.cmdFunc,		kNPcmdFuncPtr,	kNPcmdTrigger,		0, "np_cmd",	kNPcmdRotateDown, "rotate_x_down",	"i", "osc", 0, "/2/push10", "f", "", "", "" },
	{ &data->ctrl.cmdFunc,		kNPcmdFuncPtr,	kNPcmdTrigger,		0, "np_cmd",	kNPcmdRotateCCW,  "rotate_z_ccw",	"i", "osc", 0, "/2/push11", "f", "", "", "" },
	{ &data->ctrl.cmdFunc,		kNPcmdFuncPtr,	kNPcmdTrigger,		0, "np_cmd",	kNPcmdRotateCW,   "rotate_z_cw",	"i", "osc", 0, "/2/push12", "f", "", "", "" },
*/
//	{ &data->ctrl.cmdFunc,		kNPcmdFuncPtr,	kNPcmdTrigger,		0, "np_cmd",	kNPcmdRotateCW,		"rotate_z_cw",	"i", "osc", 0, "/2/push16", "f", "", "", "" },

//	{ &data->ctrl.cmdFunc,		kNPcmdFuncPtr,	kNPcmdTrigger,		0, "np_cmd",	kNPcmdZoomOn,		"zoom_in",		"i", "osc", 0, "/2/push16", "f", "", "" },
//	{ &data->ctrl.cmdFunc,		kNPint,			kNPcmdTrigger,		0, "np_cmd",	kNPcmdZoomOut,		"rotate_z_cw",	"i", "osc", 0, "/2/push12", "f", "", "" },


	//kNPmouseDown "ii"
	//kNPmouseUp
	//kNPmouseMove

	//color, texture, transparency -,+
	//topo, geo, 

	// example of mapping osc directly to np and then mapping 3rd party 'osc' to native osc 'osc_np'
	// osc - osc maps can be dynamically created and routed without passing through np...
	// if an 'osc_np' address entry is used then it can be dynamically mapped directly to an internal c-struct parameter...
	// if both osc - osc mapping pair addresses are unknown then the data can be routed RX->TX and auto-referenced by the internal global c-structure

		//zz - update to auto-generate (learn) new OSC addresses spaces, dynamic node parsing...

	//	data struct ptr			elementTypeID		type/tag		a_map_path	id	element					mapPathA,  id	element					type_tag
//	{ &data->io.gl.position,	kNPposition,		kNPfloatXY,		0, "np_gl",		1,	"position_xy",		"ii",	"osc",  1,	"/np/gl/1/position_xy",		"ii" },
//	{ &data->io.gl,screen,		kNPscreen,			kNPfloatXY,		0, "np_gl",		1,	"screen_size_xy",	"ii",	"csv",	1,	"screen_size_xy",			"ii" },


//--------------
//-------------
//---------
//--

/*
	{ &data->,		kNPfloat,		kNP,	0, "np_",	1,	"",			"f",	"osc",  0,	"/COG/NEUTRAL",				"f" },
	{ &data->,		kNPfloat,		kNP,	0, "np_",	1,	"",			"f",	"osc",  0,	"/COG/PUSH",				"f" },
	{ &data->,		kNPfloat,		kNP,	0, "np_",	1,	"",			"f",	"osc",  0,	"/COG/PULL",				"f" },
	{ &data->,		kNPfloat,		kNP,	0, "np_",	1,	"",			"f",	"osc",  0,	"/COG/LIFT",				"f" },
	{ &data->,		kNPfloat,		kNP,	0, "np_",	1,	"",			"f",	"osc",  0,	"/COG/DROP",				"f" },
	{ &data->,		kNPfloat,		kNP,	0, "np_",	1,	"",			"f",	"osc",  0,	"/COG/LEFT",				"f" },
	{ &data->,		kNPfloat,		kNP,	0, "np_",	1,	"",			"f",	"osc",  0,	"/COG/RIGHT",				"f" },
	{ &data->,		kNPfloat,		kNP,	0, "np_",	1,	"",			"f",	"osc",  0,	"/COG/ROTATE_LEFT",			"f" },
	{ &data->,		kNPfloat,		kNP,	0, "np_",	1,	"",			"f",	"osc",  0,	"/COG/ROTATE_RIGHT",		"f" },
	{ &data->,		kNPfloat,		kNP,	0, "np_",	1,	"",			"f",	"osc",  0,	"/COG/ROTATE_CLOCKWISE",	"f" },
	{ &data->,		kNPfloat,		kNP,	0, "np_",	1,	"",			"f",	"osc",  0,	"/COG/ROTATE_COUNTER_CLOCKWISE", "f" },
	{ &data->,		kNPfloat,		kNP,	0, "np_",	1,	"",			"f",	"osc",  0,	"/COG/ROTATE_FORWARDS",		"f" },
	{ &data->,		kNPfloat,		kNP,	0, "np_",	1,	"",			"f",	"osc",  0,	"/COG/ROTATE_REVERSE",		"f" },
	{ &data->,		kNPfloat,		kNP,	0, "np_",	1,	"",			"f",	"osc",  0,	"/COG/DISAPPEAR",			"f" },
*//*
	{ &data->io.clear.b,		kNPfloat,		kNPgBackgroundB,	0, "np_gl",	1,	"back_color_b",			"f",	"osc",  0,	"/AFF/Frustration",			"f" },
	{ &data->io.clear.b,		kNPfloat,		kNPgBackgroundB,	0, "np_gl",	1,	"back_color_b",			"f",	"osc",  0,	"/AFF/Engaged/Bored",		"f" },
	{ &data->io.clear.g,		kNPfloat,		kNPgBackgroundG,	0, "np_gl",	1,	"back_color_g",			"f",	"osc",  0,	"/AFF/Excitement",			"f" },
	{ &data->io.clear.b,		kNPfloat,		kNPgBackgroundB,	0, "np_gl",	1,	"back_color_b",			"f",	"osc",  0,	"/AFF/Excitement Long Term","f" },
	{ &data->io.clear.r,		kNPfloat,		kNPgBackgroundR,	0, "np_gl",	1,	"back_color_r",			"f",	"osc",  0,	"/AFF/Meditation",			"f" },

	{ &data->io.clear.r,		kNPfloat,		kNPgBackgroundB,	0, "np_gl",	1,	"back_color_r",			"f",	"osc",  0,	"/EXP/WINK_LEFT",			"f" },
	{ &data->io.clear.g,		kNPfloat,		kNPgBackgroundB,	0, "np_gl",	1,	"back_color_g",			"f",	"osc",  0,	"/EXP/WINK_RIGHT",			"f" },
	{ &data->io.clear.b,		kNPfloat,		kNPgBackgroundB,	0, "np_gl",	1,	"back_color_b",			"f",	"osc",  0,	"/EXP/BLINK",				"f" },
	{ &data->io.clear.r,		kNPfloat,		kNPgBackgroundB,	0, "np_gl",	1,	"back_color_r",			"f",	"osc",  0,	"/EXP/LEFT_LID",			"f" },
	{ &data->io.clear.g,		kNPfloat,		kNPgBackgroundB,	0, "np_gl",	1,	"back_color_g",			"f",	"osc",  0,	"/EXP/RIGHT_LID",			"f" },
	{ &data->io.clear.r,		kNPfloat,		kNPgBackgroundR,	0, "np_gl",	1,	"back_color_r",			"f",	"osc",  0,	"/EXP/HORIEYE",				"f" },
	{ &data->io.clear.g,		kNPfloat,		kNPgBackgroundG,	0, "np_gl",	1,	"back_color_g",			"f",	"osc",  0,	"/EXP/VERTEYE",				"f" },
	{ &data->io.clear.b,		kNPfloat,		kNPgBackgroundB,	0, "np_gl",	1,	"back_color_b",			"f",	"osc",  0,	"/EXP/SMILE",				"f" },
	{ &data->io.clear.r,		kNPfloat,		kNPgBackgroundB,	0, "np_gl",	1,	"back_color_r",			"f",	"osc",  0,	"/EXP/CLENCH",				"f" },
	{ &data->io.clear.g,		kNPfloat,		kNPgBackgroundB,	0, "np_gl",	1,	"back_color_g",			"f",	"osc",  0,	"/EXP/LAUGH",				"f" },
	{ &data->io.clear.b,		kNPfloat,		kNPgBackgroundB,	0, "np_gl",	1,	"back_color_b",			"f",	"osc",  0,	"/EXP/SMIRK_LEFT",			"f" },
	{ &data->io.clear.r,		kNPfloat,		kNPgBackgroundB,	0, "np_gl",	1,	"back_color_r",			"f",	"osc",  0,	"/EXP/SMIRK_RIGHT",			"f" },
	{ &data->io.clear.g,		kNPfloat,		kNPgBackgroundB,	0, "np_gl",	1,	"back_color_g",			"f",	"osc",  0,	"/EXP/FURROW",				"f" },
	{ &data->io.clear.b,		kNPfloat,		kNPgBackgroundB,	0, "np_gl",	1,	"back_color_b",			"f",	"osc",  0,	"/EXP/EYEBROW",				"f" },
*/


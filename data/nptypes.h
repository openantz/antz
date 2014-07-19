/* -----------------------------------------------------------------------------
*
*  nptypes.h
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

#ifndef NPTYPES_H_
#define NPTYPES_H_

#include <time.h>

#include "stdbool.h"

#define kPI			 3.141593f
#define kPos2PI		 6.283185f
#define kNeg2PI		-6.283185f
#define kRADtoDEG	57.29578f

#define	kNPtextureMax		2048

#define kNPkeyMapSize		256				//keyboard map
#define kNPkeyEventTypeSize 4

#define kNPmapFileBufferMax 134217727		// 128MB current file size limit
											
#define	kNPnodeMax			262144			// 1MB with 32bit ptr, 2MB if 64bit
#define kNPnodeRootMax		16383			// C99 max fix array size, 32bit OS
#define kNPnodeChildMax		266				// 1KB RAM each node with 32bit OS
											// 266 fills a sphere at 15 deg
											// switch data structure to GTK.org zz

#define kNPcameraCount		4				// default number of camera views

#define kNPfacetCube		6				// cube facet count

#define kNPcubeLength		0.7072f			// cube length, width, height
#define kNPsphereRadius		1.0f			// default radius of a sphere
#define kNPtorusRadius		1.5f			// default radius of a torus
#define kNPdefaultRatio		0.1f			// default innerRadius of a torus
#define kNPpinHeight		5.0f			// z offset of child coordinates

#define kNPgridSpacing		30.0f			// defualt grid spacing, was 10.0
#define kNPgridSegmentsX	12				// default grid 16x16, was 16
#define kNPgridSegmentsY	12				// default grid 16x16, was 16

#define kNPpaletteSize		20				// index mapped color pallete

#define	kNPmaxPath			4096			// max file path, msw supports 260
#define	kNPurlMax			4096			// maximum length of URL accepted

#define kNPdataTypeMax		4096			//used for CSV header field names
#define kNPbaseItemCount	18
#define kNPnodeItemCount	52
#define kNPcameraItemCount	17
#define kNPgridItemCount	5
#define kNPpinItemCount		12				//important to keep these up-to-date

#define kNPconsoleLineMax		40			//default is 80x40 character layout
#define kNPconsoleCharPerLine	80
#define kNPconsoleHistoryMax	800			//max entries to store in RAM
#define kNPmessageQueMax		500			//max messages stored in que
#define kNPmessageLengthMax		127			//max message length

#define kNPtitleSizeMax			63			// max length of the tag title
#define kNPdescSizeMax			4095		// max length of the tag description	
#define	kNPtagListMax			16383		// max number tags allowed to draw
#define	kNPrecordTagListMax		kNPnodeMax	// max number of record Tags


#define kNPmaxTrackToAttributeMappings 1000 //JJ maximum number of mappings supported (fixed size array)
#define kNPattributeNameOffset	3			//JJ attribute name column in the ChMap file:  id, ch, track, attribute name, id1, id2, id3
#define kNPchannelOffset		1			//JJ channel offset column in the ChMap file:   0   1    2         3           4    5    6
#define kNPtrackOffset			2			//JJ track offset column in the ChMap file:     0   1    2         3           4    5    6

#define kNPchannelMax			128			// max number of channels
#define kNPtrackBufferSize		65535		//JJ number of samples possible in the track buffer, NOTE: this is a circular
											// buffer, the maximum size available to antz at any moment
#define kNPmaxTracks			1000		//JJ maximum number of tracks supported on a channel
#define kNPmaxFloatProperties	10000		//JJ maximum number of float properties fed by channel tracks
#define kNPmaxUcharProperties	10000		//JJ maximum number of unsigned char properties fed by channel tracks
#define kNPmaxLineLength		10000		//zz-JJ move to nptypes.h
#define kNPmaxTokens			1000		//zz-JJ move to nptypes.h


//------------------------------------------------------------------------------
// Base Types - designed to be directly compatible with OpenGL

typedef struct {
	float x,y;
} NPfloatXY;

struct NPfloatXYZ{
	float x,y,z;
};
typedef struct NPfloatXYZ NPfloatXYZ;
typedef struct NPfloatXYZ * pNPfloatXYZ;

typedef struct {
	float x,y,z,angle;
} NPfloatXYZA;

typedef struct {
	float x,y,z,scaler;
} NPfloatXYZS;

typedef struct {
	float r,g,b;
} NPfloatRGB;

typedef struct {
	float r,g,b,a;
} NPfloatRGBA;

typedef struct {
	int x,y;
} NPintXY;

typedef struct {
	int x,y,z;
} NPintXYZ;

typedef struct {
	int x,y,z,s;
} NPintXYZS;

typedef struct {
	unsigned char r,g,b,a;
} NPubyteRGBA;

typedef struct {
	bool x,y,z;
} NPboolXYZ;

typedef struct {
	bool x,y,z,s;
} NPboolXYZS;


//------------------------------------------------------------------------------
// Custom Types - multiple instances -------------------------------------------

typedef void (*voidFnPtr)(void*);	//zz-JJ


struct NPmapType
{
	int		id;
	int		type;
	char*	name;
	char*	note;
};
typedef struct NPmapType NPmapType;
typedef struct NPmapType *NPmapTypePtr;


struct NPrecordTag
{
	int			id;					//the first column is table id key

	int			recordID;			//recordID of the node
	int			tableID;			//tableID of the node, may not need these, debug zz

	char		title[kNPtitleSizeMax + 1];	//one line with max width of 63 characters
	char		desc[kNPdescSizeMax + 1];	//allows for newline in desc
	
	int			titleSize;			//number of characters in title
	int			descSize;			//number of characters in description

	int			size;
};
typedef struct NPrecordTag NPrecordTag;
typedef struct NPrecordTag *pNPrecordTag;

struct NPtextTag
{
	int			tableID;			//tableID of the node, may not need these, debug zz
	int			recordID;			//recordID of the node

	char		title[kNPtitleSizeMax + 1];	//one line with max width of 63 characters
	char		desc[kNPdescSizeMax + 1];	//allows for newline in desc
	
	int			titleSize;		//number of characters in title
	int			descSize;		//number of characters in description

	void*		font;

	int			mode;			//2D, 3D billboard, fixed in model space
	float		lineWidth;		//1 line title only, 3 lines, max lines

	NPfloatXY	boxSize;

	NPubyteRGBA color;			//used for the text and box outline
	NPubyteRGBA boxColor;		//background color of box
	NPubyteRGBA lineColor;		//box outline

	int			size;
};
typedef struct NPtextTag NPtextTag;
typedef struct NPtextTag *pNPtextTag;


// nodes are organized as a branched tree, supports complexity of linked objects
// ie a recNode of type nodeCamera may have a multiple depth daughter tree that
// supports several cameras or videoNodes for circle cam rigs etc...
//-----------------------------------------------------------------------------
struct NPnode
{
	int			id;							//local node ID 
	int			type;						//node type, point, pin, cam, video

	void*		data;						//node type specific data
	
	int			selected;					//true if node currently selected

	void*		parent;						//parent node, binary tree of nodes
	int			branchLevel;				//0 is the trunk, 1 is 1st branch
											
	void*		child[kNPnodeChildMax];		//children attached to this one
	int			childIndex;					//the currently selected child
	int			childCount;					//current number of children

	int			chInputID;					//track to node channel_id map
	int			chOutputID;					//node to track record channel_id
	int			chLastUpdated;				//cycle of last channel update

	int			average;					//averaging type applied to data
	int			interval;					//the interval to be averaged

	NPintXYZ	auxA;						//reserved for future use
	NPintXYZ	auxB;						//reserved for future use

	float		colorShift;					//pseudo Hue shift, -180 to 180

	NPfloatXYZA	rotateVec;					//orientation
	NPfloatXYZ	scale;						//scale node XYZ
	NPfloatXYZ	translate;					//location relative to origin
	NPfloatXYZ	tagOffset;					//origin relative to parent origin

	NPfloatXYZ	rotateRate;					//rotation rate
	NPfloatXYZ	rotate;						//rotation angle in polar radians
	NPfloatXYZ	scaleRate;					//scale rate 
	NPfloatXYZ	translateRate;				//node velocity
	NPfloatXYZ	translateVec;				//velocity unit vector

	int			shader;						//the texture ID or procedural shader type
	int			geometry;					//cube, sphere, pin, FFT mesh...

	float		lineWidth;					//draws lines, zero for off
	float		pointSize;					//draws dots, zero for off
	float		ratio;						//geometry ratio, torus innerRadius

	int			colorIndex;
	NPubyteRGBA	color;						//color assigned to new data

	int			colorFade;					//cycles to fade color alpha, 0=off
	int			textureID;					//GL texture ID

	bool		hide;						//hide the node, data stays active
	bool		freeze;						//freezes data and physics updates

	int			topo;						//topography, cube, sphere, torus
	int			facet;						//topo face number

	NPintXYZ	autoZoom;					//scales node to fit screen, scroll, center to node origin

	NPintXYZ	triggerHi;				//triggers are multipurpose
	NPintXYZ	triggerLo;
	NPfloatXYZ	setHi;					//triggers
	NPfloatXYZ	setLo;

	NPfloatXYZ	proximity;					//used for collision engine 
	NPintXYZ	proximityMode;				//used for collision engine 
		
	NPintXYZ	segments;					//for topos and grid segments

	int			tagMode;						//user defined value
	int			formatID;					//DB translation to antz field, label
	int			tableID;					//DB table
	int			recordID;					//DB recordID

	int			size;

	// below used for transparency and tags, not needed in CSV or DB file
	pNPtextTag	tag;													//zz debug
	NPfloatXYZ	screen;						//MB-LABEL
	float		distFromCamera;				//MB-Transp					//zz debug
};
typedef struct NPnode NPnode;
typedef struct NPnode *NPnodePtr;


//------------------------------------------------------------------------------
// Global Data Structure -------------------------------------------------------

struct NPkey {
	int		modShift;
	int		modCtrl;
	int		modAlt;
	int		modCommand;
	int		map[kNPkeyEventTypeSize][kNPkeyMapSize];

	int		size;
};
typedef struct NPkey NPkey;
typedef struct NPkey * pNPkey;

struct NPmouse {
	int			x;						//raw coordinate
	int			y;
	int			z;						//typically the scroll wheel

	int			mode;
	int			pickMode;				//locks the mouse into cam mode
	int			tool;

	bool		buttonL;				//true when pressed
	bool		buttonC;
	bool		buttonR;

	NPintXYZ	window;					//window position in pixels

	NPintXYZ	previous;

	float		cmDX;
	float		cmDY;

	NPfloatXYZ	delta;					//mouse vector
	NPfloatXYZ	deltaSum;				//sum of deltas, clears each NPE period

	NPfloatXYZ	target;
	NPfloatXYZ	targetDest;
	float		targetRadius;
	float		targetRadiusDest;
	int			translating;

	bool		pinSelected;

	int			size;					// memory used, add/del should modify this, debug zz
};
typedef struct NPmouse NPmouse;
typedef struct NPmouse * pNPmouse;

struct NPmessage
{
	int			queIndex;					//the most recent message

	char		que[kNPmessageQueMax][kNPmessageLengthMax + 1]; //+1 for null

	int			size;
};
typedef struct NPmessage NPmessage;
typedef struct NPmessage * pNPmessage;

struct NPconsole {	
	int			selected;					//true if node currently selected

	NPfloatXYZ	translate;					//location relative to origin

	float		lineWidth;					//draws lines, zero for off

	int			colorIndex;
	NPubyteRGBA	color;						//color assigned to new data

	int			boxColorIndex;
	NPubyteRGBA	boxColor;					//color assigned to new data

	int			colorFade;					//color alpha fade with time
	int			textureID;					//GL texture ID used for box background

	bool		hide;						//hide the node, data stays active
	bool		freeze;						//freezes data and physics updates

	NPfloatXYZ	screen;
	NPintXY		position;

	int			mode;						//1 line, 3 lines, max lines
	NPfloatXY	box;						//background box size

	void*		font;
	int			justify;

	int			lineMax;					//default is 40 lines
	int			charPerLine;				//default is 80 characters per line

	int			lineIndex;					//the currently displayed line
	int			lineCount;
	int			historyMax;					//max number of lines stored

	char		line[kNPconsoleLineMax][kNPconsoleCharPerLine+1]; //+1 for null char

	//char*		history[kNPconsoleHistoryMax];	//debug, zz

	int			size;
};
typedef struct NPconsole NPconsole;
typedef struct NPconsole * pNPconsole;

struct NPtags {
	int			count;
	void**		list;

	int			recordCount;
	void**		recordList;

	int			size;
};
typedef struct NPtags NPtags;
typedef struct NPtags * pNPtags;

struct NPhud {	
	bool		drawTags;
	bool		drawCompass;
	bool		drawCoord;
	bool		drawFPS;
	bool		drawConsole;
	bool		drawAxes;
	bool		drawMouseMode;
	bool		drawSelectMode;

	NPtags		tags;
	NPconsole	console;
	NPtextTag	fps;

	int			size;
};
typedef struct NPhud NPhud;
typedef struct NPhud * pNPhud;

struct NPgl {
	int			normal;
	int			shade;
	int			alphaMode;

	int			textureCount;

	int			pickPass;
	int			pickID;

	int			width;
	int			height;
	int			pixelDepth;
	float		refreshRate;

	int			fullScreen;
	int			stereo;

	int			windowID;

	NPhud		hud;

	int			size;
};
typedef struct NPgl NPgl;
typedef struct NPgl * pNPgl;


struct NPmap {
	void**	node;					//root node array, size of kNPnodeRootMax
	void**	sort;					//used for z-sort during GL draw

	void**	nodeID;					//maps node ID to NPnodePtr, kNPnodeMax
	void**	sortID;					//maps node ID for sorting nodes

	int*	parentID;				//maps node ID to parentID
	int*	orphanList;				//list of orphans by node ID
	int		orphanCount;
	int		sortCount;
									// 2^24 max for picking algorithm
	int		nodeCount;				//total for root and children nodes
	int		nodeRootCount;			//number of root nodes
	int		nodeRootIndex;			//the active node root
	
	NPnodePtr	currentNode;		//active node, commands, traversing tree
	NPnodePtr	activeCam;			//active camera used for zsort distance
	NPnodePtr	selectedGrid;		//selected grid
	NPnodePtr	selectedPinNode;	//current node selection, can be a child branch
	int			selectedPinIndex;	//helpful to know which tree we are on

	NPboolXYZ	selectSet;			//current selection set to add too
	int			selectAll;			//true when all nodes selected

	NPmapTypePtr	typeMap[kNPdataTypeMax];

	NPmapTypePtr	typeMapBase;
	NPmapTypePtr	typeMapNode;
	NPmapTypePtr	typeMapCamera;
	NPmapTypePtr	typeMapPin;
	NPmapTypePtr	typeMapGrid;
	NPmapTypePtr	typeMapTorus;

	int		size;					// memory used, add/del should modify this, debug zz
};
typedef struct NPmap NPmap;
typedef struct NPmap * pNPmap;


//JJ - currently using track data column naming convention for mapping track-to-node-attribute (bottom part of structure will be unnecessary when channel file exists)
struct NPxref {
	int			channelNumber;
	int			trackOffset;
	char*		attributeName;
};
typedef struct NPxref NPxref;
typedef struct NPxref * pNPxref;

// the pointer logic is same in all cases, processing requires different casting for the "void*" and "void**"
// for example, when processing float properties, these have to be cast to "float*" and "float**"
struct NPnodePropertyTracks {
	// for mapping track-to-node-attribute -- set up when a Node subscribes to a channel 
	void*		nodePropertyMemoryLocations[kNPmaxFloatProperties];		// memory address of each node property fed from track data
	void**		nodePropertyMemoryNextLocation;							// next available location for a float property memory address
	int			propertyTracks[kNPmaxFloatProperties];					// offset of the tracks that feed each peroperty
	int			propertyTrackIndex;										// next available propertyTrack array offset
};
typedef struct NPnodePropertyTracks NPnodePropertyTracks;
typedef struct NPnodePropertyTracks * pNPnodePropertyTracks;

#define kNPchannelMapHeader "id,channel_id,track,attribute,track_table_id,ch_map_table_id,record_id"

struct NPch {
	int			setNumberToLoad;

	//
	//  LOADED FROM CHANNEL FILE
	//
	NPxref		xref[kNPmaxTrackToAttributeMappings];
	int			xrefIndex;
	char		channelFileTokenBuffer[kNPmaxLineLength];
	char*		channelFileTokens[kNPmaxTokens];			

	//
	// LOADED FROM TRACKS FILES
	//
	// data source for the tracks
	FILE*		fileDataSource;
	char*		fullFilePath;
	char		trackFileTokenBuffer[kNPmaxLineLength];
	char*		trackFileTokens[kNPmaxTokens];			

	// properties apply to all tracks on the channel
	bool		updateData;				// true when we need to update data (either read from track to update node, or vice versa)
	int			channelReadIndex;		// offset in all tracks, next data to read
	int			channelWriteIndex;		// offset in all tracks, next data to read
	int			channelDataSize;		// amount of data in each track, any value between 0 and kNPtrackBufferSize
	int			channelSampleRate;		// sample rate for this channel, # of milliseconds to keep same channel data, e.g. 50 === 20 changes per second (1000 ms/50 ms)
	long long	channelNextDataChangeTime;	// next time to change data (by updating channelReadIndex) on this channel, used with sample rate to determine when to change data
	
	// track specific data, data loaded from the track file
	int			numberTracks;			// number of tracks in this channel
	char**		trackNames;				// each track has a name, name indicates node attribute (multiple allowed, separated by anything except ",")

	// all data types created for each track (until we have way of knowing which is needed)
	float**		trackFloatData;			// list of pointers to "float" values, track data as "float"
	unsigned char** trackUcharData;		// list of pointers to "unsigned char" values, track data as "unsigned char"
	int**		trackIntData;			// list of pointers to "int" values, track data as "int"

	NPnodePropertyTracks	floatTracks;// tracks associated with "float" Node properties
	NPnodePropertyTracks	ucharTracks;// tracks associated with "unsigned char" Node properties
	NPnodePropertyTracks	intTracks;	// tracks associated with "int" Node properties

	bool		pause;

	int			size;
};
typedef struct NPch NPch;
typedef struct NPch * pNPch;


struct NPfile
{
	char		appPath[4096];
	char		csvPath[4096];
	char		mapPath[4096];
//	char		cwdPath[4096];			//zz debug, maybe better to use GetCWD
	char		userSelectedPath[4096];

	int			size;
};
typedef struct NPfile NPfile;
typedef struct NPfile * pNPfile;


struct NPio {
	int			argc;
	char**		argv;

	NPmessage	message;
	NPkey		key;
	NPmouse		mouse;
	NPgl		gl;
	NPch		ch;
	NPfile		file;

	double		time;				//system time in seconds
	double		timeStart;			//time at app start
	double		cycleDelta;			//the actual duration of the last cycle
	double		cycleRateHz;		//target rate, not necessarily the actual
	double		cyclePeriod;		//period calculated from rate
	int			cycleCount;			//number of cycles since app startup

	int			clearFrameBuffer;
	int			clearIndex;
	NPfloatRGBA clear;				//background color for clearing the screen

	int			drawMenu;

	NPboolXYZ	axes;					//selects active x,y,z, axes			debug, zz

	char*		write[kNPdataTypeMax];

	char		url[kNPurlMax];

	int			size;
};
typedef struct NPio NPio;
typedef struct NPio * pNPio;

struct NPctrl {
	float	slow;						// slow velocity, shift not pressed
	float	fast;						// fast velocity, shift key pressed 

	int		size;
};
typedef struct NPctrl NPctrl;
typedef struct NPctrl * pNPctrl;

struct Data {
	NPmap	map;
	NPio	io;
	NPctrl	ctrl;

	int		size;
};
typedef struct Data Data;
typedef struct Data * pData;


// for displaying labels related to node parameters
// displays the name at the top, followed by X,Y,Z values in a column
// unitType determines display format and appends the unit label, ie 9.8 m/s^2
// the nodeField (ie, translate.x) is first multiplied by the factor then offset
// displayValue = nodeField * factor + offset
//------------------------------------------------------------------------------
struct NPlabel
{
	char		name[64];			//author, city, population...

	NPfloatXYZS	factor;				//multiplies the value for scaling range
	NPfloatXYZS	offset;				//constant to offset value by

	NPintXYZS	unitType;			//ft, m, F, C, kg/m^3, m/s^2, R,G,B...

//	NPintXYZS	formatType;			//specifies conversion math, y = mx + c

	NPintXYZS	nodeField;			//which node parameter to display
};
typedef struct NPlabel NPlabel;
typedef struct NPlabel *NPlabelPtr;


// cameras can be GL scene cameras or real-world cameras
// video is attached as a daughter of the recNode
//-----------------------------------------------------------------------------
struct NPcamera
{
	int			id;							//node ID
	int			type;

	int			format;						//320p, 480i, 720p, 4K full app
	int			interlaced;					//interlaced field order
	int			stereo;						//stereoscopic 3D
	float		aspectRatio;				//1.0, 1.333, 1.777, 1.85, 2.25... 
	float		fps;						// 15, 24, 29.97, 30, 59.94, 120...
	int			colorSpace;					//8, 12bit, YUV, RGBA, XYZ, CMYK...
	int			width;						//res in pixels
	int			height;

	float		fov;						//FOV 35mm, 70mm...
	float		clipNear;
	float		clipFar;
	
	// real world camera parameters
	float		aperture;					//F stop
	float		exposure;					//in seconds
	int			sensorType;					//3CCD, Debayer pattern...

	int			size;
};
typedef struct NPcamera NPcamera;
typedef struct NPcamera *NPcameraPtr;


//-----------------------------------------------------------------------------
struct NPvideo
{
	int			id;							//node ID
	int			type;

	int			format;						//320p, 480i, 720p, 4K full app
	int			interlaced;					//interlaced field order
	int			stereo;						//stereoscopic 3D
	float		aspectRatio;				//1.0, 1.333, 1.777, 1.85, 2.25... 
	float		fps;						// 15, 24, 29.97, 30, 59.94, 120...
	int			colorSpace;					//8, 12bit, YUV, RGBA, XYZ, CMYK...
	int			width;						//res in pixels
	int			height;

	float		brightness;
	float		contrast;
	float		hue;
	float		saturation;
	float		blackLevel;

	NPfloatRGBA	bias;						//color shift
	NPfloatRGBA	gain;						//color multiplier

	int			size;
};
typedef struct NPvideo NPvideo;
typedef struct NPvideo *NPvideoPtr;

//-----------------------------------------------------------------------------
struct NPgis
{
	int			id;							//node ID
	int			type;

	float		*vertexArray;				//the array of vertices
	float		*normalArray;				//the array of normals
	unsigned char	*colorArray;				//each point gets color assigned
	int			vertexCount;				//
	int			normalCount;				//
	int			arrayIndex;					//current vertex index in the array
	//			polyLine
	//			polyStrip

	int			size;
};
typedef struct NPgis NPgis;
typedef struct NPgis *NPgisPtr;



//-----------------------------------------------------------------------------
struct NPsurface
{
	int			id;							//node ID
	int			type;

	float		*vertexArray;				//the array of vertices
	float		*normalArray;				//the array of normals
	unsigned char	*colorArray;				//each point gets color assigned
	int			vertexCount;				//
	int			normalCount;				//
	int			arrayIndex;					//current vertex index in the array

	int			size;
};
typedef struct NPsurface NPsurface;
typedef struct NPsurface *NPsurfacePtr;


//-----------------------------------------------------------------------------
struct NPpoints
{
	int			id;							//node ID
	int			type;
	float		*vertexArray;				//the array of data points
	unsigned char	*colorArray;				//each point gets color assigned
	int			vertexCount;				//number of points in the node
	int			arrayIndex;					//current vertex index in the array

	int			size;
};
typedef struct NPpoints NPpoints;
typedef struct NPpoints *NPpointsPtr;


//-----------------------------------------------------------------------------
struct NPpin
{
	int			id;							//node ID
	int			type;

	float		innerRadius;				//inner radius of the toroid
	float		outerRadius;				//outer radius of the toroid

	int			radiusRatioIndex;			//sorts pre-loaded GPU toriods
	int			screenSizeIndex;			//same as above for detail level
	
	int			slices;						//calculated from radiusRatioIndex
	int			stacks;						//...and from the screenSizeIndex

	NPfloatXYZ	translateTex;
	NPfloatXYZ	scaleTex;
	NPfloatXYZA	rotateTex;					//orientation of the texture map

	int			size;
};
typedef struct NPpin NPpin;
typedef struct NPpin *NPpinPtr;


//-----------------------------------------------------------------------------
struct NPgrid
{
	int			id;							//node ID
	int			type;

	NPfloatXYZ	spacing;
	bool		overlay;					//overlays relative to screen coord

	int			size;
};
typedef struct NPgrid NPgrid;
typedef struct NPgrid * NPgridPtr;


//-----------------------------------------------------------------------------
struct NPnodeData
{
	int			id;							//node ID
	int			type;

	int			index;
	void*		a;			//points to an array of data pointers
	int*		dataType;		//points to an int of the data type,

	int			size;
};
typedef struct NPnodeData NPnodeData;
typedef struct NPnodeData * pNPnodeData;


//-----------------------------------------------------------------------------
struct NPdataItem
{
	int			type;
	void*		item;
};
typedef struct NPdataItem NPdataItem;
typedef struct NPdataItem * NPdataItemPtr;

// constants ------------------------------------------------------------------
enum
{
	kNodeDefault = 0,

	kNodeCamera,			//for both real-world and virtual GL cameras
	kNodeVideo,				//video from live/stream, internal GL, or file
	kNodeSurface,			//ground, sky... background objects drawn first
	kNodePoints,			//for X/XY/XYZ line, eeg/ecg, temp, movement...
	kNodePin,				//combo of cones, spheres, toroids, etc...
	kNodeGrid,				//grid in 1D, 2D or 3D
};

/*
enum
{
	kSurfaceNull = 0,
	kSurfacePlane,
	kSurfaceCylinder,
	kSurfaceCone,
	kSurfaceSphere,
	kSurfaceTorus,
	kSurfacePin,							// ice-cream cone shape
	kSurfaceMesh,							// terrain...
	kSurfaceLayers,							// ie stack of FFT images, MRI...
	kSurfaceCamera,
	kSurfaceTypeCount
};
*/

enum
{
	kSelectNULL = 0,
	kSelectAll,
	kSelectNone,
	kSelectAdd,
	kSelectRemove,
	kSelectColor,
	kSelectParent,
	kSelectSiblings,
	kSelectChildren,
	kSelectProperty
};

enum
{
	kGetNull = 0,
	kNodeColor,
	kNodeChannel,
};

enum
{
	kNPmapNull = 0,
	
	kNPmapAll,

	kNPmapUniverses,
	kNPmapUniverse,
	kNPmapGalaxyCluster,
	kNPmapGalaxy,
	kNPmapSolar,

	kNPmapPlanet,				// Earth
	kNPmapRegion,				// < 30 ms latency, < 4000km
	kNPmapLocation,			// < 3 ms latency, < 400km
	kNPmapCluster,			// < 1 ms latency, < 40km
	kNPmapNode,				// < 1 us latency, < 4m, CPU node, not scene node
						
	kNPmapBUS,				// Node maps
	kNPmapCPU,
	kNPmapRAM,
	kNPmapGPU,

	kNPmapIO,					// typically a bus device if a node

	kNPmapDrive,
	kNPmapNetwork,
	kNPmapVideo,
	kNPmapAudio,

	kNPmapHaptic,
	kNPmapKey,
	kNPmapMouse,
	kNPmapJoystick,
	kNPmapTablet,
	kNPmapTouchScreen,
	kNPmapHID,

	kNPmapNPE,
	kNPmapDraw,				//a list of objects to draw in a GL context 
	kNPmapQuadsort,			//break scene into quadrants, cluster and NPE
	kNPmapZsort,				//GL draw order, for correct alpha rendering

	kNPmapTree,
	kNPmapList,
	kNPmapArray,
	
	kNPmapCount
};

enum
{
	kShadingNull = 0,
	kShadingWire,
	kShadingFlat,
	kShadingGouraud,
	kShadingPhong,
	kShadingReflection,
	kShadingRaytrace,
	kShadingCount
};

enum
{
	kCameraSensorNull = 0,					//don't know the sensor type
	kCameraSensorMonoCCD,					//monochrome CCD such as BW, IR
	kCameraSensorDebayerCCD,				//single color CCD debayer pattern
	kCameraSensorThreeCCD,					//3CCD color sensor
	kCameraSensorDepthCCD					//BW or color with Z depth data
};

enum										//add DPX format support, zz
{
	kVideoFormatNull = 0,
	kVideoFormatNTSC,
	kVideoFormatPAL,
	kVideoFormat720p,
	kVideoFormat1080i,
	kVideoFormat1080p,
	kVideoFormat2K,
	kVideoFormat4k,
	kVideoFormat8k,
	kVideoFormat12k,
	kVideoFormat16k,
	kVideoFormat24k,
	kVideoFormat32k,
	kVideoFormat48k,
	kVideoFormat64k
};

enum										//add DPX format support, zz
{
	kVideoFPS_Null = 0,
	kVideoFPS_23_976HZ,		//Film on Video
	kVideoFPS_24HZ,			//Film
	kVideoFPS_25HZ,			//PAL
	kVideoFPS_29_97HZ,		//NTSC
	kVideoFPS_30HZ,			
	kVideoFPS_59_94HZ,		//1080i, 60i, 60
	kVideoFPS_60HZ,
	kVideoFPS_72HZ,			//experimental format
	kVideoFPS_119_88HZ,
	kVideoFPS_120HZ
};

enum
{
	kVideoFieldNull = 0,
	kVideoFieldOdd,
	kVideoFieldEven
};

enum
{
	kVideoStereoNull,
	kVideoStereoLeft,
	kVideoStereoRight,
	kVideoStereoSequential
};

enum
{
	kColorSpaceNull = 0,
	kColorSpaceRGB_8,			//bits per channel
	kColorSpaceRGB_10,
	kColorSpaceRGB_12,
	kColorSpaceRGB_16,
	kColorSpaceRGB_24,
	kColorSpaceRGB_32,	
	kColorSpaceRGBA_8,			//bits per channel
	kColorSpaceRGBA_10,
	kColorSpaceRGBA_12,
	kColorSpaceRGBA_16,
	kColorSpaceRGBA_24,
	kColorSpaceRGBA_32,
	kColorSpaceYUV_422_8,		//bits per channel
	kColorSpaceYUV_422_10,
	kColorSpaceYUV_422_12,
	kColorSpaceYUVA_422_8,		//bits per channel
	kColorSpaceYUVA_422_10,
	kColorSpaceYUVA_422_12,
	kColorSpaceYUV_444_8,		//bits per channel
	kColorSpaceYUV_444_10,
	kColorSpaceYUV_444_12,
	kColorSpaceYUV_444_16,
	kColorSpaceYUVA_444_8,		//bits per channel
	kColorSpaceYUVA_444_10,
	kColorSpaceYUVA_444_12,
	kColorSpaceYUVA_444_16,
	kColorSpaceCMYK_8,			//bits per channel
	kColorSpaceCMYK_16,
};

enum
{
	kNPalphaModeNull = 0,
	kNPalphaModeSubtractive,	//default alphaMode
	kNPalphaModeAdditive,
	kNPalphaModeDark,
	kNPalphaModeCount
};

enum {
	kAverageNull = 0,
	kAverage,
	kAverageRootMeanSquare,
	kAverageSmoothed,
	kAverageRunning
};

enum {
	kLeftJustify = 0,
	kRightJustify,
	kCenterJustify
};

enum {
	kSmallFont = 0,
	kMediumFont,
	kLargeFont
};

enum
{
	kChannelNull = 0,
	kChannelCycleCount,
	kChannelOscilloscope
};

enum{
	kNPfileDialogNULL = 0,

	kNPfileDialogNew,
	kNPfileDialogOpen,

	kNPfileDialogSave,
	kNPfileDialogSaveAs,

	kNPfileDialogImport,
	kNPfileDialogExport,

	kNPfileDialogClose
};

enum{
	kNPmapNodeNull = 0,
	kNPmapNodeCSVvOne,
	kNPmapNodeCSV,
	kNPmapNodeKML,
	kNPmapNodeSQL
};


// 120Hz timer, 60, 30, 24
// 119.88Hz timer, 59.94, 29.97, 23.976


//---- Ctrl Command Constants ----

enum {
	//global ctrl commands
	kCtrlCommandNull = 0,

	kNPfileNew,
	kNPfileOpen,
	kNPfileClose,
	kNPfileSave,
	kNPfileSaveAs,
	kNPfileImport,
	kNPfileExport,

	kNPfileMapOne,		//used for quick loading and saving of state files
	kNPfileMapTwo,
	kNPfileMapThree,

	//global graph commands
	kNPcmdMenu = 4242,
	kNPcmdTagType,
	
	kNPcmdNew,
	kNPcmdDelete,

	kNPcmdNext,
	kNPcmdPrevious,
	kNPcmdNextBranch,
	kNPcmdPreviousBranch,

	kNPcmdCamera,
	kNPcmdGrid,
	kNPcmdLight,
	kNPcmdPin,
	kNPcmdHUD,

	kNPcmdSelect,
	kNPcmdSelectOff,
	kNPcmdSelectToggle,

	kNPcmdSelectAll,
	kNPcmdSelectNone,
	kNPcmdSelectAllInvert,

	kNPcmdSelectOne,
	kNPcmdSelectTwo,
	kNPcmdSelectThree,
	kNPcmdSelectSetNone,

	kNPcmdPresetOne,
	kNPcmdPresetTwo,
	kNPcmdPresetThree,
	kNPcmdPresetFour,
	kNPcmdPresetFive,
	kNPcmdPresetSix,
	kNPcmdPresetSeven,
	kNPcmdPresetEight,

	//translate, rotate and scale selected objects
	kNPcmdSelectAxes,
	
	kNPcmdXincrease,
	kNPcmdXincreaseOff,
	kNPcmdXdecrease,
	kNPcmdXdecreaseOff,
	
	kNPcmdYincrease,
	kNPcmdYincreaseOff,
	kNPcmdYdecrease,
	kNPcmdYdecreaseOff,
	
	kNPcmdZincrease,
	kNPcmdZincreaseOff,
	kNPcmdZdecrease,
	kNPcmdZdecreaseOff,
	
	kNPcmdRotateLeft,
	kNPcmdRotateLeftOff,
	kNPcmdRotateRight,
	kNPcmdRotateRightOff,

	kNPcmdRotateUp,
	kNPcmdRotateUpOff,
	kNPcmdRotateDown,
	kNPcmdRotateDownOff,

	kNPcmdRotateCCW,
	kNPcmdRotateCCWOff,
	kNPcmdRotateCW,
	kNPcmdRotateCWOff,

	kNPcmdZoomOn,
	kNPcmdZoomOff,
	
	kNPcmdClearFrameBuffer,
	kNPcmdBackground,
	
	//node specific graph commands
	kNPcmdSampleInterval,
	kNPcmdChannelDown,
	kNPcmdChannelUp,
	kNPcmdPause,

	kNPcmdNormalMode,
	kNPcmdShaderMode,
	kNPcmdAlphaMode,

	kNPcmdColorDown,
	kNPcmdColorFade,
	kNPcmdAltColor,
	kNPcmdColorUp,
	kNPcmdColorPallete,
	
	kNPcmdAlphaUp,
	kNPcmdAlphaDown,
	kNPcmdBiasUp,
	kNPcmdBiasDown,
	kNPcmdGainUp,
	kNPcmdGainDown,

	kNPcmdTexture,

	kNPcmdFreeze,
	kNPcmdHide,
	kNPcmdClear,
	
	kNPcmdCenter,
	kNPcmdScroll,

	kNPcmdPoints,	
	kNPcmdLines,
	kNPcmdSegments,
	kNPcmdShader,
	kNPcmdPrimitive,
	kNPcmdTopo,
	kNPcmdMesh,

	kNPcmdSetpointLo,											// add kNPcmd..,  debug zz
	kNPcmdSetpointHi,

	kNPcmdFullscreen,

	kNPcmdRatio,

	kNPcmdCount
};



enum {	kKeyDown = 1,
		kKeyRepeat,
		kKeyUp,
		kKeyModifier
};

enum {	kGlutKeyDown = 1,
		kGlutKeyUp,
		kGlutKeyDownSpecial,
		kGlutKeyUpSpecial
};

enum {		kKeyCodeA = 0x00,
				kKeyCodeS,
				kKeyCodeD,
				kKeyCodeF,
				kKeyCodeH,
				kKeyCodeG,
				kKeyCodeZ,
				kKeyCodeX,
				kKeyCodeC,
				kKeyCodeV = 0x09,
				kKeyCodeB = 0x0b,
				kKeyCodeQ,
				kKeyCodeW,
				kKeyCodeE,
				kKeyCodeR,
				kKeyCodeY,
				kKeyCodeT,
				kKeyCode1,
				kKeyCode2,
				kKeyCode3,
				kKeyCode4,
				kKeyCode6,
				kKeyCode5,
				kKeyCodeEqual,
				kKeyCode9,
				kKeyCode7,
				kKeyCodeMinus,
				kKeyCode8,
				kKeyCodeZero,
				kKeyCodeRightBracket,
				kKeyCodeO,
				kKeyCodeU,
				kKeyCodeLeftBracket,
				kKeyCodeI,
				kKeyCodeP,
				kKeyCodeReturn,
				kKeyCodeL,
				kKeyCodeJ,
				kKeyCodeRightQuote,
				kKeyCodeK,
				kKeyCodeSemiColon,
				kKeyCodeBackSlash,
				kKeyCodeComma,
				kKeyCodeSlash,
				kKeyCodeN,
				kKeyCodeM,
				kKeyCodePeriod,
				kKeyCodeTab,
				kKeyCodeSpace,
				kKeyCodeTilda,
				kKeyCodeDelete = 0x33,
				kKeyCodeBackSpace,
				kKeyCodeESC = 0x35,
				
				kKeyCodeF1 = 0x7A,
				kKeyCodeF2 = 0x78,
				kKeyCodeF3 = 0x63,
				kKeyCodeF4 = 0x76,
				kKeyCodeF5 = 0x60,
				kKeyCodeF6 = 0x61,
				kKeyCodeF7 = 0x62,
				kKeyCodeF8 = 0x64,
				kKeyCodeF13 = 0x69,
				kKeyCodeF16 = 0x6A,
				
				kKeyCodeHelp = 0x72,
				kKeyCodeHome = 0x73,
				kKeyCodePageUp = 0x74,
				kKeyCodeDeleteRight = 0x75,
				kKeyCodeEnd = 0x77,
				kKeyCodePageDown = 0x79,
				
				kKeyCodeLeft = 0x7B,
				kKeyCodeRight = 0x7C, 
				kKeyCodeDown = 0x7D,
				kKeyCodeUp = 0x7E,
				
				kKeyCodeNumPadPeriod = 0x41,
				kKeyCodeNumPadStar = 0x43,
				kKeyCodeNumPadPlus = 0x45,
				kKeyCodeNumPadClear = 0x47, 
				kKeyCodeNumPadEnter = 0x4C,
				kKeyCodeNumPadSlash = 0x4B,
				kKeyCodeNumPadMinus = 0x4E,
				kKeyCodeNumPadEqual = 0x51,
				kKeyCodeNumPadZero = 0x52,
				kKeyCodeNumPad1 = 0x53,
				kKeyCodeNumPad2 = 0x54,
				kKeyCodeNumPad3 = 0x55,
				kKeyCodeNumPad4 = 0x56,
				kKeyCodeNumPad5 = 0x57,
				kKeyCodeNumPad6 = 0x58,
				kKeyCodeNumPad7 = 0x59,
				kKeyCodeNumPad8 = 0x5B,
				kKeyCodeNumPad9 = 0x5C
};


// OpenGL types as standard  ---------------------------------------------------------

enum {
	kNPTypeNull = 0,

	// data structures
	kNPbase,
	kNPnode,
	kNPcmd,
	kNPnodeData,
	kNPcamera,
	kNPgrid,
	kNPpin,

	// fundamental C types
	kNPbool,
	kNPboolean,
	kNPubyte,
	kNPbyte,
	kNPuint,
	kNPint,
	kNPfloat,
	kNPvoidPtr,

	// C structures
	kNPboolXYZ,
	kNPboolXYZS,
	kNPintXYZ,
	kNPintXYZS,

	kNPfloatXY,
	kNPfloatXYZ,
	kNPfloatXYZA,
	kNPfloatXYZS,

	kNPubyteRGB,
	kNPubyteRGBA,

	kNPfloatRGB,
	kNPfloatRGBA,
	
	// GL types
	kNPGLboolean,
	kNPGLubyte,
	kNPGLint,
	kNPGLfloat,

	// node types
	kNPid,
	kNPtype,
	kNPdata,

	kNPselected,
	kNPparent,
	kNPbranchLevel,
	kNPchild,

	kNPchildIndex,
	kNPchildCount,

	kNPchInputID,
	kNPchOutputID,
	kNPchLastUpdated,

	kNPaverage,
	kNPinterval,

	kNPauxA,
	kNPauxB,

	kNPcolorShift,

	kNProtateVec,
	kNPscale,
	kNPtranslate,
	kNPtagOffset,

	kNProtateRate,
	kNProtate,
	kNPscaleRate,
	kNPtranslateRate,
	kNPtranslateVec,

	kNPshader,
	kNPgeometry,
	
	kNPlineWidth,
	kNPpointSize,
	kNPratio,
	
	kNPcolorIndex,
	kNPcolor,
	kNPcolorFade,
	kNPtextureID,
	
	kNPhide,
	kNPfreeze,
	
	kNPtopo,
	kNPfacet,

	kNPautoZoom,

	kNPtriggerHi,
	kNPtriggerLo,
	kNPsetHi,
	kNPsetLo,
	
	kNPproximity,
	kNPproximityMode,

	kNPsegments,

	kNPtagMode,
	kNPformatID,
	kNPtableID,
	kNPrecordID,

	// camera
	kNPformat,
	kNPinterlaced,
	kNPstereo,
	kNPaspectRatio,
	kNPfps,
	kNPcolorSpace,
	kNPwidth,
	kNPheight,
	kNPfov,
	kNPclipNear,
	kNPclipFar,
	kNPaperture,
	kNPexposure,
	kNPsensorType,
	kNPoverlay,

	// pin
	kNPinnerRadius,
	kNPouterRadius,
	kNPradiusRatioIndex,
	kNPscreenSizeIndex,
	kNPslices,
	kNPstacks,
	kNPtranslateTex,
	kNPscaleTex,
	kNProtateTex,

	// grid
	// 

	// map file
	kNPversion,
	kNPtableCount,
	kNPrecordCount,
	kNPnodeRootCount,

	// launch external app
	kNPopenURL,
	kNPopenApp,
	kNPopenAntz,

	kNPsize
};

enum
{
	// GL primitives 
	// GLUT
	kNPglutWireCube = 0,
	kNPglutSolidCube,
	kNPglutWireSphere,
	kNPglutSolidSphere,
	kNPglutWireCone,
	kNPglutSolidCone,

	kNPglutWireTorus,
	kNPglutSolidTorus,

	kNPglutWireDodecahedron,
	kNPglutSolidDodecahedron,
	kNPglutWireOctahedron,
	kNPglutSolidOctahedron,
	kNPglutWireTetrahedron,
	kNPglutSolidTetrahedron,
	kNPglutWireIcosahedron,
	kNPglutSolidIcosahedron,

	kNPprimitiveSolidPin,		// solid and wire reversed, debug zz
	kNPprimitiveWirePin,

	kNPglutWireTeapot,
	kNPglutSolidTeapot,

	kNPprimitiveCount	//add primitives, vertex, triangle, quad, circle, cylinder
};

enum
{	
	kNPglNormalNull = 0,
	kNPglRescaleNormal,
	kNPglNormalize,

	kNPglNormalSize
};

enum
{	
	kNPglShadeNull = 0,
	kNPglShadeSmooth,
	kNPglShadeFlat,

	kNPglShadeSize
};

enum
{
	kNPmouseModeNull = 0,

	kNPmouseModeCamLook,

	kNPmouseModeCamFlyA,
	kNPmouseModeCamFlyB,

	kNPmouseModeCamExamXZ,
	kNPmouseModeCamExamXY,
	
	kNPmouseModeDragXZ,
	kNPmouseModeDragXY,
	kNPmouseModeDragWorld,

	kNPdragModeSize
};

enum
{
	kNPcenterNull = 0,
	kNPcenterPerCycle,		//center on the current data point
	kNPcenterPerGrid		//center when current point exceeds 1 grid space
};

enum
{
	kNPnodeRootNull = 0,
	kNPnodeRootCamera,			// root node index for all cameras
	kNPnodeRootGrid,			// root node index for all grids
	kNPnodeRootPin				// first root node for all pins
};

enum
{
	kNPpickModeNull = 0,
	kNPpickModeCamera,			// pick cameras
	kNPpickModeGrid,			// pick grids
	kNPpickModePin,				// pick pins
	kNPpickModeAll,				// pick anything
	kNPpickModeCount
};

enum
{
	kNPtoolNull = 0,		// default behavior based on pickMode
	kNPtoolTranslate,		// pick cameras
	kNPtoolRotate,			// pick grids
	kNPtoolScale,			// pick pins
	kNPtoolInfo,			// pick anything
	kNPtoolHide,			// hide and unhide child nodes
	kNPtoolFreeze,			// freeze node and child nodes
	kNPtoolColor,			// change color and transparency
	kNPtoolCount
};

enum
{
	kNPtopoNull = 0,		//topology determined by node type
	kNPtopoCube,
	kNPtopoSphere,
	kNPtopoTorus,
	kNPtopoSurface,			//need to create type, fix in ctrlCmd debug zz
	kNPtopoPin,
	kNPtopoGrid,

	kNPtopoCount
};

enum
{
	kNPtableNull = 0,
	kNPtableGlobal,
	kNPtableCamera,
	kNPtableGrid,
	kNPtableNode,
	kNPtableChannels,
	kNPtableChannelMap,
	kNPtableTag,
	kNPtableFormat,
	kNPtableMap,
};

enum
{
	kNPmsgNull = 0,
	kNPmsgErr,
	kNPmsgWarn,
	kNPmsgDebug,
	kNPmsgCtrl,
	kNPmsgFile,
	kNPmsgNPE,
	kNPmsgGL
};

enum
{
	kNPconsoleModeNull = 0,		//hide console
	kNPconsoleModeOne,			//display 1 line
	kNPconsoleModeThree,		//display 3 lines
	kNPconsoleModeMax,			//display maximum number of lines possible
	kNPconsoleModeCount
};

enum
{
	kNP_GLUT_STROKE_ROMAN = 0,
	kNP_GLUT_STROKE_MONO_ROMAN,
//	kNP_GLUT_BITMAP_9_BY_15, //debug zz GLUT_BITMAP_9_BY_15 = ((void *)0x0002)
	kNP_GLUT_BITMAP_8_BY_13,
	kNP_GLUT_BITMAP_TIMES_ROMAN_10,
	kNP_GLUT_BITMAP_TIMES_ROMAN_24,
	kNP_GLUT_BITMAP_HELVETICA_10,
	kNP_GLUT_BITMAP_HELVETICA_12,
	kNP_GLUT_BITMAP_HELVETICA_18,
	kNPfontCount
};
# define  kNP_GLUT_BITMAP_9_BY_15      ((void *)0x0002)		//zz debug, workaround

enum
{
	kNPjustifyNull = 0,
	kNPjustifyLeft,
	kNPjustifyRight,
	kNPjustifyCenter,
	kNPjustifyCount
};

enum
{
	kNPpositionNull = 0,
	kNPpositionLeft,
	kNPpositionRight,
	kNPpositionCenter,		//may be used to specify vertical or horizontal
	kNPpositionTop,
	kNPpositionBottom,
	
	kNPpositionCount
};

enum
{
	kNPtagModeNull = 0,			//fixed to object in 3D scene, not oriented
	kNPtagModeBoxOutlineHUD,	//white text on grey box, node colored outline
	kNPtagModeBoxHUD,			//node colored text on grey box, no outline
//	kNPtagModeHUD,				//fixed size font displayed on the HUD, removed as is typically too hard to read, debug zz
//	kNPtagModeBillboard,		//text in 3D scene oriented towards cam
	kNPtagModeCount
};


#endif


/* -----------------------------------------------------------------------------
*
*  npkey.c
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

#include "npkey.h"

#include "../npdata.h"

#ifdef NP_MSW_
	#include <freeglut.h>
#endif
#ifdef NP_OSX_
	#include <GLUT/glut.h>
	#include <OpenGL/gl.h>
#endif
#ifdef NP_LINUX_
	#include <GL/freeglut.h>
#endif

#include "../npctrl.h"


void npKeyEvent (int key, int keyEventType);

//------------------------------------------------------------------------------
void npInitKey (void* dataRef)
{
	int i = 0;
	int j = 0;

	pData data = (pData) dataRef;
	pNPkey key = &data->io.key;

	key->modShift = 0;
	key->modCtrl = 0;
	key->modAlt = 0;
	key->modCommand = 0;

	for (i = 0; i < kNPkeyEventTypeSize; i++)
		for (j = 0; j < kNPkeyMapSize; j++)
			key->map[i][j] = 0;


	//the key map defines which command to execute for each key event
	//three types of fundamental keys, toggle, momentary, repeating
	//four event types generated, keyDown, KeyRepeat, KeyUp, KeyModifier

	//Fire keys, respond to keyDown only
	key->map[kKeyDown][kKeyCodeM]				= kNPcmdMenu;

	key->map[kKeyDown][kKeyCodeY]				= kNPcmdSegments;
	key->map[kKeyDown][kKeyCodeT]				= kNPcmdTexture;

	key->map[kKeyDown][kKeyCodeESC]				= kNPcmdFullscreen;
//	key->map[kKeyDown][kKeyCodeB]				= kNPcmdClearFrameBuffer;
	key->map[kKeyDown][kKeyCodeB]				= kNPcmdBackground;
	key->map[kKeyDown][kKeyCodeX]				= kNPcmdSelectAxes;

	key->map[kKeyDown][kKeyCodeC]				= kNPcmdCamera;
	key->map[kKeyDown][kKeyCodeG]				= kNPcmdGrid;

	key->map[kKeyDown][kKeyCode4]				= kNPcmdSelectAll;
	key->map[kKeyDown][kKeyCodeTilda]			= kNPcmdSelectNone;
	key->map[kKeyDown][kKeyCodeSlash]			= kNPcmdSelectToggle;
	key->map[kKeyDown][kKeyCode1]				= kNPfileMapOne;
	key->map[kKeyDown][kKeyCode2]				= kNPfileMapTwo;
	key->map[kKeyDown][kKeyCode3]				= kNPfileMapThree;

	key->map[kKeyDown][kKeyCode5]				= kNPcmdPresetOne;
	key->map[kKeyDown][kKeyCode6]				= kNPcmdPresetTwo;
	key->map[kKeyDown][kKeyCode7]				= kNPcmdPresetThree;
//	key->map[kKeyDown][kKeyCode8]				= kNPcmdPresetFour;

//	key->map[kKeyDown][kKeyCodeC]				= kNPcmdCenter;
	key->map[kKeyDown][kKeyCodeBackSlash]		= kNPcmdScroll;
	key->map[kKeyDown][kKeyCodeF]				= kNPcmdFreeze;
	key->map[kKeyDown][kKeyCodeH]				= kNPcmdHide;

	key->map[kKeyDown][kKeyCodeO]				= kNPcmdPrimitive;
	key->map[kKeyDown][kKeyCodeJ]				= kNPcmdTopo;

	key->map[kKeyDown][kKeyCodeI]				= kNPcmdTagType;
	
	key->map[kKeyDown][kKeyCodeK]				= kNPfileSave;
	key->map[kKeyDown][kKeyCodeL]				= kNPfileOpen;
	key->map[kKeyDown][kKeyCodeP]				= kNPfileImport;	// kNPcmdPoints;

	key->map[kKeyDown][kKeyCodeU]				= kNPopenURL;


//	key->map[kKeyDown][kKeyCodeL]				= kNPcmdMesh;
	

	key->map[kKeyDown][kKeyCodeLeftBracket]		= kNPcmdSetpointLo;
	key->map[kKeyDown][kKeyCodeRightBracket]	= kNPcmdSetpointHi;

	key->map[kKeyDown][kKeyCodeSpace]			= kNPcmdPause;

	//Repeat keys, don't usually respond to keyUp
	key->map[kKeyDown][kKeyCodeN]				= kNPcmdNew;
	key->map[kKeyRepeat][kKeyCodeN]				= kNPcmdNew;
	
	key->map[kKeyDown][kKeyCodeDelete]			= kNPcmdDelete;
	key->map[kKeyRepeat][kKeyCodeDelete]		= kNPcmdDelete;

	key->map[kKeyDown][kKeyCodeBackSpace]		= kNPcmdDelete;
	key->map[kKeyRepeat][kKeyCodeBackSpace]		= kNPcmdDelete;

	key->map[kKeyDown][kKeyCodeTab]				= kNPcmdPin;
	key->map[kKeyRepeat][kKeyCodeTab]			= kNPcmdPin;

	key->map[kKeyDown][kKeyCodeReturn]			= kNPcmdNextBranch;
	key->map[kKeyRepeat][kKeyCodeReturn]		= kNPcmdNextBranch;
	key->map[kKeyDown][kKeyCodeNumPadEnter]		= kNPcmdNextBranch;
	key->map[kKeyRepeat][kKeyCodeNumPadEnter]	= kNPcmdNextBranch;

	key->map[kKeyDown][kKeyCodeComma]			= kNPcmdChannelDown;
	key->map[kKeyRepeat][kKeyCodeComma]			= kNPcmdChannelDown;
	
	key->map[kKeyDown][kKeyCodePeriod]			= kNPcmdChannelUp;
	key->map[kKeyRepeat][kKeyCodePeriod]		= kNPcmdChannelUp;

	key->map[kKeyDown][kKeyCodeMinus]			= kNPcmdColorDown;
	key->map[kKeyRepeat][kKeyCodeMinus]			= kNPcmdColorDown;
	
	key->map[kKeyDown][kKeyCodeEqual]			= kNPcmdColorUp;
	key->map[kKeyRepeat][kKeyCodeEqual]			= kNPcmdColorUp;

	key->map[kKeyDown][kKeyCodeZero]			= kNPcmdAlphaUp;
	key->map[kKeyRepeat][kKeyCodeZero]			= kNPcmdAlphaUp;

	key->map[kKeyDown][kKeyCode9]				= kNPcmdAlphaDown;
	key->map[kKeyRepeat][kKeyCode9]				= kNPcmdAlphaDown;

	key->map[kKeyDown][kKeyCode8]				= kNPcmdAlphaMode;
	key->map[kKeyRepeat][kKeyCode8]				= kNPcmdAlphaMode;

	key->map[kKeyDown][kKeyCodeR]				= kNPcmdRatio; //kNPcmdNormalMode;
	key->map[kKeyRepeat][kKeyCodeR]				= kNPcmdRatio; //kNPcmdNormalMode;

	

//	key->map[kKeyDown][kKeyCode]				= kNPcmdShaderMode;
//	key->map[kKeyRepeat][kKeyCode]				= kNPcmdShaderMode;


//	key->map[kKeyDown][kKeyCode]				= kNPcmdBiasUp;
//	key->map[kKeyRepeat][kKeyCode]				= kNPcmdBiasUp;
//	key->map[kKeyDown][kKeyCode]				= kNPcmdBiasDown;
//	key->map[kKeyRepeat][kKeyCode]				= kNPcmdBiasDown;

//	key->map[kKeyDown][kKeyCode]				= kNPcmdGainUp;
//	key->map[kKeyRepeat][kKeyCode]				= kNPcmdGainUp;
//	key->map[kKeyDown][kKeyCode]				= kNPcmdGainDown;
//	key->map[kKeyRepeat][kKeyCode]				= kNPcmdGainDown;

	//Momentary Keys are active while pressed and disable upon keyup
	//Arrow keys for X,Y,Z translate, rotate and scale of selected objects

	// translate
	key->map[kKeyDown][kKeyCodeA]				= kNPcmdXdecrease;
	key->map[kKeyUp][kKeyCodeA]					= kNPcmdXdecreaseOff;

	key->map[kKeyDown][kKeyCodeD]				= kNPcmdXincrease;
	key->map[kKeyUp][kKeyCodeD]					= kNPcmdXincreaseOff;

	key->map[kKeyDown][kKeyCodeW]				= kNPcmdYincrease;
	key->map[kKeyUp][kKeyCodeW]					= kNPcmdYincreaseOff;

	key->map[kKeyDown][kKeyCodeS]				= kNPcmdYdecrease;
	key->map[kKeyUp][kKeyCodeS]					= kNPcmdYdecreaseOff;

	key->map[kKeyDown][kKeyCodeE]				= kNPcmdZincrease;
	key->map[kKeyUp][kKeyCodeE]					= kNPcmdZincreaseOff;

	key->map[kKeyDown][kKeyCodeQ]				= kNPcmdZdecrease;
	key->map[kKeyUp][kKeyCodeQ]					= kNPcmdZdecreaseOff;

	// rotate
	key->map[kKeyDown][kKeyCodeLeft]			= kNPcmdRotateLeft;
	key->map[kKeyUp][kKeyCodeLeft]				= kNPcmdRotateLeftOff;

	key->map[kKeyDown][kKeyCodeRight]			= kNPcmdRotateRight;
	key->map[kKeyUp][kKeyCodeRight]				= kNPcmdRotateRightOff;

	key->map[kKeyDown][kKeyCodeUp]				= kNPcmdRotateUp;
	key->map[kKeyUp][kKeyCodeUp]				= kNPcmdRotateUpOff;

	key->map[kKeyDown][kKeyCodeDown]			= kNPcmdRotateDown;
	key->map[kKeyUp][kKeyCodeDown]				= kNPcmdRotateDownOff;

//	key->map[kKeyDown][kKeyCodeLeft]			= kNPcmdRotateCCW;
//	key->map[kKeyUp][kKeyCodeLeft]				= kNPcmdRotateCCWOff;

//	key->map[kKeyDown][kKeyCodeRight]			= kNPcmdRotateCW;
//	key->map[kKeyUp][kKeyCodeRight]				= kNPcmdRotateCWOff;

	// scale
	key->map[kKeyDown][kKeyCodeZ]				= kNPcmdZoomOn;
	key->map[kKeyUp][kKeyCodeZ]					= kNPcmdZoomOff;
}


//------------------------------------------------------------------------------
void npCloseKey (void* dataRef)
{
	return;
}

//------------------------------------------------------------------------------
void npKeyEvent (int key, int keyEventType)
{
	int command = 0;

	pData data = (pData) npGetDataRef();

	command = data->io.key.map[keyEventType][key];

	if (command != 0)
		npCtrlCommand (command, data);
}

//------------------------------------------------------------------------------
// Process key events from GLUT
void npKeyGlut (int key, int x, int y, int keyTypeGlut, int modifiers)
{
	pData data = (pData) npGetDataRef();


	if (modifiers == GLUT_ACTIVE_SHIFT)		// move these GLUT commands to npGlut, debug zz
		data->io.key.modShift = true;
	else
		data->io.key.modShift = false;

	if (modifiers == GLUT_ACTIVE_CTRL)
		data->io.key.modCtrl = true;
	else
		data->io.key.modCtrl = false;

	if (modifiers == GLUT_ACTIVE_ALT)
		data->io.key.modAlt = true;
	else
		data->io.key.modAlt = false;


	switch (keyTypeGlut) 
	{
		case kGlutKeyDown:					// ASCII key codes converted
			switch (key) 
			{
				case 8  : npKeyEvent (kKeyCodeBackSpace, kKeyDown); break;
				case 9  : npKeyEvent (kKeyCodeTab, kKeyDown); break;
				case 13 : npKeyEvent (kKeyCodeReturn, kKeyDown); break;
				case 27 : npKeyEvent (kKeyCodeESC, kKeyDown); break;
				case 32 : npKeyEvent (kKeyCodeSpace, kKeyDown); break;
				case 33 : npKeyEvent (kKeyCode1, kKeyDown); break;
				case 34 : npKeyEvent (kKeyCodeRightQuote, kKeyDown); break;  // double "
				case 35 : npKeyEvent (kKeyCode3, kKeyDown); break;
				case 36 : npKeyEvent (kKeyCode4, kKeyDown); break;
				case 37 : npKeyEvent (kKeyCode5, kKeyDown); break;
				case 38 : npKeyEvent (kKeyCode7, kKeyDown); break;
				case 39 : npKeyEvent (kKeyCodeRightQuote, kKeyDown); break;  // single '
				case 40 : npKeyEvent (kKeyCode9, kKeyDown); break;
				case 41 : npKeyEvent (kKeyCodeZero, kKeyDown); break;
				case 42 : npKeyEvent (kKeyCode8, kKeyDown); break;
				case 43 : npKeyEvent (kKeyCodeEqual, kKeyDown); break;
				case 44 : npKeyEvent (kKeyCodeComma, kKeyDown); break;
				case 45 : npKeyEvent (kKeyCodeMinus, kKeyDown); break;
				case 46 : npKeyEvent (kKeyCodePeriod, kKeyDown); break;	
				case 47 : npKeyEvent (kKeyCodeSlash, kKeyDown); break;
				case 48 : npKeyEvent (kKeyCodeZero, kKeyDown); break;
				case 49 : npKeyEvent (kKeyCode1, kKeyDown); break;
				case 50 : npKeyEvent (kKeyCode2, kKeyDown); break;
				case 51 : npKeyEvent (kKeyCode3, kKeyDown); break;
				case 52 : npKeyEvent (kKeyCode4, kKeyDown); break;
				case 53 : npKeyEvent (kKeyCode5, kKeyDown); break;
				case 54 : npKeyEvent (kKeyCode6, kKeyDown); break;
				case 55 : npKeyEvent (kKeyCode7, kKeyDown); break;
				case 56 : npKeyEvent (kKeyCode8, kKeyDown); break;
				case 57 : npKeyEvent (kKeyCode9, kKeyDown); break;
				case 58 : npKeyEvent (kKeyCodeSemiColon, kKeyDown); break;
				case 59 : npKeyEvent (kKeyCodeSemiColon, kKeyDown); break;
				case 60 : npKeyEvent (kKeyCodeComma, kKeyDown); break;
				case 61 : npKeyEvent (kKeyCodeEqual, kKeyDown); break;
				case 62 : npKeyEvent (kKeyCodePeriod, kKeyDown); break;	
				case 63 : npKeyEvent (kKeyCodeSlash, kKeyDown); break;
				case 64 : npKeyEvent (kKeyCode2, kKeyDown); break;
				case 65 : npKeyEvent (kKeyCodeA, kKeyDown); break;
				case 66 : npKeyEvent (kKeyCodeB, kKeyDown); break;
				case 67 : npKeyEvent (kKeyCodeC, kKeyDown); break;
				case 68 : npKeyEvent (kKeyCodeD, kKeyDown); break;
				case 69 : npKeyEvent (kKeyCodeE, kKeyDown); break;
				case 70 : npKeyEvent (kKeyCodeF, kKeyDown); break;
				case 71 : npKeyEvent (kKeyCodeG, kKeyDown); break;
				case 72 : npKeyEvent (kKeyCodeH, kKeyDown); break;
				case 73 : npKeyEvent (kKeyCodeI, kKeyDown); break;
				case 74 : npKeyEvent (kKeyCodeJ, kKeyDown); break;
				case 75 : npKeyEvent (kKeyCodeK, kKeyDown); break;
				case 76 : npKeyEvent (kKeyCodeL, kKeyDown); break;
				case 77 : npKeyEvent (kKeyCodeM, kKeyDown); break;
				case 78 : npKeyEvent (kKeyCodeN, kKeyDown); break;
				case 79 : npKeyEvent (kKeyCodeO, kKeyDown); break;
				case 80 : npKeyEvent (kKeyCodeP, kKeyDown); break;
				case 81 : npKeyEvent (kKeyCodeQ, kKeyDown); break;
				case 82 : npKeyEvent (kKeyCodeR, kKeyDown); break;
				case 83 : npKeyEvent (kKeyCodeS, kKeyDown); break;
				case 84 : npKeyEvent (kKeyCodeT, kKeyDown); break;
				case 85 : npKeyEvent (kKeyCodeU, kKeyDown); break;
				case 86 : npKeyEvent (kKeyCodeV, kKeyDown); break;
				case 87 : npKeyEvent (kKeyCodeW, kKeyDown); break;
				case 88 : npKeyEvent (kKeyCodeX, kKeyDown); break;
				case 89 : npKeyEvent (kKeyCodeY, kKeyDown); break;
				case 90 : npKeyEvent (kKeyCodeZ, kKeyDown); break;
				case 91 : npKeyEvent (kKeyCodeLeftBracket, kKeyDown); break;
				case 92 : npKeyEvent (kKeyCodeBackSlash, kKeyDown); break;
				case 93 : npKeyEvent (kKeyCodeRightBracket, kKeyDown); break;
				case 94 : npKeyEvent (kKeyCode6, kKeyDown); break;	
				case 95 : npKeyEvent (kKeyCodeMinus, kKeyDown); break;
				case 96 : npKeyEvent (kKeyCodeTilda, kKeyDown); break;
				case 97 : npKeyEvent (kKeyCodeA, kKeyDown); break;
				case 98 : npKeyEvent (kKeyCodeB, kKeyDown); break;
				case 99 : npKeyEvent (kKeyCodeC, kKeyDown); break;
				case 100 : npKeyEvent (kKeyCodeD, kKeyDown); break;
				case 101 : npKeyEvent (kKeyCodeE, kKeyDown); break;
				case 102 : npKeyEvent (kKeyCodeF, kKeyDown); break;
				case 103 : npKeyEvent (kKeyCodeG, kKeyDown); break;
				case 104 : npKeyEvent (kKeyCodeH, kKeyDown); break;
				case 105 : npKeyEvent (kKeyCodeI, kKeyDown); break;
				case 106 : npKeyEvent (kKeyCodeJ, kKeyDown); break;
				case 107 : npKeyEvent (kKeyCodeK, kKeyDown); break;
				case 108 : npKeyEvent (kKeyCodeL, kKeyDown); break;
				case 109 : npKeyEvent (kKeyCodeM, kKeyDown); break;
				case 110 : npKeyEvent (kKeyCodeN, kKeyDown); break;
				case 111 : npKeyEvent (kKeyCodeO, kKeyDown); break;
				case 112 : npKeyEvent (kKeyCodeP, kKeyDown); break;
				case 113 : npKeyEvent (kKeyCodeQ, kKeyDown); break;
				case 114 : npKeyEvent (kKeyCodeR, kKeyDown); break;
				case 115 : npKeyEvent (kKeyCodeS, kKeyDown); break;
				case 116 : npKeyEvent (kKeyCodeT, kKeyDown); break;
				case 117 : npKeyEvent (kKeyCodeU, kKeyDown); break;
				case 118 : npKeyEvent (kKeyCodeV, kKeyDown); break;
				case 119 : npKeyEvent (kKeyCodeW, kKeyDown); break;
				case 120 : npKeyEvent (kKeyCodeX, kKeyDown); break;
				case 121 : npKeyEvent (kKeyCodeY, kKeyDown); break;
				case 122 : npKeyEvent (kKeyCodeZ, kKeyDown); break;
				case 123 : npKeyEvent (kKeyCodeLeftBracket, kKeyDown); break;
				case 124 : npKeyEvent (kKeyCodeBackSlash, kKeyDown); break;
				case 125 : npKeyEvent (kKeyCodeRightBracket, kKeyDown); break;
				case 126 : npKeyEvent (kKeyCodeTilda, kKeyDown); break;
				case 127 : npKeyEvent (kKeyCodeDelete, kKeyDown); break;
				default : break;
			}
			break;
		case kGlutKeyUp:					// ASCII key codes converted
			switch (key) 
			{
				case 8  : npKeyEvent (kKeyCodeBackSpace, kKeyUp); break;
				case 9  : npKeyEvent (kKeyCodeTab, kKeyUp); break;
				case 13 : npKeyEvent (kKeyCodeReturn, kKeyUp); break;
				case 27 : npKeyEvent (kKeyCodeESC, kKeyUp); break;
				case 32 : npKeyEvent (kKeyCodeSpace, kKeyUp); break;
				case 33 : npKeyEvent (kKeyCode1, kKeyUp); break;
				case 34 : npKeyEvent (kKeyCodeRightQuote, kKeyUp); break;
				case 35 : npKeyEvent (kKeyCode3, kKeyUp); break;
				case 36 : npKeyEvent (kKeyCode4, kKeyUp); break;
				case 37 : npKeyEvent (kKeyCode5, kKeyUp); break;
				case 38 : npKeyEvent (kKeyCode7, kKeyUp); break;
				case 39 : npKeyEvent (kKeyCodeRightQuote, kKeyUp); break;
				case 40 : npKeyEvent (kKeyCode9, kKeyUp); break;
				case 41 : npKeyEvent (kKeyCodeZero, kKeyUp); break;
				case 42 : npKeyEvent (kKeyCode8, kKeyUp); break;
				case 43 : npKeyEvent (kKeyCodeEqual, kKeyUp); break;
				case 44 : npKeyEvent (kKeyCodeComma, kKeyUp); break;
				case 45 : npKeyEvent (kKeyCodeMinus, kKeyUp); break;
				case 46 : npKeyEvent (kKeyCodePeriod, kKeyUp); break;	
				case 47 : npKeyEvent (kKeyCodeSlash, kKeyUp); break;
				case 48 : npKeyEvent (kKeyCodeZero, kKeyUp); break;
				case 49 : npKeyEvent (kKeyCode1, kKeyUp); break;
				case 50 : npKeyEvent (kKeyCode2, kKeyUp); break;
				case 51 : npKeyEvent (kKeyCode3, kKeyUp); break;
				case 52 : npKeyEvent (kKeyCode4, kKeyUp); break;
				case 53 : npKeyEvent (kKeyCode5, kKeyUp); break;
				case 54 : npKeyEvent (kKeyCode6, kKeyUp); break;
				case 55 : npKeyEvent (kKeyCode7, kKeyUp); break;
				case 56 : npKeyEvent (kKeyCode8, kKeyUp); break;
				case 57 : npKeyEvent (kKeyCode9, kKeyUp); break;
				case 58 : npKeyEvent (kKeyCodeSemiColon, kKeyUp); break;
				case 59 : npKeyEvent (kKeyCodeSemiColon, kKeyUp); break;
				case 60 : npKeyEvent (kKeyCodeComma, kKeyUp); break;
				case 61 : npKeyEvent (kKeyCodeEqual, kKeyUp); break;
				case 62 : npKeyEvent (kKeyCodePeriod, kKeyUp); break;	
				case 63 : npKeyEvent (kKeyCodeSlash, kKeyUp); break;
				case 64 : npKeyEvent (kKeyCode2, kKeyUp); break;
				case 65 : npKeyEvent (kKeyCodeA, kKeyUp); break;
				case 66 : npKeyEvent (kKeyCodeB, kKeyUp); break;
				case 67 : npKeyEvent (kKeyCodeC, kKeyUp); break;
				case 68 : npKeyEvent (kKeyCodeD, kKeyUp); break;
				case 69 : npKeyEvent (kKeyCodeE, kKeyUp); break;
				case 70 : npKeyEvent (kKeyCodeF, kKeyUp); break;
				case 71 : npKeyEvent (kKeyCodeG, kKeyUp); break;
				case 72 : npKeyEvent (kKeyCodeH, kKeyUp); break;
				case 73 : npKeyEvent (kKeyCodeI, kKeyUp); break;
				case 74 : npKeyEvent (kKeyCodeJ, kKeyUp); break;
				case 75 : npKeyEvent (kKeyCodeK, kKeyUp); break;
				case 76 : npKeyEvent (kKeyCodeL, kKeyUp); break;
				case 77 : npKeyEvent (kKeyCodeM, kKeyUp); break;
				case 78 : npKeyEvent (kKeyCodeN, kKeyUp); break;
				case 79 : npKeyEvent (kKeyCodeO, kKeyUp); break;
				case 80 : npKeyEvent (kKeyCodeP, kKeyUp); break;
				case 81 : npKeyEvent (kKeyCodeQ, kKeyUp); break;
				case 82 : npKeyEvent (kKeyCodeR, kKeyUp); break;
				case 83 : npKeyEvent (kKeyCodeS, kKeyUp); break;
				case 84 : npKeyEvent (kKeyCodeT, kKeyUp); break;
				case 85 : npKeyEvent (kKeyCodeU, kKeyUp); break;
				case 86 : npKeyEvent (kKeyCodeV, kKeyUp); break;
				case 87 : npKeyEvent (kKeyCodeW, kKeyUp); break;
				case 88 : npKeyEvent (kKeyCodeX, kKeyUp); break;
				case 89 : npKeyEvent (kKeyCodeY, kKeyUp); break;
				case 90 : npKeyEvent (kKeyCodeZ, kKeyUp); break;
				case 91 : npKeyEvent (kKeyCodeLeftBracket, kKeyUp); break;
				case 92 : npKeyEvent (kKeyCodeBackSlash, kKeyUp); break;
				case 93 : npKeyEvent (kKeyCodeRightBracket, kKeyUp); break;
				case 94 : npKeyEvent (kKeyCode6, kKeyUp); break;	
				case 95 : npKeyEvent (kKeyCodeMinus, kKeyUp); break;
				case 96 : npKeyEvent (kKeyCodeTilda, kKeyUp); break;
				case 97 : npKeyEvent (kKeyCodeA, kKeyUp); break;
				case 98 : npKeyEvent (kKeyCodeB, kKeyUp); break;
				case 99 : npKeyEvent (kKeyCodeC, kKeyUp); break;
				case 100 : npKeyEvent (kKeyCodeD, kKeyUp); break;
				case 101 : npKeyEvent (kKeyCodeE, kKeyUp); break;
				case 102 : npKeyEvent (kKeyCodeF, kKeyUp); break;
				case 103 : npKeyEvent (kKeyCodeG, kKeyUp); break;
				case 104 : npKeyEvent (kKeyCodeH, kKeyUp); break;
				case 105 : npKeyEvent (kKeyCodeI, kKeyUp); break;
				case 106 : npKeyEvent (kKeyCodeJ, kKeyUp); break;
				case 107 : npKeyEvent (kKeyCodeK, kKeyUp); break;
				case 108 : npKeyEvent (kKeyCodeL, kKeyUp); break;
				case 109 : npKeyEvent (kKeyCodeM, kKeyUp); break;
				case 110 : npKeyEvent (kKeyCodeN, kKeyUp); break;
				case 111 : npKeyEvent (kKeyCodeO, kKeyUp); break;
				case 112 : npKeyEvent (kKeyCodeP, kKeyUp); break;
				case 113 : npKeyEvent (kKeyCodeQ, kKeyUp); break;
				case 114 : npKeyEvent (kKeyCodeR, kKeyUp); break;
				case 115 : npKeyEvent (kKeyCodeS, kKeyUp); break;
				case 116 : npKeyEvent (kKeyCodeT, kKeyUp); break;
				case 117 : npKeyEvent (kKeyCodeU, kKeyUp); break;
				case 118 : npKeyEvent (kKeyCodeV, kKeyUp); break;
				case 119 : npKeyEvent (kKeyCodeW, kKeyUp); break;
				case 120 : npKeyEvent (kKeyCodeX, kKeyUp); break;
				case 121 : npKeyEvent (kKeyCodeY, kKeyUp); break;
				case 122 : npKeyEvent (kKeyCodeZ, kKeyUp); break;
				case 123 : npKeyEvent (kKeyCodeLeftBracket, kKeyUp); break;
				case 124 : npKeyEvent (kKeyCodeBackSlash, kKeyUp); break;
				case 125 : npKeyEvent (kKeyCodeRightBracket, kKeyUp); break;
				case 126 : npKeyEvent (kKeyCodeTilda, kKeyUp); break;
				case 127 : npKeyEvent (kKeyCodeDelete, kKeyUp); break;
				default : break;
			}
			break;
		case kGlutKeyDownSpecial:			// non-ASCII Glut constants
			switch (key) 
			{
				case GLUT_KEY_F1 : npKeyEvent (kKeyCodeF1, kKeyDown); break;
				case GLUT_KEY_F2 : npKeyEvent (kKeyCodeF2, kKeyDown); break;
				case GLUT_KEY_F3 : npKeyEvent (kKeyCodeF3, kKeyDown); break;
				case GLUT_KEY_F4 : npKeyEvent (kKeyCodeF4, kKeyDown); break;
				case GLUT_KEY_F5 : npKeyEvent (kKeyCodeF5, kKeyDown); break;
				case GLUT_KEY_F6 : npKeyEvent (kKeyCodeF6, kKeyDown); break;
				case GLUT_KEY_F7 : npKeyEvent (kKeyCodeF7, kKeyDown); break;
				case GLUT_KEY_F8 : npKeyEvent (kKeyCodeF8, kKeyDown); break;
//				case GLUT_KEY_F9 : npKeyEvent (kKeyCodeF9, kKeyDown); break;
//				case GLUT_KEY_F10 : npKeyEvent (kKeyCodeF10, kKeyDown); break;
//				case GLUT_KEY_F11 : npKeyEvent (kKeyCodeF11, kKeyDown); break;
//				case GLUT_KEY_F12 : npKeyEvent (kKeyCodeF12, kKeyDown); break;
				case GLUT_KEY_LEFT : npKeyEvent (kKeyCodeLeft, kKeyDown); break;
				case GLUT_KEY_RIGHT : npKeyEvent (kKeyCodeRight, kKeyDown); break;
				case GLUT_KEY_UP : npKeyEvent (kKeyCodeUp, kKeyDown); break;
				case GLUT_KEY_DOWN : npKeyEvent (kKeyCodeDown, kKeyDown); break;
				case GLUT_KEY_PAGE_UP : npKeyEvent (kKeyCodePageUp, kKeyDown); break;
				case GLUT_KEY_PAGE_DOWN : npKeyEvent (kKeyCodePageDown, kKeyDown); break;
				case GLUT_KEY_HOME : npKeyEvent (kKeyCodeHome, kKeyDown); break;
				case GLUT_KEY_END : npKeyEvent (kKeyCodeEnd, kKeyDown); break;
//				case GLUT_KEY_INSERT : npKeyEvent (kKeyCode, kKeyDown); break;	// not compatible with Mac
				default : break;
			}
			break;
		case kGlutKeyUpSpecial:				// non-ASCII Glut constants
			switch (key) 
			{
				case GLUT_KEY_F1 : npKeyEvent (kKeyCodeF1, kKeyUp); break;
				case GLUT_KEY_F2 : npKeyEvent (kKeyCodeF2, kKeyUp); break;
				case GLUT_KEY_F3 : npKeyEvent (kKeyCodeF3, kKeyUp); break;
				case GLUT_KEY_F4 : npKeyEvent (kKeyCodeF4, kKeyUp); break;
				case GLUT_KEY_F5 : npKeyEvent (kKeyCodeF5, kKeyUp); break;
				case GLUT_KEY_F6 : npKeyEvent (kKeyCodeF6, kKeyUp); break;
				case GLUT_KEY_F7 : npKeyEvent (kKeyCodeF7, kKeyUp); break;
				case GLUT_KEY_F8 : npKeyEvent (kKeyCodeF8, kKeyUp); break;
//				case GLUT_KEY_F9 : npKeyEvent (kKeyCodeF9, kKeyUp); break;
//				case GLUT_KEY_F10 : npKeyEvent (kKeyCodeF10, kKeyUp); break;
//				case GLUT_KEY_F11 : npKeyEvent (kKeyCodeF11, kKeyUp); break;
//				case GLUT_KEY_F12 : npKeyEvent (kKeyCodeF12, kKeyUp); break;
				case GLUT_KEY_LEFT : npKeyEvent (kKeyCodeLeft, kKeyUp); break;
				case GLUT_KEY_RIGHT : npKeyEvent (kKeyCodeRight, kKeyUp); break;
				case GLUT_KEY_UP : npKeyEvent (kKeyCodeUp, kKeyUp); break;
				case GLUT_KEY_DOWN : npKeyEvent (kKeyCodeDown, kKeyUp); break;
				case GLUT_KEY_PAGE_UP : npKeyEvent (kKeyCodePageUp, kKeyUp); break;
				case GLUT_KEY_PAGE_DOWN : npKeyEvent (kKeyCodePageDown, kKeyUp); break;
				case GLUT_KEY_HOME : npKeyEvent (kKeyCodeHome, kKeyUp); break;
				case GLUT_KEY_END : npKeyEvent (kKeyCodeEnd, kKeyUp); break;
//				case GLUT_KEY_INSERT : npKeyEvent (kKeyCode, kKeyUp); break;	// not compatible with Mac
				default : break;
			}
			break;
	}
}
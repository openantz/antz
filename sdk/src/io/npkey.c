/* -----------------------------------------------------------------------------
*
*  npkey.c
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

#include "npkey.h"

#include "../npdata.h"
#include "npconsole.h"

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

/*------------------------------------------------------------------------------
* --- KEY ARCHITECTURE PLAN ---
*
* process both the OS keyboard API (low-latency) and HID devices (slower)
* HID allows support for multiple independent devices which are then
* mapped to the active context(s) as either ASCII text or RAW keys.
*
* uses include action keys, quick keys, typing and editing with dynamic-suggest
*
* allow for multiple keyboards to have independent target contexts to allow
* a multi-user(s) to have separate consoles and/or other active contexts.
*
* event driven model
* first sets modifiers states
* then converts API key code to ASCII or RAW
* next call GUI/\console event handlers that are registered for the device(s)
*
* startup...
* console->mode = kNPconsoleCmd; //default keyboard context is rootHUD
* keyboard[i]->context = rootHUD //root HUD console
*
* active window is called (currently handled by GLUT)
*   keyboard (and mouse) updates based on active context
*		key/mouse (KM) pair assigned together with unique selection set
*			sub-selection / context within the window
*				ie. control two separate consoles
*
* focus within the window is directed to the devices active context
*  rootHUD console and other node consoles... anywhere in tree
*
* event handler(s) called in order until event is handled
*   first handler could be ASCII Viewer user input while next is 3D Viewer, etc
*	   allow for multiple simultaneous methods of user feedback, DB tree viewer
*
* currently supports a standard US English QWERTY keyboard
* allow user re-mapping for non-QWERTY layout and non-English keyboards
* API keyboard support for ASCII, GLUT, OSX, MSW, Linux, HID
* the two fundamental types are ASCII (typewriter style) and Game Mode	
* gamemode = kNPkeyRAW or edit interface uses kNPkeyASCII + special / modifiers
*	 keyEventType = kNPkeyDown //Up Repeat
*	 keyType = QWERTY / ASCII / modifier / special
*	 other HID devices, languages...? 
*	 keyRepeatRate = 15
*	 optionally load keymaps from CSV
*	 default translate ASCII to local ASCII-friendly key code list
*
* Cross-Platform issues include the Apple 'Command' key and how the user
* expects the ctrl, alt and command key to behave, such as copy paste...
* also, ALT key only generates an event when another key gets pressed
* releasing SHIFT key stops a key repeat, pressing it down does not...
* so... CapsLock, Shift, Alt and Command are all a little different!
*
*----------------------------------------------------------------------------- */

	
struct NPkeyMap
{
	int		id;
	int		type;
	char*	name;
	char*	note;
};
typedef struct NPkeyMap NPkeyMap;
typedef struct NPkeyMap *NPkeyMapPtr;


//arrays of hard coded keyboard mappings for US English QWERTY
//common types are ASCII, HID, Linux, MSW, OSX, GLUT
//------------------------------------------------------------------------------
void npInitKeyMap (void* dataRef)
{
	int i = 0;
	pData data = (pData) dataRef;

//	static NPkeyMap base[] = {

//	{ kNPkeyASCII_a, kNPkeyHID, kNPkeyLinux, kNPkeyMSW, kNPkeyOSX, kNPkeyGLUT},
//	{ kNPkeyASCII_b, kNPkeyHID, kNPkeyLinux, kNPkeyMSW, kNPkeyOSX, kNPkeyGLUT},
//	{ kNPkeyASCII_c, kNPkeyHID, kNPkeyLinux, kNPkeyMSW, kNPkeyOSX, kNPkeyGLUT}
//	};

}

//Game Mode executes dynamically mapped commands for RAW key events
//the default preset for game mode control commands is hard-coded here
//three types of fundamental keys: toggle, momentary, repeating
//and the odd-ball caps-lock which only sends (trigger) key down events 
//------------------------------------------------------------------------------
void npInitKey (void* dataRef)
{
	int i = 0;
	int j = 0;

	pData data = (pData) dataRef;
	pNPkey key = &data->io.key;

	//set all to zero
	key->modAlphaUpper = 0;

	key->modCapsLock = 0;

	key->modShift = 0;
	key->modCtrl = 0;
	key->modAlt = 0;
	key->modCommand = 0;

	key->modShiftLeft = 0;
	key->modShiftRight = 0;
	key->modCtrlLeft = 0;
	key->modCtrlRight = 0;
	key->modAltLeft = 0;
	key->modAltRight = 0;
	key->modCommandLeft = 0;
	key->modCommandRight = 0;

	key->skipCount = 0;

	for (i = 0; i < kNPkeyEventTypeSize; i++)
		for (j = 0; j < kNPkeyMapSize; j++)
			key->map[i][j] = 0;


	//Fire Commands only respond to key down event
	key->map[kKeyDown][kKeyCodeM]				= kNPcmdMenu;

	key->map[kKeyDown][kKeyCodeBackSlash]		= kNPcmdSubsample;			//zzhp

	key->map[kKeyDown][kKeyCodeY]				= kNPcmdSegments;
	key->map[kKeyDown][kKeyCodeV]				= kNPcmdTexture;

	key->map[kKeyDown][kKeyCodeESC]				= kNPcmdFullscreen;
//	key->map[kKeyDown][kKeyCodeB]				= kNPcmdClearFrameBuffer;
	key->map[kKeyDown][kKeyCodeB]				= kNPcmdBackground;

	key->map[kKeyDown][kKeyCodeT]				= kNPcmdTool;
	key->map[kKeyDown][kKeyCodeX]				= kNPcmdSelectAxes;

	key->map[kKeyDown][kKeyCodeC]				= kNPcmdCamera;
	key->map[kKeyDown][kKeyCodeG]				= kNPcmdGrid;

	key->map[kKeyDown][kKeyCode4]				= kNPcmdSelectAll;
	key->map[kKeyDown][kKeyCodeTilda]			= kNPcmdSelectNone;
//	key->map[kKeyDown][kKeyCodeSlash]			= kNPcmdSelectToggle; //zzsql
	key->map[kKeyDown][kKeyCode1]				= kNPcmdFileMapOne;
	key->map[kKeyDown][kKeyCode2]				= kNPcmdFileMapTwo;
	key->map[kKeyDown][kKeyCode3]				= kNPcmdFileMapThree;

	key->map[kKeyDown][kKeyCode5]				= kNPcmdPresetOne;
	key->map[kKeyDown][kKeyCode6]				= kNPcmdPresetTwo;
	key->map[kKeyDown][kKeyCode7]				= kNPcmdPresetThree;
//	key->map[kKeyDown][kKeyCode8]				= kNPcmdPresetFour;

//	key->map[kKeyDown][kKeyCodeC]				= kNPcmdCenter;
//	key->map[kKeyDown][kKeyCodeBackSlash]		= kNPcmdScroll;
	key->map[kKeyDown][kKeyCodeF]				= kNPcmdFreeze;
	key->map[kKeyDown][kKeyCodeH]				= kNPcmdHide;

	key->map[kKeyDown][kKeyCodeO]				= kNPcmdGeometry;
	key->map[kKeyDown][kKeyCodeJ]				= kNPcmdTopo;

	key->map[kKeyDown][kKeyCodeI]				= kNPcmdTagMode;
	
	key->map[kKeyDown][kKeyCodeK]				= kNPcmdFileSave;
	key->map[kKeyDown][kKeyCodeL]				= kNPcmdFileOpen;
	key->map[kKeyDown][kKeyCodeP]				= kNPcmdFileImport;	// kNPcmdPoints;

	key->map[kKeyDown][kKeyCodeSlash]			= kNPcmdViewer;

	key->map[kKeyDown][kKeyCodeU]				= kNPcmdOpenURL;


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
/*
	key->map[kKeyDown][kKeyCodeRightQuote]		= kNPcmdNextBranch;
	key->map[kKeyRepeat][kKeyCodeRightQuote]	= kNPcmdNextBranch;
	key->map[kKeyDown][kKeyCodeSemiColon]		= kNPcmdPrevBranch;
	key->map[kKeyRepeat][kKeyCodeSemiColon]		= kNPcmdPrevBranch;
*/	
	key->map[kKeyDown][kKeyCodeReturn]			= kNPcmdConsole;
	key->map[kKeyRepeat][kKeyCodeReturn]		= kNPcmdConsole;
	key->map[kKeyDown][kKeyCodeNumPadEnter]		= kNPcmdConsole;
	key->map[kKeyRepeat][kKeyCodeNumPadEnter]	= kNPcmdConsole;

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

	// 
	key->map[kKeyDown][kKeyCodeLeft]			= kNPcmdPrev;
	key->map[kKeyRepeat][kKeyCodeLeft]			= kNPcmdPrev;
	key->map[kKeyUp][kKeyCodeLeft]				= kNPcmdPrevOff;

	key->map[kKeyDown][kKeyCodeRight]			= kNPcmdNext;
	key->map[kKeyRepeat][kKeyCodeRight]			= kNPcmdNext;
	key->map[kKeyUp][kKeyCodeRight]				= kNPcmdNextOff;
	
	key->map[kKeyDown][kKeyCodeDown]			= kNPcmdPrevBranch;
	key->map[kKeyRepeat][kKeyCodeDown]			= kNPcmdPrevBranch;
	key->map[kKeyUp][kKeyCodeDown]				= kNPcmdPrevBranchOff;
	
	key->map[kKeyDown][kKeyCodeUp]				= kNPcmdNextBranch;
	key->map[kKeyRepeat][kKeyCodeUp]			= kNPcmdNextBranch;
	key->map[kKeyUp][kKeyCodeUp]				= kNPcmdNextBranchOff;
	
	// rotate
	//key->map[kKeyDown][kKeyCodeLeft]			= kNPcmdRotateLeft;
	//key->map[kKeyUp][kKeyCodeLeft]			= kNPcmdRotateLeftOff;

	//key->map[kKeyDown][kKeyCodeRight]			= kNPcmdRotateRight;
	//key->map[kKeyUp][kKeyCodeRight]			= kNPcmdRotateRightOff;

	//key->map[kKeyDown][kKeyCodeDown]			= kNPcmdRotateDown;
	//key->map[kKeyUp][kKeyCodeDown]			= kNPcmdRotateDownOff;

	//key->map[kKeyDown][kKeyCodeUp]			= kNPcmdRotateUp;
	//key->map[kKeyUp][kKeyCodeUp]				= kNPcmdRotateUpOff;

	//key->map[kKeyDown][kKeyCodeLeft]			= kNPcmdRotateCCW;
	//key->map[kKeyUp][kKeyCodeLeft]			= kNPcmdRotateCCWOff;

	//key->map[kKeyDown][kKeyCodeRight]			= kNPcmdRotateCW;
	//key->map[kKeyUp][kKeyCodeRight]			= kNPcmdRotateCWOff;

	// scale
	key->map[kKeyDown][kKeyCodeZ]				= kNPcmdZoomOn;
	key->map[kKeyUp][kKeyCodeZ]					= kNPcmdZoomOff;

	key->map[kKeyDown][kKeyCodeF4]				= kNPcmdScreenGrab;
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

	if (key == -1)	//unknown key type
		return;

	if ( data->io.gl.hud.console.mode > 0 )
		npConsoleKeyEvent (key, keyEventType, data);
	else
	{
		command = data->io.key.map[keyEventType][key];

		if (command != 0)
			npCtrlCommand (command, data);
	}
}
/*
enum
{
	kNPkeyNULL = 0,
	kNPkeyDown,
	kNPkeyUp,
	kNPkeyRepeat,
	kNPkeyDownSpecial,
	kNPkeyUpSpecial,
	kNPkeyRepeatSpecial
};
*/
enum
{
	kNPkeyCodeDefault = 0,
	kNPkeyCodeASCII,
	kNPkeyCodeGLUTspecial,
	kNPkeyCodeMSW,
	kNPkeyCodeOSX,
	kNPkeyCodeLinux,
	kNPkeyCodeHID
};

int npKeyRAW (int key, int keyCodeType);
//zz debug replace with keymap array
// Converts GLUT keycode to native keycode
//returns the converted key value constant or -1 if unknown input
//------------------------------------------------------------------------------
int npKeyRAW (int convertKey, int keyCodeType)
{
	int key = -1;	// -1 for unknown key

//	printf("%d   %d \n", convertKey, keyCodeType);

	switch (keyCodeType) {
		case kNPkeyCodeASCII :
			switch (convertKey) {
				case 8  : key = kKeyCodeBackSpace; break;
				case 9  : key = kKeyCodeTab; break;
				case 13 : key = kKeyCodeReturn; break;

				//GLUT re-uses ASCII characters 16, 17, 18 and 20
				//if context active then key up event when both L and R released
				// case 16 : key = kKeyCodeShift; break;
				// case 17 : key = kKeyCodeControl; break;
				// case 18 : key = kKeyCodeAlt; break;
				// case 20 : key = kKeyCodeCapsLock; break; //key down event only

				case 27 : key = kKeyCodeESC; break;
				case 32 : key = kKeyCodeSpace; break;
				case 33 : key = kKeyCode1; break;
				case 34 : key = kKeyCodeRightQuote; break;  // double "
				case 35 : key = kKeyCode3; break;
				case 36 : key = kKeyCode4; break;
				case 37 : key = kKeyCode5; break;
				case 38 : key = kKeyCode7; break;
				case 39 : key = kKeyCodeRightQuote; break;  // single '
				case 40 : key = kKeyCode9; break;
				case 41 : key = kKeyCodeZero; break;
				case 42 : key = kKeyCode8; break;
				case 43 : key = kKeyCodeEqual; break;
				case 44 : key = kKeyCodeComma; break;
				case 45 : key = kKeyCodeMinus; break;
				case 46 : key = kKeyCodePeriod; break;	
				case 47 : key = kKeyCodeSlash; break;
				case 48 : key = kKeyCodeZero; break;
				case 49 : key = kKeyCode1; break;
				case 50 : key = kKeyCode2; break;
				case 51 : key = kKeyCode3; break;
				case 52 : key = kKeyCode4; break;
				case 53 : key = kKeyCode5; break;
				case 54 : key = kKeyCode6; break;
				case 55 : key = kKeyCode7; break;
				case 56 : key = kKeyCode8; break;
				case 57 : key = kKeyCode9; break;
				case 58 : key = kKeyCodeSemiColon; break;
				case 59 : key = kKeyCodeSemiColon; break;
				case 60 : key = kKeyCodeComma; break;
				case 61 : key = kKeyCodeEqual; break;
				case 62 : key = kKeyCodePeriod; break;	
				case 63 : key = kKeyCodeSlash; break;
				case 64 : key = kKeyCode2; break;
				case 65 : key = kKeyCodeA; break;
				case 66 : key = kKeyCodeB; break;
				case 67 : key = kKeyCodeC; break;
				case 68 : key = kKeyCodeD; break;
				case 69 : key = kKeyCodeE; break;
				case 70 : key = kKeyCodeF; break;
				case 71 : key = kKeyCodeG; break;
				case 72 : key = kKeyCodeH; break;
				case 73 : key = kKeyCodeI; break;
				case 74 : key = kKeyCodeJ; break;
				case 75 : key = kKeyCodeK; break;
				case 76 : key = kKeyCodeL; break;
				case 77 : key = kKeyCodeM; break;
				case 78 : key = kKeyCodeN; break;
				case 79 : key = kKeyCodeO; break;
				case 80 : key = kKeyCodeP; break;
				case 81 : key = kKeyCodeQ; break;
				case 82 : key = kKeyCodeR; break;
				case 83 : key = kKeyCodeS; break;
				case 84 : key = kKeyCodeT; break;
				case 85 : key = kKeyCodeU; break;
				case 86 : key = kKeyCodeV; break;
				case 87 : key = kKeyCodeW; break;
				case 88 : key = kKeyCodeX; break;
				case 89 : key = kKeyCodeY; break;
				case 90 : key = kKeyCodeZ; break;
				case 91 : key = kKeyCodeLeftBracket; break;
				case 92 : key = kKeyCodeBackSlash; break;
				case 93 : key = kKeyCodeRightBracket; break;
				case 94 : key = kKeyCode6; break;	
				case 95 : key = kKeyCodeMinus; break;
				case 96 : key = kKeyCodeTilda; break;
				case 97 : key = kKeyCodeA; break;
				case 98 : key = kKeyCodeB; break;
				case 99 : key = kKeyCodeC; break;
				case 100 : key = kKeyCodeD; break;
				case 101 : key = kKeyCodeE; break;
				case 102 : key = kKeyCodeF; break;
				case 103 : key = kKeyCodeG; break;
				case 104 : key = kKeyCodeH; break;
				case 105 : key = kKeyCodeI; break;
				case 106 : key = kKeyCodeJ; break;
				case 107 : key = kKeyCodeK; break;
				case 108 : key = kKeyCodeL; break;
				case 109 : key = kKeyCodeM; break;
				case 110 : key = kKeyCodeN; break;
				case 111 : key = kKeyCodeO; break;
				case 112 : key = kKeyCodeP; break;
				case 113 : key = kKeyCodeQ; break;
				case 114 : key = kKeyCodeR; break;
				case 115 : key = kKeyCodeS; break;
				case 116 : key = kKeyCodeT; break;
				case 117 : key = kKeyCodeU; break;
				case 118 : key = kKeyCodeV; break;
				case 119 : key = kKeyCodeW; break;
				case 120 : key = kKeyCodeX; break;
				case 121 : key = kKeyCodeY; break;
				case 122 : key = kKeyCodeZ; break;
				case 123 : key = kKeyCodeLeftBracket; break;
				case 124 : key = kKeyCodeBackSlash; break;
				case 125 : key = kKeyCodeRightBracket; break;
				case 126 : key = kKeyCodeTilda; break;
				case 127 : key = kKeyCodeDelete; break;
				default : break;
			}
			break;
	
		//non-ASCII Glut key constants
		case kNPkeyCodeGLUTspecial :
			switch (convertKey) {
				case GLUT_KEY_F1 : key = kKeyCodeF1; break;
				case GLUT_KEY_F2 : key = kKeyCodeF2; break;
				case GLUT_KEY_F3 : key = kKeyCodeF3; break;
				case GLUT_KEY_F4 : key = kKeyCodeF4; break;
				case GLUT_KEY_F5 : key = kKeyCodeF5; break;
				case GLUT_KEY_F6 : key = kKeyCodeF6; break;
				case GLUT_KEY_F7 : key = kKeyCodeF7; break;
				case GLUT_KEY_F8 : key = kKeyCodeF8; break;
				// case GLUT_KEY_F9 : key = kKeyCodeF9; break;		//in use by OSX
				// case GLUT_KEY_F10 : key = kKeyCodeF10; break;	//in use by OSX
				// case GLUT_KEY_F11 : key = kKeyCodeF11; break;	//in use by OSX
				// case GLUT_KEY_F12 : key = kKeyCodeF12; break;	//in use by OSX
				case GLUT_KEY_LEFT : key = kKeyCodeLeft; break;
				case GLUT_KEY_RIGHT : key = kKeyCodeRight; break;
				case GLUT_KEY_UP : key = kKeyCodeUp; break;
				case GLUT_KEY_DOWN : key = kKeyCodeDown; break;
				case GLUT_KEY_PAGE_UP : key = kKeyCodePageUp; break;
				case GLUT_KEY_PAGE_DOWN : key = kKeyCodePageDown; break;
				case GLUT_KEY_HOME : key = kKeyCodeHome; break;
				case GLUT_KEY_END : key = kKeyCodeEnd; break;
				// case GLUT_KEY_INSERT : key = kKeyCode; break;	//incompatible with OSX
				// case 112 : key = kKeyCodeShiftLeft; break;		//handled by npKeyGlut()
				// case 113 : key = kKeyCodeShiftRight; break;
				// case 114 : key = kKeyCodeControlLeft; break;
				// case 115 : key = kKeyCodeControlRight; break;
				// case 116 : key = kKeyCodeAltLeft; break;
				// case 117 : key = kKeyCodeAltRight; break;
				default : break;
			}
			break;
		case kNPkeyCodeMSW : break;
		case kNPkeyCodeOSX : break;
		case kNPkeyCodeLinux : break;
		default : break;
	}

	//return converted keycode
	return key;
}

enum {
	kKeyCodeModifiers = 1024,
	kKeyCodeCapsLock,
	kKeyCodeShiftLeft,
	kKeyCodeShiftRight,
	kKeyCodeControlLeft,
	kKeyCodeControlRight,
	kKeyCodeAltLeft,
	kKeyCodeAltRight
};

enum {
	kNPkeyModeRAW = 0,
	kNPkeyModeASCII
};
//------------------------------------------------------------------------------
//Process key events from GLUT, both ASCII and 'Special' keys and modifiers
//note that freeglut supports shift, alt, and ctrl but not the OSX 'command' key
//
//there are two distinct fundamental keyboard modes of operation
//either ASCII text mode 'typewriter style' or raw key events 'game mode'
//
//game mode treats every key as a RAW switch, no difference between 'a' and 'A'
//caps lock key is unique in that it only generates key down events
//all other keys have up and down events as well as L vs R modifier distinction
//note that modifier key behavior varies depending on the OS and library
//
//freeglut mixes a modifier key up event as an ASCII event 16, 17 or 18
//it generates 'Special' key up and down events 112-117 for both L/R modifiers
//then follows with an additional ASCII key up, 16(Shift), 17(Ctrl) and 18(Alt)
//
//key handling is often confusing, inconsistent and un-documented across OS APIs
//try printing raw event values and testing all keys when implementing a new API
//------------------------------------------------------------------------------
void npKeyGlut (int key, int x, int y, int keyTypeGlut, int modifiers)
{
	int keyMode = 0;

	pData data = (pData) npGetDataRef();

	pNPkey keyboard = &data->io.key;

// Shift modifier doesn't work, on the mac, without using glutGetModifiers, lde
#ifdef NP_OSX_
	if (modifiers == GLUT_ACTIVE_SHIFT)		//no longer using this method
	{
		data->io.key.modShift = true;
		keyboard->modShiftLeft = true;
		keyboard->modShiftRight = true;
	}
	else
	{
		data->io.key.modShift = false;
		keyboard->modShiftLeft = false;
		keyboard->modShiftRight = false;
	}
#endif

/*
	if (modifiers == GLUT_ACTIVE_CTRL)
		data->io.key.modCtrl = true;
	else
		data->io.key.modCtrl = false;
	if (modifiers == GLUT_ACTIVE_ALT)
		data->io.key.modAlt = true;
	else
		data->io.key.modAlt = false;
*/
	//first catch and calculate the modifier key states
	//zz debug, far as i know... this is un-documented freeglut behavior
	//needs testing, may not work with Apple GLUT or other OS
	//typically would call glutGetModifiers() instead of this method
	//this will take care of returning the states in case of a missed key up
	switch (keyTypeGlut) {
		case kGlutKeyUp :		//note that these are ONLY key up events
			switch (key) {
				case 16 :		//both SHIFT keys released
					keyboard->modShift = false; 
					keyboard->modShiftLeft = false; 
					keyboard->modShiftRight = false; 
					break;
				case 17 :		//both CTRL keys released
					keyboard->modCtrl = false; 
					keyboard->modCtrlLeft = false; 
					keyboard->modCtrlRight = false; 
					break;
				case 18 :		//both ALT keys released			
					keyboard->modAlt = false; 
					keyboard->modAltLeft = false; 
					keyboard->modAltRight = false; 
					break;
								//initial caps states needs to be detected	//zz debug
				case 20 :					//toggle stored caps state
					keyboard->modCapsLock = 1 - keyboard->modCapsLock;
					break;
				default : break;
			}
			break;
		case kGlutKeyDownSpecial:		// GLUT key codes for non-ASCII keys
			switch (key) {
				case 112 : keyboard->modShiftLeft = true; break;
				case 113 : keyboard->modShiftRight = true; break;
				case 114 : keyboard->modCtrlLeft = true; break;
				case 115 : keyboard->modCtrlRight = true; break;
				case 116 : keyboard->modAltLeft = true; break;
				case 117 : keyboard->modAltRight = true; break;
				default : break;
			}
			break;
		case kGlutKeyUpSpecial:			// non-ASCII Glut constants
			switch (key) {
				case 112 : keyboard->modShiftLeft = false; break;
				case 113 : keyboard->modShiftRight = false; break;
				case 114 : keyboard->modCtrlLeft = false; break;
				case 115 : keyboard->modCtrlRight = false; break;
				case 116 : keyboard->modAltLeft = false; break;
				case 117 : keyboard->modAltRight = false; break;
				default : break;
			}
			break;
		default : break;
	}

	//calculate sum states
	if (keyboard->modShiftLeft || keyboard->modShiftRight)
		keyboard->modShift = true;
	else
		keyboard->modShift = false;

	if (keyboard->modAltLeft || keyboard->modAltRight)
		keyboard->modAlt = true;
	else
		keyboard->modAlt = false;

	if (keyboard->modCtrlLeft || keyboard->modCtrlRight)
		keyboard->modCtrl = true;
	else
		keyboard->modCtrl = false;
	
	if (keyboard->modShift || keyboard->modCapsLock)
	{
		if (keyboard->modShift && keyboard->modCapsLock)
			keyboard->modAlphaUpper = false;
		else
			keyboard->modAlphaUpper = true;
	}
	else
		keyboard->modAlphaUpper = false;

	//printf("caps: %d   shift: %d  upper: %d   alt: %d   ctrl: %d \n", 
	//	keyboard->modCapsLock, keyboard->modShift, keyboard->modAlphaUpper,
	//	keyboard->modAlt, keyboard->modCtrl);

	//set the key process mode, either raw keys or ASCII typewriter style
	if( data->io.gl.hud.console.mode > 0 )
		keyMode = kNPkeyModeASCII;
	else
		keyMode = kNPkeyModeRAW;
	
	//call the relevant key handler based on the target mode
	switch (keyMode) {
		case kNPkeyModeRAW :
			switch (keyTypeGlut) {
				case kGlutKeyDown:			// ASCII key codes converted to native
					npKeyEvent (npKeyRAW(key, kNPkeyCodeASCII), kKeyDown);
					break;
				case kGlutKeyUp:			// ASCII key codes converted
					npKeyEvent (npKeyRAW(key, kNPkeyCodeASCII), kKeyUp);
					break;
				case kGlutKeyDownSpecial:	// GLUT key codes for non-ASCII keys
					npKeyEvent (npKeyRAW(key, kNPkeyCodeGLUTspecial), kKeyDown);
					break;
				case kGlutKeyUpSpecial:		// non-ASCII Glut constants
					npKeyEvent (npKeyRAW(key, kNPkeyCodeGLUTspecial), kKeyUp);
					break;
				default : break;
			}
			break;
		case kNPkeyModeASCII :				//send raw ASCII text to console to process
			switch (keyTypeGlut) {
				case kGlutKeyDown: npConsoleKeyEvent (key, kKeyDownASCII, data);
					break;
				case kGlutKeyDownSpecial:	// non-ASCII Glut constants
					switch (key) {
						case GLUT_KEY_LEFT :
							npConsoleKeyEvent (kKeyCodeLeft, kKeyDownSpecial, data);
							break;
						case GLUT_KEY_RIGHT :
							npConsoleKeyEvent (kKeyCodeRight, kKeyDownSpecial, data);
							break;
						case GLUT_KEY_UP :
							npConsoleKeyEvent (kKeyCodeUp, kKeyDownSpecial, data);
							break;
						case GLUT_KEY_DOWN :
							npConsoleKeyEvent (kKeyCodeDown, kKeyDownSpecial, data);
							break;
						default : break;
					}
					break;
				default : break;
			}
		default : break;
	}
}


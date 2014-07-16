/* -----------------------------------------------------------------------------
*
*  npconsole.h
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

#ifndef NPCONSOLE_H_
#define NPCONSOLE_H_

#include "../npdata.h"

//manages console structure, formatting and user interaction
//console is drawn separately in npgldraw.h using the console structure
//
//supports multiple consoles of various types
//standard ASCII terminals, GUI based and hybrid
//hybrid consoles have simultaneous ASCII command prompt and 3D GUI
//3D object tree responds to command prompt typing akin to auto-complete
//user selection via mouse, key or other HID device
//user activity parametizes choice suggestions
//support for multiple users

void npInitConsole (void* dataRef);
void npCloseConsole (void* dataRef);

void npUpdateConsole (void* dataRef);	//global once per cycle update

void npUpdateConsoleUserText(pNPconsole console, void* dataRef);
void npUpdateConsoleMenu (pNPconsole console, void* dataRef);

void npConsoleKeyEvent (int key, int keyEventType, void* dataRef);

void npConsoleCLS (pNPconsole console, void* dataRef);

void npConsoleMenu (void (*pMenuCallback)(int menuItem, void* dataRef), 
					pNPconsole console, void* dataRef);

void npMainArgcArgvHandler( int argc, char** argv, void* dataRef );

void npSystemConsoleHelp (int argc, char** argv);

void npConsoleCmd( pNPconsole console, void* dataRef );

void npConsoleCmdText( pNPconsole console, void* dataRef );

void npConsolePrompt(pNPconsole console, void* dataRef);

#endif

